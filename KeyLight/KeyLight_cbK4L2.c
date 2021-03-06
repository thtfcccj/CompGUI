/*****************************************************************************

		              KeyLight回调函数-在(IO实现)4键扫2灯扫中的实现

******************************************************************************/

#include "KeyLight.h"
#if KEY_LIGHT_L_SCAN_COUNT == 2

/*****************************************************************************
                            回调函数接口实现
******************************************************************************/

//-----------------------------置按键扫描位置-------------------------------
//< 0表示关闭扫描，否则还需同时将读按键IO置为输入
void KeyLight_cbSetKeyScan(signed char Pos)
{
  //先关闭扫描所有
  SetKeySo();
  if(Pos < 0) return; ////关闭扫描
  //扫描对应位置
  switch(Pos){
    case 0: ClrKeySo0(); break;
    case 1: ClrKeySo1(); break;
    case 2: ClrKeySo2(); break;
    case 3: ClrKeySo3(); break;
  }
  InKlIo(); //置复用IO为输入
}

//----------------------------置扫描灯扫描位置-------------------------------
//< 0表示关闭扫描，需同时将写指示灯IO置为输出
void KeyLight_cbSetLightScan(signed char Pos)
{
  //先关闭扫描所有
  SetLightEn();
  if(Pos < 0) return; //关闭扫描
  //扫描对应位置
  switch(Pos){
    case 0: ClrLightEn0(); break;
    case 1: ClrLightEn1(); break;
  }
  OutKlIo(); //置复用IO为输出
}

#endif //KEY_LIGHT_L_SCAN_COUNT == 1

