/***************************************************************************
                       数码管显示驱动程序,回调函数实现
//独立于硬件
***************************************************************************/
#include "LedDrv.h"
#include "IOCtrl.h"

//-----------------------------写Com口--------------------------
//即将该Com置为显示
void LedDrv_cbSetComm(unsigned char Ch)
{
  /*)switch(Ch){
  case 0: ClrCom0(); break;
  case 1: ClrCom1(); break;
  case 2: ClrCom2(); break; 
  case 3: ClrCom3(); break; 
  case 4: ClrCom4(); break; 
  case 5: ClrCom5(); break;   
  default: ClrCom6(); break;
  }*/
  //PIC的省代码方式:
  if(Ch == 0) ClrCom0();
  else if(Ch == 1) ClrCom1();
  else if(Ch == 2) ClrCom2();
  else if(Ch == 3) ClrCom3();
  else if(Ch == 4) ClrCom4();
  else if(Ch == 5) ClrCom5();
  else ClrCom6();
}

//-----------------------------清Com口--------------------------
//即将该Com置为消隐
void LedDrv_cbClrComm(unsigned char Ch)
{
  /*switch(Ch){
  case 0: SetCom0(); break;
  case 1: SetCom1(); break;
  case 2: SetCom2(); break; 
  case 3: SetCom3(); break; 
  case 4: SetCom4(); break; 
  case 5: SetCom5(); break;   
  default: SetCom6(); break;   
  }*/
  //PIC的省代码方式:
  if(Ch == 0) SetCom0();
  else if(Ch == 1) SetCom1();
  else if(Ch == 2) SetCom2();
  else if(Ch == 3) SetCom3();
  else if(Ch == 4) SetCom4();
  else if(Ch == 5) SetCom5();
  else SetCom6();
}

