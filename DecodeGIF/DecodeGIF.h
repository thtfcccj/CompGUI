/*******************************************************************************

                    GIF格式解码实现
此模块仅用于解压一组gif图的压缩数据，不负责gif图的解析
支持多线程调用！
********************************************************************************/
#ifndef _DECODER_GIF_H
#define _DECODER_GIF_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*******************************************************************************
                             相关说明及常量定义
********************************************************************************/

//系统用到的，最大图宽的宽度，用于内部定义输出数据缓冲区,值越大越占内存
#ifndef DECODER_GIF_W_MAX 
  #define DECODER_GIF_W_MAX         480
#endif

//定义数据缓冲区大小,不需要往回找，至少1.5倍宽DECODER_GIF_W_MAX宽度
#ifndef DECODER_GIF_DATA_BUF_SIZE
  #define DECODER_GIF_DATA_BUF_SIZE  (DECODER_GIF_W_MAX * 2 + 10)
#endif

//定义字典
#ifndef DECODER_DICT_SIZE 
  #define DECODER_DICT_SIZE      4096  //GIF标准为4096,固定
#endif

//字典里字符的最大长度
#ifndef DECODER_STRING_SIZE 
  #define DECODER_STRING_SIZE    DECODER_GIF_W_MAX
#endif

/*******************************************************************************
                              相关结构
********************************************************************************/
#include "bReader.h"   //输入数据
#include "winWriter.h" //输出数据

//------------------------------------主结构----------------------------------
//回调输出(用于绘屏输出或保存)结构，需输出的数据位置out->data+1, 长out->U16Para-1
typedef void (*DecodeGIF_cbOutLine_t)(const struct  _winWriter *out);

struct _DecodeGIF{
  //主要数据:
  bReader_t Reader;                                     //输入数据   
  DecodeGIF_cbOutLine_t cbOutLine;                      //回调输出函数
  winWriter_t wOut;                                    //缓冲的数据结构
  unsigned char OutData[DECODER_GIF_DATA_BUF_SIZE]; //wOut->data用
 
  //LZW字典:
  //采用定长循环查找方式(以时间换空间)实现
  //gif最大256色，采用变长编码,字典到1023，再编码就改为10位，以此类推
  unsigned short DictId[DECODER_DICT_SIZE];   //字典头部ID(或字符串),可能多次嵌套
  unsigned char  DictChar[DECODER_DICT_SIZE]; //尾部字符串gif最大256色,故此值最大255
  
  //多次嵌套递归查找时,反向输出的字符串
  unsigned char  AntiChar[DECODER_STRING_SIZE];
};

/*******************************************************************************
                              相关函数
********************************************************************************/

//-----------------------------GIF译码器实现-----------------------------------
//暂仅支持索引色,逐行扫，默认压缩方式与自动滤波器格式
signed char DecodeGIF(struct _DecodeGIF *pDecode,//无需初始化
                        DecodeGIF_cbOutLine_t cbOutLine,//回调输出函数
                        unsigned char bbp,//色深,1,2,4,8索引色
                        unsigned short w, //图像宽度
                        unsigned short h,  //图像高度
                        const unsigned char *idat, //gif图像压缩数据
                        unsigned long idatLen);    //gif图像数据长度


#endif //_DECODER_GIF_H


