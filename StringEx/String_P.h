/*****************************************************************************

                 string��غ���-���ֳ���/���ݿռ�ʱ��ͳһ�ӿ�
MCU���
*****************************************************************************/
#ifndef  __STRING_P_H
#define  __STRING_P_H  


#ifdef SUPPORT_AVR //AVR��Ƭ��ֱ�Ӷ�Ӧ
  #include "pgmspace.h"


#else //ʹ����ͨ��ʽ
  #include <string.h>
  #define memcpy_P(a,b,l) memcpy(a,b,l)


#endif


#endif //__PSTRING_H
