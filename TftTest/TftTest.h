/*******************************************************************************

                         显示屏测试功能实现
暂仅支持标准色测试,支持暂停没前显示以用于测试屏幕好坏
*******************************************************************************/
#ifndef _TFT_TEST_H 
#define _TFT_TEST_H

/*******************************************************************************
                          相关配置
*******************************************************************************/

#define TFT_TEST_OV     (3*8)  //  测试时间,秒为单位, 8的倍数

/*******************************************************************************
                          相关结构
*******************************************************************************/
struct _TftTest{
  unsigned char Type;          //测试类型
  unsigned char Timer;         //测试定时器，置位时指示灯全亮
};

extern struct _TftTest TftTest;

/*******************************************************************************
                                   相关函数
*******************************************************************************/

//---------------------------------初始化----------------------------
#define TftTest_Init() do{TftTest.Timer = 0;}while(0)

//---------------------------------任务函数-------------------------
//1s更新一次
void TftTest_Task(void);

//---------------------------是否在测试状态--------------------------
#define TftTest_IsDoing() (TftTest.Timer)

//----------------------------启动测试-------------------------------
//测试中调用将一直当前显示7s,否则开始测试
void TftTest_Start(unsigned char Type); //测试类型,见说明

//-------------------------退出测试------------------------------
void TftTest_Exit(void);

/*******************************************************************************
                            上层回调函数
*******************************************************************************/

//----------------------------退出测试通报--------------------------------
void TftTest_cbExitNotify(void);
//#define TftTest_cbExitNotify() do{}while(0)


#endif //#ifndef _TEST_H 

