/*******************************************************************************

                    TFT DBI����ͳһ�ӿ�-��ϲ�������ʵ��

********************************************************************************/
#include "TftDbi.h"

struct _TftDbi TftDbi; //ֱ�ӵ�����

//---------------------------------дU16���ݺ���-------------------------------
//�ȸߺ��˳��
void TftDbi_WrDataS(unsigned short Data)
{
  TftDbi_WrData(Data >> 8);
  TftDbi_WrData(Data & 0xff);
}

//----------------------------------��U16���ݺ���-------------------------------
//�ȸߺ��˳��
unsigned short TftDbi_RdDataS(void)
{
  unsigned char Data;
  Data = TftDbi_RdData();
  return TftDbi_RdData() | ((unsigned short)Data << 8);
}

//---------------------------дָ��-��1Byte��������-----------------------------
//ָ��̶�Ϊ1��Byte
void TftDbi_WrCmdB1(unsigned char Cmd, unsigned char Para)
{
  TftDbi_WrCmd(Cmd);
  TftDbi_WrData(Para);
}

//---------------------------дָ��-��1Short��������----------------------------
//�����ȸߺ��˳��
void TftDbi_WrCmdS1(unsigned char Cmd, unsigned short Para)
{
  TftDbi_WrCmd(Cmd);
  TftDbi_WrData(Para >> 8);
  TftDbi_WrData(Para & 0xff);
}



