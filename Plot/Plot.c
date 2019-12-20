/*******************************************************************************
                         ��ͼ���׼�ӿ�
�˽ӿ�������TftDrv�ṩ��֡����
********************************************************************************/

#include "Plot.h"
#include "TftDrv.h"  //��ʾ������
#include <string.h>

/*******************************************************************************
                             ��ɫ�����
*******************************************************************************/
struct _PlotPalette PlotPalette; //��֧�ֵ��̵߳���

//-------------------------------���õ�ɫ��---------------------------------
//penStyle��0
void Plot_CfgPalette(Color_t penColor, Color_t brushColor, u8 brushStyle)
{
  PlotPalette.penColor = penColor;
  PlotPalette.brushColor = brushColor;
  PlotPalette.brushStyle = brushStyle;  
}

/*******************************************************************************
                             ��������
*******************************************************************************/
#include "GB2312ZM.h"

//---------------------------���Ƶ���GB2312�����ַ�-----------------------------
//����ȡģ16*16��λ
void Plot_GB2312(u16 x, u16 y, u16 code)
{
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  uc8 *mask = GB2312ZM_pGetHZ(code);
  uc8 *endmask = mask + 32; //��ģ����λ��
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�

  for( ;mask < endmask; ){  
    u16 ZM = *mask++; //������->ָ�������
    ZM <<= 8;
    ZM |= *mask++;   //������->ָ�����к�����
    //��䱾��16����ɫ
    for(u16 Mask = 0x8000; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = color;
      else if(PlotPalette.brushStyle) *pBuf = PlotPalette.brushColor;
    }  
    pBuf += (TFT_DRV_H_PIXEl - 16); //��һ�����λ����ʼ
  }  
}
  
//----------------------���Ƶ���GB2312�����ַ�->˫����С------------------------
//����ȡģ32*32��λ
void Plot_GB2312_Scale2(u16 x,u16 y,u16 code)
{
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  uc8 *mask = GB2312ZM_pGetHZ(code);
  uc8 *endmask = mask + 32; //��ģ����λ��
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for( ;mask < endmask; ){  
    u16 ZM = *mask++; //������->ָ�������
    ZM <<= 8;
    ZM |= *mask++;   //������->ָ�����к�����
    //��䱾��16����ɫ
    for(u16 Mask = 0x8000; Mask > 0; Mask >>= 1, pBuf += 2){
      Color_t fullColor;
      if(ZM & Mask) fullColor = color;
      else if(PlotPalette.brushStyle) fullColor = PlotPalette.brushColor;
      else continue;
      //x��,y�������������
      *pBuf = fullColor;
      *(pBuf + 1) = fullColor;       
      *(pBuf + TFT_DRV_H_PIXEl) = fullColor; 
      *(pBuf + (TFT_DRV_H_PIXEl + 1)) = fullColor; 
    }
    pBuf += (TFT_DRV_H_PIXEl - 32) + TFT_DRV_H_PIXEl;//�����������λ����ʼ
  }
}

//----------------------���Ƶ���Asc�ַ�-----------------------------------
//����ȡģ8*16��λ
void Plot_Asc(u16 x,u16 y, u8 code)
{
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  uc8 *mask = GB2312ZM_pGetAsc(code);
  uc8 *endmask = mask + 16; //��ģ����λ��
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //���е���->ָ������
    //��䱾��8����ɫ
    for(u8 Mask = 0x80; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = color;
      else if(PlotPalette.brushStyle) *pBuf = PlotPalette.brushColor;
    }  
    pBuf += (TFT_DRV_H_PIXEl - 8); //��һ�����λ����ʼ
  }
}

//----------------------���Ƶ���Asc�ַ�->8*8����-------------------------------
//����ȡģ8*8��λ
void Plot_Asc8(u16 x,u16 y,u8 code)
{
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  uc8 *mask = GB2312ZM_pGetAsc8(code);
  uc8 *endmask = mask + 8; //��ģ����λ��
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //���е���->ָ������
    //��䱾��8����ɫ
    for(u8 Mask = 0x80; Mask > 0; Mask >>= 1, pBuf++){
      if(ZM & Mask) *pBuf = color;
      else if(PlotPalette.brushStyle) *pBuf = PlotPalette.brushColor;
    }  
    pBuf += (TFT_DRV_H_PIXEl - 8); //��һ�����λ����ʼ
  }
}

//----------------------���Ƶ���Asc�ַ�->˫����С------------------------
//����ȡģ16*32��λ
void Plot_Asc_Scale2(u16 x,u16 y,u8 code)
{
  Color_t *pBuf = &TftDrv_Buf[y][x];  
  uc8 *mask = GB2312ZM_pGetAsc(code);
  uc8 *endmask = mask + 16; //��ģ����λ��
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //���е���->ָ������
    //��䱾��16����ɫ
    for(u8 Mask = 0x80; Mask > 0; Mask >>= 1, pBuf += 2){
      Color_t fullColor;
      if(ZM & Mask) fullColor = color;
      else if(PlotPalette.brushStyle) fullColor = PlotPalette.brushColor;
      else continue;
      //x��,y�������������
      *pBuf = fullColor;
      *(pBuf + 1) = fullColor;       
      *(pBuf + TFT_DRV_H_PIXEl) = fullColor; 
      *(pBuf + (TFT_DRV_H_PIXEl + 1)) = fullColor; 
    }
    pBuf += (TFT_DRV_H_PIXEl - 16) + TFT_DRV_H_PIXEl;//�����������λ����ʼ
  }
}

//------------------------�����ַ���->֧��GB2312--------------------------
void Plot_String(u16 x,u16 y, cc8* ptr,u8 length)//����Ϊ0ʱΪ��strlen����
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    if(c < 0x80){//ASCIIֱ��ת��
      Plot_Asc(x, y, c);
      x += 8;
    }
    else{//δGB2312�����ȷ�ԣ�������
      ptr++; //ȡ�°����
      Plot_GB2312(x,y,((u16)c << 8) | *ptr); //��λ��ǰ
      x += 16;
    }
  }
}

//---------------------------����asc8*8�ַ���------------------------------
void Plot_StrAsc8(u16 x,u16 y,cc8* ptr,u8 length)//����Ϊ0ʱΪ��strlen����
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    if(c < 0x80){//ASCIIֱ��ת��
      Plot_Asc8(x, y, c);
      x += 8;
    }
    //else{}//��֧��,����
  }
}

//----------------------�����ַ���->->˫����С��֧��GB2312-----------------------
void Plot_String_Scale2(u16 x,u16 y, cc8* ptr,u8 length)//����Ϊ0ʱΪ��strlen����
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    if(c < 0x80){//ASCIIֱ��ת��
      Plot_Asc_Scale2(x, y, c);
      x += 16;
    }
    else{//δGB2312�����ȷ�ԣ�������
      ptr++; //ȡ�°����
      Plot_GB2312_Scale2(x,y,((u16)c << 8) | *ptr); //��λ��ǰ
      x += 32;
    }
  }
}

/*******************************************************************************
                              ��׼ͼ�ο�
********************************************************************************/

//------------------------------------����-------------------------------------
void Plot_Clear(void)
{
  Plot_FullRect(0, 0, TFT_DRV_H_PIXEl, TFT_DRV_V_PIXEl);
}

//----------------------------���Ƶ�����ˮƽֱ��---------------------------------
void Plot_LineH(u16 x,u16 y,u16 length)
{
  Color_t *pBuf = &TftDrv_Buf[y][x]; 
  Color_t *pEndBuf = pBuf + length; 
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for(; pBuf < pEndBuf; pBuf++) *pBuf = color;  
}

//----------------------------���Ƶ����ش�ֱֱ��---------------------------------
void Plot_LineV(u16 x,u16 y,u16 high)
{
  Color_t *pBuf = &TftDrv_Buf[y][x]; 
  Color_t *pEndBuf = pBuf + high * TFT_DRV_H_PIXEl;  
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for(; pBuf < pEndBuf; pBuf+= TFT_DRV_H_PIXEl) *pBuf = color;  
}

//----------------------------���Ƶ�����ֱ��---------------------------------
//ԭDrawLine
void Plot_Line(u16 x,u16 y,u16 length,
               u8 direction) //0:���򣬷�0:����
{
  if(direction) Plot_LineV(x, y, length);
  else Plot_LineH(x, y, length);
}

//----------------------------�����ο�-------------------------------------
//��ˢ��ɫ��������ɫ
void Plot_FullRect(u16 x,u16 y,u16 w,u16 h)
{
  Color_t *pBuf = &TftDrv_Buf[y][x]; 
  Color_t color = PlotPalette.brushColor; //�������Ĵ������Լ��ٶ�
  
  for(; h > 0; h--){
    Color_t *pEndBuf = pBuf + w; 
    for(; pBuf < pEndBuf; pBuf++) *pBuf = color;
    pBuf += (TFT_DRV_H_PIXEl - w);
  }
}

//-----------------------------������----------------------------------------
void Plot_Rect(u16 x,u16 y,u16 w,u16 h)
{
  Plot_LineH(x, y,    w);        //�Ϻ�
  Plot_LineH(x, y + h, w);       //�º�
  Plot_LineV(x,    y, h);        //����
  Plot_LineV(x+ w, y, h);        //����  
}

//-----------------------------�������λͼ����---------------------------------
//�ɵ�ɫ���С�õ�ռλ
static unsigned char _GetBitSize(u8 mapSize)
{
  if(mapSize <=  2) return 1;
  if(mapSize <=  4) return 2;
  if(mapSize <=  8) return 3;
  if(mapSize <=  16) return 4;
  if(mapSize <=  32) return 5;
  if(mapSize <=  64) return 6;
  if(mapSize <=  128) return 7;
  return 8;
}

//��ռλ�õ�����
static const u8 _BitSize2Mask[9] = {
  0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

//-----------------------------������λͼ-------------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
void Plot_IndexBmp(u16 x,u16 y,u16 w,u16 h,
                   uc8 *data,      //���ݾ���,��λ��ǰ
                   u8 mapSize,     //��ɫ���С
                   Color_t *map)  //��ɫ����ұ�,���ұ���Ϊ0ʱ��ʾ͸��
{
  Color_t *pBuf = &TftDrv_Buf[y][x];     //��ʾ��������ʼ
  
  u8 bitSize = _GetBitSize(mapSize);     //ռλ
  u32 dataBitSize = w * h * bitSize;     //��λ��������ݾ����С  
  u8 bitMask = _BitSize2Mask[bitSize];   //λ����

  Color_t *pLineEndBuf = pBuf + w;       //��ʾ���屾�н���
  u8 Data = *data; //��ȡ�׸�����
  for(u32 dataBit = 0; dataBit < dataBitSize; dataBit += bitSize){//������λ���ƶ�
    //1. �õ�����ɫ
    u8 curShift = dataBit & 0x07;                   //��ǰ��������Data�е���ʼBit
    u8 indexColor; //����ɫ
    //indexColor = (Data >> curShift) & bitMask; //��λ��ǰʱ��ȡ�������ܲ�ȫ
    //�¸���������Data�е���ʼBit
    u8 nextShift = (dataBit + bitSize) & 0x07;      
    if(nextShift <= curShift){//�¸�������ػ��ˣ������¸�data��
      indexColor = Data & _BitSize2Mask[8 - curShift];//��λ��ǰʱȡ��λ����
      data++; //�����¸�����
      Data = *data; //�������¸�����
      if(nextShift){//�¸������㲻�Ǵ�0��ʼ�ģ������β�ȫ����ȡ�����εĸ�λ����
        //indexColor |= (Data & _BitSize2Mask[nextShift]) << curShift;//��λ��ǰʱ
        indexColor <<= nextShift; //��λ��ǰʱ,��λ��ǰ��ǰ��
        indexColor |= (Data >> (8 - nextShift)) & _BitSize2Mask[nextShift];
      }
    }
    else{//��λ��ǰʱ���ˣ�ֱ��ȡ��
      indexColor = (Data >> ((8 - bitSize) - curShift)) & bitMask;
    }
    
    //2. ���ɫ
    Color_t color = *(map + indexColor);
    if(color) *pBuf = color; //��͸��ʱ���
    
    //3. �����¸���䣬���������ж�
    pBuf++;
    if(pBuf >= pLineEndBuf){//һ�н�����
      pLineEndBuf += TFT_DRV_H_PIXEl; //���н���
      pBuf = pLineEndBuf - w;  //������ʼ
    }
  }
}

//-----------------------------����ɫλͼ-------------------------------------
//����ȡģw*h��λ,�˺����������ⲿ��ɫ��
//paraΪλ���壺0x01:͸�� 0x02 ��ɫ��ʾ
void Plot_Bmp1(u16 x,u16 y,u16 w,u16 h, uc8 *data, u8 para)
{
  Color_t map[2];//��ɫ����ұ�  
  if(para & 0x01){//͸��ʱ
    map[0] = 0;//����ɫ��Ϊ0ʱ��ʾ͸��
    map[1] = PlotPalette.penColor;  //��ǰ��ɫ
  }
  else{//��͸��ʱ
    map[0] = PlotPalette.penColor;  //��ǰ��ɫ
    map[1] = PlotPalette.brushColor;//�ñ���ɫ
  }
  
  if(para & 0x02){ //��ɫʱ����
    Color_t s = map[0];
    map[0] = map[1];
    map[1] = s;
  }
  Plot_IndexBmp(x,y,w,h, data, 2, map);
}

//--------------------����Ӧ�����ǰ��ɫ�滻Ϊָ��ɫ----------------------------
void Plot_ReplacePenColor(u16 x,u16 y,u16 w,u16 h, Color_t newColor)
{
  Color_t *pBuf = &TftDrv_Buf[y][x]; 
  Color_t color = PlotPalette.brushColor; //�������Ĵ������Լ��ٶ�
  
  for(; h > 0; h--){
    Color_t *pEndBuf = pBuf + w; 
    for(; pBuf < pEndBuf; pBuf++){
      if(*pBuf != color) *pBuf = newColor;
    }
    pBuf += (TFT_DRV_H_PIXEl - w);
  }
}



