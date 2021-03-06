/*******************************************************************************
                         绘图库标准接口
此接口依赖于TftDrv提供的帧缓冲
********************************************************************************/

#include "PlotPB.h"
#include "Plot_cbHw.h"//底层操作函数
#include "GB2312ZM.h"  //取字模依赖

#include <string.h> 

/****************************************************************************
							               内部函数实现	
****************************************************************************/

//---------------------带前景背景色绘制单个ASC字符---------------------------
//不支持背景色透明
void PlotPB_Asc(unsigned short x,
                unsigned short y,
                char Code,             //字符
                Color_t Pen,           //前景色
                Color_t Brush)         //背景色
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 8, 16);  
  const unsigned char *mask = GB2312ZM_pGetAsc(Code);
  const unsigned char *endmask = mask + 16; //字模结束位置
  
  for( ;mask < endmask; ){  
    unsigned char ZM = *mask++; //本行点阵->指向下行
    //填充本行8点颜色
    for(unsigned char Mask = 0x80; Mask > 0; Mask >>= 1){
      if(ZM & Mask) Plot_cbSetCurColor(pBuf, Pen);
      else Plot_cbSetCurColor(pBuf, Brush);
    }  
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 8); //下一行填充位置起始
  }
}

//-------------------带前景背景色绘制单个GB2312字符---------------------------
//不支持背景色透明
void PlotPB_GB2312(unsigned short x,
                   unsigned short y,
                   unsigned short Code,   //字符
                   Color_t Pen,            //前景色
                   Color_t Brush)           //背景色
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 16, 16);  
  const unsigned char *mask = GB2312ZM_pGetHZ(Code);
  const unsigned char *endmask = mask + 32; //字模结束位置

  for( ;mask < endmask; ){  
    unsigned short ZM = *mask++; //横向左->指向横向右
    ZM <<= 8;
    ZM |= *mask++;   //横向右->指向下行横向左
    //填充本行16点颜色
    for(unsigned short Mask = 0x8000; Mask > 0; Mask >>= 1){
      if(ZM & Mask) Plot_cbSetCurColor(pBuf, Pen);
      else Plot_cbSetCurColor(pBuf, Brush);
    }
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 16);//下一行填充位置起始
  } 
}

//--------------------------------填充固定长度字符------------------------------
//不支持背景色透明
void PlotPB_FullC(unsigned short x,
                  unsigned short y,
                  const char Code,     //字符
                  const Color_t Pen,   //前景色
                  Color_t Brush,         //背景色
                  unsigned char Len)    //长度
{          
  for(; Len > 0; Len--, x += 8){
    PlotPB_Asc(x, y, Code, Pen, Brush);
  }
}

//-------------------带独立前景与背景色绘制一组GB2312字符------------------------
//不支持背景色透明
void PlotPB_StrColor(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //字符
                      const Color_t *pPen,   //前景色
                      const Color_t *pBrush, //背景色
                      unsigned char Len)    //长度
{
  if(!Len) Len = strlen(pCode);
  for(; Len > 0; Len--){
    char c = *pCode++;
    //ASCII直接转换
    if(c < 0x80){
      PlotPB_Asc(x, y, c, *pPen, *pBrush);
    }
    //未检查GB2312正确性！！！！
    else{//取下半个字
      PlotPB_GB2312(x, y,((unsigned char)c << 8) | *pCode++, *pPen, *pBrush); //高位在前
      //后半字增加
      x += 8;
      pPen++;
      pBrush++;      
    }
    //半字统一增加
    x += 8;
    pPen++;
    pBrush++;
  }
}

//-------------------带独立前景单背景色绘制一组GB2312字符---------------------------
//不支持背景色透明
void PlotPB_StrColorP(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //字符
                      const Color_t *pPen,   //前景色
                      Color_t Brush,         //背景色
                      unsigned char Len)    //长度
{
  for(; Len > 0; Len--){
    char c = *pCode;
    //ASCII直接转换
    if(c < 0x80){
      PlotPB_Asc(x, y, c, *pPen, Brush);
    }
    //未检查GB2312正确性！！！！
    else{
      if(Len == 1) break;//半个汉字结束了，异常 
      pCode++; //取下半个字
      PlotPB_GB2312(x, y,
                    ((unsigned short)c << 8) | *pCode, //高位在前
                    *pPen, Brush); 
      //后半增加
      x += 8;
      pPen++;  
    }
    //各变量这里统一增加
    x += 8;
    pCode++;
    pPen++;
  }
}

//-------------------带单前景单背景色绘制一组GB2312字符-------------------------
//不支持背景色透明
void PlotPB_StrColorPB(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //字符
                      Color_t Pen,            //前景色
                      Color_t Brush,         //背景色
                      unsigned char Len)    //长度
{
  for(; Len > 0; Len--){
    char c = *pCode;
    //ASCII直接转换
    if(c < 0x80){
      PlotPB_Asc(x, y, c, Pen, Brush);
    }
    //未检查GB2312正确性！！！！
    else{
      if(Len == 1) break;//半个汉字结束了，异常 
      pCode++; //取下半个字
      PlotPB_GB2312(x, y,
                    ((unsigned short)c << 8) | *pCode, //高位在前
                    Pen, Brush); 
      //后半增加
      x += 8;
    }
    //各变量这里统一增加
    x += 8;
    pCode++;
  }
}

//---------------------------一组GB2312样式字绘制------------------------------
//不支持背景色透明,返回已填充字符长度
unsigned char PlotPB_StyleStr(unsigned short x,
                               unsigned short y,
                               Color_t Pen,          //固定前景色
                               Color_t Brush,         //固定背景色
                               const struct _StyleStr *pStyleStr,
                               unsigned char Len)    //指定填充长度,0自动
{
  const char *pStr = pStyleStr->pCode;
  unsigned char StrLen = strlen(pStr);
  if(Len && (StrLen > Len)) StrLen = Len; //超长了
  if(pStyleStr->pPen != NULL){//固定前景与背景色
    PlotPB_StrColorPB(x, y, pStr, Pen, Brush, StrLen);
  }
  else if(pStyleStr->pBrush != NULL){//固定背景色
    PlotPB_StrColorP(x, y, pStr, pStyleStr->pPen, Brush, StrLen);
  }
  else if(pStyleStr->pBrush != NULL){//可变前景与背景色
    PlotPB_StrColor(x, y, pStr, pStyleStr->pPen, pStyleStr->pBrush, StrLen);
  }
  return StrLen;
}






