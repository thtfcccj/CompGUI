/*******************************************************************************

                    TFT MCU接口驱动-使用TypeB 16位并口时实现
此接口与具体使用显示屏控制器无关
********************************************************************************/

#include "TftDbi.h"
#include "TftDbi_B16.h"
#include "Color.h"

//--------------------------------硬件初始化函数------------------------------
void TftDbi_HwInit(void)
{
  TftDbi_cbIoCfg();
  //硬件复位
  TftDbi_cbStartRst();
  TftDbi_cbDelayHwRst();
  TftDbi_cbEndRst();
  TftDbi_cbDelayHwRstFinal();  //等待复位完成
  //初始化寄存器不在此
}

//---------------------------------写指令函数----------------------------------
//指令固定为1个Byte
void TftDbi_WrCmd(unsigned char Cmd)
{
  TftDbi_cbCsEn();
  TftDbi_cbDelayTcs(); //片选建立时间
  TftDbi_cbRsCmd();
  TftDbi_cbDbOut();   //置输出
  TftDbi_cbWrClkL();  //低电平时入数据
  TftDbi_cbDbWr(Cmd);
  //等待期间复位结构  
  TftDbi_cbDelayTwrl();//等待数据稳定
  TftDbi.LastCmd = Cmd;
  TftDbi.Timer = TFT_DRV_TIM_OV;
  TftDbi_cbWrClkH();  //上升沿锁存
  TftDbi_cbDelayTwrh();  //等待锁存
  
  TftDbi_cbRsData(); //最后恢复数据状态,片选保持
}

//----------------------------------片选禁止-----------------------------------
//在写指令时自动片选，结束后可手动禁止以提高稳定性。
void TftDbi_CsDis(void)
{
  TftDbi_cbCsDis();  
  TftDbi_cbDelayTchw(); //高电平最短时间
}

//------------------------------------写数据函数-------------------------------
void TftDbi_WrData(unsigned char Data)
{
  //默认在输出状态不需置输出
  TftDbi_cbWrClkL();  //低电平时入数据
  TftDbi_cbDbWr(Data);  
  TftDbi_cbDelayTwrl();//等待数据稳定
  TftDbi_cbWrClkH();  //上升沿锁存
  TftDbi_cbDelayTwrh();  //等待锁存  
}

//------------------------------------读数据函数-------------------------------
//注意大多数指令读出的首个数据为无效值
unsigned char TftDbi_RdData(void)
{
  TftDbi_cbDbIn();   //默放为输出状态需转换为输入
  TftDbi_cbRdClkL();  //低电平时准备数据 
  TftDbi_cbDelayTrdl();//等待数据及输出稳定
  unsigned char Data = TftDbi_cbDbRd(); //上升沿之前读出数据  
  TftDbi_cbRdClkH();  //上升告知读回来了
  
  TftDbi_cbDelayTrdh(); //为下个数据准备
  //注：不恢复以支持连续读回，在写指令时会将DB转为输出状态。
  
  return Data;
}

extern unsigned short  Color_RGB666_2RGB[];

//-----------------------------------写颜色函数--------------------------------
void TftDbi_WrColor(Color_t Color)
{
  TftDbi_cbWrClkL();  //低电平时入数据
  unsigned short Data;
  #ifdef SUPPORT_COLOR_RGB666//索引色转换
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
  
  //索引色转换
  #ifdef SUPPORT_COLOR_RGB666
    Color_t Color = Data; //略
  #else
    Color_t Color = Data; //默认与定义一一对应
  #endif
  return Data;
}


