/***************************************************************************
                       数码管显示驱动程序-使用SPI显示板时的实现
此模块负责扫描驱动数码管的显示,以及从SPI获得键值
此模块使用3131的显示板做为驱动,通过SPI接口操作数据
此模块仅操作SPI接口的0x02~0x0b缓冲,不送闪动等控制
***************************************************************************/

#ifndef __LED_DRV_SPI_H
#define __LED_DRV_SPI_H

/***************************************************************************
                              相关配置
***************************************************************************/

//定义操作SPI接口的起始地址
#define LED_DRV_SPI_ADR_START   0x02     //起始地址

//定义操作SPI接口的硬件地址总数
#define LED_DRV_SPI_ADR_COUNT     8 //接键玛1+相关状态1+小数点1+显示缓冲区5个

//针对SPI操作缓冲区,地址相关定义为(此值+起始地址=SPI发送地址):
#define LED_DRV_SPI_ADR_KEY     0x00    //按键值
#define LED_DRV_SPI_ADR_STATE   0x01    //相关状态,见协议与定义
#define LED_DRV_SPI_ADR_DOT     0x02    //小数点显示缓冲区
#define LED_DRV_SPI_ADR_SEG     0x03    //7段码的显示起始
#define LED_DRV_SPI_SEG_COUNT   5       //支持7段码的个数
//STATE使和到的位定义为:
#define LED_DRV_SPI_UPDATE      0x80    //立即更新标志

/***************************************************************************
                              相关结构
***************************************************************************/

//Spi状态机
enum _eLedDrv_Spi{
  eLedDrv_Spi_Idie =    0,    //空闲模式,检查主控(从机显示板)中断请求
  eLedDrv_Spi_Wait =    1,    //有中断了,等待CS为高
  eLedDrv_Spi_Doing =   2,    //发送数据个程中
};

struct _LedDrv_Spi{
enum _eLedDrv_Spi eState; //SPI状态机  
  unsigned char Buf[LED_DRV_SPI_ADR_COUNT];   //SPI有效缓冲区
};

extern struct _LedDrv_Spi  LedDrv_Spi;

/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void LedDrv_Spi_Init(void);

//----------------------------快速任务函数-----------------------------
//放入快速进程中扫描
void LedDrv_Spi_FastTask(void);

//---------------------------立即置更新数据标志函数---------------------------
//void LedDrv_Spi_SetUpdate(void);
#define LedDrv_Spi_SetUpdate() do{\
LedDrv_Spi.Buf[LED_DRV_SPI_ADR_STATE] |= LED_DRV_SPI_UPDATE;}while(0)

//--------------------------设置某个数码管函数---------------------------
void LedDrv_Spi_SetLed(unsigned char Pos,     //数码管位置
                       unsigned char SetDot);//带LED_SIGN_DOT的显示段码值

//--------------------------得到某个数码管函数---------------------------
//返回带LED_SIGN_DOT的显示段码值
unsigned char LedDrv_Spi_GetLed(unsigned char Pos);     //数码管位置

//--------------------------得到某个无小数点数码管函数---------------------------
//unsigned char LedDrv_Spi_GetLedNoLed(unsigned char Pos);     //数码管位置
#define LedDrv_Spi_GetLedNoLed(pos) (LedDrv_Spi.Buf[LED_DRV_SPI_ADR_SEG + pos])

//--------------------------小数点操作函数---------------------------
#define LedDrv_Spi_SetDot(mask) \
  do{LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] |= mask;}while(0)
#define LedDrv_Spi_ClrSetDot(mask) \
  do{LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] &= ~(mask);}while(0)
#define LedDrv_Spi_IsSetDot(mask) \
  (LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] &= ~(mask))

/***************************************************************************
                    资源需求回调函数或所需定义声明
***************************************************************************/


/***************************************************************************
                           回调函数
***************************************************************************/

//--------------------------是否在测试状态-----------------------------
//测试状态指示灯全亮
#include "Test.h"
#define LedDrv_Spi_cbIsTest()    Test_IsDoing()

//-------------------------有键值通报-----------------------------
//检查到有按键时将会调用此函数,接口符合MenuMng标准
#include "Key_CB.h"
#define LedDrv_Spi_cbKeyNotify(key)    Key_CB_SetKey(key)


#endif //#define __LED_DRV_SPI_H

