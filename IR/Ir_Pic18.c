/***************************************************************************
                       ������ճ������ж���Pic18�е�ʵ��
ʹ��TIMER1+ECCP2����(SSOP28ʱΪ13��)ʵ��
***************************************************************************/

#include "IrDrv.h"
#include "IOCtrl.h"

#include <string.h>
#include <pic.h>
#include "PicBit.h"
#include "IRQ_Pic.h"

struct _Ir_Pic{
  unsigned short IdieWidth;   //������Ir�ز�ʱ������,0ʱ��ʾû�иû��弴�����ź�����
  unsigned short WidthAppend;//���ڲ����ж�Ӱ������ļ�����ʧ
};
struct _Ir_Pic  Ir_Pic;


//���嶨ʱ����ֵ,���ڶ�ʱ���������,��ʱ�����ʱ��涨Ϊ8ms
#ifndef TMR1_INIT
  #define    TMR1_INIT        0
#endif

/***************************************************************************
                              IR�ж����
***************************************************************************/
//---------------------IrDrvӲ����ʼ���ص�����ʵ��-----------------------------
void IrDrv_cbHwInit(void)
{
  Ir_Pic.IdieWidth = 0;
  Ir_Pic.WidthAppend = 0;  
  
  CfgIR();//��ʼ��IO��

  //����Timer1Ϊ��ʱ��
  T1CON = 0; //ʹ��ָ��ʱ8MHZ,����Ƶһ������ж�Ϊ8mS(�ر��ſ�ģʽ)
  TMR1 = TMR1_INIT; //�ó�ֵ
  
  CCPTMRS &= ~PICB_C2TSEL;// CCP2����TMR1(�����Ƚ�ʱ)��TMR2(PWM)����

  #if(IR_DRV_VALID_LEVEL != 0) //�ߵ�ƽΪ��Ч�ز�ʱ,����Ϊ�����ж�,�����½���
    CCP2CON = PICB_CCP2M2 | PICB_CCP2M0;//0101= ��׽ģʽ��ÿ�������ز�׽һ��
  #else
    CCP2CON = PICB_CCP2M2 | 0;//0100= ��׽ģʽ��ÿ���½��ز�׽һ��
  #endif
  PIR1 &= ~PICB_TMR1IF; //��ʱ��1���ж�
  PIE1 |= PICB_TMR1IE; //��ʱ��1�ж�����
  PIR3 &= ~PICB_CCP2IF; //�岶���ж�
  PIE3 |= PICB_CCP2IE; //�����ж�����

  T1CON |= PICB_TMR1ON; //��ʼ���ö�ʱ��
}

//----------------------��ʱ������жϴ������----------------------------
//��ʱ�� / ������1 ���
void Timer_Ov_int(void)
{
  IrDrv_IRQ(65535, 0);  //һ���ڽ���
  Ir_Pic.IdieWidth = 0;//�л�Ϊʶ��Idieģʽ
  TMR1 = TMR1_INIT;   //������λ
  PIE1 &= ~PICB_TMR1IE;    //�ݹر�����ж�(�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���))
  PIR1 &= ~PICB_TMR1IF;     //������ж�
}

//��ʱ�� / ������1�¼���׽
void Timer_Capt_int(void)
{
  //������ס��ǰ��ʱֵ����λ
  unsigned short WidthAppend = TMR1; 
  TMR1 = TMR1_INIT;
  unsigned short Width = CCPR2L | ((unsigned short)CCPR2H << 8);//CCPR1; //��׽����ֵ
  
  //׼���������
  WidthAppend -= Width;//���жϵ���ǰΪ���л��ص�ʱ��,���б���
  Width -= TMR1_INIT;
  Width += Ir_Pic.WidthAppend; //���ѵı���
  Ir_Pic.WidthAppend = WidthAppend;
  
  //��⵽��������ʱ(��ǰ��Ir�����)
  if(!Ir_Pic.IdieWidth){
    Ir_Pic.IdieWidth = Width;
    #if(IR_PIC_IR_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�����ؼ�����ź�ʱ��
      CCP2CON = PICB_CCP2M2 | PICB_CCP2M0;//0101= ��׽ģʽ��ÿ�������ز�׽һ��
    #else
      CCP2CON = PICB_CCP2M2 | 0;//0100= ��׽ģʽ��ÿ���½��ز�׽һ��
    #endif
  }
  else{ //���Ir����ʱ(��ǰ�ڿ������ڼ����)
    #if(IR_PIC_IR_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
      CCP2CON = PICB_CCP2M2 | 0;//0100= ��׽ģʽ��ÿ���½��ز�׽һ��
    #else
      CCP2CON |= PICB_CCP2M2 | PICB_CCP2M0;//0101= ��׽ģʽ��ÿ�������ز�׽һ��
    #endif
    IrDrv_IRQ(Ir_Pic.IdieWidth, Width);  //���ݴ���  
    Ir_Pic.IdieWidth = 0;//�л�Ϊʶ���������ģʽ
    if(!(PIE1 & PICB_TMR1IE)){//�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���),�ڼ䲻��λ
      PIR1 &= ~PICB_TMR1IF;//�������жϱ�־
      PIE1 |= PICB_TMR1IE;//��������ж�
    }
  }
  PIR3 &= ~PICB_CCP2IF;     //������ж�
}
