/***************************************************************************

                  多例化TM1628驱动模块实现
此模块负责一个TM1628的自动更新与显示(从GRID1开始)，以及获取键值(从KS1开始)
暂仅支持阻塞方式读写数据,所有实例均应位于同一进程下
***************************************************************************/
#ifndef __LED_DRV_1628_H
#define __LED_DRV_1628_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***************************************************************************
                              相关配置
***************************************************************************/

//支持此模块时，外部全局定义
//#define SUPPORT_TM1628M 

//支持获取键值时定义,只要有一个实例有按键，则需定义
//#define SUPPORT_TM1628_KEY

//定义LED最大个数, 以最大实例数为准，根据1628资料，最大支持2*7Byte
//注：扫描从第0段开始
#ifndef TM1628_LED_MAX_COUNT
  #define TM1628_LED_MAX_COUNT      (2 * 4 + 2) //默认4位数码管+1组指示灯 
#endif

//定义指令缓冲区大小
#if TM1628_LED_MAX_COUNT > 4
  #define TM1628_COM_BUF   (TM1628_LED_MAX_COUNT + 2)
#else
  #define TM1628_COM_BUF   4
#endif

//定义周期初始化时间, 以用于断开或中途插入时恢复，16ms为单位, <=255
#ifndef TM1628_LED_PERIOD_INIT_OV 
  #define TM1628_LED_PERIOD_INIT_OV    255
#endif

/***************************************************************************
                              相关结构
***************************************************************************/

//静态配置
struct _TM1628Static{
  //显示模式在开机时调用:0: 4位13段 1: 5位12段 2: 6位11段, 3: 7位10段
  unsigned char DispMode; 
  //显示段总数
  unsigned char LedCount;
  //用户定义段至硬件查找表，以对用户友好
  unsigned char ToHwLut[TM1628_LED_MAX_COUNT];
  //读取按键字节数，0时无按键，1时有按键
  unsigned char KeyByteCount; 
};

struct _TM1628{
  unsigned short DirtyMask;             //LED对应段脏，即需立即更新
  unsigned char DispBuf[TM1628_LED_MAX_COUNT];  //显示缓冲区,与硬件排列用查找表实现映射
  unsigned char Gray;//缓存的灰度值， 0~7越高越亮, 8关闭显示
  unsigned char PeriodInitTimer;        //周期初始化定时器,以响应实时插入
  unsigned char Ch;                     //此TM1628通道号
  #ifdef SUPPORT_TM1628_KEY //支持按键时
    unsigned char Flag;                 //相关标志，见定义
  #endif  
};

//相关标志定义为
#define TM1628_KEY_PERIOD   0x80 //按键读取周期，否则在显示更新周期
#define TM1628_KEY_ID_MASK  0x7f //扫描出的键值ID, >=用户定义时无按键

extern unsigned char TM1628_CommBuf[TM1628_COM_BUF];//内建的通讯缓冲区
/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void TM1628_Init(struct _TM1628 *pTM1628, 
                 unsigned char Ch,         //此TM1628通道号
                 unsigned char Gray);    //0~7越高越亮, 8关闭显示

//--------------------------------任务函数-----------------------------
//约10ms调用一次
void TM1628_Task(struct _TM1628 *pTM1628);

//-------------------------------更新数码管显示-------------------------
void TM1628_UpdateDisp(struct _TM1628 *pTM1628, 
                       unsigned char UserPos,//内部由TM1628_TO_HW转换
                       unsigned char Seg);   //段码

//-------------------------------得到数码管显示-------------------------
unsigned char TM1628_GetDisp(const struct _TM1628 *pTM1628, 
                             unsigned char UserPos);

//---------------------------------更新灰度级别-------------------------
void TM1628_UpdateGray(struct _TM1628 *pTM1628, 
                       unsigned char Gray);    //0~7越高越亮, 8关闭显示

/***************************************************************************
                           回调函数
***************************************************************************/

//----------------------得到当前通道对应Static结构--------------------------
//const struct _TM1628Static *TM1628_cbGetStatic(unsigned char Ch);
//直接宏实现时：
extern const struct _TM1628Static TM1628_CbStatic[];
#define TM1628_cbGetStatic(ch) (&TM1628_CbStatic[ch]);

//----------------------------发送数据---------------------------------------
//TM1628_CommBuf存放要发送的数据,发送完成后返回
void TM1628_cbSendData(unsigned char Ch,
                       unsigned char SendLen);

//----------------------------按收按键值-------------------------------------
#ifdef SUPPORT_TM1628_KEY //支持按键时
  //TM1628_CommBuf[0]为已填充为读键值的1Byte指令,读回键值起始于TM1628_CommBuf[1]
  void TM1628_cbFullKey(unsigned char Ch, unsigned char KeyByteLen);
#endif

//--------------------------键值更新通报-----------------------------
#ifdef SUPPORT_TM1628_KEY //支持按键时
  //获取到键值时将调用此函数(不管是否有按键),键值ID位于TM1628_KEY_ID_MASK
  void TM1628_cbKeyUpdate(const struct _TM1628 *pTM1628);
#endif

//--------------------------是否在测试状态-----------------------------
//测试状态指示灯全亮,直接实现：
#include "Test.h"
#define TM1628_cbIsTest()    Test_IsDoing()

#endif //#define __LED_DRV_1628_H

