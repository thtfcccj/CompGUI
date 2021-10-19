/***************************************************************************

                  TM1628驱动回调函数-在Spi3mIo模块上的实现
使用独占方式时
***************************************************************************/
#include "TM1628.h"
#include "Spi3mIo.h"
#include <string.h>

//Spi3mIo_CbCs实现，这里单例独占
const struct _Spi3mIoCs Spi3mIo_CbCs[SPI3_MIO_CS_COUNT] = {
  {//TM1628片选信息
    SPI3_MIO_CPOL_H | SPI3_MIO_CPHA_END | SPI3_MIO_DORD_LSB, //上升沿锁存,低位在前，结束沿采数据
    8, //最后字节位长度为8
    10, //位间电平保持时间, 这里为1us即500KHZ速率
    20, //字节间电平保持时间
  }
};

/***************************************************************************
                           回调函数实现
***************************************************************************/

//-----------------------------硬件初始化------------------------------------
void TM1628_cbHwInit(void)
{
  Spi3mIo_Init();
}

//----------------------------发送数据---------------------------------------
//TM1628.CommBuf存放要发送的数据,发送完成后返回
void TM1628_cbSendData(unsigned char SendLen)
{
  Spi3mIo_RW(0, TM1628.CommBuf, SendLen, NULL, 0);//比Spi3mIo_WO少级堆栈
}

//----------------------------按收按键值-------------------------------------
#ifdef SUPPORT_TM1628_KEY //支持按键时
  //TM1628.CommBuf[0]为已填充为读键值的1Byte指令,读回键值起始于TM1628。CommBuf[1]
void TM1628_cbFullKey(unsigned char KeyByteLen)
{
  Spi3mIo_RW(0, TM1628.CommBuf, 1, &TM1628.CommBuf[1], KeyByteLen);
}
#endif



