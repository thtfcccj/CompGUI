/***************************************************************************

            LedDrv1652在使用HC32f00xUsart时的实现
HC32f00xUsart没有寄偶校验位，需手工计算
***************************************************************************/
#include "LedDrv1652.h"
#include "CMSIS.h"
#include "IoCtrl.h"

#ifndef pUsartHw  //硬件USART指针, M0P_UART_TypeDef结构，默认USART1(对应用TIM1)
  #define pUsartHw     M0P_UART1      
  #define pTimer  M0P_TIM1 
  #define UART_IRQHandler  UART1_IRQHandler  //中断入口
  #define UART_IRQn  UART1_IRQn
#endif

unsigned char _SendLen = 0;//发送数据长度
unsigned char _SendPos = 0;//发送位置

//-----------------------------硬件初始化------------------------------------
void LedDrv1652_cbHwInit(void)
{
  CfgIo_Usart1652();//IO配置
  
  M0P_SYSCTRL->PERI_CLKEN_f.BASETIM = 1;//TIM012 模块时钟使能  
  //配置定时器为产生19200波特率
  //资料公式: Bandrate = (倍率取1*FpClk) / (over取32 * SCNT)
  //fosc为FpClk时钟
  pTimer->ARR = 65536 - ((unsigned long)SYS_MHZ * 1000000)/ 16 / 19200;
  pTimer->CR = (1 << 0) | (1 << 1); //自动重装(b1),并开启定时器(b0)
  //工作模式3(11位,TB8(b3)用作校验位0),两倍时钟，同时关闭所有
  pUsartHw->SCON = (3 << 6) | (1 << 9); 
	//pUsartHw->ISR = 0;//清除状态寄存器值
  pUsartHw->SCON |= 0x02;//上电即发送完成中断使能(后续不写BUF不会中断)
  
  NVIC_ClearPendingIRQ(UART_IRQn); //清挂起中断
  NVIC_EnableIRQ(UART_IRQn);       //允许中断
  NVIC_SetPriority(UART_IRQn, 2);  //置优先级(抢占式越小越高)
}

//------------------------------填充TB8---------------------------------------
static void _FullTB8(void)
{
  //计算校验结果
  unsigned char Data = LedDrv1652.CommBuf[_SendPos];
  unsigned char OddCount = 0;
  for(unsigned char i = 0; i < 8; i++){
    if(Data & 0x01) OddCount++;
    Data >>= 1;
  }
  if(OddCount & 0x01)//1个数为寄数，填充0
    pUsartHw->SCON &= ~(1 << 3);
  else pUsartHw->SCON |= (1 << 3); //填充1
}


//------------------------------发送数据----------------------------------------
//LedDrv1652.CommBuf存放要发送的数据
void LedDrv1652_cbSendStart(unsigned char SendSize)
{
  _SendLen = SendSize;
  _SendPos = 0;
  _FullTB8(); //填充寄校验位
  pUsartHw->SBUF = LedDrv1652.CommBuf[0];//装载数据准备发送
  //pUsartHw->SCON |= 0x02;//发送完成中断使能
}

//----------------------------中断处理函数---------------------------------------
void UART_IRQHandler (void)
{
  pUsartHw->ICR &= ~0x02; //清发送完成中断
  
  _SendPos++; //已写入缓冲区一个数了
  if(_SendPos < _SendLen){//未发送完成
    _FullTB8(); //填充寄校验位
    pUsartHw->SBUF = LedDrv1652.CommBuf[_SendPos];//继续发送数
  }
  else{//发送完成了
    //pUsartHw->SCON &= ~0x02;//关中断
    LedDrv1652.Timer = 3; //通讯完成,但需间隔3ms时间
  }
}



