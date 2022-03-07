/*******************************************************************************

          ePic��ʽͼ����ģ��-����BMPͼ���ѹ������ͼʵ��

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

//-----------------------------����ѹ������λͼ-------------------------------------
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
  const unsigned char *data = map + sizeof(Color_t) * mapSize;
  
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //��ʾ��������ʼ
  unsigned char bitMask = Plot_BitSize2Mask[bitSize];   //λ����
  for(; h > 0; h++){//��Ϊ��λ����
    u16 cw = w;
    do{//���һ������
      unsigned char Data = *data++; //ȡλ����
      unsigned char curLen;
      if(cw < (unsigned short)bitSize) curLen = cw; //��ȶ��룬���¶���
      else curLen = 8;
      for(unsigned char pos = 0; pos < curLen; pos += bitSize,cw--){
        //���ɫ
        unsigned char indexColor = (Data >> pos) & bitMask;
        pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, indexColor);
      }//end for
    }while(cw > 0);
    //��һ����
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
  }//end for h
}
