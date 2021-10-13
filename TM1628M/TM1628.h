/***************************************************************************

                  ������TM1628����ģ��ʵ��
��ģ�鸺��һ��TM1628���Զ���������ʾ(��GRID1��ʼ)���Լ���ȡ��ֵ(��KS1��ʼ)
�ݽ�֧��������ʽ��д����,����ʵ����Ӧλ��ͬһ������
***************************************************************************/
#ifndef __LED_DRV_1628_H
#define __LED_DRV_1628_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/***************************************************************************
                              �������
***************************************************************************/

//֧�ִ�ģ��ʱ���ⲿȫ�ֶ���
//#define SUPPORT_TM1628M 

//֧�ֻ�ȡ��ֵʱ����,ֻҪ��һ��ʵ���а��������趨��
//#define SUPPORT_TM1628_KEY

//����LED������, �����ʵ����Ϊ׼������1628���ϣ����֧��2*7Byte
//ע��ɨ��ӵ�0�ο�ʼ
#ifndef TM1628_LED_MAX_COUNT
  #define TM1628_LED_MAX_COUNT      (2 * 4 + 2) //Ĭ��4λ�����+1��ָʾ�� 
#endif

//����ָ�������С
#if TM1628_LED_MAX_COUNT > 4
  #define TM1628_COM_BUF   (TM1628_LED_MAX_COUNT + 2)
#else
  #define TM1628_COM_BUF   4
#endif

//�������ڳ�ʼ��ʱ��, �����ڶϿ�����;����ʱ�ָ���16msΪ��λ, <=255
#ifndef TM1628_LED_PERIOD_INIT_OV 
  #define TM1628_LED_PERIOD_INIT_OV    255
#endif

/***************************************************************************
                              ��ؽṹ
***************************************************************************/

//��̬����
struct _TM1628Static{
  //��ʾģʽ�ڿ���ʱ����:0: 4λ13�� 1: 5λ12�� 2: 6λ11��, 3: 7λ10��
  unsigned char DispMode; 
  //��ʾ������
  unsigned char LedCount;
  //�û��������Ӳ�����ұ��Զ��û��Ѻ�
  unsigned char ToHwLut[TM1628_LED_MAX_COUNT];
  //��ȡ�����ֽ�����0ʱ�ް�����1ʱ�а���
  unsigned char KeyByteCount; 
};

struct _TM1628{
  unsigned short DirtyMask;             //LED��Ӧ���࣬������������
  unsigned char DispBuf[TM1628_LED_MAX_COUNT];  //��ʾ������,��Ӳ�������ò��ұ�ʵ��ӳ��
  unsigned char Gray;//����ĻҶ�ֵ�� 0~7Խ��Խ��, 8�ر���ʾ
  unsigned char PeriodInitTimer;        //���ڳ�ʼ����ʱ��,����Ӧʵʱ����
  unsigned char Ch;                     //��TM1628ͨ����
  #ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
    unsigned char Flag;                 //��ر�־��������
  #endif  
};

//��ر�־����Ϊ
#define TM1628_KEY_PERIOD   0x80 //������ȡ���ڣ���������ʾ��������
#define TM1628_KEY_ID_MASK  0x7f //ɨ����ļ�ֵID, >=�û�����ʱ�ް���

extern unsigned char TM1628_CommBuf[TM1628_COM_BUF];//�ڽ���ͨѶ������
/***************************************************************************
                              ��غ���
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void TM1628_Init(struct _TM1628 *pTM1628, 
                 unsigned char Ch,         //��TM1628ͨ����
                 unsigned char Gray);    //0~7Խ��Խ��, 8�ر���ʾ

//--------------------------------������-----------------------------
//Լ10ms����һ��
void TM1628_Task(struct _TM1628 *pTM1628);

//-------------------------------�����������ʾ-------------------------
void TM1628_UpdateDisp(struct _TM1628 *pTM1628, 
                       unsigned char UserPos,//�ڲ���TM1628_TO_HWת��
                       unsigned char Seg);   //����

//-------------------------------�õ��������ʾ-------------------------
unsigned char TM1628_GetDisp(const struct _TM1628 *pTM1628, 
                             unsigned char UserPos);

//---------------------------------���»Ҷȼ���-------------------------
void TM1628_UpdateGray(struct _TM1628 *pTM1628, 
                       unsigned char Gray);    //0~7Խ��Խ��, 8�ر���ʾ

/***************************************************************************
                           �ص�����
***************************************************************************/

//----------------------�õ���ǰͨ����ӦStatic�ṹ--------------------------
//const struct _TM1628Static *TM1628_cbGetStatic(unsigned char Ch);
//ֱ�Ӻ�ʵ��ʱ��
extern const struct _TM1628Static TM1628_CbStatic[];
#define TM1628_cbGetStatic(ch) (&TM1628_CbStatic[ch]);

//----------------------------��������---------------------------------------
//TM1628_CommBuf���Ҫ���͵�����,������ɺ󷵻�
void TM1628_cbSendData(unsigned char Ch,
                       unsigned char SendLen);

//----------------------------���հ���ֵ-------------------------------------
#ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
  //TM1628_CommBuf[0]Ϊ�����Ϊ����ֵ��1Byteָ��,���ؼ�ֵ��ʼ��TM1628_CommBuf[1]
  void TM1628_cbFullKey(unsigned char Ch, unsigned char KeyByteLen);
#endif

//--------------------------��ֵ����ͨ��-----------------------------
#ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
  //��ȡ����ֵʱ�����ô˺���(�����Ƿ��а���),��ֵIDλ��TM1628_KEY_ID_MASK
  void TM1628_cbKeyUpdate(const struct _TM1628 *pTM1628);
#endif

//--------------------------�Ƿ��ڲ���״̬-----------------------------
//����״ָ̬ʾ��ȫ��,ֱ��ʵ�֣�
#include "Test.h"
#define TM1628_cbIsTest()    Test_IsDoing()

#endif //#define __LED_DRV_1628_H

