/*************************************************************************

                单一自检键GUI实现

**************************************************************************/
#include "TestK1GUI.h"
#include <string.h>

struct _TestK1GUI TestK1GUI;

//----------------------------------初始化函数--------------------------
void TestK1GUI_Init(void)
{
  memset(&TestK1GUI, 0, sizeof(struct _TestK1GUI));
}

//------------------------------退出菜单函数----------------------------
void TestK1GUI_QuitMenu(void)
{
  //if(TestK1GUI.eState >= TestK1GUI_eMenuSel)
  TestK1GUI_cbQuitMenu();
  TestK1GUI.QuitIndex = 0;
  TestK1GUI.eState = TestK1GUI_eIdie;
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
  TestK1GUI.eState = TestK1GUI_eMenuSel;//菜单内了
}

//------------------------菜单选择状态切换函数---------------------
static void _MenuSelSwitch(void)
{
  if(TestK1GUI.Sel < TestK1GUI_cbGetMaxMenuSel())
    TestK1GUI.Sel++;
  else TestK1GUI.Sel = 0; //回环了
}

//--------------------进入或切换数值位初始化函数---------------------
static void _NumEnterInit(unsigned char DecBit) //对应进入制调整位
{
  unsigned char Bit = 1 << (1 + DecBit);
  //切换时自动边界纠错
  unsigned short Max = TestK1GUI_cbGetMaxVol(TestK1GUI.Sel);
  unsigned short Vol = TestK1GUI.CurVol;  
  if(Vol > Max) Vol = Max;
  else{
    unsigned short Min = TestK1GUI_cbGetMinVol(TestK1GUI.Sel);
    if(Vol < Min) Vol = Min;
  }
  TestK1GUI.CurVol = Vol;
  TestK1GUI.eState = (enum _TestK1GUI_eState)(DecBit + TestK1GUI_eNumH);//对应调整位
}

//------------------------数值键改变函数-------------------------
static void _NumChange(void)
{
  unsigned char BitPos = TestK1GUI.eState - TestK1GUI_eNumH;
  unsigned short Vol = TestK1GUI.CurVol;
  unsigned char BNum = _GetDecBitNum(Vol, BitPos);//此位值
  unsigned short DecMask = _DecBit2Mask[BitPos];
  Vol -= DecMask * BNum; //去此位
  BNum++;//增加了，需检查是否回环
  unsigned short Max = TestK1GUI_cbGetMaxVol(TestK1GUI.Sel);
  if(BNum >= 10){//回环了
    if(Max / (DecMask * 10)) BNum = 0; //高位有值时可到底
    else{//无高位时，本位只能取最低
      BNum = _GetDecBitNum(TestK1GUI_cbGetMinVol(TestK1GUI.Sel), BitPos);
    }
  }
  else if((Max / DecMask) < 10){//增加有限制了
    if(BNum > _GetDecBitNum(Max, BitPos))
      BNum = _GetDecBitNum(TestK1GUI_cbGetMinVol(TestK1GUI.Sel), BitPos);
  }
  TestK1GUI.CurVol = Vol + DecMask * BNum; //加此位
}

//----------------------------------任务函数----------------------------
//建议放入16mS或32ms进程中
void TestK1GUI_Task(void)
{
  //===========================得到当前按键状态=============================
  unsigned char KeyState = 0; //0无效,1短按 2长按
  //检查按键的按下与松开时刻
  if(TestK1GUI_cbIsKeyDown()){//一直按着计时
    if(TestK1GUI.KeyIndex < (TEST_K1_GUI_KEY_OV - 1)) TestK1GUI.KeyIndex++;
    #ifdef SUPPORT_TEST_K1_GUI_ACT_FUN3     //自检期间一直 按住为功能3
      else if(TestK1GUI.KeyIndex == (TEST_K1_GUI_KEY_OV - 1)){
        TestK1GUI.KeyIndex = TEST_K1_GUI_KEY_OV;//结束
        if(TestK1GUI.eState == TestK1GUI_eTest) //进入功能3
          TestK1GUI_cbSetCurVol(-3, 0); 
      }
    #endif
    //直到需到长按
    if(TestK1GUI.KeyIndex != TEST_K1_GUI_KEY_LONG) return;
    KeyState = 2;//长按了
  }
  else{//没按或松开了
    unsigned char KeyIndex = TestK1GUI.KeyIndex;
    TestK1GUI.KeyIndex = 0;//重新计数
    if((KeyIndex >= TEST_K1_GUI_KEY_VILID) &&  //按键有效了
       (KeyIndex < TEST_K1_GUI_KEY_LONG)){//为短按了
       KeyState = 1;//短按了
    }
    else return; //无效
  }
  //按键有效了
  TestK1GUI.QuitIndex = TEST_K1_GUI_QUIT_OV;//有效按键触发了
  if(TestK1GUI_cbKeyValidNotify(KeyState)) return; //有效按键通报

  //===========================有效按键处理=============================
  enum _TestK1GUI_eState ePrv = TestK1GUI.eState;
  switch(TestK1GUI.eState){
    //空闲状态，准备识别长短按
    case TestK1GUI_eIdie: 
      if(KeyState == 1){ //短按进入自检状态
        TestK1GUI.eState = TestK1GUI_eTest;
        TestK1GUI_cbSetCurVol(-1, 0); 
      }
      else{//长按为菜单准备
        TestK1GUI.eState = TestK1GUI_eMenuRdy;
      }
      break;
    case TestK1GUI_eTest: //自检状态
      if(KeyState == 2)//长按进入自检输出状态
        TestK1GUI_cbSetCurVol(-2, 0);             
      else TestK1GUI_cbSetCurVol(-4, 0); //短按功能4(退出自检或动作恢复)
      break;
    case TestK1GUI_eMenuRdy://菜单准备状态
      if(KeyState == 1){//短按进入菜单
        TestK1GUI_cbEnterMenu();
        TestK1GUI.Sel = 0;//从菜单0开始方便直接退出
        _MenuEnterInit();
      }
      else TestK1GUI_QuitMenu(); //长按退出菜单
      break;
    case TestK1GUI_eMenuSel: //菜单选择状态按键
      if(KeyState == 1) _MenuSelSwitch(); //短按键菜单项切换
      else{//长按数值的进入最高位调整
        if(TestK1GUI.Sel == 0){//主动退出菜单
          TestK1GUI_QuitMenu(); 
          return;
        }
        //先加载原始值与决定调整位
        TestK1GUI.CurVol = TestK1GUI_cbGetCurVol(TestK1GUI.Sel); 
        //为只读状态或指令
        if(TestK1GUI_cbGetMaskVol(TestK1GUI.Sel) == 0){
          TestK1GUI_cbSetCurVol(TestK1GUI.Sel, 0);//通报用户执行
          break;
        }
        unsigned short Mask = TestK1GUI_cbGetMaskVol(TestK1GUI.Sel);
        if(Mask & 0x40) _NumEnterInit(0);//有最高位
        else if(Mask & 0x20) _NumEnterInit(1);//有中间位
        else _NumEnterInit(2);//只有最低位
      }
    break;
    case TestK1GUI_eNumH: 
    case TestK1GUI_eNumM:      
    case TestK1GUI_eNumL:    
      if(KeyState == 1) _NumChange(); //短按键数值调整项切换
      else{//长按时切换下一位，最低位长按保存
        unsigned char Pos = TestK1GUI.eState + 1;
        if((Pos > TestK1GUI_eNumL) || //最低位了
          !(TestK1GUI_cbGetMaskVol(TestK1GUI.Sel) & 0x01)){//最低位禁止调整时
          TestK1GUI_cbSetCurVol(TestK1GUI.Sel, TestK1GUI.CurVol);//保存
          _MenuEnterInit(); //重新进菜单
        }
        else{//切换下一位 
          _NumEnterInit(Pos - TestK1GUI_eNumH); //下一位初始化
        }
      }
    break;
    default: TestK1GUI_QuitMenu(); break;
  }
  
  TestK1GUI_cbUpdateDisp(ePrv);//更新显示
}



