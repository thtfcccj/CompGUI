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
  const struct _QMenuFun *pFunAry;
  //���ڵ����Ĳ���
  struct _QMenuPara Para;  
};

//����,�˵���μ�QMenu�еĲ�ζ���:

//��ر�־����Ϊ:
#define QMENU_MNG_ID_MASK   0x3F   //������ʾʱ,������ID��  

//����λǿ��������ʾ��־,���򰴼���ʹ����λǿ����ʾ,�˱�־�ڵ���DISP���Զ�ȡ��
#define QMENU_MNG_FLASH_HIDDEN   0x80  

/***********************************************************************
                        ��غ���
***********************************************************************/

//--------------------------������--------------------------
//���˺�������ϵͳ250mS������,���ڸ���ʵʱֵ����ʾ
void QMenuMng_Task(struct _QMenuMng *pMng);

//----------------------���²˵���ʾ����--------------------------
//���ô˺����������½�����ʾ
void QMenuMng_UpdateDisp(struct _QMenuMng *pMng);

//----------------------����˵���ʼ������--------------------------
//��������׼������˵�ʱ���ùҽӵĲ˵����ұ�����ڲ���ʼ��
//��pLUT != NULL �������QMenuMng_Key(QMENU_MNG_KEY_ENTER)��ʽ����
void QMenuMng_EnterInit(struct _QMenuMng *pMng,
                        const struct _QMenuFun *pFunAry,//����ΪNULL
                        const unsigned char *pLUT);

//----------------------�˳��˵�����--------------------------
//�ڲ˵�״̬,�ⲿǿ���˳��˵�ʱ���ô˺���
void QMenuMng_Exit(struct _QMenuMng *pMng);
                  
//---------------------��ʼ������----------------------------
//����ʱ����
//void QMenuMng_Init(struct _QMenuMng *pMng,
//                     const struct _QMenuFun *pFunAry,//����ΪNULL
#define QMenuMng_Init(mng) do{QMenuMng_EnterInit(mng,funAry,NULL);}while(0)

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

//-------------------------�õ���ǰ�˵�ID��--------------------
unsigned char QMenuMng_GetId(struct _QMenuMng *pMng);

//-------------------------�õ�����ǰ��ֵ-----------------------
//signed short QMenuMng_GetOrgAdj(struct _QMenuMng *pMng);
#define QMenuMng_GetOrgAdj(mng) ((mng)->OrgAdj)

//-------------------------�õ���ǰ����λ------------------------
//unsigned char QMenuMng_GetWrIndex(struct _QMenuMng *pMng);
#define QMenuMng_GetWrIndex(mng) ((mng)->WrIndex)

//------------------�ж��Ƿ���ʵʱ��ʾ״̬------------------------
signed char QMenuMng_IsRealDisp(struct _QMenuMng *pMng);

/***********************************************************************
                    �ص�����
***********************************************************************/
#include "QMenuMng_cbDisp.h"

//---------------------------�˳��˵�ϵͳͨ��-----------------------
void QMenuMng_cbExitNotify(unsigned char MngId);


#endif //_QMENU_MNG_H
