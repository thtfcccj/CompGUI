/***************************************************************************

          频率可控蜂鸣器模块底层驱动接口-在ASM32单片机定时器中的实现
//使用单向驱动， 最大频率范围10K
****************************************************************************/
#include "BuzzerDrv.h"
#include "IoCtrl.h"
#ifndef SUPPORT_DIS_BUZZER_DRV      //支持时


#ifndef BZ_TIM   //默认定义时用T9
  #define BZ_TIM     TIM9  
  #define BZ_TIM_ClockEn() do{RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; }while(0)
#endif

/***************************************************************************
                          相关函数实现
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerDrv_Init(void)
{
  CfgBZ();          //IO与时钟配置
  BZ_CfgPWMOut();   //外部实现
}

//-----------------------------启动函数-------------------------------------
//形参为频率,0表示暂时不响
//注：频率范围与分辨率与硬件相关,注意分辨率的不连续性对声音的影响
void BuzzerDrv_Start(unsigned short Hz)
{
  if(Hz == 0){//关闭
    BuzzerDrv_Stop();
    return;
  }
  
  //计算预分频值 :输出频率 = SYS_MHZ / (分频值 * 定时器值)
  unsigned long Clk = (unsigned long)Hz << 15;
  unsigned char Div;
  if(Clk < ((unsigned long)SYS_MHZ * 1000000)) 
    Div = ((unsigned long)SYS_MHZ * 1000000) / Clk;
  else Div = 1;
  BZ_TIM->PSC = Div - 1;

  //计算并填充定时器值,并开启定时器
  unsigned short Pr = ((unsigned long)SYS_MHZ * 1000000) / Div / Hz;
	BZ_TIM->ARR = Pr;			     //设定计数器自动重装值 
	BZ_TIM->BZ_CCR = Pr >> 1; //分频值设为50%
  #ifdef BZ_CCRN //配置有负极性时(配置需反向)
  	BZ_TIM->BZ_CCRN = Pr >> 1; //分频值设为50%
  #endif
  BZ_TIM->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;    //开启定时器并启用预装载
  EnOutBZ(); //允许输出
}

//-----------------------------停止函数-------------------------------------
void BuzzerDrv_Stop(void)
{
  BZ_TIM->CR1 = 0;//关闭定时器
  DisOutBZ();//禁止输出
}

//-----------------------------是否开启-------------------------------------
unsigned char BuzzerDrv_IsStart(void)
{
  return BZ_TIM->CR1 & TIM_CR1_CEN;
}

#endif //#ifndef SUPPORT_DIS_BUZZER_DRV      //支持时
