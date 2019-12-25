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

//-------------------------中断任务函数------------------------------------
//放在1ms中断里实现
void KeyLight_IntTask(void)
{
  if(KeyLight.KeyPos & 0x80){//读取周期时,读后立即置指示灯
    unsigned char KeyPos = KeyLight.KeyPos & 0x7f;
    //可读取键值时，读键值(否则扫灯)
    if(KeyPos < KEY_LIGHT_K_SCAN_COUNT){
      KeyLight.Key[KeyPos] = KeyLight_cbGetKeyVol();
      KeyLight_cbSetKeyScan(-1); //关闭按描扫描
      //最后预置下一键值扫描状态
      KeyPos++;
    }
    KeyLight.KeyPos = KeyPos;
    
    //扫下一灯
    KeyLight_cbSetLightVol(KeyLight.Light[KeyLight.LightPos]);//先置灯状态
    KeyLight_cbSetLightScan(KeyLight.LightPos++);//置灯位
    if(KeyLight.LightPos >= KEY_LIGHT_L_SCAN_COUNT) KeyLight.LightPos = 0;
  }
  else{//准备读周期时
    KeyLight_cbSetLightScan(-1);            //关闭指示灯扫描
    KeyLight_cbSetKeyScan(KeyLight.KeyPos); //开启按键扫描
    KeyLight.KeyPos |= 0x80;                //置下一周期为扫描周期标志
  }
}

//-----------------------------任务函数------------------------------------
//1ms调用一次,返回0,按键未扫描完，其它扫描完
signed char KeyLight_Task(void)
{
  //扫描完了
  if((KeyLight.KeyPos & 0x7f) >= KEY_LIGHT_K_SCAN_COUNT){
    KeyLight.KeyPos = 0;//到准备周期开始扫第一个键值
    return 1; 
  }
  return 0;
}



