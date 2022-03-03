/*****************************************************************************

		              扫描时置指示灯与获得按键-在EO8(如595)上的实现

******************************************************************************/

#include "KeyLight.h"
#include <string.h>
#include "EO8.h"

struct _KeyLight KeyLight; //直接单例化

//定义595上，指示灯与按键的扫描掩码    
static const unsigned char _ScanLut[] = KeyLight_cbScanLut(); 

/*****************************************************************************
                            相关函数实现
******************************************************************************/

//-------------------------初始化函数---------------------------------------
void KeyLight_Init(void)
{
  memset(&KeyLight, 0, sizeof(struct _KeyLight));
  KeyLight_cbInitIo();  
  EO8_Init();
}

//-------------------------中断任务函数------------------------------------
//放在1~10ms中断里实现
void KeyLight_IntTask(void)
{
  //先读取键值
  unsigned char ScanPos = KeyLight.ScanPos;
  KeyLight.Key[ScanPos] = KeyLight_cbGetKeyVol();
  //下一行
  ScanPos++; 
  if(KeyLight.ScanPos >= KEY_LIGHT_SCAN_COUNT) ScanPos = 0;
  KeyLight.ScanPos = ScanPos;
  //获得需更新的数据
  unsigned char Data = KeyLight.Light[ScanPos]; //指示灯列
  Data |= _ScanLut[ScanPos] | KeyLight_cbGetFixBit(); //扫描位及固定位更新
  //一起更新了
  EO8_SetAllFast(Data);
}




