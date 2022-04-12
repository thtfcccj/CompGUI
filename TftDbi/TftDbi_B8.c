/*******************************************************************************

            TFT MCU接口驱动-TftDbi相关函数使用8位并口时，针对颜色操作的实现
//此接口支持RGB565, RGB666,RGB888色域，此实现与具体与具体使用显示屏控制器无关
********************************************************************************/

#include "TftDbi.h"
#include "TftDbi_Bx.h"
#include "Color.h"

//------------------------------色系转换外部声明--------------------------------
//RGB666索引色转换为显示屏用色系表
extern unsigned short  Color_RGB666_2RGB[]; 

//-----------------------------------写颜色函数--------------------------------
void TftDbi_WrColor(Color_t Color)
{
  TftDbi_cbWrClkL();  //低电平时准备入数据
  #ifndef SUPPORT_RGB_HW_565 //全色域
    unsigned long Data;
  #else
    unsigned short Data;
  #endif
  
  //系统用色系转换为显示屏用色系
  #ifdef SUPPORT_COLOR_RGB666//索引色
    if(Color < COLOR_COUNT) Data = Color_RGB666_2RGB[Color];
    else Data = *(TftDrv_pcbGetUserCLut() + (Color - COLOR_COUNT));
  #else 
    Data = Color; //其它与定义一一对应
  #endif
    
  //16~23b送出,颜色以高位在前送出
  #ifndef SUPPORT_RGB_HW_565 //GB666,RGB888，8位仅支持最大3位色值传输
    TftDbi_cbDbWr(Data >> 24);  
    TftDbi_cbDelayTwrl();//等待数据稳定
    TftDbi_cbWrClkH();  //上升沿锁存
    TftDbi_cbDelayTwrh();  //等待锁存
    TftDbi_cbWrClkL();    //低电平时准备入数据
  #endif
  //8~15b送出
  TftDbi_cbDbWr(Data >> 8);  
  TftDbi_cbDelayTwrl();//等待数据稳定
  TftDbi_cbWrClkH();  //上升沿锁存
  TftDbi_cbDelayTwrh();  //等待锁存
  TftDbi_cbWrClkL();    //低电平时准备入数据
  
  //0~7b送出
  TftDbi_cbDbWr(Data);  
  TftDbi_cbDelayTwrl();//等待数据稳定
  TftDbi_cbWrClkH();  //上升沿锁存
  TftDbi_cbDelayTwrh();  //等待锁存
  
}

//-----------------------------------读颜色函数--------------------------------
//读状态定义为:0结束读，负读起始，正读过程中
Color_t TftDbi_RdColor(void)
{
  TftDbi_cbDbIn();   //默认为输出状态需转换为输入
  
  TftDbi_cbRdClkL();  //低电平时准备数据 
  TftDbi_cbDelayTrdl();//等待数据及输出稳定
  unsigned char Data0 = TftDbi_cbDbRd(); //上升沿之前读出数据  
  TftDbi_cbWrClkH();  //上升告知读回来了
  
  TftDbi_cbRdClkL();  //低电平时准备数据 
  TftDbi_cbDelayTrdl();//等待数据及输出稳定
  unsigned char Data1 = TftDbi_cbDbRd(); //上升沿之前读出数据  
  TftDbi_cbWrClkH();  //上升告知读回来了  
  
  #ifndef SUPPORT_RGB_HW_565 //GB666,RGB888，8位仅支持最大3位色值传输
    TftDbi_cbRdClkL();  //低电平时准备数据 
    TftDbi_cbDelayTrdl();//等待数据及输出稳定
    unsigned char Data2 = TftDbi_cbDbRd(); //上升沿之前读出数据  
    TftDbi_cbWrClkH();  //上升告知读回来了
    Color_t Color = ((unsigned long)Data0 << 16) | 
                    ((unsigned long)Data1 << 8) | Data2;
  #else
    Color_t Color = ((unsigned short)Data0 << 8) | Data1;
  #endif

  //TftDbi_cbDelayTrdh(); //为下个数据准备
  //注：不恢复以支持连续读回，在写指令时会将DB转为输出状态。
  
  //显示屏用色系转换为系统用色系
  #ifdef SUPPORT_COLOR_RGB666
    //略
  #endif
  return Color;
}


