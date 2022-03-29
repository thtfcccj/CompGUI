/*******************************************************************************

                   ��ɫ��ʾ��ʱ,��׼��ɫ����ӿ�

********************************************************************************/
#ifndef _COLOR_H
#define _COLOR_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

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

  //�����û�����ɫ֧��˵��:
  //6*6*6����ɫ�ɱ�ʾΪ216��ɫ��ʹ��u8�洢ʱ���û�ɫ�ɶ���,��Χ����Ϊ:
  #define COLOR_USTART  COLOR_COUNT
  #define COLOR_UEND    255  //��
  #define COLOR_UCOUNT  (COLOR_UEND - COLOR_USTART + 1) //�û�֧��ɫ����
  //�ײ��ͨ��TftDrv_pcbGetUserCLut()������ȡ��ʵɫ���ұ�,�ڲ����岢ʵ��Ϊ:
  //COLOR_RGB666_UTYPE_S ������: ָ��ɫ��Ӧ�ù̶�, ��ΪĬ��,������ʱ֧��
  //COLOR_RGB666_UTYPE_V  ������Ϊ�������������RAM�У�������Ӧ�������ɵ���
  //COLOR_RGB666_UTYPE_P  ���������ж��ָ�룬��ָ��ʹ��(�̶���RAM)��ͬɫϵ

  //-----------------------�ɲ�Ʒ��ɫʽ�������Զ�����ɫ----------------------------
  #include "Color_User.h"   //�û���ͬʱʵ��Color_User.c
#endif


//-----------------------------ָ��ɫ��亯��-----------------------------------
void Color_Full(Color_t C, Color_t *pC, unsigned char cCount);

//-------------------------------��ɫcopy����-----------------------------------
void Color_Copy(Color_t *pDist, const Color_t *pSource, unsigned char cCount);

//--------------------------RGBȫɫ����Ӳ��֧��ɫ�����--------------------------
#ifdef SUPPORT_RGB_HW_565  //565ģʽ(ȫ���ﶨ��)
  //Ӳ����ɫ���Ͷ���
  #define RgbHwType  unsigned short 
  //RGBȫɫ��ת��ΪӲ��֧������
  #define RGB2Hw(RGB24) ((((unsigned long)(RGB24) >> 3) & 0x001f) |\
                         (((unsigned long)(RGB24) >> (8 + 2 - 5)) & 0x07E0) |\
                         (((unsigned long)(RGB24) >> (16 + 3 - 11)) & 0xF800))
#else  //ȫɫ��ģʽ
  //Ӳ����ɫ���Ͷ���
  #define RgbHwType  unsigned long
  //RGBȫɫ��ת��ΪӲ��֧������
  #define RGB2Hw(RGB24)  (RGB24)
#endif

#endif //_COLOR_H




