/***************************************************************************
                       独立IO按键实现模块
***************************************************************************/

#include "LedDrv.h"
#include <string.h>

struct _LedDrv  LedDrv;

/***************************************************************************
                              相关函数实现
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void LedDrv_Init(void)
{
  LedDrv_cbCfgIo();
  memset(&LedDrv, 0, sizeof(struct _LedDrv));
  //memset(&LedDrv.Buf, 0xff, LED_DRV_LED_COUNT);  //开机预置全亮
  //LedDrv.DispEn = 0x1f;//默认低4数码管与最高位数码管
}

//--------------------------得到下个扫描位-----------------------------
//返回 >= LED_DRV_LED_COUNT表示未找到
static unsigned char _GetNextScanPos(unsigned char ScanPos)
{
  //得到当前需点亮的数码管
  unsigned char Mask = 1 << ScanPos;
  for(; ScanPos < LED_DRV_LED_COUNT; ScanPos++, Mask <<= 1){
    if(Mask & LedDrv.DispEn) break;//显示开启
  }
  return ScanPos;
}

//-------------------------------任务函数-----------------------------
//放入进程或中断中扫描,扫描率决定显示情况
void LedDrv_Task(void)
{
  //找下一位置
  unsigned char ScanPos = _GetNextScanPos(LedDrv.ScanPos + 1);
  //回环了,从头开始找(假定有开启,都能找到)
  if(ScanPos >= LED_DRV_LED_COUNT){
    if(LecDrv_cbPeriodFinal()) return; //一周期扫描完成通报
    ScanPos = _GetNextScanPos(0);
  }
  
  unsigned char Data;  //得到需送显的数据
  if(LedDrv_cbIsTest()) Data = 0xff;  //测试状态为全亮
  else Data = LedDrv.Buf[ScanPos];    //可以置段码了 
  
  //检查节能模式,当不在开启状态时,将显示置为关闭
  unsigned char Mask = 1 << ScanPos;
  if(Mask & LedDrv.LowEn){//节能模式开启时
    if(Mask & LedDrv.LowHalf){//1/2亮度时
      if(LedDrv.ScanIndex & 0x01) Data = 0;
    }
    else{ //1/4亮度时
      if(LedDrv.ScanIndex & 0x03) Data = 0;
    }
  }
 
  //置数码管时为防止中断影响不连续,临界区操作
  LedDrv_cbEnterCritical();
  LedDrv_cbClrComm(LedDrv.ScanPos);//先消隐原有显示
  LedDrv_cbSetSeg(Data); //写显示数据
  LedDrv_cbSetComm(ScanPos);//显示当前段
  LedDrv_cbExitCritical();  
  
  //为下次作准备
  LedDrv.ScanPos = ScanPos;
  LedDrv.ScanIndex++;
}

