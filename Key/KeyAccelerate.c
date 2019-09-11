/***************************************************************************

                          ���ּ����ٹ���ģ��ʵ��

***************************************************************************/

#include "KeyAccelerate.h"
#include "Key.h"
#include <string.h>

struct _KeyAccelerate  KeyAccelerate;

//-----------------------------��ʼ������-----------------------------
void KeyAccelerate_Init(void)
{
  memset(&KeyAccelerate, 0, sizeof(struct _KeyAccelerate));
}

//------------------------------����������������-------------------------
//��Ҫ��������ʱ���ô˺���
//�βΣ���ǰ��Ҫ���ٵİ���ֵ
void KeyAccelerate_Start(void)
{
  //�����������:
  unsigned char Gene;
  Gene = KeyAccelerate.Gene;
  if(!Gene){   //�������ٶ�ʱ��
    Gene = KEY_ACCELERATE_MAX >> 1; //�Ӽ���һ�뿪ʼ
    KeyAccelerate.Index = Gene + KEY_ACCELERATE_DELAY; //�״�����ʱ����ʱ
  }
  else if(KeyAccelerate.Gene > 1) //1ʱ��ֹͣ��
    Gene = KeyAccelerate.Gene - 1;
  KeyAccelerate.Gene = Gene; 
}

//-------------------------------������-----------------------------
//����16��32ms������
void KeyAccelerate_Task(void)
{
  //û�мӻ����ӣ���ֹʹ�ü��ٹ���
  if(!KeyAccelerate.Gene) return;
  //���ٶ�ʱ����ʱ
  if(KeyAccelerate.Index){
    KeyAccelerate.Index--;
    if(!KeyAccelerate.Index){    //��ʱ�����ð�������
      #ifdef SUPPORT_KEY_KEEP_TO_SHORT 
        Key_cbShortNotify(Key_GetKeyId());//����Ϊ�̰���
      #else
        Key_cbKeepNotify(Key_GetKeyId());
      #endif
      KeyAccelerate.Index = KeyAccelerate.Gene;//��λ��ʱ��
    }  
  }
}







