/*******************************************************************************

                 ��ɫָʾ��ͨ������ģ��ʵ��
��ģ��ר����������ɫָʾ���Բ�ͬ��ʽ��ʾ,Ҳ������������������Ӧ��
********************************************************************************/
#include "SLightDrv.h"
#include <string.h>

//-----------------------------------��ʼ������------------------------------
void SLightDrv_Init(struct  _SLightDrv *pDrv,
                     unsigned char SLightDrvId)//�����ID��
{
  memset(pDrv, 0, sizeof(struct  _SLightDrv));
  pDrv->Flag = SLightDrvId;
}

//----------------------ָʾ��ɨ��������------------------------------
//�������128mS������
void SLightDrv_Task(struct  _SLightDrv *pDrv)
{
  //ǿ��������ȴ���
  const struct  _SLightDrvCfg *pCfg = pDrv->pCfg;
  unsigned char SLightDrvId = pDrv->Flag & SLIGHT_DRV_ID_MASK;
  if(pCfg == NULL){
    SLightDrv_cbClrOn(SLightDrvId);
    return;
  }
  //С���ڼ���:����δ���������ڵ�״̬
  if(pDrv->IndexUnit){
    pDrv->IndexUnit--;
    if(pDrv->IndexUnit) return;
  }
  if(pDrv->Index) pDrv->Index--;//�����ڼ���
  
  unsigned char CurOn = pDrv->Flag & SLIGHT_DRV_ON;
  //������δ���
  if(pDrv->Index){
    //������ʱ,�л�����
    if(pDrv->Index > pCfg->StopUnit){
        if(CurOn) CurOn = 0;
        else CurOn = SLIGHT_DRV_ON;
    }
    //������ʱ,ֱ�ӵ���
    else CurOn = 0;
  }
  else{//��������ɸ�λ
    pDrv->Index = pCfg->StopUnit + (pCfg->FlashCount << 1);
    if(pCfg->FlashCount) CurOn = SLIGHT_DRV_ON;
    else CurOn = 0; 
  }
  
  //װ���������ڲ��õƵ�����
  if(CurOn){
    pDrv->Flag |= SLIGHT_DRV_ON;
    if(!pCfg->OnUnit) return;//��������ʱ����
    pDrv->IndexUnit = pCfg->OnUnit;
    SLightDrv_cbSetOn(SLightDrvId);
  }
  else{
    pDrv->Flag &= ~SLIGHT_DRV_ON;    
    if(!pCfg->OffUnit) return;//��������ʱ����
    pDrv->IndexUnit = pCfg->OffUnit; 
    SLightDrv_cbClrOn(SLightDrvId);
  }
}

//--------------------------ָʾ������Id�ı亯����ʵ��--------------------------
void SLightDrv_CfgChange(struct  _SLightDrv *pDrv, 
                         const struct  _SLightDrvCfg *pCfg)//NULLʱǿ�����
{
  if(pDrv->pCfg == pCfg) return ;//δ�ı�
  pDrv->pCfg = pCfg;
  //���¿�ʼ
  pDrv->IndexUnit = 0;
  pDrv->Index = 0;
}


