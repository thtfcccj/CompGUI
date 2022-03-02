/***************************************************************************

          频率可控蜂鸣器模块底层驱动接口-在HC32F4 TIMERA中的实现
//此实现为使用单向驱动
****************************************************************************/

#include "BuzzerDrv.h"
#include "IoCtrl.h"

#ifndef TIMb   //默认定义时用M4_TMRA1
  #define TIMb     M4_TMRA1  
  #define TIMb_ClockEn() do{M4_MSTP->FCG2_f.TIMERA_1 = 1; }while(0)
#endif

/***************************************************************************
                          相关函数实现
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerDrv_Init(void)
{
	TIMb_ClockEn(); 	                  //TIMb时钟使能    	  
  CfgBuzzerIo();                      //PWM对应引脚设置为输出
}

//-----------------------------启动函数-------------------------------------
//形参为频率,0表示暂时不响
//注：频率范围与分辨率与硬件相关,注意分辨率的不连续性对声音的影响
void BuzzerDrv_Start(unsigned short Hz)
{
  if(Hz == 0){//暂时不响
    BuzzerDrv_Stop();
    return;
  }

  //计算预分频值 :输出频率 = (SYS_MHZ >> 分频值) / 定时器值
  unsigned char Shift = 0;
  unsigned long Pr;
  for(; Shift < 10; Shift++){
    Pr = (((unsigned long)SYS_MHZ * 1000000) >> Shift) / Hz;
    if(Pr <= 0xffff) break;
  }
	TIMb->PERAR = Pr;			            //设定计数器自动重装值 
  Pr >>= 1;//分频值设为50%
	TIMb->CMPARn = Pr; 
  TIMb->BCSTR = (Shift << 4) | 1; //置分频值，并开始计时
  EnOutBZ();
}

//-----------------------------停止函数-------------------------------------
void BuzzerDrv_Stop(void)
{
  TIMb->BCSTR = 0;//关闭定时器
  DisOutBZ();  //关闭输出
}

//-----------------------------是否开启-------------------------------------
unsigned char BuzzerDrv_IsStart(void)
{
  return (TIMb->BCSTR & 1);
}


