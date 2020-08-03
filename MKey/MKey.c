/*****************************************************************************

//		                  多例化按键模块实现

******************************************************************************/

#include "MKey.h"
#include <string.h>

/*******************************************************************************
					                       相关函数实现
*******************************************************************************/

//------------------------------按键加速启动函数-------------------------
//需要按键加速时调用此函数
//形参：当前需要加速的按键值
#ifdef SUPPORT_MKEY_ACCELERATE  //支持按键加速时
void _AccelerateStart(struct _MKey *pKey)
{
  //计算加速因子:
  unsigned char Gene;
  Gene = pKey->Accelerate.Gene;
  if(!Gene){   //启动加速定时器
    Gene = MKEY_ACCELERATE_MAX >> 1; //从加速一半开始
    pKey->Accelerate.Index = Gene + MKEY_ACCELERATE_DELAY; //首次启动时有延时
  }
  else if(pKey->Accelerate.Gene > 1) //1时就停止了
    Gene = pKey->Accelerate.Gene - 1;
  pKey->Accelerate.Gene = Gene; 
}

//----------------------------按键加速任务函数-----------------------------
//放入16或32ms进程中
void MKey_AccelerateTask(struct _MKey *pKey)
{
  //没有加还因子，禁止使用加速功能
  if(!pKey->Accelerate.Gene) return;
  //加速定时器延时
  if(pKey->Accelerate.Index){
    pKey->Accelerate.Index--;
    if(!pKey->Accelerate.Index){    //定时到调用按键处理
      #ifdef SUPPORT_KEY_KEEP_TO_SHORT 
        MKey_cbShortNotify(pKey->AryId, pKey->MKeyId);//处理为短按键
      #else
        MKey_cbKeepNotify(pKey->AryId, pKey->MKeyId);
      #endif
      pKey->Accelerate.Index = pKey->Accelerate.Gene;//复位计时器
    }  
  }
}

#endif

//---------------------------------初始化函数-----------------------------------
void MKey_Init(struct _MKey *pKey, unsigned char AryId)
{
  memset(pKey, 0, sizeof(struct _MKey));
  pKey->AryId = AryId;  
  pKey->MKeyId = _INVALID_MKEY;
  MKey_cbCfgIO(AryId);
}

//------------------------------------任务函数----------------------------------
//每8ms调用一次以获取键值
void MKey_Task(struct _MKey *pKey)
{
  unsigned char AryId = pKey->AryId;
  //=======================获得原始键值======================================
  #ifdef SUPPORT_MKEY_ID//获得键值ID
    MKeySize_t MKeyId = MKey_cbGetOrgMKeyId(AryId);
    if(MKeyId != _INVALID_MKEY) MKeyId = MKey_cbMKeyId2MKey(AryId, MKeyId); //直接获得键值
  #endif
    
  #ifdef SUPPORT_MKEY_MASK //获得键值掩码
    MKeySize_t MKeyId = 0;
    MKeySize_t MKeyShift = 0x01;
    for(unsigned char MKeySel = 0; MKeySel < MKEY_COUNT; MKeySel++){
      if(MKey_cbIsMKeySel(AryId, MKeySel)) MKeyId |= MKeyShift;
      MKeyShift <<= 1;
    }
  #endif
    
  //==========================按键处理======================================
	if(MKeyId != _INVALID_MKEY){//有按键时
    if(pKey->MKeyId !=  MKeyId){	//键值变化,重新开始检测新的按键
      pKey->MKeyId =  MKeyId;//记住当前按键值
      pKey->Index = 0;
	  }
	  else{//保持当前键值
      pKey->Index++;
      if(pKey->Index == MKEY_TIMER_LONG){	//长按键通报
        MKey_cbLongNotify(AryId, MKeyId);
      }      
      if(pKey->Index == (MKEY_TIMER_LONG + MKEY_TIMER_HODE)){	//保持键通报
        #ifdef SUPPORT_MKEY_KEEP_TO_SHORT 
          MKey_cbShortNotify(AryId, MKeyId);//处理为短按键
        #else
          MKey_cbKeepNotify(AryId, MKeyId);
        #endif
        pKey->Index = MKEY_TIMER_LONG; //复位
        #ifdef SUPPORT_MKEY_ACCELERATE  //支持按键加速时
          if(MKey_cbIsAccelarateMKey(AryId, pKey->MKeyId))//判断是否为需要的保持按键
            _AccelerateStart(pKey);
        #endif
      }
	  }  
	}
	else{	//无按键或松开按键时
    //松开按键时检查，到达去抖时间时检查长短与保持按键
    if((pKey->Index >= MKEY_VALID_COUNT) && (pKey->Index < MKEY_TIMER_LONG)){
      MKey_cbShortNotify(AryId, pKey->MKeyId);	//短按键通报
    }
    //最后复位
    pKey->Index = 0;
    pKey->MKeyId = _INVALID_MKEY;
    #ifdef SUPPORT_MKEY_ACCELERATE  //支持按键加速时
      pKey->Accelerate.Gene = 0; //停止
    #endif    
  }
}





