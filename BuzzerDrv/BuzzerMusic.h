/***************************************************************************

          控蜂鸣器音乐驱动模块 ->产生几种音乐供外部调用并循环播放

****************************************************************************/
#ifndef _BUZZER_MUSIC_H
#define _BUZZER_MUSIC_H

/***************************************************************************
                              相关配置
****************************************************************************/

//默认音速,与Task周期有关
#ifndef BUZZER_MUSIC_SPEED_DEFAULT
  #define BUZZER_MUSIC_SPEED_DEFAULT      0 
#endif

//------------------------定义信息音频流格式-----------------------------
//通过配置下述几位，可降低音频流空间占用
//注：为方便使用，通一以“MusicXXXX_t或MUSIC_XXXX型式”实现：
#ifndef MusicType_t
  #define MusicType_t unsigned char  //定义音乐流占位,通过转换以减少体积
#endif
#ifndef MusicLen_t
  #define MusicLen_t  unsigned char  //定义音乐流信息占位
#endif
#ifndef MUSIC_SHIFT
  #define MUSIC_SHIFT      4         //定义音乐流转换时的移位值,直接使用时为0
#endif
#ifndef MUSIC_START
  #define MUSIC_START      2000      //定义音乐流转换时的起始频率,直接使用时为0
#endif

/***************************************************************************
                          相关结构
****************************************************************************/

struct _BuzzerMusic{
  unsigned char Timer;            //控制声音速度的计数器
  unsigned char Speed;            //控制声音速度

  const MusicType_t *pMusic;      //音乐源
  MusicLen_t MusicLen;            //音乐长度
  unsigned char PlayPos;          //当前播放位置
};

extern struct _BuzzerMusic BuzzerMusic;

/***************************************************************************
                          相关函数
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerMusic_Init(void);

//-----------------------------启动函数-------------------------------------
//形参为音乐片段的ID号
void BuzzerMusic_Start(const MusicType_t *pMusic,   //音乐源
                       MusicLen_t MusicLen);        //音乐长度

//-----------------------------任务函数-------------------------------------
//将此任务放入固定进程中以控制节拍(转换越快，调用也需越快), 建议放在中断中
void BuzzerMusic_Task(void);

//-----------------------------停止函数-------------------------------------
void BuzzerMusic_Stop(void);

//----------------------------设置速度-------------------------------------
#define BuzzerMusic_SetSpeed(speed) do{BuzzerMusic.Speed = speed;}while(0)


#endif //_BUZZER_MUSIC_H


