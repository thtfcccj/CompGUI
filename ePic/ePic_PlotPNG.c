/*******************************************************************************

          ePic��ʽͼ����ģ��-����BMPͼ���ѹ������ͼʵ��

********************************************************************************/

#include "ePic.h"
#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

#include "DecodePNG.h"

static unsigned short _x;
static unsigned short _y;

//-----------------------------��PNGͼ-----------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//�ݽ�֧��8B����
signed char ePic_PlotPNG(u16 x,u16 y)
{
  _x = x;
  _y = y;
  
  unsigned char DeepInfo = ePicBuf.Header.DeepInfo;
  if(DeepInfo & 0xe0 != (3 << 5)) return -1;  //ͼ�����ͣ�ֻ֧��������ɫͼ��
  DeepInfo &= ~0x1f;  //����ɫ�� 
  
  //b7��idat����,ɨ�跽��(b6~5), �˲�������(b4~2), ѹ������(b1~0)  
  unsigned char ZipInfo = ePicBuf.Header.ZipInfo;
  if(ZipInfo != 0) return -1;//ֻ֧������ɨ��Ĭ��ѹ�����Զ��˲���(������ʽ��)
  
  //�õ���ɫ����Ϣ
  unsigned short MapCount = ePicBuf.Header.PaletteCount;
  if(MapCount == 0) MapCount = 256;
   MapCount *= sizeof(Color_t);
  if(ePicBuf.NextDataSize <= MapCount) return -1;//�쳣

  //PNG����
  return DecodePNG(DeepInfo,//ɫ��,1,2,4,8,ֻ��֧������ɫ
                    ePicBuf.Header.w, //ͼ����
                    ePicBuf.Header.h,  //ͼ��߶�
                    ePicBuf.pNextData + MapCount,//idat�����ѹ������
                    ePicBuf.NextDataSize - MapCount);    //idat�������ݳ���
}

/*******************************************************************************
                              DecodePNG�ص�����
********************************************************************************/

//---------------------------�������Ƶ�n������---------------------------------
//��������DecodePNG()�е���,�Ա��������һ��ͼ��
void DecodePNG_cbOutLine(const unsigned char *data, //����ɫ����ʵɫ
                          unsigned short Line)        //��Ҫ���Ƶ���,1��ʼ
{
  static Color_t *pBuf; //��̬���´�ʹ��
  unsigned short w = ePicBuf.Header.w;
  //��Ӧ����Ϊ��ͼʹ��
  if(Line == 1)
    pBuf = Plot_cbAbsLocalArea(_x,_y, w, ePicBuf.Header.h); //��һ�У���ʾ��������ʼ
  else
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
   
  const unsigned char *map = ePicBuf.pNextData;  
  unsigned char mapSize = ePicBuf.Header.PaletteCount;
  for(unsigned short w = ePicBuf.Header.w; w > 0; w--, data++){
    pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data);
  }
}




