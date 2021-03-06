/*******************************************************************************

                   彩色显示屏时,标准颜色定义接口

********************************************************************************/
#ifndef _COLOR_H
#define _COLOR_H

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
#endif

//-----------------------由产品配色式决定的自定义颜色----------------------------
#include "Color_User.h"   //用户需同时实现Color_User.c

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




