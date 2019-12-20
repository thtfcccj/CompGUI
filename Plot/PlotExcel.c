/*******************************************************************************

                          Plot绘制表格功能实现

*******************************************************************************/
#include "PlotExcel.h"

#include <String.h>

/*******************************************************************************
                                 相关函数实现
*******************************************************************************/
//------------------------------------初始化函数--------------------------------
//注：当作为子窗口显示时，应让空白窗口作为其父窗口
void PlotExcel_Init(struct _PlotExcel *pExcel,
                       const struct _PlotExcelData *pData,  //用户数据接口
                       const struct _PlotExcelInfo *pInfo)  //相关信息
{
  memset(pExcel, 0, sizeof(struct _PlotExcel));  //初始化到0
  pExcel->pData = pData; 
  pExcel->pInfo = pInfo;
  PlotExcel_ReBulid(pExcel);
}

//-------------------------更新数据完成通报-------------------------------------
//更新数据后，调用此函数说明数据更新完成以刷屏
void PlotExcel_UpdateDataFinal(struct _PlotExcel *pExcel,
                                signed short Line)//负时表示更新全页,否则为相对行
{
  const struct _PlotExcelInfo *pInfo = pExcel->pInfo;
  const struct _PlotExcelData *pData = pExcel->pData; 
  unsigned short ItemCount = pData->GetItemCount(pData);
  unsigned short StartLine, EndLine;
  if(Line >= 0){ //更新对应行
    StartLine = Line;
    EndLine = StartLine;
  }
  else{//全部
    pExcel->Flag |= PLOT_EXCEL_DATA_FINAL;  
    StartLine = 0;
    EndLine = (pInfo->h >> 4) - 1; //高度
  }
  unsigned short Item = pExcel->StartItem;
  
  unsigned short y = pInfo->y;
  if(pInfo->HeaderPen != pInfo->HeaderBrush) y += 16;//有标题头时
  y += ((unsigned short)StartLine << 4);// *16;

  Plot_CfgPalette(pInfo->DataPen, pInfo->DataBrush, PLOT_BRUSH_STYLE_FULL);
  unsigned char Len = pInfo->w >> 3;
  for(; StartLine <= EndLine; StartLine++, Item++, y += 16){
    if(Item < ItemCount){//填充内容
      unsigned char FullLen = PlotPB_StyleStr(pInfo->x, y, 
                                              pInfo->DataPen, pInfo->DataBrush,
                                              pData->pGetLine(pData, Item), 
                                              Len);
      if(FullLen < Len){//后面填清除
        unsigned short xPos = ((unsigned short)(Len - FullLen) << 3);
        Plot_FullRect(pInfo->x + xPos, y, pInfo->w - xPos, 16);
      }
    }
    else{//更新空白区
      Plot_FullRect(pInfo->x, y, pInfo->w, 16);
    }
  }
}

//-------------------------重建函数-------------------------------------
//重新更新当页所有数据并显示(不负责标题头)
void PlotExcel_ReBulid(struct _PlotExcel *pExcel)
{
  const struct _PlotExcelData *pData = pExcel->pData;
  
  //更新数据完成
  if(pData->UpdateData(pData, pExcel->StartItem, pExcel->pInfo->w >> 3)){               
     PlotExcel_UpdateDataFinal(pExcel, -1);
  }
  else{//更新数据未完成
    pExcel->Flag &= ~PLOT_EXCEL_DATA_FINAL;
  }
}

//--------------------刷新函数------------------------------
void PlotExcel_Refurbish(struct _PlotExcel *pExcel)
{
  const struct _PlotExcelInfo *pInfo = pExcel->pInfo;
  //更新标题头
  if(pInfo->HeaderPen != pInfo->HeaderBrush){//有标题头时
    const struct _PlotExcelData *pData = pExcel->pData;
  unsigned char Len = pInfo->w >> 3;
    unsigned char FullLen = PlotPB_StyleStr(pInfo->x, pInfo->y, 
                                            pInfo->HeaderPen, pInfo->HeaderBrush,
                                            pData->pGetHeader(pData), Len);   
    if(FullLen < Len){//后面填清除
        unsigned short xPos = ((unsigned short)(Len - FullLen) << 3);
        Plot_FullRect(pInfo->x + xPos, 0, pInfo->w - xPos, 16);
    }
  }
  //更新内容
  PlotExcel_UpdateDataFinal(pExcel, -1);
}
