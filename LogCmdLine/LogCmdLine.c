/***********************************************************************

		                    命令行方式显示日志
本模块主要用于如BOOT时，以行方式在显示屏上显示信息。
***********************************************************************/
#include "LogCmdLine.h"

struct _LogCmdLine LogCmdLine;          //直接实例化

//光标闪动字符
static const char _CurcorOff[] = {" "};
static const char _CurcorOn[] = {"|"};

/***********************************************************************
		                      相关函数实现
***********************************************************************/

//----------------------普通任务函数-------------------------------
//放入512ms进程中扫描以实现闪动
void LogCmdLine_Task(void)
{
  if(LogCmdLine.Index & 0x01) //显示周期
    LogCmdLine_cbUpdateLine(LogCmdLine.Line, _CurcorOn);
  else LogCmdLine_cbUpdateLine(LogCmdLine.Line, _CurcorOff); 
  LogCmdLine.Index++;
}

//---------------------------更新日志------------------------------
void LogCmdLine_UpdateLog(const char *pStr)
{
  LogCmdLine_cbUpdateLine(LogCmdLine.Line, pStr); //更新当前行显示
  LogCmdLine.Line++;
  if(LogCmdLine.Line >= LOG_CMD_LINE_COUNT) //回环了
    LogCmdLine.Line = LOG_CMD_LINE_NEXT_START;
  //清除下一行显示以为闪动及送字符做好准备
  LogCmdLine_cbClrLine(LogCmdLine.Line);
}




