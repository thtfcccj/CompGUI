/*******************************************************************************

                    PNG格式解码实现
此模块支持多线程调用！
********************************************************************************/
#ifndef _DECODER_PNG_H
#define _DECODER_PNG_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*******************************************************************************
                             相关说明及常量定义
********************************************************************************/

//系统用到的，最大图宽的宽度，用于内部定义输出数据缓冲区,值越大越占内存
#ifndef DECODER_PNG_W_MAX 
  #define DECODER_PNG_W_MAX         480
#endif

//用到的PNG图片压缩前向查找表最大距离，与PNG图片宽度及PNG压缩深有关,值越大越占内存
#ifndef DECODER_PNG_RESERVED_SPACE
  #define DECODER_PNG_RESERVED_SPACE   512
#endif

/*******************************************************************************
                              相关结构
********************************************************************************/
#include "ZlibDecompress.h" //PNG的压缩方式

//定义数据缓冲区大小,至少两倍宽最大显示宽度
#define DECODER_PNG_DATA_BUF_SIZE  \
                     ((DECODER_PNG_W_MAX) * 2 + (DECODER_PNG_RESERVED_SPACE))

                       
                       
                       
//------------------------------------主结构----------------------------------
//回调输出(用于绘屏输出或保存)结构，需输出的数据位置out->data+1, 长out->U16Para-1
typedef void (*DecodePNG_cbOutLine_t)(const struct  _winWriter *out);

struct _DecodePNG{
  struct _ZlibDecompress Base;                         //底层驱动需要
  DecodePNG_cbOutLine_t cbOutLine;                      //回调输出函数
  winWriter_t wOut;                                    //缓冲的数据结构
  unsigned char OutData[DECODER_PNG_DATA_BUF_SIZE];   //wOut->data用
};

/*******************************************************************************
                              相关函数
********************************************************************************/

//-----------------------------PNG译码器实现-----------------------------------
//暂仅支持索引色,逐行扫，默认压缩方式与自动滤波器格式
signed char DecodePNG(struct _DecodePNG *pDecode,//无需初始化
                        DecodePNG_cbOutLine_t cbOutLine,//回调输出函数
                        unsigned char bbp,//色深,1,2,4,8,只仅支持索引色
                        unsigned short w, //图像宽度
                        unsigned short h,  //图像高度
                        const unsigned char *idat,//idat区域的压缩数据
                        unsigned long idatLen);    //idat区域数据长度


#endif //_DECODER_PNG_H


