/***********************************************************************

		                    �����з�ʽ��ʾ��־
��ģ����Ҫ������BOOTʱ�����з�ʽ����ʾ������ʾ��Ϣ��
***********************************************************************/
#include "LogCmdLine.h"

struct _LogCmdLine LogCmdLine;          //ֱ��ʵ����

//��������ַ�
static const char _CurcorOff[] = {" "};
static const char _CurcorOn[] = {"|"};

/***********************************************************************
		                      ��غ���ʵ��
***********************************************************************/

//----------------------��ͨ������-------------------------------
//����512ms������ɨ����ʵ������
void LogCmdLine_Task(void)
{
  if(LogCmdLine.Index & 0x01) //��ʾ����
    LogCmdLine_cbUpdateLine(LogCmdLine.Line, _CurcorOn);
  else LogCmdLine_cbUpdateLine(LogCmdLine.Line, _CurcorOff); 
  LogCmdLine.Index++;
}

//---------------------------������־------------------------------
void LogCmdLine_UpdateLog(const char *pStr)
{
  LogCmdLine_cbUpdateLine(LogCmdLine.Line, pStr); //���µ�ǰ����ʾ
  LogCmdLine.Line++;
  if(LogCmdLine.Line >= LOG_CMD_LINE_COUNT) //�ػ���
    LogCmdLine.Line = LOG_CMD_LINE_NEXT_START;
  //�����һ����ʾ��Ϊ���������ַ�����׼��
  LogCmdLine_cbClrLine(LogCmdLine.Line);
}




