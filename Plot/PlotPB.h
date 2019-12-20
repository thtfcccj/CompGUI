/*******************************************************************************
                        绘图库标准接口-独立函数功能(带前景背景色)
此接口依赖于TftDrv提供的帧缓冲, 坐标零点：左上角
此接口提供的函数不依赖于调色板，仅能实现前景背景色功能(PB为Pan and Brush简写)
********************************************************************************/
#ifndef _PLOT_PB_H
#define _PLOT_PB_H

#include "Color.h"

/*******************************************************************************
                             相关说明
********************************************************************************/

//接口函数及说明简写说明:
//x,y,w,h: 坐标x,y, 宽度，长度

//注：所有函数均未检查显示缓冲超限！！！！！！！！！！！ 

/*******************************************************************************
                             相关函数
*******************************************************************************/

//---------------------带前景背景色绘制单个ASC字符---------------------------
//不支持背景色透明
void PlotPB_Asc(unsigned short x,
                unsigned short y,
                char Code,             //字符
                Color_t Pen,           //前景色
                Color_t Brush);        //背景色

//----------------------带前景背景色绘制单个GB2312字符---------------------------
//不支持背景色透明
void PlotPB_GB2312(unsigned short x,
                   unsigned short y,
                   unsigned short Code,   //字符
                   Color_t Pen,            //前景色
                   Color_t Brush);         //背景色

//--------------------------------填充固定长度字符------------------------------
//不支持背景色透明
void PlotPB_FullC(unsigned short x,
                  unsigned short y,
                  const char Code,     //字符
                  const Color_t Pen,   //前景色
                  Color_t Brush,         //背景色
                  unsigned char Len);    //长度

//-------------------带独立前景背景色绘制一组GB2312字符------------------------
//不支持背景色透明
void PlotPB_StrColor(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //字符
                      const Color_t *pPen,   //前景色
                      const Color_t *pBrush, //背景色
                      unsigned char Len);    //长度

//-------------------带独立前景单背景色绘制一组GB2312字符-----------------------
//不支持背景色透明
void PlotPB_StrColorP(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //字符
                      const Color_t *pPen,   //前景色
                      Color_t Brush,         //背景色
                      unsigned char Len);    //长度

//-------------------带单前景单背景色绘制一组GB2312字符-------------------------
//不支持背景色透明
void PlotPB_StrColorPB(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //字符
                      Color_t Pen,            //前景色
                      Color_t Brush,         //背景色
                      unsigned char Len);   //长度

//--------------------------------带样式字推荐结构定义--------------------------
struct _StyleStr{
  const char *pCode;     //字符
  const Color_t *pPen;   //前景色,为NULL时定义为固定前景与背景色
  const Color_t *pBrush; //背景色,为NULL时定义为固定背景色
};

//---------------------------一组GB2312样式字绘制------------------------------
//不支持背景色透明,返回已填充字符长度
unsigned char PlotPB_StyleStr(unsigned short x,
                               unsigned short y,
                               Color_t Pen,          //固定前景色
                               Color_t Brush,         //固定背景色
                               const struct _StyleStr *pStyleStr,
                               unsigned char Len);    //指定填充长度,0自动


#endif //_PLOT_H


