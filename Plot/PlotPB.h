/*******************************************************************************
                        ��ͼ���׼�ӿ�-������������(��ǰ������ɫ)
�˽ӿ�������TftDrv�ṩ��֡����, ������㣺���Ͻ�
�˽ӿ��ṩ�ĺ����������ڵ�ɫ�壬����ʵ��ǰ������ɫ����(PBΪPan and Brush��д)
********************************************************************************/
#ifndef _PLOT_PB_H
#define _PLOT_PB_H

#include "Color.h"

/*******************************************************************************
                             ���˵��
********************************************************************************/

//�ӿں�����˵����д˵��:
//x,y,w,h: ����x,y, ��ȣ�����

//ע�����к�����δ�����ʾ���峬�ޣ��������������������� 

/*******************************************************************************
                             ��غ���
*******************************************************************************/

//---------------------��ǰ������ɫ���Ƶ���ASC�ַ�---------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_Asc(unsigned short x,
                unsigned short y,
                char Code,             //�ַ�
                Color_t Pen,           //ǰ��ɫ
                Color_t Brush);        //����ɫ

//----------------------��ǰ������ɫ���Ƶ���GB2312�ַ�---------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_GB2312(unsigned short x,
                   unsigned short y,
                   unsigned short Code,   //�ַ�
                   Color_t Pen,            //ǰ��ɫ
                   Color_t Brush);         //����ɫ

//--------------------------------���̶������ַ�------------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_FullC(unsigned short x,
                  unsigned short y,
                  const char Code,     //�ַ�
                  const Color_t Pen,   //ǰ��ɫ
                  Color_t Brush,         //����ɫ
                  unsigned char Len);    //����

//-------------------������ǰ������ɫ����һ��GB2312�ַ�------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_StrColor(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //�ַ�
                      const Color_t *pPen,   //ǰ��ɫ
                      const Color_t *pBrush, //����ɫ
                      unsigned char Len);    //����

//-------------------������ǰ��������ɫ����һ��GB2312�ַ�-----------------------
//��֧�ֱ���ɫ͸��
void PlotPB_StrColorP(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //�ַ�
                      const Color_t *pPen,   //ǰ��ɫ
                      Color_t Brush,         //����ɫ
                      unsigned char Len);    //����

//-------------------����ǰ��������ɫ����һ��GB2312�ַ�-------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_StrColorPB(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //�ַ�
                      Color_t Pen,            //ǰ��ɫ
                      Color_t Brush,         //����ɫ
                      unsigned char Len);   //����

//--------------------------------����ʽ���Ƽ��ṹ����--------------------------
struct _StyleStr{
  const char *pCode;     //�ַ�
  const Color_t *pPen;   //ǰ��ɫ,ΪNULLʱ����Ϊ�̶�ǰ���뱳��ɫ
  const Color_t *pBrush; //����ɫ,ΪNULLʱ����Ϊ�̶�����ɫ
};

//---------------------------һ��GB2312��ʽ�ֻ���------------------------------
//��֧�ֱ���ɫ͸��,����������ַ�����
unsigned char PlotPB_StyleStr(unsigned short x,
                               unsigned short y,
                               Color_t Pen,          //�̶�ǰ��ɫ
                               Color_t Brush,         //�̶�����ɫ
                               const struct _StyleStr *pStyleStr,
                               unsigned char Len);    //ָ����䳤��,0�Զ�


#endif //_PLOT_H


