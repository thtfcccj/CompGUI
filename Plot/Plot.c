/*******************************************************************************
                         绘图库标准接口
此接口依赖于TftDrv提供的帧缓冲
为适用不同显示屏，定义显示屏左上角为0点，各函数遵循从左到右(写完)，从上到下写原则。
********************************************************************************/

#include "Plot.h"
#include "Plot_cbHw.h"//底层操作函数
#include <string.h>

/*******************************************************************************
                             调色板相关
*******************************************************************************/
struct _PlotPalette PlotPalette; //仅支持单线程调用

//-------------------------------配置调色板---------------------------------
//penStyle置0
void Plot_CfgPalette(Color_t penColor, Color_t brushColor, u8 brushStyle)
{
  PlotPalette.penColor = penColor;
  PlotPalette.brushColor = brushColor;
  PlotPalette.brushStyle = brushStyle;  
}

/*******************************************************************************
                             字填充相关
*******************************************************************************/
#include "GB2312ZM.h"

//---------------------------绘制单个GB2312汉字字符-----------------------------
//横向取模16*16点位
void Plot_GB2312(u16 x, u16 y, u16 code)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 16, 16);  
  uc8 *mask = GB2312ZM_pGetHZ(code);
  uc8 *endmask = mask + 32; //字模结束位置
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度

  for( ;mask < endmask; ){  
    u16 ZM = *mask++; //横向左->指向横向右
    ZM <<= 8;
    ZM |= *mask++;   //横向右->指向下行横向左
    //填充本行16点颜色
    for(u16 Mask = 0x8000; Mask > 0; Mask >>= 1){
      if(ZM & Mask) Plot_cbSetCurColor(pBuf, color);
      else if(PlotPalette.brushStyle) 
        Plot_cbSetCurColor(pBuf, PlotPalette.brushColor);
      else Plot_cbUpdateNext(pBuf);//透明时不更新颜色
    }
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 16);//下一行填充位置起始
  }  
}
  
//----------------------绘制单个GB2312汉字字符->双倍大小------------------------
//横向取模32*32点位
void Plot_GB2312_Scale2(u16 x,u16 y,u16 code)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 32, 32);  
  uc8 *mask = GB2312ZM_pGetHZ(code);
  uc8 *endmask = mask + 32; //字模结束位置
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for( ;mask < endmask; ){  
    u16 ZM = *mask++; //横向左->指向横向右
    ZM <<= 8;
    ZM |= *mask++;   //横向右->指向下行横向左
    
    //填充两行32点颜色
    for(u8 dLine = 0; dLine < 2; dLine++){//行数
      for(u16 Mask = 0x8000; Mask > 0; Mask >>= 1){
        Color_t fullColor;
        if(ZM & Mask) fullColor = color;
        else if(PlotPalette.brushStyle) fullColor = PlotPalette.brushColor;
        else{//透明时不更新颜色
          Plot_cbUpdateNext(pBuf);
          Plot_cbUpdateNext(pBuf);
          continue;
        }
        //x轴连续填充两次
        Plot_cbSetCurColor(pBuf, fullColor);
        Plot_cbSetCurColor(pBuf, fullColor);
      }
      pBuf = Plot_cbToNextRowStart(pBuf, (TFT_DRV_H_PIXEl - 32));//到下行32点
    }
  }
}

//----------------------绘制单个Asc字符-----------------------------------
//横向取模8*16点位
void Plot_Asc(u16 x,u16 y, u8 code)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 8, 16);  
  uc8 *mask = GB2312ZM_pGetAsc(code);
  uc8 *endmask = mask + 16; //字模结束位置
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //本行点阵->指向下行
    //填充本行8点颜色
    for(u8 Mask = 0x80; Mask > 0; Mask >>= 1){
      if(ZM & Mask) Plot_cbSetCurColor(pBuf, color);
      else if(PlotPalette.brushStyle) 
        Plot_cbSetCurColor(pBuf, PlotPalette.brushColor);
      else Plot_cbUpdateNext(pBuf);//透明时不更新颜色
    }  
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 8); //下一行填充位置起始
  }
}

//----------------------绘制单个Asc字符->8*8点阵-------------------------------
//横向取模8*8点位
void Plot_Asc8(u16 x,u16 y,u8 code)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 8, 8);  
  uc8 *mask = GB2312ZM_pGetAsc8(code);
  uc8 *endmask = mask + 8; //字模结束位置
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //本行点阵->指向下行
    //填充本行8点颜色
    for(u8 Mask = 0x80; Mask > 0; Mask >>= 1){
      if(ZM & Mask) Plot_cbSetCurColor(pBuf, color);
      else if(PlotPalette.brushStyle) 
        Plot_cbSetCurColor(pBuf, PlotPalette.brushColor);
      else Plot_cbUpdateNext(pBuf);//透明时不更新颜色
    }  
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 8); //下一行填充位置起始
  }
}

//----------------------绘制单个Asc字符->双倍大小------------------------
//横向取模16*32点位
void Plot_Asc_Scale2(u16 x,u16 y,u8 code)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 16, 32);  
  uc8 *mask = GB2312ZM_pGetAsc(code);
  uc8 *endmask = mask + 16; //字模结束位置
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for( ;mask < endmask; ){  
    u8 ZM = *mask++; //本行点阵->指向下行
    //填充两行16点颜色
    for(u8 dLine = 0; dLine < 2; dLine++){//行数
      for(u8 Mask = 0x80; Mask > 0; Mask >>= 1){
        Color_t fullColor;
        if(ZM & Mask) fullColor = color;
        else if(PlotPalette.brushStyle) fullColor = PlotPalette.brushColor;
        else{//透明时不更新颜色
          Plot_cbUpdateNext(pBuf);
          Plot_cbUpdateNext(pBuf);
          continue;
        }
        //x轴连续填充两次
        Plot_cbSetCurColor(pBuf, fullColor);
        Plot_cbSetCurColor(pBuf, fullColor); 
      }
      pBuf = Plot_cbToNextRowStart(pBuf, (TFT_DRV_H_PIXEl - 16));//到下行16点
    }
  }
}

//------------------------绘制字符串->支持GB2312--------------------------
void Plot_String(u16 x,u16 y, cc8* ptr,u8 length)//长度为0时为用strlen代替
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    if(c < 0x80){//ASCII直接转换
      Plot_Asc(x, y, c);
      x += 8;
    }
    else{//未GB2312检查正确性！！！！
      ptr++; //取下半个字
      Plot_GB2312(x,y,((u16)c << 8) | *ptr); //高位在前
      x += 16;
    }
  }
}

//---------------------------绘制asc8*8字符串------------------------------
void Plot_StrAsc8(u16 x,u16 y,cc8* ptr,u8 length)//长度为0时为用strlen代替
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    if(c < 0x80){//ASCII直接转换
      Plot_Asc8(x, y, c);
      x += 8;
    }
    //else{}//不支持,忽略
  }
}

//----------------------绘制字符串->->双倍大小，支持GB2312-----------------------
void Plot_String_Scale2(u16 x,u16 y, cc8* ptr,u8 length)//长度为0时为用strlen代替
{
  if(length == 0) length = strlen(ptr);
  cc8 *end = ptr + length;
  for( ;ptr < end; ptr++){
    u8 c = *ptr;
    if(c < 0x80){//ASCII直接转换
      Plot_Asc_Scale2(x, y, c);
      x += 16;
    }
    else{//未GB2312检查正确性！！！！
      ptr++; //取下半个字
      Plot_GB2312_Scale2(x,y,((u16)c << 8) | *ptr); //高位在前
      x += 32;
    }
  }
}

/*******************************************************************************
                              标准图形库
********************************************************************************/

//------------------------------------清屏-------------------------------------
void Plot_Clear(void)
{
  Plot_FullRect(0, 0, TFT_DRV_H_PIXEl, TFT_DRV_V_PIXEl);
}

//----------------------------绘制单像素水平直线---------------------------------
void Plot_LineH(u16 x,u16 y,u16 length)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, length, 1); 
  Color_t *pEndBuf = pBuf + length; 
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for(; pBuf < pEndBuf;) Plot_cbSetCurColor(pBuf, color);
}

//----------------------------绘制单像素垂直直线---------------------------------
void Plot_LineV(u16 x,u16 y,u16 high)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, 1, high); 
  Color_t *pEndBuf = pBuf + high * TFT_DRV_H_PIXEl;  
  Color_t color = PlotPalette.penColor; //缓冲至寄存器，以加速度
  
  for(; pBuf < pEndBuf;){
    Plot_cbSetCurColor(pBuf, color);
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - 1); //下一行
  }
}

//----------------------------绘制单像素直线---------------------------------
//原DrawLine
void Plot_Line(u16 x,u16 y,u16 length,
               u8 direction) //0:横向，非0:纵向
{
  if(direction) Plot_LineV(x, y, length);
  else Plot_LineH(x, y, length);
}

//----------------------------填充矩形框-------------------------------------
//画刷颜色，即背景色
void Plot_FullRect(u16 x,u16 y,u16 w,u16 h)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h); 
  Color_t color = PlotPalette.brushColor; //缓冲至寄存器，以加速度
  
  for(; h > 0; h--){
    Color_t *pEndBuf = pBuf + w; 
    for(; pBuf < pEndBuf;) Plot_cbSetCurColor(pBuf, color);

    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
  }
}

//-----------------------------画矩形----------------------------------------
void Plot_Rect(u16 x,u16 y,u16 w,u16 h)
{
  Plot_LineH(x, y,    w);        //上横
  Plot_LineH(x, y + h, w);       //下横
  Plot_LineV(x,    y, h);        //左竖
  Plot_LineV(x+ w, y, h);        //右竖  
}

//-----------------------------填充索引位图辅助---------------------------------
//由调色板大小得到占位
static unsigned char _GetBitSize(u8 mapSize)
{
  if(mapSize <=  2) return 1;
  if(mapSize <=  4) return 2;
  if(mapSize <=  8) return 3;
  if(mapSize <=  16) return 4;
  if(mapSize <=  32) return 5;
  if(mapSize <=  64) return 6;
  if(mapSize <=  128) return 7;
  return 8;
}

//由占位得到掩码
static const u8 _BitSize2Mask[9] = {
  0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

//-----------------------------画索引位图-------------------------------------
//横向取模w*h点位,此函数不依赖调色板
void Plot_IndexBmp(u16 x,u16 y,u16 w,u16 h,
                   uc8 *data,      //数据矩阵,高位在前
                   u8 mapSize,     //调色板大小
                   Color_t *map)  //调色板查找表,查找表结果为0时表示透明
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h);     //显示缓冲行起始
  
  u8 bitSize = _GetBitSize(mapSize);     //占位
  u32 dataBitSize = w * h * bitSize;     //以位计算的数据矩阵大小  
  u8 bitMask = _BitSize2Mask[bitSize];   //位掩码

  Color_t *pLineEndBuf = pBuf + w;       //显示缓冲本行结束
  u8 Data = *data; //读取首个数据
  for(u32 dataBit = 0; dataBit < dataBitSize; dataBit += bitSize){//索引点位置移动
    //1. 得到索引色
    u8 curShift = dataBit & 0x07;                   //当前索引点在Data中的起始Bit
    u8 indexColor; //索引色
    //indexColor = (Data >> curShift) & bitMask; //低位在前时先取出，可能不全
    //下个索引点在Data中的起始Bit
    u8 nextShift = (dataBit + bitSize) & 0x07;      
    if(nextShift <= curShift){//下个索引点回环了，即在下个data里
      indexColor = Data & _BitSize2Mask[8 - curShift];//高位在前时取高位数据
      data++; //移至下个数据
      Data = *data; //更新至下个数据
      if(nextShift){//下个索引点不是从0开始的，即本次不全，需取出本次的高位数据
        //indexColor |= (Data & _BitSize2Mask[nextShift]) << curShift;//低位在前时
        indexColor <<= nextShift; //高位在前时,高位移前到前面
        indexColor |= (Data >> (8 - nextShift)) & _BitSize2Mask[nextShift];
      }
    }
    else{//高位在前时够了，直接取数
      indexColor = (Data >> ((8 - bitSize) - curShift)) & bitMask;
    }
    
    //2. 填充色
    Color_t color = *(map + indexColor);
    if(color) Plot_cbSetCurColor(pBuf, color); //非透明时填充并移至下一点
    else Plot_cbUpdateNext(pBuf);
    
    //3. 下一点判断
    if(pBuf >= pLineEndBuf){//一行结束了
      pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
      pLineEndBuf += TFT_DRV_H_PIXEl; //下行结束
    }
  }
}

//-----------------------------画单色位图-------------------------------------
//横向取模w*h点位,此函数不依赖外部调色板
//para为位定义：0x01:透明 0x02 反色显示
void Plot_Bmp1(u16 x,u16 y,u16 w,u16 h, uc8 *data, u8 para)
{
  Color_t map[2];//调色板查找表  
  if(para & 0x01){//透明时
    map[0] = 0;//背景色设为0时表示透明
    map[1] = PlotPalette.penColor;  //用前景色
  }
  else{//不透明时
    map[0] = PlotPalette.penColor;  //用前景色
    map[1] = PlotPalette.brushColor;//用背景色
  }
  
  if(para & 0x02){ //反色时交换
    Color_t s = map[0];
    map[0] = map[1];
    map[1] = s;
  }
  Plot_IndexBmp(x,y,w,h, data, 2, map);
}

//--------------------将对应区域的前景色替换为指定色----------------------------
//此函数只支持内置显存
void Plot_ReplacePenColor(u16 x,u16 y,u16 w,u16 h, Color_t newColor)
{
  Color_t *pBuf = Plot_cbAbsLocalArea(x,y, w, h); 
  Color_t color = PlotPalette.brushColor; //缓冲至寄存器，以加速度
  
  for(; h > 0; h--){
    Color_t *pEndBuf = pBuf + w; 
    for(; pBuf < pEndBuf;){
      if(*pBuf != color) Plot_cbSetCurColor(pBuf, color);//*pBuf只能操作内置显存！！
    }
    pBuf = Plot_cbToNextRowStart(pBuf, TFT_DRV_H_PIXEl - w); //下一行
  }
}



