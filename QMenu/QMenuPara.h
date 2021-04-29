/***********************************************************************

		             �˵�ϵͳ֮4λ�����ʱ-���û��Ľ����ӿ�
//�ɲ����뺯���ӿڹ�ͬ���
//�û�����ͨ���˽ӿڣ���˵���������ϵ
***********************************************************************/
#ifndef _QMENU_PARA_H
#define	_QMENU_PARA_H

/***********************************************************************
		                           ��ѡ����
***********************************************************************/

//�Ƿ�֧��ʮ��λ����ģʽ,ȫ���ﶨ��
//#define SUPPORT_QMENU_ADJ_HEX 

/***********************************************************************
		             ���û������Ĳ����ӿ�
***********************************************************************/
struct _QMenuPara{
  signed short Adj;      //��ǰ����ֵ
  signed short Min;      //����ֵ����Сֵ,��  
  signed short Max;      //����ֵ�����ֵ,��
  unsigned char Cfg;     //��������,ÿ�˵����������,������
  unsigned char Cfg2;    //����������,����ҪʱΪ0,������
};

//�������ֶ���Ϊ:
#define QMENU_CFG_RD        0x80    //�˵�Ϊֻ��״̬,�����ܹ��������
#define QMENU_CFG_WR        0x40    //�˵�Ϊֻд״̬,��ֱ�ӽ�����������
#define QMENU_CFG_REAL_RD   0x20    //�˵���ʾʱ��ʵʱ����ˢ��
#define QMENU_CFG_REAL_WR   0x10    //�˵�����ʱ��ʵʱдˢ��,����ֵ�ı�ʱ������]
#define QMENU_CFG_REAL_REFRESH 0x08    //�˵�����ʾ�����ʱ����ʵʱˢ��(�Ը�����ʱ��������)

#define QMENU_CFG_ADJ_MASK   0x07  //�˵���ĵ�����ʽ:
#define QMENU_CFG_ADJ_BIT    0x00  //��λ����ģʽ
#define QMENU_CFG_ADJ_ALL    0x01  //�������ģʽ(�����Сֵ�뵱ǰֵ��ͬʱ��������ֵ)
#define QMENU_CFG_ADJ_LOGIC  0x02  //�߼�������ģʽ
#define QMENU_CFG_ADJ_CMD    0x03  //����ģʽ,������ȷ�ϼ�
#define QMENU_CFG_ADJ_PASS   0x04  //��������ģʽ��ͬ��λ���ڣ���
                                   //���һλ�л�ʱ�����һ��WrData(QMENU_CFG_WR)
                                   //д����ʵ���бȽ�������˿��˳��˵���������Ӧ����

#ifdef SUPPORT_QMENU_ADJ_HEX
  #define QMENU_CFG_ADJ_HEX    0x05  //ʮ������, ����ģʽ,��ÿλ��Χ0~F
#endif

//���������ֶ���Ϊ:
#define QMENU_CFG2_QUIT_SAVE  0x80    //��Ӧ�˳�ʱ����,���ڲ���Ҫ�����ı���,��ָ��ֳ�
#define QMENU_CFG2_QUIT       0x40    //������ɺ��˳��˵���־
#define	QMENU_CFG2_NEGATIVE   0x20    //������Ϊ������ʾ�����,������ʾ��ֵ

#define	QMENU_CFG2_DOT_MASK   0x0f    //���4λ���ڿ�����ʾ������ʱ��С����λ��ʾ
//#define	QMENU_CFG2_DOT0       0x01    //��������:��1��С������ʾ
#define	QMENU_CFG2_DOT1       0x02    //��������:��2���������ʾ
#define	QMENU_CFG2_DOT2       0x04    //��������:��3���������ʾ
#define	QMENU_CFG2_DOT3       0x08    //��������:��4���������ʾ

/***********************************************************************
		             ���û������ĺ����ȹ��ܽӿ�
***********************************************************************/

//�����˵���εĶ���:
#define QMENU_LAYER_NULL   0 //���ڲ˵���
#define QMENU_LAYER_SEL    1 //��ѡ��˵�״̬
#define QMENU_LAYER_RD     2 //�ڲ鿴(��)״̬
#define QMENU_LAYER_WR     3 //���޸�(д)״̬

#include "QMenuPixel.h" //���ػ�֧�ֽṹ

struct _QMenuFun{
  #ifdef SUPPORT_QMENU_PIXEL  //���ػ�֧��ʱ
    const struct QMenuPixel_Desc Desc;//�����ṹ
  #else //4λ�����ʱ:
    unsigned char Note[4];//�˵�ѡ�������ʾ��(��С����)
  #endif
  //��ȡ������QMenuPara,�β�:
  //QMENU_CFG_REAL_RD: Ϊʵʱ��ȡ״̬λ��,ʵʱ����ʱ����
  //QMENU_CFG_REAL_WR: ΪͨѶ�����,д����������
  //��������Ϊ�˵����,��ʾ����ò�˵�ʱ����,�ɸ����������QMenuPara��ʼ��:
  void (*GetData)(struct _QMenuPara *pPara, unsigned char Type);
  //QMenuPara.Adj���,��������,�β�:
  //QMENU_CFG_REAL_WR: Ϊʵʱд״̬λ��,ʵʱд������ֵ�仯ʱ����
  //QMENU_CFG2_QUIT_SAVE: Ϊ�˳�ʱ����,�˳����湦�ܴ�,���˳�ǿ�Ƶ���(����ֵ�Ƿ�仯)
  //QMENU_LAYER_WR,��ʾ�˳�д״̬ʱ����,���������ֵ�仯ʱ����д����
  void (*SetData)(struct _QMenuPara *pPara, unsigned char Type);
};

/***********************************************************************
		                       ��غ����ӿ�
***********************************************************************/
//----------------------QMenuPara��ʼ������--------------------------
void QMenuPara_Init(struct _QMenuPara *pPara);

//----------------------QMenuPara->SetData�պ���----------------------
//�˵���Ϊֻ�������¼���Ҫ����ʱ�ɵ���
void QMenuPara_SetNULL(struct _QMenuPara *pPara, unsigned char Type);

/***********************************************************************
		                       ��ػص�����
***********************************************************************/

//---------------------DataSetʵ�������˳��˵�����-----------------------
//��������Ӧ�ò���ʵ��SetData�������ڲ�����(�ڲ��������˳��ص������Է�ֹǶ��)
void QMenuPara_cbQuitMenuInSet(struct _QMenuPara *pPara);

#endif


