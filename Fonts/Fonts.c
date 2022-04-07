/*******************************************************************************

                       �������ģ��ʵ��

********************************************************************************/

#include "Fonts.h"

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

//struct _FontsDesc _Desc;//����
//----------------------------���ұ�--------------------------------------------
//ASCǰ�������͵Ļ�ַ
static const unsigned char _TypeHalfBase[3] = {0, 32, '*'};
//ASCǰ�������͵�֧���ַ�����
static const unsigned char _TypeHalfCount[3] = {128, 128 - 32, 16};

//2312ȡģ����׼�����뾫������ʱ���� BO��ʼλ��
static const unsigned char _Type2312B0[2] = {9,5};

//2312ȡģ����׼�����뾫������ʱ���� A1~AF��Ӧλ��

static const unsigned char _Type2312A1[][2] = {
  /*A1*/{0,0},/*A2*/{1,1},/*A3*/{2,2},/*A4*/{3,2}, 
  /*A5*/{4,2},/*A6*/{5,3},/*A7*/{6,3},/*A8*/{7,3},
  /*A9*/{8,4},/*AA*/{8,4},/*AB*/{8,4},/*AC*/{8,4},
  /*AD*/{8,4},/*AE*/{8,4},/*AF*/{8,4}, 
};

//--------------------------------�õ�����ַ���ģ------------------------------
//����NULL�쳣������
const unsigned char *Font_pGetHmode(const struct _FontsDesc *pHfonts,//�������
                                     unsigned char c)
{
  unsigned char Type = pHfonts->Type;
  if(Type >= 3) return NULL; //���ǰ������
  unsigned char Base = _TypeHalfBase[Type];
  //���������ʾ��Χ����ʾ��С�ַ�
  if(c < Base) c = 0;
  else{
    c -= Base;
    if(c >= _TypeHalfCount[Type]) c = 0;
  }
  unsigned short Count = ((pHfonts->w + 7) >> 3) * pHfonts->h;
  return Font_pGetZM(pHfonts->Base + c * Count, Count);
}

//--------------------------------�õ�ȫ���ַ���ģ-------------------------------
//����NULL�쳣������
const unsigned char *Font_pGetFmode(const struct _FontsDesc *pHfonts,//�������
                                     unsigned char c1, //����
                                     unsigned char c2) //λ��
{
  unsigned char Type = pHfonts->Type;
  if((Type < 3) || (Type < 6))return NULL;//��ֻ֧��GB2312
  
  //�õ���ǰ�ֶ�Ӧ����ģλ�ã���λ�벻��ʱ����ģλ���Զ�������0
  unsigned short Pos;
  if((c1 < 0xA1) || (c1 > 0xF7)) Pos = 0;
  else if((c2 < 0xA1) || (c2 > 0xFE)) Pos = 0;
  else{//��λ�����
    c1 -= 0xA1;
    if(Type != FONTS_2312_NOR){//��׼�򾫼�����ʱ
      unsigned char Id = Type - FONTS_2312_NOR;
      if(Pos >= (0xB0 - 0xA1)) c1 -= _Type2312B0[Id]; //B0��
      else c1 = _Type2312A1[Id][c1];
    }
    Pos = c1 * 94 + (c2 - 0xa1);
  }
  unsigned short Count = ((pHfonts->w + 7) >> 3) * pHfonts->h;
  return Font_pGetZM(pHfonts->Base + Pos * Count, Count);
}

//------------------------------����ָ����ģ��------------------------------
static void _PlotModule(const unsigned char *pMode,//ȡ�õ���ģ
                       unsigned short x,               //���ϵ�x��λ��
                       unsigned short y,               //���ϵ�y��λ��
                        unsigned char w, 
                        unsigned char h,
                        unsigned char MaxH)
{
  //�ײ�����ʱ�����������Ϊ����ɫ
  if(h < MaxH){
    Plot_FullRect(x,y,w,MaxH - h);
    y += MaxH - h;
  }
  
  //���Ƶ�ǰ�ַ�
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //��ʾ��������ʼ
  for(; h > 0; h--){//��Ϊ��λ����
    unsigned char curW = w;
    do{//���һ������
      unsigned char Data = *pMode++; //ȡ��ģ
      unsigned char curLen;
      if(curW < 8) curLen = w; //��ȶ��룬���¶���
      else curLen = 8;
      for(unsigned char pos = 0; pos < curLen; pos++, curW--){
        Color_t color;
        if(Data & (1 << pos)) color = Plot_GetPenColor();
        else color = Plot_GetBrushColor();
        Plot_cbSetCurColor(pBuf, color);
      }//end for
    }while(curW > 0);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //��һ��
  }//end for h
}

//------------------------------����һ��ָ�������ַ���------------------------------
//����ǰ����ǰ���ú�ǰ���뱳��,�������߶Ȳ�ͬʱ���ײ�����
signed char Font_PlotLine(const struct _FontsDesc *pHfonts,//���ʱʹ�õ�����
                       const struct _FontsDesc *pFfonts,    //ȫ��ʱʹ�õ�����
                       unsigned short x,                    //���ϵ�x��λ��
                       unsigned short y,                    //���ϵ�y��λ��
                       const char *pString)                 //Ҫ���Ƶ��ַ���
{
  //�õ����߶ȣ��߶Ȳ�ͬʱ���Եײ�����
  unsigned short Len = 0;
  unsigned char MaxH = 0;
  const char *ps = pString;  
  for(char c = *ps; c != '\0'; ps++){
    unsigned char h;
    if(c < 0x80) h = pHfonts->h; //ASCIIֱ��ת��
    else h = pFfonts->h;
    if(h > MaxH) MaxH = h;
    Len++;
    if(Len >= FONTS_STR_MAX) return -1; //�����쳣������
  }
  
  for(char c = *pString; c != '\0'; pString++){
    //============�õ��߿�����ģ===============
    unsigned char w, h; 
    const unsigned char *pMode; //ȡ�õ���ģ
    if(c < 0x80){//ASCIIֱ��ת��
      w = pHfonts->w;
      h = pHfonts->h; 
      pMode = Font_pGetHmode(pHfonts,c);
    }
    else{//δGB2312�����ȷ�ԣ�������
      w = pFfonts->w;
      h = pFfonts->h; 
      pMode = Font_pGetFmode(pHfonts, c, *pString++);//ȡ�°����
    }
    x += w; //�¸��ַ�λ��
    if(pMode != NULL) _PlotModule(pMode, x, y, w, h, MaxH);
  };
  return 0;
}



