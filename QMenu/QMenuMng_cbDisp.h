/***********************************************************************

//		             �˵�ϵͳ֮4λ�����ʱ�Ĺ�����-��ʾ���ֻص��ӿ�
//��ģ�����Ϊ��������������ʱ��ʵ��Ϊֱ����Led.h�Խ�
***********************************************************************/
#ifndef _QMENU_MNG_CB_DISP_H
#define	_QMENU_MNG_CB_DISP_H

/***********************************************************************
                    �������
***********************************************************************/

//������ʱ֧��,����ȫ���ﶨ��
//  #define SUPPORT_QMENU_MNG_SINGLETON

/*********************************************************************
                    ���˵��
Ϊ��ֹ��ͬʵ��ʱ������ܶ��벻ͬ(��Ӳ������ģ������IO��ͬ��),������
MngId�ɽ�������
*********************************************************************/
//��ģ����ʾ���ֶ�����ϵͳ,��Ҫ4��"8.8.8.8"�Ķ�����ʾ,�û���ʵ���нӿ�
//��8.8.8.8���ⲿ��������:
//1.ÿ��8�ж�������ʾ����,��0~3��ʾ(˳���ָ��)
//2.ÿ��8�ж�������������,������������������,��λ��������Ӧ�þ���
//3.ÿ��С�������ʾ�������ɶ�������,��������������

#ifdef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  #include "Led.h"
  #include "LedSign.h"
#endif

//---------------------------4λ������---------------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  //������ʾ,Ϊ��ʱ��ʾ����,ָ��С����
  void QMenuMng_cbDispNum4(unsigned char MngId,
                           signed short Data,//Ҫ���Ե���
                           //0~1bit������ʾλ��-1(��������λ)-1 2bit:�Ƿ��������ʾ,
                           //3bit:������ʱ,��ʱ�Ƿ����0    4~7bitС������ʾ����
                           unsigned char Flag);
#else //������ʱ
  #include "LedNum4.h" //LedNum4_Disp();
  #define QMenuMng_cbDispNum4(mngId, vol, flag)  LedNum4_Disp(vol, flag)
#endif

//---------------------�ɵ���λ�õõ���ʾλ������----------------------
//QMenuMng�鶨���Ҳ�λ��ΪPos0,ͨ����ת��Ϊ��Ӧ����ܻ�С����λ��
//unsigned char QMenuMng_cbPos2Mask(unsigned char Pos);
#define QMenuMng_cbPos2Mask(mngId, pos) (1 << pos)

//---------------------��ʮ������ֵ�õ�������ʾ----------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
unsigned char QMenuMng_cbVol2Seg(unsigned char MngId, unsigned char Vol);
#else //������ʱ
  #define QMenuMng_cbVol2Seg(mngId,vol) LED_SIGN_NumLut[vol]
#endif

//---------------------�õ����ŵĶ����ʾ----------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  unsigned char QMenuMng_cbGetNegaSeg(unsigned char MngId);
#else //������ʱ  
  #define QMenuMng_cbGetNegaSeg(mngId) (LED_SIGN__)
#endif
//------------------------����ĳ����ʾ��������ʾ------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbSetSegDisp(unsigned char MngId, 
                             unsigned char Pos, unsigned char Seg);
#else //������ʱ
  #define QMenuMng_cbSetSegDisp(pos, seg) do{Led.SegDisp[pos] = seg;}while(0)
#endif
  
//------------------------ʹ�����Ӧ�����������------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
void QMenuMng_cbSetSegFlash(unsigned char MngId, 
                            unsigned char Mask);
#else //������ʱ
  #define QMenuMng_cbSetSegFlash(mask)  do{Led.SegFlash |= mask;}while(0)
#endif

//----------------------------ʹ���������ͣ��--------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbClrSegFlash(unsigned char MngId);
#else //������ʱ
  #define QMenuMng_cbClrSegFlash()  do{Led.SegFlash &= ~0x0f;}while(0)
#endif
  
//------------------------ʹ�����Ӧ��С������ʾ------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbSetDotDisp(unsigned char MngId, unsigned char Mask);
#else //������ʱ
  #define QMenuMng_cbSetDotDisp(mask)  do{Led.DotDisp |= mask;}while(0)
#endif
  
//------------------------ʹ����С����ͣ��------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbClrDotDisp(unsigned char MngId);
#else //������ʱ  
  #define QMenuMng_cbClrDotDisp()  do{Led.DotDisp &= ~0x0f;}while(0)
#endif
  
//------------------------ʹ�����Ӧ��С��������------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbSetDotFlash(unsigned char MngId, 
                              unsigned char Mask);
#else //������ʱ
  #define QMenuMng_cbSetDotFlash(mask)  do{Led.DotFlash |= mask;}while(0)
#endif
  
//----------------------------ʹ����С����ͣ��----------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbClrDotFlash(unsigned char MngId);
#else //������ʱ
  #define QMenuMng_cbClrDotFlash()  do{Led.DotFlash &= ~0x0f;}while(0)
#endif
  
//------------------------------ǿ�Ƹ����Լ���������-----------------------
//����ǿ�Ʊ�־,ͬʱˢ����ʾ��
//�β�Ϊ�Ƿ�ǿ������������
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  void QMenuMng_cbForceFlash(unsigned char MngId, unsigned char IsForceHidden);
#else //������ʱ
  #define QMenuMng_cbForceFlash(isforcehidden)\
    do{if(isforcehidden) Led.Flag |= (LED_FLASH_NOW | LED_FLASH_DIS);\
       else Led.Flag |= (LED_FLASH_NOW); Led_Task();}while(0)
#endif
         
//--------------������QMenuNote�ж����С����λ������----------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //������ʱ
  unsigned char QMenuMng_cbGetDotMask(unsigned char MngId); 
#else //������ʱ
  QMenuMng_cbGetDotMask(mngId)    LED_SIGN_DOT
#endif
    
#endif //_QMENU_MNG_CB_DISP_H
