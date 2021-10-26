/***********************************************************************

//                 蜂鸣器哔哔声通用驱动模块实现

************************************************************************/
#include "BeepDrv.h"
#include <string.h>

/****************************************************************************
                      相关结构及函数
****************************************************************************/
struct _BeepDrv BeepDrv;

//------------------------- 初始化函数---------------------
void BeepDev_Init(void)
{
  BeepDrv_cbCfgIo();
  BeepDrv.RepeatIndex = 0; //未初始化cfg,故开机不响
}

//----------------- 蜂鸣器扫描任务函数------------------
//建议放入128mS进程中
void BeepDev_Task(void)
{
  if(!BeepDrv.RepeatIndex) return;//没有开启或重复响次数到了

  //小周期计数:计数未到保持现在的状态
  if(BeepDrv.IndexUnit){
    BeepDrv.IndexUnit--;
    if(BeepDrv.IndexUnit) return;
  }

  if(BeepDrv.Index) BeepDrv.Index--;//大周期计数

  //大周期未完成
  if(BeepDrv.Index){
    //响周期时,切换响灭
    if(BeepDrv.Index > BeepDrv.Cfg.StopUnit){
        if(BeepDrv.RepeatIndex & BEEP_DRV_BEEP_ON)
          BeepDrv.RepeatIndex &= ~BEEP_DRV_BEEP_ON;
        else
          BeepDrv.RepeatIndex |= BEEP_DRV_BEEP_ON;
    }
    //灭周期时,直接到灭
    else{
      BeepDrv.RepeatIndex &= ~BEEP_DRV_BEEP_ON;
    }
  }
  else{//大周期完成复位
    //重复次数计数
    if(!(BeepDrv.Cfg.Repeat & BEEP_DRV_REPEAT_ALL)){//一直响关闭时:
      if(BeepDrv.RepeatIndex) BeepDrv.RepeatIndex--;
      if(!BeepDrv.RepeatIndex){//没有开启或重复响次数到了
        BeepDrv_cbClrOn();//强制停止
        return;
      }
    }

    BeepDrv.Index = BeepDrv.Cfg.StopUnit + (BeepDrv.Cfg.FlashCount << 1);
    if(BeepDrv.Cfg.FlashCount)
      BeepDrv.RepeatIndex |= BEEP_DRV_BEEP_ON;
    else
      BeepDrv.RepeatIndex &= ~BEEP_DRV_BEEP_ON;
  }

  //装载响灭周期并置灯的响灭
  if(BeepDrv.RepeatIndex & BEEP_DRV_BEEP_ON){//响周期
    if(!BeepDrv.Cfg.OnUnit) return;//无响周期时保持
    BeepDrv.IndexUnit = BeepDrv.Cfg.OnUnit;
    BeepDrv_cbSetOn();
  }
  else{
    if(!BeepDrv.Cfg.OffUnit) return;//无灭周期时保持
    BeepDrv.IndexUnit = BeepDrv.Cfg.OffUnit;
    BeepDrv_cbClrOn();
  }
}

//------------------ 蜂鸣器配置改变函数-----------------------
void BeepDev_CfgChange(const struct  _BeepDrvCfg *pCfg)
{
  memcpy(&BeepDrv.Cfg, pCfg, sizeof(struct  _BeepDrvCfg));
  BeepDrv.IndexUnit = 0;
  BeepDrv.Index = 0;
  BeepDrv.RepeatIndex = (BeepDrv.Cfg.Repeat & BEEP_DRV_REPEAT_MASK) + 1;
}

