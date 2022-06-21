/*******************************************************************************

		             单一自检键GUI实现
此模块使用一个单独的测试键，实现相关指令及菜单系统以实现参数的修改
此模块仅负责单键操作时的状态切换，具体显示由外部实现，如：
//  借用4个指示灯，可实现与Lt4K1GUI相同的效果(此模块后继将不再维护)
//  借用1~4位七段码(数码管或液晶)可实现简单的数值调整
//  借用定制数码管或液晶，可实现较复杂的功能
//  调整项最大值为3项，实际可为1~3项。
*******************************************************************************/
#ifndef _TEST_K1_GUI_H
#define	_TEST_K1_GUI_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/********************************************************************************
		                      按键说明
********************************************************************************/

//------------------------------主界面按键-----------------------------
//空闲时短按: 指令-1：由用户进入标准自检模式
//自检期间短按：
//   单通道： 指令-4：由用户决定此项功能，可实现退出自检状态
//   多通道： 进入循显状态，每短按一次切换一次！
//自检期间长按：指令-2：由用户决定此项功能，如高级自检
//自检期间一直按：可选功能，指令-3：由用户决定此项功能

//空闲时长按：      进入菜单准备模式
//菜单准备模式短按: 进入菜单选择模式

//----------------------------------------菜单内操作--------------------------
//菜单选择模式: 
//  短按：在菜单间切换。
//  长按首项：       退出菜单，可防止用户误操作
//  长按+不可调整：  执行用户指令
//  长按+可调整：    进入数值调整模式

/*************************************************************************
                               相关配置
**************************************************************************/

//在全局里定义以支持此模块，不定义则不支持
//SUPPORT_TEST_K1_GUI 

//支持多值切换显示时
//#define SUPPORT_TEST_K1_GUI_SWITCH_VOL   

//自检期间一直按住为功能3，一般不需要支持
//#define SUPPORT_TEST_K1_GUI_ACT_FUN3     

//按键时刻判断
#ifndef TEST_K1_GUI_KEY_VILID
  #define   TEST_K1_GUI_KEY_VILID       2    //定义有效按键时间,>=2
#endif
#ifndef TEST_K1_GUI_KEY_LONG
  #define   TEST_K1_GUI_KEY_LONG        25   //定义长按键时间
#endif

#ifndef TEST_K1_GUI_QUIT_OV//退出菜单计时器超时值, 16mS或32ms为单位
  #define   TEST_K1_GUI_QUIT_OV         250  
#endif

#ifndef TEST_K1_GUI_KEY_OV//定义一起按住键后停止时间，主要供功能3时时用
  #define   TEST_K1_GUI_KEY_OV         255   //需>TEST_K1_GUI_KEY_LONG  
#endif

/*************************************************************************
                               相关结构
*************************************************************************/
//状态机
enum _TestK1GUI_eState{
  TestK1GUI_eIdie    = 0,   //空闲状态：准备识别按键
  TestK1GUI_eTest    = 1,   //空闲状态短按：进入自检状态
  TestK1GUI_eSwVol   = 2,   //自检时短按：多通道时切换浓度显示(暂未支持)
  
  TestK1GUI_eMenuRdy = 3,   //空闲状态长按：菜单准备模式
  TestK1GUI_eMenuSel = 4,   //菜单准备时短按：进入菜单选择状态
  
  TestK1GUI_eNumH    = 5,   //数值高位调整状态  
  TestK1GUI_eNumM    = 6,   //数值中位调整状态
  TestK1GUI_eNumL    = 7,   //数值低位调整状态(非浓度显时)
};

struct _TestK1GUI{
  unsigned char KeyIndex;       //按键计数，用于判断长按与短按等
  enum _TestK1GUI_eState eState; //状态机
  unsigned char Sel;             //菜单项
  unsigned short CurVol;         //当前调整的数值
  unsigned short QuitIndex;      //退出菜单计数
};

extern struct _TestK1GUI TestK1GUI;

/********************************************************************
                               相关函数-支持此模块时
********************************************************************/

//------------------------------初始化函数--------------------------
void TestK1GUI_Init(void);

//-------------------------------任务函数---------------------------
//建议放入16mS或32ms进程中
void TestK1GUI_Task(void);

//------------------------------是否在自检中---------------------------
#define TestK1GUI_IsInTest()    (TestK1GUI.eState == TestK1GUI_eTest)

//-----------------------------是否在菜单准备中-----------------------
#define TestK1GUI_IsMenuRdy()    (TestK1GUI.eState == TestK1GUI_eMenuRdy)

//-----------------------------是否在菜单中-----------------------
#define TestK1GUI_IsInMenu()    (TestK1GUI.eState >= TestK1GUI_eMenuSel)

//------------------------------退出菜单函数----------------------------
void TestK1GUI_QuitMenu(void);

/*************************************************************************
                        相关回调函数-底层相关
*************************************************************************/
#include "IOCtrl.h"

//----------------------判断按键是否按下--------------------------
//unsigned char TestK1GUI_cbIsKeyDown(void);
#define TestK1GUI_cbIsKeyDown()  IsKeyTest()

//---------------------------有效按键通报--------------------------
//形参为1:短按 2:长按
//形参为1:短按 2:长按
//返回非0：暂停执行后续操作
signed char TestK1GUI_cbKeyValidNotify(unsigned char KeyState);

//---------------------------更新显示通报--------------------------
//状态改变或用户有效按键后，将调用此函数
void TestK1GUI_cbUpdateDisp(enum _TestK1GUI_eState ePrv);

/*************************************************************************
                        相关回调函数-应用相关
*************************************************************************/

//-------------------------进入菜单系统通报--------------------------
//进入后，此GUI要接管指示灯，相关连的状态(如测试)应退至普通状态
void TestK1GUI_cbEnterMenu(void);

//-------------------------退出菜单系统通报--------------------------
//退出后，可用此回调恢复现场
void TestK1GUI_cbQuitMenu(void);

/*************************************************************************
                        相关回调函数-参数相关
*************************************************************************/
//--------------------------得到菜单项数----------------------------
unsigned char TestK1GUI_cbGetMaxMenuSel(void);

//----------------------------加载数值相关----------------------------
unsigned short TestK1GUI_cbGetCurVol(unsigned char Sel); //得到当前值
unsigned short TestK1GUI_cbGetMinVol(unsigned char Sel); //得到最小值
unsigned short TestK1GUI_cbGetMaxVol(unsigned char Sel); //得到最大值
//得到可调整位掩码,0时无可调整项，为只读状态或指令
unsigned char TestK1GUI_cbGetMaskVol(unsigned char Sel); 

//----------------------------指令或保存数值相关----------------------------
//负值为指令：-1声光自检，-2输出动作，-3自检期间一直按住，-4：退出自检或输出恢复，
//正值为菜单内保存
void TestK1GUI_cbSetCurVol(signed char Sel, unsigned short CurVol);

#endif //_TEST_K1_GUI_H


