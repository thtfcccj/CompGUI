/*******************************************************************************

                    彩屏指定区域指示模块

********************************************************************************/
#include "UiTips.h"
#include <string.h>
#include "math_3.h"

#include "Plot.h"
#include "PlotPB.h"

struct _UiTips UiTips; //直接静态定义
//struct _UiTipsDesc DescTest;//占位测试

/******************************************************************************
                              相关函数实现
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void UiTips_Init(void)
{
  memset(&UiTips, 0, sizeof(struct _UiTips));
}

//----------------------------清除余下部分函数----------------------------------
static void _ClrNextDisp(unsigned char UnsedLen)//已使用部分
{
  const struct _UiTipsDesc *pDesc = UiTips_cbGetDesc();
  if(UnsedLen >= pDesc->w) return;//占满了
  Plot_SetBrushColor(pDesc->cBg);
  Plot_FullRect(pDesc->x + 8 * UnsedLen,pDesc->y,
                (pDesc->w - UnsedLen) * 8, 16); 
}

//----------------------------更新字符串显示---------------------------------
static void _UpdateString(unsigned char StrPos)//短0长1
{
  //用字符串前缀0~3更新前景色
  const char *pStr = UiTips.pStr[StrPos];
  const struct _UiTipsDesc *pDesc = UiTips_cbGetDesc(); 
  char Num = *pStr; 
  Color_t cFg;
  if((Num >= '0') && (Num <= '3')){//使用指定
    cFg = pDesc->cFg[Num - '0'];
    pStr++;//使用了一位
  }
  else cFg = pDesc->cFg[StrPos]; //默认景
  UiTips_cbUsedNotify();  
  Plot_SetBrushColor(pDesc->cBg);
  Plot_SetBrushColor(cFg);   
  unsigned char Len = strlen(pStr);
  if(Len > pDesc->w) Len = pDesc->w;
  Plot_String(pDesc->x, pDesc->y, pStr, Len);
  _ClrNextDisp(Len);//余下的部分清除
}

//------------------------------任务函数-----------------------------------
//任务函数,1s调用一次
void UiTips_Task(void)
{
  //============================短期提示优先且无条件更新======================
  if(UiTips.TimerS){
    UiTips.TimerS--;

    //短时任务到了
    if(UiTips.TimerS == 0){
      if(UiTips.TimerL == 0){//长时也无任务了
        _ClrNextDisp(0);
        UiTips_cbUnUsedNotify();
      }
      return;
    }
 
    //开始填充
    if(UiTips.pStr[0] == NULL) return; //异常
    _UpdateString(0); //更新字符显示
    return;
  }

  //============================长期任务处理===========================            
  if(UiTips.TimerL == 0) return; //无长任务退出
  
  //长提示要一直显示时，被占用将退出
  if(UiTips.TimerL == 0xff){
    if(UiTips_cbIsUsed()){ //被占用时退出
      UiTips_cbUnUsedNotify();
      return;
    }
  }
  //长时计时
  else{ 
    UiTips.TimerL--;
    if(UiTips.TimerL == 0){//无任务了
      _ClrNextDisp(0);  
      UiTips_cbUnUsedNotify();
      return;
    }
  }
  
  //开始填充
  if(UiTips.pStr[1] == NULL) return; //异常
  _UpdateString(1); //更新字符显示
  return;
}

//------------------------------更新长期提示----------------------------------
void UiTips_UpdateL(const char *pStr,        //提示字符，NULL表示取消
                         unsigned char TimeOv)   //定时时间，0xff表示长期
{
  UiTips.pStr[1] = pStr; 
  UiTips.TimerL = TimeOv;
  #ifdef SUPPORT_UI_TIPS_AT
    UiTips.CurStr = 1;
    UiTips.ReplaceCount = 0;    
  #endif
}

//------------------------------更新短期提示----------------------------------
void UiTips_UpdateS(const char *pStr)  //提示字符
{
  UiTips.pStr[0] = pStr; 
  UiTips.TimerS = UiTips_cbGetDesc()->Sov;
  #ifdef SUPPORT_UI_TIPS_AT
    UiTips.CurStr = 0;
    UiTips.ReplaceCount = 0;    
  #endif
}

//--------------------------------替换@函数-------------------------------
//此函数将字符串里首个找到的@替换为接Info要求组织的para
#ifdef SUPPORT_UI_TIPS_AT 
#include "stringEx.h"
  //Info定义为：7b：para为带符号数显示+-号，6~3b：替换长度,低3bit:小数点位置
void UiTips_ReplaceAt(unsigned char Info, //7b
                      unsigned long  para)  //需替换的参数
{
  if(UiTips.ReplaceCount == 0xff) return; //已替换完成了
  char *pStr = UiTips.ParaBuf[UiTips.CurStr];
  if(UiTips.ReplaceCount == 0){//首次使用需缓冲
    strcpyEx(pStr, UiTips.pStr[UiTips.CurStr], SUPPORT_UI_TIPS_AT);
    UiTips.pStr[UiTips.CurStr] = pStr; //指向新的位置
  }
  unsigned char ParaLen = (Info >> 3) & 0x0f;
  if(ParaLen > 5) //暂不支持带小数点与正负号
    Value4StringMin(para, UiTips.Para, ParaLen);
  else //允许带小数点信息与正负号
    pNum2StringFlag(para, UiTips.Para, ParaLen, Info);
  //替换
  if(StringReplace(pStr, "@",  UiTips.Para)) 
    UiTips.ReplaceCount = 0xff; //未找到
  else UiTips.ReplaceCount++;
}

#endif








