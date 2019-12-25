/*****************************************************************************

//		        ����ģ��ʵ��

******************************************************************************/

#include "Key.h"

#ifdef SUPPORT_KEY_ACCELERATE  //֧�ְ�������ʱ
  #include "KeyAccelerate.h"
#endif

struct _Key Key;

/*******************************************************************************
					                       ��غ���ʵ��
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void Key_Init(void)
{
  Key.Index = 0;
  Key.KeyId = 0xff;
  Key_cbCfgIO();
  #ifdef SUPPORT_KEY_ACCELERATE  //֧�ְ�������ʱ
    KeyAccelerate_Init();
  #endif 
}

//------------------------------------������----------------------------------
//ÿ8ms����һ���Ի�ȡ��ֵ
void Key_Task(void)
{
  //=======================���ԭʼ��ֵ======================================
  #ifdef SUPPORT_KEY_ID//��ü�ֵID
    KeySize_t KeyId = Key_cbGetOrgKeyId();
    if(KeyId != _INVALID_KEY) KeyId = Key_cbKeyId2Key(KeyId); //ֱ�ӻ�ü�ֵ
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
	if(KeyId != _INVALID_KEY){//�а���ʱ
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
        #ifdef SUPPORT_KEY_KEEP_TO_SHORT 
          Key_cbShortNotify(KeyId);//����Ϊ�̰���
        #else
          Key_cbKeepNotify(KeyId);
        #endif
        Key.Index = KEY_TIMER_LONG; //��λ
        #ifdef SUPPORT_KEY_ACCELERATE  //֧�ְ�������ʱ
          if(Key_cbIsAccelarateKey(Key.KeyId)//�ж��Ƿ�Ϊ��Ҫ�ı��ְ���
            KeyAccelerate_Start();
        #endif
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
    Key.KeyId = _INVALID_KEY;
    #ifdef SUPPORT_KEY_ACCELERATE  //֧�ְ�������ʱ
      KeyAccelerate_Stop();
    #endif    
  }
}





