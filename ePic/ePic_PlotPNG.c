/*******************************************************************************

          ePic��ʽͼ����ģ��-����PNGͼ���ѹ������ͼʵ��
��ģ��֧�ֶ��̵߳��ã�
********************************************************************************/
#include "ePic_PlotPNG.h"


#include "ePic.h"
#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>
#include "struct.h" //struct_get()

//struct _PlotPNG Plot; //������Ҫ�ڴ�,Ĭ��ʱ��9100�ֽ�;
/*******************************************************************************
                              ��غ���ʵ��
********************************************************************************/

//---------------------------�������Ƶ�n������---------------------------------
//��������DecodePNG()�е���,�Ա��������һ��ͼ��
static void _cbOutLine(const struct  _winWriter *out)
{
  struct _PlotPNG *pPlot = struct_get(out, struct _PlotPNG, Decode.wOut);
  
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
  unsigned char bpp = out->U8Para;
  if(bpp >= 8){//�ֽ�Ϊ��λ
    for(unsigned short w = ePicBuf.Header.w; w > 0; w--, data++){
      pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data);
    }
  }
  else{//λΪ��λ
    ePic_pPlotIndexLine(pBuf, map, mapSize, data, bpp, w);//����ǰ��
  }
  
  pPlot->pBuf = pBuf;
}

/*******************************************************************************
                             ����
********************************************************************************/
//-----------------------------��PNGͼ-----------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//�ݽ�֧��8B����
signed char ePic_PlotPNG(u16 x,u16 y)
{
  struct _PlotPNG *pPlot = 
                   (struct _PlotPNG *)ePic_cbGetDecodeSpace(sizeof(struct _PlotPNG));
  pPlot->x = x;
  pPlot->y = y;  

  unsigned char DeepInfo = ePicBuf.Header.DeepInfo;
  if(DeepInfo & 0xe0 != (3 << 5)) return -1;  //ͼ�����ͣ�ֻ֧��������ɫͼ��
  DeepInfo &= 0x1f;  //����ɫ�� 
  
  //b7��idat����,ɨ�跽��(b6~5), �˲�������(b4~2), ѹ������(b1~0)  
  unsigned char ZipInfo = ePicBuf.Header.ZipInfo;
  if(ZipInfo != 0x80) return -1;//ֻ֧������ɨ��Ĭ��ѹ�����Զ��˲���(������ʽ��)
  
  //�õ���ɫ����Ϣ
  unsigned short MapCount = ePicBuf.Header.PaletteCount;
  if(MapCount == 0) MapCount = 256;
   MapCount *= sizeof(Color_t);
  if(ePicBuf.NextDataSize <= MapCount) return -1;//�쳣

  //PNG����
  return DecodePNG(&pPlot->Decode,//δ��ʼ���Ļ���
                    _cbOutLine,//�ص��������
                    DeepInfo,//ɫ��,1,2,4,8,ֻ��֧������ɫ
                    ePicBuf.Header.w, //ͼ����
                    ePicBuf.Header.h,  //ͼ��߶�
                    ePicBuf.pNextData + MapCount,//idat�����ѹ������
                    ePicBuf.NextDataSize - MapCount);    //idat�������ݳ���
}





