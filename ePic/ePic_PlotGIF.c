/*******************************************************************************

          ePic��ʽͼ����ģ��-����GIFͼ���ѹ������ͼʵ��
��ģ��֧�ֶ��̵߳��ã�
********************************************************************************/

#include "ePic.h"
#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>
#include "struct.h" //struct_get()

#include "DecodeGIF.h"

//�ڲ��ṹ
struct _Plot{
  unsigned short x;
  unsigned short y;  
  Color_t *pBuf;            //�´�ʹ��
  struct _DecodeGIF Decode; //������
};

//struct _Plot Plot; //������Ҫ�ڴ�,Ĭ��ʱ��13780�ֽ�;
/*******************************************************************************
                              ��غ���ʵ��
********************************************************************************/

//---------------------------�������Ƶ�n������---------------------------------
//��������DecodeGIF()�е���,�Ա��������һ��ͼ��
static void _cbOutLine(const struct  _winWriter *out)
{
  struct _Plot *pPlot = struct_get(out, struct _Plot, Decode.wOut);
  
  const unsigned char *data = out->data + 1; //����ɫ����ʵɫ
  unsigned short w = ePicBuf.Header.w;
  //��Ӧ����Ϊ��ͼʹ��
  Color_t *pBuf = pPlot->pBuf;
  if(out->OutedSize <= out->U16Para)//��һ�У���ʾ��������ʼ
    pBuf = Plot_cbAbsLocalArea(pPlot->x,pPlot->y, w, ePicBuf.Header.h); 
  else
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
   
  const unsigned char *map = ePicBuf.pNextData;  
  unsigned char mapSize = ePicBuf.Header.PaletteCount;
  
  //����������������ֽ�Ϊ��λ!
  for(unsigned short w = ePicBuf.Header.w; w > 0; w--, data++){
    pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data);
  }
  pPlot->pBuf = pBuf;
}

/*******************************************************************************
                             ����
********************************************************************************/
//-----------------------------��GIFͼ-----------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//�ݽ�֧�� ȫ�־�̬��͸��ͼ����
signed char ePic_PlotGIF(u16 x,u16 y)
{
  struct _Plot *pPlot = 
                   (struct _Plot *)ePic_cbGetDecodeSpace(sizeof(struct _Plot));
  pPlot->x = x;
  pPlot->y = y;  

  //m(b7��ȫ�ֵ�ɫ��)cr(b6~b4,��ɫ���-1)s(b3) pixel(b3~0ȫ�ֵ�ɫ�����-1)��
  unsigned char DeepInfo = ePicBuf.Header.DeepInfo;
  DeepInfo = ((DeepInfo >> 4) & 0x07) + 1;  //����ɫ�� 
   
  //ePicBuf.Header.ZipInfo; ->������ɫ
  
  //�õ���ɫ����Ϣ
  unsigned short MapCount = ePicBuf.Header.PaletteCount;
  if(MapCount == 0) MapCount = 256;
   MapCount *= sizeof(Color_t);
  if(ePicBuf.NextDataSize <= MapCount) return -1;//�쳣

  //GIF����
  return DecodeGIF(&pPlot->Decode,//δ��ʼ���Ļ���
                    _cbOutLine,//�ص��������
                    DeepInfo,//ɫ��,1,2,4,8,ֻ��֧������ɫ
                    ePicBuf.Header.w, //ͼ�����
                    ePicBuf.Header.h,  //ͼ��߶�
                    ePicBuf.pNextData + MapCount,//idat�����ѹ������
                    ePicBuf.NextDataSize - MapCount);    //idat�������ݳ���
}




