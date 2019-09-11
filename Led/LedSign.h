/***************************************************************************
                       数码管字模定义-统一按共阴定义
***************************************************************************/

#ifndef __LED_SIGN_H
#define __LED_SIGN_H

//------------------------------段码定义------------------------------------
//段码定义 标准时
#ifdef SUPPORT_LED_SIGN_01234567
  #include "LedSign_01234567.h"
#endif

//段码定义
#ifdef SUPPORT_LED_SIGN_76412053
  #include "LedSign_76412053.h"
#endif

//段码定义
#ifdef SUPPORT_LED_SIGN_27510463
  #include "LedSign_27510463.h"
#endif

//段码定义
#ifdef SUPPORT_LED_SIGN_20643175
  #include "LedSign_20643175.h"
#endif

//段码定义
#ifdef SUPPORT_LED_SIGN_76421053
  #include "LedSign_76421053.h"
#endif

#ifdef SUPPORT_LED_SIGN_01357624
  #include "LedSign_01357624.h"
#endif

#ifdef SUPPORT_LED_SIGN_06421753
  #include "LedSign_06421753.h"
#endif


//------------------------------指示灯定义------------------------------------
#define LED_SIGN_DLIGHT_POS    4     //双色指示灯所在Buf位置,固定为4
//绿灯与红灯在BIT上的位置选择如下定义之一:

#ifdef SUPPORT_DLIGHT_SIGN_G0R1 //绿灯0红灯1时
  #define LED_SIGN_DLIGHT_MASK   0x03  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x02  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x01  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G1R0 //绿灯1红灯0时
  #define LED_SIGN_DLIGHT_MASK   0x03  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x01  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x02  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G1R2 //绿灯1红灯2时
  #define LED_SIGN_DLIGHT_MASK   0x06  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x04  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x02  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G4R2 //绿灯4红灯2时
  #define LED_SIGN_DLIGHT_MASK   0x14  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x04  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x10  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G0R7 //绿灯0红灯7时
  #define LED_SIGN_DLIGHT_MASK   0x81  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x80  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x01  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G6R7 //绿灯6红灯7时
  #define LED_SIGN_DLIGHT_MASK   0xC0  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x80  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x40  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G0R6 //绿灯0红灯6时
  #define LED_SIGN_DLIGHT_MASK   0x41  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x40  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x01  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_G6R0 //绿灯6红灯0时
  #define LED_SIGN_DLIGHT_MASK   0x41  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x01  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x40  //双色指示灯中的绿灯掩码
#endif

#ifdef SUPPORT_DLIGHT_SIGN_NONE   //不支持此程方式获取指示灯时
  #define LED_SIGN_DLIGHT_MASK   0x00  //双色指示灯掩码
  #define LED_SIGN_DLIGHT_RED    0x00  //双色指示灯中的红灯掩码
  #define LED_SIGN_DLIGHT_GREEN  0x00  //双色指示灯中的绿灯掩码
#endif


//---------------------------4位共阳数码管显示0-9字模---------------------------
extern __flash const unsigned char LED_SIGN_NumLut[];

#endif //#ifndef __LED_SIGN_H
