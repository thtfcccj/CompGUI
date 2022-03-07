/*******************************************************************************

          ePic格式图像子模块-绘制BMP图像非压缩引引图实现

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

//-----------------------------画PNG图-----------------------------------
//横向取模w*h点位,此函数不依赖调色板
//暂仅支持8B绘制
signed char ePic_PlotPNG(u16 x,u16 y)
{
  //缓冲以空间换时间
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;  
  unsigned char bitSize = ePicBuf.Header.DeepInfo;//色深，仅支持1，2，4，8值
  unsigned char mapSize =  ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = map + sizeof(Color_t) * mapSize;
  
  //数据头： 压缩信息固定为0x78, 0xda,第三位为压缩深度
  unsigned char Data = *data++; //取位数据
  //RFC-1950 CMF: 0-3b:CM=8表示deflate压缩方法, 4~7CINFO为7表示最大32K窗口
  if(Data != 0x78) return -1;//不支持的格式
  Data = *data++; //取位数据
  //FLG块: 0~4b: FCHECK:对CMF和FLG的校验位,b5:有预置字典  b6-7压缩等级
  //这里：b5=1有预置字典，压缩等级最高L3
  if(Data != 0xDA) return -1;//不支持的格式
  //因b5=1有预置字典，后跟DICTID，紧跟4字节组成
  
  Data = *data++; //取位压缩深度

  //未完，比理解的要复杂！
  
  
  
  return 0;
}
