/*******************************************************************************

                          Plot绘制表格功能
 此模块使用Plot模块绘制一组表格，含标题头与内容(8X16或16X16点阵)
 此模块仅绘制，无光标，按键响应等功能。
*******************************************************************************/
#ifndef __PLOT_EXCEL_H 
#define __PLOT_EXCEL_H

/*******************************************************************************
                            相关结构
*******************************************************************************/
#include "PlotPB.h"
#include "Plot.h"

//-------------------------------相关辅助结构-----------------------------------
//数据及操作函数结构
struct _PlotExcelData{
  //得到数据行总数
  unsigned short(*GetItemCount)(const struct _PlotExcelData*);  
  //得到标题头 
  const struct _StyleStr*(*pGetHeader)(const struct _PlotExcelData*);
  //得到行数据,返回NULL表示结束
  const struct _StyleStr*(*pGetLine)(const struct _PlotExcelData*,
                                       unsigned short Line); //指定行   
  //更新数据(可实现为查找表项),返回是否更新完成
  signed char (*UpdateData)(const struct _PlotExcelData*,
                             unsigned short StartLine, //起始行 
                             unsigned short Count);    //总数
};

//相关信息:
struct _PlotExcelInfo{
  //管理区域:
  unsigned short x;    //x坐标
  unsigned short y;    //y坐标  
  unsigned short w;    //宽度  
  unsigned short h;    //高度
  //颜色
  Color_t HeaderBrush; //默认标题底色  ->标题底色与标题色相同时，表示无标题栏
  Color_t HeaderPen;   //默认标题色
  Color_t DataBrush;   //默认数据底色
  Color_t DataPen;     //默认数据色
};

//---------------------------------主结构---------------------------------------
struct _PlotExcel{
  //用户相关：
  const struct _PlotExcelData *pData;   //挂接的用户数据内容及操作函数
  const struct _PlotExcelInfo *pInfo;    //相关信息
  
  unsigned short StartItem;             //绝对数据的起始行
  unsigned char Flag;                   //相关标志，见定义
};

//相关标志定义为：
//数据准备完成标志,当数据不能及时更新时，此标志用于同步(内部使用)
#define   PLOT_EXCEL_DATA_FINAL   0x01

/*******************************************************************************
                                 相关函数
*******************************************************************************/

//------------------------------------初始化函数--------------------------------
//注：当作为子窗口显示时，应让空白窗口作为其父窗口
void PlotExcel_Init(struct _PlotExcel *pExcel,
                       const struct _PlotExcelData *pData,  //用户数据接口
                       const struct _PlotExcelInfo *pInfo);  //相关信息

//-------------------------更新数据完成通报-------------------------------------
//更新数据后，调用此函数说明数据更新完成以刷屏
void PlotExcel_UpdateDataFinal(struct _PlotExcel *pExcel,
                                signed short Line);//负时表示更新全页,否则为相对行

//--------------------------------重建函数--------------------------------------
//重新更新当页所有数据并显示(不负责标题头)
void PlotExcel_ReBulid(struct _PlotExcel *pExcel);

//-------------------------------------刷新函数---------------------------------
void PlotExcel_Refurbish(struct _PlotExcel *pExcel);


#endif //__PLOT_EXCEL_H

