/*******************************************************************************

          ePic格式图像子模块-绘制GIF图像非压缩引引图实现接口
此模块支持多线程调用！
********************************************************************************/
#ifndef __EPIC_PLOT_GIF_H
#define __EPIC_PLOT_GIF_H

/*******************************************************************************
                             主结构
********************************************************************************/
#include "Color.h"
#include "DecodeGIF.h"

struct _PlotGIF{
  unsigned short x;
  unsigned short y;  
  Color_t *pBuf;            //下次使用
  struct _DecodeGIF Decode; //译码器
};

/*******************************************************************************
                             相关函数
********************************************************************************/
//-----------------------------------画GIF图-----------------------------------
//暂仅支持 全局静态非透明图绘制
signed char ePic_PlotGIF(unsigned short x,unsigned short y);



#endif

