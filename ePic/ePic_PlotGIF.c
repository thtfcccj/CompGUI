/*******************************************************************************

          ePic格式图像子模块-绘制GIF图像非压缩引引图实现
此模块支持多线程调用！
********************************************************************************/

#include "ePic_PlotGIF.h"

#include "ePic.h"
#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>
#include "struct.h" //struct_get()


/*******************************************************************************
                              相关函数实现
********************************************************************************/

//---------------------------保存或绘制第n行数据---------------------------------
//此行数在DecodeGIF()中调用,以保存或或绘制一行图像
static void _cbOutLine(const struct  _winWriter *out)
{
  struct _PlotGIF *pPlot = struct_get(out, struct _PlotGIF, Decode.wOut);

  unsigned short w = ePicBuf.Header.w;
  //此应用作为绘图使用
  Color_t *pBuf = pPlot->pBuf;
  if(out->OutedSize <= out->U16Para)//第一行：显示缓冲行起始
    pBuf = Plot_cbAbsLocalArea(pPlot->x,pPlot->y, w, ePicBuf.Header.h);
  else
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
   
  pBuf = ePic_pPlotIndexLine(pBuf, 
                             ePicBuf.pNextData, 
                             ePicBuf.Header.PaletteCount, 
                             out->data, 
                             8, w);//画当前行,字节为单位
  pPlot->pBuf = pBuf;
}

/*******************************************************************************
                             主函
********************************************************************************/
//-----------------------------画GIF图-----------------------------------
//横向取模w*h点位,此函数不依赖调色板
//暂仅支持 全局静态非透明图绘制
signed char ePic_PlotGIF(u16 x,u16 y)
{
  struct _PlotGIF *pPlot = 
                   (struct _PlotGIF *)ePic_cbGetDecodeSpace(sizeof(struct _PlotGIF));
  pPlot->x = x;
  pPlot->y = y;  

  //m(b7有全局调色板)cr(b6~b4,颜色深度-1)s(b3) pixel(b3~0全局调色板个数-1)域
  unsigned char DeepInfo = ePicBuf.Header.DeepInfo;
  DeepInfo = ((DeepInfo >> 4) & 0x07) + 1;  //保留色深 
   
  //ePicBuf.Header.ZipInfo; ->背景颜色
  
  //得到调色板信息
  unsigned short MapCount = ePicBuf.Header.PaletteCount;
  if(MapCount == 0) MapCount = 256;
   MapCount *= sizeof(Color_t);
  if(ePicBuf.NextDataSize <= MapCount) return -1;//异常

  //GIF译码
  return DecodeGIF(&pPlot->Decode,//未初始化的缓冲
                    _cbOutLine,//回调输出函数
                    DeepInfo,//色深,1,2,4,8,只仅支持索引色
                    ePicBuf.Header.w, //图像宽度
                    ePicBuf.Header.h,  //图像高度
                    ePicBuf.pNextData + MapCount,//idat区域的压缩数据
                    ePicBuf.NextDataSize - MapCount);    //idat区域数据长度
}





