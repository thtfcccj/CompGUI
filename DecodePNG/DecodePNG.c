/*******************************************************************************

                             PNG格式解码实现

********************************************************************************/

#include "DecodePNG.h"
#include "Adler32.h" //Adler32压缩包校验方式
#include "struct.h" //struct_get()
#include <string.h>


//--------------------------接收数据缓冲out使用声明---------------------------
//1： out->Cfg,     底层已用0xc0
//2： out->U32Para  底层用作了Adler32中的校验结果缓存
//3： out->U8Para   用作记录色深bpp
//4： out->U16Para  用作有作记录一行大小

/*******************************************************************************
                           相关函数实现
********************************************************************************/
#define _abs(x) ((x) < 0 ? -(x) : (x))

//----------------------------行反滤波处理中的paeth预测器处理--------------------
static unsigned char _paethPredictor(short a, short b, short c)
{
  short pa = _abs(b - c);
  short pb = _abs(a - c);
  short pc = _abs(a + b - c - c);
  /* return input value associated with smallest of 
     pa, pb, pc (with certain priority if equal) */
  if(pb < pa) { a = b; pa = pb; }
  return (pc < pa) ? c : a;
}

//---------------------------------反滤波第二行数据处理--------------------------
//数据可能存放的可能是与上行或本行差分计算后的值，需进行还原
//本函数在data区缓冲了两行数据，且用于解第二行数据
static signed char _unfilterL2(const struct  _winWriter *out)
{
  unsigned short  length = out->U16Para;            //一行数据占位
  //if(out->start < (length * 2)) return 0;           //不够两行的数据
  unsigned char *precon = out->data + 1;      //上次输出为第一行数据
  unsigned char *recon  = precon + length;         //输出的数据也是第二行
  const unsigned char *scanline = recon;          //本行扫描处理的是第于数据
  length--;//去除类型标志
  unsigned short bytewidth = (out->U8Para + 7) >> 3;//色深占位
  
  //此段代码出自unfilterScanline()函数，未优化
  brsize_t i;
  switch(*(scanline - 1)){ //filterType为首个数
    case 0: break;//没有滤波
    case 1: {
      brsize_t j = 0;
      for(i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
      for(i = bytewidth; i != length; ++i, ++j) recon[i] = scanline[i] + recon[j];
      break;
    }
    case 2:
      if(precon) {
        for(i = 0; i != length; ++i) recon[i] = scanline[i] + precon[i];
      } else {
        for(i = 0; i != length; ++i) recon[i] = scanline[i];
      }
      break;
    case 3:
      if(precon) {
        brsize_t j = 0;
        for(i = 0; i != bytewidth; ++i) recon[i] = scanline[i] + (precon[i] >> 1u);
        /* Unroll independent paths of this predictor. A 6x and 8x version is also possible but that adds
        too much code. Whether this speeds up anything depends on compiler and settings. */
        if(bytewidth >= 4) {
          for(; i + 3 < length; i += 4, j += 4) {
            unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2], s3 = scanline[i + 3];
            unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2], r3 = recon[j + 3];
            unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2], p3 = precon[i + 3];
            recon[i + 0] = s0 + ((r0 + p0) >> 1u);
            recon[i + 1] = s1 + ((r1 + p1) >> 1u);
            recon[i + 2] = s2 + ((r2 + p2) >> 1u);
            recon[i + 3] = s3 + ((r3 + p3) >> 1u);
          }
        } else if(bytewidth >= 3) {
          for(; i + 2 < length; i += 3, j += 3) {
            unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2];
            unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2];
            unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2];
            recon[i + 0] = s0 + ((r0 + p0) >> 1u);
            recon[i + 1] = s1 + ((r1 + p1) >> 1u);
            recon[i + 2] = s2 + ((r2 + p2) >> 1u);
          }
        } else if(bytewidth >= 2) {
          for(; i + 1 < length; i += 2, j += 2) {
            unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1];
            unsigned char r0 = recon[j + 0], r1 = recon[j + 1];
            unsigned char p0 = precon[i + 0], p1 = precon[i + 1];
            recon[i + 0] = s0 + ((r0 + p0) >> 1u);
            recon[i + 1] = s1 + ((r1 + p1) >> 1u);
          }
        }
        for(; i != length; ++i, ++j) recon[i] = scanline[i] + ((recon[j] + precon[i]) >> 1u);
      } else {
        brsize_t j = 0;
        for(i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
        for(i = bytewidth; i != length; ++i, ++j) recon[i] = scanline[i] + (recon[j] >> 1u);
      }
      break;
    case 4:
      if(precon) {
        brsize_t j = 0;
        for(i = 0; i != bytewidth; ++i) {
          recon[i] = (scanline[i] + precon[i]); /*_paethPredictor(0, precon[i], 0) is always precon[i]*/
        }

        /* Unroll independent paths of the paeth predictor. A 6x and 8x version is also possible but that
        adds too much code. Whether this speeds up anything depends on compiler and settings. */
        if(bytewidth >= 4) {
          for(; i + 3 < length; i += 4, j += 4) {
            unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2], s3 = scanline[i + 3];
            unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2], r3 = recon[j + 3];
            unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2], p3 = precon[i + 3];
            unsigned char q0 = precon[j + 0], q1 = precon[j + 1], q2 = precon[j + 2], q3 = precon[j + 3];
            recon[i + 0] = s0 + _paethPredictor(r0, p0, q0);
            recon[i + 1] = s1 + _paethPredictor(r1, p1, q1);
            recon[i + 2] = s2 + _paethPredictor(r2, p2, q2);
            recon[i + 3] = s3 + _paethPredictor(r3, p3, q3);
          }
        } else if(bytewidth >= 3) {
          for(; i + 2 < length; i += 3, j += 3) {
            unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2];
            unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2];
            unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2];
            unsigned char q0 = precon[j + 0], q1 = precon[j + 1], q2 = precon[j + 2];
            recon[i + 0] = s0 + _paethPredictor(r0, p0, q0);
            recon[i + 1] = s1 + _paethPredictor(r1, p1, q1);
            recon[i + 2] = s2 + _paethPredictor(r2, p2, q2);
          }
        } else if(bytewidth >= 2) {
          for(; i + 1 < length; i += 2, j += 2) {
            unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1];
            unsigned char r0 = recon[j + 0], r1 = recon[j + 1];
            unsigned char p0 = precon[i + 0], p1 = precon[i + 1];
            unsigned char q0 = precon[j + 0], q1 = precon[j + 1];
            recon[i + 0] = s0 + _paethPredictor(r0, p0, q0);
            recon[i + 1] = s1 + _paethPredictor(r1, p1, q1);
          }
        }

        for(; i != length; ++i, ++j) {
          recon[i] = (scanline[i] + _paethPredictor(recon[i - bytewidth], precon[i], precon[j]));
        }
      } else {
        brsize_t j = 0;
        for(i = 0; i != bytewidth; ++i) {
          recon[i] = scanline[i];
        }
        for(i = bytewidth; i != length; ++i, ++j) {
          /*_paethPredictor(recon[i - bytewidth], 0, 0) is always recon[i - bytewidth]*/
          recon[i] = (scanline[i] + recon[j]);
        }
      }
      break;
    default: return 36; /*error: invalid filter type given*/
  }
  return 0;
}

//-------------------------数据后续处理函数-----------------------------------
//返回用掉了多少数据
static brsize_t _LaterPro(struct  _winWriter *out)
{
  unsigned short start = out->start;
  unsigned short length = out->U16Para;               //一行数据占位
  if(start < ((length * 2) + DECODER_PNG_RESERVED_SPACE)) return 0;//不够两行的数据时
  if(out->OutedSize == 0){//统计第一行累加和
     out->U32Para = Adler32_Get(1, out->data, length);//Checksum
     out->OutedSize = length;
  }
  //统计第二行累加和
  out->U32Para += Adler32_Get(out->U32Para, 
                              out->data + length, length);
  _unfilterL2(out); //反滤波处理第二行数据
  struct _DecodePNG *pDecode = struct_get(out, struct _DecodePNG, wOut);
  pDecode->cbOutLine(out);//绘制第一行数据
  out->OutedSize += length;
  return length;   //第一行数据被绘制了
}

//-------------------------数据结束处理函数-----------------------------------
static void _EndPro(struct  _winWriter *out)
{
  unsigned short count = out->start;
  unsigned short length = out->U16Para; 
  unsigned short curLien = out->OutedSize / length;
  //依次处理前两行数据
  struct _DecodePNG *pDecode = struct_get(out, struct _DecodePNG, wOut);
  for(; count >= (length * 2); count-= length){
    _unfilterL2(out); //反滤波处理第二行数据
    pDecode->cbOutLine(out);//绘制第一行数据
    //移出绘制完成的数据，并将数据前移
    out->start -= length;
    curLien++;
    memcpy(out->data, out->data + length, length);
  }
  //最后第2行数据移入第一行
  memcpy(out->data, out->data + length, length);
  pDecode->cbOutLine(out);//绘制第一行数据
}

//-----------------------------默认OutLine-----------------------------------
static void _cbOutLine(const struct  _winWriter *out){(void)out;}

//-----------------------------PNG译码器实现-----------------------------------
//暂仅支持索引色,逐行扫，默认压缩方式与自动滤波器格式
signed char DecodePNG(struct _DecodePNG *pDecode,//无需初始化
                        DecodePNG_cbOutLine_t cbOutLine,//回调输出函数
                        unsigned char bbp,//色深,1,2,4,8,只仅支持索引色
                        unsigned short w, //图像宽度
                        unsigned short h,  //图像高度
                        const unsigned char *idat,//idat区域的压缩数据
                        unsigned long idatLen)    //idat区域数据长度
{
  //if(pDecode == NULL) return -1;//异常
  if(cbOutLine == NULL) pDecode->cbOutLine = _cbOutLine;//防止异常
  else  pDecode->cbOutLine = cbOutLine;
  
  //得到每行实际占用字节数
  unsigned short Line = ((w  >> 3) * bbp) + 1; //整数部分占位+行首滤波标志 
  Line += ((w & 0x07) * bbp + 7) >> 3; //末尾像素未对齐部分占1字节
  
  //初始化winWriter
  winWriter_t *out = &pDecode->wOut;
  winWriter_Clr(out);
  out->U8Para = bbp;  //用作记录色深bpp
  out->U16Para = Line; //有作记录一行大小
  out->data = pDecode->OutData;
  out->capability = DECODER_PNG_DATA_BUF_SIZE;
  out->MaxOutSize = Line * h;
  out->Cfg = 0xf0; //启用所有功能
  out->LaterPro = _LaterPro; //窗口处理函数

  //解压缩数据
  signed char err = ZlibDecompress(&pDecode->Base, idat, idatLen, out);
  //解压
  if(err) return err; //解压异常
  _EndPro(out); //后续图像处理
  return 0;
}
