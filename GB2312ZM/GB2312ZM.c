/***********************************************************************

                     GB2312字库模块相关函数实现
此实现为头部标准128个8*16ASC->后跟8*8英文字模->预留->标准GB2312字模
***********************************************************************/
#include "GB2312ZM.h"

//-----------------------------基址定义--------------------------
#define _ASC16_BASE    0x00000  //ASC码基址，128个8*16ASC,空间2048
#define _ASC8_BASE     0x00800  //ASC8*8码基址，128个8*8ASC,空间1024
                                //预留1024可作它用
#define _HZK_BASE      0x01000  //汉字基址

//--------------------------------获得英文字模-----------------------------
const unsigned char *GB2312ZM_pGetAsc(char Asc)
{
  if(Asc >= 128) Asc = ' ';  //异常检查并纠错
  return GB2312ZM_pGetZM(_ASC16_BASE + Asc * 16, 16);
}

//--------------------------------获得中文字模-----------------------------
const unsigned char *GB2312ZM_pGetHZ(unsigned short GB2312)
{
  //异常检查并纠错(0xnnFF忽略)
  if((GB2312 < 0xA1A1) || (GB2312 > 0xF7FE))
    return GB2312ZM_pGetZM(_HZK_BASE + 0, 32);
  
  unsigned short Pos = (GB2312 >> 8) - 0xa1; //区号
  Pos = Pos * 94 + ((GB2312 - 0xa1) & 0xff); //位置
  return GB2312ZM_pGetZM(_HZK_BASE + (unsigned long)Pos * 32, 32);
}

//--------------------------------获得英文8*8字模--------------------------
const unsigned char *GB2312ZM_pGetAsc8(char Asc)
{
  if(Asc >= 128) Asc = ' ';  //异常检查并纠错
  return GB2312ZM_pGetZM(_ASC8_BASE + Asc * 8, 8);
}
