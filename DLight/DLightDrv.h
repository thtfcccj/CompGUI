/***********************************************************************
                 三色指示灯通用驱动模块
原DLightDrv.h
************************************************************************/
#ifndef __DLIGHT_DRV_H
#define __DLIGHT_DRV_H


//LED驱动配置结构
struct  _DLightDrvCfg{
  unsigned char On_En;       //LED打开使能(1绿灯使能,2红灯,3黄灯)
  unsigned char OnUnit;      //亮周期单位,0时无亮周期即全灭
  unsigned char OffUnit;     //灭周期单位,0时无灭周期即全亮
  unsigned char FlashCount;  //闪亮次数,0时固定为1
  unsigned char StopUnit;    //停亮周期单位,0时停亮周期
};
//配置结构各部分作用如下(一个周期为例):
//指示灯状态:|---亮---|---灭--|---亮---|---灭--|-------灭--------|
//时间相关:  | OnUnit |OffUnit| OnUnit |OffUnit|----StopUnit-----|
//波形示意:  |--------|_______|--------|_______|_________________|
//次数相关:  |------------FlashCount = 2-------|
//注:Unit结尾是以模块固定调用周期为单位的计数值

struct  _DLightDrv{
  struct  _DLightDrvCfg Cfg;   //配置结构
  //内部变量
  unsigned char IndexUnit;   //小周期单位计时
  unsigned char Index;     //大周期计数器
  unsigned char CurOn;    //当前指示灯状态
};
extern struct _DLightDrv DLightDrv;


/*****************************************************************
                     相关函数
****************************************************************/

//-----------------指示灯扫描任务函数------------------
//建议放入128mS进程中
void DLightDrv_Task(void);

//------------------指示灯配置改变函数宏实现-----------------------
#include "string_P.h"
#define   DLightDrv_CfgChange(pCfg) \
  do{memcpy_P(&DLightDrv.Cfg,pCfg,sizeof(struct  _DLightDrvCfg));\
    DLightDrv.IndexUnit = 0;\
    DLightDrv.Index = 0;\
 }while(0)

/********************************************************
                     回调函数
******************************************************/

//------------------置指示灯函数-----------------------
void DLightDrv_cbSetOn(unsigned char DLightDrv_CurOn);

//------------------清指示灯函数-----------------------
void DLightDrv_cbClrOn(void);

//--------------------附加任务函数------------------
//在DLightDrv_Task()后调用此函数
#ifdef SUPPORT_LCD
  void DLightDrv_cbAppendTask(void);
#else
  #define DLightDrv_cbAppendTask()  do{}while(0)
#endif

#endif
