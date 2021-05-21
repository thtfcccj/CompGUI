/***************************************************************************

                       红外接收通用驱动程序实现

***************************************************************************/
#include "IrDrv.h"
#include <string.h>

struct _IrDrv  IrDrv;

/***************************************************************************
                              相关函数实现
***************************************************************************/
//-----------------------------初始化函数-----------------------------
void IrDrv_Init(void)
{
  memset(&IrDrv, 0, sizeof(IrDrv));
  IrDrv.PrvKey = 0xff;
  IrDrv_cbHwInit();//初始化硬件
}

//-------------------------------普通任务函数-----------------------------
//放入16ms进程中扫描
void IrDrv_Task(void)
{
  if(!IrDrv.KeyKeepIndex) return;
  //连接按键计数
  IrDrv.KeyKeepIndex--;  
  if(IrDrv.PrvKey >= IR_DRV_KEY_COUNT) return; //新键值不处理按键  
  
  if(!IrDrv.KeyKeepIndex){//用户取消连续接键了
    //释放时检测短按键
    if(IrDrv.KeyHoldIndex < IR_DRV_KEY_LONG_COUNT) 
      IrDrv_cbKeyNotify((0 << 6) | IrDrv.PrvKey);
    else IrDrv_cbKeyNotify(0xff); //无按键通报
    if(IrDrv.PrvKey < IR_DRV_KEY_COUNT){//有效键时值时复位(识别无效键值切换时复位)
      IrDrv.PrvKey = 0xff;
    }
    return;
  }
  //连续按过程中,保持按计数
  IrDrv.KeyHoldIndex++;
  if(IrDrv.KeyHoldIndex == IR_DRV_KEY_LONG_COUNT)
    IrDrv_cbKeyNotify((1 << 6) | IrDrv.PrvKey);
  else if(IrDrv.KeyHoldIndex == (IR_DRV_KEY_LONG_COUNT + IR_DRV_KEY_HOLD_SPACE)){
    IrDrv_cbKeyNotify((2 << 6) | IrDrv.PrvKey);
    IrDrv.KeyHoldIndex = IR_DRV_KEY_LONG_COUNT;
  }
}

//-------------------------------快速任务函数-----------------------------
//放入快进程中扫描
void IrDrv_FastTask(void)
{
  if(IrDrv.eState != eIrDrvState_Final) return;

  //连续收到码时,强制认为是上次按键的重复码
  if(IrDrv.KeyKeepIndex){
    IrDrv.KeyKeepIndex = IR_DRV_KEEP_COUNT;    
    IrDrv.eState = eIrDrvState_Rdy; //已准备好收下个数据了
    return;
  } 
  IrDrv.KeyKeepIndex = IR_DRV_KEEP_COUNT; 
  
  //键值检测
  unsigned char Key = 0;  
  for(; Key < IR_DRV_KEY_COUNT; Key++){
    if(!memcmp(&IrDrv.Buf, &cbIrDrvInfo.Code[Key], sizeof(struct _IrDrvCode)))//相同了
      break;
  }
  //按键计数
  if(Key != IrDrv.PrvKey){//两次按键不同,初始化
    IrDrv.PrvKey = Key;
    IrDrv.KeyHoldIndex = 0;
    if(Key >= IR_DRV_KEY_COUNT){//记住新键值用于对码
      if(IrDrv.PrvKey == IR_DRV_KEY_COUNT)//新键值切换通报
        IrDrv_cbNewKeySwitchNotify();       
      memcpy(&IrDrv.CodeIdent, &IrDrv.Buf, sizeof(struct _IrDrvCode));
    }
  }
  else if(Key >= IR_DRV_KEY_COUNT){//两次按键相同了,检查新键值是否相同
    if(!memcmp(&IrDrv.CodeIdent, &IrDrv.Buf, sizeof(struct _IrDrvCode))){
      IrDrv_cbNewKeyNotify(); //新键值通报
    }
    else{ //无效键值切换时复位
      IrDrv.PrvKey = 0xff;
    }
  }
  
  IrDrv.eState = eIrDrvState_Rdy; //已准备好收下个数据了
  memset(&IrDrv.Buf, 0, sizeof(IrDrv.Buf));//部分MCU中断里无效,提前清
}

//------------------------脉宽检测函数-----------------------------
//与标准宽度比较,获得是高电还是低电平,还是标准守宽度错误
//返回负值:标准宽度错误,正值,比标准宽度宽, 0:与标准完成相同
static signed char _GetWidthInfo(unsigned short StdWidth,  //标准宽度
                                 unsigned short Width)     //捕获的宽度
{
  if(Width < StdWidth){
    if(Width < (StdWidth - (StdWidth >> 2))) //小于3/4宽度时,认为标准宽度错误
      return -1;
    //else return 0; //3/4至1之间,认为是标准宽度
  }
  else{
    if(Width > (StdWidth + (StdWidth >> 2))) //大于5/4宽度时,认为是宽电平
      return 1;
    //else return 0; //4/4至1之间,认为是标准宽度
  }
  return 0;
}

//-----------------------------设置脉宽函数---------------------------------
//与标准宽度比较,获得是高电还是低电平,还是标准守宽度错误
//返回负值:标准宽度错误,正值,比标准宽度宽, 0:与标准完成相同
static void _SetWidthInfo(unsigned char *pWidthMask,  //脉宽掩码起始位置
                          unsigned char Pos,          //脉宽操作位置
                          signed char Result)       //_GetWidthInfo返回信息
{
  if(Result < 0){//反转信息
    //暂清全部置位,检测到时复位
    memset(pWidthMask, 0xff, IR_DRV_CODE_SIZE / 8); 
    
    //注: 若一个字里全是宽电平,则以前检测的字应是0xff但看做0
    Result = 0;
  }
  pWidthMask += Pos >> 3;
  Pos = 1 << (Pos & 0x07); 
  if(Result) *pWidthMask |= Pos;
  else *pWidthMask &= ~Pos;
}

//---------------------------------中断处理函数------------------------------
//放入捕获中断中,在有信号周期结束(即处在空闲周期)时调用此函数
//注:规定当IdieWidth>=65535时表示当前码值检测结束
void IrDrv_IRQ(unsigned short IdieWidth,  //无信号周期的脉冲宽度,>=0
               unsigned short IrWidth)   //有信号周期的脉冲宽度,>=0
{
  //信号结束了
  if(IdieWidth == 65535){ 
    if(IrDrv.eState >= eIrDrvState_SampIdieW) //有码时(部分遥控板只发一个Ir表示重复码)
       IrDrv.eState = eIrDrvState_Final; 
    else{// 准备接收码值
      IrDrv.eState = eIrDrvState_Rdy;
    }
    return;
  }
  //根据状态机处理数据
  switch(IrDrv.eState){
    case eIrDrvState_Rdy://已准备好起始,准备采集Ir宽度
      IrDrv.eState = eIrDrvState_SampIrW;
      //memset(&IrDrv.Buf, 0, sizeof(IrDrv.Buf));//PIC中断里无效,提前清
      IrDrv.eState = eIrDrvState_SampIrW;
      break;
    case eIrDrvState_SampIrW://采集Ir宽度,准备采集非Ir宽度
      IrDrv.IrWidth = IrWidth;
      IrDrv.eState = eIrDrvState_SampIdieW;
      break;
    case eIrDrvState_SampIdieW://采集非Ir宽度并开始处理数据
      IrDrv.IdieWidth = IdieWidth;
      IrDrv.eState = eIrDrvState_Doing;
      //继续break;
    case eIrDrvState_Doing:{//采集非Ir宽度并开始处理数据
      if(IrDrv.Buf.Count >= IR_DRV_CODE_SIZE) return; //超限了,后面的丢弃
      //处理空闲宽度
      signed char Result = _GetWidthInfo(IrDrv.IdieWidth, IdieWidth);
      if(Result < 0) IrDrv.IdieWidth = IdieWidth; //重新更新空闲时宽度  
      _SetWidthInfo(IrDrv.Buf.IdieWidthMask, IrDrv.Buf.Count, Result);
      //处理有信息时的宽度
      Result = _GetWidthInfo(IrDrv.IrWidth, IrWidth);
      if(Result < 0) IrDrv.IrWidth = IrWidth; //重新更新空闲时宽度
      _SetWidthInfo(IrDrv.Buf.IrWidthMask, IrDrv.Buf.Count, Result);
      IrDrv.Buf.Count++; //增加一位了
      break;
    }      
    //case eIrDrvState_Idie; //还没开始呢
    //case eIrDrvState_Final //还没完成呢
    default:break;
  }
}
