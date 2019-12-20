/*******************************************************************************

                          Plot���Ʊ����
 ��ģ��ʹ��Plotģ�����һ���񣬺�����ͷ������(8X16��16X16����)
 ��ģ������ƣ��޹�꣬������Ӧ�ȹ��ܡ�
*******************************************************************************/
#ifndef __PLOT_EXCEL_H 
#define __PLOT_EXCEL_H

/*******************************************************************************
                            ��ؽṹ
*******************************************************************************/
#include "PlotPB.h"
#include "Plot.h"

//-------------------------------��ظ����ṹ-----------------------------------
//���ݼ����������ṹ
struct _PlotExcelData{
  //�õ�����������
  unsigned short(*GetItemCount)(const struct _PlotExcelData*);  
  //�õ�����ͷ 
  const struct _StyleStr*(*pGetHeader)(const struct _PlotExcelData*);
  //�õ�������,����NULL��ʾ����
  const struct _StyleStr*(*pGetLine)(const struct _PlotExcelData*,
                                       unsigned short Line); //ָ����   
  //��������(��ʵ��Ϊ���ұ���),�����Ƿ�������
  signed char (*UpdateData)(const struct _PlotExcelData*,
                             unsigned short StartLine, //��ʼ�� 
                             unsigned short Count);    //����
};

//�����Ϣ:
struct _PlotExcelInfo{
  //��������:
  unsigned short x;    //x����
  unsigned short y;    //y����  
  unsigned short w;    //���  
  unsigned short h;    //�߶�
  //��ɫ
  Color_t HeaderBrush; //Ĭ�ϱ����ɫ  ->�����ɫ�����ɫ��ͬʱ����ʾ�ޱ�����
  Color_t HeaderPen;   //Ĭ�ϱ���ɫ
  Color_t DataBrush;   //Ĭ�����ݵ�ɫ
  Color_t DataPen;     //Ĭ������ɫ
};

//---------------------------------���ṹ---------------------------------------
struct _PlotExcel{
  //�û���أ�
  const struct _PlotExcelData *pData;   //�ҽӵ��û��������ݼ���������
  const struct _PlotExcelInfo *pInfo;    //�����Ϣ
  
  unsigned short StartItem;             //�������ݵ���ʼ��
  unsigned char Flag;                   //��ر�־��������
};

//��ر�־����Ϊ��
//����׼����ɱ�־,�����ݲ��ܼ�ʱ����ʱ���˱�־����ͬ��(�ڲ�ʹ��)
#define   PLOT_EXCEL_DATA_FINAL   0x01

/*******************************************************************************
                                 ��غ���
*******************************************************************************/

//------------------------------------��ʼ������--------------------------------
//ע������Ϊ�Ӵ�����ʾʱ��Ӧ�ÿհ״�����Ϊ�丸����
void PlotExcel_Init(struct _PlotExcel *pExcel,
                       const struct _PlotExcelData *pData,  //�û����ݽӿ�
                       const struct _PlotExcelInfo *pInfo);  //�����Ϣ

//-------------------------�����������ͨ��-------------------------------------
//�������ݺ󣬵��ô˺���˵�����ݸ��������ˢ��
void PlotExcel_UpdateDataFinal(struct _PlotExcel *pExcel,
                                signed short Line);//��ʱ��ʾ����ȫҳ,����Ϊ�����

//--------------------------------�ؽ�����--------------------------------------
//���¸��µ�ҳ�������ݲ���ʾ(���������ͷ)
void PlotExcel_ReBulid(struct _PlotExcel *pExcel);

//-------------------------------------ˢ�º���---------------------------------
void PlotExcel_Refurbish(struct _PlotExcel *pExcel);


#endif //__PLOT_EXCEL_H

