/*****************************************************************************

//		        ����ģ��ʵ��

******************************************************************************/

#include "Key.h"

struct _Key Key;

/*******************************************************************************
					                       ��غ���ʵ��
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void Key_Init(void)
{
  Key.Index = 0;
  Key.KeyId = KEY_INVALID_KEY;
  Key_cbCfgIO();
}

//------------------------------------������----------------------------------
//ÿ8ms����һ���Ի�ȡ��ֵ
void Key_Task(void)
{
  //=======================���ԭʼ��ֵ======================================
  #ifdef SUPPORT_KEY_ID//��ü�ֵID
    KeySize_t KeyId = Key_cbGetOrgKeyId();
    if(KeyId != KEY_INVALID_KEY) KeyId = Key_cbKeyId2Key(KeyId); //ֱ�ӻ�ü�ֵ
  #endif
    
  #ifdef SUPPORT_KEY_MASK //��ü�ֵ����
    KeySize_t KeyId = 0;
    KeySize_t KeyShift = 0x01;
    for(unsigned char KeySel = 0; KeySel < KEY_COUNT; KeySel++){
      if(Key_cbIsKeySel(KeySel)) KeyId |= KeyShift;
      KeyShift <<= 1;
    }
  #endif
    
  //==========================��������======================================
	if(KeyId != KEY_INVALID_KEY){//�а���ʱ
    if(Key.KeyId !=  KeyId){	//��ֵ�仯,���¿�ʼ����µİ���
      Key.KeyId =  KeyId;//��ס��ǰ����ֵ
      Key.Index = 0;
	  }
	  else{//���ֵ�ǰ��ֵ
      Key.Index++;
      if(Key.Index == KEY_TIMER_LONG){	//������ͨ��
        Key_cbLongNotify(KeyId);
      }      
      if(Key.Index == (KEY_TIMER_LONG + KEY_TIMER_HODE)){	//���ּ�ͨ��
        Key_cbKeepNotify(KeyId);
        Key.Index = KEY_TIMER_LONG; //��λ
      }
	  }  
	}
	else{	//�ް������ɿ�����ʱ
    //�ɿ�����ʱ��飬����ȥ��ʱ��ʱ��鳤���뱣�ְ���
    if((Key.Index >= KEY_VALID_COUNT) && (Key.Index < KEY_TIMER_LONG)){
      Key_cbShortNotify(Key.KeyId);	//�̰���ͨ��
    }
    //���λ
    Key.Index = 0;
    Key.KeyId = KEY_INVALID_KEY;
    Key_cbRlsNotify();//�ɿ�����ͨ��
  }
}





