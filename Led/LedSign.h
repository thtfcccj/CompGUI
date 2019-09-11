/***************************************************************************
                       �������ģ����-ͳһ����������
***************************************************************************/

#ifndef __LED_SIGN_H
#define __LED_SIGN_H

//------------------------------���붨��------------------------------------
//���붨�� ��׼ʱ
#ifdef SUPPORT_LED_SIGN_01234567
  #include "LedSign_01234567.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN_76412053
  #include "LedSign_76412053.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN_27510463
  #include "LedSign_27510463.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN_20643175
  #include "LedSign_20643175.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN_76421053
  #include "LedSign_76421053.h"
#endif

#ifdef SUPPORT_LED_SIGN_01357624
  #include "LedSign_01357624.h"
#endif

#ifdef SUPPORT_LED_SIGN_06421753
  #include "LedSign_06421753.h"
#endif


//------------------------------ָʾ�ƶ���------------------------------------
#define LED_SIGN_DLIGHT_POS    4     //˫ɫָʾ������Bufλ��,�̶�Ϊ4
//�̵�������BIT�ϵ�λ��ѡ�����¶���֮һ:

#ifdef SUPPORT_DLIGHT_SIGN_G0R1 //�̵�0���1ʱ
  #define LED_SIGN_DLIGHT_MASK   0x03  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x02  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x01  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G1R0 //�̵�1���0ʱ
  #define LED_SIGN_DLIGHT_MASK   0x03  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x01  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x02  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G1R2 //�̵�1���2ʱ
  #define LED_SIGN_DLIGHT_MASK   0x06  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x04  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x02  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G4R2 //�̵�4���2ʱ
  #define LED_SIGN_DLIGHT_MASK   0x14  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x04  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x10  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G0R7 //�̵�0���7ʱ
  #define LED_SIGN_DLIGHT_MASK   0x81  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x80  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x01  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G6R7 //�̵�6���7ʱ
  #define LED_SIGN_DLIGHT_MASK   0xC0  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x80  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x40  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G0R6 //�̵�0���6ʱ
  #define LED_SIGN_DLIGHT_MASK   0x41  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x40  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x01  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G6R0 //�̵�6���0ʱ
  #define LED_SIGN_DLIGHT_MASK   0x41  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x01  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x40  //˫ɫָʾ���е��̵�����
#endif

#ifdef SUPPORT_DLIGHT_SIGN_NONE   //��֧�ִ˳̷�ʽ��ȡָʾ��ʱ
  #define LED_SIGN_DLIGHT_MASK   0x00  //˫ɫָʾ������
  #define LED_SIGN_DLIGHT_RED    0x00  //˫ɫָʾ���еĺ������
  #define LED_SIGN_DLIGHT_GREEN  0x00  //˫ɫָʾ���е��̵�����
#endif


//---------------------------4λ�����������ʾ0-9��ģ---------------------------
extern __flash const unsigned char LED_SIGN_NumLut[];

#endif //#ifndef __LED_SIGN_H
