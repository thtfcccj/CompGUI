/*******************************************************************************

          ePic格式图像子模块-绘制PNG图像非压缩引引图实现
此模块支持多线程调用！
********************************************************************************/
#include "ePic_PlotPNG.h"


#include "ePic.h"
#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>
#include "struct.h" //struct_get()

//struct _PlotPNG Plot; //测试需要内存,默认时需9100字节;
/*******************************************************************************
                              相关函数实现
********************************************************************************/

//---------------------------保存或绘制第n行数据---------------------------------
//此行数在DecodePNG()中调用,以保存或或绘制一行图像
static void _cbOutLine(const struct  _winWriter *out)
{
  struct _PlotPNG *pPlot = struct_get(out, struct _PlotPNG, Decode.wOut);
  
  const unsigned char *data = out->data + 1; //索引色或真实色
  unsigned short w = ePicBuf.Header.w;
  //此应用作为绘图使用
  Color_t *pBuf = pPlot->pBuf;
  if(out->OutedSize <= out->U16Para)//第一行：显示缓冲行起始
    pBuf = Plot_cbAbsLocalArea(pPlot->x,pPlot->y, w, ePicBuf.Header.h); 
  else
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
   
  const unsigned char *map = ePicBuf.pNextData;  
  unsigned char mapSize = ePicBuf.Header.PaletteCount;
  unsigned char bpp = out->U8Para;
  if(bpp >= 8){//字节为单位
    for(unsigned short w = ePicBuf.Header.w; w > 0; w--, data++){
      pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data);
    }
  }
  else{//位为单位
    ePic_pPlotIndexLine(pBuf, map, mapSize, data, bpp, w);//画当前行
  }
  
  pPlot->pBuf = pBuf;
}

/*******************************************************************************
                             主函
********************************************************************************/
//-----------------------------画PNG图-----------------------------------
//横向取模w*h点位,此函数不依赖调色板
//暂仅支持8B绘制
signed char ePic_PlotPNG(u16 x,u16 y)
{
  struct _PlotPNG *pPlot = 
                   (struct _PlotPNG *)ePic_cbGetDecodeSpace(sizeof(struct _PlotPNG));
  pPlot->x = x;
  pPlot->y = y;  

  unsigned char DeepInfo = ePicBuf.Header.DeepInfo;
  if(DeepInfo & 0xe0 != (3 << 5)) return -1;  //图像类型：只支持索引彩色图像
  DeepInfo &= 0x1f;  //保留色深 
  
  //b7仅idat数据,扫描方法(b6~5), 滤波器方法(b4~2), 压缩方法(b1~0)  
  unsigned char ZipInfo = ePicBuf.Header.ZipInfo;
  if(ZipInfo != 0x80) return -1;//只支持逐行扫，默认压缩与自动滤波器(索引格式无)
  
  //得到调色板信息
  unsigned short MapCount = ePicBuf.Header.PaletteCount;
  if(MapCount == 0) MapCount = 256;
   MapCount *= sizeof(Color_t);
  if(ePicBuf.NextDataSize <= MapCount) return -1;//异常

  //PNG译码
  return DecodePNG(&pPlot->Decode,//未初始化的缓冲
                    _cbOutLine,//回调输出函数
                    DeepInfo,//色深,1,2,4,8,只仅支持索引色
                    ePicBuf.Header.w, //图像宽度
                    ePicBuf.Header.h,  //图像高度
                    ePicBuf.pNextData + MapCount,//idat区域的压缩数据
                    ePicBuf.NextDataSize - MapCount);    //idat区域数据长度
}





