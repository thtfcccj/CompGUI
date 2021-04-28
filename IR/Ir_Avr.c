/***************************************************************************
                       红外接收程序的中断-在AVR中的实现
此模块实现与单片机相关,用于辅助Ir_Avr完成数据接收,并完成里面的回调函数
***************************************************************************/

#include "Ir.h"
#include "IrDrv.h"
#include "IOCtrl.h"

#include <string.h>
#include <ioavr.h>


struct _Ir_Avr{
  unsigned short IdieWidth;   //缓冲无Ir载波时的脉宽,0时表示没有该缓冲即在有信号周期
  unsigned short WidthAppend;//用于补偿中断影响引起的计数丢失
};

struct _Ir_Avr  Ir_Avr;


//定义定时器初值,用于定时器溢出处理,定时器溢出时间规定为8ms
#define    TCNT1_INIT        0


/***************************************************************************
                              IR中断相关
***************************************************************************/
//---------------------IrDrv硬件初始化回调函数实现--------------------------
void IrDrv_cbHwInit(void)
{
  Ir_Avr.IdieWidth = 0;
  Ir_Avr.WidthAppend = 0;  
  
  CfgIR();//初始化IO口
  
  //设置分频值,让5mS左右定时器溢出
  TCCR1B |= (1 << ICNC1) | (1 << CS10); //8MHZ时,不分频一个溢出中断为8mS
  #if(IR_VALID_LEVEL != 0) //高电平为有效载波时,先置为上沿中断,否则下降沿
    TCCR1B |=  (1 << ICES1);  
  #endif
  
  TIMSK |= (1 << TICIE1) | (1 <<  TOIE1); //同时打开溢出中断与捕获中断
}

//-----------------------中断处理程序----------------------------
//定时器 / 计数器1 溢出
#pragma vector = TIMER1_OVF_vect 
__interrupt void Timer_Ov_int(void)
{
  IrDrv_IRQ(65535, 0);  //一周结束
  Ir_Avr.IdieWidth = 0;//切换为识别Idie模式
  TCNT1 = TCNT1_INIT;  //立即复位
  TIMSK &=  ~(1 <<  TOIE1);//暂关闭溢出中断(首个IR周期时,可能很长(>10ms的都有))
}

//定时器 / 计数器1事件捕捉
#pragma vector = TIMER1_CAPT_vect 
__interrupt void Timer_Capt_int(void)
{
  //立即记住当前定时值并复位
  unsigned short WidthAppend = TCNT1; 
  TCNT1 = TCNT1_INIT;
  unsigned short Width = ICR1;  
  
  //准备宽度数据
  WidthAppend -= Width;//由中断到当前为已切换回的时间,进行被偿
  Width -= TCNT1_INIT;
  Width += Ir_Avr.WidthAppend; //自已的被偿
  Ir_Avr.WidthAppend = WidthAppend;
  
  //检测到空闲周期时(当前在Ir检测中)
  if(!Ir_Avr.IdieWidth){
    Ir_Avr.IdieWidth = Width;
    #if(IR_VALID_LEVEL == 0) //低电平为有效载波时,置为上升沿检测有信号时间
      TCCR1B |=  (1 << ICES1);     
    #else
      TCCR1B &= ~(1 << ICES1); 
    #endif    

  }
  else{ //检测Ir周期时(当前在空闲周期检测中)
    #if(IR_VALID_LEVEL == 0) //低电平为有效载波时,置为下降沿检测空无信号时间
       TCCR1B &= ~(1 << ICES1);    
    #else
      TCCR1B |=  (1 << ICES1); 
    #endif
    IrDrv_IRQ(Ir_Avr.IdieWidth, Width);  //数据处理  
    Ir_Avr.IdieWidth = 0;//切换为识别空闲周期模式
    if(!(TIMSK &  (1 <<  TOIE1))){//首个IR周期时,可能很长(>10ms的都有),期间不复位
      TIFR |=  (1 << TOV1);//清除溢出中断标志
      TIMSK |=  (1 <<  TOIE1);//开启溢出中断
    }
  }
}

#endif //#ifdef SUPPORT_IR
