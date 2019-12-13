/*****************************************************************************

		              KeyLight�ص�����-��(IOʵ��)4��ɨ1��ɨ�е�ʵ��

******************************************************************************/

#include "KeyLight.h"

/*****************************************************************************
                            �ص������ӿ�ʵ��
******************************************************************************/

//-----------------------------�ð���ɨ��λ��-------------------------------
//< 0��ʾ�ر�ɨ�裬������ͬʱ��������IO��Ϊ����
void KeyLight_cbSetKeyScan(signed char Pos)
{
  //�ȹر�ɨ������
  SetKeySo0();
  SetKeySo1();
  SetKeySo2();
  SetKeySo3();
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
  if(Pos < 0){ //�ر�ɨ��
    SetLightEn();
    return;
  }
  //ɨ���Ӧλ��
  ClrLightEn();
  OutKlIo(); //�ø���IOΪ���
}

