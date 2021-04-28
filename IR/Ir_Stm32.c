/***************************************************************************
                       ������ճ�����ж�-��STM32�е�ʵ��
��ģ��ʵ���뵥Ƭ�����,���ڸ���Ir_STM32������ݽ���,���������Ļص�����
ʹ�ö�ʱ��3ͨ��1
***************************************************************************/
#include "IrDrv.h"
#include "IOCtrl.h"

#ifdef STM32F0XX     //ʹ��STM32F0��ʱ
  #include "stm32f0xx.h"
#else
  #include "STM32.h"
#endif

struct _Ir_Stm32{
  unsigned short IdieWidth;     //������Ir�ز�ʱ������0ʱ��ʾû�иû��弴�����ź�����
  unsigned short WidthAppend;   //���ڲ����ж�Ӱ������ļ�����ʧ
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

//---------------------IrDrvӲ����ʼ���ص�����ʵ��-----------------------------
void IrDrv_cbHwInit(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;//��Timer3ʱ��	

  CfgIR();

  //2.����Timer3�����������(10mS���һ��),��ʱ��ʱ������Ϊ1MHZ��ÿ�μ���ʱ��Ϊ1uS
  TIM3->ARR = (10000-1);//��ʱ���Զ���װֵ������8000�ξ���8mS
  TIM3->PSC |= (48-1);//��ϵͳʱ��Դ(48MHZ)��Ƶ����48��Ƶ��1MHZ��

  //3.Timer3ͨ��1���벶���������
  TIM3->CCMR1 |= 1<<0;//IC1ӳ����TI1��,���˲�������ÿ����ִ��һ���ж��¼�
  TIM3->CCER |= 1<<0;//���벶��ʹ�ܣ������ز���ʹ��
  TIM3->DIER |= 0x03;//����CC1 �жϡ������ж�
  //4.Timer3�ж����ȼ��������	
  NVIC_Init(1,TIM3_IRQn);//Timer3�ж�����

  //5.����Timer3
  TIM3->CR1 |=  1<<0;//ʹ�ܶ�ʱ��3�����ϼ���ģʽ
}

//-----------------------�жϴ������----------------------------
void TIM3_IRQHandler(void)
{
  //��������жϲ���
  if(TIM3->SR & (1<<0))
  { 
    IrDrv_IRQ(65535, 0);    //һ�ܽ���
    Ir_Stm32.IdieWidth = 0; //�л�Ϊʶ��Idieģʽ
    TIM3->CNT = 0;
    TIM3->DIER &= (unsigned char)~0x01;//�ݹر�����ж�(�׸�IR����ʱ�����ܺܳ�(>10ms�Ķ���))
  }

  //�����жϲ���
  if(TIM3->SR & (1<<1))
  {	  
    //������ס��ǰ��ʱֵ����λ
    unsigned short WidthAppend = TIM3->CNT; 
    TIM3->CNT = 0;
    unsigned short Width = TIM3->CCR1;  
    
    //׼���������
    WidthAppend -= Width;//���жϵ���ǰΪ���л��ص�ʱ��,���б���
    Width -= 0;
    Width += Ir_Stm32.WidthAppend; //���ѵı���
    Ir_Stm32.WidthAppend = WidthAppend;
    
    //��⵽��������ʱ(��ǰ��Ir�����)
    if(!Ir_Stm32.IdieWidth){
      Ir_Stm32.IdieWidth = Width;
      #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�����ؼ�����ź�ʱ��
        TIM3->CCER &= ~(1<<1);//����Ϊ�����ز�����CC1P=0     
      #else
        TIM3->CCER |= (1<<1); 
      #endif    
    }
    //���Ir����ʱ(��ǰ�ڿ������ڼ����)  
    else
    { 
      #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
         TIM3->CCER |= (1<<1);    
      #else
        TIM3->CCER &= ~(1<<1); 
      #endif
      IrDrv_IRQ(Ir_Stm32.IdieWidth, Width);  //���ݴ���  
      Ir_Stm32.IdieWidth = 0;//�л�Ϊʶ���������ģʽ
      if(!(TIM3->DIER & 1)){//�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���),�ڼ䲻��λ
        TIM3->DIER |= 0x01;//��������ж�
      }
    }			 		     	    					   
  }
  TIM3->SR &= ~((1<<1)|(1<<0));//�������ж��벶���жϱ�־
}
