/*******************************************************************************

                 单色指示灯通用驱动模块
此模块专用于驱动单色指示灯以不同方式显示,也可用于驱动蜂鸣器等应用
此模块独立于硬件与应用
********************************************************************************/
#ifndef __SLIGHT_DRV_H
#define __SLIGHT_DRV_H

/*******************************************************************************
                               相关配置
*******************************************************************************/
//支持此模块需在全局里定义(仅供应用层使用)
//#define SUPPORT_SLIGHT 

/*******************************************************************************
                               相关结构
*******************************************************************************/
//配置结构
struct  _SLightDrvCfg{
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

struct  _SLightDrv{
  const struct  _SLightDrvCfg *pCfg; //当前挂载的结构
  unsigned char IndexUnit;           //小周期单位计时
  unsigned char Index;               //大周期计数器  
  unsigned char Flag;                //相关标志，见定义
};

//相关标志定义为：
#define  SLIGHT_DRV_ID_MASK   0x7f     //配置结构ID,主要用于回调
#define  SLIGHT_DRV_ON        0x80     //指示灯开关

/*************************************************************************
                               相关函数
************************************************************************/

//-----------------------------------初始化函数------------------------------
void SLightDrv_Init(struct  _SLightDrv *pDrv,
                     unsigned char SLightDrvId);//分配的ID号


//--------------------------指示灯配置Id改变函数------------------------------
void SLightDrv_CfgChange(struct  _SLightDrv *pDrv, 
                         const struct  _SLightDrvCfg *pCfg);//NULL时强制灭灯

//---------------------------指示灯扫描任务函数------------------------------
//建议放入128mS进程中
void SLightDrv_Task(struct  _SLightDrv *pDrv);

/*****************************************************************************
                           回调函数
******************************************************************************/

//-----------------------------------置指示灯函数--------------------------------
void SLightDrv_cbSetOn(unsigned char SLightDrvId);

//--------------------------------清指示灯函数-----------------------------------
//SLightDrvId =SLIGHT_DRV_ID_MASK时，灭所有灯
void SLightDrv_cbClrOn(unsigned char SLightDrvId);

#endif
