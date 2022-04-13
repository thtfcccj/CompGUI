/*******************************************************************************

                       字体管理模块
此模块描术了不同字库字体文件，用于统一绘制不同字体的文字或数据
********************************************************************************/
#ifndef _FONTS_H
#define _FONTS_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

#include "Color.h"

/******************************************************************************
                             相关结构
*******************************************************************************/

//支持的最多字体类型，由资源描
#ifndef FONTS_ID_MAX
  #define FONTS_ID_MAX   8
#endif

//支持的最大字符串长度，超过此长度不再绘制
#ifndef FONTS_STR_MAX
  #define FONTS_STR_MAX   (320 / 8)
#endif



/******************************************************************************
                             相关结构
*******************************************************************************/

//字体描述，可固化在资源中，每个固定占用32字节，组成陈列最多FONTS_ID_MAX个
struct _FontsDesc{
  unsigned long Base;        //存放在FLASH中的基址
  signed char  w;           //字体宽度
  signed char  h;           //字体高度
  unsigned char Type;       //取模类型，见定义
  char Note[25];             //展示在显示屏上的字体信息
};

//取模类型定义为：
//前3为ASC字体：
#define FONTS_ASC_STD   0  //ASC点阵标准字库 即0~127区域，共127个
#define FONTS_ASC_NANO  1 //ASC点阵简化字库 即32~127区域，共96个
#define FONTS_ASC_NUM   2 //ASC数字点阵字库 即ASC区:0X2A-0X39, 含*+-.,/和0~9， 共16个
//3-6为GB2312类型：
#define FONTS_2312_STD  3 //GB2312点阵标准字库: 即标准GB2312字，含无用区域
#define FONTS_2312_NOR  4 //GB2312点阵简化字库: 去除GB2312字中的空白区域(AA~AF)
#define FONTS_2312_NANO 5 //GB2312点阵精简字库: 去除GB2312字中的空白(AA~AF)与不常用(A4-A5，A7-A8)区域


#define FONTS_TYPE_COUNT 4 //最大支持种类
/*******************************************************************************
                              相关函数
********************************************************************************/

//------------------------------绘制一行指定字体字符串------------------------------
//调用前需提前设置好前景与背景,混合字体时，底部对齐
signed char Font_PlotLine(const struct _FontsDesc *pHfonts,//半角时使用的字体
                       const struct _FontsDesc *pFfonts, //全角时使用的字体
                       unsigned short x,               //屏上的x轴位置
                       unsigned short y,               //屏上的y轴位置
                       const char *pString);           //要绘制的字符串

/*******************************************************************************
                              回调函数
********************************************************************************/

//-------------------------由位置得到字模------------------------------------
//定义为程序内部时，直接指向转换
#ifdef FONTS_INNER  
#define  Font_pGetZM(base,len)    (const unsigned char*)(base)
//3. 字库芯片或外部Flash里，需实现缓冲:
#else
  const unsigned char *Font_pGetZM(unsigned long Base, unsigned char Len);      
#endif

#endif 


