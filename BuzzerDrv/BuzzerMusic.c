/***************************************************************************

             �ط�������������ģ��->���û����úõ����ֽ��в���
//��ģ�齨����BuzzerDrv����֮��
****************************************************************************/

#include <string.h>
#include "BuzzerMusic.h"
#include "BuzzerDrv.h"

struct _BuzzerMusic BuzzerMusic;

/***************************************************************************
                          ��غ���ʵ��
****************************************************************************/

//------------------------------��ʼ������---------------------------------
void BuzzerMusic_Init(void)
{
  memset(&BuzzerMusic, 0, sizeof(struct _BuzzerMusic));
  BuzzerMusic.Speed = BUZZER_MUSIC_SPEED_DEFAULT;
  BuzzerDrv_Init();
}

//-----------------------------��������-------------------------------------
//�β�Ϊ����Ƭ�ε�ID��
void BuzzerMusic_Start(const MusicType_t *pMusic,   //����Դ
                       MusicLen_t MusicLen)        //���ֳ���
{
  BuzzerMusic.pMusic = pMusic;
  BuzzerMusic.MusicLen = MusicLen;
  BuzzerMusic.PlayPos  = 0;
  //�����ײ㲥��
  BuzzerDrv_Start((*pMusic << MUSIC_SHIFT) + MUSIC_START);
  BuzzerMusic.Timer = BuzzerMusic.Speed;
}

//-----------------------------������-------------------------------------
//��������������ٽ�����
void BuzzerMusic_Task(void)
{
  if(!BuzzerMusic.MusicLen) return; //û������Ҫ����

  //�ȴ��������
  if(BuzzerMusic.Timer){
    BuzzerMusic.Timer--;
    return;   
  }
  BuzzerMusic.Timer = BuzzerMusic.Speed;

  //�л���һ����
  BuzzerMusic.PlayPos++;
  if(BuzzerMusic.PlayPos >= BuzzerMusic.MusicLen)
    BuzzerMusic.PlayPos = 0;//ѭ������
  //�����ײ㲥��
  BuzzerDrv_Start((*(BuzzerMusic.pMusic + BuzzerMusic.PlayPos) << MUSIC_SHIFT)
                   + MUSIC_START);
}

//-----------------------------ֹͣ����-------------------------------------
void BuzzerMusic_Stop(void)
{
  BuzzerMusic.MusicLen = 0;
  BuzzerDrv_Stop();
}

