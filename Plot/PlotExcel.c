/*******************************************************************************

                          Plot���Ʊ����ʵ��

*******************************************************************************/
#include "PlotExcel.h"

#include <String.h>

/*******************************************************************************
                                 ��غ���ʵ��
*******************************************************************************/
//------------------------------------��ʼ������--------------------------------
//ע������Ϊ�Ӵ�����ʾʱ��Ӧ�ÿհ״�����Ϊ�丸����
void PlotExcel_Init(struct _PlotExcel *pExcel,
                       const struct _PlotExcelData *pData,  //�û����ݽӿ�
                       const struct _PlotExcelInfo *pInfo)  //�����Ϣ
{
  memset(pExcel, 0, sizeof(struct _PlotExcel));  //��ʼ����0
  pExcel->pData = pData; 
  pExcel->pInfo = pInfo;
  PlotExcel_ReBulid(pExcel);
}

//-------------------------�����������ͨ��-------------------------------------
//�������ݺ󣬵��ô˺���˵�����ݸ��������ˢ��
void PlotExcel_UpdateDataFinal(struct _PlotExcel *pExcel,
                                signed short Line)//��ʱ��ʾ����ȫҳ,����Ϊ�����
{
  const struct _PlotExcelInfo *pInfo = pExcel->pInfo;
  const struct _PlotExcelData *pData = pExcel->pData; 
  unsigned short ItemCount = pData->GetItemCount(pData);
  unsigned short StartLine, EndLine;
  if(Line >= 0){ //���¶�Ӧ��
    StartLine = Line;
    EndLine = StartLine;
  }
  else{//ȫ��
    pExcel->Flag |= PLOT_EXCEL_DATA_FINAL;  
    StartLine = 0;
    EndLine = (pInfo->h >> 4) - 1; //�߶�
  }
  unsigned short Item = pExcel->StartItem;
  
  unsigned short y = pInfo->y;
  if(pInfo->HeaderPen != pInfo->HeaderBrush) y += 16;//�б���ͷʱ
  y += ((unsigned short)StartLine << 4);// *16;

  Plot_CfgPalette(pInfo->DataPen, pInfo->DataBrush, PLOT_BRUSH_STYLE_FULL);
  unsigned char Len = pInfo->w >> 3;
  for(; StartLine <= EndLine; StartLine++, Item++, y += 16){
    if(Item < ItemCount){//�������
      unsigned char FullLen = PlotPB_StyleStr(pInfo->x, y, 
                                              pInfo->DataPen, pInfo->DataBrush,
                                              pData->pGetLine(pData, Item), 
                                              Len);
      if(FullLen < Len){//���������
        unsigned short xPos = ((unsigned short)(Len - FullLen) << 3);
        Plot_FullRect(pInfo->x + xPos, y, pInfo->w - xPos, 16);
      }
    }
    else{//���¿հ���
      Plot_FullRect(pInfo->x, y, pInfo->w, 16);
    }
  }
}

//-------------------------�ؽ�����-------------------------------------
//���¸��µ�ҳ�������ݲ���ʾ(���������ͷ)
void PlotExcel_ReBulid(struct _PlotExcel *pExcel)
{
  const struct _PlotExcelData *pData = pExcel->pData;
  
  //�����������
  if(pData->UpdateData(pData, pExcel->StartItem, pExcel->pInfo->w >> 3)){               
     PlotExcel_UpdateDataFinal(pExcel, -1);
  }
  else{//��������δ���
    pExcel->Flag &= ~PLOT_EXCEL_DATA_FINAL;
  }
}

//--------------------ˢ�º���------------------------------
void PlotExcel_Refurbish(struct _PlotExcel *pExcel)
{
  const struct _PlotExcelInfo *pInfo = pExcel->pInfo;
  //���±���ͷ
  if(pInfo->HeaderPen != pInfo->HeaderBrush){//�б���ͷʱ
    const struct _PlotExcelData *pData = pExcel->pData;
  unsigned char Len = pInfo->w >> 3;
    unsigned char FullLen = PlotPB_StyleStr(pInfo->x, pInfo->y, 
                                            pInfo->HeaderPen, pInfo->HeaderBrush,
                                            pData->pGetHeader(pData), Len);   
    if(FullLen < Len){//���������
        unsigned short xPos = ((unsigned short)(Len - FullLen) << 3);
        Plot_FullRect(pInfo->x + xPos, 0, pInfo->w - xPos, 16);
    }
  }
  //��������
  PlotExcel_UpdateDataFinal(pExcel, -1);
}
