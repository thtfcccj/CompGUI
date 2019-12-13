/*****************************************************************************

		              ָʾ���밴��ʱ��ʵ��

******************************************************************************/

#include "KeyLight.h"
#include <string.h>

struct _KeyLight KeyLight; //ֱ�ӵ�����


/*****************************************************************************
                            ��غ���ʵ��
******************************************************************************/

//-------------------------��ʼ������---------------------------------------
void KeyLight_Init(void)
{
  memset(&KeyLight, 0, sizeof(struct _KeyLight));
  KeyLight_cbInitIo();
}

//-----------------------------������------------------------------------
//1ms����һ��,���ظ���׼��״̬�������´�ɨ�谴��λ��
signed char KeyLight_Task(void)
{
  if(KeyLight.KeyPos & 0x80){//��ȡ����ʱ
    //���ȶ�ȡ��ֵ
    unsigned char KeyPos = KeyLight.KeyPos & 0x7f;
    KeyLight.Key[KeyPos] = KeyLight_cbGetKeyVol();
    //Ȼ����ָʾ��
    KeyLight_cbSetKeyScan(-1); //�رհ���ɨ��
    KeyLight_cbSetLightScan(KeyLight.LightPos++);
    if(KeyLight.LightPos >= KEY_LIGHT_L_SCAN_COUNT) KeyLight.LightPos = 0;
    //���Ԥ����һ��ֵɨ��״̬
    KeyPos++;
    if(KeyPos >= KEY_LIGHT_L_SCAN_COUNT) KeyPos = 0;
    KeyLight.KeyPos = KeyPos;
    return KeyPos; //�м�ֵ��
  }
  else{//׼��������ʱ
    KeyLight_cbSetLightScan(-1);            //�ر�ָʾ��ɨ��
    KeyLight_cbSetKeyScan(KeyLight.KeyPos); //��������ɨ��
    KeyLight.KeyPos |= 0x80;                //����һ����Ϊɨ�����ڱ�־
    return -1;// - KeyLight.LightPos;
  }
}


