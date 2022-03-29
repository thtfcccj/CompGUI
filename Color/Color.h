/*******************************************************************************

                   彩色显示屏时,标准颜色定义接口

********************************************************************************/
#ifndef _COLOR_H
#define _COLOR_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

//-----------------------由显示方式决定的标准颜色-------------------------------
#ifdef SUPPORT_COLOR_RGB16   //RGB16显示方案(ARGB=0565)
   #include "Color_RGB16.h"
#endif

#ifdef SUPPORT_COLOR_ARGB16   //ARGB16显示方案(ARGB=1555)
   #include "Color_ARGB16.h"
#endif

#ifdef SUPPORT_COLOR_ARGB32   //ARGB32显示方案(ARGB=8888)
   #include "Color_ARGB32.h"
#endif

#ifdef SUPPORT_COLOR_INDEX   //自定义索引色显示方案(帧缓冲区直接填充索引色)
   #include "Color_Index.h"
#endif

#ifdef SUPPORT_COLOR_RGB666   //6*6*6索引色显示方案(帧缓冲区直接填充索引色)
   #include "Color_RGB666.h"

  //关于用户索引色支持说明:
  //6*6*6索引色可表示为216种色，使用u8存储时，用户色可定制,范围定义为:
  #define COLOR_USTART  COLOR_COUNT
  #define COLOR_UEND    255  //含
  #define COLOR_UCOUNT  (COLOR_UEND - COLOR_USTART + 1) //用户支持色种类
  //底层可通过TftDrv_pcbGetUserCLut()函数获取真实色查找表,内部定义并实现为:
  //COLOR_RGB666_UTYPE_S 常量表: 指定色此应用固定, 此为默认,不定义时支持
  //COLOR_RGB666_UTYPE_V  变量表为变量，即存放在RAM中，即可在应用中自由调整
  //COLOR_RGB666_UTYPE_P  变量表内有多个指针，可指定使用(固定或RAM)不同色系

  //-----------------------由产品配色式决定的自定义颜色----------------------------
  #include "Color_User.h"   //用户需同时实现Color_User.c
#endif


//-----------------------------指定色填充函数-----------------------------------
void Color_Full(Color_t C, Color_t *pC, unsigned char cCount);

//-------------------------------颜色copy函数-----------------------------------
void Color_Copy(Color_t *pDist, const Color_t *pSource, unsigned char cCount);

//--------------------------RGB全色域与硬件支持色域相关--------------------------
#ifdef SUPPORT_RGB_HW_565  //565模式(全局里定义)
  //硬件颜色类型定义
  #define RgbHwType  unsigned short 
  //RGB全色域转换为硬件支持类型
  #define RGB2Hw(RGB24) ((((unsigned long)(RGB24) >> 3) & 0x001f) |\
                         (((unsigned long)(RGB24) >> (8 + 2 - 5)) & 0x07E0) |\
                         (((unsigned long)(RGB24) >> (16 + 3 - 11)) & 0xF800))
#else  //全色域模式
  //硬件颜色类型定义
  #define RgbHwType  unsigned long
  //RGB全色域转换为硬件支持类型
  #define RGB2Hw(RGB24)  (RGB24)
#endif

#endif //_COLOR_H




