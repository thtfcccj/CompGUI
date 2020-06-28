/***************************************************************************

             控蜂鸣器音乐驱动模块->将用户设置好的音乐进行播放
//此模块建立在BuzzerDrv基础之上
****************************************************************************/

#include <string.h>
#include "BuzzerMusic.h"
#include "BuzzerDrv.h"

struct _BuzzerMusic BuzzerMusic;

/***************************************************************************
                          相关函数实现
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerMusic_Init(void)
{
  memset(&BuzzerMusic, 0, sizeof(struct _BuzzerMusic));
  BuzzerMusic.Speed = BUZZER_MUSIC_SPEED_DEFAULT;
  BuzzerDrv_Init();
}

//-----------------------------启动函数-------------------------------------
//形参为音乐片段的ID号
void BuzzerMusic_Start(const MusicType_t *pMusic,   //音乐源
                       MusicLen_t MusicLen)        //音乐长度
{
  BuzzerMusic.pMusic = pMusic;
  BuzzerMusic.MusicLen = MusicLen;
  BuzzerMusic.PlayPos  = 0;
  //启动底层播放
  BuzzerDrv_Start((*pMusic << MUSIC_SHIFT) + MUSIC_START);
  BuzzerMusic.Timer = BuzzerMusic.Speed;
}

//-----------------------------任务函数-------------------------------------
//将此任务放入慢速进程中
void BuzzerMusic_Task(void)
{
  if(!BuzzerMusic.MusicLen) return; //没有音乐要播放

  //等待播放完成
  if(BuzzerMusic.Timer){
    BuzzerMusic.Timer--;
    return;   
  }
  BuzzerMusic.Timer = BuzzerMusic.Speed;

  //切换下一音符
  BuzzerMusic.PlayPos++;
  if(BuzzerMusic.PlayPos >= BuzzerMusic.MusicLen)
    BuzzerMusic.PlayPos = 0;//循环播放
  //启动底层播放
  BuzzerDrv_Start((*(BuzzerMusic.pMusic + BuzzerMusic.PlayPos) << MUSIC_SHIFT)
                   + MUSIC_START);
}

//-----------------------------停止函数-------------------------------------
void BuzzerMusic_Stop(void)
{
  BuzzerMusic.MusicLen = 0;
  BuzzerDrv_Stop();
}

