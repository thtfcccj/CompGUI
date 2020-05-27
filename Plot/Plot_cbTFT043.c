/*******************************************************************************
              绘图库标准接口-底层操作函数接口-在常用4.3寸显示屏上的实现
通过此接口使Plot模块独立于度层显示屏硬件实现
********************************************************************************/

#include <string.h>
#include "Plot_cbHw.h"
#include "TFTDbi.h"
#include "TFTCmd_043.h"


/*******************************************************************************
                              外部显存实现
********************************************************************************/

//---------------------------绝对定位某个位置与区域函数-------------------------
//这里返回显示地址位置以便下次使用
//->这里设置显存操作范围，并预置写显存命令，下一步写显存,可返回NULL指针
Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
                              unsigned short w, unsigned short h)
{
  //设置y轴范围
  TftDbi_WrCmd(TFT_CMD_WR_CASET);
  TftDbi_WrDataS(y);
  TftDbi_WrDataS(h); 
  //设置x辆范围
  TftDbi_WrCmd(TFT_CMD_WR_PASET);
  TftDbi_WrDataS(x);
  TftDbi_WrDataS(w); 
  //预置为连续写状态准备写入数据
  TftDbi_WrCmd(TFT_CMD_WR_RAM);  
  return (Color_t *)NULL;
}

//----------------------------更新至下一位置--------------------------------
//读此点以到下一点后恢复写显存命令
void Plot_cbUpdateNextFun(void)
{
  TftDbi_WrCmd(TFT_CMD_RD_RAM);       //读显存
  Color_t Color =  TftDbi_RdColor();  //读1个，显存下一位
  TftDbi_WrCmd(TFT_CMD_WR_RAM);//预置为连续写状态准备写入数据
}





