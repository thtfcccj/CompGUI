/***********************************************************************
                 三色指示灯通用驱动模块
原DLightDrv.c
************************************************************************/

#include "DLightDrv.h"

struct _DLightDrv DLightDrv;

//-----------------指示灯扫描任务函数------------------
//建议放入128mS进程中
void DLightDrv_Task(void)
{
  //小周期计数:计数未到保持现在的状态
  if(DLightDrv.IndexUnit){
    DLightDrv.IndexUnit--;
    if(DLightDrv.IndexUnit) return;
  }
  
  if(DLightDrv.Index) DLightDrv.Index--;//大周期计数
  
  //大周期未完成
  if(DLightDrv.Index){
    //亮周期时,切换亮灭
    if(DLightDrv.Index > DLightDrv.Cfg.StopUnit){
        if(DLightDrv.CurOn) DLightDrv.CurOn = 0;
        else
          DLightDrv.CurOn = DLightDrv.Cfg.On_En;
    }
    //灭周期时,直接到灭
    else DLightDrv.CurOn = 0;
  }
  else{//大周期完成复位
    DLightDrv.Index = DLightDrv.Cfg.StopUnit + (DLightDrv.Cfg.FlashCount << 1);
    if(DLightDrv.Cfg.FlashCount) DLightDrv.CurOn = DLightDrv.Cfg.On_En;
    else DLightDrv.CurOn = 0; 
  }
  
  //装载亮灭周期并置灯的亮灭
  if(DLightDrv.CurOn){
    if(!DLightDrv.Cfg.OnUnit) goto __End;//无亮周期时保持
    DLightDrv.IndexUnit = DLightDrv.Cfg.OnUnit;
    DLightDrv_cbSetOn(DLightDrv.CurOn);
  }
  else{
    if(!DLightDrv.Cfg.OffUnit) goto __End;//无灭周期时保持
    DLightDrv.IndexUnit = DLightDrv.Cfg.OffUnit; 
    DLightDrv_cbClrOn();
  }
__End:
  DLightDrv_cbAppendTask(); //附加任务
}
