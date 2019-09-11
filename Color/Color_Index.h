/*******************************************************************************

                   彩色显示屏时,标准颜色-在使用自定义索引色时的标准颜色定义

********************************************************************************/
#ifndef _COLOR_INDEX_H
#define _COLOR_INDEX_H


//-----------------------------定义颜色类型占位----------------------------------
typedef unsigned char Color_t;

//-------------------------------颜色索引值定义---------------------------------

//                         //0预留
#define COLOR_WHITE    1   //白色
#define COLOR_RED      2   //红色
#define COLOR_GREEN    3   //绿色
#define COLOR_BLUE     4   //蓝色
#define COLOR_YELLOW   5   //黄色
#define COLOR_VIOLET   6   //蓝紫色
#define COLOR_CYAN     7   //青色
#define COLOR_ORANGE   8   //橙色
#define COLOR_BLACK    9   //黑色
#define COLOR_GRAY     10  //半灰色

#define COLOR_COUNT    10 //支持的颜色数量

#endif //#define _COLOR_INDEX_H
