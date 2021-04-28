/***************************************************************************
              红外接收程序的中断-在HC32中的高级定时器中实现
此模块实现与单片机相关,用于辅助Ir_HC32完成数据接收,并完成里面的回调函数
使用硬件铺获A(匹配寄存器A)
***************************************************************************/

#include "IrDrv.h"
#include "IOCtrl.h"

#include "HC32.h"
#include "Delay.h"


/***************************************************************************
                            内部配置
****************************************************************************/
//IoCtrl里需定义:

//#define IR_TIM        M0P_TIM4    //指定定时器
//#define IR_CH         0            //通道号, A通道时为0, B通道时为1
//定时器相关：配置其分频与溢出值，以保证溢出时间为10ms
//#define IR_CKDIV      3           //分频值,<4其移位值,5:64分,6:256分,7:1024分
//#define IR_PERAR      30000      //周期溢出值
//中断相关: 
//#define IR_TIM_IRQHandler TIM4_IRQHandler //定义中断函数宏
//#define IR_TIM_IRQ  TIM4_IRQn             //中断编号

/***************************************************************************
                          相关函数实现
****************************************************************************/

struct _Ir_Hc32{
  unsigned short IdieWidth;     //缓冲无Ir载波时的脉宽，0时表示没有该缓冲即在有信号周期
  unsigned short WidthAppend;   //用于补偿中断影响引起的计数丢失
};

struct _Ir_Hc32 Ir_Hc32;

#ifdef IR_HW_DEBUG
  struct {
    unsigned char Pos;
    unsigned short IdieWidth[IR_DRV_CODE_SIZE];
    unsigned short Width[IR_DRV_CODE_SIZE];
    unsigned short Next[IR_DRV_CODE_SIZE];    
  }_Ir_Debug;
#endif

//--------------------------通道相关的配置-----------------------------
#if (IR_CH == 0)
  //配置为上升沿捕获：硬件捕获A条件4：CHxA 端口上采样到上升沿
  #define _CfgRiseCap() do{IR_TIM->HCPAR =  (1 << 4);}while(0) 
  //配置为下降沿捕获：硬件捕获A条件5：CHxA 端口上采样到下降沿
  #define _CfgFailCap() do{IR_TIM->HCPAR =  (1 << 5);}while(0)
#else           
  //配置为上升沿捕获：硬件捕获A条件6：CHxB 端口上采样到上升沿
  #define _CfgRiseCap() do{IR_TIM->HCPAR =  (1 << 6);}while(0) 
  //配置为下降沿捕获：硬件捕获A条件7：CHxB 端口上采样到下降沿
  #define _CfgFailCap() do{IR_TIM->HCPAR =  (1 << 7);}while(0) 
#endif

//---------------------IrDrv硬件初始化回调函数实现-----------------------------
void IrDrv_cbHwInit(void)
{
  CfgIR();  
  
  //1. TIM456 模块时钟使能
  M0P_SYSCTRL->PERI_CLKEN_f.ADVTIM = 1;
  
  //2.配置定时器计数相关设置(10mS溢出一次),定时器时钟设置为1MHZ，每次计数时间为1uS
  IR_TIM->PERAR = IR_PERAR - 1;          //定时周期
  IR_TIM->GCONR_f.CKDIV = IR_CKDIV; //分频为配置，其它默认(减计数,A波)
  
  //3.Timer通道输入捕获相关设置
  #if(IR_DRV_VALID_LEVEL == 0) //低电平为有效载波时,置为下降沿检测空无信号时间
    _CfgFailCap();    
  #else
    _CfgRiseCap(); 
  #endif
    
  IR_TIM->PCONR =  (1 << 0);     //捕获输入功能A(b0) 
  
  //4.Timer3中断优先级相关设置	
  NVIC_ClearPendingIRQ(IR_TIM_IRQ); //清挂起中断
  NVIC_EnableIRQ(IR_TIM_IRQ);       //允许中断
  NVIC_SetPriority(IR_TIM_IRQ, 1);  //置优先级(抢占式越小越高)
  
  //5.启动定时器
  IR_TIM->ICONR = (1 << 6) | (1 << 0);//上溢中断使能(b5)与计数匹配中断使能A
  IR_TIM->GCONR_f.START = 1;    //开启定时器
}

//-----------------------中断处理程序----------------------------
void IR_TIM_IRQHandler(void)
{
  //计数溢出中断部分
  if(IR_TIM->IFR_f.OVFF){ //上溢中断了
    IrDrv_IRQ(65535, 0);       //一周结束
    Ir_Hc32.IdieWidth = 0;     //切换为识别Idie模式
    IR_TIM->ICONR_f.INTENOVF = 0;//暂关闭溢出中断(首个IR周期时，可能很长(>10ms的都有))
    #if(IR_DRV_VALID_LEVEL == 0) //低电平为有效载波时,置为下降沿检测空无信号时间
      _CfgFailCap();    
    #else
      _CfgRiseCap(); 
    #endif
    #ifdef IR_HW_DEBUG
      _Ir_Debug.Pos = 0;
    #endif
  }

  //捕获中断部分
  if(IR_TIM->IFR_f.CMAF){  
    //立即记住当前定时值并复位
    unsigned short WidthAppend = IR_TIM->CNTER; 
    unsigned short Width = IR_TIM->GCMAR; 
    
    do{//同步才能关闭
      IR_TIM->GCONR_f.START = 0;    //关闭定时器
      IR_TIM->CNTER = 0;
    }while(IR_TIM->CNTER);
    IR_TIM->GCONR_f.START = 1;    //开启定时器
    #ifdef IR_HW_DEBUG
      _Ir_Debug.Next[_Ir_Debug.Pos] = IR_TIM->CNTER;
    #endif  
 
    //准备宽度数据
    WidthAppend -= Width;//由中断到当前为已切换回的时间,进行补偿
    Width -= 0;
    Width += Ir_Hc32.WidthAppend; //自已的被偿
    Ir_Hc32.WidthAppend = WidthAppend;
    
    //检测到空闲周期时(当前在Ir检测中)
    if(!Ir_Hc32.IdieWidth){
      Ir_Hc32.IdieWidth = Width;
      #if(IR_DRV_VALID_LEVEL == 0) //低电平为有效载波时,置为上升沿检测有信号时间
        _CfgRiseCap();//设置为上升沿捕捕获：CC1P=0     
      #else
        _CfgFailCap(); 
      #endif 
        
      #ifdef IR_HW_DEBUG
        _Ir_Debug.IdieWidth[_Ir_Debug.Pos] = Width;
      #endif
    }
    //检测Ir周期时(当前在空闲周期检测中)  
    else{ 
      #if(IR_DRV_VALID_LEVEL == 0) //低电平为有效载波时,置为下降沿检测空无信号时间
        _CfgFailCap();    
      #else
        _CfgRiseCap(); 
      #endif
      #ifdef IR_HW_DEBUG
        _Ir_Debug.Width[_Ir_Debug.Pos] = Width;
        if(_Ir_Debug.Pos < (IR_DRV_CODE_SIZE - 1)) _Ir_Debug.Pos++;
      #endif  
      IrDrv_IRQ(Ir_Hc32.IdieWidth, Width);  //数据处理  
      Ir_Hc32.IdieWidth = 0;//切换为识别空闲周期模式
      if(!IR_TIM->ICONR_f.INTENOVF){//首个IR周期时,可能很长(>10ms的都有),期间不复位
        IR_TIM->ICONR_f.INTENOVF = 1;//开启溢出中断
      }
    }			 		     	    					   
  }
  IR_TIM->ICLR = 0; //`((1 << 6) | (1 << 0));//清除溢出中断与捕获中断标志
}
