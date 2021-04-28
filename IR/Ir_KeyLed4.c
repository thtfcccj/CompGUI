/***************************************************************************

      红外接收程序之对码-在4个键值及4位数码管应用时的实现

***************************************************************************/
#ifdef SUPPORT_IR_KEY_MATCH    //支持对码时

#include "Ir_KeyLed4.h"
#include "IrDrv.h"

#include "InfoBase.h"
#include "Eeprom.h"

#include "Led.h"
#include "LedSign.h"

#include <string.h>

struct _Ir_KeyLed4 Ir_KeyLed4; 

struct _IrDrvInfo cbIrDrvInfo;//实现为变量

//-----------------------------初始化函数-----------------------------
void Ir_KeyLed4_Init(signed char Inited)
{
  memset(&Ir_KeyLed4, 0, sizeof(Ir_KeyLed4));
  //初始化cbIrDrvInfo:
  if(!Inited){
    memcpy(&cbIrDrvInfo, &cbIrDrvInfoDefault, sizeof(struct _IrDrvInfo));
    Eeprom_Wr(IrDev_GetInfoBase(),
              &cbIrDrvInfo,
              sizeof(struct _IrDrvInfo));    
  }
  else{
    Eeprom_Rd(IrDev_GetInfoBase(),
              &cbIrDrvInfo,
              sizeof(struct _IrDrvInfo));
  }
  
}

//各键对码时，显示符号定义
static const unsigned char _DispLut[][4] = {
  {0,          LED_SIGN_P, LED_SIGN_U            },   //UP
  {LED_SIGN_N, LED_SIGN_U, LED_SIGN_O, LED_SIGN_D},   //DOWN 
  {LED_SIGN_R, LED_SIGN_T, LED_SIGN_N, LED_SIGN_E},   //ENTER
  {LED_SIGN_T, LED_SIGN_I, LED_SIGN_U, LED_SIGN_Q},   //QUIT
  {LED_SIGN_E, LED_SIGN_U, LED_SIGN_A, LED_SIGN_S},   //SAVE
};

//------------------------更新界面显示函数-----------------------------
static void _UpDateDisp(void)
{
  if(!Ir_KeyLed4.IdentState) return;  //没在识别状态
  unsigned char Pos;
  //if(Ir_KeyLed4.IdentState > Ir_KeyLed4_KEY_COUNT) Pos = 4; //保存状态中
  //else Pos = Ir_KeyLed4.IdentState - 1;
  Pos = Ir_KeyLed4.IdentState - 1;
  memcpy(Led.SegDisp, _DispLut[Pos], 4);
  Led.SegFlash = 0x0f; //闪动显示
}

//------------------------------滤波处理函数-------------------------------
//防止干扰信号影响识别,返回1:异常信号
unsigned char _IrFilter(void)
{
  if(IrDrv.CodeIdent.Count < 8) return 1; //太少了
  
  //检查无信号时的脉冲宽度掩码是否全为0或全为1
  unsigned char IdieCount = 0;
  for(unsigned char i = 0; i < IrDrv.CodeIdent.Count; i++){
    if(IrDrv.CodeIdent.IdieWidthMask[i >> 3] & (1 << (i & 0x07)))
      IdieCount++;
  }
  unsigned char WidthCount = 0;
  for(unsigned char i = 0; i < IrDrv.CodeIdent.Count; i++){
    if(IrDrv.CodeIdent.IrWidthMask[i >> 3] & (1 << (i & 0x07)))
      WidthCount++;
  }
  //空闲周期为0或全了
  if((!IdieCount) || (IdieCount >= IrDrv.CodeIdent.Count)){
    //IR周期不能全相同
    if((!WidthCount) || (WidthCount >= IrDrv.CodeIdent.Count))
      return 1;
  } 
  //信号周期为0或全了 
  if((!WidthCount) || (WidthCount >= IrDrv.CodeIdent.Count)){
    //空闲IR周期不能全相同
    if((!IdieCount) || (IdieCount >= IrDrv.CodeIdent.Count))
      return 1;
  }
  return 0; //检测正确
}
//-------------------------------任务函数-------------------------------
//放入256mS进程中扫描
void Ir_KeyLed4_Task(void)
{
  //==============按键进入识别模式单独处理===================
  if(!Ir_KeyLed4.IdentState){
    if(Ir_KeyLed4.KeyCount >= IR_KEY_LED4_IDENT_ENTER_COUNT){//次数到了
      //进入识别模式初始化
      Ir_KeyLed4.IdentState = 1;
      memset(&cbIrDrvInfo, 0, sizeof(struct _IrDrvInfo)); //清除原有的信息
      Led_Init();//重新初始化显示
      _UpDateDisp();//手动更新显示
      Ir_KeyLed4.Timer = IR_KEY_LED4_IDENT_EXIT_COUNT; //启动首次计时
      Ir_KeyLed4.Flag &= ~IR_KEY_LED4_REPEAT; 
    }
    return;
  }
  
  //===============退出识别中超时退出处理=======================
  if(Ir_KeyLed4.Timer){
    Ir_KeyLed4.Timer--;
    if(Ir_KeyLed4.Timer) return;//时间未到
  }
  //超时,退出识别模式
   Eeprom_Rd(IrDev_GetInfoBase(),&cbIrDrvInfo, sizeof(struct _IrDrvInfo));//恢复码值
   Ir_KeyLed4.IdentState = 0;//
   Ir_KeyLed4.KeyCount = 0; //
   Ir_KeyLed4_cbLedInit();  //恢复KEY_LED4现场
   
   //while(1);//Reset(); //复位重启       
   return; //为防止有规律的红外信号进入,改为返回
}

//-----------------------------按键处理函数---------------------------
//在识别状态调用此函数处理按键
void Ir_KeyLed4_Key(unsigned char Key)
{
  if(Ir_KeyLed4.IdentState > IR_KEY_LED4_KEY_COUNT){ //保存状态中
    if(Key & 0x04){//定义为确认键了
      //保存码值
      Eeprom_Wr(IrDev_GetInfoBase(), &cbIrDrvInfo, sizeof(struct _IrDrvInfo));       
      //void(*Reset)(void) = 0;
      while(1);//Reset(); //复位重启    
    }
  }
}

/***************************************************************************
                          IrDrv相关回调函数实现
***************************************************************************/

//---------------------------新码值通报函数--------------------------------
//识别到对码不对的键值时,通报新键值
//可用于用户进入对码状态或保存键值
void IrDrv_cbNewKeyNotify(void)
{
  //判断进入识别状态
  if(!Ir_KeyLed4.IdentState){
   if(_IrFilter())//异常退出
      Ir_KeyLed4.KeyCount = 0; //重新开始
    else Ir_KeyLed4.KeyCount++;
    return;
  }
  //用户保存键确认
  if(Ir_KeyLed4.IdentState > IR_KEY_LED4_KEY_COUNT) return;

  //=================识别键值过程中=========================
  if(Ir_KeyLed4.Flag & IR_KEY_LED4_REPEAT){//已再次按键了
    memcpy(&cbIrDrvInfo.Code[Ir_KeyLed4.IdentState - 1],//临时更新保存
           &IrDrv.CodeIdent, 
           sizeof(struct _IrDrvCode));
    Ir_KeyLed4.IdentState++; //下个按键
    _UpDateDisp();//更新显示
    Ir_KeyLed4.Flag &= ~IR_KEY_LED4_REPEAT;
  }
  else{//仅按一次
    Ir_KeyLed4.Flag |= IR_KEY_LED4_REPEAT;
  }
  //清定时器
  Ir_KeyLed4.Timer = IR_KEY_LED4_IDENT_EXIT_COUNT; 
}

//--------------------新键值切换通报函数------------------------------
//识别到多次相同键值无效按键后,检测到不一样时通报
void IrDrv_cbNewKeySwitchNotify(void)
{
  Ir_KeyLed4.Flag &= ~IR_KEY_LED4_REPEAT;
  Ir_KeyLed4.KeyCount = 0;
}

#endif //#ifdef SUPPORT_IR_KEY_MATCH    //支持对码时
