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
    LogCmdLine_cbUpdateLine(LogCmdLine.Pos, _CurcorOn);
  else LogCmdLine_cbUpdateLine(LogCmdLine.Pos, _CurcorOff); 
  LogCmdLine.Index++;
}

//---------------------------������־------------------------------
void LogCmdLine_UpdateLog(const char *pStr)
{
  LogCmdLine_cbUpdateLine(LogCmdLine.Pos, pStr); //���µ�ǰ����ʾ
  LogCmdLine.Pos++;
  if(LogCmdLine.Pos >= LOG_CMD_LINE_HIGHT) //�ػ���
    LogCmdLine.Pos = LOG_CMD_LINE_NEXT_START;
  //�����һ����ʾ��Ϊ���������ַ�����׼��
  LogCmdLine_cbClrLine(LogCmdLine.Pos);
}




