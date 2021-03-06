/***************************************************************************

                          保持键加速功能模块实现

***************************************************************************/

#include "KeyAccelerate.h"
#include <string.h>

struct _KeyAccelerate  KeyAccelerate;

//-----------------------------初始化函数-----------------------------
void KeyAccelerate_Init(void)
{
  memset(&KeyAccelerate, 0, sizeof(struct _KeyAccelerate));
}

//-------------------------按键加速启动函数--------------------------------
//检查到为需要加速的保持按键时，调用此函数实现加速
void KeyAccelerate_Start(KeyAccelerate_KeyType_t Key)//键值
{
  //计算加速因子:
  unsigned char Gene;
  Gene = KeyAccelerate.Gene;
  if(!Gene){   //启动加速定时器
    Gene = KEY_ACCELERATE_MAX >> 1; //从加速一半开始
    KeyAccelerate.Index = Gene + KEY_ACCELERATE_DELAY; //首次启动时有延时
    KeyAccelerate.Key = Key;
  }
  else if(KeyAccelerate.Gene > 1) //1时就停止了
    Gene = KeyAccelerate.Gene - 1;
  KeyAccelerate.Gene = Gene; 
}

//-------------------------------任务函数-----------------------------
//放入16或32ms进程中
void KeyAccelerate_Task(void)
{
  //没有加还因子，禁止使用加速功能
  if(!KeyAccelerate.Gene) return;
  //加速定时器延时
  if(KeyAccelerate.Index){
    KeyAccelerate.Index--;
    if(!KeyAccelerate.Index){    //定时到调用按键处理
      KeyAccelerate_cbKey(KeyAccelerate.Key);   //留给用户处理加速
      KeyAccelerate.Index = KeyAccelerate.Gene;//复位计时器
    }  
  }
}







