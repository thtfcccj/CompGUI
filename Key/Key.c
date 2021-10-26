/*****************************************************************************

//		        按键模块实现

******************************************************************************/

#include "Key.h"

struct _Key Key;

/*******************************************************************************
					                       相关函数实现
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void Key_Init(void)
{
  Key.Index = 0;
  Key.KeyId = KEY_INVALID_KEY;
  Key_cbCfgIO();
}

//------------------------------------任务函数----------------------------------
//每8ms调用一次以获取键值
void Key_Task(void)
{
  //=======================获得原始键值======================================
  #ifdef SUPPORT_KEY_ID//获得键值ID
    KeySize_t KeyId = Key_cbGetOrgKeyId();
    if(KeyId != KEY_INVALID_KEY) KeyId = Key_cbKeyId2Key(KeyId); //直接获得键值
  #endif
    
  #ifdef SUPPORT_KEY_MASK //获得键值掩码
    KeySize_t KeyId = 0;
    KeySize_t KeyShift = 0x01;
    for(unsigned char KeySel = 0; KeySel < KEY_COUNT; KeySel++){
      if(Key_cbIsKeySel(KeySel)) KeyId |= KeyShift;
      KeyShift <<= 1;
    }
  #endif
    
  //==========================按键处理======================================
	if(KeyId != KEY_INVALID_KEY){//有按键时
    if(Key.KeyId !=  KeyId){	//键值变化,重新开始检测新的按键
      Key.KeyId =  KeyId;//记住当前按键值
      Key.Index = 0;
	  }
	  else{//保持当前键值
      Key.Index++;
      if(Key.Index == KEY_TIMER_LONG){	//长按键通报
        Key_cbLongNotify(KeyId);
      }      
      if(Key.Index == (KEY_TIMER_LONG + KEY_TIMER_HODE)){	//保持键通报
        Key_cbKeepNotify(KeyId);
        Key.Index = KEY_TIMER_LONG; //复位
      }
	  }  
	}
	else{	//无按键或松开按键时
    //松开按键时检查，到达去抖时间时检查长短与保持按键
    if((Key.Index >= KEY_VALID_COUNT) && (Key.Index < KEY_TIMER_LONG)){
      Key_cbShortNotify(Key.KeyId);	//短按键通报
    }
    //最后复位
    Key.Index = 0;
    Key.KeyId = KEY_INVALID_KEY;
    Key_cbRlsNotify();//松开按键通报
  }
}





