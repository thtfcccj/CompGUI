/***************************************************************************
                       �������ʾ�ӿ�
��ģ�鸺�����û�����,��������д�����������
��ģ���������Ӳ��
�������ӿ�ͳһ����Ϊ��1��,0��  ��ʼ��Ϊȫ��״̬
***************************************************************************/

#ifndef __LED_H
#define __LED_H

/***************************************************************************
                              �������
***************************************************************************/

//����LED����, <= 8
#ifndef LED_LED_COUNT               
  #define LED_LED_COUNT               7  
#endif

//����ɶ������ƶ��������Ķ�����������,��������չָʾ��,< LED_LED_COUNT
#define LED_SEG_FLASH_COUNT        3

//���嶨ʱ��,��������Ƶ��
#define LED_FLASH_TIMER_COUNT      32

/***************************************************************************
                              ��ؽṹ
***************************************************************************/
struct _Led{
  //�û�������ʾ������:Ϊ��Ӧ�ò��Ӧ,���ܸı��������ݵķ���λ��
  //��������λ�����Ϊ: ��λ��Ӧ��������λ,�����ҵ�������,0~3Ϊ����ר��
  unsigned char DotDisp;           //ÿ�������С������ʾ
  unsigned char SegDisp[LED_LED_COUNT]; //������ʾ������
  unsigned char SegFlash;          //����������������
  unsigned char DotFlash;          //ÿ�������С��������
  unsigned char SegGrpFlash[LED_SEG_FLASH_COUNT];   //���Ե������ƶ��������Ķ�
  unsigned char DisBuf[LED_LED_COUNT];        //����:��ֹ���µĶλ�����
  unsigned char Timer;                        //����:��ʱ��
  
  unsigned char Flag;   //��ر�־,������
};
//��ر�־����Ϊ:(��Ӧ�ò��Ӧ,���ܸı��·���λ��)
#define LED_SYSTEM        0x07      //��3λΪϵͳԤ������1s��0,�Ժ���2
#define LED_TEST          0x08      //���ڲ���״̬,�������ȫ��,�ڵײ�ʵ��
#define LED_DISP          0x10      //����ʾ����,��������������
#define LED_FLASH_NOW     0x20      //ǿ�Ƹ���������־
#define LED_FLASH_DIS     0x40      //ǿ�Ƹ���������־��λʱ,��������������ʾ����
#define LED_UPDATE_NOW    0x80      //ǿ�Ƹ���ɨ���־,��λ��λ��ǿ�Ƹ���ɨ��

#define LED_COMM_MASK     0xe8       //����SPI���Ƶ�λ,�����ڲ���λ

extern struct _Led  Led;

/***************************************************************************
                              ��غ���
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void Led_Init(void);

//-------------------------------������-----------------------------
//����0.5s������ɨ��,�Ի������Ч��,Ҳ��ֱ�ӵ��ø�������
void Led_Task(void);

//---------------------------����״̬�ж������-----------------------------
#define Led_IsTest()  (Led.Flag & LED_TEST)
#define Led_ClrTest()  do{Led.Flag &= ~LED_TEST;}while(0)

//-----------------------�������״̬�ж�-----------------------------
#define Led_IsNor()  (Led.Flag & LED_SYSTEM)

/***************************************************************************
                    ��Դ����ص����������趨������
***************************************************************************/
#include "IOCtrl.h"

//-----------����Ӳ��ʵ��ʱ,С�����ڶ����е�λ������---------------
#include "LedSign.h"
#define LED_DOT_BUF_MASK            LED_SIGN_DOT

//-----------------------����ʾ���庯��-----------------------------
#ifdef SUPPORT_LCD
  #include "LcdDrv.h"
  #define Led_cbSetBuf(pos, buf)  \
    do{LcdDrv.Buf[(pos) + LCD_BUF_MD_START] = buf;}while(0)
#endif

#ifdef SUPPORT_LED
  #include "LedDrv.h"
  #define Led_cbSetBuf(pos, buf)  do{LedDrv.Buf[pos] = buf;}while(0)
#endif

#ifdef SUPPORT_LED_SPI
  #include "LedDrv_Spi.h"
  #define Led_cbSetBuf(pos, buf)  do{LedDrv_Spi_SetLed(pos, buf);}while(0)
#endif

//-----------------------�õ���ʾ���庯��-----------------------------
#ifdef SUPPORT_LCD
  #define Led_cbGetBuf(pos)  (LcdDrv.Buf[(pos) + LCD_BUF_MD_START])
#endif

#ifdef SUPPORT_LED
  #define Led_cbGetBuf(pos)  (LedDrv.Buf[pos])
#endif

#ifdef SUPPORT_LED_SPI
  #define Led_cbGetBuf(pos)  LedDrv_Spi_GetLed(pos)
#endif

/***************************************************************************
                           �ص�����
***************************************************************************/

//--------------------------Led���ӹ��ܳ�ʼ������------------------------
//������ɳ�4����ʾ���������ʾ���ܵ�(��LCDʱ���ַ���)��ʼ��
//�β�ΪFlagλ����
#ifdef SUPPORT_LCD
  void Led_cbAppendInit(void);
#else
  #define Led_cbAppendInit()  do{}while(0)
#endif

//--------------------------Led���ӹ��ܸ��º���------------------------
//������ɳ�4����ʾ���������ʾ���ܵ�(��LCDʱ���ַ���)����ʾ����
//�β�ΪFlagλ����
#ifdef SUPPORT_LCD
  void Led_cbAppendUpdate(unsigned char Flag);
#else
  #define Led_cbAppendUpdate(Flag)  do{}while(0)
#endif

#endif //#define __LED_H

