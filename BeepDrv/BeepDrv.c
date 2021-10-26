/***********************************************************************

//                 ������������ͨ������ģ��ʵ��

************************************************************************/
#include "BeepDrv.h"
#include <string.h>

/****************************************************************************
                      ��ؽṹ������
****************************************************************************/
struct _BeepDrv BeepDrv;

//------------------------- ��ʼ������---------------------
void BeepDev_Init(void)
{
  BeepDrv_cbCfgIo();
  BeepDrv.RepeatIndex = 0; //δ��ʼ��cfg,�ʿ�������
}

//----------------- ������ɨ��������------------------
//�������128mS������
void BeepDev_Task(void)
{
  if(!BeepDrv.RepeatIndex) return;//û�п������ظ����������

  //С���ڼ���:����δ���������ڵ�״̬
  if(BeepDrv.IndexUnit){
    BeepDrv.IndexUnit--;
    if(BeepDrv.IndexUnit) return;
  }

  if(BeepDrv.Index) BeepDrv.Index--;//�����ڼ���

  //������δ���
  if(BeepDrv.Index){
    //������ʱ,�л�����
    if(BeepDrv.Index > BeepDrv.Cfg.StopUnit){
        if(BeepDrv.RepeatIndex & BEEP_DRV_BEEP_ON)
          BeepDrv.RepeatIndex &= ~BEEP_DRV_BEEP_ON;
        else
          BeepDrv.RepeatIndex |= BEEP_DRV_BEEP_ON;
    }
    //������ʱ,ֱ�ӵ���
    else{
      BeepDrv.RepeatIndex &= ~BEEP_DRV_BEEP_ON;
    }
  }
  else{//��������ɸ�λ
    //�ظ���������
    if(!(BeepDrv.Cfg.Repeat & BEEP_DRV_REPEAT_ALL)){//һֱ��ر�ʱ:
      if(BeepDrv.RepeatIndex) BeepDrv.RepeatIndex--;
      if(!BeepDrv.RepeatIndex){//û�п������ظ����������
        BeepDrv_cbClrOn();//ǿ��ֹͣ
        return;
      }
    }

    BeepDrv.Index = BeepDrv.Cfg.StopUnit + (BeepDrv.Cfg.FlashCount << 1);
    if(BeepDrv.Cfg.FlashCount)
      BeepDrv.RepeatIndex |= BEEP_DRV_BEEP_ON;
    else
      BeepDrv.RepeatIndex &= ~BEEP_DRV_BEEP_ON;
  }

  //װ���������ڲ��õƵ�����
  if(BeepDrv.RepeatIndex & BEEP_DRV_BEEP_ON){//������
    if(!BeepDrv.Cfg.OnUnit) return;//��������ʱ����
    BeepDrv.IndexUnit = BeepDrv.Cfg.OnUnit;
    BeepDrv_cbSetOn();
  }
  else{
    if(!BeepDrv.Cfg.OffUnit) return;//��������ʱ����
    BeepDrv.IndexUnit = BeepDrv.Cfg.OffUnit;
    BeepDrv_cbClrOn();
  }
}

//------------------ ���������øı亯��-----------------------
void BeepDev_CfgChange(const struct  _BeepDrvCfg *pCfg)
{
  memcpy(&BeepDrv.Cfg, pCfg, sizeof(struct  _BeepDrvCfg));
  BeepDrv.IndexUnit = 0;
  BeepDrv.Index = 0;
  BeepDrv.RepeatIndex = (BeepDrv.Cfg.Repeat & BEEP_DRV_REPEAT_MASK) + 1;
}

