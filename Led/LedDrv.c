/***************************************************************************
                       ����IO����ʵ��ģ��
***************************************************************************/

#include "LedDrv.h"
#include <string.h>

struct _LedDrv  LedDrv;

/***************************************************************************
                              ��غ���ʵ��
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void LedDrv_Init(void)
{
  LedDrv_cbCfgIo();
  memset(&LedDrv, 0, sizeof(struct _LedDrv));
  //memset(&LedDrv.Buf, 0xff, LED_DRV_LED_COUNT);  //����Ԥ��ȫ��
  //LedDrv.DispEn = 0x1f;//Ĭ�ϵ�4����������λ�����
}

//--------------------------�õ��¸�ɨ��λ-----------------------------
//���� >= LED_DRV_LED_COUNT��ʾδ�ҵ�
static unsigned char _GetNextScanPos(unsigned char ScanPos)
{
  //�õ���ǰ������������
  unsigned char Mask = 1 << ScanPos;
  for(; ScanPos < LED_DRV_LED_COUNT; ScanPos++, Mask <<= 1){
    if(Mask & LedDrv.DispEn) break;//��ʾ����
  }
  return ScanPos;
}

//-------------------------------������-----------------------------
//������̻��ж���ɨ��,ɨ���ʾ�����ʾ���
void LedDrv_Task(void)
{
  //����һλ��
  unsigned char ScanPos = _GetNextScanPos(LedDrv.ScanPos + 1);
  //�ػ���,��ͷ��ʼ��(�ٶ��п���,�����ҵ�)
  if(ScanPos >= LED_DRV_LED_COUNT){
    if(LecDrv_cbPeriodFinal()) return; //һ����ɨ�����ͨ��
    ScanPos = _GetNextScanPos(0);
  }
  
  unsigned char Data;  //�õ������Ե�����
  if(LedDrv_cbIsTest()) Data = 0xff;  //����״̬Ϊȫ��
  else Data = LedDrv.Buf[ScanPos];    //�����ö����� 
  
  //������ģʽ,�����ڿ���״̬ʱ,����ʾ��Ϊ�ر�
  unsigned char Mask = 1 << ScanPos;
  if(Mask & LedDrv.LowEn){//����ģʽ����ʱ
    if(Mask & LedDrv.LowHalf){//1/2����ʱ
      if(LedDrv.ScanIndex & 0x01) Data = 0;
    }
    else{ //1/4����ʱ
      if(LedDrv.ScanIndex & 0x03) Data = 0;
    }
  }
 
  //�������ʱΪ��ֹ�ж�Ӱ�첻����,�ٽ�������
  LedDrv_cbEnterCritical();
  LedDrv_cbClrComm(LedDrv.ScanPos);//������ԭ����ʾ
  LedDrv_cbSetSeg(Data); //д��ʾ����
  LedDrv_cbSetComm(ScanPos);//��ʾ��ǰ��
  LedDrv_cbExitCritical();  
  
  //Ϊ�´���׼��
  LedDrv.ScanPos = ScanPos;
  LedDrv.ScanIndex++;
}

