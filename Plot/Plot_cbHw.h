/*******************************************************************************
                        ��ͼ���׼�ӿ�-�ײ���������ӿ�
ͨ���˽ӿ�ʹPlotģ������ڶȲ���ʾ��Ӳ��ʵ��
********************************************************************************/
#ifndef _PLOT_CB_HW_H
#define _PLOT_CB_HW_H

/*******************************************************************************
                              �ⲿ�Դ�ʵ��
********************************************************************************/
#ifdef SUPPORT_EX_VFB    //�ⲿ�Դ�ʵ��
#include "TftDbi.h"      //ͨ��DBI�ӿڲ�����ʾ���Դ�

//---------------------------���Զ�λĳ��λ����������-------------------------
//���ﷵ����ʾ��ַλ���Ա��´�ʹ��
//->���������Դ������Χ����Ԥ��д�Դ������һ��д�Դ�,����NULLָ��
Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
                              unsigned short w, unsigned short h);

//---------------------------����һ����ʼ-------------------------------------
//������ʾ��ɶ�λ����һ�У�������ʾ��ַλ���Ա��´�ʹ��
//void Plot_cbToNextRowStart(Color_t *pColor, unsigned short NextPosInfo);
//->������Ϊ����д�ڴ�ģʽ���Զ�ָ����һ�����ʴ˺����ݲ���ʵ��
#define Plot_cbToNextRowStart(pcolor, nextPosInfo)  ((pcolor) + (nextPosInfo))  

//----------------------------���õ�ǰλ����ɫ--------------------------------
//ͬʱָ��һ��д���λ��
//void Plot_cbSetCurColor(Color_t *pColor, Color_t Color);
//->������ǰԤ�ö�д�Դ��������ֱ��д���ݵ��Դ漴�ɡ�
#define Plot_cbSetCurColor(pcolor, cc) do{TftDbi_WrColor(cc); (pcolor)++; }while(0)

//----------------------------��������һλ��--------------------------------
//��д����ɫ�������´�д��λ��������һ����
//void Plot_cbUpdateNext(Color_t *pColor);
//��д��ʱ��������ʾ�޷�����һ�㣬������ʵ��Ϊ�����˵��Ե���һ���ָ�д�Դ�����
extern void Plot_cbUpdateNextFun(void);
#define Plot_cbUpdateNext(pcolor) do{Plot_cbUpdateNextFun(); (pcolor)++; }while(0)

/*******************************************************************************
                        Ĭ��ʹ���ڲ��Դ�ʵ��
********************************************************************************/
#else //�����Դ�:
#include "TftDrv.h"
//---------------------------���Զ�λĳ��λ����������-------------------------
//���ﷵ����ʾ��ַλ���Ա��´�ʹ��
//Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
//                              unsigned short w, unsigned short h);
#define Plot_cbAbsLocalArea(xX, yY, wW, hH)  (&TftDrv_Buf[yY][xX])  

//---------------------------����һ����ʼ-------------------------------------
//������ʾ��ɶ�λ����һ�У�������ʾ��ַλ���Ա��´�ʹ��
//void Plot_cbToNextRowStart(Color_t *pColor, unsigned short NextPosInfo);
#define Plot_cbToNextRowStart(pcolor, nextPosInfo)  ((pcolor) + (nextPosInfo))  

//----------------------------���õ�ǰλ����ɫ--------------------------------
//ͬʱָ��һ��д���λ��
//void Plot_cbSetCurColor(Color_t *pColor, Color_t Color);
#define Plot_cbSetCurColor(pcolor, cc)  do{*(pcolor) = cc; (pcolor)++; }while(0)

//----------------------------��������һλ��--------------------------------
//��д����ɫ�������´�д��λ��������һ����
//Color_t *pColor Plot_cbUpdateNext(Color_t *pColor);
#define Plot_cbUpdateNext(pcolor)  do{(pcolor)++; }while(0)


#endif 
#endif //_PLOT_CB_HW_H


