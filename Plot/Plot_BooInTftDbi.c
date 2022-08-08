/*******************************************************************************
                  ��ͼ���׼�ӿ�-ר����BOOT������ʾ����TftDbi�ϵ�ʵ��
Ϊ���ò�ͬ��ʾ����������ʾ�����Ͻ�Ϊ0�㣬��������ѭ������(д��)�����ϵ���дԭ��

��ģ�����Ϊ������ʡ�ռ䣬�����ṩboot�������һ����дASC5*6ʵ��
********************************************************************************/

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include "GB2312ZM.h"//GB2312ZM_pGetAsc57()
#include <string.h>

/*******************************************************************************
                             ��ɫ�����
*******************************************************************************/
struct _PlotPalette PlotPalette; //��֧�ֵ��̵߳���

//----------------------���Ƶ���Asc�ַ�->5*7����->˫����С-----------------------
//����ȡģ8*8��λ
void Plot_Asc57_Scale2(u16 x,u16 y,u8 code)
{
  /*Color_t *pBuf = */Plot_cbAbsLocalArea(x,y, 10, 16);  
  uc8 *mask = GB2312ZM_pGetAsc57(code);
  uc8 *endmask = mask + 7; //��ģ����λ��
  Color_t color = PlotPalette.penColor; //�������Ĵ������Լ��ٶ�
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //���е���->ָ������
    //�������5����ɫ
    for(u8 dLine = 0; dLine < 2; dLine++){//����
      for(u8 Mask = 0x80; Mask > 0x04; Mask >>= 1){
        Color_t fullColor;
        if(ZM & Mask) fullColor = color;
        else fullColor = PlotPalette.brushColor;
        //x�������������
        TftDbi_WrColor(fullColor); //Plot_cbSetCurColor(pBuf, fullColor);
        TftDbi_WrColor(fullColor); //Plot_cbSetCurColor(pBuf, fullColor); 
      }  
      //pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 10); //��һ�����λ����ʼ
    }
  }
}

//---------------------------����asc5*7�ַ���->˫����С-------------------------
void Plot_StrAsc57_Scale2(u16 x,u16 y,cc8* ptr,u8 length)//����Ϊ0ʱΪ��strlen����
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    //if(c < 0x80){//ASCIIֱ��ת��
      Plot_Asc57_Scale2(x, y, c);
      x += 10;
    //}
    //else{}//��֧��,����
  }
}

//----------------------------�����ο�-------------------------------------
//��ˢ��ɫ��������ɫ
void Plot_FullRect(u16 x,u16 y,u16 w,u16 h)
{
  /*Color_t *pBuf = */Plot_cbAbsLocalArea(x,y, w, h); 
  Color_t color = PlotPalette.brushColor; //�������Ĵ������Լ��ٶ�
  
  for(u32 c = h * w; c > 0; c--){
    TftDbi_WrColor(color); //Plot_cbSetCurColor(pBuf, fullColor);
  }
}


