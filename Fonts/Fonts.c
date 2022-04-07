/*******************************************************************************

                       字体管理模块实现

********************************************************************************/

#include "Fonts.h"

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

//struct _FontsDesc _Desc;//测试
//----------------------------查找表--------------------------------------------
//ASC前三个类型的基址
static const unsigned char _TypeHalfBase[3] = {0, 32, '*'};
//ASC前三个类型的支持字符总量
static const unsigned char _TypeHalfCount[3] = {128, 128 - 32, 16};

//2312取模：标准类型与精简类型时区码 BO开始位置
static const unsigned char _Type2312B0[2] = {9,5};

//2312取模：标准类型与精简类型时区码 A1~AF对应位置

static const unsigned char _Type2312A1[][2] = {
  /*A1*/{0,0},/*A2*/{1,1},/*A3*/{2,2},/*A4*/{3,2}, 
  /*A5*/{4,2},/*A6*/{5,3},/*A7*/{6,3},/*A8*/{7,3},
  /*A9*/{8,4},/*AA*/{8,4},/*AB*/{8,4},/*AC*/{8,4},
  /*AD*/{8,4},/*AE*/{8,4},/*AF*/{8,4}, 
};

//--------------------------------得到半角字符字模------------------------------
//返回NULL异常不绘制
const unsigned char *Font_pGetHmode(const struct _FontsDesc *pHfonts,//半角字体
                                     unsigned char c)
{
  unsigned char Type = pHfonts->Type;
  if(Type >= 3) return NULL; //不是半角字体
  unsigned char Base = _TypeHalfBase[Type];
  //不在最低显示范围，显示最小字符
  if(c < Base) c = 0;
  else{
    c -= Base;
    if(c >= _TypeHalfCount[Type]) c = 0;
  }
  unsigned short Count = ((pHfonts->w + 7) >> 3) * pHfonts->h;
  return Font_pGetZM(pHfonts->Base + c * Count, Count);
}

//--------------------------------得到全角字符字模-------------------------------
//返回NULL异常不绘制
const unsigned char *Font_pGetFmode(const struct _FontsDesc *pHfonts,//半角字体
                                     unsigned char c1, //区码
                                     unsigned char c2) //位码
{
  unsigned char Type = pHfonts->Type;
  if((Type < 3) || (Type < 6))return NULL;//暂只支持GB2312
  
  //得到当前字对应的字模位置，区位码不对时，字模位置自动纠正至0
  unsigned short Pos;
  if((c1 < 0xA1) || (c1 > 0xF7)) Pos = 0;
  else if((c2 < 0xA1) || (c2 > 0xFE)) Pos = 0;
  else{//区位码对了
    c1 -= 0xA1;
    if(Type != FONTS_2312_NOR){//标准或精简字体时
      unsigned char Id = Type - FONTS_2312_NOR;
      if(Pos >= (0xB0 - 0xA1)) c1 -= _Type2312B0[Id]; //B0后
      else c1 = _Type2312A1[Id][c1];
    }
    Pos = c1 * 94 + (c2 - 0xa1);
  }
  unsigned short Count = ((pHfonts->w + 7) >> 3) * pHfonts->h;
  return Font_pGetZM(pHfonts->Base + Pos * Count, Count);
}

//------------------------------绘制指定字模字------------------------------
static void _PlotModule(const unsigned char *pMode,//取得的字模
                       unsigned short x,               //屏上的x轴位置
                       unsigned short y,               //屏上的y轴位置
                        unsigned char w, 
                        unsigned char h,
                        unsigned char MaxH)
{
  //底部对齐时，顶部需填充为背景色
  if(h < MaxH){
    Plot_FullRect(x,y,w,MaxH - h);
    y += MaxH - h;
  }
  
  //绘制当前字符
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //显示缓冲行起始
  for(; h > 0; h--){//行为单位绘制
    unsigned char curW = w;
    do{//宽度一点点绘制
      unsigned char Data = *pMode++; //取字模
      unsigned char curLen;
      if(curW < 8) curLen = w; //宽度对齐，余下丢弃
      else curLen = 8;
      for(unsigned char pos = 0; pos < curLen; pos++, curW--){
        Color_t color;
        if(Data & (1 << pos)) color = Plot_GetPenColor();
        else color = Plot_GetBrushColor();
        Plot_cbSetCurColor(pBuf, color);
      }//end for
    }while(curW > 0);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
  }//end for h
}

//------------------------------绘制一行指定字体字符串------------------------------
//调用前需提前设置好前景与背景,混合字体高度不同时，底部对齐
signed char Font_PlotLine(const struct _FontsDesc *pHfonts,//半角时使用的字体
                       const struct _FontsDesc *pFfonts,    //全角时使用的字体
                       unsigned short x,                    //屏上的x轴位置
                       unsigned short y,                    //屏上的y轴位置
                       const char *pString)                 //要绘制的字符串
{
  //得到最大高度，高度不同时，以底部对齐
  unsigned short Len = 0;
  unsigned char MaxH = 0;
  const char *ps = pString;  
  for(char c = *ps; c != '\0'; ps++){
    unsigned char h;
    if(c < 0x80) h = pHfonts->h; //ASCII直接转换
    else h = pFfonts->h;
    if(h > MaxH) MaxH = h;
    Len++;
    if(Len >= FONTS_STR_MAX) return -1; //长度异常不绘制
  }
  
  for(char c = *pString; c != '\0'; pString++){
    //============得到高宽与字模===============
    unsigned char w, h; 
    const unsigned char *pMode; //取得的字模
    if(c < 0x80){//ASCII直接转换
      w = pHfonts->w;
      h = pHfonts->h; 
      pMode = Font_pGetHmode(pHfonts,c);
    }
    else{//未GB2312检查正确性！！！！
      w = pFfonts->w;
      h = pFfonts->h; 
      pMode = Font_pGetFmode(pHfonts, c, *pString++);//取下半个字
    }
    x += w; //下个字符位置
    if(pMode != NULL) _PlotModule(pMode, x, y, w, h, MaxH);
  };
  return 0;
}



