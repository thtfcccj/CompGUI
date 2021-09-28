/***************************************************************************
                       数码管显示接口
此模块负责与用户交互,并将数据写到输出缓冲区
此模块独立于软硬件
缓冲区接口统一定义为：1亮,0灭  初始化为全亮状态
***************************************************************************/

#ifndef __LED_H
#define __LED_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***************************************************************************
                              相关配置
***************************************************************************/

//定义LED个数, <= 8
#ifndef LED_LED_COUNT               
  #define LED_LED_COUNT            4  
#endif

//定义可独立控制段码闪动的段闪动控制数,可用于扩展指示灯,< LED_LED_COUNT
#define LED_SEG_FLASH_COUNT        3

//定义定时器,决定闪动频率
#define LED_FLASH_TIMER_COUNT      32

/***************************************************************************
                              相关结构
***************************************************************************/
struct _Led{
  //用户操作显示缓冲区:为与应用层对应,不能改变下列数据的放置位置
  //缓冲区或位掩码均为: 低位对应数码管最低位,即由右到左排列,0~3为段码专用
  unsigned char DotDisp;           //每个段码的小数点显示
  unsigned char SegDisp[LED_LED_COUNT]; //段码显示缓冲区
  unsigned char SegFlash;          //定义段码的闪动控制
  unsigned char DotFlash;          //每个段码的小数点闪动
  unsigned char SegGrpFlash[LED_SEG_FLASH_COUNT];   //可以单独控制段码闪动的段
  unsigned char DisBuf[LED_LED_COUNT];        //附加:禁止更新的段缓冲区
  unsigned char Timer;                        //附加:定时器
  
  unsigned char Flag;   //相关标志,见定义
};
//相关标志定义为:(与应用层对应,不能改变下放置位置)
#define LED_DISP          0x10      //在显示周期,否则在消隐周期
#define LED_FLASH_NOW     0x20      //强制更新闪动标志
#define LED_FLASH_DIS     0x40      //强制更新闪动标志置位时,处于消隐还是显示周期
#define LED_UPDATE_NOW    0x80      //强制更新扫描标志,此位置位将强制更新扫描

extern struct _Led  Led;

/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void Led_Init(void);

//-------------------------------任务函数-----------------------------
//放入16ms进程中扫描,以获得闪动效果,也可直接调用更新数据
void Led_Task(void);

/***************************************************************************
                    资源需求回调函数或所需定义声明
***************************************************************************/
#include "IOCtrl.h"

//-----------定义硬件实现时,小数点在段码中的位置掩码---------------
#include "LedSign.h"
#define LED_DOT_BUF_MASK            LED_SIGN_DOT

//-----------------------置显示缓冲函数-----------------------------
#ifdef SUPPORT_LCD
  #include "LcdDrv.h"
  #define Led_cbSetBuf(pos, buf)  \
    do{LcdDrv.Buf[(pos) + LCD_BUF_MD_START] = buf;}while(0)
#endif

#ifdef SUPPORT_LED
  #include "LedDrv.h"
  #define Led_cbSetBuf(pos, buf)  do{LedDrv.Buf[pos] = buf;}while(0)
#endif

#ifdef SUPPORT_LED_SPI
  #include "LedDrv_Spi.h"
  #define Led_cbSetBuf(pos, buf)  do{LedDrv_Spi_SetLed(pos, buf);}while(0)
#endif
      
#ifdef SUPPORT_LED_1652
  #include "LedDrv1652.h"
  #define Led_cbSetBuf(pos, buf)  do{\
      LedDrv1652.Buf[pos] = buf; LedDrv1652.UpdateMask |= 1 << (Pos); }while(0)
#endif

#ifdef SUPPORT_TM1628
  #include "TM1628.h"
  #define Led_cbSetBuf(pos, buf)  do{TM1628_UpdateDisp(pos, buf); }while(0)
#endif

//-----------------------得到显示缓冲函数-----------------------------
#ifdef SUPPORT_LCD
  #define Led_cbGetBuf(pos)  (LcdDrv.Buf[(pos) + LCD_BUF_MD_START])
#endif

#ifdef SUPPORT_LED
  #define Led_cbGetBuf(pos)  (LedDrv.Buf[pos])
#endif

#ifdef SUPPORT_LED_SPI
  #define Led_cbGetBuf(pos)  LedDrv_Spi_GetLed(pos)
#endif
      
#ifdef SUPPORT_LED_1652
  #define Led_cbGetBuf(pos)  (LedDrv1652.Buf[pos])
#endif

#ifdef SUPPORT_TM1628
  #define Led_cbGetBuf(pos)  (TM1628_GetDisp(pos))
#endif

/***************************************************************************
                           回调函数
***************************************************************************/

//--------------------------Led附加功能初始化函数------------------------
//用于完成除4段显示外的其它显示功能等(如LCD时米字符等)初始化
//形参为Flag位定义
#ifdef SUPPORT_LCD
  void Led_cbAppendInit(void);
#else
  #define Led_cbAppendInit()  do{}while(0)
#endif

//--------------------------Led附加功能更新函数------------------------
//用于完成除4段显示外的其它显示功能等(如LCD时米字符等)的显示更新
//形参为Flag位定义
#ifdef SUPPORT_LCD
  void Led_cbAppendUpdate(unsigned char Flag);
#elif defined(SUPPORT_LED_1652)//置全部更新标志
  #define Led_cbAppendUpdate(Flag)  do{LedDrv1652.UpdateMask = 0xff;}while(0)
#else
  #define Led_cbAppendUpdate(Flag)  do{}while(0)
#endif

#endif //#define __LED_H

