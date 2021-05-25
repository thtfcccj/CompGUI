/*******************************************************************************

                 单色指示灯通用驱动模块实现
此模块专用于驱动单色指示灯以不同方式显示,也可用于驱动蜂鸣器等应用
********************************************************************************/
#include "SLightDrv.h"
#include <string.h>

//-----------------------------------初始化函数------------------------------
void SLightDrv_Init(struct  _SLightDrv *pDrv,
                     unsigned char SLightDrvId)//分配的ID号
{
  memset(pDrv, 0, sizeof(struct  _SLightDrv));
  pDrv->Flag = SLightDrvId;
}

//----------------------指示灯扫描任务函数------------------------------
//建议放入128mS进程中
void SLightDrv_Task(struct  _SLightDrv *pDrv)
{
  //强制灭灯优先处理
  const struct  _SLightDrvCfg *pCfg = pDrv->pCfg;
  unsigned char SLightDrvId = pDrv->Flag & SLIGHT_DRV_ID_MASK;
  if(pCfg == NULL){
    SLightDrv_cbClrOn(SLightDrvId);
    return;
  }
  //小周期计数:计数未到保持现在的状态
  if(pDrv->IndexUnit){
    pDrv->IndexUnit--;
    if(pDrv->IndexUnit) return;
  }
  if(pDrv->Index) pDrv->Index--;//大周期计数
  
  unsigned char CurOn = pDrv->Flag & SLIGHT_DRV_ON;
  //大周期未完成
  if(pDrv->Index){
    //亮周期时,切换亮灭
    if(pDrv->Index > pCfg->StopUnit){
        if(CurOn) CurOn = 0;
        else CurOn = SLIGHT_DRV_ON;
    }
    //灭周期时,直接到灭
    else CurOn = 0;
  }
  else{//大周期完成复位
    pDrv->Index = pCfg->StopUnit + (pCfg->FlashCount << 1);
    if(pCfg->FlashCount) CurOn = SLIGHT_DRV_ON;
    else CurOn = 0; 
  }
  
  //装载亮灭周期并置灯的亮灭
  if(CurOn){
    pDrv->Flag |= SLIGHT_DRV_ON;
    if(!pCfg->OnUnit) return;//无亮周期时保持
    pDrv->IndexUnit = pCfg->OnUnit;
    SLightDrv_cbSetOn(SLightDrvId);
  }
  else{
    pDrv->Flag &= ~SLIGHT_DRV_ON;    
    if(!pCfg->OffUnit) return;//无灭周期时保持
    pDrv->IndexUnit = pCfg->OffUnit; 
    SLightDrv_cbClrOn(SLightDrvId);
  }
}

//--------------------------指示灯配置Id改变函数宏实现--------------------------
void SLightDrv_CfgChange(struct  _SLightDrv *pDrv, 
                         const struct  _SLightDrvCfg *pCfg)//NULL时强制灭灯
{
  if(pDrv->pCfg == pCfg) return ;//未改变
  pDrv->pCfg = pCfg;
  //重新开始
  pDrv->IndexUnit = 0;
  pDrv->Index = 0;
}


