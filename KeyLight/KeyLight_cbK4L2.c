/*****************************************************************************

		              KeyLight�ص�����-��(IOʵ��)4��ɨ2��ɨ�е�ʵ��

******************************************************************************/

#include "KeyLight.h"
#if KEY_LIGHT_L_SCAN_COUNT == 2

/*****************************************************************************
                            �ص������ӿ�ʵ��
******************************************************************************/

//-----------------------------�ð���ɨ��λ��-------------------------------
//< 0��ʾ�ر�ɨ�裬������ͬʱ��������IO��Ϊ����
void KeyLight_cbSetKeyScan(signed char Pos)
{
  //�ȹر�ɨ������
  SetKeySo();
  if(Pos < 0) return; ////�ر�ɨ��
  //ɨ���Ӧλ��
  switch(Pos){
    case 0: ClrKeySo0(); break;
    case 1: ClrKeySo1(); break;
    case 2: ClrKeySo2(); break;
    case 3: ClrKeySo3(); break;
  }
  InKlIo(); //�ø���IOΪ����
}

//----------------------------��ɨ���ɨ��λ��-------------------------------
//< 0��ʾ�ر�ɨ�裬��ͬʱ��дָʾ��IO��Ϊ���
void KeyLight_cbSetLightScan(signed char Pos)
{
  //�ȹر�ɨ������
  SetLightEn();
  if(Pos < 0) return; //�ر�ɨ��
  //ɨ���Ӧλ��
  switch(Pos){
    case 0: ClrLightEn0(); break;
    case 1: ClrLightEn1(); break;
  }
  OutKlIo(); //�ø���IOΪ���
}

#endif //KEY_LIGHT_L_SCAN_COUNT == 1

