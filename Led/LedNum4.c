/***************************************************************************
                       4位数码管显示相关函数实现
***************************************************************************/

#include "LedNum4.h"
#include "Led.h"
#include "LedSign.h"

#include <string.h>
//-----------------------4位共阳数码管显示0-9字模----------------------

__flash const unsigned char LED_SIGN_NumLut[10] = \
{LED_SIGN_0,LED_SIGN_1,LED_SIGN_2,LED_SIGN_3,LED_SIGN_4,
 LED_SIGN_5,LED_SIGN_6,LED_SIGN_7,LED_SIGN_8,LED_SIGN_9};

//----------------------4位数显示函数-------------------------------
//靠右显示,为负时显示负号,指定小数点
void LedNum4_Disp(signed short Data,
                  //0~1bit最少显示位数-1(不含符号位)-1 2bit:是否带符号显示, 
                  //3bit:带符号时,正时是否填充0    4~7bit小数点显示掩码
                  unsigned char Flag) 
{
  //填充小数点掩码
  Led.DotDisp &= ~0x0f;
  Led.DotDisp |= Flag >> 4;
  
  //先清除显示
  unsigned char Pos = 0;  
  for(; Pos < 4; Pos++) Led.SegDisp[Pos] = 0;
  
  //填充正负号显示
  unsigned short Value;
  Pos = (Flag & 0x03) + 1; //最高位,最大==4
  
  if(Data < 0){//负值时
    if(Flag & 0x04){//允许显示负值时
      if(Pos == 4) Pos = 3;//截取显示
      Led.SegDisp[Pos] = LED_SIGN__;//负号  
      if(Data < -999) Data = -999; //负超限了(暂未支持截取显示)          
      Value = (unsigned short)(0 - Data); //置为正
    }
    else Value = 0;//不允许显示负时,显示0
  }
  else{//正值时
    Value = (unsigned short)Data;
    //错误修正:
    if(Pos == 4){//4位显示时
      if(Value > 9999) Value = 9999;//超量程截取显示
    }
    else{//3位及以下时
      if((Pos == 3) && (Value > 999)) Value = 999;//3位显示时，超量程截取显示
      else if((Pos == 2) && (Value > 99)) Value = 99;//2位显示时，超量程截取显示
      if((Flag & 0x0c) == 0x0c) //允许显示正时
        Led.SegDisp[Pos] = LED_SIGN_0;//上一位填充0表示正
    }
  }
  
  //填充数值,从最低位开始
  for(unsigned char i = 0; i < Pos; i++){
      Led.SegDisp[i] = LED_SIGN_NumLut[Value % 10];
      Value /= 10;
  }
}


//----------------------3位数显示函数-------------------------------
//最左位不管，3位靠右显示,为负时显示负号,指定小数点
void LedNum3_Disp(signed short Data,
                  //0~1bit最少显示位数-1(不含符号位) 2bit:是否带符号显示, 
                  //3bit:带符号时,正时是否填充0    4~6bit小数点显示掩码
                  unsigned char Flag) 
{
  //填充小数点掩码
  Led.DotDisp &= ~0x07;
  Led.DotDisp |= (Flag >> 4) & 0x07;
  
  //先清除低3位显示
  unsigned char Pos = 0;  
  for(; Pos < 3; Pos++) Led.SegDisp[Pos] = 0;
  
  //错误修正
  if(Data > 999) Data = 999;
  else if(Data < -999) Data = -999;
    
  //填充正负号显示
  unsigned short Value;
  Pos = (Flag & 0x03) + 1; //最高位
  if(Pos > 3) Pos = 3; //超限了
  
  if(Data < 0){//负值时
    if(Flag & 0x04){//允许显示负值时
      Led.SegDisp[Pos] = LED_SIGN__;//负号  
      if(Data < -99) Data = -99; //负超限了          
      Value = (unsigned short)(0 - Data); //置为正
    }
    else Value = 0;//不允许显示负时,显示0
  }
  else{
    Value = (unsigned short)Data;
    if(((Flag & 0x0c) == 0x0c)) 
      Led.SegDisp[Pos] = LED_SIGN_0;//无负时填充0
    else if((Flag & 0x04)){//符号占一位时
      if(Data > 99) Pos++; //超限了,符号位允许显示最高位值
    }
  }
  
  //填充数值,从最低位开始
  for(unsigned char i = 0; i < Pos; i++){
      Led.SegDisp[i] = LED_SIGN_NumLut[Value % 10];
      Value /= 10;
  }
}

//----------------------4位数显示ErrX函数-------------------------------
void LedNum4_ErrX(unsigned char ErrX) //故障码
{
  Led.SegDisp[0] = LED_SIGN_NumLut[ErrX];
  Led.SegDisp[1] = LED_SIGN_R;
  Led.SegDisp[2] = LED_SIGN_R;
  Led.SegDisp[3] = LED_SIGN_E;
  Led.SegFlash |= 0x0f;  //闪动显示
}

//---------------------------压缩十进制数值显示-------------------------------
//将Led.SegDisp中的数压缩显示，如000显示为0, 00.1显示为0.1
//05.00显示为5.00，小数点位用于表示精度，故不压缩显示,为负值时，不压缩显示
void LedNum4_NumDecZip(void) 
{
  //从高位SegDisp开始检查，若此项为0且无小数点则去掉不显示
  for(unsigned char Pos = 3; Pos > 0; Pos--){//至少显示一位
    if(Led.DotDisp & (1 << Pos)) break; //查找到小数点即结束了
    unsigned char Disp = Led.SegDisp[Pos];
    if(Disp == LED_SIGN_0) Led.SegDisp[Pos] = 0;//去除显示
    else if(Disp != 0) break;//有显示时(如负/数值/符号)，结束了
  }
}




