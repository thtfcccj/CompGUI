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

//-------------------------�ж�������------------------------------------
//����1ms�ж���ʵ��
void KeyLight_IntTask(void)
{
  if(KeyLight.KeyPos & 0x80){//��ȡ����ʱ,����������ָʾ��
    unsigned char KeyPos = KeyLight.KeyPos & 0x7f;
    //�ɶ�ȡ��ֵʱ������ֵ(����ɨ��)
    if(KeyPos < KEY_LIGHT_K_SCAN_COUNT){
      KeyLight.Key[KeyPos] = KeyLight_cbGetKeyVol();
      KeyLight_cbSetKeyScan(-1); //�رհ���ɨ��
      //���Ԥ����һ��ֵɨ��״̬
      KeyPos++;
    }
    KeyLight.KeyPos = KeyPos;
    
    //ɨ��һ��
    KeyLight_cbSetLightVol(KeyLight.Light[KeyLight.LightPos]);//���õ�״̬
    KeyLight_cbSetLightScan(KeyLight.LightPos++);//�õ�λ
    if(KeyLight.LightPos >= KEY_LIGHT_L_SCAN_COUNT) KeyLight.LightPos = 0;
  }
  else{//׼��������ʱ
    KeyLight_cbSetLightScan(-1);            //�ر�ָʾ��ɨ��
    KeyLight_cbSetKeyScan(KeyLight.KeyPos); //��������ɨ��
    KeyLight.KeyPos |= 0x80;                //����һ����Ϊɨ�����ڱ�־
  }
}

//-----------------------------������------------------------------------
//1ms����һ��,����0,����δɨ���꣬����ɨ����
signed char KeyLight_Task(void)
{
  //ɨ������
  if((KeyLight.KeyPos & 0x7f) >= KEY_LIGHT_K_SCAN_COUNT){
    KeyLight.KeyPos = 0;//��׼�����ڿ�ʼɨ��һ����ֵ
    return 1; 
  }
  return 0;
}



