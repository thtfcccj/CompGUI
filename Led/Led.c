/***************************************************************************
                       �������ʾ�ӿ�
***************************************************************************/

#include "Led.h"
#include <string.h>

struct _Led  Led;

/***************************************************************************
                              ��غ���ʵ��
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void Led_Init(void)
{
  memset(&Led, 0, sizeof(struct _Led));
  //memset(&Led.SegDisp, 0xff, LED_LED_COUNT); //ȫ��״̬
  Led.Flag = 0x02;  //Ԥ��ϵͳΪ����
  //Led.Timer = LED_FLASH_TIMER_COUNT * 2;//ϵͳԤ��׼��
  Led_cbAppendInit(); //���ӳ�ʼ��
}

//-------------------------------������-----------------------------
//����16ms������ɨ��,�Ի������Ч��,Ҳ��ֱ�ӵ��ø�������
void Led_Task(void)
{
  //�ж�����ʱ,����ʾ��������������
  unsigned char Disp;
  if(Led.Flag & LED_FLASH_NOW){//��������
    if(Led.Flag & LED_FLASH_DIS) Disp = 0;
    else Disp = 0xff;
  }
  else if((Led.Flag & LED_UPDATE_NOW) || (!Led.Timer)){//�������»�ʱ���µ�
    if(Led.Flag & LED_DISP) Disp = 0;
    else Disp = 0xff;    
  }
  else{//��ʱδ��,��ˢ��
    Led.Timer--;
    return; 
  }
  Led.Flag &= ~(LED_UPDATE_NOW | LED_FLASH_NOW | LED_FLASH_DIS); //ȡ����ر�־  
  //Led.Flag |= 0x02;  //Ԥ��ϵͳΪ����  
  Led.Timer = LED_FLASH_TIMER_COUNT;//���¿�ʼ

  unsigned char DotDisp = Led.DotDisp;
  if(Disp){//����ʾ����
    Led.Flag |= LED_DISP;
    DotDisp |= Led.DotFlash; //��������ʾ��С����
  }
  else{//��������
    Led.Flag &= ~LED_DISP;
    Disp = ~Led.SegFlash; //�����Ĳ���ʾ
    DotDisp &= ~Led.DotFlash; //ȥ��������ʾ��С����  
  }
  Led.Flag &= ~LED_UPDATE_NOW;//ȡ���������±�־
  
  //��������ֱܷ�����
  unsigned char PosShift = 0x01;
  unsigned char Pos = 0;
  for(; Pos < LED_LED_COUNT; Pos++, PosShift <<= 1){
    unsigned char Data;
    if(Disp & PosShift){//������
      Data = Led.SegDisp[Pos] & (~LED_DOT_BUF_MASK);  //С���㲻ȡ
      if(Pos >= (LED_LED_COUNT - LED_SEG_FLASH_COUNT)){//��������������ʱ
        if(!(Led.Flag & LED_DISP)) //������,����������
          Data &= ~Led.SegGrpFlash[Pos - (LED_LED_COUNT - LED_SEG_FLASH_COUNT)];
        else //������,������ǿ����ʾ
          Data |= Led.SegGrpFlash[Pos - (LED_LED_COUNT - LED_SEG_FLASH_COUNT)];
      }
    }
    else Data = 0;
    if(DotDisp & PosShift) //С��������
      Data |= LED_DOT_BUF_MASK;
    
    #ifndef SUPPORT_LCD
      Data &= ~ Led.DisBuf[Pos]; //ȥ����ֹ���µĶ�
      Data |= Led_cbGetBuf(Pos) & Led.DisBuf[Pos]; //����ԭ�������еĶ���ʾ
    #endif
    Led_cbSetBuf(Pos, Data);
  }
  Led_cbAppendUpdate(Led.Flag);  //���ӹ�������(LCD��������������θ���)
}

