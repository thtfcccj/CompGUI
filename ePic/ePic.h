/*******************************************************************************

                    ePic��ʽͼ��ģ��

********************************************************************************/
#ifndef _EPIC_H
#define _EPIC_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

#include "Color.h"

//���嵱ǰ�汾(ԭ�汾�����֧��git)
#define EPIC_STR_VERSION    "ePic Decoder V1.00.032"

/*******************************************************************************
                             ���˵������������
********************************************************************************/

//֧�ֵ�ͼ���ʽ�У�
//wbm: ����ɫλͼתePic��ʽ
//bmp��֧��2,4,8λ����λͼ��֧��RLEѹ����ʽͼ�Ļ���
//     bmp��ʽ�������ϻ��ƣ����Դ�˳��ԭ�����Ϊ�������£��ʵ���ʱ�费ֱ��������
//png: 8λ����λ�û���

//��Ӧ����ֻ֧��һ������ʱ������̶�֧�ֵ�ͼ�����ͣ���Ϊ 'w' 'b' 'p'֮һ
//������������Ϊѹ����ʽʱ���� | 0x80Ĭ�Ͼ�֧�����趨��
//#define EPIC_FIX_PIC_TYPE  ('w' | 0x80)

//��������ͷ���뷽ʽ��Ĭ��ΪLSB����
//#define SUPPORT_EPIC_MSB  

//WBM��ʽ����ͷʱ�����п����߶����ݣ�������λ
//#define SUPPORT_EPIC_WBM_WH 

//����ͷ���ȣ�Ĭ��˫�ֽ�(�ݽ�֧�ֵ��ֽڻ�˫�ֽ�)
#ifndef EPIC_HEADER_LEN
  #define EPIC_HEADER_LEN  2
#endif

/******************************************************************************
                             ��ؽṹ
*******************************************************************************/

//����ePic��ʽ��׼����ͷ
struct _ePicHeader{
  unsigned char Type;        //ͼ������������,0��֧��
  unsigned char DeepInfo;    //���������Ϣ,��ͼ���ʽ���
  signed short  w;           //ͼ����
  signed short  h;           //ͼ�����
  unsigned char ZipInfo;     //ѹ�����ݵ���Ϣ
  unsigned char PaletteCount;//��ɫ������,0ʱ����
};

//���ݻ���
struct _ePicBuf{
  struct _ePicHeader Header;      //����ͷ���壬��֧�ֵ��̻߳��� 
  const unsigned char *pNextData;//���������ͷ�������
  unsigned long NextDataSize;     //���������ͷ������ݳ���
  //��ɫ�建�壺
  //const unsigned char *map,//�������
  //unsigned char mapSize,    //�������С
};

extern struct _ePicBuf ePicBuf; //��֧�ֵ��߳�

/*******************************************************************************
                              ������Ժ���
********************************************************************************/

//-------------------------------�õ�����ͷ��Ϣ--------------------------------
#define ePic_GetType()         (ePicBuf.Header.Type)
#define ePic_GetDeepInfo()     (ePicBuf.Header.DeepInfo)
#define ePic_GetType()         (ePicBuf.Header.Type)
#define ePic_GetW()            (ePicBuf.Header.w)
#define ePic_GetH()            (ePicBuf.Header.h)
#define ePic_GetZipInfo()      (ePicBuf.Header.ZipInfo)
#define ePic_GetPaletteCount() (ePicBuf.Header.PaletteCount)

//-----------------------------------�õ�ͼ��������------------------------------
const unsigned char *ePic_pGetImageData(void);

//-------------------------------���������ͷ�������---------------------------
#define ePic_pGetNextData() (ePicBuf.pNextData)


//----------------------���õ��ð���Ƶ�ǰ��----------------------------
//�ڲ�ʹ�ã�����pBufλ��
Color_t *ePic_pPlotIndexDot(Color_t *pBuf,//����λ��
                       const unsigned char *map,//�������
                       unsigned char mapSize,    //�������С
                       unsigned char indexColor); //����ɫ

//----------------------���õ�ɫ���ظ����Ƶ�ǰ��----------------------------
//����pBufλ��
Color_t *ePic_pPlotIndexDotRepeat(Color_t *pBuf,//����λ��
                       const unsigned char *map,//�������
                       unsigned char mapSize,    //�������С
                       unsigned char indexColor, //����ɫ
                       unsigned short RepeatCount);//�ظ�����

//----------------------���õ�ɫ����Ƶ�ǰ��---------------------------
//����pBufλ��
Color_t *ePic_pPlotIndexLine(Color_t *pBuf,//����λ��
                             const unsigned char *map,//�������
                             unsigned char mapSize,    //�������С
                             const unsigned char *data,//������
                             unsigned char bpp,        //ɫռλ,1,2,4,8
                             unsigned short w);        //���
                       

/*******************************************************************************
                              �����Ϊ����
********************************************************************************/

//------------------------��ePicͼ���ʽ���ݸ�������ͷ---------------------------
//ͬʱ��䷵�ط�0ͼ���ʽ ����
signed char ePic_ePicBuf(const unsigned char *ePicData,
                          unsigned long IconSize);

//------------------------��Wbmͼ���ʽ���ݸ�������ͷ----------------------------
//ͬʱ��䷵�ط�0ͼ���ʽ����
//signed char ePic_ePicBuf_Wbm(const unsigned char *pWbm);�ݲ�֧�ֶԴ�ͷ�Ľ���

//------------------------��BMPͼ���ʽ���ݸ�������ͷ----------------------------
//ͬʱ��䷵�ط�0ͼ���ʽ����
//signed char ePic_ePicBuf_Bmp(const unsigned char *pBmp);�ݲ�֧�ֶԴ�ͷ�Ľ���

//------------------------��PNGͼ���ʽ���ݸ�������ͷ---------------------------
//ͬʱ��䷵�ط�0ͼ���ʽ ����
//signed char ePic_ePicBuf_Png(const unsigned char *pPng);�ݲ�֧�ֶԴ�ͷ�Ľ���

//------------------------------���ƻ��������-----------------------------------
//���ô˺���ǰ��ePic_ePicBuf()��Ȼ�����úñ���ɫ��Plot_SetBrushColor()
//��Ϊwbm��ʽ����������ǰ����ǰ��ɫ��Plot_SetPenColor()
//����0��ȷ���ƣ������쳣
signed char ePic_Plot(unsigned short x,
                       unsigned short y);

/*******************************************************************************
                             �ص�����
********************************************************************************/
//------------------------����ָ�������Ľ��뻺��ռ�-----------------------------
//���ؿռ�ȷ��4�ֽڶ���,NULL����ʧ��
void *ePic_cbGetDecodeSpace(unsigned long Size);



#endif //_EPIC_H


