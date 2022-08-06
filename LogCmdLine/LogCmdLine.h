/***********************************************************************

		                    �����з�ʽ��ʾ��־
��ģ����Ҫ������BOOTʱ�����з�ʽ����ʾ������ʾ��Ϣ��
***********************************************************************/
#ifndef _LOG_CMD_LINE_H
#define	_LOG_CMD_LINE_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/***********************************************************************
		                      �������
***********************************************************************/

//��������ʹ�õĸ߶�
#ifndef LOG_CMD_FONT_H
  #define LOG_CMD_FONT_H 7
#endif

//��������ʹ�õĿ��
#ifndef LOG_CMD_FONT_W
  #define LOG_CMD_FONT_W 5
#endif

//������ʾ���ܹ���ʾ������
#ifndef LOG_CMD_LINE_COUNT
  #define LOG_CMD_LINE_COUNT    (320 / LOG_CMD_FONT_H)
#endif

//������ʾ��ˮƽ������
#ifndef LOG_CMD_H_PIXEl
  #define LOG_CMD_H_PIXEl    (480)
#endif

//��һ�����У���������������������(������ LogCmdLine_cbUpdateLine() ֱ�ӿ���)
#ifndef LOG_CMD_LINE_NEXT_START
  #define LOG_CMD_LINE_NEXT_START  1
#endif

/***********************************************************************
		                      ��ؽṹ
***********************************************************************/

struct _LogCmdLine{
  unsigned char Line;        //��ǰ������
  unsigned char Index;      //��ʱ��
};

extern struct _LogCmdLine LogCmdLine;          //ֱ��ʵ����

/***********************************************************************
		                      ��غ���
***********************************************************************/

//-----------------------��ʼ������-------------------------------
//void LogCmdLine_Init(void);
#define LogCmdLine_Init() do{LogCmdLine.Line = LOG_CMD_LINE_NEXT_START;}while(0)

//----------------------��ͨ������-------------------------------
//����512ms������ɨ����ʵ������
void LogCmdLine_Task(void);

//---------------------------������־------------------------------
void LogCmdLine_UpdateLog(const char *pStr);

/***********************************************************************
		                     �ص�����
***********************************************************************/

//---------------------------���һ����ʾ------------------------------
void LogCmdLine_cbClrLine(unsigned char CurLine);

//--------------------------���ַ�����������ʾ��------------------------
void LogCmdLine_cbUpdateLine(unsigned char CurLine,
                             const char *pStr);

#endif
