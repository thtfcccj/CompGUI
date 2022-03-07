/*******************************************************************************

          ePic格式图像子模块-绘制BMP图像非压缩引引图实现

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

//-----------------------------画非压缩索引位图-------------------------------------
//横向取模w*h点位,此函数不依赖调色板
//支持1,2,4,8位索引位图的绘制
void ePic_PlotIndexBmp(u16 x,u16 y)
{
  //缓冲以空间换时间
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;  
  unsigned char bitSize = ePicBuf.Header.DeepInfo;//色深，仅支持1，2，4，8值
  unsigned char mapSize =  ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = map + sizeof(Color_t) * mapSize;
  
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //显示缓冲行起始
  unsigned char bitMask = Plot_BitSize2Mask[bitSize];   //位掩码
  for(; h > 0; h++){//行为单位绘制
    u16 cw = w;
    do{//宽度一点点绘制
      unsigned char Data = *data++; //取位数据
      unsigned char curLen;
      if(cw < (unsigned short)bitSize) curLen = cw; //宽度对齐，余下丢弃
      else curLen = 8;
      for(unsigned char pos = 0; pos < curLen; pos += bitSize,cw--){
        //填充色
        unsigned char indexColor = (Data >> pos) & bitMask;
        pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, indexColor);
      }//end for
    }while(cw > 0);
    //下一行了
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
  }//end for h
}
