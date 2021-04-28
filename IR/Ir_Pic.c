/***************************************************************************
                       红外接收程序有中断在Pic(特指PIC16)中的实现
使用TIMER1+ECCP1引脚(SSOP28时为13脚)实现
//注:因中断问题，中断处理程序放在其它地方引起IR异常，故将中断程序放在了此模块里
***************************************************************************/

#include "IOCtrl.h"
#include <string.h>
#include <pic.h>
#include "PicBit.h"
#include "IRQ_Pic.h"

#include "IrDrv.h"

struct _Ir_Pic{
  unsigned short IdieWidth;   //缓冲无Ir载波时的脉宽,0时表示没有该缓冲即在有信号周期
  unsigned short WidthAppend;//用于补偿中断影响引起的计数丢失
};
struct _Ir_Pic  Ir_Pic;


//定义定时器初值,用于定时器溢出处理,定时器溢出时间规定为8ms
#define    TMR1_INIT        0

/***************************************************************************
                              IR中断相关程序
***************************************************************************/
//---------------------IrDrv硬件初始化回调函数实现-----------------------------
void IrDrv_cbHwInit(void)
{
  Ir_Pic.IdieWidth = 0;
  Ir_Pic.WidthAppend = 0;
  
  CfgIR();//初始化IO口

  //配置Timer1为定时器
  T1CON = 0; //使用指令时8MHZ,不分频一个溢出中断为8mS(关闭门控模式)
  TMR1 = TMR1_INIT; //置初值

  #if(IR_DRV_VALID_LEVEL != 0) //高电平为有效载波时,先置为上沿中断,否则下降沿
    CCP1CON = PICB_CCP1M2 | PICB_CCP1M0;//0101= 捕捉模式：每个上升沿捕捉一次
  #else
    CCP1CON = PICB_CCP1M2 | 0;//0100= 捕捉模式：每个下降沿捕捉一次
  #endif
  PIR1 &= ~(PICB_TMR1IF | PICB_CCP1IF); //清中断
  PIE1 |= (PICB_TMR1IE | PICB_CCP1IE); //定时器1中断与/捕获中断允许

  T1CON |= PICB_TMR1ON; //开始启用定时器
}

//----------------------定时器溢出中断处理程序----------------------------
//定时器 / 计数器1 溢出
void Timer_Ov_int(void)
{
  IrDrv_IRQ(65535, 0);  //一周期结束
  Ir_Pic.IdieWidth = 0;//切换为识别Idie模式
  TMR1 = TMR1_INIT;   //立即复位
  PIE1 &= ~PICB_TMR1IE;    //暂关闭溢出中断(首个IR周期时,可能很长(>10ms的都有))
  PIR1 &= ~PICB_TMR1IF;     //最后清中断
}

//定时器 / 计数器1事件捕捉
void Timer_Capt_int(void)
{
  //立即记住当前定时值并复位
  unsigned short WidthAppend = TMR1; 
  TMR1 = TMR1_INIT;
  unsigned short Width = CCPR1L | ((unsigned short)CCPR1H << 8);//CCPR1; //捕捉到的值
  
  //准备宽度数据
  WidthAppend -= Width;//由中断到当前为已切换回的时间,进行被偿
  Width -= TMR1_INIT;
  Width += Ir_Pic.WidthAppend; //自已的被偿
  Ir_Pic.WidthAppend = WidthAppend;
  
  //检测到空闲周期时(当前在Ir检测中)
  if(!Ir_Pic.IdieWidth){
    Ir_Pic.IdieWidth = Width;
    #if(IR_PIC_IR_LEVEL == 0) //低电平为有效载波时,置为上升沿检测有信号时间
      CCP1CON = PICB_CCP1M2 | PICB_CCP1M0;//0101= 捕捉模式：每个上升沿捕捉一次
    #else
      CCP1CON = PICB_CCP1M2 | 0;//0100= 捕捉模式：每个下降沿捕捉一次
    #endif
  }
  else{ //检测Ir周期时(当前在空闲周期检测中)
    #if(IR_PIC_IR_LEVEL == 0) //低电平为有效载波时,置为下降沿检测空无信号时间
      CCP1CON = PICB_CCP1M2 | 0;//0100= 捕捉模式：每个下降沿捕捉一次
    #else
      CCP1CON |= PICB_CCP1M2 | PICB_CCP1M0;//0101= 捕捉模式：每个上升沿捕捉一次
    #endif
    IrDrv_IRQ(Ir_Pic.IdieWidth, Width);  //数据处理  
    Ir_Pic.IdieWidth = 0;//切换为识别空闲周期模式
    if(!(PIE1 & PICB_TMR1IE)){//首个IR周期时,可能很长(>10ms的都有),期间不复位
      PIR1 &= ~PICB_TMR1IF;//清除溢出中断标志
      PIE1 |= PICB_TMR1IE;//开启溢出中断
    }
  }
  PIR1 &= ~PICB_CCP1IF;     //最后清中断
}

