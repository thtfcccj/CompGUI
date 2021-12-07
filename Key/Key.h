/*****************************************************************************

//		               ����ģ���׼�ӿ�
//���ֲɼ���ʽ��
//SUPPORT_KEY_ID��ʽ���Ψһ��ֵ������ʶ����ϼ�
//SUPPORT_KEY_MASK��ʽ���ÿ�����İ���״̬����ʶ����ϼ�
****************************************************************************/
#ifndef __KEY_H
#define __KEY_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*******************************************************************************
					                  ȫ�ֶ���˵��
********************************************************************************/

//�Ƿ�֧��Ӳ��������,��֧���ǲ�����ģ��(�����ȿ��Բ�֧��)
//#define SUPPORT_KEY

//ͨ������Ϊ����������ֱ�ͨ��,�������󣬼�ֵΪԭʼ����Ӧȷ����ֵ<64��
//#define SUPPORT_KEY_NOTIFY_ALONE

/*******************************************************************************
					                  �ڲ�������
********************************************************************************/
#ifdef SUPPORT_QGUI   //ʹ��QGUIʱר��ʵ��
  #include "Key_QGUI.h"
#else
  #include "KeyDef.h"        //�û���˽��ʵ��
#endif

#ifdef SUPPORT_KEY_ID 
  #define KeySize_t    unsigned char  //���趨���ֵ
  #ifndef KEY_INVALID_KEY
    #define KEY_INVALID_KEY    0xff         //������Ч��ֵ
  #endif
#endif

#ifdef SUPPORT_KEY_MASK //����ʱ��������Ĭ��ֻ��Ӧ5������
  #ifndef KeySize_t
    #define KeySize_t    unsigned char
  #endif
  #ifndef KEY_INVALID_KEY
    #define KEY_INVALID_KEY    0         //������Ч��ֵ
  #endif
#endif

/*******************************************************************************
					                     ��ؽṹ
*******************************************************************************/
//����������
struct _Key{
  KeySize_t KeyId;         //����ֵID 
  unsigned char Index;		//����������
};

extern struct _Key Key;

/*******************************************************************************
					                     ��غ���
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void Key_Init(void);

//------------------------------------������----------------------------------
//ÿ8ms����һ���Ի�ȡ��ֵ
void Key_Task(void);

//----------------------------------������һ�μ�ֵ����------------------------
#define Key_GetKeyId()   (Key.KeyId)


/*******************************************************************************
					                    �ص�����-�ײ�
*******************************************************************************/
//--------------------------------��ʼʼIO--------------------------------------
#include "IoCtrl.h" //ֱ��ʵ��
#define Key_cbCfgIO()       CfgKey()

//---------------------------------�õ�ԭʼ��ֵ����-----------------------------
//SUPPORT_KEY_IDʱ��ʵ��
unsigned char Key_cbGetOrgKeyId(void);

//-----------------------------�ж϶�Ӧ���Ƿ���-----------------------------
//SUPPORT_KEY_MASKʱ��ʵ��
unsigned char Key_cbIsKeySel(unsigned char KeySel);

/*******************************************************************************
					                    �ص�����-�ϲ�
*******************************************************************************/

//----------------------------�������ֵID�õ���ֵ-----------------------------
//SUPPORT_KEY_IDʱ��ʵ��
#ifdef SUPPORT_KEY_NOTIFY_ALONE //����IDʱ�����û���ת��
  #define Key_cbKeyId2Key(keyId)  (keyId)
#else //�ڴ˲�ʵ��
  KeySize_t Key_cbKeyId2Key(KeySize_t KeyId);
#endif

//-------------------------------��������ͨ������---------------------------------
//�ûص��ķ�ʽʵ�ְ�������
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  //KeyState����Ϊ�� b7:6: ����ͨ��״̬:0�̰�,1����,2���֣�3����(0xc0)���ͷ�(0xff)
  //                 b5:0: ��ǰ��ֵID
  void Key_cbNotify(KeySize_t KeyState);
#endif

//-------------------------------�̰���ͨ������---------------------------------
//�ûص��ķ�ʽʵ�ְ�������
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbShortNotify(key)  Key_cbNotify(0x00 | (key))
#else
  void Key_cbShortNotify(KeySize_t Key);
#endif 

//-------------------------------������ͨ������---------------------------------
//��֧��ʱ��ʵ��Ϊ��
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbLongNotify(key)  Key_cbNotify(0x40 | (key))
#else
  void Key_cbLongNotify(KeySize_t Key);
#endif 

//-------------------------------���ְ���ͨ������------------------------------
//��֧��ʱ��ʵ��Ϊ��
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbKeepNotify(key)  Key_cbNotify(0x80 | (key))
#else
  void Key_cbKeepNotify(KeySize_t Key);
#endif 

//------------------------------�ɿ�����ͨ������------------------------------
//������֧�ְ������٣���֧��ʱ��ʵ��Ϊ��
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbRlsNotify()  Key_cbNotify(0xff)
#else
  void Key_cbRlsNotify(void);
#endif 
#endif





