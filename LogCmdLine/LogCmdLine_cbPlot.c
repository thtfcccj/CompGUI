/*******************************************************************************

           命令行方式显示日志回调函数-在使用Plot模块时的实现

*******************************************************************************/
#include "LogCmdLine.h"
#include "Plot.h"

//---------------------------清除一行显示------------------------------
void LogCmdLine_cbClrLine(unsigned char CurLine)
{
  Plot_FullRect(0, CurLine * LOG_CMD_FONT_H, LOG_CMD_H_PIXEl, LOG_CMD_FONT_H);
}

//--------------------------将字符串更新至显示屏------------------------
void LogCmdLine_cbUpdateLine(unsigned char CurLine,
                             const char *pStr)
{
  #if (LOG_CMD_FONT_H == 8)
    Plot_StrAsc8(0, CurLine * LOG_CMD_FONT_H, pStr, 0);
  #else //使用双倍高字体
    #if (LOG_CMD_FONT_W == 5)
      Plot_StrAsc57_Scale2(0, CurLine * LOG_CMD_FONT_H, pStr, 0);
    #else
      Plot_StrAsc8_Scale2(0, CurLine * LOG_CMD_FONT_H, pStr, 0);
    #endif
  #endif
}













