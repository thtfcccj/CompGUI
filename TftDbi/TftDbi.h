/*******************************************************************************

                         TFT DBI����ͳһ�ӿ�
�˽ӿ�Ϊ�����ӿڣ��ײ���Ϊ���ݾ���DBI���粢��, SPI��
********************************************************************************/
#ifndef _TFT_DBI_H
#define _TFT_DBI_H

/*******************************************************************************
                                  �������
********************************************************************************/

#ifndef TFT_DRV_H_PIXEl//ˮƽ���ص���
  #define TFT_DRV_H_PIXEl    320     //����ʱ
#endif

#ifndef TFT_DRV_V_PIXEl//��ֱ���ص���
  #define TFT_DRV_V_PIXEl    480      //����ʱ
#endif

#ifndef TFT_DRV_TIM_OV   //��ʱ�����ʱ��,16msΪ��λ
  #define TFT_DRV_TIM_OV    2
#endif

/*******************************************************************************
                                  ��ؽṹ
********************************************************************************/
struct _TftDbi{
  unsigned char LastCmd; //�������д�������
  unsigned char Timer;   //��ʱ��
};

extern struct _TftDbi TftDbi; //ֱ�ӵ�����

/*******************************************************************************
                         ��ز�������-��ʾ����ز�������
�ӿ�ͳһ���ɵײ�ʵ��,ȫ��Ϊ����ʽ
********************************************************************************/
#include "Color.h"

//---------------------------------��ʼ������-----------------------------------
void TftDbi_Init(void);

//---------------------------------������-----------------------------------
//��16ms������ɨ��
void TftDbi_Task(void);

/*******************************************************************************
                         ��ز�������-Ӳ����ز�������
�ӿ�ͳһ���ɵײ�ʵ��,ȫ��Ϊ����ʽ
********************************************************************************/

//--------------------------------Ӳ����ʼ������------------------------------
void TftDbi_HwInit(void);

//---------------------------------дָ���----------------------------------
//ָ��̶�Ϊ1��Byte
void TftDbi_WrCmd(unsigned char Cmd);

//------------------------------------д���ݺ���-------------------------------
void TftDbi_WrData(unsigned char Data);

//------------------------------------�����ݺ���-------------------------------
//ע������ָ��������׸�����Ϊ��Чֵ
unsigned char TftDbi_RdData(void);

//-----------------------------------д��ɫ����--------------------------------
void TftDbi_WrColor(Color_t Color);

//-----------------------------------����ɫ����--------------------------------
Color_t TftDbi_RdColor(void);

//------------------------------------����Ч���ݺ���---------------------------
//��ָ���,����������Чֵ,����ô˺���
//void TftDbi_RdInvalid(void);
#define TftDbi_RdInvalid()  do{TftDbi_RdData(); }while(0)

/*******************************************************************************
                         ��ز�������-��ϲ�������
�ɱ�ģ����õײ�Ԫ��������ʵ��
********************************************************************************/

//---------------------------------дU16���ݺ���-------------------------------
//�ȸߺ��˳��
void TftDbi_WrDataS(unsigned short Data);

//----------------------------------��U16���ݺ���-------------------------------
//�ȸߺ��˳��
unsigned short TftDbi_RdDataS(void);

//---------------------------дָ��-��1Byte��������-----------------------------
//ָ��̶�Ϊ1��Byte
void TftDbi_WrCmdB1(unsigned char Cmd, unsigned char Para);

//---------------------------дָ��-��1Short��������----------------------------
//�����ȸߺ��˳��
void TftDbi_WrCmdS1(unsigned char Cmd, unsigned short Para);

/*******************************************************************************
                                 �ص�����
********************************************************************************/

//---------------------------------Ӳ����ʼ������----------------------------------
void TftDbi_cbHwInit(void);

//----------------------�õ��û���ɫ���ұ�----------------------------------
#if defined(SUPPORT_COLOR_RGB666) || defined(SUPPORT_COLOR_INDEX)
  const unsigned long *TftDrv_pcbGetUserCLut(void); //��
  unsigned char TftDrv_cbGetUserCLutSize(void); //����С,< (255 - COLOR_COUNT)
#endif

#endif //#define _TFT_DRV_H



