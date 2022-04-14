/*******************************************************************************

                    PNG��ʽ����ʵ��
��ģ��֧�ֶ��̵߳��ã�
********************************************************************************/
#ifndef _DECODER_PNG_H
#define _DECODER_PNG_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*******************************************************************************
                             ���˵������������
********************************************************************************/

//ϵͳ�õ��ģ����ͼ��Ŀ�ȣ������ڲ�����������ݻ�����,ֵԽ��Խռ�ڴ�
#ifndef DECODER_PNG_W_MAX 
  #define DECODER_PNG_W_MAX         480
#endif

//�õ���PNGͼƬѹ��ǰ����ұ������룬��PNGͼƬ��ȼ�PNGѹ�����й�,ֵԽ��Խռ�ڴ�
#ifndef DECODER_PNG_RESERVED_SPACE
  #define DECODER_PNG_RESERVED_SPACE   512
#endif

/*******************************************************************************
                              ��ؽṹ
********************************************************************************/
#include "ZlibDecompress.h" //PNG��ѹ����ʽ

//�������ݻ�������С,���������������ʾ���
#define DECODER_PNG_DATA_BUF_SIZE  \
                     ((DECODER_PNG_W_MAX) * 2 + (DECODER_PNG_RESERVED_SPACE))

                       
                       
                       
//------------------------------------���ṹ----------------------------------
//�ص����(���ڻ�������򱣴�)�ṹ�������������λ��out->data+1, ��out->U16Para-1
typedef void (*DecodePNG_cbOutLine_t)(const struct  _winWriter *out);

struct _DecodePNG{
  struct _ZlibDecompress Base;                         //�ײ�������Ҫ
  DecodePNG_cbOutLine_t cbOutLine;                      //�ص��������
  winWriter_t wOut;                                    //��������ݽṹ
  unsigned char OutData[DECODER_PNG_DATA_BUF_SIZE];   //wOut->data��
};

/*******************************************************************************
                              ��غ���
********************************************************************************/

//-----------------------------PNG������ʵ��-----------------------------------
//�ݽ�֧������ɫ,����ɨ��Ĭ��ѹ����ʽ���Զ��˲�����ʽ
signed char DecodePNG(struct _DecodePNG *pDecode,//�����ʼ��
                        DecodePNG_cbOutLine_t cbOutLine,//�ص��������
                        unsigned char bbp,//ɫ��,1,2,4,8,ֻ��֧������ɫ
                        unsigned short w, //ͼ����
                        unsigned short h,  //ͼ��߶�
                        const unsigned char *idat,//idat�����ѹ������
                        unsigned long idatLen);    //idat�������ݳ���


#endif //_DECODER_PNG_H


