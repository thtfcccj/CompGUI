/*******************************************************************************

                       �������ģ��
��ģ�������˲�ͬ�ֿ������ļ�������ͳһ���Ʋ�ͬ��������ֻ�����
********************************************************************************/
#ifndef _FONTS_H
#define _FONTS_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

#include "Color.h"

/******************************************************************************
                             ��ؽṹ
*******************************************************************************/

//֧�ֵ�����������ͣ�����Դ��
#ifndef FONTS_ID_MAX
  #define FONTS_ID_MAX   8
#endif

//֧�ֵ�����ַ������ȣ������˳��Ȳ��ٻ���
#ifndef FONTS_STR_MAX
  #define FONTS_STR_MAX   (320 / 8)
#endif



/******************************************************************************
                             ��ؽṹ
*******************************************************************************/

//�����������ɹ̻�����Դ�У�ÿ���̶�ռ��32�ֽڣ���ɳ������FONTS_ID_MAX��
struct _FontsDesc{
  unsigned long Base;        //�����FLASH�еĻ�ַ
  signed short  w;           //�������
  signed short  h;           //����߶�
  unsigned char Type;       //ȡģ���ͣ�������
  unsigned char Note[25];   //չʾ����ʾ���ϵ�������Ϣ
};

//ȡģ���Ͷ���Ϊ��
//ǰ3ΪASC���壺
#define FONTS_ASC_STD   0  //ASC�����׼�ֿ� ��0~127���򣬹�127��
#define FONTS_ASC_NANO  1 //ASC������ֿ� ��32~127���򣬹�96��
#define FONTS_ASC_NUM   2 //ASC���ֵ����ֿ� ��ASC��:0X2A-0X39, ��*+-.,/��0~9�� ��16��
//3-6ΪGB2312���ͣ�
#define FONTS_2312_STD  3 //GB2312�����׼�ֿ�: ����׼GB2312�֣�����������
#define FONTS_2312_NOR  4 //GB2312������ֿ�: ȥ��GB2312���еĿհ�����(AA~AF)
#define FONTS_2312_NANO 5 //GB2312���󾫼��ֿ�: ȥ��GB2312���еĿհ�(AA~AF)�벻����(A4-A5��A7-A8)����


#define FONTS_TYPE_COUNT 6 //���֧������
/*******************************************************************************
                              ��غ���
********************************************************************************/

//------------------------------����һ��ָ�������ַ���------------------------------
//����ǰ����ǰ���ú�ǰ���뱳��,�������ʱ���ײ�����
signed char Font_PlotLine(const struct _FontsDesc *pHfonts,//���ʱʹ�õ�����
                       const struct _FontsDesc *pFfonts, //ȫ��ʱʹ�õ�����
                       unsigned short x,               //���ϵ�x��λ��
                       unsigned short y,               //���ϵ�y��λ��
                       const char *pString);           //Ҫ���Ƶ��ַ���

/*******************************************************************************
                              �ص�����
********************************************************************************/

//-------------------------��λ�õõ���ģ------------------------------------
//����Ϊ�����ڲ�ʱ��ֱ��ָ��ת��
#ifdef FONTS_INNER  
#define  Font_pGetZM(base,len) (const unsigned char*)(base)
//3. �ֿ�оƬ���ⲿFlash���ʵ�ֻ���:
#else
  const unsigned char *Font_pGetZM(unsigned long Base, unsigned char Len);      
#endif

#endif 

