/*******************************************************************************
          绘图库标准接口-底层操作函数接口-在常用3.5寸外置GRAM显示屏上的实现
通过此接口使Plot模块独立于度层显示屏硬件实现
********************************************************************************/

#include <string.h>
#include "Plot_cbHw.h"
#include "TFTDbi.h"
#include "TFTCmd_035.h"

/*******************************************************************************
                              外部显存实现
********************************************************************************/

//---------------------------绝对定位某个位置与区域函数-------------------------
//这里返回显示地址位置以便下次使用
//->这里设置显存操作范围，并预置写显存命令，下一步写显存,可返回NULL指针
Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
                              unsigned short w, unsigned short h)
{
  //设置x轴范围
  TftDbi_WrCmd(TFT_CMD_WR_CASET);
  TftDbi_WrDataS(x);
  TftDbi_WrDataS(x+w-1); 
  //TftDbi_CsDis();  
  //设置y轴范围
  TftDbi_WrCmd(TFT_CMD_WR_PASET);
  TftDbi_WrDataS(y);
  TftDbi_WrDataS(y+h-1); 
  //预置为连续写状态准备写入数据   
  TftDbi_WrCmd(TFT_CMD_WR_RAM);  
  return (Color_t *)NULL;
}

//----------------------------更新至下一位置--------------------------------
//读此点以到下一点后恢复写显存命令
void Plot_cbUpdateNextFun(void)
{
  //读写显示指令会将位置移至x,y轴开始,实测不可用
  //连继读写指令实测读写互不相关，故也不可行，暂法实现此功能
  
  //且此机制在本次前景改变时，上次前景会被认为是背景导致背景不能刷新，
  //故就算能实现也只能应用于固定有或无场合,或在每次在刷新背景时再使用
  
  //TftDbi_WrCmd(TFT_CMD_RD_MEM_CONT);       //读显存
  //Color_t Color =  TftDbi_RdColor();  //读1个，显存下一位
  //TftDbi_WrCmd(TFT_CMD_WR_MEM_CONT);//预置为连续写状态准备写入数据
}

//---------------------------读绝对定位某个位置与区域函数------------------------
//有读颜色需求时需实现
Color_t * Plot_cbAbsLocalAreaRd(unsigned short x, unsigned short y, 
                                unsigned short w, unsigned short h)
{
  //设置x轴范围
  TftDbi_WrCmd(TFT_CMD_WR_CASET);
  TftDbi_WrDataS(x);
  TftDbi_WrDataS(x+w-1); 
  //TftDbi_CsDis();  
  //设置y轴范围
  TftDbi_WrCmd(TFT_CMD_WR_PASET);
  TftDbi_WrDataS(y);
  TftDbi_WrDataS(y+h-1); 
  
  //预置为连续读状态准备读出数据   
  TftDbi_WrCmd(TFT_CMD_RD_RAM); 
  TftDbi_RdData(); //首个无效
  return (Color_t *)NULL;
}


                              


