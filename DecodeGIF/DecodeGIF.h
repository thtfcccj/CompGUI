/*******************************************************************************

                    GIF��ʽ����ʵ��
��ģ������ڽ�ѹһ��gifͼ��ѹ�����ݣ�������gifͼ�Ľ���
֧�ֶ��̵߳��ã�
********************************************************************************/
#ifndef _DECODER_GIF_H
#define _DECODER_GIF_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*******************************************************************************
                             ���˵������������
********************************************************************************/

//ϵͳ�õ��ģ����ͼ��Ŀ�ȣ������ڲ�����������ݻ�����,ֵԽ��Խռ�ڴ�
#ifndef DECODER_GIF_W_MAX 
  #define DECODER_GIF_W_MAX         480
#endif

//�������ݻ�������С,����Ҫ�����ң�����1.5����DECODER_GIF_W_MAX���
#ifndef DECODER_GIF_DATA_BUF_SIZE
  #define DECODER_GIF_DATA_BUF_SIZE  (DECODER_GIF_W_MAX * 2 + 10)
#endif

//�����ֵ�
#ifndef DECODER_DICT_SIZE 
  #define DECODER_DICT_SIZE      4096  //GIF��׼Ϊ4096,�̶�
#endif

//�ֵ����ַ�����󳤶�
#ifndef DECODER_STRING_SIZE 
  #define DECODER_STRING_SIZE    DECODER_GIF_W_MAX
#endif

/*******************************************************************************
                              ��ؽṹ
********************************************************************************/
#include "bReader.h"   //��������
#include "winWriter.h" //�������

//------------------------------------���ṹ----------------------------------
//�ص����(���ڻ�������򱣴�)�ṹ�������������λ��out->data+1, ��out->U16Para-1
typedef void (*DecodeGIF_cbOutLine_t)(const struct  _winWriter *out);

struct _DecodeGIF{
  //��Ҫ����:
  bReader_t Reader;                                     //��������   
  DecodeGIF_cbOutLine_t cbOutLine;                      //�ص��������
  winWriter_t wOut;                                    //��������ݽṹ
  unsigned char OutData[DECODER_GIF_DATA_BUF_SIZE]; //wOut->data��
 
  //LZW�ֵ�:
  //���ö���ѭ�����ҷ�ʽ(��ʱ�任�ռ�)ʵ��
  //gif���256ɫ�����ñ䳤����,�ֵ䵽1023���ٱ���͸�Ϊ10λ���Դ�����
  unsigned short DictId[DECODER_DICT_SIZE];   //�ֵ�ͷ��ID(���ַ���),���ܶ��Ƕ��
  unsigned char  DictChar[DECODER_DICT_SIZE]; //β���ַ���gif���256ɫ,�ʴ�ֵ���255
  
  //���Ƕ�׵ݹ����ʱ,����������ַ���
  unsigned char  AntiChar[DECODER_STRING_SIZE];
};

/*******************************************************************************
                              ��غ���
********************************************************************************/

//-----------------------------GIF������ʵ��-----------------------------------
//�ݽ�֧������ɫ,����ɨ��Ĭ��ѹ����ʽ���Զ��˲�����ʽ
signed char DecodeGIF(struct _DecodeGIF *pDecode,//�����ʼ��
                        DecodeGIF_cbOutLine_t cbOutLine,//�ص��������
                        unsigned char bbp,//ɫ��,1,2,4,8����ɫ
                        unsigned short w, //ͼ����
                        unsigned short h,  //ͼ��߶�
                        const unsigned char *idat, //gifͼ��ѹ������
                        unsigned long idatLen);    //gifͼ�����ݳ���


#endif //_DECODER_GIF_H


