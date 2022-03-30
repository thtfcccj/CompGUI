/*******************************************************************************

                    ����ָ������ָʾģ��

********************************************************************************/
#include "UiTips.h"
#include <string.h>
#include "math_3.h"

#include "Plot.h"
#include "PlotPB.h"

struct _UiTips UiTips; //ֱ�Ӿ�̬����
//struct _UiTipsDesc DescTest;//ռλ����

/******************************************************************************
                              ��غ���ʵ��
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void UiTips_Init(void)
{
  memset(&UiTips, 0, sizeof(struct _UiTips));
}

//----------------------------������²��ֺ���----------------------------------
static void _ClrNextDisp(unsigned char UnsedLen)//��ʹ�ò���
{
  const struct _UiTipsDesc *pDesc = UiTips_cbGetDesc();
  if(UnsedLen >= pDesc->w) return;//ռ����
  Plot_SetBrushColor(pDesc->cBg);
  Plot_FullRect(pDesc->x + 8 * UnsedLen,pDesc->y,
                (pDesc->w - UnsedLen) * 8, 16); 
}

//----------------------------�����ַ�����ʾ---------------------------------
static void _UpdateString(unsigned char StrPos)//��0��1
{
  //���ַ���ǰ׺0~3����ǰ��ɫ
  const char *pStr = UiTips.pStr[StrPos];
  const struct _UiTipsDesc *pDesc = UiTips_cbGetDesc(); 
  char Num = *pStr; 
  Color_t cFg;
  if((Num >= '0') && (Num <= '3')){//ʹ��ָ��
    cFg = pDesc->cFg[Num - '0'];
    pStr++;//ʹ����һλ
  }
  else cFg = pDesc->cFg[StrPos]; //Ĭ�Ͼ�
  UiTips_cbUsedNotify();  
  Plot_SetBrushColor(pDesc->cBg);
  Plot_SetBrushColor(cFg);   
  unsigned char Len = strlen(pStr);
  if(Len > pDesc->w) Len = pDesc->w;
  Plot_String(pDesc->x, pDesc->y, pStr, Len);
  _ClrNextDisp(Len);//���µĲ������
}

//------------------------------������-----------------------------------
//������,1s����һ��
void UiTips_Task(void)
{
  //============================������ʾ����������������======================
  if(UiTips.TimerS){
    UiTips.TimerS--;

    //��ʱ������
    if(UiTips.TimerS == 0){
      if(UiTips.TimerL == 0){//��ʱҲ��������
        _ClrNextDisp(0);
        UiTips_cbUnUsedNotify();
      }
      return;
    }
 
    //��ʼ���
    if(UiTips.pStr[0] == NULL) return; //�쳣
    _UpdateString(0); //�����ַ���ʾ
    return;
  }

  //============================����������===========================            
  if(UiTips.TimerL == 0) return; //�޳������˳�
  
  //����ʾҪһֱ��ʾʱ����ռ�ý��˳�
  if(UiTips.TimerL == 0xff){
    if(UiTips_cbIsUsed()){ //��ռ��ʱ�˳�
      UiTips_cbUnUsedNotify();
      return;
    }
  }
  //��ʱ��ʱ
  else{ 
    UiTips.TimerL--;
    if(UiTips.TimerL == 0){//��������
      _ClrNextDisp(0);  
      UiTips_cbUnUsedNotify();
      return;
    }
  }
  
  //��ʼ���
  if(UiTips.pStr[1] == NULL) return; //�쳣
  _UpdateString(1); //�����ַ���ʾ
  return;
}

//------------------------------���³�����ʾ----------------------------------
void UiTips_UpdateL(const char *pStr,        //��ʾ�ַ���NULL��ʾȡ��
                         unsigned char TimeOv)   //��ʱʱ�䣬0xff��ʾ����
{
  UiTips.pStr[1] = pStr; 
  UiTips.TimerL = TimeOv;
  #ifdef SUPPORT_UI_TIPS_AT
    UiTips.CurStr = 1;
    UiTips.ReplaceCount = 0;    
  #endif
}

//------------------------------���¶�����ʾ----------------------------------
void UiTips_UpdateS(const char *pStr)  //��ʾ�ַ�
{
  UiTips.pStr[0] = pStr; 
  UiTips.TimerS = UiTips_cbGetDesc()->Sov;
  #ifdef SUPPORT_UI_TIPS_AT
    UiTips.CurStr = 0;
    UiTips.ReplaceCount = 0;    
  #endif
}

//--------------------------------�滻@����-------------------------------
//�˺������ַ������׸��ҵ���@�滻Ϊ��InfoҪ����֯��para
#ifdef SUPPORT_UI_TIPS_AT 
#include "stringEx.h"
  //Info����Ϊ��7b��paraΪ����������ʾ+-�ţ�6~3b���滻����,��3bit:С����λ��
void UiTips_ReplaceAt(unsigned char Info, //7b
                      unsigned long  para)  //���滻�Ĳ���
{
  if(UiTips.ReplaceCount == 0xff) return; //���滻�����
  char *pStr = UiTips.ParaBuf[UiTips.CurStr];
  if(UiTips.ReplaceCount == 0){//�״�ʹ���軺��
    strcpyEx(pStr, UiTips.pStr[UiTips.CurStr], SUPPORT_UI_TIPS_AT);
    UiTips.pStr[UiTips.CurStr] = pStr; //ָ���µ�λ��
  }
  unsigned char ParaLen = (Info >> 3) & 0x0f;
  if(ParaLen > 5) //�ݲ�֧�ִ�С������������
    Value4StringMin(para, UiTips.Para, ParaLen);
  else //�����С������Ϣ��������
    pNum2StringFlag(para, UiTips.Para, ParaLen, Info);
  //�滻
  if(StringReplace(pStr, "@",  UiTips.Para)) 
    UiTips.ReplaceCount = 0xff; //δ�ҵ�
  else UiTips.ReplaceCount++;
}

#endif








