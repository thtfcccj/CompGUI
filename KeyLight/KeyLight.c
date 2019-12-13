/*****************************************************************************

		              指示灯与按键时的实现

******************************************************************************/

#include "KeyLight.h"
#include <string.h>

struct _KeyLight KeyLight; //直接单例化


/*****************************************************************************
                            相关函数实现
******************************************************************************/

//-------------------------初始化函数---------------------------------------
void KeyLight_Init(void)
{
  memset(&KeyLight, 0, sizeof(struct _KeyLight));
  KeyLight_cbInitIo();
}

//-----------------------------任务函数------------------------------------
//1ms调用一次,返回负：准备状态，正：下次扫描按键位置
signed char KeyLight_Task(void)
{
  if(KeyLight.KeyPos & 0x80){//读取周期时
    //首先读取键值
    unsigned char KeyPos = KeyLight.KeyPos & 0x7f;
    KeyLight.Key[KeyPos] = KeyLight_cbGetKeyVol();
    //然后置指示灯
    KeyLight_cbSetKeyScan(-1); //关闭按描扫描
    KeyLight_cbSetLightScan(KeyLight.LightPos++);
    if(KeyLight.LightPos >= KEY_LIGHT_L_SCAN_COUNT) KeyLight.LightPos = 0;
    //最后预置下一键值扫描状态
    KeyPos++;
    if(KeyPos >= KEY_LIGHT_L_SCAN_COUNT) KeyPos = 0;
    KeyLight.KeyPos = KeyPos;
    return KeyPos; //有键值了
  }
  else{//准备读周期时
    KeyLight_cbSetLightScan(-1);            //关闭指示灯扫描
    KeyLight_cbSetKeyScan(KeyLight.KeyPos); //开启按键扫描
    KeyLight.KeyPos |= 0x80;                //置下一周期为扫描周期标志
    return -1;// - KeyLight.LightPos;
  }
}


