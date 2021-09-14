/***************************************************************************

                       �������ʾ��������-��ʹ��1652ʱ��ʵ��

***************************************************************************/
#include "LedDrv1652.h"
#include <string.h>

struct _LedDrv1652  LedDrv1652;

static const unsigned char _ToHwLut[LED_DRV_LED_COUNT] = LED1652_TO_HW_LUT;

//�������ٸ���ʱ�������ϴ�ֵ
#ifdef LED1652_FAST_BIT 
  static unsigned char _PrvFastVol;
#endif

/***************************************************************************
                    ͨѶ����-ͨ��USARTʵ��
***************************************************************************/
#ifdef SUPPORT_LED_DRV_1652_USART

//--------------------Usart�����жϴ���������--------------------------------
//�β�Ϊ��struct _UsartDevָ��
static signed char _UsartDevInt(void *pv)
{
  LedDrv1652.Timer = 3; //ͨѶ���,������3msʱ��
  return 0;
}

//----------------------------��������---------------------------------------
static void LedDrv1652_cbSendStart(unsigned char SendSize)
{
  UsartDev_SendStart(LedDrv1652_cbpGetUsartDev(), //�����豸
                     LedDrv1652.CommBuf,                    //���ͻ�����
                     0x8000 | SendSize,//���ͻ�������С,�������ж�
                     _UsartDevInt);               //���ͻص�����
}
#endif

/***************************************************************************
                              ��غ���ʵ��
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void LedDrv1652_Init(void)
{
  memset(&LedDrv1652, 0, sizeof(struct _LedDrv1652));
  //����1652
  LedDrv1652.CommBuf[0] = 0x18;//��ʾ��������
  LedDrv1652.CommBuf[1] = LED1652_CFG;
  LedDrv1652_cbHwInit();
  LedDrv1652_cbSendStart(2);
  LedDrv1652.UpdateMask = 0xff;//�״θ�������,ͬʱ��ͨѶ�б�־
  LedDrv1652.Timer = 3; //ͨѶ�У�����3msʱ��
}

//-------------------------------������-----------------------------
//������̻��ж���ɨ��,ɨ���ʾ�����ʾ���
void LedDrv1652_Task(void)
{
  if(LedDrv1652.Timer){//ͨѶ�ȴ������� 
    LedDrv1652.Timer--;
    return; 
  }
  if(LedDrv1652.UpdateMask == 0){//�������ʱ
    return; 
  }
  
  //����ģʽ����
  if(LedDrv1652_cbIsTest()){
    memset(&LedDrv1652.CommBuf[1], 0xff, LED_DRV_LED_COUNT);
    #ifdef LED1652_RSV_MASK//�б���λʱ����ԭֵ
      LedDrv1652.CommBuf[LED1652_RSV_POS + 1] &= ~LED1652_RSV_MASK;  
      LedDrv1652.CommBuf[LED1652_RSV_POS + 1] |= 
        LedDrv1652.Buf[_ToHwLut[LED1652_RSV_POS]] & LED1652_RSV_MASK;       
    #endif
  }
  //�������ٸ���: ��ָʾ�Ƶȿ��ٸ���
  #ifdef LED1652_FAST_BIT 
  else if(LedDrv1652.UpdateMask == (1 << LED1652_FAST_BIT)){
    LedDrv1652.UpdateMask =  (1 << LED1652_FAST_BIT);//  ���߻���
    unsigned char Cur = LedDrv1652.Buf[_ToHwLut[LED1652_FAST_BIT]];
    if(_PrvFastVol == Cur) return;//��û�иı�
    _PrvFastVol = Cur;
    LedDrv1652.CommBuf[LED1652_FAST_BIT + 1] = Cur;
    LedDrv1652.CommBuf[0] = 0x08 | LED1652_FAST_BIT;//��ʾ��ַ����
    LedDrv1652.Timer = 255; //ͨѶ��
    LedDrv1652_cbSendStart(2);
    return;
  }
  #endif
  //�������£���ʹ��ֱ��ȫ�����²����Խ�ʡ����
  else{   
    LedDrv1652.CommBuf[0] = 0x08;//��ʾ��ַ����
    LedDrv1652.CommBuf[1] = LedDrv1652.Buf[_ToHwLut[0]];
    LedDrv1652.CommBuf[2] = LedDrv1652.Buf[_ToHwLut[1]];
    LedDrv1652.CommBuf[3] = LedDrv1652.Buf[_ToHwLut[2]];
    LedDrv1652.CommBuf[4] = LedDrv1652.Buf[_ToHwLut[3]];
    LedDrv1652.CommBuf[5] = LedDrv1652.Buf[_ToHwLut[4]];
    #if LED_DRV_LED_COUNT >= 6
      LedDrv1652.CommBuf[6] = LedDrv1652[_ToHwLut[5]];
    #endif
      
  }
  LedDrv1652.CommBuf[0] = 0x08;//��ʾ��ַ����  
  LedDrv1652.UpdateMask = 0;//  ���߻���
  LedDrv1652.Timer = 255; //ͨѶ��
  LedDrv1652_cbSendStart(LED_DRV_LED_COUNT + 1);
}


