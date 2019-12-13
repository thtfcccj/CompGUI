/*****************************************************************************

		              ����ģ��ص�-��ʹ��KeyLightģ��ʱ��ʵ��

******************************************************************************/

#include "Key.h"
#include "KeyLight.h"

//---------------------------------�õ�ԭʼ��ֵ����-----------------------------
#ifdef SUPPORT_KEY_ID
unsigned char Key_cbGetOrgKeyId(void)
{
  //ԽС���ȼ�Խ��
  for(unsigned char Pos = 0; Pos < KEY_LIGHT_K_SCAN_COUNT; Pos++){
    unsigned char Key = KeyLight.Key[Pos];
    if(Key){//�м�ֵ�ˣ���Ҫ����λ�ж�
      for(unsigned char bit = 0; bit < KEY_LIGHT_K_BIT_COUNT; bit++){
        if(Key & (1 << bit)) //�ҵ���
          return Pos * KEY_LIGHT_K_BIT_COUNT + bit;
      }
    }
  }
  return 0xff;         //������Ч��ֵ
}
#endif //#ifdef SUPPORT_KEY_ID

//-----------------------------�ж϶�Ӧ���Ƿ���-----------------------------
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



