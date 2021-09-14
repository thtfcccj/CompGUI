/***************************************************************************

            LedDrv1652��ʹ��HC32f00xUsartʱ��ʵ��
HC32f00xUsartû�м�żУ��λ�����ֹ�����
***************************************************************************/
#include "LedDrv1652.h"
#include "CMSIS.h"
#include "IoCtrl.h"

#ifndef pUsartHw  //Ӳ��USARTָ��, M0P_UART_TypeDef�ṹ��Ĭ��USART1(��Ӧ��TIM1)
  #define pUsartHw     M0P_UART1      
  #define pTimer  M0P_TIM1 
  #define UART_IRQHandler  UART1_IRQHandler  //�ж����
  #define UART_IRQn  UART1_IRQn
#endif

unsigned char _SendLen = 0;//�������ݳ���
unsigned char _SendPos = 0;//����λ��

//-----------------------------Ӳ����ʼ��------------------------------------
void LedDrv1652_cbHwInit(void)
{
  CfgIo_Usart1652();//IO����
  
  M0P_SYSCTRL->PERI_CLKEN_f.BASETIM = 1;//TIM012 ģ��ʱ��ʹ��  
  //���ö�ʱ��Ϊ����19200������
  //���Ϲ�ʽ: Bandrate = (����ȡ1*FpClk) / (overȡ32 * SCNT)
  //foscΪFpClkʱ��
  pTimer->ARR = 65536 - ((unsigned long)SYS_MHZ * 1000000)/ 16 / 19200;
  pTimer->CR = (1 << 0) | (1 << 1); //�Զ���װ(b1),��������ʱ��(b0)
  //����ģʽ3(11λ,TB8(b3)����У��λ0),����ʱ�ӣ�ͬʱ�ر�����
  pUsartHw->SCON = (3 << 6) | (1 << 9); 
	//pUsartHw->ISR = 0;//���״̬�Ĵ���ֵ
  pUsartHw->SCON |= 0x02;//�ϵ缴��������ж�ʹ��(������дBUF�����ж�)
  
  NVIC_ClearPendingIRQ(UART_IRQn); //������ж�
  NVIC_EnableIRQ(UART_IRQn);       //�����ж�
  NVIC_SetPriority(UART_IRQn, 2);  //�����ȼ�(��ռʽԽСԽ��)
}

//------------------------------���TB8---------------------------------------
static void _FullTB8(void)
{
  //����У����
  unsigned char Data = LedDrv1652.CommBuf[_SendPos];
  unsigned char OddCount = 0;
  for(unsigned char i = 0; i < 8; i++){
    if(Data & 0x01) OddCount++;
    Data >>= 1;
  }
  if(OddCount & 0x01)//1����Ϊ���������0
    pUsartHw->SCON &= ~(1 << 3);
  else pUsartHw->SCON |= (1 << 3); //���1
}


//------------------------------��������----------------------------------------
//LedDrv1652.CommBuf���Ҫ���͵�����
void LedDrv1652_cbSendStart(unsigned char SendSize)
{
  _SendLen = SendSize;
  _SendPos = 0;
  _FullTB8(); //����У��λ
  pUsartHw->SBUF = LedDrv1652.CommBuf[0];//װ������׼������
  //pUsartHw->SCON |= 0x02;//��������ж�ʹ��
}

//----------------------------�жϴ�����---------------------------------------
void UART_IRQHandler (void)
{
  pUsartHw->ICR &= ~0x02; //�巢������ж�
  
  _SendPos++; //��д�뻺����һ������
  if(_SendPos < _SendLen){//δ�������
    _FullTB8(); //����У��λ
    pUsartHw->SBUF = LedDrv1652.CommBuf[_SendPos];//����������
  }
  else{//���������
    //pUsartHw->SCON &= ~0x02;//���ж�
    LedDrv1652.Timer = 3; //ͨѶ���,������3msʱ��
  }
}



