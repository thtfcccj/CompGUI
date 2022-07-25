/*******************************************************************************

                    ePic格式图像模块

********************************************************************************/
#ifndef _EPIC_H
#define _EPIC_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

#include "Color.h"

//定义当前版本(原版本或其分支见git)
#define EPIC_STR_VERSION    "ePic Decoder V1.00.032"

/*******************************************************************************
                             相关说明及常量定义
********************************************************************************/

//支持的图像格式有：
//wbm: 即单色位图转ePic格式
//bmp：支持2,4,8位索引位图，支持RLE压缩方式图的绘制
//     bmp格式由下往上绘制，因显存顺序原因解析为由上向下，故导入时需垂直镜像处理！！
//png: 8位索引位置绘制

//若应用中只支持一种类型时，定义固定支持的图像类型，需为 'w' 'b' 'p'之一
//若长度与宽度域为压缩格式时，需 | 0x80默认均支持无需定义
//#define EPIC_FIX_PIC_TYPE  ('w' | 0x80)

//定义索引头对齐方式，默认为LSB对齐
//#define SUPPORT_EPIC_MSB  

//WBM格式数据头时，仅有宽度与高度数据，无其它位
//#define SUPPORT_EPIC_WBM_WH 

//索引头长度，默认双字节(暂仅支持单字节或双字节)
#ifndef EPIC_HEADER_LEN
  #define EPIC_HEADER_LEN  2
#endif

/******************************************************************************
                             相关结构
*******************************************************************************/

//定义ePic格式标准数据头
struct _ePicHeader{
  unsigned char Type;        //图像类型描述符,0不支持
  unsigned char DeepInfo;    //像素深度信息,与图像格式相关
  signed short  w;           //图像宽度
  signed short  h;           //图像深度
  unsigned char ZipInfo;     //压缩数据等信息
  unsigned char PaletteCount;//调色板数量,0时所有
};

//数据缓冲
struct _ePicBuf{
  struct _ePicHeader Header;      //数据头缓冲，仅支持单线程绘制 
  const unsigned char *pNextData;//缓冲的数据头后的数据
  unsigned long NextDataSize;     //缓冲的数据头后的数据长度
  //调色板缓冲：
  //const unsigned char *map,//索引表表
  //unsigned char mapSize,    //索引表大小
};

extern struct _ePicBuf ePicBuf; //仅支持单线程

/*******************************************************************************
                              相关属性函数
********************************************************************************/

//-------------------------------得到数据头信息--------------------------------
#define ePic_GetType()         (ePicBuf.Header.Type)
#define ePic_GetDeepInfo()     (ePicBuf.Header.DeepInfo)
#define ePic_GetType()         (ePicBuf.Header.Type)
#define ePic_GetW()            (ePicBuf.Header.w)
#define ePic_GetH()            (ePicBuf.Header.h)
#define ePic_GetZipInfo()      (ePicBuf.Header.ZipInfo)
#define ePic_GetPaletteCount() (ePicBuf.Header.PaletteCount)

//-----------------------------------得到图像数据区------------------------------
const unsigned char *ePic_pGetImageData(void);

//-------------------------------缓冲的数据头后的数据---------------------------
#define ePic_pGetNextData() (ePicBuf.pNextData)


//----------------------利用调用板绘制当前点----------------------------
//内部使用，返回pBuf位置
Color_t *ePic_pPlotIndexDot(Color_t *pBuf,//绘制位置
                       const unsigned char *map,//索引表表
                       unsigned char mapSize,    //索引表大小
                       unsigned char indexColor); //索引色

//----------------------利用调色板重复绘制当前点----------------------------
//返回pBuf位置
Color_t *ePic_pPlotIndexDotRepeat(Color_t *pBuf,//绘制位置
                       const unsigned char *map,//索引表表
                       unsigned char mapSize,    //索引表大小
                       unsigned char indexColor, //索引色
                       unsigned short RepeatCount);//重复次数

//----------------------利用调色板绘制当前行---------------------------
//返回pBuf位置
Color_t *ePic_pPlotIndexLine(Color_t *pBuf,//绘制位置
                             const unsigned char *map,//索引表表
                             unsigned char mapSize,    //索引表大小
                             const unsigned char *data,//数据流
                             unsigned char bpp,        //色占位,1,2,4,8
                             unsigned short w);        //宽度
                       

/*******************************************************************************
                              相关行为函数
********************************************************************************/

//------------------------由ePic图像格式数据更新数据头---------------------------
//同时填充返回非0图像格式 错误
signed char ePic_ePicBuf(const unsigned char *ePicData,
                          unsigned long IconSize);

//------------------------由Wbm图像格式数据更新数据头----------------------------
//同时填充返回非0图像格式错误
//signed char ePic_ePicBuf_Wbm(const unsigned char *pWbm);暂不支持对此头的解析

//------------------------由BMP图像格式数据更新数据头----------------------------
//同时填充返回非0图像格式错误
//signed char ePic_ePicBuf_Bmp(const unsigned char *pBmp);暂不支持对此头的解析

//------------------------由PNG图像格式数据更新数据头---------------------------
//同时填充返回非0图像格式 错误
//signed char ePic_ePicBuf_Png(const unsigned char *pPng);暂不支持对此头的解析

//------------------------------绘制缓冲的数据-----------------------------------
//调用此函数前需ePic_ePicBuf()，然后设置好背景色：Plot_SetBrushColor()
//若为wbm格式，还在需提前设置前景色：Plot_SetPenColor()
//返回0正确绘制，否则异常
signed char ePic_Plot(unsigned short x,
                       unsigned short y);

/*******************************************************************************
                             回调函数
********************************************************************************/
//------------------------申请指定容量的解码缓存空间-----------------------------
//返回空间确保4字节对齐,NULL申请失败
void *ePic_cbGetDecodeSpace(unsigned long Size);



#endif //_EPIC_H


