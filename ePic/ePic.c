/*******************************************************************************

                            ePic格式图像模块实现

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

struct _ePicBuf ePicBuf; //仅支持单线程

//------------------------得到双字节数据--------------------------------
#if SUPPORT_RI_MSB //高位在前
  #define _GetU16Data(data) ((unsigned short)*(data) << 8) + *((data) + 1);
#else //低位在前
  #define _GetU16Data(data) (((unsigned short)*((data) + 1)) << 8) + *(data);
#endif

//-----------------------------------得到图像数据区------------------------------
const unsigned char *ePic_pGetImageData(void)
{
  const unsigned char *pData = ePicBuf.pNextData;
  if(ePicBuf.Header.Type == 'w') return pData;  //无调色板
  if(ePicBuf.Header.PaletteCount) //局部调色板时
    return pData + sizeof(Color_t) * ePicBuf.Header.PaletteCount; 
  //值为0表示全调色板
  //bmp格式固定调色板是为256
  if((ePicBuf.Header.Type == 'b') && (ePicBuf.Header.ZipInfo & 0x80))
    return pData + sizeof(Color_t) * 256;
  return pData + sizeof(Color_t) * ((unsigned short)1 << ePicBuf.Header.DeepInfo);
}

//------------------------由ePic图像格式数据更新数据头---------------------------
//同时填充返回非0图像格式 错误
signed char ePic_ePicBuf(const unsigned char *ePicData,
                          unsigned long IconSize)
{
  memset(&ePicBuf, 0,sizeof(struct _ePicBuf));
  
  if(IconSize < 5) return -1; //数据太小
  
  const unsigned char *eOrgData = ePicData;
  //==================图像类型与色深解析====================
  unsigned char Data;
  #ifdef EPIC_FIX_PIC_TYPE //支持一种图像格式时
    Data = EPIC_FIX_PIC_TYPE;
  #else
    Data = *ePicData++;    
  #endif
  unsigned char Type = Data & 0x7f;//图像类型    
  if((Type != 'w') && (Type != 'b') && (Type != 'p') && (Type != 'g')){
    ePicBuf.Header.Type = 0;//0不支持  
    return -1; //不支持的图像格式
  }
  ePicBuf.Header.Type = Type;
  
  //WBM格式数据头时，仅有宽度与高度数据，色深固定为1
  #ifdef SUPPORT_EPIC_WBM_WH
    if(Type == 'w')
      ePicBuf.Header.DeepInfo = 1;//wbmp格式固定1位色深
    else ePicBuf.Header.DeepInfo = *ePicData++;
  #else
    ePicBuf.Header.DeepInfo = *ePicData++;
  #endif
  
  //===================图像长宽解析===================
  if(Data & 0x80){//压缩长度
    Data = *ePicData++;
    if(Data & 0x80)
      ePicBuf.Header.w = ((unsigned short)(*ePicData++) << 7) + (Data & 0x7f);
    else ePicBuf.Header.w = Data;
    Data = *ePicData++;
    if(Data & 0x80)
      ePicBuf.Header.h = ((unsigned short)(*ePicData++) << 7) + (Data & 0x7f);
    else ePicBuf.Header.h = Data;
  }
  else{//固定长度
    #if EPIC_HEADER_LEN == 1
      ePicBuf.Header.w = *ePicData++;
      ePicBuf.Header.h = *ePicData++;
    #else // RI_HEADER_LEN == 2
      ePicBuf.Header.w = _GetU16Data(ePicData);
      ePicData += 2;
      ePicBuf.Header.h = _GetU16Data(ePicData);
      ePicData += 2;  
    #endif
  }
  
  //===================压缩数据与调色板数量解析===================
  //WBM格式数据头时，仅有宽度与高度数据，无压缩与调色板
  if(Type == 'w'){
    #ifndef SUPPORT_EPIC_WBM_WH //未省略此两位时
    ePicBuf.Header.ZipInfo = *ePicData++;
    ePicBuf.Header.PaletteCount = *ePicData++; 
    #endif
  }
  else{ 
    ePicBuf.Header.ZipInfo = *ePicData++;
    ePicBuf.Header.PaletteCount = *ePicData++;
  }

  ePicBuf.pNextData = ePicData; //图像或调色板数据了    
  unsigned char HeaderSize = ePicData - eOrgData;
  if(HeaderSize >= IconSize) return -1;//异常
  ePicBuf.NextDataSize = IconSize - HeaderSize;
  return 0; //数据正确了
}

//-----------------------------画非压缩索引位图---------------------
//横向取模w*h点位,此函数不依赖调色板
//支持1,2,4,8位索引位图的绘制
void ePic_PlotIndexBmp(u16 x,u16 y);

//-----------------------绘制BMP格式RLE4/8数据-----------------------
signed char ePic_PlotRle4(unsigned short x, unsigned short y);
signed char ePic_PlotRle8(unsigned short x, unsigned short y);

#include "ePic_PlotGIF.h"
#include "ePic_PlotPNG.h"

//------------------------------绘制缓冲的数据----------------------------------
//调用此函数前需ePic_ePicBuf()，然后设置好背景色：Plot_SetBrushColor()
//若为wbm格式，还在需提前设置前景色：Plot_SetPenColor()
//返回0正确绘制，否则异常
signed char ePic_Plot(unsigned short x,
                       unsigned short y)
{
  //根据类型分别解析
  if(ePicBuf.Header.Type ==  'w'){//WBMP格式时
    Plot_Bmp1(x,y,ePicBuf.Header.w,ePicBuf.Header.h,
              ePicBuf.pNextData,      //数据矩阵,高位在前
              0);//正色显示不透明
    return 0;
  }
  //BMP格式绘图
  if(ePicBuf.Header.Type ==  'b'){
    unsigned char ZipMode = ePicBuf.Header.ZipInfo & 0x7f;
    if(ZipMode == 0){//不压缩时
      if(ePicBuf.Header.DeepInfo > 8){
        return -1;//暂不支持16，24，32位非索引图像
      }
      else{//1，2，4，8索引色
        ePic_PlotIndexBmp(x,y);
        return 0;
      }
    }
    else if(ZipMode == 1)//LRE8压缩
      return ePic_PlotRle8(x,y);
    else if(ZipMode == 2)//LRE4压缩
      return ePic_PlotRle4(x,y);
    return -1;//异常
  }
  
  //PNG格式时
  if(ePicBuf.Header.Type ==  'p'){
    return ePic_PlotPNG(x,y);
  }
  //GIF格式时
  if(ePicBuf.Header.Type ==  'g'){
    return ePic_PlotGIF(x,y);
  }
  
  return -1;//其它暂不支持  
}

//----------------------利用调色板绘制当前点----------------------------
//返回pBuf位置
Color_t *ePic_pPlotIndexDot(Color_t *pBuf,//绘制位置
                       const unsigned char *map,//索引表表
                       unsigned char mapSize,    //索引表大小
                       unsigned char indexColor) //索引色
{
  //非透明时填充并移至下一点
  if((indexColor < mapSize) ||!mapSize){
    Color_t color; //为防止查找表数据未对齐
    memcpy(&color, //MSB,LSB自适应
           map + indexColor * sizeof(Color_t),
           sizeof(color));
    Plot_cbSetCurColor(pBuf, color);
  }
  else{//忽略对当前点的绘制
    Plot_cbUpdateNext(pBuf);
  }
  return pBuf;
}

//----------------------利用调色板重复绘制当前点----------------------------
//返回pBuf位置
Color_t *ePic_pPlotIndexDotRepeat(Color_t *pBuf,//绘制位置
                       const unsigned char *map,//索引表表
                       unsigned char mapSize,    //索引表大小
                       unsigned char indexColor, //索引色
                       unsigned short RepeatCount)//重复次数
{
  //非透明时填充并移至下一点
  if((indexColor < mapSize) ||!mapSize){
    Color_t color; //为防止查找表数据未对齐
    memcpy(&color, //MSB,LSB自适应
           map + indexColor * sizeof(Color_t),
           sizeof(color));
    for(; RepeatCount > 0; RepeatCount--) 
      Plot_cbSetCurColor(pBuf, color);
  }
  else{//忽略对当前点的绘制
    for(; RepeatCount > 0; RepeatCount--) 
      Plot_cbUpdateNext(pBuf);
  }
  return pBuf;
}

//----------------------利用调色板绘制当前行---------------------------
//返回pBuf位置
Color_t *ePic_pPlotIndexLine(Color_t *pBuf,//绘制位置
                             const unsigned char *map,//索引表表
                             unsigned char mapSize,    //索引表大小
                             const unsigned char *data,//数据流
                             unsigned char bpp,        //色占位,1,2,4,8
                             unsigned short w)         //宽度
{
  if(bpp >= 8){//字节为单位
    for(; w > 0; w--, data++){
      pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data);
    }
    return pBuf;
  }
  //1,2,4时:
  unsigned char bitMask = Plot_BitSize2Mask[bpp];   //位掩码
  do{//宽度一点点绘制
    unsigned char Data = *data++; //取位数据
    signed char curLen;
      if(w < (8 / bpp)) curLen = w * bpp; //宽度对齐，余下丢弃
      else curLen = 8;
      for(curLen -= bpp; curLen >= 0; curLen -= bpp, w--){
        //填充色
        unsigned char indexColor = (Data >> curLen) & bitMask;
        pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, indexColor);
      }//end for
    }while(w > 0);
  return pBuf;
}



