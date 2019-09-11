/***************************************************************************
                       �������ʾ��������-ʹ��SPI��ʾ��ʱ��ʵ��
��ģ�鸺��ɨ����������ܵ���ʾ,�Լ���SPI��ü�ֵ
��ģ��ʹ��3131����ʾ����Ϊ����,ͨ��SPI�ӿڲ�������
��ģ�������SPI�ӿڵ�0x02~0x0b����,���������ȿ���
***************************************************************************/

#ifndef __LED_DRV_SPI_H
#define __LED_DRV_SPI_H

/***************************************************************************
                              �������
***************************************************************************/

//�������SPI�ӿڵ���ʼ��ַ
#define LED_DRV_SPI_ADR_START   0x02     //��ʼ��ַ

//�������SPI�ӿڵ�Ӳ����ַ����
#define LED_DRV_SPI_ADR_COUNT     8 //�Ӽ���1+���״̬1+С����1+��ʾ������5��

//���SPI����������,��ַ��ض���Ϊ(��ֵ+��ʼ��ַ=SPI���͵�ַ):
#define LED_DRV_SPI_ADR_KEY     0x00    //����ֵ
#define LED_DRV_SPI_ADR_STATE   0x01    //���״̬,��Э���붨��
#define LED_DRV_SPI_ADR_DOT     0x02    //С������ʾ������
#define LED_DRV_SPI_ADR_SEG     0x03    //7�������ʾ��ʼ
#define LED_DRV_SPI_SEG_COUNT   5       //֧��7����ĸ���
//STATEʹ�͵���λ����Ϊ:
#define LED_DRV_SPI_UPDATE      0x80    //�������±�־

/***************************************************************************
                              ��ؽṹ
***************************************************************************/

//Spi״̬��
enum _eLedDrv_Spi{
  eLedDrv_Spi_Idie =    0,    //����ģʽ,�������(�ӻ���ʾ��)�ж�����
  eLedDrv_Spi_Wait =    1,    //���ж���,�ȴ�CSΪ��
  eLedDrv_Spi_Doing =   2,    //�������ݸ�����
};

struct _LedDrv_Spi{
enum _eLedDrv_Spi eState; //SPI״̬��  
  unsigned char Buf[LED_DRV_SPI_ADR_COUNT];   //SPI��Ч������
};

extern struct _LedDrv_Spi  LedDrv_Spi;

/***************************************************************************
                              ��غ���
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void LedDrv_Spi_Init(void);

//----------------------------����������-----------------------------
//������ٽ�����ɨ��
void LedDrv_Spi_FastTask(void);

//---------------------------�����ø������ݱ�־����---------------------------
//void LedDrv_Spi_SetUpdate(void);
#define LedDrv_Spi_SetUpdate() do{\
LedDrv_Spi.Buf[LED_DRV_SPI_ADR_STATE] |= LED_DRV_SPI_UPDATE;}while(0)

//--------------------------����ĳ������ܺ���---------------------------
void LedDrv_Spi_SetLed(unsigned char Pos,     //�����λ��
                       unsigned char SetDot);//��LED_SIGN_DOT����ʾ����ֵ

//--------------------------�õ�ĳ������ܺ���---------------------------
//���ش�LED_SIGN_DOT����ʾ����ֵ
unsigned char LedDrv_Spi_GetLed(unsigned char Pos);     //�����λ��

//--------------------------�õ�ĳ����С��������ܺ���---------------------------
//unsigned char LedDrv_Spi_GetLedNoLed(unsigned char Pos);     //�����λ��
#define LedDrv_Spi_GetLedNoLed(pos) (LedDrv_Spi.Buf[LED_DRV_SPI_ADR_SEG + pos])

//--------------------------С�����������---------------------------
#define LedDrv_Spi_SetDot(mask) \
  do{LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] |= mask;}while(0)
#define LedDrv_Spi_ClrSetDot(mask) \
  do{LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] &= ~(mask);}while(0)
#define LedDrv_Spi_IsSetDot(mask) \
  (LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] &= ~(mask))

/***************************************************************************
                    ��Դ����ص����������趨������
***************************************************************************/


/***************************************************************************
                           �ص�����
***************************************************************************/

//--------------------------�Ƿ��ڲ���״̬-----------------------------
//����״ָ̬ʾ��ȫ��
#include "Test.h"
#define LedDrv_Spi_cbIsTest()    Test_IsDoing()

//-------------------------�м�ֵͨ��-----------------------------
//��鵽�а���ʱ������ô˺���,�ӿڷ���MenuMng��׼
#include "Key_CB.h"
#define LedDrv_Spi_cbKeyNotify(key)    Key_CB_SetKey(key)


#endif //#define __LED_DRV_SPI_H

