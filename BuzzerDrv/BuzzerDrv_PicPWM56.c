/***************************************************************************

          Ƶ�ʿɿط�����ģ��ײ������ӿ�-��PIC��Ƭ������PWM5+PWM6ʱ��ʵ��
//ʹ��˫�������������ѹ�ȼ�:��V-G,G-V����
//ע:��ECCPʱ,��PWM�޷����ܣ�����ʹ��PWM5+PWM6ʵ��
****************************************************************************/

#include <pic.h>
#include <string.h>
#include "PicBit.h"
#include "IoCtrl.h"

#include "BuzzerDrv.h"
#include "string_P.h"

/***************************************************************************
                            �ڲ�����
****************************************************************************/

//����MCU�Ĺ���Ƶ��
#ifndef _MCU_HZ
  #define _MCU_HZ         8000000   //����MCU�Ĺ���Ƶ��
#endif

//����ʹ�õĶ�ʱ��,PWMģʽ��ʹ��(��8λģʽ)2/4/6��ʱ���е�һ��,������ʱʹ�ö�ʱ��2
//#define BUZZER_USE_TIMER_4 //ʹ�ö�ʱ��4ʱ����
//#define BUZZER_USE_TIMER_6 //ʹ�ö�ʱ��6ʱ����

/***************************************************************************
                           �ڲ����ú�ĺ�ת��
****************************************************************************/

//------------------------���ڶ�ʱ��2�Ĵ�����ת��------------------------
#ifndef BUZZER_USE_TIMER_4
#ifndef BUZZER_USE_TIMER_6
  #define TbCON        T2CON       //����
  #define PRb          PR2         //��Ƶ
#endif //#ifndef BUZZER_USE_TIMER_4
#endif //#ifndef BUZZER_USE_TIMER_6

//------------------------���ڶ�ʱ��4�Ĵ�����ת��------------------------
#ifdef BUZZER_USE_TIMER_4
  #define TbCON        T4CON       //����
  #define PRb          PR4         //��Ƶ
#endif //#ifndef BUZZER_USE_TIMER_4

//------------------------���ڶ�ʱ��6�Ĵ�����ת��------------------------
#ifdef BUZZER_USE_TIMER_6
  #define TbCON        T6CON       //����
  #define PRb          PR6         //��Ƶ
#endif //#ifndef BUZZER_USE_TIMER_6

/***************************************************************************
                          ��غ���ʵ��
****************************************************************************/


//����Ƶ����Ӧ����ͷ�����Ƶ��ֵ
static const __flash unsigned short _PsForMinHz[] = {
  (unsigned long )_MCU_HZ >> (8 + 0),   //1��Ƶ
  (unsigned long )_MCU_HZ >> (8 + 2),   //4��Ƶ
  (unsigned long )_MCU_HZ >> (8 + 4),   //16��Ƶ
  (unsigned long )_MCU_HZ >> (8 + 6),   //64��Ƶ
};

//��Ƶ���±��Ӧ�ķ�Ƶ��
static const __flash unsigned long _Ps2Div[] = {
  1,     //1��Ƶ
  4,     //4��Ƶ
  16,    //16��Ƶ
  64,    //64��Ƶ
};

//------------------------------��ʼ������---------------------------------
void BuzzerDrv_Init(void)
{
   //���Ƶ�� = _MCU_HZ / ((Ԥ��Ƶֵ(4bit) * (��Ƶ��ֵ(8bit))
   //����˵��:
   //  1.����ʹ��PPSģ��ѡ��(��CfgBuzzerIo()ʱʵ��)���ֱ�����ߵ͵�ƽ��ʵ��˫������
   //  2.PWM5,PWM6��ֵΪ��ʱֵ��һ����ʵ��50%ռ�ձ�
   //  3.ͨ�����ö�ʱ���������Ϊ��Ҫ��Ƶ�ʣ��Զ��ַ�������Ƶ�ʿ���
   TbCON = 0; //�ȹر�
   PWM5CON  = 0;
   PWM6CON  = 0;
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
  //���Ƶ�� = _MCU_HZ / ((Ԥ��Ƶֵ(4bit) * (��Ƶ��ֵ(8bit))

  //����Ԥ��Ƶֵ
  unsigned char Ps;
  for(Ps = 0; Ps < 4; Ps++){
    if(Hz > _PsForMinHz[Ps]) break;
  }

  //���㲢��䶨ʱ��ֵ,��������ʱ��
  unsigned char Pr = (unsigned long )_MCU_HZ / (Hz * _Ps2Div[Ps]);
  PRb = Pr;
  //��Ƶֵ��Ϊ50%,����2λ����(50%����̫��ȷ))
  Pr >>= 1;
  PWM5DCH = Pr;
  PWM6DCH = Pr;
  PWM5CON  = 0x80;  //������������
  PWM6CON  = 0x90;  //����������
  TbCON = PICB_TMR2ON | Ps; //ʹ�����Fosc/4(8MHZ)��ʵ��Ƶ�ʵ�ϸ΢����
  
  //ע��4KHZ������ʵ��Ƶ�����Ϊ��
  //	116	4310.344828
  //	117	4273.504274
  //	118	4237.288136
  //	119	4201.680672
  //	120	4166.666667
  //	121	4132.231405
  //	122	4098.360656
  //	123	4065.04065
  //	124	4032.258065
  //	125	4000
  //	126	3968.253968
  //	127	3937.007874
  //	128	3906.25
  //	129	3875.968992
  //	130	3846.153846
  //	131	3816.793893
  //	132	3787.878788
}

//-----------------------------ֹͣ����-------------------------------------
void BuzzerDrv_Stop(void)
{
  TbCON = 0;//�رն�ʱ��
   PWM5CON  = 0;
   PWM6CON  = 0;
  ClrBuzzerIo();//����Ϊ�͵�ƽ�޵������
}