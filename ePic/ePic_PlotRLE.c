/*******************************************************************************

          ePic��ʽͼ����ģ��-����BMPͼ��RLEѹ����ʽʵ��

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

//-----------------------���ָ������0����-----------------------------
static Color_t * _Full0(Color_t *pBuf,
                        unsigned short cw, //��ǰ�����²���
                        unsigned short h,  //���²��ָ߶�
                        unsigned short ch) //�����ĸ߶�
{
  unsigned short w = ePicBuf.Header.w;
  if((cw != 0) && (cw != w)){//���������²�����
    pBuf = ePic_pPlotIndexDotRepeat(pBuf, ePicBuf.pNextData, 
                                    ePicBuf.Header.PaletteCount, 0, cw);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - cw);
    h--;  
    if(h == 0) return pBuf; //�쳣����
  }
  for(; ch > 0; ch--){//yƫ��,������
    pBuf = ePic_pPlotIndexDotRepeat(pBuf, ePicBuf.pNextData, 
                                    ePicBuf.Header.PaletteCount, 0, w);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w);
    h--;
    if(h == 0) break; //����
  }
  return pBuf; //����
}

//-----------------------����BMP��ʽRLE4����-----------------------
signed char ePic_PlotRle4(unsigned short x, unsigned short y)
{
  //�����Կռ任ʱ��
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;  
  unsigned char mapSize =  ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = ePic_pGetImageData();
  
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //��ʾ��������ʼ
  for(; h > 0; h--){//��Ϊ��λ����
    signed short cw = w;
    do{//���һ������
      unsigned char Data0 = *data++; //ȡλ����
      //=======================���ݲ�ѹ������======================
      if(Data0 == 0){
        unsigned char Count = *data++;
        if(Count == 0){//���н���
          pBuf = _Full0(pBuf, cw, h, 0);//��䱾������
          break; 
        }
        if(Count == 1){//ͼ�����,���������
          if(h == 0) return 0;
          h --;
          pBuf = _Full0(pBuf, cw, h, h);
          return 0; 
        }
        if(Count == 2){
          //���������ֽڱ�ʾ��һ�����شӵ�ǰ��ʼ��ˮƽ�ʹ�ֱλ�õ�ƫ������
          //https://docs.microsoft.com/zh-cn/windows/win32/gdi/bitmap-compression
          pBuf = _Full0(pBuf, cw, h, 0);//��䱾������
          unsigned short offsetX = *data++; //xƫ��
          unsigned short offsetY = *data++; //Yƫ��
          if(offsetY){//Yƫ�����  
            pBuf = _Full0(pBuf, 0, h, offsetY);
            h -= offsetY;
          }
          if(offsetX){//x��ƫ��ʱ���������
            cw -= offsetX; 
            pBuf = ePic_pPlotIndexDotRepeat(pBuf, map, mapSize, 0, (w - cw));
          }
        }
        else{//����״̬��ʾ�������˫�ֽڶ��룬�磺
          //�磺00 06 45 56 67 00 ��ӦΪ��4 5 5 6 6 7
          unsigned char ByteCount =  Count >> 1;
          do{
            unsigned char DotD = *data++;//һ��ȡ������
            pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD >> 4);//��λ��ǰ
            Count--; cw--;//�ڶ���
            if((Count == 0) || (cw == 0)) break; //�˵����н�����
            pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD & 0x0f);//��λ�ں�
            Count--; cw--;//��һ��
          }while((Count > 0) && (cw > 0)); 
          if(ByteCount & 0x01) data++; //˫�ֽڶ���,�����ֽڶ���  
        }
      }
      //=======================ѹ�����ݴ���=========================      
      else{//,Data0Ϊ�ظ��������磺05 06 ����Ϊ��0 6 0 6 0
        unsigned char DotD = *data++;//��ǰ����,һ��ȡ������
        do{
          pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD >> 4);//��λ��ǰ
          Data0--; cw--;//�ڶ���
          if((Data0 == 0) || (cw == 0)) break; //�˵����н�����
          pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD & 0x0f);//��λ�ں�
          Data0--; cw--;//��һ��
        }while((Data0 > 0) && (cw > 0));
      }
    }while(cw >= 0);
    //��һ����
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
  }//end for h
  return 0;
}

//-----------------------����BMP��ʽRLE8����-----------------------
signed char ePic_PlotRle8(unsigned short x, unsigned short y)
{
  //�����Կռ任ʱ�� 
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;   
  unsigned char mapSize = ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = ePic_pGetImageData();
  
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //��ʾ��������ʼ
  for(; h > 0; h--){//��Ϊ��λ����
    signed short cw = w;
    do{//���һ������
      volatile unsigned char Data0 = *data++; //ȡλ����
      //=======================���ݲ�ѹ������======================
      if(Data0 == 0){
        unsigned char Count = *data++;
        if(Count == 0){//���н���
          pBuf = _Full0(pBuf, cw, h, 0);//��䱾������
          break; 
        }
        if(Count == 1){//ͼ�����,���������
          if(h == 0) return 0;
          h --;
          pBuf = _Full0(pBuf, cw, h, h);
          return 0; 
        }
        if(Count == 2){
          //���������ֽڱ�ʾ��һ�����شӵ�ǰ��ʼ��ˮƽ�ʹ�ֱλ�õ�ƫ������
          //https://docs.microsoft.com/zh-cn/windows/win32/gdi/bitmap-compression
          pBuf = _Full0(pBuf, cw, h, 0);//��䱾������
          unsigned short offsetX = *data++; //xƫ��
          unsigned short offsetY = *data++; //Yƫ��
          if(offsetY){//Yƫ�����  
            pBuf = _Full0(pBuf, 0, h, offsetY);
            h -= offsetY;
          }
          if(offsetX){//x��ƫ��ʱ���������
            cw -= offsetX; 
            pBuf = ePic_pPlotIndexDotRepeat(pBuf, map, mapSize, 0, (w - cw));
          }
        }
        else{//����״̬��ʾ�������˫�ֽڶ��룬�磺
          //�磺00 03 45 56 67 00  ����Ϊ��45 56 67
          unsigned char ByteCount =  Count;
          do{
            pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data++);
            Count--; cw--;//��һ��
          }while((Count > 0) && (cw > 0)); 
          if(ByteCount & 0x01) data++; //˫�ֽڶ���,�����ֽڶ���   
        }
      }
      //=======================ѹ�����ݴ���=========================      
      else{//Data0Ϊ�ظ��������磺03 04  ����Ϊ��04 04 04
        pBuf = ePic_pPlotIndexDotRepeat(pBuf, map, mapSize, *data++, Data0);
        cw -= Data0;
      }
    }while(cw >= 0);
    //��һ����
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
  }//end for h
  return 0;
}



