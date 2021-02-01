/***************************************************************************

          频率可控蜂鸣器模块底层驱动接口-在STM32单片机定时器中的实现
//使用双向驱动以提高声压等级:即V-G,G-V交换
****************************************************************************/

#include "BuzzerDrv.h"
#include "IoCtrl.h"
#include "misc.h"

#ifndef TIMb   //默认定义时用T9
  #define TIMb     TIM9  
  #define TIMb_ClockEn() do{RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; }while(0)
#endif

/***************************************************************************
                          相关函数实现
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerDrv_Init(void)
{
	TIMb_ClockEn(); 	                  //TIMb时钟使能    	  
	TIMb->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;  //CCP1->OCxM1位配置输出模式为保持它的电平(OCxM=011)
	TIMb->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  //CCP2->OCxM2位配置输出模式为保持它的电平(OCxM=011)
  TIMb->CCMR1 &= ~TIM_CCMR1_OC1PE |TIM_CCMR1_OC2PE;  //CCR1/2寄存器取消预装载模式,直接生效  

   CfgBuzzerIo();//PWM对应引脚设置为输出
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
  
  //计算预分频值 :输出频率 = SYS_MHZ / (分频值 * 定时器值)
  unsigned long Clk = (unsigned long)Hz << 15;
  unsigned char Div;
  if(Clk < ((unsigned long)SYS_MHZ * 1000000)) 
    Div = ((unsigned long)SYS_MHZ * 1000000) / Clk;
  else Div = 1;
  TIMb->PSC = Div - 1;

  //计算并填充定时器值,并开启定时器
  unsigned short Pr = ((unsigned long)SYS_MHZ * 1000000) / Div / Hz;
	TIMb->ARR = Pr;			            //设定计数器自动重装值 
  Pr >>= 1;//分频值设为50%
	TIMb->CCR1 = Pr; 
	TIMb->CCR2 = Pr;
	TIMb->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC2E;//两极性相反
  TIMb->CR1 = TIM_CR1_CEN; //开始计时
}

//-----------------------------停止函数-------------------------------------
void BuzzerDrv_Stop(void)
{
  TIMb->CR1 = 0;//关闭定时器
	TIMb->CCER = 0;//关闭输出
  //ClrBuzzerIo();//驱动为低电平无电流输出
}

//-----------------------------是否开启-------------------------------------
unsigned char BuzzerDrv_IsStart(void)
{
  return (TIMb->CR1 & TIM_CR1_CEN);
}


