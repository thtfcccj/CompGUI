/***********************************************************************
                 ��ɫָʾ��ͨ������ģ��
ԭDLightDrv.h
************************************************************************/
#ifndef __DLIGHT_DRV_H
#define __DLIGHT_DRV_H


//LED�������ýṹ
struct  _DLightDrvCfg{
  unsigned char On_En;       //LED��ʹ��(1�̵�ʹ��,2���,3�Ƶ�)
  unsigned char OnUnit;      //�����ڵ�λ,0ʱ�������ڼ�ȫ��
  unsigned char OffUnit;     //�����ڵ�λ,0ʱ�������ڼ�ȫ��
  unsigned char FlashCount;  //��������,0ʱ�̶�Ϊ1
  unsigned char StopUnit;    //ͣ�����ڵ�λ,0ʱͣ������
};
//���ýṹ��������������(һ������Ϊ��):
//ָʾ��״̬:|---��---|---��--|---��---|---��--|-------��--------|
//ʱ�����:  | OnUnit |OffUnit| OnUnit |OffUnit|----StopUnit-----|
//����ʾ��:  |--------|_______|--------|_______|_________________|
//�������:  |------------FlashCount = 2-------|
//ע:Unit��β����ģ��̶���������Ϊ��λ�ļ���ֵ

struct  _DLightDrv{
  struct  _DLightDrvCfg Cfg;   //���ýṹ
  //�ڲ�����
  unsigned char IndexUnit;   //С���ڵ�λ��ʱ
  unsigned char Index;     //�����ڼ�����
  unsigned char CurOn;    //��ǰָʾ��״̬
};
extern struct _DLightDrv DLightDrv;


/*****************************************************************
                     ��غ���
****************************************************************/

//-----------------ָʾ��ɨ��������------------------
//�������128mS������
void DLightDrv_Task(void);

//------------------ָʾ�����øı亯����ʵ��-----------------------
#include "string_P.h"
#define   DLightDrv_CfgChange(pCfg) \
  do{memcpy_P(&DLightDrv.Cfg,pCfg,sizeof(struct  _DLightDrvCfg));\
    DLightDrv.IndexUnit = 0;\
    DLightDrv.Index = 0;\
 }while(0)

/********************************************************
                     �ص�����
******************************************************/

//------------------��ָʾ�ƺ���-----------------------
void DLightDrv_cbSetOn(unsigned char DLightDrv_CurOn);

//------------------��ָʾ�ƺ���-----------------------
void DLightDrv_cbClrOn(void);

//--------------------����������------------------
//��DLightDrv_Task()����ô˺���
#ifdef SUPPORT_LCD
  void DLightDrv_cbAppendTask(void);
#else
  #define DLightDrv_cbAppendTask()  do{}while(0)
#endif

#endif
