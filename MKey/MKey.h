/*****************************************************************************

//		               多例化按键模块标准接口
//此模块为Key模块的多例化，并可与Key模块同时使用
//两种采集方式：
//SUPPORT_MKEY_ID方式获得唯一键值，不能识别组合键
//SUPPORT_MKEY_MASK方式获得每个键的按下状态，可识别组合键
****************************************************************************/
#ifndef __MKEY_H
#define __MKEY_H

/*******************************************************************************
					                  全局定义说明
********************************************************************************/

//是否支持全局加速,全局里定义
//#define SUPPORT_MKEY_ACCELERATE

/*******************************************************************************
					                  内部配置区
********************************************************************************/

#include "MKeyDef.h"        //用户的私有实现

#ifdef SUPPORT_MKEY_ID 
  #define MKeySize_t    unsigned char  //不需定义键值
  #define _INVALID_MKEY    0xff         //定义无效键值
#endif

#ifdef SUPPORT_MKEY_MASK //掩码时，不定义默认只响应5个按键
  #ifndef MKeySize_t
    #define MKeySize_t    unsigned char
  #endif
  #define _INVALID_MKEY    0         //定义无效键值
#endif

//定义在MKey_AccelerateTask()调用周期内,按键的最大次数
#define MKEY_ACCELERATE_MAX     23

//加速功能延时计数值，即判断是连续按后，还要再延时一个时间，才会进入加速
//单位为Task调用时间
#define MKEY_ACCELERATE_DELAY   (MKEY_ACCELERATE_MAX * 2)

/*******************************************************************************
					                     相关结构
*******************************************************************************/

//加速功能结构:
struct _MKeyAccelerate{
  unsigned char Gene;   //加速因子，按键加速频率和该值成正比
  unsigned char Index; //计数器
};

//按键管理器
struct _MKey{
  unsigned char AryId;   //全局分配的ID号
  MKeySize_t MKeyId;      //按键值ID 
  unsigned char Index;		//按键计数器
  #ifdef SUPPORT_MKEY_ACCELERATE  //支持按键加速时
    struct _MKeyAccelerate Accelerate;
  #endif 
};

/*******************************************************************************
					                     相关函数
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void MKey_Init(struct _MKey *pKey, unsigned char AryId);

//------------------------------------任务函数----------------------------------
//每8ms调用一次以获取键值
void MKey_Task(struct _MKey *pKey);

//----------------------------按键加速任务函数-----------------------------
//放入16或32ms进程中
#ifdef SUPPORT_MKEY_ACCELERATE  //支持按键加速时
  void MKey_AccelerateTask(struct _MKey *pKey);
#else
  #define MKey_AccelerateTask(pkey)  do{}while(0)
#endif

//----------------------------------获得最后一次键值函数------------------------
#define MKey_GetMKeyId(mKey)   ((mKey)->MKeyId)

/*******************************************************************************
					                    回调函数-底层
*******************************************************************************/
//--------------------------------初始始IO-------------------------------------
#include "IoCtrl.h" //直接实现
#define MKey_cbCfgIO(aryId)       CfgMKey(aryId)

//---------------------------------得到原始键值函数-----------------------------
//SUPPORT_MKEY_ID时需实现
unsigned char MKey_cbGetOrgMKeyId(unsigned char AryId);

//-----------------------------判断对应键是否按下-----------------------------
//SUPPORT_MKEY_MASK时需实现
unsigned char MKey_cbIsMKeySel(unsigned char AryId, unsigned char MKeySel);

//------------------------判断当前保持按键是否为加速键------------------------
//SUPPORT_MKEY_ACCELERATE时需实现,一般将上下键设置为加速键
unsigned char MKey_cbIsAccelarateMKey(unsigned char AryId, MKeySize_t MKey);

/*******************************************************************************
					                    回调函数-上层
*******************************************************************************/

//----------------------------由物理键值ID得到键值-----------------------------
//SUPPORT_MKEY_ID时需实现
MKeySize_t MKey_cbMKeyId2MKey(unsigned char AryId, MKeySize_t MKeyId);

//-------------------------------短按键通报函数---------------------------------
//用回调的方式实现按键处理
void MKey_cbShortNotify(unsigned char AryId, MKeySize_t MKey);

//-------------------------------长按键通报函数---------------------------------
//不支持时可实现为空
void MKey_cbLongNotify(unsigned char AryId, MKeySize_t MKey);

//-------------------------------保持按键通报函数------------------------------
//不支持时可实现为空
void MKey_cbKeepNotify(unsigned char AryId, MKeySize_t MKey);

#endif

