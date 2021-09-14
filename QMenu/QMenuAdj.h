/***********************************************************************
		             菜单系统之4位数码管-调整子模块接口
//此模块为所有调整实现的总接口
***********************************************************************/

#ifndef	_QMENU_ADJ_H
#define	_QMENU_ADJ_H
#include "QMenuMng.h"

/***********************************************************************
		                          公共部分
***********************************************************************/

//----------------------更新最大调整位置函数---------------------------
//用于决定显示多少位
void QMenuAdj_UpdateMaxPos(struct _QMenuMng *pMng);

//---------------------重新设定新数值函数-----------------------
void QMenuAdj_SetValue(struct _QMenuMng *pMng, signed short Value);

//----------------------显示数值函数---------------------------
//仅更新数值显示,不负责小数点,闪动位驱动
void QMenuAdj_DispVol(struct _QMenuMng *pMng);

/***********************************************************************
		                    子模块-按位调节模式
***********************************************************************/
//-----------------------初始化函数---------------------------
void QMenuAdjBit_Init(struct _QMenuMng *pMng);

//-------------------------按键处理函数---------------------------
void QMenuAdjBit_Key(struct _QMenuMng *pMng, unsigned char Key);

//----------------------显示更新函数---------------------------
void QMenuAdjBit_Disp(struct _QMenuMng *pMng);

/***********************************************************************
		                    子模块-整体调节模式
***********************************************************************/
//-----------------------初始化函数---------------------------
//void QMenuAdjAll_Init(void);
#define QMenuAdjAll_Init(mng) do{QMenuAdj_UpdateMaxPos(mng);}while(0)

//-------------------------按键处理函数---------------------------
void QMenuAdjAll_Key(struct _QMenuMng *pMng, unsigned char Key);

//----------------------显示更新函数---------------------------
void QMenuAdjAll_Disp(struct _QMenuMng *pMng);

/***********************************************************************
		                    子模块-逻辑数调节模式
***********************************************************************/
//-----------------------初始化函数---------------------------
void QMenuAdjLogic_Init(struct _QMenuMng *pMng);

//-----------------读数据时的上下键翻页处理函数---------------------------
void QMenuAdjLogic_RdPageKey(struct _QMenuMng *pMng);

//-------------------------按键处理函数---------------------------
void QMenuAdjLogic_Key(struct _QMenuMng *pMng, unsigned char Key);

//----------------------显示更新函数---------------------------
void QMenuAdjLogic_Disp(struct _QMenuMng *pMng);

/***********************************************************************
		                    子模块-十六进制调节模式
***********************************************************************/
#ifdef SUPPORT_QMENU_ADJ_HEX //可选子模式

//-----------------------初始化函数---------------------------
void QMenuAdjHex_Init(struct _QMenuMng *pMng);

//-------------------------按键处理函数---------------------------
void QMenuAdjHex_Key(struct _QMenuMng *pMng, unsigned char Key);

//----------------------显示更新函数---------------------------
void QMenuAdjHex_Disp(struct _QMenuMng *pMng);

#endif


#endif



