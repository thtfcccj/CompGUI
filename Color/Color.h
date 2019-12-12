/*******************************************************************************

                   ��ɫ��ʾ��ʱ,��׼��ɫ����ӿ�

********************************************************************************/
#ifndef _COLOR_H
#define _COLOR_H

//-----------------------����ʾ��ʽ�����ı�׼��ɫ-------------------------------
#ifdef SUPPORT_COLOR_RGB16   //RGB16��ʾ����(ARGB=0565)
   #include "Color_RGB16.h"
#endif

#ifdef SUPPORT_COLOR_ARGB16   //ARGB16��ʾ����(ARGB=1555)
   #include "Color_ARGB16.h"
#endif

#ifdef SUPPORT_COLOR_ARGB32   //ARGB32��ʾ����(ARGB=8888)
   #include "Color_ARGB32.h"
#endif

#ifdef SUPPORT_COLOR_INDEX   //�Զ�������ɫ��ʾ����(֡������ֱ���������ɫ)
   #include "Color_Index.h"
#endif

#ifdef SUPPORT_COLOR_RGB666   //6*6*6����ɫ��ʾ����(֡������ֱ���������ɫ)
   #include "Color_RGB666.h"
#endif

//-----------------------�ɲ�Ʒ��ɫʽ�������Զ�����ɫ----------------------------
#include "Color_User.h"   //�û���ͬʱʵ��Color_User.c


#endif //_COLOR_H




