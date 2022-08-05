/***********************************************************************

		                    命令行方式显示日志
本模块主要用于如BOOT时，以行方式在显示屏上显示信息。
***********************************************************************/
#ifndef _LOG_CMD_LINE_H
#define	_LOG_CMD_LINE_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***********************************************************************
		                      相关配置
***********************************************************************/

//定义显示屏高度
#ifndef LOG_CMD_LINE_HIGHT
  #define LOG_CMD_LINE_HIGHT    (320 / 8)
#endif

//下一行首行，可用于留顶部不变区域(此区域 LogCmdLine_cbUpdateLine() 直接控制)
#ifndef LOG_CMD_LINE_NEXT_START
  #define LOG_CMD_LINE_NEXT_START  1
#endif

/***********************************************************************
		                      相关结构
***********************************************************************/

struct _LogCmdLine{
  unsigned char Pos;        //当前所处位置
  unsigned char Index;      //
};

extern struct _LogCmdLine LogCmdLine;          //直接实例化

/***********************************************************************
		                      相关函数
***********************************************************************/

//-----------------------初始化函数-------------------------------
//void LogCmdLine_Init(void);
#define LogCmdLine_Init() do{LogCmdLine.Pos = LOG_CMD_LINE_NEXT_START;}while(0)

//----------------------普通任务函数-------------------------------
//放入512ms进程中扫描以实现闪动
void LogCmdLine_Task(void);

//---------------------------更新日志------------------------------
void LogCmdLine_UpdateLog(const char *pStr);

/***********************************************************************
		                     回调函数
***********************************************************************/

//---------------------------清除一行显示------------------------------
void LogCmdLine_cbClrLine(unsigned char CurLine);

//--------------------------将字符串更新至显示屏------------------------
void LogCmdLine_cbUpdateLine(unsigned char CurLine,
                             const char *pStr);

#endif
