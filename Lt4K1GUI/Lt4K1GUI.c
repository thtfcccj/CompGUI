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
  if(Lt4K1Gui.eState < Lt4K1Gui_eMenuSel) return;//不在受控状态
  
  //这里只负责当前被激活指示灯的显示更新
  unsigned char IsOn; //当前是否显示
  if(!Lt4K1Gui.LtIndex){//下个周期开始了，首个周期亮
    IsOn = 1;
    if(Lt4K1Gui.LtCount) //非0时闪
       Lt4K1Gui.LtIndex = (Lt4K1Gui.LtCount + LT4K1_GUI_LED_IDIE) * 2;
    else Lt4K1Gui.LtIndex = LT4K1_GUI_LED_IDIE * 4; //0时亮双周期
  }
  else{
    Lt4K1Gui.LtIndex--;
    if(Lt4K1Gui.LtIndex >= (LT4K1_GUI_LED_IDIE * 2)){//指示周期
      if(Lt4K1Gui.LtCount){ //非0时闪
        if(Lt4K1Gui.LtIndex & 0x01) IsOn = 0;//单数周期灭
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
  Lt4K1Gui.Sel = 1;//从菜单1开始(其它亮，菜单一闪一闪表示进入)
  Lt4K1Gui.LtCount = 1;
  Lt4K1Gui.LtIndex = 0; //重新初始化
  Lt4K1Gui.eState = Lt4K1Gui_eMenuSel;//菜单内了
}

//------------------------菜单选择状态切换函数---------------------
static void _MenuSelSwitch(void)
{
  if(Lt4K1Gui.Sel < Lt4K1Gui_cbGetMaxMenuSel())
    Lt4K1Gui.Sel++;
  else Lt4K1Gui.Sel = 1; //回环了
  Lt4K1Gui.LtCount = Lt4K1Gui.Sel;//更新闪动次数
  Lt4K1Gui.LtIndex = 0; //重头闪动
}

//--------------------进入或切换数值位初始化函数---------------------
static void _NumEnterInit(unsigned char DecBit) //对应进入制调整位
{
  unsigned char Bit = 1 << (1 + DecBit);
  Lt4K1Gui_cbOnLight(Bit); //调整位亮
  Lt4K1Gui_cbOffLight(~Bit); //其它全灭
  Lt4K1Gui.LtCount = _GetDecBitNum(Lt4K1Gui.CurVol, DecBit);//此位值
  Lt4K1Gui.LtIndex = 0; //重新初始化
  Lt4K1Gui.eState = (enum _Lt4K1Gui_eState)(DecBit + Lt4K1Gui_eNumH);//对应调整位
}

//------------------------数值键改变函数-------------------------
static void _NumChange(void)
{
  unsigned char BitPos = Lt4K1Gui.eState - Lt4K1Gui_eNumH;
  unsigned short Vol = Lt4K1Gui.CurVol;
  unsigned char BNum = _GetDecBitNum(Vol, BitPos);//此位值
  Vol -= _DecBit2Mask[BitPos] * BNum; //去此位
  BNum++;//增加了，需检查是否回环
  if(BNum > _GetDecBitNum(Lt4K1Gui_cbGetMaxVol(Lt4K1Gui.Sel), BitPos))
    BNum = _GetDecBitNum(Lt4K1Gui_cbGetMinVol(Lt4K1Gui.Sel), BitPos);
  Lt4K1Gui.CurVol = Vol + _DecBit2Mask[BitPos] * BNum; //加此位
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
  }
  else{//没按或松开了
    if((Lt4K1Gui.KeyIndex >= LT4K1_GUI_KEY_VILID) &&  //按键有效了
       (Lt4K1Gui.KeyIndex < (LT4K1_GUI_KEY_LONG * 3))){//长按时间不能过长
      if(Lt4K1Gui.KeyIndex < LT4K1_GUI_KEY_LONG) KeyState = 1;//短按
      else  KeyState = 2;//长按
      Lt4K1Gui.QuitIndex = LT4K1_GUI_QUIT_OV;//有效按键触发了
      Lt4K1Gui_cbKeyValidNotify(KeyState); //有效按键通报
    }
    //else KeyState = 0;
    Lt4K1Gui.KeyIndex = 0;//重新计数
  }
  
  //=========================自动退出菜单处理===============================
  unsigned char ePrvState = Lt4K1Gui.eState;
  if(Lt4K1Gui.QuitIndex){
    Lt4K1Gui.QuitIndex--;
    if(!Lt4K1Gui.QuitIndex)  Lt4K1Gui_QuitMenu(); //时间到，退出菜单
  }
  else return; //没在菜单内
  if(KeyState == 0) return; //当前无按键

  //===========================有效按键处理=============================
  switch(Lt4K1Gui.eState){
    //空闲状态，准备识别长按
    case Lt4K1Gui_eIdie: 
      Lt4K1Gui.eState = (enum _Lt4K1Gui_eState)KeyState;//1,2对应
      Lt4K1Gui_cbSetCurVol(0 - KeyState, 0);
      break;
    case Lt4K1Gui_eFun0: //功能0状态
      if(KeyState == 2){//长按进入菜单
        Lt4K1Gui_cbEnterGUI();
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
        Lt4K1Gui_cbGetCurVol(Lt4K1Gui.Sel); //加载原始值  
        _NumEnterInit(0);
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
    default: Lt4K1Gui_QuitMenu(); break;
  }
}



