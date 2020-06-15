/*******************************************************************************

                    TFT DBI操作统一接口-组合操作函数实现

********************************************************************************/
#include "TftDbi.h"

struct _TftDbi TftDbi; //直接单例化

//---------------------------------写U16数据函数-------------------------------
//先高后低顺序
void TftDbi_WrDataS(unsigned short Data)
{
  TftDbi_WrData(Data >> 8);
  TftDbi_WrData(Data & 0xff);
}

//----------------------------------读U16数据函数-------------------------------
//先高后低顺序
unsigned short TftDbi_RdDataS(void)
{
  unsigned char Data;
  Data = TftDbi_RdData();
  return TftDbi_RdData() | ((unsigned short)Data << 8);
}

//---------------------------写指令-带1Byte参数函数-----------------------------
//指令固定为1个Byte
void TftDbi_WrCmdB1(unsigned char Cmd, unsigned char Para)
{
  TftDbi_WrCmd(Cmd);
  TftDbi_WrData(Para);
}

//---------------------------写指令-带1Short参数函数----------------------------
//参数先高后低顺序
void TftDbi_WrCmdS1(unsigned char Cmd, unsigned short Para)
{
  TftDbi_WrCmd(Cmd);
  TftDbi_WrData(Para >> 8);
  TftDbi_WrData(Para & 0xff);
}



