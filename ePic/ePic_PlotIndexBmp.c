/*******************************************************************************

          ePic��ʽͼ����ģ��-����BMPͼ���ѹ������ͼʵ��

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

//-----------------------------����ѹ������λͼ---------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//֧��1,2,4,8λ����λͼ�Ļ���
void ePic_PlotIndexBmp(u16 x,u16 y)
{
  //�����Կռ任ʱ��
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;  
  unsigned char bitSize = ePicBuf.Header.DeepInfo;//ɫ���֧��1��2��4��8ֵ
  unsigned char mapSize =  ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = ePic_pGetImageData();
  //����ÿ������ռ�ȣ�4�ֽڶ���
  unsigned short DataW = 8 / bitSize;
  DataW = (w + (DataW - 1)) / DataW;
  if(DataW & 0x03) DataW += 4 - (DataW & 0x03);//4�ֽڶ���
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //��ʾ��������ʼ  
  for(; h > 0; h--){//��Ϊ��λ����
    pBuf = ePic_pPlotIndexLine(pBuf, map, mapSize, data, bitSize, w);//����ǰ��
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
    data += DataW; //��������
  }//end for h
}
