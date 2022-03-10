/*******************************************************************************

                             PNG��ʽ����ʵ��

********************************************************************************/

#include "DecodePNG.h"
#include "Adler32.h" //Adler32ѹ����У�鷽ʽ
#include "struct.h" //struct_get()
#include <string.h>


//--------------------------�������ݻ���outʹ������---------------------------
//1�� out->Cfg,     �ײ�����0xc0
//2�� out->U32Para  �ײ�������Adler32�е�У��������
//3�� out->U8Para   ������¼ɫ��bpp
//4�� out->U16Para  ����������¼һ�д�С

/*******************************************************************************
                           ��غ���ʵ��
********************************************************************************/
#define _abs(x) ((x) < 0 ? -(x) : (x))

//----------------------------�з��˲������е�paethԤ��������--------------------
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

//---------------------------------���˲��ڶ������ݴ���--------------------------
//���ݿ��ܴ�ŵĿ����������л��в�ּ�����ֵ������л�ԭ
//��������data���������������ݣ������ڽ�ڶ�������
static signed char _unfilterL2(const struct  _winWriter *out)
{
  unsigned short  length = out->U16Para;            //һ������ռλ
  //if(out->start < (length * 2)) return 0;           //�������е�����
  unsigned char *precon = out->data + 1;      //�ϴ����Ϊ��һ������
  unsigned char *recon  = precon + length;         //���������Ҳ�ǵڶ���
  const unsigned char *scanline = recon;          //����ɨ�账����ǵ�������
  length--;//ȥ�����ͱ�־
  unsigned short bytewidth = (out->U8Para + 7) >> 3;//ɫ��ռλ
  
  //�˶δ������unfilterScanline()������δ�Ż�
  brsize_t i;
  switch(*(scanline - 1)){ //filterTypeΪ�׸���
    case 0: break;//û���˲�
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

//-------------------------���ݺ���������-----------------------------------
//�����õ��˶�������
static brsize_t _LaterPro(struct  _winWriter *out)
{
  unsigned short start = out->start;
  unsigned short length = out->U16Para;               //һ������ռλ
  if(start < ((length * 2) + DECODER_PNG_RESERVED_SPACE)) return 0;//�������е�����ʱ
  if(out->OutedSize == 0){//ͳ�Ƶ�һ���ۼӺ�
     out->U32Para = Adler32_Get(1, out->data, length);//Checksum
     out->OutedSize = length;
  }
  //ͳ�Ƶڶ����ۼӺ�
  out->U32Para += Adler32_Get(out->U32Para, 
                              out->data + length, length);
  _unfilterL2(out); //���˲�����ڶ�������
  struct _DecodePNG *pDecode = struct_get(out, struct _DecodePNG, wOut);
  pDecode->cbOutLine(out);//���Ƶ�һ������
  out->OutedSize += length;
  return length;   //��һ�����ݱ�������
}

//-------------------------���ݽ���������-----------------------------------
static void _EndPro(struct  _winWriter *out)
{
  unsigned short count = out->start;
  unsigned short length = out->U16Para; 
  unsigned short curLien = out->OutedSize / length;
  //���δ���ǰ��������
  struct _DecodePNG *pDecode = struct_get(out, struct _DecodePNG, wOut);
  for(; count >= (length * 2); count-= length){
    _unfilterL2(out); //���˲�����ڶ�������
    pDecode->cbOutLine(out);//���Ƶ�һ������
    //�Ƴ�������ɵ����ݣ���������ǰ��
    out->start -= length;
    curLien++;
    memcpy(out->data, out->data + length, length);
  }
  //����2�����������һ��
  memcpy(out->data, out->data + length, length);
  pDecode->cbOutLine(out);//���Ƶ�һ������
}

//-----------------------------Ĭ��OutLine-----------------------------------
static void _cbOutLine(const struct  _winWriter *out){(void)out;}

//-----------------------------PNG������ʵ��-----------------------------------
//�ݽ�֧������ɫ,����ɨ��Ĭ��ѹ����ʽ���Զ��˲�����ʽ
signed char DecodePNG(struct _DecodePNG *pDecode,//�����ʼ��
                        DecodePNG_cbOutLine_t cbOutLine,//�ص��������
                        unsigned char bbp,//ɫ��,1,2,4,8,ֻ��֧������ɫ
                        unsigned short w, //ͼ����
                        unsigned short h,  //ͼ��߶�
                        const unsigned char *idat,//idat�����ѹ������
                        unsigned long idatLen)    //idat�������ݳ���
{
  //if(pDecode == NULL) return -1;//�쳣
  if(cbOutLine == NULL) pDecode->cbOutLine = _cbOutLine;//��ֹ�쳣
  else  pDecode->cbOutLine = cbOutLine;
  
  //�õ�ÿ��ʵ��ռ���ֽ���
  unsigned short Line = ((w  >> 3) * bbp) + 1; //��������ռλ+�����˲���־ 
  Line += ((w & 0x07) * bbp + 7) >> 3; //ĩβ����δ���벿��ռ1�ֽ�
  
  //��ʼ��winWriter
  winWriter_t *out = &pDecode->wOut;
  winWriter_Clr(out);
  out->U8Para = bbp;  //������¼ɫ��bpp
  out->U16Para = Line; //������¼һ�д�С
  out->data = pDecode->OutData;
  out->capability = DECODER_PNG_DATA_BUF_SIZE;
  out->MaxOutSize = Line * h;
  out->Cfg = 0xf0; //�������й���
  out->LaterPro = _LaterPro; //���ڴ�����

  //��ѹ������
  signed char err = ZlibDecompress(&pDecode->Base, idat, idatLen, out);
  //��ѹ
  if(err) return err; //��ѹ�쳣
  _EndPro(out); //����ͼ����
  return 0;
}
