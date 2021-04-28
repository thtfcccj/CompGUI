/***************************************************************************
                       红外接收程序的中断-在STM32中的实现
此模块实现与单片机相关,用于辅助Ir_STM32完成数据接收,并完成里面的回调函数
使用定时器3通道1
***************************************************************************/
#include "IrDrv.h"
#include "IOCtrl.h"

#ifdef STM32F0XX     //使用STM32F0库时
  #include "stm32f0xx.h"
#else
  #include "STM32.h"
#endif

struct _Ir_Stm32{
  unsigned short IdieWidth;     //缓冲无Ir载波时的脉宽，0时表示没有该缓冲即在有信号周期
  unsigned short WidthAppend;   //用于补偿中断影响引起的计数丢失
};

struct _Ir_Stm32 Ir_Stm32;


void NVIC_Init(unsigned char Priority,unsigned char IRQChannel)
{
  uint32_t temp = 0x00;
  temp = NVIC->IP[IRQChannel >> 0x02];
  temp &= (~(0xFF << ((IRQChannel & 0x03) * 8)));
  temp |= (((Priority << 6) & 0xFF) << ((IRQChannel & 0x03) * 8));    
  NVIC->IP[IRQChannel >> 0x02] = temp;
  NVIC->ISER[0] = 0x01 << (IRQChannel & 0x1F);
}

//---------------------IrDrv硬件初始化回调函数实现-----------------------------
void IrDrv_cbHwInit(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;//开Timer3时钟	

  CfgIR();

  //2.配置Timer3计数相关设置(10mS溢出一次),定时器时钟设置为1MHZ，每次计数时间为1uS
  TIM3->ARR = (10000-1);//定时器自动重装值，计数8000次就是8mS
  TIM3->PSC |= (48-1);//对系统时钟源(48MHZ)分频数，48分频（1MHZ）

  //3.Timer3通道1输入捕获相关设置
  TIM3->CCMR1 |= 1<<0;//IC1映射在TI1上,无滤波采样，每个沿执行一次中断事件
  TIM3->CCER |= 1<<0;//输入捕获使能，上升沿捕获使能
  TIM3->DIER |= 0x03;//允许CC1 中断、更新中断
  //4.Timer3中断优先级相关设置	
  NVIC_Init(1,TIM3_IRQn);//Timer3中断配置

  //5.开启Timer3
  TIM3->CR1 |=  1<<0;//使能定时器3，向上计数模式
}

//-----------------------中断处理程序----------------------------
void TIM3_IRQHandler(void)
{
  //计数溢出中断部分
  if(TIM3->SR & (1<<0))
  { 
    IrDrv_IRQ(65535, 0);    //一周结束
    Ir_Stm32.IdieWidth = 0; //切换为识别Idie模式
    TIM3->CNT = 0;
    TIM3->DIER &= (unsigned char)~0x01;//暂关闭溢出中断(首个IR周期时，可能很长(>10ms的都有))
  }

  //捕获中断部分
  if(TIM3->SR & (1<<1))
  {	  
    //立即记住当前定时值并复位
    unsigned short WidthAppend = TIM3->CNT; 
    TIM3->CNT = 0;
    unsigned short Width = TIM3->CCR1;  
    
    //准备宽度数据
    WidthAppend -= Width;//由中断到当前为已切换回的时间,进行被偿
    Width -= 0;
    Width += Ir_Stm32.WidthAppend; //自已的被偿
    Ir_Stm32.WidthAppend = WidthAppend;
    
    //检测到空闲周期时(当前在Ir检测中)
    if(!Ir_Stm32.IdieWidth){
      Ir_Stm32.IdieWidth = Width;
      #if(IR_DRV_VALID_LEVEL == 0) //低电平为有效载波时,置为上升沿检测有信号时间
        TIM3->CCER &= ~(1<<1);//设置为上升沿捕捕获：CC1P=0     
      #else
        TIM3->CCER |= (1<<1); 
      #endif    
    }
    //检测Ir周期时(当前在空闲周期检测中)  
    else
    { 
      #if(IR_DRV_VALID_LEVEL == 0) //低电平为有效载波时,置为下降沿检测空无信号时间
         TIM3->CCER |= (1<<1);    
      #else
        TIM3->CCER &= ~(1<<1); 
      #endif
      IrDrv_IRQ(Ir_Stm32.IdieWidth, Width);  //数据处理  
      Ir_Stm32.IdieWidth = 0;//切换为识别空闲周期模式
      if(!(TIM3->DIER & 1)){//首个IR周期时,可能很长(>10ms的都有),期间不复位
        TIM3->DIER |= 0x01;//开启溢出中断
      }
    }			 		     	    					   
  }
  TIM3->SR &= ~((1<<1)|(1<<0));//清除溢出中断与捕获中断标志
}
