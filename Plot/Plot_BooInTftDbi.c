/*******************************************************************************
                  绘图库标准接口-专用于BOOT启动显示且在TftDbi上的实现
为适用不同显示屏，定义显示屏左上角为0点，各函数遵循从左到右(写完)，从上到下写原则。

此模块设计为尽量节省空间，并仅提供boot所需的清一行与写ASC5*6实现
********************************************************************************/

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include "GB2312ZM.h"//GB2312ZM_pGetAsc57()
#include <string.h>

/*******************************************************************************
                             调色板相关
*******************************************************************************/
struct _PlotPalette PlotPalette; //仅支持单线程调用

//----------------------绘制单个Asc字符->5*7点阵->双倍大小-----------------------
//横向取模8*8点位
void Plot_Asc57_Scale2(u16 x,u16 y,u8 code)
{
  /*Color_t *pBuf = */Plot_cbAbsLocalArea(x,y, 10, 16);  
  uc8 *mask = GB2312ZM_pGetAsc57(code);
  uc8 *endmask = mask + 7; //字模结束位置
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //本行点阵->指向下行
    //填充两行5点颜色
    for(u8 dLine = 0; dLine < 2; dLine++){//行数
      for(u8 Mask = 0x80; Mask > 0x04; Mask >>= 1){
        Color_t fullColor;
        if(ZM & Mask) fullColor = color;
        else fullColor = PlotPalette.brushColor;
        //x轴连续填充两次
        TftDbi_WrColor(fullColor); //Plot_cbSetCurColor(pBuf, fullColor);
        TftDbi_WrColor(fullColor); //Plot_cbSetCurColor(pBuf, fullColor); 
      }  
      //pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 10); //下一行填充位置起始
    }
  }
}

//---------------------------绘制asc5*7字符串->双倍大小-------------------------
void Plot_StrAsc57_Scale2(u16 x,u16 y,cc8* ptr,u8 length)//长度为0时为用strlen代替
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    //if(c < 0x80){//ASCII直接转换
      Plot_Asc57_Scale2(x, y, c);
      x += 10;
    //}
    //else{}//不支持,忽略
  }
}

//----------------------------填充矩形框-------------------------------------
//画刷颜色，即背景色
void Plot_FullRect(u16 x,u16 y,u16 w,u16 h)
{
  /*Color_t *pBuf = */Plot_cbAbsLocalArea(x,y, w, h); 
  Color_t color = PlotPalette.brushColor; //缓冲至寄存器，以加速度
  
  for(u32 c = h * w; c > 0; c--){
    TftDbi_WrColor(color); //Plot_cbSetCurColor(pBuf, fullColor);
  }
}


