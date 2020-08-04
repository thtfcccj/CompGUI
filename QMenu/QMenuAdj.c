/***********************************************************************
		             菜单系统之4位数码管-公共部分实现
//此模块为所有调整实现的总接口
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"
#include "QMenuMath.h"

//----------------------更新最大调整位置函数---------------------------
//用于决定显示多少位
void QMenuAdj_UpdateMaxPos(struct _QMenuMng *pMng)
{
  
  unsigned char Pos;
  //以绝对值作为条件
  unsigned short Max,Min; 
  Max = QMenuMath_Ss2Su(pMng->Para.Max);
  Min = QMenuMath_Ss2Su(pMng->Para.Min);
  if(Min > Max) Max = Min;
  //Min = QMenuMath_Ss2Su(pMng->Para.Adj); 
  //if(Min > Max) Max = Min;
  
  if(Max >= 1000) Pos = 3; //最大4位
  else{
    if(Max >= 100) Pos = 2;
    else if(Max >= 10) Pos = 1;
    else Pos = 0;
    if(pMng->Para.Cfg2 & QMENU_CFG2_NEGATIVE) Pos++;//带符号了
  }
  pMng->WrMaxPos = Pos;
}


//---------------------重新设定新数值函数-----------------------
void QMenuAdj_SetValue(struct _QMenuMng *pMng, signed short Value)
{
  if(Value < pMng->Para.Min) Value = pMng->Para.Min;
  else if(Value > pMng->Para.Max) Value = pMng->Para.Max;
  pMng->Para.Adj = Value;          
}

//----------------------显示数值函数---------------------------
//仅更新数值显示,不负责小数点,闪动位驱动

void QMenuAdj_DispVol(struct _QMenuMng *pMng)
{
  unsigned char Flag = pMng->WrMaxPos; //小数点
  if(pMng->Para.Cfg2 & QMENU_CFG2_NEGATIVE){
    Flag--;//符号位小数点不算
    Flag |= 0x04;
    //调整模式时,正号也要带符号(实时刷新模式除外)
    if((pMng->Layer == QMENU_LAYER_WR) && !(pMng->Para.Cfg & QMENU_CFG_REAL_REFRESH))
      Flag |= 0x08;      
  }
  //专用送显
  QMenuMng_cbDispNum4(pMng->Flag & QMENU_MNG_ID_MASK,
                      pMng->Para.Adj, Flag);
}

