/*************************************************************************

                一键+4灯GUI程序
//此模块通过1个按键(负责扫描)，4个指示灯，可实现最大0~999数值调整
//按键一般为自检键，灯1定义为菜单灯，灯2~4对应为十进制低中高位灯，即最大999  
//此模块独立于应用
**************************************************************************/
#ifndef __LT4K1_GUI_H
#define __LT4K1_GUI_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

//操作及显示说明:
//功能1：短按一下，进入功能1， 一般实现为自检声光
//功能2：功能1后，长按键2~5s时松手，进入功能2， 一般实现为自检继电器输出等
//(可选)功能3：功能2后，若一直长按不放，则在计数周期达到满时，实现功能3
//功能4：功能1后短按
//进入菜单：长按后时松手，再次短按，菜单灯闪动提示对应菜单项，其它灯常亮
//选择菜单：在菜单中按短按键循环切换菜单，以指菜单灯闪动次数表示菜单位置
//进入调整：在选择菜单中长按进入,此时除最高调整位灯外，其它灯灭，
//          注：十进制低中高位灯长亮为0，否则闪动次数为对应值。
//调整数值：每短按一次键增加一个值，超过最大时自动从新开始
//切换调整位：每长按一次键切换至到最低位显示(会首先到亮周期)
//保存并退至菜单选择：最低位调整结束时，再次长按保存并退至菜单选择。
//不修改退出菜单：上述任意状态，停留时间超过一定时间，自动退出

/*************************************************************************
                               相关配置
**************************************************************************/

//在全局里定义以支持此模块，不定义则不支持
//SUPPORT_LT4K1_GUI 

//支持功能3时定义
//#define SUPPORT_LT4K1_GUI_FUN3     

//定义指示灯停止显示的时间,<255 - 10*2;
#ifndef LT4K1_GUI_LED_IDIE
  #define   LT4K1_GUI_LED_IDIE          4   
#endif

//按键时刻判断
#ifndef LT4K1_GUI_KEY_VILID
  #define   LT4K1_GUI_KEY_VILID       2    //定义有效按键时间,>=2
#endif
#ifndef LT4K1_GUI_KEY_LONG
  #define   LT4K1_GUI_KEY_LONG        25   //定义长按键时间
#endif

#ifndef LT4K1_GUI_QUIT_OV//退出菜单计时器超时值, 16mS或32ms为单位
  #define   LT4K1_GUI_QUIT_OV         250  
#endif

#ifndef LT4K1_GUI_KEY_OV//定义一起按住键后停止时间，主要供功能3时时用
  #define   LT4K1_GUI_KEY_OV         255   //需>LT4K1_GUI_KEY_LONG  
#endif

/*************************************************************************
                               相关结构
*************************************************************************/
//状态机
enum _Lt4K1Gui_eState{
  Lt4K1Gui_eIdie    = 0,   //空闲状态，准备识别按键
  Lt4K1Gui_eFun0    = 1,   //短按进入功能0状态
  Lt4K1Gui_eFun1    = 2,   //长按进入功能1状态
  Lt4K1Gui_eMenuSel = 3,   //短按再长按进入菜单选择状态 
  Lt4K1Gui_eNumH    = 4,   //数值高位调整状态  
  Lt4K1Gui_eNumM    = 5,   //数值中位调整状态
  Lt4K1Gui_eNumL    = 6,   //数值低位调整状态
};

struct _Lt4K1Gui{
  unsigned char LtCount;        //当前状态机对应指示灯闪动次数,0~9
  unsigned char LtIndex;        //指示灯正在闪动计数
  unsigned char KeyIndex;       //按键计数，用于判断长按与短按等
  enum _Lt4K1Gui_eState eState; //状态机
  unsigned char Sel;            //菜单项
  unsigned short CurVol;        //当前调整的数值
  unsigned short QuitIndex;    //退出菜单计数
};

extern struct _Lt4K1Gui Lt4K1Gui;

/*************************************************************************
                               相关函数-支持此模块时
*************************************************************************/
#ifdef SUPPORT_LT4K1_GUI
//----------------------------初始化函数--------------------------
void Lt4K1Gui_Init(void);

//----------------------------------任务函数----------------------------
//建议放入16mS或32ms进程中
void Lt4K1Gui_Task(void);

//--------------------------指示灯任务函数-------------------------
//放入256mS进程中
void Lt4K1Gui_LedTask(void);

//-----------------------------是否在GUI系统中-----------------------
#define Lt4K1Gui_IsInGUI()    (Lt4K1Gui.eState >= Lt4K1Gui_eMenuSel)

//-----------------------------是否在GUI预备中-----------------------
//长按在预备状态,外部可触发灯
#define Lt4K1Gui_IsGUIRdy()    (Lt4K1Gui.eState == Lt4K1Gui_eFun1)

//-----------------------------是否功能1中-----------------------
#define Lt4K1Gui_IsFun1()    (Lt4K1Gui.eState == Lt4K1Gui_eFun0)

//------------------------------退出菜单函数----------------------------
void Lt4K1Gui_QuitMenu(void);

/*************************************************************************
                               相关函数-不支持此模块时
*************************************************************************/
#else //#ifdef SUPPORT_LT4K1_GUI

//----------------------------初始化函数--------------------------
#define Lt4K1Gui_Init() do{}while(0)
 
//----------------------------------任务函数-------------------------
//建议放入128mS进程中
#define Lt4K1Gui_Task() do{}while(0)

//--------------------------指示灯任务函数-------------------------
//放入256mS进程中
#define Lt4K1Gui_LedTask() do{}while(0)

//-----------------------------是否在GUI系统中-----------------------
#define Lt4K1Gui_IsInGUI()    (0)

//-----------------------------是否在GUI预备中-----------------------
//长按在预备状态,外部可触发灯
#define Lt4K1Gui_IsGUIRdy()    (0)

//-----------------------------是否功能1中-----------------------
#define Lt4K1Gui_IsFun1()    (0)

//------------------------------退出菜单函数----------------------------
#define Lt4K1Gui_QuitMenu() do{}while(0)

#endif //#ifdef SUPPORT_LT4K1_GUI
/*************************************************************************
                        相关回调函数-底层相关
*************************************************************************/
#include "IOCtrl.h"

//----------------------判断按键是否按下--------------------------
//unsigned char Lt4K1Gui_cbIsKeyDown(void);
#define Lt4K1Gui_cbIsKeyDown()  IsKeyTest()

//-------------------------指示灯控制--------------------------
void Lt4K1Gui_cbOnLight(unsigned char Mask);   //对应掩码灯亮
void Lt4K1Gui_cbOffLight(unsigned char Mask);  //对应掩码灯灭

//---------------------------有效按键通报--------------------------
//形参为1:短按 2:长按
void Lt4K1Gui_cbKeyValidNotify(unsigned char KeyState);

/*************************************************************************
                        相关回调函数-应用相关
*************************************************************************/

//-------------------------进入GUI系统通报--------------------------
//进入后，此GUI要接管指示灯，相关连的状态(如测试)应退至普通状态
void Lt4K1Gui_cbEnterGUI(void);

//-------------------------退出GUI系统通报--------------------------
//退出后，可用此回调恢复现场
void Lt4K1Gui_cbQuitGUI(void);

  /*************************************************************************
                        相关回调函数-参数相关
*************************************************************************/
//--------------------------得到菜单项数----------------------------
unsigned char Lt4K1Gui_cbGetMaxMenuSel(void);

//----------------------------加载数值相关----------------------------
unsigned short Lt4K1Gui_cbGetCurVol(unsigned char Sel); //得到当前值
unsigned short Lt4K1Gui_cbGetMinVol(unsigned char Sel); //得到最小值
unsigned short Lt4K1Gui_cbGetMaxVol(unsigned char Sel); //得到最大值
//-1:短按，2，长按，>=1菜单内保存
void Lt4K1Gui_cbSetCurVol(signed char Sel, unsigned short CurVol);

#endif //#define __LT4K1_GUI_H

  
  


