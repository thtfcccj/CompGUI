/*****************************************************************************

                 string相关函数-区分程序/数据空间时的统一接口
MCU相关
*****************************************************************************/
#ifndef  __STRING_P_H
#define  __STRING_P_H  


#ifdef SUPPORT_AVR //AVR单片机直接对应
  #include "pgmspace.h"


#else //使用普通方式
  #include <string.h>
  #define memcpy_P(a,b,l) memcpy(a,b,l)


#endif


#endif //__PSTRING_H
