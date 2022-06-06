/*******************************************************************************

          ePic格式图像子模块-绘制BMP图像，RLE压缩方式实现

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

//-----------------------填充指定区域0函数-----------------------------
static Color_t * _Full0(Color_t *pBuf,
                        unsigned short cw, //当前行余下部分
                        unsigned short h,  //余下部分高度
                        unsigned short ch) //需填充的高度
{
  unsigned short w = ePicBuf.Header.w;
  if((cw != 0) && (cw != w)){//本行有余下部分了
    pBuf = ePic_pPlotIndexDotRepeat(pBuf, ePicBuf.pNextData, 
                                    ePicBuf.Header.PaletteCount, 0, cw);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - cw);
    h--;  
    if(h == 0) return pBuf; //异常结束
  }
  for(; ch > 0; ch--){//y偏移,即行数
    pBuf = ePic_pPlotIndexDotRepeat(pBuf, ePicBuf.pNextData, 
                                    ePicBuf.Header.PaletteCount, 0, w);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w);
    h--;
    if(h == 0) break; //结束
  }
  return pBuf; //结束
}

//-----------------------绘制BMP格式RLE4数据-----------------------
signed char ePic_PlotRle4(unsigned short x, unsigned short y)
{
  //缓冲以空间换时间
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;  
  unsigned char mapSize =  ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = ePic_pGetImageData();
  
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //显示缓冲行起始
  for(; h > 0; h--){//行为单位绘制
    signed short cw = w;
    do{//宽度一点点绘制
      unsigned char Data0 = *data++; //取位数据
      //=======================数据不压缩处理======================
      if(Data0 == 0){
        unsigned char Count = *data++;
        if(Count == 0){//此行结束
          pBuf = _Full0(pBuf, cw, h, 0);//填充本行余下
          break; 
        }
        if(Count == 1){//图像结束,需填充余下
          if(h == 0) return 0;
          h --;
          pBuf = _Full0(pBuf, cw, h, h);
          return 0; 
        }
        if(Count == 2){
          //其后的两个字节表示下一个像素从当前开始的水平和垂直位置的偏移量。
          //https://docs.microsoft.com/zh-cn/windows/win32/gdi/bitmap-compression
          pBuf = _Full0(pBuf, cw, h, 0);//填充本行余下
          unsigned short offsetX = *data++; //x偏移
          unsigned short offsetY = *data++; //Y偏移
          if(offsetY){//Y偏移填充  
            pBuf = _Full0(pBuf, 0, h, offsetY);
            h -= offsetY;
          }
          if(offsetX){//x带偏移时，需向后移
            cw -= offsetX; 
            pBuf = ePic_pPlotIndexDotRepeat(pBuf, map, mapSize, 0, (w - cw));
          }
        }
        else{//其它状态表示点个数，双字节对齐，如：
          //如：00 06 45 56 67 00 由应为：4 5 5 6 6 7
          unsigned char ByteCount =  Count >> 1;
          do{
            unsigned char DotD = *data++;//一次取两个点
            pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD >> 4);//高位在前
            Count--; cw--;//第二点
            if((Count == 0) || (cw == 0)) break; //此点或此行结束了
            pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD & 0x0f);//低位在后
            Count--; cw--;//下一点
          }while((Count > 0) && (cw > 0)); 
          if(ByteCount & 0x01) data++; //双字节对齐,补齐字节丢弃  
        }
      }
      //=======================压缩数据处理=========================      
      else{//,Data0为重复次数，如：05 06 解析为：0 6 0 6 0
        unsigned char DotD = *data++;//当前点数,一次取两个点
        do{
          pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD >> 4);//高位在前
          Data0--; cw--;//第二点
          if((Data0 == 0) || (cw == 0)) break; //此点或此行结束了
          pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, DotD & 0x0f);//低位在后
          Data0--; cw--;//下一点
        }while((Data0 > 0) && (cw > 0));
      }
    }while(cw >= 0);
    //下一行了
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
  }//end for h
  return 0;
}

//-----------------------绘制BMP格式RLE8数据-----------------------
signed char ePic_PlotRle8(unsigned short x, unsigned short y)
{
  //缓冲以空间换时间 
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;   
  unsigned char mapSize = ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = ePic_pGetImageData();
  
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //显示缓冲行起始
  for(; h > 0; h--){//行为单位绘制
    signed short cw = w;
    do{//宽度一点点绘制
      volatile unsigned char Data0 = *data++; //取位数据
      //=======================数据不压缩处理======================
      if(Data0 == 0){
        unsigned char Count = *data++;
        if(Count == 0){//此行结束
          pBuf = _Full0(pBuf, cw, h, 0);//填充本行余下
          break; 
        }
        if(Count == 1){//图像结束,需填充余下
          if(h == 0) return 0;
          h --;
          pBuf = _Full0(pBuf, cw, h, h);
          return 0; 
        }
        if(Count == 2){
          //其后的两个字节表示下一个像素从当前开始的水平和垂直位置的偏移量。
          //https://docs.microsoft.com/zh-cn/windows/win32/gdi/bitmap-compression
          pBuf = _Full0(pBuf, cw, h, 0);//填充本行余下
          unsigned short offsetX = *data++; //x偏移
          unsigned short offsetY = *data++; //Y偏移
          if(offsetY){//Y偏移填充  
            pBuf = _Full0(pBuf, 0, h, offsetY);
            h -= offsetY;
          }
          if(offsetX){//x带偏移时，需向后移
            cw -= offsetX; 
            pBuf = ePic_pPlotIndexDotRepeat(pBuf, map, mapSize, 0, (w - cw));
          }
        }
        else{//其它状态表示点个数，双字节对齐，如：
          //如：00 03 45 56 67 00  解析为：45 56 67
          unsigned char ByteCount =  Count;
          do{
            pBuf = ePic_pPlotIndexDot(pBuf, map, mapSize, *data++);
            Count--; cw--;//下一点
          }while((Count > 0) && (cw > 0)); 
          if(ByteCount & 0x01) data++; //双字节对齐,补齐字节丢弃   
        }
      }
      //=======================压缩数据处理=========================      
      else{//Data0为重复次数，如：03 04  解析为：04 04 04
        pBuf = ePic_pPlotIndexDotRepeat(pBuf, map, mapSize, *data++, Data0);
        cw -= Data0;
      }
    }while(cw >= 0);
    //下一行了
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
  }//end for h
  return 0;
}



