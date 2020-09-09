/***********************************************************************
		             菜单系统之4位数码管-整体调节模式实现
//此模块为QMenuMng的内部实现,非独立使用模块
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

//-------------------------按键处理函数---------------------------
void QMenuAdjAll_Key(struct _QMenuMng *pMng, unsigned char Key)
{
  //确认键无效
  if(Key & QMENU_MNG_KEY_ENTER) return;
  //上键调整
  if(Key & QMENU_MNG_KEY_UP){
    if(pMng->Para.Adj < pMng->Para.Max) pMng->Para.Adj++;
    else pMng->Para.Adj = pMng->Para.Min;//回环了
    return;
  }
  //下键调整
  if(pMng->Para.Adj > pMng->Para.Min) pMng->Para.Adj--;
  else pMng->Para.Adj = pMng->Para.Max;//回环了
}

//----------------------显示更新函数---------------------------
void QMenuAdjAll_Disp(struct _QMenuMng *pMng)
{
  QMenuAdj_DispVol(pMng); //先更新值显示
  if(pMng->Layer == QMENU_LAYER_WR){//写状态置全部调整位闪动
    QMenuMng_cbSetSegFlash(pMng->Flag & QMENU_MNG_ID_MASK, 0x0f);
    QMenuMng_cbSetDotFlash(pMng->Flag & QMENU_MNG_ID_MASK,
                           pMng->Para.Cfg2 & 0x0f);//将小数点一起置为闪动
  }
}

