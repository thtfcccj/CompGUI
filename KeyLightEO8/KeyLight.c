/*****************************************************************************

		              ɨ��ʱ��ָʾ�����ð���-����8λ����ϵ�ʵ��

******************************************************************************/

#include "KeyLight.h"
#include <string.h>

struct _KeyLight KeyLight; //ֱ�ӵ�����

//����595�ϣ�ָʾ���밴����ɨ������    
static const unsigned char _ScanLut[] = KeyLight_cbScanLut(); 

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
//����1~10ms�ж���ʵ��
void KeyLight_IntTask(void)
{
  //�ȶ�ȡ��ֵ
  unsigned char ScanPos = KeyLight.ScanPos;
  KeyLight.Key[ScanPos] = KeyLight_cbGetKeyVol();
  //��һ��
  ScanPos++; 
  if(ScanPos >= KEY_LIGHT_SCAN_COUNT) ScanPos = 0;
  KeyLight.ScanPos = ScanPos;
  //�������µ�����
  unsigned char Data;
  if(KeyLight_cbIsTest()) Data = KeyLight_cbLightOn(); //ָʾ��ȫ��
  else Data = KeyLight.Light[ScanPos] & KeyLight_cbLightMask(); //ָʾ����
  Data |= _ScanLut[ScanPos] | KeyLight_cbGetFixBit(); //ɨ��λ���̶�λ����
  //һ�������
  KeyLight_cbUpdateOut(Data);
}




