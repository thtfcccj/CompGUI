/*******************************************************************************

                            ePic��ʽͼ��ģ��ʵ��

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

struct _ePicBuf ePicBuf; //��֧�ֵ��߳�

//------------------------�õ�˫�ֽ�����--------------------------------
#if SUPPORT_RI_MSB //��λ��ǰ
  #define _GetU16Data(data) ((unsigned short)*(data) << 8) + *((data) + 1);
#else //��λ��ǰ
  #define _GetU16Data(data) (((unsigned short)*((data) + 1)) << 8) + *(data);
#endif

//------------------------��ePicͼ���ʽ���ݸ�������ͷ--------------------------------
//ͬʱ��䷵�ط�0ͼ���ʽ ����
signed char ePic_ePicBuf(const unsigned char *ePicData)
{
  //==================ͼ��������ɫ�����====================
  unsigned char Data;
  #ifdef EPIC_FIX_PIC_TYPE
    Data = EPIC_FIX_PIC_TYPE;
  #else
    Data = *ePicData++;    
  #endif
  unsigned char Type = Data & 0x7f;//ͼ������    
  if((Type != 'w') || (Type != 'b') || (Type != 'p')){
      ePicBuf.Header.Type = 0;//0��֧��  
      return -1; //��֧�ֵ�ͼ���ʽ
  }
  ePicBuf.Header.Type = Type;
  
  //WBM��ʽ����ͷʱ�����п����߶����ݣ�ɫ��̶�Ϊ1
  #ifdef SUPPORT_EPIC_WBM_WH
    if(Type != 'w'){
      ePicBuf.Header.DeepInfo = *ePicData++;
    }
  #else
    ePicBuf.Header.DeepInfo = *ePicData++;
  #endif
  
  //===================ͼ�񳤿����===================
  if(Data & 0x80){//ѹ������
    Data = *ePicData++;
    if(Data & 0x80){
      ePicBuf.Header.w = ((unsigned short)(*ePicData++) << 7) + (Data & 0x7f);
    }
    Data = *ePicData++;
    if(Data & 0x80){
      ePicBuf.Header.h = ((unsigned short)(*ePicData++) << 7) + (Data & 0x7f);
    }
  }
  else{//�̶�����
    #if EPIC_HEADER_LEN == 1
      ePicBuf.Header.w = *ePicData++;
      ePicBuf.Header.h = *ePicData++;
    #else // RI_HEADER_LEN == 2
      ePicBuf.Header.w = _GetU16Data(ePicData);
      ePicData += 2;
      ePicBuf.Header.h = _GetU16Data(ePicData);
      ePicData += 2;  
    #endif
  }
  
  //===================ѹ���������ɫ����������===================
  //WBM��ʽ����ͷʱ�����п����߶����ݣ���ѹ�����ɫ��
  #ifdef SUPPORT_EPIC_WBM_WH
    if(Type != 'w'){
      ePicBuf.Header.ZipInfo = *ePicData++;
      ePicBuf.Header.PaletteCount = *ePicData++;      
    }
  #else
    ePicBuf.Header.ZipInfo = *ePicData++;
    ePicBuf.Header.PaletteCount = *ePicData++;
  #endif

  ePicBuf.pNextData = ePicData;
  return 0; //������ȷ��
}

//-----------------------------����ѹ������λͼ---------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//֧��1,2,4,8λ����λͼ�Ļ���
void ePic_PlotIndexBmp(u16 x,u16 y);

//-----------------------����BMP��ʽRLE4/8����-----------------------
signed char ePic_PlotRle4(unsigned short x, unsigned short y);
signed char ePic_PlotRle8(unsigned short x, unsigned short y);

//-----------------------------��PNGͼ-----------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//�ݽ�֧��8B����
signed char ePic_PlotPNG(u16 x,u16 y);

//------------------------------���ƻ��������----------------------------------
//���ô˺���ǰ��ePic_ePicBuf()��Ȼ�����úñ���ɫ��Plot_SetBrushColor()
//��Ϊwbm��ʽ����������ǰ����ǰ��ɫ��Plot_SetPenColor()
//����0��ȷ���ƣ������쳣
signed char ePic_Plot(unsigned short x,
                       unsigned short y)
{
  //�������ͷֱ����
  if(ePicBuf.Header.Type ==  'w'){//WBMP��ʽʱ
    Plot_Bmp1(x,y,ePicBuf.Header.w,ePicBuf.Header.h,
              ePicBuf.pNextData,      //���ݾ���,��λ��ǰ
              0);//��ɫ��ʾ��͸��
    return 0;
  }
  //BMP��ʽ��ͼ
  if(ePicBuf.Header.Type ==  'b'){
    if(ePicBuf.Header.ZipInfo == 0){//��ѹ��ʱ
      if(ePicBuf.Header.DeepInfo > 8){
        
        return -1;//�ݲ�֧��16��24��32λ������ͼ��
      }
      else{//1��2��4��8����ɫ
        ePic_PlotIndexBmp(x,y);
        return 0;
      }
    }
    else if(ePicBuf.Header.DeepInfo == 4)//LRE4ѹ����ʽʱ
      return ePic_PlotRle4(x,y);//RLE4��ʽ��ͼ
    else if(ePicBuf.Header.DeepInfo == 8)//LRE8ѹ����ʽʱ
      return ePic_PlotRle8(x,y);//RLE8��ʽ��ͼ
    return -1;//�쳣
  }
  
  //PNG��ʽʱ
  if(ePicBuf.Header.Type ==  'p'){
    return ePic_PlotPNG(x,y);
  }
  return -1;//�����ݲ�֧��  
}

//----------------------���õ��ð���Ƶ�ǰ��----------------------------
//����pBufλ��
Color_t *ePic_pPlotIndexDot(Color_t *pBuf,//����λ��
                       const unsigned char *map,//�������
                       unsigned char mapSize,    //�������С
                       unsigned char indexColor) //����ɫ
{
  //��͸��ʱ��䲢������һ��
  if((indexColor < mapSize) ||!mapSize){
    Color_t color; //Ϊ��ֹ���ұ�����δ����
    memcpy(&color, //MSB,LSB����Ӧ
           map + indexColor * sizeof(Color_t),
           sizeof(color));
    Plot_cbSetCurColor(pBuf, color);
  }
  else{//���ԶԵ�ǰ��Ļ���
    Plot_cbUpdateNext(pBuf);
  }
  return pBuf;
}



