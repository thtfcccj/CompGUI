/***************************************************************************

          Ƶ�ʿɿط�����ģ��ײ������ӿ�-��STM32��Ƭ����ʱ���е�ʵ��
//ʹ��˫�������������ѹ�ȼ�:��V-G,G-V����
****************************************************************************/

#include "BuzzerDrv.h"
#include "IoCtrl.h"
#include "misc.h"

#ifndef TIMb   //Ĭ�϶���ʱ��T9
  #define TIMb     TIM9  
  #define TIMb_ClockEn() do{RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; }while(0)
#endif

/***************************************************************************
                          ��غ���ʵ��
****************************************************************************/

//------------------------------��ʼ������---------------------------------
void BuzzerDrv_Init(void)
{
	TIMb_ClockEn(); 	                  //TIMbʱ��ʹ��    	  
	TIMb->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;  //CCP1->OCxM1λ�������ģʽΪ�������ĵ�ƽ(OCxM=011)
	TIMb->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  //CCP2->OCxM2λ�������ģʽΪ�������ĵ�ƽ(OCxM=011)
  TIMb->CCMR1 &= ~TIM_CCMR1_OC1PE |TIM_CCMR1_OC2PE;  //CCR1/2�Ĵ���ȡ��Ԥװ��ģʽ,ֱ����Ч  

   CfgBuzzerIo();//PWM��Ӧ��������Ϊ���
}

//-----------------------------��������-------------------------------------
//�β�ΪƵ��,0��ʾ��ʱ����
//ע��Ƶ�ʷ�Χ��ֱ�����Ӳ�����,ע��ֱ��ʵĲ������Զ�������Ӱ��
void BuzzerDrv_Start(unsigned short Hz)
{
  if(Hz == 0){//��ʱ����
    BuzzerDrv_Stop();
    return;
  }
  
  //����Ԥ��Ƶֵ :���Ƶ�� = SYS_MHZ / (��Ƶֵ * ��ʱ��ֵ)
  unsigned long Clk = (unsigned long)Hz << 15;
  unsigned char Div;
  if(Clk < ((unsigned long)SYS_MHZ * 1000000)) 
    Div = ((unsigned long)SYS_MHZ * 1000000) / Clk;
  else Div = 1;
  TIMb->PSC = Div - 1;

  //���㲢��䶨ʱ��ֵ,��������ʱ��
  unsigned short Pr = ((unsigned long)SYS_MHZ * 1000000) / Div / Hz;
	TIMb->ARR = Pr;			            //�趨�������Զ���װֵ 
  Pr >>= 1;//��Ƶֵ��Ϊ50%
	TIMb->CCR1 = Pr; 
	TIMb->CCR2 = Pr;
	TIMb->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC2E;//�������෴
  TIMb->CR1 = TIM_CR1_CEN; //��ʼ��ʱ
}

//-----------------------------ֹͣ����-------------------------------------
void BuzzerDrv_Stop(void)
{
  TIMb->CR1 = 0;//�رն�ʱ��
	TIMb->CCER = 0;//�ر����
  //ClrBuzzerIo();//����Ϊ�͵�ƽ�޵������
}

//-----------------------------�Ƿ���-------------------------------------
unsigned char BuzzerDrv_IsStart(void)
{
  return (TIMb->CR1 & TIM_CR1_CEN);
}


