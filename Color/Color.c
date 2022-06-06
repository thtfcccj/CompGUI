/*******************************************************************************

                   彩色显示屏时,标准颜色辅助函数实现

********************************************************************************/

#include "Color.h"


//-----------------------------指定色填充函数-----------------------------------
void Color_Full(Color_t C, Color_t *pC, unsigned short cCount)
{
  for(; cCount > 0; cCount--){
    *pC++ = C;
  }
}

//-------------------------------颜色copy函数-----------------------------------
void Color_Copy(Color_t *pDist, const Color_t *pSource, unsigned short cCount)
{
  for(; cCount > 0; cCount--){
    *pDist++ = *pSource++;
  }
}









