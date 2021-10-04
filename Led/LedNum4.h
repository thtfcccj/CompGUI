/***************************************************************************
                       4位数码管显示相关函数实现接口
***************************************************************************/

#ifndef __LED_NUM4_H
#define __LED_NUM4_H

/***************************************************************************
                              相关配置
***************************************************************************/


/***************************************************************************
                              相关结构
***************************************************************************/

/***************************************************************************
                              相关函数
***************************************************************************/


//----------------------4位数显示函数-------------------------------
//靠右显示,为负时显示负号,指定小数点
void LedNum4_Disp(signed short Data,
                  //0~1bit最少显示位数-1(不含符号位)-1 2bit:是否带符号显示, 
                  //3bit:带符号时,正时是否填充0    4~7bit小数点显示掩码
                  unsigned char Flag);

//----------------------3位数显示函数-------------------------------
//最左位不管，3位靠右显示,为负时显示负号,指定小数点
void LedNum3_Disp(signed short Data,
                  //0~1bit最少显示位数-1(不含符号位) 2bit:是否带符号显示, 
                  //3bit:带符号时,正时是否填充0    4~6bit小数点显示掩码
                  unsigned char Flag);

//----------------------4位数显示ErrX函数-------------------------------
void LedNum4_ErrX(unsigned char ErrX); //故障码

//---------------------------压缩十进制数值显示-------------------------------
//将Led.SegDisp中的数压缩显示，如000显示为0, 00.1显示为0.1
//05.00显示为5.00，小数点位用于表示精度，故不压缩显示,为负值时，不压缩显示
void LedNum4_NumDecZip(void);


/***************************************************************************
                           回调函数
***************************************************************************/

#endif //#define __LED_H

