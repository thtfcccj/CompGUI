/*****************************************************************************

//		        按键模块回调-在每个按键为独立IO时的实现

******************************************************************************/

#include "Key.h"

//---------------------------------得到原始键值函数-----------------------------
#ifdef SUPPORT_KEY_ID
unsigned char Key_cbGetOrgKeyId(void)
{
  if(!IsKey1()) return 0;
  #if KEY_COUNT >= 2
  if(!IsKey2()) return 1;
  #endif
  #if KEY_COUNT >= 3
  if(!IsKey3()) return 2;
  #endif
  #if KEY_COUNT >= 4
  if(!IsKey4()) return 3;
  #endif
  #if KEY_COUNT >= 5
  if(!IsKey5()) return 4;
  #endif
  #if KEY_COUNT >= 6
  if(!IsKey6()) return 5;
  #endif
  #if KEY_COUNT >= 7
  if(!IsKey7()) return 6;
  #endif
  #if KEY_COUNT >= 8
  if(!IsKey8()) return 7;
  #endif
  #if KEY_COUNT >= 9
  if(!IsKey9()) return 8;
  #endif
  #if KEY_COUNT >= 10
  if(!IsKey9()) return 9;
  #endif
  #if KEY_COUNT >= 11
  if(!IsKey9()) return 10;
  #endif
  #if KEY_COUNT >= 12
  if(!IsKey9()) return 11;
  #endif
  return 0xff;
}
#endif //#ifdef SUPPORT_KEY_ID

//-----------------------------判断对应键是否按下-----------------------------
#ifdef SUPPORT_KEY_MASK
unsigned char Key_cbIsKeySel(unsigned char KeySel)
{
  switch(KeySel){
    case 0: return !IsKey1();
    #if KEY_COUNT >= 2
    case 1: return !IsKey2();
    #endif
    #if KEY_COUNT >= 3
    case 2: return !IsKey3();  
    #endif
    #if KEY_COUNT >= 4
    case 3: return !IsKey4(); 
    #endif       
    #if KEY_COUNT >= 5
    case 4: return !IsKey5();
    #endif
    #if KEY_COUNT >= 6
    case 5: return !IsKey6();  
    #endif
    #if KEY_COUNT >= 7
    case 6: return !IsKey7(); 
    #endif
    #if KEY_COUNT >= 8
    case 7: return !IsKey8();
    #endif
  }
  return 0;
}
#endif //#ifdef SUPPORT_KEY_MASK



