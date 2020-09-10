/*******************************************************************************

                         TFT DBI操作统一接口
此接口为函数接口，底层需为根据具体DBI口如并口, SPI等
********************************************************************************/
#ifndef _TFT_DBI_H
#define _TFT_DBI_H

/*******************************************************************************
                                  相关配置
********************************************************************************/

#ifndef TFT_DRV_H_PIXEl//水平像素点数
  #define TFT_DRV_H_PIXEl    320     //竖屏时
#endif

#ifndef TFT_DRV_V_PIXEl//垂直像素点数
  #define TFT_DRV_V_PIXEl    480      //竖屏时
#endif

#ifndef TFT_DRV_TIM_OV   //定时器溢出时间,16ms为单位
  #define TFT_DRV_TIM_OV    2
#endif

/*******************************************************************************
                                  相关结构
********************************************************************************/
struct _TftDbi{
  unsigned char LastCmd; //缓冲最后写入的命令
  unsigned char Timer;   //定时器
};

extern struct _TftDbi TftDbi; //直接单例化

/*******************************************************************************
                         相关操作函数-显示屏相关操作函数
接口统一，由底层实现,全部为柱塞式
********************************************************************************/
#include "Color.h"

//---------------------------------初始化函数-----------------------------------
//返回负设备有问题
signed char TftDbi_Init(void);

//---------------------------------任务函数-----------------------------------
//放16ms进程中扫描
void TftDbi_Task(void);

/*******************************************************************************
                         相关操作函数-硬件相关操作函数
接口统一，由底层实现,全部为柱塞式
********************************************************************************/

//--------------------------------硬件初始化函数------------------------------
void TftDbi_HwInit(void);

//---------------------------------写指令函数----------------------------------
//指令固定为1个Byte
void TftDbi_WrCmd(unsigned char Cmd);

//----------------------------------片选禁止-----------------------------------
//在写指令时自动片选，结束后可手动禁止以提高稳定性。
void TftDbi_CsDis(void);

//------------------------------------写数据函数-------------------------------
void TftDbi_WrData(unsigned char Data);

//------------------------------------读数据函数-------------------------------
//注意大多数指令读出的首个数据为无效值
unsigned char TftDbi_RdData(void);

//-----------------------------------写颜色函数--------------------------------
void TftDbi_WrColor(Color_t Color);

//-----------------------------------读颜色函数--------------------------------
Color_t TftDbi_RdColor(void);

//------------------------------------读无效数据函数---------------------------
//若指令后,读出的是无效值,需调用此函数
//void TftDbi_RdInvalid(void);
#define TftDbi_RdInvalid()  do{TftDbi_RdData(); }while(0)

/*******************************************************************************
                         相关操作函数-组合操作函数
由本模块调用底层元操作函数实现
********************************************************************************/

//---------------------------------写U16数据函数-------------------------------
//先高后低顺序
void TftDbi_WrDataS(unsigned short Data);

//-------------------------------读首个带命令数据-------------------------------
//丢弃首个数据，返回Byte
unsigned char TftDbi_RdDataCmd(unsigned char Cmd);

//----------------------------------读U16数据函数-------------------------------
//先高后低顺序
unsigned short TftDbi_RdDataS(void);

//---------------------------写指令-带1Byte参数函数-----------------------------
//指令固定为1个Byte
void TftDbi_WrCmdB1(unsigned char Cmd, unsigned char Para);

//---------------------------写指令-带1Short参数函数----------------------------
//参数先高后低顺序
void TftDbi_WrCmdS1(unsigned char Cmd, unsigned short Para);

/*******************************************************************************
                                 回调函数
********************************************************************************/

//---------------------------------硬件初始化函数----------------------------------
void TftDbi_cbHwInit(void);

//----------------------得到用户颜色查找表----------------------------------
#if defined(SUPPORT_COLOR_RGB666) || defined(SUPPORT_COLOR_INDEX)
  const RgbHwType *TftDrv_pcbGetUserCLut(void); //表
  unsigned char TftDrv_cbGetUserCLutSize(void); //表大小,< (255 - COLOR_COUNT)
#endif

#endif //#define _TFT_DRV_H




