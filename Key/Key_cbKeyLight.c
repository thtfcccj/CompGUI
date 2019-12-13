/*****************************************************************************

		              按键模块回调-在使用KeyLight模块时的实现

******************************************************************************/

#include "Key.h"
#include "KeyLight.h"

//---------------------------------得到原始键值函数-----------------------------
#ifdef SUPPORT_KEY_ID
unsigned char Key_cbGetOrgKeyId(void)
{
  //越小优先级越高
  for(unsigned char Pos = 0; Pos < KEY_LIGHT_K_SCAN_COUNT; Pos++){
    unsigned char Key = KeyLight.Key[Pos];
    if(Key){//有键值了，还要进行位判断
      for(unsigned char bit = 0; bit < KEY_LIGHT_K_BIT_COUNT; bit++){
        if(Key & (1 << bit)) //找到了
          return Pos * KEY_LIGHT_K_BIT_COUNT + bit;
      }
    }
  }
  return 0xff;         //定义无效键值
}
#endif //#ifdef SUPPORT_KEY_ID

//-----------------------------判断对应键是否按下-----------------------------
#ifdef SUPPORT_KEY_MASK
unsigned char Key_cbIsKeySel(unsigned char KeySel)
{
  if(KeySel < 8) return KeyLight.Key[0] & (1 << KeySel);
  #if KEY_LIGHT_K_SCAN_COUNT > 1;
    if(KeySel < 16) return KeyLight.Key[1] & (1 << (KeySel - 8));
  #endif
  #if KEY_LIGHT_K_SCAN_COUNT > 2;
    if(KeySel < 16) return KeyLight.Key[2] & (1 << (KeySel - 16));
  #endif
  #if KEY_LIGHT_K_SCAN_COUNT > 3;
    if(KeySel < 16) return KeyLight.Key[3] & (1 << (KeySel - 24));
  #endif
  return 0;
}
#endif //#ifdef SUPPORT_KEY_MASK



