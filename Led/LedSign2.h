/***************************************************************************
                   �ڶ��������ģ����-ͳһ����������
***************************************************************************/

#ifndef __LED_SIGN2_H
#define __LED_SIGN2_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

//------------------------------���붨��------------------------------------
//���붨�� ��׼ʱ
#ifdef SUPPORT_LED_SIGN2_01234567
  #include "LedSign_01234567.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN2_76412053
  #include "LedSign_76412053.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN2_27510463
  #include "LedSign_27510463.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN2_20643175
  #include "LedSign_20643175.h"
#endif

//���붨��
#ifdef SUPPORT_LED_SIGN2_76421053
  #include "LedSign_76421053.h"
#endif

#ifdef SUPPORT_LED_SIGN2_01357624
  #include "LedSign_01357624.h"
#endif

#ifdef SUPPORT_LED_SIGN2_06421753
  #include "LedSign_06421753.h"
#endif

#ifdef SUPPORT_LED_SIGN2_76543210
  #include "LedSign_76543210.h"
#endif

#ifdef SUPPORT_LED_SIGN2_03457612
  #include "LedSign_03457612.h"
#endif

#ifdef SUPPORT_LED_SIGN2_21675430
  #include "LedSign_21675430.h"
#endif

#ifdef SUPPORT_LED_SIGN2_02476135
  #include "LedSign_02476135.h"
#endif

#ifdef SUPPORT_LED_SIGN2_71234650
  #include "LedSign_71234650.h"
#endif

#ifdef SUPPORT_LED_SIGN2_51432067
  #include "LedSign_51432067.h"
#endif

//---------------------------4λ�����������ʾ0-9��ģ---------------------------
extern __flash const unsigned char LED_SIGN2_NumLut[];

#endif //#ifndef __LED_SIGN2_H
