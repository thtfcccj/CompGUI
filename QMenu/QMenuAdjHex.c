/***********************************************************************
		         菜单系统之4位数码管-十六进制调节模式实现

//WrIndex位移值分别为:0,1,2,3, 即0为最十六进制位
//对就调整位分别为:   0,1,2,3, 即从高到低位调节
//每一个值的可调节范围由Max和Min控制,Min>=Max时不可调
//此模块为QMenuMng的内部实现,非独立使用模块
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

//-----------------------得到下个可调整位---------------------------
//>= 4表未找到
static unsigned char _GetNextWrIndex(struct _QMenuMng *pMng, 
                                       unsigned char NextIndex)
{
  unsigned short Mask = 0x0f << ((3 - NextIndex) * 4);//最高位为0
  for(; NextIndex < 4; NextIndex++, Mask >>= 4){
    if((Mask & pMng->Para.Max) > (Mask & pMng->Para.Min))
      break;
  }
  return NextIndex;
}

//-----------------------初始化函数---------------------------
void QMenuAdjHex_Init(struct _QMenuMng *pMng)
{
  pMng->WrIndex = _GetNextWrIndex(pMng, 0);
}

//-------------------------按键处理函数---------------------------
void QMenuAdjHex_Key(struct _QMenuMng *pMng, 
                       unsigned char Key)
{
  unsigned char Data;
  
  //确认键切换调整位, pMng->WrIndex表示当前调整的数值位
  if(Key & QMENU_MNG_KEY_ENTER){
    Data = _GetNextWrIndex(pMng, pMng->WrIndex + 1);
    if(Data >= 4) Data = _GetNextWrIndex(pMng, 0); //回环了
    pMng->WrIndex = Data;
    
    pMng->Flag |= QMENU_MNG_FLASH_HIDDEN; //切换时被切换位消隐
    return;
  }
  //范围不超过最大最小值定义
  Data = (3 - pMng->WrIndex) * 4;
  unsigned short Mask = 0x0f << Data; 
  unsigned short Hex = pMng->Para.Adj & Mask;
  unsigned short Min = pMng->Para.Min & Mask;
  unsigned short Max = pMng->Para.Max & Mask;
  pMng->Para.Adj &= ~Mask;  
  unsigned short Add =  1 << Data;   
  //上键调整位+1,
  if(Key & QMENU_MNG_KEY_UP){
    if(Hex == Max) Hex = Min;
    else Hex += Add;
  }
  else{//下键调整位-1
    if(Hex == Min) Hex = Max;
    else Hex -= Add;
  }
  pMng->Para.Adj |= Hex;
}

//----------------------显示更新函数---------------------------
void QMenuAdjHex_Disp(struct _QMenuMng *pMng)
{
  
  unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;  
  
  //将最大数送最左边,由小到大送
  unsigned short Data = pMng->Para.Adj;
  for(unsigned char Pos = 0; Pos < 4; Pos++, Data >>= 4){
    QMenuMng_cbSetSegDisp(MngId, 
                          Pos, 
                          QMenuMng_cbVol2Seg(MngId, Data & 0x0f));     
  }

  if(pMng->Layer == QMENU_LAYER_WR)//写状态置调整位闪动
    QMenuMng_cbSetSegFlash(MngId, QMenuMng_cbPos2Mask(MngId, 3 - pMng->WrIndex));
}

