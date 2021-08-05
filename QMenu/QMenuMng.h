/***********************************************************************

//		             �˵�ϵͳ֮4λ�����ʱ�Ĺ�����ʵ��
//��ģ�����Ϊ��������������ʱ��ʵ��Ϊֱ����Led.h�Խ�
//�˶�������Ϊ���ݵ�������MCU��ջ����Դ���ޣ���ʹ�ô�ģ���Ӧ�ĵ���ģ��
***********************************************************************/
#ifndef _QMENU_MNG_H
#define	_QMENU_MNG_H

/***********************************************************************
                            ˵��
***********************************************************************/
//�˵���������:
//��һ��Ϊ�˵�ѡ��,��ʾһ������ַ���ʾ,��ʱ��ѡ�������˳�,
//�ڶ���Ϊ������ʾ,��һ���Զ�����ʾֵ,Ҳ��ʵʱ������ʾֵ
//������Ϊ��������:���ڶ����ֻ��ʱ,��ȷ�ϼ�����
//�ڶ�,���������������п���
//�˵�ϵͳ����Ӧ����,�˳�,��,�¼��Ķ̰�,����,����ʱ��Ӧ

/***********************************************************************
                    �������
***********************************************************************/

//֧�����ػ�ʱ���壬����Ĭ��֧��4λ�����, ����ȫ���ﶨ��
//#define SUPPORT_QMENU_PIXEL   //֧�����ػ�ʱ

//֧��������ʾӳ�䷽ʽʱ(�����ػ�)��QMenuMng_cbDispOther.h����û��Զ���
//#define SUPPORT_QMENU_MAP_DISP_OTHER 


/***********************************************************************
                    ��ؽṹ-�˵�������
//���û�����������QMenu.h�ﶨ��
***********************************************************************/

#include "QMenuPara.h"
//���ڲ˵�ϵͳ���й���,�û���ֱֹ�ӷ���
struct _QMenuMng{
  //��ǰ������,������0:���ڲ˵�,������1,2,3�˵���
  unsigned char Layer;
  //��ǰ���ڵڼ����˵�λ��
  unsigned char Index;
  //���������ֵ,���ڼ���ϵͳǶ�׵���
  unsigned char Key;
  //��ر�־,������
  unsigned char Flag;
  
  //��������ʱ,�������ڲ�����:
  unsigned char WrIndex;//������ֵʱ��λ��
  unsigned char WrMaxPos;//������ֵʱ�����Χ,��
  signed short OrgAdj;  //�������ʱ�ĵ�����ԭʼֵ
  
   //�û��ҽӵĲ˵����ұ�,��λΪ���ұ��С. ��ʹ�ô˻���ʵ��Ȩ�޷���
  const unsigned char *pLUT;  
  //�û������ӿ�����
  const struct _QMenuFun *const *pFunAry;
  //���ڵ����Ĳ���
  struct _QMenuPara Para;  
};

//����,�˵���μ�QMenu�еĲ�ζ���:

//��ر�־����Ϊ:
#define QMENU_MNG_ID_MASK   0x3F     //������ʾʱ,������ID��

#define QMENU_MNG_REFRESH_NOW 0x40   //����ˢ�±�־

//����λǿ��������ʾ��־,���򰴼���ʹ����λǿ����ʾ,�˱�־�ڵ���DISP���Զ�ȡ��
#define QMENU_MNG_FLASH_HIDDEN   0x80  

/***********************************************************************
                        ��غ���
***********************************************************************/

//--------------------------������--------------------------
//���˺�������ϵͳ250mS������,���ڸ���ʵʱֵ����ʾ
void QMenuMng_Task(struct _QMenuMng *pMng);

//----------------------���²˵���ʾ����--------------------------
//���ô˺����������½�����ʾ(�㶯���������ڵ���)
void QMenuMng_UpdateDisp(struct _QMenuMng *pMng);

//------------------------����װ���û�����------------------------
void QMenuMng_ReloadPara(struct _QMenuMng *pMng);

//----------------------����˵���ʼ������--------------------------
//��������׼������˵�ʱ���ùҽӵĲ˵����ұ�����ڲ���ʼ��
//pLUT: pFunAry�Ĳ��ұ����λΪ�����������Ӧ��ID
//�������QMenuMng_Key(QMENU_MNG_KEY_ENTER)������ʽ����˵�
void QMenuMng_EnterInit(struct _QMenuMng *pMng,
                        const struct _QMenuFun *const *pFunAry,//����ΪNULL
                        const unsigned char *pLUT);//����ΪNULL

//----------------------------�˳��˵�����--------------------------
//�ڲ˵�״̬,�ⲿǿ���˳��˵�ʱ���ô˺���
void QMenuMng_Exit(struct _QMenuMng *pMng);
                  
//----------------------------��ʼ������----------------------------
//����ʱ����
void QMenuMng_Init(struct _QMenuMng *pMng,
                    unsigned char Id); //�����ID��

//----------------------�Ӽ�������--------------------------
//�β�Key��λ����Ϊ:
#define QMENU_MNG_KEY_MASK   0x0f  //��ֵ��������:
#define QMENU_MNG_KEY_MODE   0x70  //������ʽ����
#define QMENU_MNG_KEY_UP     0x01  //�ϼ�
#define QMENU_MNG_KEY_DOWN   0x02  //�¼�
#define QMENU_MNG_KEY_ENTER  0x04  //�����
#define QMENU_MNG_KEY_EXIT   0x08  //�˳���

#define QMENU_MNG_KEY_TYPE_MASK 0x70//�������ͣ�
#define QMENU_MNG_KEY_SHORT  0x10  //�̰���
#define QMENU_MNG_KEY_LONG   0x20  //������
#define QMENU_MNG_KEY_KEEP   0x40  //���ְ���(��ס����)

#define QMENU_MNG_KEY_USER   0x80  //�û���������(�ڲ���ʹ��)

//void QMenuMng_Key(unsigned char Key);
#define QMenuMng_Key(pmng, key) do{(pmng)->Key = key;}while(0)

//--------------------------����������--------------------------
//���˺�������ϵͳ���ٽ�����,���ڴ����ֵ
void QMenuMng_FastTask(struct _QMenuMng *pMng);

//------------------------������ڵ�����˵�ָ������------------------------
//�������жϵ�ǰλ������ʾ��Ӧͼ����ʾ��,�˵��ڲſɵ���
#define QMenuMng_pGetCurFunAry(mng) ((mng)->pFunAry)

//------------------------������ڵ�����˵�ָ��------------------------
//�������жϵ�ǰλ������ʾ��Ӧͼ����ʾ��,�˵��ڲſɵ���
#define QMenuMng_pGetCurFun(mng) ((mng)->pFunAry[QMenuMng_GetId(mng)])

//-------------------------�õ���ǰ�˵�ID��--------------------
unsigned char QMenuMng_GetId(const struct _QMenuMng *pMng);

//-------------------------�õ�����ǰֵ-----------------------
//signed short QMenuMng_GetOrgAdj(struct _QMenuMng *pMng);
#define QMenuMng_GetOrgAdj(mng) ((mng)->OrgAdj)

//-------------------------�õ���ǰ����λ------------------------
//unsigned char QMenuMng_GetWrIndex(struct _QMenuMng *pMng);
#define QMenuMng_GetWrIndex(mng) ((mng)->WrIndex)

//------------------�ж��Ƿ���ʵʱ��ʾ״̬------------------------
signed char QMenuMng_IsRealDisp(struct _QMenuMng *pMng);

//---------------------------�õ��ڵ���ֵ------------------------
//���ڵ���ֵ�ڲ˵�δ�л�ʱ����ı�
//unsigned short QMenuMng_GetAdj(struct _QMenuMng *pMng);
#define QMenuMng_GetAdj(mng) ((mng)->Para.Adj)

//---------------------------�õ��˵����------------------------
//���ڵ���ֵ�ڲ˵�δ�л�ʱ����ı�
//unsigned char QMenuMng_GetLayer(struct _QMenuMng *pMng);
#define QMenuMng_GetLayer(mng) ((mng)->Layer)

/***********************************************************************
                              �ص�����
***********************************************************************/
#if defined(SUPPORT_QMENU_MAP_DISP_OTHER) || defined(SUPPORT_QMENU_PIXEL) 
  #include "QMenuMng_cbDispOther.h"
#else
  #include "QMenuMng_cbDisp.h"
#endif

//---------------------------�˳��˵�ϵͳͨ��-----------------------
void QMenuMng_cbExitNotify(unsigned char MngId);


#endif //_QMENU_MNG_H
