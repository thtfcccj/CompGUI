/***********************************************************************
                 ��ɫָʾ��ͨ������ģ��
ԭDLightDrv.c
************************************************************************/

#include "DLightDrv.h"

struct _DLightDrv DLightDrv;

//-----------------ָʾ��ɨ��������------------------
//�������128mS������
void DLightDrv_Task(void)
{
  //С���ڼ���:����δ���������ڵ�״̬
  if(DLightDrv.IndexUnit){
    DLightDrv.IndexUnit--;
    if(DLightDrv.IndexUnit) return;
  }
  
  if(DLightDrv.Index) DLightDrv.Index--;//�����ڼ���
  
  //������δ���
  if(DLightDrv.Index){
    //������ʱ,�л�����
    if(DLightDrv.Index > DLightDrv.Cfg.StopUnit){
        if(DLightDrv.CurOn) DLightDrv.CurOn = 0;
        else
          DLightDrv.CurOn = DLightDrv.Cfg.On_En;
    }
    //������ʱ,ֱ�ӵ���
    else DLightDrv.CurOn = 0;
  }
  else{//��������ɸ�λ
    DLightDrv.Index = DLightDrv.Cfg.StopUnit + (DLightDrv.Cfg.FlashCount << 1);
    if(DLightDrv.Cfg.FlashCount) DLightDrv.CurOn = DLightDrv.Cfg.On_En;
    else DLightDrv.CurOn = 0; 
  }
  
  //װ���������ڲ��õƵ�����
  if(DLightDrv.CurOn){
    if(!DLightDrv.Cfg.OnUnit) goto __End;//��������ʱ����
    DLightDrv.IndexUnit = DLightDrv.Cfg.OnUnit;
    DLightDrv_cbSetOn(DLightDrv.CurOn);
  }
  else{
    if(!DLightDrv.Cfg.OffUnit) goto __End;//��������ʱ����
    DLightDrv.IndexUnit = DLightDrv.Cfg.OffUnit; 
    DLightDrv_cbClrOn();
  }
__End:
  DLightDrv_cbAppendTask(); //��������
}
