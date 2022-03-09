/*******************************************************************************

          ePic格式图像子模块-绘制BMP图像非压缩引引图实现

********************************************************************************/

#include "ePic.h"
#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

#include "DecodePNG.h"

static unsigned short _x;
static unsigned short _y;

//-----------------------------画PNG图-----------------------------------
//横向取模w*h点位,此函数不依赖调色板
//暂仅支持8B绘制
signed char ePic_PlotPNG(u16 x,u16 y)
{
  _x = x;
  _y = y;
  
  unsigned char DeepInfo = ePicBuf.Header.DeepInfo;
  if(DeepInfo & 0xe0 != (3 << 5)) return -1;  //图像类型：只支持索引彩色图像
  DeepInfo &= ~0x1f;  //保留色深 
  
  //b7仅idat数据,扫描方法(b6~5), 滤波器方法(b4~2), 压缩方法(b1~0)  
  unsigned char ZipInfo = ePicBuf.Header.ZipInfo;
  if(ZipInfo != 0) return -1;//只支持逐行扫，默认压缩与自动滤波器(索引格式无)
  
  //得到调色板信息
  unsigned short MapCount = ePicBuf.Header.PaletteCount;
  if(MapCount == 0) MapCount = 256;
   MapCount *= sizeof(Color_t);
  if(ePicBuf.NextDataSize <= MapCount) return -1;//异常

  //PNG译码
  return DecodePNG(DeepInfo,//色深,1,2,4,8,只仅支持索引色
                    ePicBuf.Header.w, //图像宽度
                    ePicBuf.Header.h,  //图像高度
                    ePicBuf.pNextData + MapCount,//idat区域的压缩数据
                    ePicBuf.NextDataSize - MapCount);    //idat区域数据长度
}

/*******************************************************************************
                              DecodePNG回调函数
********************************************************************************/

//---------------------------保存或绘制第n行数据---------------------------------
//此行数在DecodePNG()中调用,以保存或或绘制一行图像
void DecodePNG_cbOutLine(const unsigned char *data, //索引色或真实色
                          unsigned short Line)        //需要绘制的行,1开始
{
  static Color_t *pBuf; //静态以下次使用
  unsigned short w = ePicBuf.Header.w;
  //此应用作为绘图使用
  if(Line == 1)
    pBuf = Plot_cbAbsLocalArea(_x,_y, w, ePicBuf.Header.h); //第一行：显示缓冲行起始
  else
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
   
  const unsigned char *map = ePicBuf.pNextData;  
  unsigned char mapSize = ePicBuf.Header.PaletteCount;
  for(unsigned short w = ePicBuf.Header.w; w > 0; w--, data++){
    pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data);
  }
}




