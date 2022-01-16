/***************************************************************************

          Ƶ�ʿɿط�����ģ��ײ������ӿ�-��ASM32��Ƭ����ʱ���е�ʵ��
//ʹ�õ��������� ���Ƶ�ʷ�Χ10K
****************************************************************************/
#include "BuzzerDrv.h"
#include "IoCtrl.h"
#ifndef SUPPORT_DIS_BUZZER_DRV      //֧��ʱ


#ifndef BZ_TIM   //Ĭ�϶���ʱ��T9
  #define BZ_TIM     TIM9  
  #define BZ_TIM_ClockEn() do{RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; }while(0)
#endif

/***************************************************************************
                          ��غ���ʵ��
****************************************************************************/

//------------------------------��ʼ������---------------------------------
void BuzzerDrv_Init(void)
{
  CfgBZ();          //IO��ʱ������
  BZ_CfgPWMOut();   //�ⲿʵ��
}

//-----------------------------��������-------------------------------------
//�β�ΪƵ��,0��ʾ��ʱ����
//ע��Ƶ�ʷ�Χ��ֱ�����Ӳ�����,ע��ֱ��ʵĲ������Զ�������Ӱ��
void BuzzerDrv_Start(unsigned short Hz)
{
  if(Hz == 0){//�ر�
    BuzzerDrv_Stop();
    return;
  }
  
  //����Ԥ��Ƶֵ :���Ƶ�� = SYS_MHZ / (��Ƶֵ * ��ʱ��ֵ)
  unsigned long Clk = (unsigned long)Hz << 15;
  unsigned char Div;
  if(Clk < ((unsigned long)SYS_MHZ * 1000000)) 
    Div = ((unsigned long)SYS_MHZ * 1000000) / Clk;
  else Div = 1;
  BZ_TIM->PSC = Div - 1;

  //���㲢��䶨ʱ��ֵ,��������ʱ��
  unsigned short Pr = ((unsigned long)SYS_MHZ * 1000000) / Div / Hz;
	BZ_TIM->ARR = Pr;			     //�趨�������Զ���װֵ 
	BZ_TIM->BZ_CCR = Pr >> 1; //��Ƶֵ��Ϊ50%
  #ifdef BZ_CCRN //�����и�����ʱ(�����跴��)
  	BZ_TIM->BZ_CCRN = Pr >> 1; //��Ƶֵ��Ϊ50%
  #endif
  BZ_TIM->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;    //������ʱ��������Ԥװ��
  EnOutBZ(); //�������
}

//-----------------------------ֹͣ����-------------------------------------
void BuzzerDrv_Stop(void)
{
  BZ_TIM->CR1 = 0;//�رն�ʱ��
  DisOutBZ();//��ֹ���
}

//-----------------------------�Ƿ���-------------------------------------
unsigned char BuzzerDrv_IsStart(void)
{
  return BZ_TIM->CR1 & TIM_CR1_CEN;
}

#endif //#ifndef SUPPORT_DIS_BUZZER_DRV      //֧��ʱ
