/*************************************************************************

                一键+4灯GUI程序实现

**************************************************************************/

#include "Lt4K1Gui.h"
#include <string.h>

struct _Lt4K1Gui Lt4K1Gui;

//----------------------------------初始化函数--------------------------
void Lt4K1Gui_Init(void)
{
  memset(&Lt4K1Gui, 0, sizeof(struct _Lt4K1Gui));
}

//--------------------------指示灯任务函数-------------------------
//放入256mS进程中
void Lt4K1Gui_LedTask(void)
{
  //=========================自动退出菜单处理===============================
  unsigned char ePrvState = Lt4K1Gui.eState;
  if(Lt4K1Gui.QuitIndex){
    Lt4K1Gui.QuitIndex--;
    if(!Lt4K1Gui.QuitIndex)  Lt4K1Gui_QuitMenu(); //时间到，退出菜单
  }

  if(Lt4K1Gui.eState < Lt4K1Gui_eMenuSel) return;//不在受控状态
  
  //========================当前被激活指示灯的显示更新=======================
  unsigned char IsOn; //当前是否显示
  if(!Lt4K1Gui.LtIndex){//下个周期开始了，首个周期亮
    IsOn = 1;
    if(Lt4K1Gui.LtCount) //非0时闪
       Lt4K1Gui.LtIndex = Lt4K1Gui.LtCount * 2 + ((LT4K1_GUI_LED_IDIE * 2) - 1);
    else Lt4K1Gui.LtIndex = LT4K1_GUI_LED_IDIE * 4; //0时亮双周期
  }
  else{
    Lt4K1Gui.LtIndex--;
    if(Lt4K1Gui.LtIndex >= (LT4K1_GUI_LED_IDIE * 2)){//指示周期
      if(Lt4K1Gui.LtCount){ //非0时闪
        if(!(Lt4K1Gui.LtIndex & 0x01)) IsOn = 0;//双数周期灭
        else IsOn = 1; //双周期亮
      }
      else IsOn = 1; //0时亮周期全亮
    }
    else IsOn = 0;//停显周期亮
  }
  unsigned char Mask = 1 << (Lt4K1Gui.eState - Lt4K1Gui_eMenuSel);  
  if(IsOn) Lt4K1Gui_cbOnLight(Mask);
  else  Lt4K1Gui_cbOffLight(Mask);
}

//------------------------------退出菜单函数----------------------------
void Lt4K1Gui_QuitMenu(void)
{
  if(Lt4K1Gui.eState >= Lt4K1Gui_eMenuSel)
    Lt4K1Gui_cbQuitGUI();
  Lt4K1Gui.eState = Lt4K1Gui_eIdie;
}

//-----------------------------得到十进制位数值------------------------
static const unsigned short _DecBit2Mask[] = {100,10,1};
static unsigned char _GetDecBitNum(unsigned short Num,
                                   unsigned char DecBit)//支持0,1,2
{
  unsigned char BitNum = Num / _DecBit2Mask[DecBit]; //对应位在最底了
  return BitNum % 10;//得到了
}

//------------------------菜单选择状态进入初始化函数---------------------
static void _MenuEnterInit(void)
{
  Lt4K1Gui_cbOnLight(0xff); //数值全亮,
  Lt4K1Gui.LtCount = Lt4K1Gui.Sel;
  Lt4K1Gui.LtIndex = 0; //重新初始化
  Lt4K1Gui.eState = Lt4K1Gui_eMenuSel;//菜单内了
}

//------------------------菜单选择状态切换函数---------------------
static void _MenuSelSwitch(void)
{
  if(Lt4K1Gui.Sel < Lt4K1Gui_cbGetMaxMenuSel())
    Lt4K1Gui.Sel++;
  else Lt4K1Gui.Sel = 0; //回环了
  Lt4K1Gui.LtCount = Lt4K1Gui.Sel;//更新闪动次数
  Lt4K1Gui.LtIndex = 0; //重头闪动
}

//--------------------进入或切换数值位初始化函数---------------------
static void _NumEnterInit(unsigned char DecBit) //对应进入制调整位
{
  unsigned char Bit = 1 << (1 + DecBit);
  Lt4K1Gui_cbOnLight(Bit); //调整位亮
  Lt4K1Gui_cbOffLight(~Bit); //其它全灭
  //切换时自动边界纠错
  unsigned short Max = Lt4K1Gui_cbGetMaxVol(Lt4K1Gui.Sel);
  unsigned short Vol = Lt4K1Gui.CurVol;  
  if(Vol > Max) Vol = Max;
  else{
    unsigned short Min = Lt4K1Gui_cbGetMinVol(Lt4K1Gui.Sel);
    if(Vol < Min) Vol = Min;
  }
  Lt4K1Gui.CurVol = Vol;
  Lt4K1Gui.LtCount = _GetDecBitNum(Vol, DecBit);//此位值
  Lt4K1Gui.LtIndex = 0; //重新初始化
  Lt4K1Gui.eState = (enum _Lt4K1Gui_eState)(DecBit + Lt4K1Gui_eNumH);//对应调整位
}

//------------------------数值键改变函数-------------------------
static void _NumChange(void)
{
  unsigned char BitPos = Lt4K1Gui.eState - Lt4K1Gui_eNumH;
  unsigned short Vol = Lt4K1Gui.CurVol;
  unsigned char BNum = _GetDecBitNum(Vol, BitPos);//此位值
  unsigned short DecMask = _DecBit2Mask[BitPos];
  Vol -= DecMask * BNum; //去此位
  BNum++;//增加了，需检查是否回环
  unsigned short Max = Lt4K1Gui_cbGetMaxVol(Lt4K1Gui.Sel);
  if(BNum >= 10){//回环了
    if(Max / (DecMask * 10)) BNum = 0; //高位有值时可到底
    else{//无高位时，本位只能取最低
      BNum = _GetDecBitNum(Lt4K1Gui_cbGetMinVol(Lt4K1Gui.Sel), BitPos);
    }
  }
  else if((Max / DecMask) < 10){//增加有限制了
    if(BNum > _GetDecBitNum(Max, BitPos))
      BNum = _GetDecBitNum(Lt4K1Gui_cbGetMinVol(Lt4K1Gui.Sel), BitPos);
  }
  Lt4K1Gui.CurVol = Vol + DecMask * BNum; //加此位
  Lt4K1Gui.LtCount = BNum;//更新闪动次数
  Lt4K1Gui.LtIndex = 0; //重头闪动  
}

//----------------------------------任务函数----------------------------
//建议放入64mS进程中
void Lt4K1Gui_Task(void)
{
  //===========================得到当前按键状态=============================
  unsigned char KeyState = 0; //0无效,1短按 2长按
  //检查按键的按下与松开时刻
  if(Lt4K1Gui_cbIsKeyDown()){//一直按着计时
    if(Lt4K1Gui.KeyIndex < 255) Lt4K1Gui.KeyIndex++;
    //直到需到长按
    if(Lt4K1Gui.KeyIndex != LT4K1_GUI_KEY_LONG) return;
    KeyState = 2;//长按了
  }
  else{//没按或松开了
    unsigned char KeyIndex = Lt4K1Gui.KeyIndex;
    Lt4K1Gui.KeyIndex = 0;//重新计数
    if((KeyIndex >= LT4K1_GUI_KEY_VILID) &&  //按键有效了
       (KeyIndex < LT4K1_GUI_KEY_LONG)){//为短按了
       KeyState = 1;//短按了
    }
    else return; //无效
  }
  //按键有效了
  Lt4K1Gui.QuitIndex = LT4K1_GUI_QUIT_OV;//有效按键触发了
  Lt4K1Gui_cbKeyValidNotify(KeyState); //有效按键通报

  //===========================有效按键处理=============================
  unsigned short Max;
  switch(Lt4K1Gui.eState){
    //空闲状态，准备识别长按
    case Lt4K1Gui_eIdie: 
      Lt4K1Gui.eState = (enum _Lt4K1Gui_eState)KeyState;//1,2对应
      Lt4K1Gui_cbSetCurVol(0 - KeyState, 0);
      break;
    case Lt4K1Gui_eFun0: //功能0状态
      if(KeyState == 2){//长按进入菜单
        Lt4K1Gui_cbEnterGUI();
        Lt4K1Gui.Sel = 1;//从菜单1开始(其它亮，菜单一闪一闪表示进入)
        _MenuEnterInit();
      }
      else Lt4K1Gui_QuitMenu(); //短按退出菜单
      break;
    case Lt4K1Gui_eFun1://功能1状态按键直接退出菜单    
      Lt4K1Gui_QuitMenu(); 
      break;
    case Lt4K1Gui_eMenuSel: //菜单选择状态按键
      if(KeyState == 1) _MenuSelSwitch(); //短按键菜单项切换
      else{//长按数值的进入最高位调整
        if(Lt4K1Gui.Sel == 0){//主动退出菜单
          Lt4K1Gui_QuitMenu(); 
          return;
        }
        //加载原始值与决定调整位
        Lt4K1Gui.CurVol = Lt4K1Gui_cbGetCurVol(Lt4K1Gui.Sel); 
        unsigned short Max = Lt4K1Gui_cbGetMaxVol(Lt4K1Gui.Sel);
        if(Max >= 100) _NumEnterInit(0);//有最高位
        else if(Max >= 10) _NumEnterInit(1);//有中间位
        else _NumEnterInit(2);//只有最低位
      }
    break;
    case Lt4K1Gui_eNumH: 
    case Lt4K1Gui_eNumM:      
    case Lt4K1Gui_eNumL:    
      if(KeyState == 1) _NumChange(); //短按键数值调整项切换
      else{//长按时切换下一位，最低位长按保存
        unsigned char Pos = Lt4K1Gui.eState + 1;
        if(Pos > Lt4K1Gui_eNumL){//最低位了
          Lt4K1Gui_cbSetCurVol(Lt4K1Gui.Sel, Lt4K1Gui.CurVol);//保存
          _MenuEnterInit(); //重新进菜单
        }
        else{//切换下一位 
          _NumEnterInit(Pos - Lt4K1Gui_eNumH); //下一位初始化
        }
      }
    break;
    default: Lt4K1Gui_QuitMenu(); break;
  }
}



