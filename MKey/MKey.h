/*****************************************************************************

//		               ����������ģ���׼�ӿ�
//��ģ��ΪKeyģ��Ķ�������������Keyģ��ͬʱʹ��
//���ֲɼ���ʽ��
//SUPPORT_MKEY_ID��ʽ���Ψһ��ֵ������ʶ����ϼ�
//SUPPORT_MKEY_MASK��ʽ���ÿ�����İ���״̬����ʶ����ϼ�
****************************************************************************/
#ifndef __MKEY_H
#define __MKEY_H

/*******************************************************************************
					                  ȫ�ֶ���˵��
********************************************************************************/

//�Ƿ�֧��ȫ�ּ���,ȫ���ﶨ��
//#define SUPPORT_MKEY_ACCELERATE

/*******************************************************************************
					                  �ڲ�������
********************************************************************************/

#include "MKeyDef.h"        //�û���˽��ʵ��

#ifdef SUPPORT_MKEY_ID 
  #define MKeySize_t    unsigned char  //���趨���ֵ
  #define _INVALID_MKEY    0xff         //������Ч��ֵ
#endif

#ifdef SUPPORT_MKEY_MASK //����ʱ��������Ĭ��ֻ��Ӧ5������
  #ifndef MKeySize_t
    #define MKeySize_t    unsigned char
  #endif
  #define _INVALID_MKEY    0         //������Ч��ֵ
#endif

//������MKey_AccelerateTask()����������,������������
#define MKEY_ACCELERATE_MAX     23

//���ٹ�����ʱ����ֵ�����ж����������󣬻�Ҫ����ʱһ��ʱ�䣬�Ż�������
//��λΪTask����ʱ��
#define MKEY_ACCELERATE_DELAY   (MKEY_ACCELERATE_MAX * 2)

/*******************************************************************************
					                     ��ؽṹ
*******************************************************************************/

//���ٹ��ܽṹ:
struct _MKeyAccelerate{
  unsigned char Gene;   //�������ӣ���������Ƶ�ʺ͸�ֵ������
  unsigned char Index; //������
};

//����������
struct _MKey{
  unsigned char AryId;   //ȫ�ַ����ID��
  MKeySize_t MKeyId;      //����ֵID 
  unsigned char Index;		//����������
  #ifdef SUPPORT_MKEY_ACCELERATE  //֧�ְ�������ʱ
    struct _MKeyAccelerate Accelerate;
  #endif 
};

/*******************************************************************************
					                     ��غ���
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void MKey_Init(struct _MKey *pKey, unsigned char AryId);

//------------------------------------������----------------------------------
//ÿ8ms����һ���Ի�ȡ��ֵ
void MKey_Task(struct _MKey *pKey);

//----------------------------��������������-----------------------------
//����16��32ms������
#ifdef SUPPORT_MKEY_ACCELERATE  //֧�ְ�������ʱ
  void MKey_AccelerateTask(struct _MKey *pKey);
#else
  #define MKey_AccelerateTask(pkey)  do{}while(0)
#endif

//----------------------------------������һ�μ�ֵ����------------------------
#define MKey_GetMKeyId(mKey)   ((mKey)->MKeyId)

/*******************************************************************************
					                    �ص�����-�ײ�
*******************************************************************************/
//--------------------------------��ʼʼIO-------------------------------------
#include "IoCtrl.h" //ֱ��ʵ��
#define MKey_cbCfgIO(aryId)       CfgMKey(aryId)

//---------------------------------�õ�ԭʼ��ֵ����-----------------------------
//SUPPORT_MKEY_IDʱ��ʵ��
unsigned char MKey_cbGetOrgMKeyId(unsigned char AryId);

//-----------------------------�ж϶�Ӧ���Ƿ���-----------------------------
//SUPPORT_MKEY_MASKʱ��ʵ��
unsigned char MKey_cbIsMKeySel(unsigned char AryId, unsigned char MKeySel);

//------------------------�жϵ�ǰ���ְ����Ƿ�Ϊ���ټ�------------------------
//SUPPORT_MKEY_ACCELERATEʱ��ʵ��,һ�㽫���¼�����Ϊ���ټ�
unsigned char MKey_cbIsAccelarateMKey(unsigned char AryId, MKeySize_t MKey);

/*******************************************************************************
					                    �ص�����-�ϲ�
*******************************************************************************/

//----------------------------�������ֵID�õ���ֵ-----------------------------
//SUPPORT_MKEY_IDʱ��ʵ��
MKeySize_t MKey_cbMKeyId2MKey(unsigned char AryId, MKeySize_t MKeyId);

//-------------------------------�̰���ͨ������---------------------------------
//�ûص��ķ�ʽʵ�ְ�������
void MKey_cbShortNotify(unsigned char AryId, MKeySize_t MKey);

//-------------------------------������ͨ������---------------------------------
//��֧��ʱ��ʵ��Ϊ��
void MKey_cbLongNotify(unsigned char AryId, MKeySize_t MKey);

//-------------------------------���ְ���ͨ������------------------------------
//��֧��ʱ��ʵ��Ϊ��
void MKey_cbKeepNotify(unsigned char AryId, MKeySize_t MKey);

#endif

