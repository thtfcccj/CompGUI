/*******************************************************************************

         TFT MCU接口驱动-TftDbi相关函数使用16位并口时，针对颜色操作的实现
//此实现仅支持SUPPORT_RGB_HW_565,具体与具体使用显示屏控制器无关
********************************************************************************/
#include "TftDbi.h"
#include "TftDbi_Bx.h"
#include "Color.h"

#ifdef SUPPORT_RGB_HW_565

//------------------------------色系转换外部声明--------------------------------
//RGB666索引色转换为显示屏用色系表
extern unsigned short  Color_RGB666_2RGB[]; 

//-----------------------------------写颜色函数--------------------------------
//此实现仅支持SUPPORT_RGB_HW_565
void TftDbi_WrColor(Color_t Color)
{
  TftDbi_cbWrClkL();  //低电平时入数据
  unsigned short Data;
  
  //系统用色系转换为显示屏用色系
  #ifdef SUPPORT_COLOR_RGB666//索引色
    if(Color < COLOR_COUNT) Data = Color_RGB666_2RGB[Color];
    else Data = *(TftDrv_pcbGetUserCLut() + (Color - COLOR_COUNT));
  #else 
    Data = Color; //默认与定义一一对应
  #endif
  
  TftDbi_cbDbWr(Data);  
  TftDbi_cbDelayTwrl();//等待数据稳定
  TftDbi_cbWrClkH();  //上升沿锁存
  TftDbi_cbDelayTwrh();  //等待锁存  
}

//-----------------------------------读颜色函数--------------------------------
//读状态定义为:0结束读，负读起始，正读过程中
Color_t TftDbi_RdColor(void)
{
  TftDbi_cbDbIn();   //默放为输出状态需转换为输入
  TftDbi_cbRdClkL();  //低电平时准备数据 
  TftDbi_cbDelayTrdl();//等待数据及输出稳定
  unsigned short Data = TftDbi_cbDbRd(); //上升沿之前读出数据  
  TftDbi_cbWrClkH();  //上升告知读回来了
  
  //TftDbi_cbDelayTrdh(); //为下个数据准备
  //注：不恢复以支持连续读回，在写指令时会将DB转为输出状态。
  
  //显示屏用色系转换为系统用色系
  #ifdef SUPPORT_COLOR_RGB666
    Color_t Color = Data; //略
  #else
    Color_t Color = Data; //默认与定义一一对应
  #endif
  return Color;
}

#endif //SUPPORT_RGB_HW_565

