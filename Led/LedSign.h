/***************************************************************************
                       �������ģ����-ͳһ����������
***************************************************************************/

#ifndef __LED_SIGN_H
#define __LED_SIGN_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

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

#ifdef SUPPORT_LED_SIGN_76543210
  #include "LedSign_76543210.h"
#endif

#ifdef SUPPORT_LED_SIGN_03457612
  #include "LedSign_03457612.h"
#endif

#ifdef SUPPORT_LED_SIGN_21675430
  #include "LedSign_21675430.h"
#endif

#ifdef SUPPORT_LED_SIGN_02476135
  #include "LedSign_02476135.h"
#endif

#ifdef SUPPORT_LED_SIGN_71234650
  #include "LedSign_71234650.h"
#endif

#ifdef SUPPORT_LED_SIGN_51432067
  #include "LedSign_51432067.h"
#endif

//---------------------------4λ�����������ʾ0-9��ģ---------------------------
extern __flash const unsigned char LED_SIGN_NumLut[];

#endif //#ifndef __LED_SIGN_H
