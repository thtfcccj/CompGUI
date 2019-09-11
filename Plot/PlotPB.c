/*******************************************************************************
                         ��ͼ���׼�ӿ�
�˽ӿ�������TftDrv�ṩ��֡����
********************************************************************************/

#include "PlotPB.h"
#include "TftDrv.h"  //��ʾ������
#include    "GB2312ZM.h"  //ȡ��ģ����

/****************************************************************************
							               �ڲ�����ʵ��	
****************************************************************************/

//---------------------��ǰ������ɫ���Ƶ���ASC�ַ�---------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_Asc(unsigned short x,
                unsigned short y,
                char Code,             //�ַ�
                Color_t Pen,           //ǰ��ɫ
                Color_t Brush)         //����ɫ
{
  Color_t *pBuf = &TftDrv_Buf[y][x]; 
  const unsigned char *mask = GB2312ZM_pGetAsc(Code);
  const unsigned char *endmask = mask + 16; //��ģ����λ��
  
  for( ;mask < endmask; ){  
    unsigned char ZM = *mask++; //���е���->ָ������
    //��䱾��8����ɫ
    for(unsigned char Mask = 0x80; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = Pen;
      else  *pBuf = Brush;
    }  
    pBuf += (TFT_DRV_H_PIXEl - 8); //��һ�����λ����ʼ
  }
}

//-------------------��ǰ������ɫ���Ƶ���GB2312�ַ�---------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_GB2312(unsigned short x,
                   unsigned short y,
                   unsigned short Code,   //�ַ�
                   Color_t Pen,            //ǰ��ɫ
                   Color_t Brush)           //����ɫ
{
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  const unsigned char *mask = GB2312ZM_pGetHZ(Code);
  const unsigned char *endmask = mask + 32; //��ģ����λ��

  for( ;mask < endmask; ){  
    unsigned short ZM = *mask++; //������->ָ�������
    ZM <<= 8;
    ZM |= *mask++;   //������->ָ�����к�����
    //��䱾��16����ɫ
    for(unsigned short Mask = 0x8000; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = Pen;
      else  *pBuf = Brush;
    }
    pBuf += (TFT_DRV_H_PIXEl - 16); //��һ�����λ����ʼ
  } 
}

//-------------------������ǰ��������ɫ����һ��GB2312�ַ�---------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_StrColorP(unsigned short x,
                      unsigned short y,
                      const char *pCode,     //�ַ�
                      const Color_t *pPen,   //ǰ��ɫ
                      Color_t Brush,         //����ɫ
                      unsigned char Len)    //����
{
  for(; Len > 0; Len--){
    char c = *pCode;
    //ASCIIֱ��ת��
    if(c < 0x80){
      PlotPB_Asc(x, y, c, *pPen, Brush);
    }
    //δ���GB2312��ȷ�ԣ�������
    else{
      if(Len == 1) break;//������ֽ����ˣ��쳣 
      pCode++; //ȡ�°����
      PlotPB_GB2312(x, y,
                    ((unsigned short)c << 8) | *pCode, //��λ��ǰ
                    *pPen, Brush); 
      //�������
      x += 8;
      pPen++;  
    }
    //����������ͳһ����
    x += 8;
    pCode++;
    pPen++;
  }
}

//--------------------------------���̶������ַ�------------------------------
//��֧�ֱ���ɫ͸��
void PlotPB_FullC(unsigned short x,
                  unsigned short y,
                  const char Code,     //�ַ�
                  const Color_t Pen,   //ǰ��ɫ
                  Color_t Brush,         //����ɫ
                  unsigned char Len)    //����
{          
  for(; Len > 0; Len--, x += 8){
    PlotPB_Asc(x, y, Code, Pen, Brush);
  }
} 

