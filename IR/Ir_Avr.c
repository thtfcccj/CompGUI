/***************************************************************************
                       ������ճ�����ж�-��AVR�е�ʵ��
��ģ��ʵ���뵥Ƭ�����,���ڸ���Ir_Avr������ݽ���,���������Ļص�����
***************************************************************************/

#include "Ir.h"
#include "IrDrv.h"
#include "IOCtrl.h"

#include <string.h>
#include <ioavr.h>


struct _Ir_Avr{
  unsigned short IdieWidth;   //������Ir�ز�ʱ������,0ʱ��ʾû�иû��弴�����ź�����
  unsigned short WidthAppend;//���ڲ����ж�Ӱ������ļ�����ʧ
};

struct _Ir_Avr  Ir_Avr;


//���嶨ʱ����ֵ,���ڶ�ʱ���������,��ʱ�����ʱ��涨Ϊ8ms
#define    TCNT1_INIT        0


/***************************************************************************
                              IR�ж����
***************************************************************************/
//---------------------IrDrvӲ����ʼ���ص�����ʵ��--------------------------
void IrDrv_cbHwInit(void)
{
  Ir_Avr.IdieWidth = 0;
  Ir_Avr.WidthAppend = 0;  
  
  CfgIR();//��ʼ��IO��
  
  //���÷�Ƶֵ,��5mS���Ҷ�ʱ�����
  TCCR1B |= (1 << ICNC1) | (1 << CS10); //8MHZʱ,����Ƶһ������ж�Ϊ8mS
  #if(IR_VALID_LEVEL != 0) //�ߵ�ƽΪ��Ч�ز�ʱ,����Ϊ�����ж�,�����½���
    TCCR1B |=  (1 << ICES1);  
  #endif
  
  TIMSK |= (1 << TICIE1) | (1 <<  TOIE1); //ͬʱ������ж��벶���ж�
}

//-----------------------�жϴ������----------------------------
//��ʱ�� / ������1 ���
#pragma vector = TIMER1_OVF_vect 
__interrupt void Timer_Ov_int(void)
{
  IrDrv_IRQ(65535, 0);  //һ�ܽ���
  Ir_Avr.IdieWidth = 0;//�л�Ϊʶ��Idieģʽ
  TCNT1 = TCNT1_INIT;  //������λ
  TIMSK &=  ~(1 <<  TOIE1);//�ݹر�����ж�(�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���))
}

//��ʱ�� / ������1�¼���׽
#pragma vector = TIMER1_CAPT_vect 
__interrupt void Timer_Capt_int(void)
{
  //������ס��ǰ��ʱֵ����λ
  unsigned short WidthAppend = TCNT1; 
  TCNT1 = TCNT1_INIT;
  unsigned short Width = ICR1;  
  
  //׼���������
  WidthAppend -= Width;//���жϵ���ǰΪ���л��ص�ʱ��,���б���
  Width -= TCNT1_INIT;
  Width += Ir_Avr.WidthAppend; //���ѵı���
  Ir_Avr.WidthAppend = WidthAppend;
  
  //��⵽��������ʱ(��ǰ��Ir�����)
  if(!Ir_Avr.IdieWidth){
    Ir_Avr.IdieWidth = Width;
    #if(IR_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�����ؼ�����ź�ʱ��
      TCCR1B |=  (1 << ICES1);     
    #else
      TCCR1B &= ~(1 << ICES1); 
    #endif    

  }
  else{ //���Ir����ʱ(��ǰ�ڿ������ڼ����)
    #if(IR_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
       TCCR1B &= ~(1 << ICES1);    
    #else
      TCCR1B |=  (1 << ICES1); 
    #endif
    IrDrv_IRQ(Ir_Avr.IdieWidth, Width);  //���ݴ���  
    Ir_Avr.IdieWidth = 0;//�л�Ϊʶ���������ģʽ
    if(!(TIMSK &  (1 <<  TOIE1))){//�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���),�ڼ䲻��λ
      TIFR |=  (1 << TOV1);//�������жϱ�־
      TIMSK |=  (1 <<  TOIE1);//��������ж�
    }
  }
}

#endif //#ifdef SUPPORT_IR
