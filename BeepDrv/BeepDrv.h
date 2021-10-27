/***********************************************************************

//                 蜂鸣器哔哔声通用驱动模块
此模块通过长短及间隔时间配合，可通过蜂鸣器哔哔声发出不同状态的声音
************************************************************************/
#ifndef __BEEP_DEV_H
#define __BEEP_DEV_H

/*********************************************************************
                          相关配置
***********************************************************************/
//外部支持此模块时建议的定义
//#define SUPPORT_BEEP_DRV     //支持蜂鸣器时 

/*********************************************************************
                          相关结构
***********************************************************************/
//BEEP驱动配置结构
struct  _BeepDrvCfg{
  unsigned char Repeat;       //BEEP重复响次数,见定义
  unsigned char OnUnit;      //响周期单位,0时无响周期即全灭
  unsigned char OffUnit;     //灭周期单位,0时无灭周期即全响
  unsigned char FlashCount;  //重响次数,0时固定为1
  unsigned char StopUnit;    //停响周期单位,0时停响周期
};
//配置结构各部分作用如下(一个周期为例):
// 蜂鸣器状态:|---响---|---灭--|---响---|---灭--|-------灭--------|
//时间相关:  | OnUnit |OffUnit| OnUnit |OffUnit|----StopUnit-----|
//波形示意:  |--------|_______|--------|_______|_________________|
//次数相关:  |------------FlashCount = 2-------|
//注:Unit结尾是以模块固定调用周期为单位的计数值

//Repeat位定义为:
#define BEEP_DRV_REPEAT_ALL     0x80    //定义为一直响下去
#define BEEP_DRV_REPEAT_MASK    0x7f    //定义停止响前,重复次数,<=125

struct  _BeepDrv{
  struct  _BeepDrvCfg Cfg;   //配置结构
  //内部变量
  unsigned char IndexUnit;       //小周期单位计时
  unsigned char Index;           //大周期计数器
  unsigned char RepeatIndex;    //重复次数,最高位用于表示响灭状态
};

extern struct _BeepDrv BeepDrv;

//RepeatIndex定义为:
#define BEEP_DRV_BEEP_ON    0x80    //RepeatIndex最高位用于表示响灭状态

/*************************************************************
                     相关函数
***********************************************************/
//------------------------- 初始化函数---------------------
void BeepDev_Init(void);

//-------------------- 蜂鸣器扫描任务函数-----------------------
//建议放入128mS进程中
void BeepDev_Task(void);

//------------------ 蜂鸣器配置改变函数-----------------------
void BeepDev_CfgChange(const struct  _BeepDrvCfg *pCfg);

/**************************************************************
                     回调函数
***************************************************************/
#include "IoCtrl.h"
//--------------------------初始化声音IO----------------------
#define BeepDrv_cbCfgIo() CfgBeep()

//-----------------------开声音--------------------
#define BeepDrv_cbSetOn() OnBeep()

//-----------------------关声音--------------------
#define BeepDrv_cbClrOn() OffBeep()

#endif
