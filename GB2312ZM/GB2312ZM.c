/***********************************************************************

                     GB2312�ֿ�ģ����غ���ʵ��
��ʵ��Ϊͷ����׼128��8*16ASC->���8*8Ӣ����ģ->Ԥ��->��׼GB2312��ģ
***********************************************************************/
#include "GB2312ZM.h"

//-----------------------------��ַ����--------------------------
#define _ASC16_BASE    0x00000  //ASC���ַ��128��8*16ASC,�ռ�2048
#define _ASC8_BASE     0x00800  //ASC8*8���ַ��128��8*8ASC,�ռ�1024
                                //Ԥ��1024��������
#define _HZK_BASE      0x01000  //���ֻ�ַ

//--------------------------------���Ӣ����ģ-----------------------------
const unsigned char *GB2312ZM_pGetAsc(char Asc)
{
  if(Asc >= 128) Asc = ' ';  //�쳣��鲢����
  return GB2312ZM_pGetZM(_ASC16_BASE + Asc * 16, 16);
}

//--------------------------------���������ģ-----------------------------
const unsigned char *GB2312ZM_pGetHZ(unsigned short GB2312)
{
  //�쳣��鲢����(0xnnFF����)
  if((GB2312 < 0xA1A1) || (GB2312 > 0xF7FE))
    return GB2312ZM_pGetZM(_HZK_BASE + 0, 32);
  
  unsigned short Pos = (GB2312 >> 8) - 0xa1; //����
  Pos = Pos * 94 + ((GB2312 - 0xa1) & 0xff); //λ��
  return GB2312ZM_pGetZM(_HZK_BASE + (unsigned long)Pos * 32, 32);
}

//--------------------------------���Ӣ��8*8��ģ--------------------------
const unsigned char *GB2312ZM_pGetAsc8(char Asc)
{
  if(Asc >= 128) Asc = ' ';  //�쳣��鲢����
  return GB2312ZM_pGetZM(_ASC8_BASE + Asc * 8, 8);
}
