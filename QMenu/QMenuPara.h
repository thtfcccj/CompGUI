/***********************************************************************

		             �˵�ϵͳ֮4λ�����ʱ-���û��Ľ����ӿ�
/�ɲ����뺯���ӿڹ�ͬ���:
***********************************************************************/
#ifndef _QMENU_PARA_H
#define	_QMENU_PARA_H

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
//ע:ֻ��ʱ����SetData����Ϊ��.����Ϊ��,���û�����ȷ�ϼ��󽫵��ô˺������˳�

#define QMENU_CFG_ADJ_MASK   0x03  //�˵���ĵ�����ʽ:
#define QMENU_CFG_ADJ_BIT    0x00  //��λ����ģʽ
#define QMENU_CFG_ADJ_ALL    0x01  //�������ģʽ(�����Сֵ�뵱ǰֵ��ͬʱ��������ֵ)
#define QMENU_CFG_ADJ_LOGIC  0x02  //�߼�������ģʽ
#define QMENU_CFG_ADJ_CMD    0x03  //����ģʽ,������ȷ�ϼ�

//���������ֶ���Ϊ:
#define QMENU_CFG2_QUIT       0x80    //������ɺ��˳��˵���־
#define QMENU_CFG2_QUIT_SAVE  0x40    //��Ӧ�˳�ʱ����,���ڲ���Ҫ�����ı���,��ָ��ֳ�
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

struct _QMenuFun{
  unsigned char Note[4];//�˵�ѡ�������ʾ��(��С����)
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
//-------------------QMenuPara��ʼ������--------------------------
void QMenuPara_Init(struct _QMenuPara *pPara);


#endif
