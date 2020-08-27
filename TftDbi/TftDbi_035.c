/*******************************************************************************

                     TFT DBI操作统一接口-035显示屏相关函数实现
此接口与具体使用显示屏控制器无关
********************************************************************************/
#include "TftDbi.h"
#include "TftCmd_035.h"

/*******************************************************************************
                                  相关函数-普通函数实现
********************************************************************************/
//---------------------------------初始化函数-----------------------------------
//返回负设备有问题
signed char TftDbi_Init(void)
{
  TftDbi.Timer = 0;
  TftDbi_HwInit();
  
  //读软件识别信息,回4个参数: 1无效 2工厂ID 3驱动版本 4驱动ID
  unsigned char Id = TftDbi_RdDataCmd(TFT_CMD_RD_DIDIF);
  unsigned short Info = TftDbi_RdDataS();

  if((Id == 0) || (Id == 0xff)) return -1; //设备不对
  if((Info == 0) || (Info == 0xffff)) return -2; //设备信息不对
  
  //035显示屏初始化
  //可参考: https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html  
  TftDbi_WrCmd(TFT_CMD_WR_DISPON); //开启显示
  TftDbi_WrCmd(TFT_CMD_WR_ALLPON); //全白显示测试
  TftDbi_WrCmd(TFT_CMD_WR_ALLPOFF); //全黑显示测试   

  return 0;
}

//---------------------------------任务函数-----------------------------------
//放16ms进程中扫描
void TftDbi_Task(void)
{
  if(!TftDbi.Timer) return;
  TftDbi.Timer--;
  if(TftDbi.Timer) return;   
  //内存读写时，强制封闭指令
  if((TftDbi.LastCmd == TFT_CMD_RD_RAM) || (TftDbi.LastCmd == TFT_CMD_WR_RAM)) { 
    TftDbi_WrCmd(TFT_CMD_WR_NOP);
    TftDbi_CsDis();
  }
}


