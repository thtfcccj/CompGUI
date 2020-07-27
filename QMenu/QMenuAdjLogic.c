/***********************************************************************
		             菜单系统之4位数码管-逻辑数调节模式实现
//最低位逻辑值,位于高位显示
//WrIndex位移值分别为:3,2,1,0, 7,6,5,4
//对就调整位分别为:   0,1,2,3, 4,5,6,7
//逻辑数的可调节位由Max位控制,置1时表示可调节,否则只读
//此模块为QMenuMng的内部实现,非独立使用模块
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

//-----------------------得到下个可调整位---------------------------
//>= 8表未找到
static unsigned char _GetNextWrIndex(struct _QMenuMng *pMng, 
                                     unsigned char NextIndex)
{
  for(; NextIndex < 8; NextIndex++)
    if((1 << NextIndex) & pMng->Para.Max) break;
  return NextIndex;
}

//-----------------------初始化函数---------------------------
void QMenuAdjLogic_Init(struct _QMenuMng *pMng)
{
  pMng->WrIndex = _GetNextWrIndex(pMng, 0);
}

//-------------------------按键处理函数---------------------------
void QMenuAdjLogic_Key(struct _QMenuMng *pMng, 
                       unsigned char Key)
{
  unsigned char Data;
  
  //确认键切换调整位, pMng->WrIndex表示当前调整的数值位
  if(Key & QMENU_MNG_KEY_ENTER){
    Data = _GetNextWrIndex(pMng, pMng->WrIndex + 1);
    if(Data >= 8) Data = _GetNextWrIndex(pMng, 0); //回环了
    pMng->WrIndex = Data;
    
    pMng->Flag |= QMENU_MNG_FLASH_HIDDEN; //切换时被切换位消隐
    return;
  }
  
  //上下键切换逻辑
  Data = 1 << pMng->WrIndex;
  if(pMng->Para.Adj & Data) pMng->Para.Adj &= ~Data;
  else pMng->Para.Adj |= Data;
}

//-----------------读数据时的上下键翻页处理函数---------------------------
void QMenuAdjLogic_RdPageKey(struct _QMenuMng *pMng)
{
  unsigned char Data = _GetNextWrIndex(pMng, pMng->WrIndex + 4);
  if(Data >= 8) Data = _GetNextWrIndex(pMng,0); //回环了
  pMng->WrIndex = Data;
}

//----------------------显示更新函数---------------------------
void QMenuAdjLogic_Disp(struct _QMenuMng *pMng)
{
  unsigned char Pos,IsHi;
  unsigned char Data = pMng->Para.Adj;
  
  if(pMng->WrIndex >= 4) Data >>= 4;//显示高位值及符号
    unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;
  
  //将最小数送最左边
  for(Pos = 0; Pos < 4; Pos++){ //判断数值
    if(Data & (1 << Pos)) IsHi = 1;
    else  IsHi = 0;
    QMenuMng_cbSetSegDisp(MngId, 3 - Pos, QMenuMng_cbVol2Seg(MngId, IsHi));     
  }
  
  if(pMng->WrIndex >= 4){
    Pos = pMng->WrIndex - 4;
    IsHi = 0x02;
  }
  else{
    Pos = pMng->WrIndex;
    IsHi = 0x08;    
  }
  QMenuMng_cbSetDotDisp(MngId, IsHi); //置小数点表示在第2页
  
  if(pMng->Layer == QMENU_LAYER_WR)//写状态置调整位闪动
    QMenuMng_cbSetSegFlash(MngId, QMenuMng_cbPos2Mask(MngId, 3 - Pos));
}

