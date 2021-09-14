/***************************************************************************
                       数码管显示接口
***************************************************************************/

#include "Led.h"
#include <string.h>

struct _Led  Led;

/***************************************************************************
                              相关函数实现
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void Led_Init(void)
{
  memset(&Led, 0, sizeof(struct _Led));
  //memset(&Led.SegDisp, 0xff, LED_LED_COUNT); //全亮状态
  Led.Flag = 0x02;  //预置系统为正常
  //Led.Timer = LED_FLASH_TIMER_COUNT * 2;//系统预热准备
  Led_cbAppendInit(); //附加初始化
}

//-------------------------------任务函数-----------------------------
//放入16ms进程中扫描,以获得闪动效果,也可直接调用更新数据
void Led_Task(void)
{
  //判断闪动时,在显示还是在闪动周期
  unsigned char Disp;
  if(Led.Flag & LED_FLASH_NOW){//闪动立即
    if(Led.Flag & LED_FLASH_DIS) Disp = 0;
    else Disp = 0xff;
  }
  else if((Led.Flag & LED_UPDATE_NOW) || (!Led.Timer)){//立即更新或定时更新到
    if(Led.Flag & LED_DISP) Disp = 0;
    else Disp = 0xff;    
  }
  else{//定时未到,不刷新
    Led.Timer--;
    return; 
  }
  Led.Flag &= ~(LED_UPDATE_NOW | LED_FLASH_NOW | LED_FLASH_DIS); //取消相关标志  
  //Led.Flag |= 0x02;  //预置系统为正常  
  Led.Timer = LED_FLASH_TIMER_COUNT;//重新开始

  unsigned char DotDisp = Led.DotDisp;
  if(Disp){//在显示周期
    Led.Flag |= LED_DISP;
    DotDisp |= Led.DotFlash; //置闪动显示的小数点
  }
  else{//消隐周期
    Led.Flag &= ~LED_DISP;
    Disp = ~Led.SegFlash; //闪动的不显示
    DotDisp &= ~Led.DotFlash; //去掉闪动显示的小数点  
  }
  Led.Flag &= ~LED_UPDATE_NOW;//取消立即更新标志
  
  //各个数码管分别送显
  unsigned char PosShift = 0x01;
  unsigned char Pos = 0;
  for(; Pos < LED_LED_COUNT; Pos++, PosShift <<= 1){
    unsigned char Data;
    if(Disp & PosShift){//段显了
      Data = Led.SegDisp[Pos] & (~LED_DOT_BUF_MASK);  //小数点不取
      if(Pos >= (LED_LED_COUNT - LED_SEG_FLASH_COUNT)){//独立段闪动控制时
        if(!(Led.Flag & LED_DISP)) //灭周期,闪动的消隐
          Data &= ~Led.SegGrpFlash[Pos - (LED_LED_COUNT - LED_SEG_FLASH_COUNT)];
        else //亮周期,闪动的强制显示
          Data |= Led.SegGrpFlash[Pos - (LED_LED_COUNT - LED_SEG_FLASH_COUNT)];
      }
    }
    else Data = 0;
    if(DotDisp & PosShift) //小数点显了
      Data |= LED_DOT_BUF_MASK;
    
    #ifndef SUPPORT_LCD
      Data &= ~ Led.DisBuf[Pos]; //去掉禁止更新的段
      Data |= Led_cbGetBuf(Pos) & Led.DisBuf[Pos]; //加上原缓冲区中的段显示
    #endif
    Led_cbSetBuf(Pos, Data);
  }
  Led_cbAppendUpdate(Led.Flag);  //附加功能任务(LCD除段码外的其它段更新)
}

