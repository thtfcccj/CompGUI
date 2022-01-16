/***************************************************************************

                    频率可控蜂鸣器模块底层驱动接口
//注：频率范围较宽，输出的频率也可供扬声器使用(引脚个数由内部实现，注意电路功率)
****************************************************************************/
#ifndef _BUZZER_DRV_H
#define _BUZZER_DRV_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***************************************************************************
                          相关配置
****************************************************************************/
//#define SUPPORT_DIS_BUZZER_DRV      //不支持时定义

/***************************************************************************
                          相关结构
****************************************************************************/

//此模块无相关结构

/***************************************************************************
                          相关函数
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerDrv_Init(void);

//-----------------------------启动函数-------------------------------------
//形参为频率,0表示暂时不响
//注：频率范围与分辨率与硬件相关,注意分辨率的不连续性对声音的影响
void BuzzerDrv_Start(unsigned short Hz);

//-----------------------------停止函数-------------------------------------
void BuzzerDrv_Stop(void);

//-----------------------------是否开启-------------------------------------
unsigned char BuzzerDrv_IsStart(void);

/***************************************************************************
                            回调函数
****************************************************************************/



#endif //_BUZZER_DRV_H
