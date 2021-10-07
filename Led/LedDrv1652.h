/***************************************************************************
                       �������ʾ��������-��ʹ��1652ʱ��ʵ��
��ģ�鸺��ɨ����������ܵ���ʾ
��ģ�������Ӳ��
�������ӿ�ͳһ����Ϊ��1��,0�� 
***************************************************************************/

#ifndef __LED_DRV_1652_H
#define __LED_DRV_1652_H

/***************************************************************************
                              �������
***************************************************************************/
//����Usart����
//�̶�Ϊ19200bps 8������λ����У�飨1����Ϊ����ʱУ��λΪ0����1ֹͣλ

//����LED����, 5��6
#ifndef LED_DRV_LED_COUNT
  #define LED_DRV_LED_COUNT      5 
#endif

//ʹ�ñ�׼USART(������)������ʹ�ûص�����
//#define SUPPORT_LED_DRV_1652_USART   

/***************************************************************************
                              ��ؽṹ
***************************************************************************/
struct _LedDrv1652{
  unsigned char Buf[LED_DRV_LED_COUNT];  //��ʾ������,��Ӳ�������ò��ұ�ʵ��ӳ��
  unsigned char CommBuf[LED_DRV_LED_COUNT + 1];//ͨѶ���ͻ�����
  unsigned char Timer;                   //��ʱ��
  unsigned char UpdateMask;             //�����λ������
};

extern struct _LedDrv1652  LedDrv1652;

/***************************************************************************
                              ��غ���
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void LedDrv1652_Init(void);

//-------------------------------������-----------------------------
//1ms����һ��
void LedDrv1652_Task(void);

/***************************************************************************
                    ��Դ����ص����������趨������
***************************************************************************/

//---------------------���û���λ�õ�Ӳ��---------------------------
#include "IOCtrl.h"
//λ�ò��ұ�����:
#ifndef LED1652_TO_HW_LUT
  #define   LED1652_TO_HW_LUT  {0,1,2,3,4}
#endif
//����λ,b4:7:λ����ռ�ձ�b3:1�ε���ռ�ձȣ�b0:8��5λ(0Ĭ��)��7��6λ(1)
#ifndef LED1652_CFG
 #define   LED1652_CFG  0xfe
#endif

//����λ���붨��,��ʱ������
//����λ���������̵����ȣ��ڲ���ʱ����λ��λ��LED_DRV_LED_COUNT - 1λ��
//#define LED1652_RSV_POS      4
//#define LED1652_RSV_MASK     0xff

//��������ٸ��µ�λ����ָʾ��,��ʱ������
//#define LED1652_FAST_BIT     4


/***************************************************************************
                           �ص�����
***************************************************************************/

//-----------------------------Ӳ����ʼ��------------------------------------
#ifndef SUPPORT_LED_DRV_1652_USART //�ڽ�USARTʱ
  void LedDrv1652_cbHwInit(void);
#else //ͨ��USART
  #define LedDrv1652_cbHwInit() do{}while(0)//������ʼ��
#endif
//----------------------------��������---------------------------------------
//LedDrv1652.CommBuf���Ҫ���͵�����
#ifndef SUPPORT_LED_DRV_1652_USART //�ڽ�USARTʱ
  void LedDrv1652_cbSendStart(unsigned char SendSize);
#else //ͨ��USART�� �ڲ���ʵ�֣����������ӿ�:
  #include "UsartDev.h"
  struct _UsartDev *LedDrv1652_cbpGetUsartDev(void);  
#endif

//--------------------------�Ƿ��ڲ���״̬-----------------------------
//����״ָ̬ʾ��ȫ��,ֱ��ʵ�֣�
#include "Test.h"
#define LedDrv1652_cbIsTest()    Test_IsDoing()

#endif //#define __LED_DRV_1652_H
