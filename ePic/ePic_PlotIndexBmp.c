/*******************************************************************************

          ePic格式图像子模块-绘制BMP图像非压缩引引图实现

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

//-----------------------------画非压缩索引位图---------------------------------
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
  const unsigned char *data = ePic_pGetImageData();
  //计行每行数据占比，4字节对齐
  unsigned short DataW = 8 / bitSize;
  DataW = (w + (DataW - 1)) / DataW;
  if(DataW & 0x03) DataW += 4 - (DataW & 0x03);//4字节对齐
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //显示缓冲行起始  
  for(; h > 0; h--){//行为单位绘制
    pBuf = ePic_pPlotIndexLine(pBuf, map, mapSize, data, bitSize, w);//画当前行
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
    data += DataW; //下组数据
  }//end for h
}
