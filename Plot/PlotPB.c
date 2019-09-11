/*******************************************************************************
                         绘图库标准接口
此接口依赖于TftDrv提供的帧缓冲
********************************************************************************/

#include "PlotPB.h"
#include "TftDrv.h"  //显示缓冲区
#include    "GB2312ZM.h"  //取字模依赖

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
  Color_t *pBuf = &TftDrv_Buf[y][x]; 
  const unsigned char *mask = GB2312ZM_pGetAsc(Code);
  const unsigned char *endmask = mask + 16; //字模结束位置
  
  for( ;mask < endmask; ){  
    unsigned char ZM = *mask++; //本行点阵->指向下行
    //填充本行8点颜色
    for(unsigned char Mask = 0x80; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = Pen;
      else  *pBuf = Brush;
    }  
    pBuf += (TFT_DRV_H_PIXEl - 8); //下一行填充位置起始
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
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  const unsigned char *mask = GB2312ZM_pGetHZ(Code);
  const unsigned char *endmask = mask + 32; //字模结束位置

  for( ;mask < endmask; ){  
    unsigned short ZM = *mask++; //横向左->指向横向右
    ZM <<= 8;
    ZM |= *mask++;   //横向右->指向下行横向左
    //填充本行16点颜色
    for(unsigned short Mask = 0x8000; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = Pen;
      else  *pBuf = Brush;
    }
    pBuf += (TFT_DRV_H_PIXEl - 16); //下一行填充位置起始
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

