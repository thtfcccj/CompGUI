/***********************************************************************
		             菜单系统之4位数码管-按位调整实现
//此模块为QMenuMng的内部实现,非独立使用模块
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"
#include "QMenuMath.h"

//----------------得到指定位置数值函数------------------------
unsigned char _GetDigitalInPos(unsigned short Value,
                               unsigned char Pos)
{
  while(Pos--){
    Value /= 10;
  }
  return Value % 10;
}

//-----------------------初始化函数---------------------------
void QMenuAdjBit_Init(struct _QMenuMng *pMng)
{
  QMenuAdj_UpdateMaxPos(pMng);
  pMng->WrIndex = pMng->WrMaxPos; //用作调整位
}

//由调整项得到倍率
static __flash const unsigned short _Bit2Muti[4] = {1,10,100,1000};

//-------------------------按键处理函数---------------------------
void QMenuAdjBit_Key(struct _QMenuMng *pMng, unsigned char Key)
{
  unsigned short Value, Muti;
  unsigned char Vol;
  
  //确认键在调整位间切换
  if(Key & QMENU_MNG_KEY_ENTER){
    if(pMng->WrIndex) pMng->WrIndex--;
    else pMng->WrIndex = pMng->WrMaxPos; //回环到最高位了
    pMng->Flag |= QMENU_MNG_FLASH_HIDDEN; //切换时被切换位消隐
    return;
  }
  //上下键符号位调整
  if((pMng->Para.Cfg2 & QMENU_CFG2_NEGATIVE) && (pMng->WrIndex == pMng->WrMaxPos)){
    pMng->Para.Adj = 0 - pMng->Para.Adj; //反转
    return;
  }
  //上下键调整
  Value = QMenuMath_Ss2Su(pMng->Para.Adj);
  Muti = _Bit2Muti[pMng->WrIndex];//倍率数
  Vol = _GetDigitalInPos(Value,pMng->WrIndex);  //得到指定位数值
  Value -= Muti * Vol; //去掉该位了
  if(Key & QMENU_MNG_KEY_UP){
    if(Vol < 9) Vol++;
    else Vol = 0;
  }
  else{
    if(Vol) Vol--;
    else Vol =  9;
  }
  Value += Vol * Muti; //新的该位数值
  
  //设定新数值
  if(pMng->Para.Adj < 0) QMenuAdj_SetValue(pMng, 0 - Value);
  else QMenuAdj_SetValue(pMng, Value);
}

//----------------------显示更新函数---------------------------
void QMenuAdjBit_Disp(struct _QMenuMng *pMng)
{
  QMenuAdj_DispVol(pMng); //先更新值显示
  if(pMng->Layer == QMENU_LAYER_WR){//写状态置正在调整位闪动
    unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;
    QMenuMng_cbSetSegFlash(MngId, 
                           QMenuMng_cbPos2Mask(MngId, pMng->WrIndex));
  }
}

