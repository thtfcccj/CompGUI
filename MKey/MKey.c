/*****************************************************************************

//		                  ����������ģ��ʵ��

******************************************************************************/

#include "MKey.h"
#include <string.h>

/*******************************************************************************
					                       ��غ���ʵ��
*******************************************************************************/

//------------------------------����������������-------------------------
//��Ҫ��������ʱ���ô˺���
//�βΣ���ǰ��Ҫ���ٵİ���ֵ
#ifdef SUPPORT_MKEY_ACCELERATE  //֧�ְ�������ʱ
void _AccelerateStart(struct _MKey *pKey)
{
  //�����������:
  unsigned char Gene;
  Gene = pKey->Accelerate.Gene;
  if(!Gene){   //�������ٶ�ʱ��
    Gene = MKEY_ACCELERATE_MAX >> 1; //�Ӽ���һ�뿪ʼ
    pKey->Accelerate.Index = Gene + MKEY_ACCELERATE_DELAY; //�״�����ʱ����ʱ
  }
  else if(pKey->Accelerate.Gene > 1) //1ʱ��ֹͣ��
    Gene = pKey->Accelerate.Gene - 1;
  pKey->Accelerate.Gene = Gene; 
}

//----------------------------��������������-----------------------------
//����16��32ms������
void MKey_AccelerateTask(struct _MKey *pKey)
{
  //û�мӻ����ӣ���ֹʹ�ü��ٹ���
  if(!pKey->Accelerate.Gene) return;
  //���ٶ�ʱ����ʱ
  if(pKey->Accelerate.Index){
    pKey->Accelerate.Index--;
    if(!pKey->Accelerate.Index){    //��ʱ�����ð�������
      #ifdef SUPPORT_KEY_KEEP_TO_SHORT 
        MKey_cbShortNotify(pKey->AryId, pKey->MKeyId);//����Ϊ�̰���
      #else
        MKey_cbKeepNotify(pKey->AryId, pKey->MKeyId);
      #endif
      pKey->Accelerate.Index = pKey->Accelerate.Gene;//��λ��ʱ��
    }  
  }
}

#endif

//---------------------------------��ʼ������-----------------------------------
void MKey_Init(struct _MKey *pKey, unsigned char AryId)
{
  memset(pKey, 0, sizeof(struct _MKey));
  pKey->AryId = AryId;  
  pKey->MKeyId = _INVALID_MKEY;
  MKey_cbCfgIO(AryId);
}

//------------------------------------������----------------------------------
//ÿ8ms����һ���Ի�ȡ��ֵ
void MKey_Task(struct _MKey *pKey)
{
  unsigned char AryId = pKey->AryId;
  //=======================���ԭʼ��ֵ======================================
  #ifdef SUPPORT_MKEY_ID//��ü�ֵID
    MKeySize_t MKeyId = MKey_cbGetOrgMKeyId(AryId);
    if(MKeyId != _INVALID_MKEY) MKeyId = MKey_cbMKeyId2MKey(AryId, MKeyId); //ֱ�ӻ�ü�ֵ
  #endif
    
  #ifdef SUPPORT_MKEY_MASK //��ü�ֵ����
    MKeySize_t MKeyId = 0;
    MKeySize_t MKeyShift = 0x01;
    for(unsigned char MKeySel = 0; MKeySel < MKEY_COUNT; MKeySel++){
      if(MKey_cbIsMKeySel(AryId, MKeySel)) MKeyId |= MKeyShift;
      MKeyShift <<= 1;
    }
  #endif
    
  //==========================��������======================================
	if(MKeyId != _INVALID_MKEY){//�а���ʱ
    if(pKey->MKeyId !=  MKeyId){	//��ֵ�仯,���¿�ʼ����µİ���
      pKey->MKeyId =  MKeyId;//��ס��ǰ����ֵ
      pKey->Index = 0;
	  }
	  else{//���ֵ�ǰ��ֵ
      pKey->Index++;
      if(pKey->Index == MKEY_TIMER_LONG){	//������ͨ��
        MKey_cbLongNotify(AryId, MKeyId);
      }      
      if(pKey->Index == (MKEY_TIMER_LONG + MKEY_TIMER_HODE)){	//���ּ�ͨ��
        #ifdef SUPPORT_MKEY_KEEP_TO_SHORT 
          MKey_cbShortNotify(AryId, MKeyId);//����Ϊ�̰���
        #else
          MKey_cbKeepNotify(AryId, MKeyId);
        #endif
        pKey->Index = MKEY_TIMER_LONG; //��λ
        #ifdef SUPPORT_MKEY_ACCELERATE  //֧�ְ�������ʱ
          if(MKey_cbIsAccelarateMKey(AryId, pKey->MKeyId))//�ж��Ƿ�Ϊ��Ҫ�ı��ְ���
            _AccelerateStart(pKey);
        #endif
      }
	  }  
	}
	else{	//�ް������ɿ�����ʱ
    //�ɿ�����ʱ��飬����ȥ��ʱ��ʱ��鳤���뱣�ְ���
    if((pKey->Index >= MKEY_VALID_COUNT) && (pKey->Index < MKEY_TIMER_LONG)){
      MKey_cbShortNotify(AryId, pKey->MKeyId);	//�̰���ͨ��
    }
    //���λ
    pKey->Index = 0;
    pKey->MKeyId = _INVALID_MKEY;
    #ifdef SUPPORT_MKEY_ACCELERATE  //֧�ְ�������ʱ
      pKey->Accelerate.Gene = 0; //ֹͣ
    #endif    
  }
}





