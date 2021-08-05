/***********************************************************************

//		             菜单系统之4位数码管时的管理功能实现

***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

#include <string.h>

//struct _QMenuMng Mng;  //占位测试

/***********************************************************************
                    相关函数实现
***********************************************************************/

//------------------判断是否在实时显示状态------------------------
signed char QMenuMng_IsRealDisp(struct _QMenuMng *pMng)
{
  if((pMng->Layer == QMENU_LAYER_RD) && 
    (pMng->Para.Cfg & (QMENU_CFG_REAL_RD | QMENU_CFG_REAL_REFRESH))) 
    return -1;
  if((pMng->Layer == QMENU_LAYER_WR) && (pMng->Para.Cfg & QMENU_CFG_REAL_REFRESH)) 
    return -2; 
  return 0;
}

//------------------------重新装载用户数据------------------------
void QMenuMng_ReloadPara(struct _QMenuMng *pMng)
{
  QMenuPara_Init(&pMng->Para); //重新初始化
  pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para,
                                               pMng->Layer);//菜单层次
  pMng->Flag |= QMENU_MNG_REFRESH_NOW;
}

//--------------------------任务函数--------------------------
//将此函数放入系统250mS进程中,用于更新实时值的显示
void QMenuMng_Task(struct _QMenuMng *pMng)
{
  if(QMenuMng_IsRealDisp(pMng)){
    //开始实时更新
    pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para, QMENU_CFG_REAL_RD); 
    QMenuMng_UpdateDisp(pMng); //更新显示
  }
  //仅更新
  else if(pMng->Flag & QMENU_MNG_REFRESH_NOW){
    pMng->Flag &= ~QMENU_MNG_REFRESH_NOW;
    QMenuMng_UpdateDisp(pMng); //更新显示
  }
}

//----------------------进入菜单初始化函数--------------------------
//由主界面准备进入菜单时调用挂接的菜单查找表并完成内部初始化
void QMenuMng_EnterInit(struct _QMenuMng *pMng, 
                        const struct _QMenuFun * const *pFunAry,//不能为NULL
                        const unsigned char *pLUT)//不能为NULL
{
  unsigned char Id = pMng->Flag & QMENU_MNG_ID_MASK;
  memset(pMng, 0, sizeof(struct _QMenuMng));
  pMng->Flag = Id;
  pMng->pFunAry = pFunAry;
  pMng->pLUT = pLUT;
}

//-------------------------得到当前菜单ID号--------------------
unsigned char QMenuMng_GetId(const struct _QMenuMng *pMng)
{
  return  *(pMng->pLUT + pMng->Index + 1);
}

//----------------------更新菜单显示函数--------------------------
//调用此函数主动更新界面显示
void QMenuMng_UpdateDisp(struct _QMenuMng *pMng)
{
  unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;
  
  //统一初始化显示部分(不管数码管的显示)
  QMenuMng_cbClrSegFlash(MngId);
  QMenuMng_cbClrDotFlash(MngId);
  QMenuMng_cbClrDotDisp(MngId);  
  
  if(pMng->Layer == QMENU_LAYER_SEL){//直接用菜单项符号更新显示(含小数点)
    #ifdef SUPPORT_QMENU_PIXEL   //支持像素化时交给外部实现
      QMenuMng_cbUpdateDispMenuSel(pMng);
    #else //4位数码管时，依次更新菜单提示的显示
      const unsigned char *pNote = pMng->pFunAry[QMenuMng_GetId(pMng)]->Note;
      for(unsigned char Pos = 0; Pos < 4; Pos++){
        unsigned char Data = *pNote++;
        QMenuMng_cbSetSegDisp(MngId, Pos, Data); 
        if(Data & QMenuMng_cbGetDotMask(MngId)) 
          QMenuMng_cbSetDotDisp(MngId, QMenuMng_cbPos2Mask(MngId, Pos));
      }
    #endif
  }
  else{
    //根据调整方式调用相关模块显示
    switch(pMng->Para.Cfg & QMENU_CFG_ADJ_MASK){
    case QMENU_CFG_ADJ_BIT: 
    case QMENU_CFG_ADJ_PASS:     
      QMenuAdjBit_Disp(pMng); 
      QMenuMng_cbSetDotDisp(MngId, pMng->Para.Cfg2 & QMENU_CFG2_DOT_MASK);//手动打上小数点
      break; 
    case QMENU_CFG_ADJ_ALL: 
    case QMENU_CFG_ADJ_CMD:       
      QMenuAdjAll_Disp(pMng); 
      QMenuMng_cbSetDotDisp(MngId, pMng->Para.Cfg2 & QMENU_CFG2_DOT_MASK);//手动打上小数点
      break;
    #ifdef SUPPORT_QMENU_ADJ_HEX
      case QMENU_CFG_ADJ_HEX: QMenuAdjHex_Disp(pMng); 
      QMenuMng_cbSetDotDisp(MngId, pMng->Para.Cfg2 & QMENU_CFG2_DOT_MASK);//手动打上小数点
      break;
    #endif
    //case QMENU_CFG_ADJ_LOGIC:
    default:QMenuAdjLogic_Disp(pMng); break;
    }
  }
  QMenuMng_cbForceFlash(MngId, pMng->Flag & QMENU_MNG_FLASH_HIDDEN); //强制更新
  pMng->Flag &= ~QMENU_MNG_FLASH_HIDDEN;
}

/***********************************************************************
                    按键处理相关函数实现
***********************************************************************/

//---------------------进入读写状态初始化函数--------------------------
void _EnterRdWr(struct _QMenuMng *pMng, unsigned char IsWr)
{
  if(IsWr){
    pMng->Layer = QMENU_LAYER_WR;
  }
  else pMng->Layer = QMENU_LAYER_RD;
  
  pMng->WrIndex = 0;
  pMng->OrgAdj = pMng->Para.Adj;
  
  //根据调整方式初始化相关模块
  switch(pMng->Para.Cfg & QMENU_CFG_ADJ_MASK){
    case QMENU_CFG_ADJ_PASS:    
    case QMENU_CFG_ADJ_BIT: QMenuAdjBit_Init(pMng); break;
    case QMENU_CFG_ADJ_CMD: pMng->OrgAdj++;//让命令模式可写
    case QMENU_CFG_ADJ_ALL: QMenuAdjAll_Init(pMng); break;
    #ifdef SUPPORT_QMENU_ADJ_HEX
      case QMENU_CFG_ADJ_HEX: QMenuAdjHex_Init(pMng); break;
    #endif
    //case QMENU_CFG_ADJ_LOGIC:
    default:QMenuAdjLogic_Init(pMng); break;
  }
}
//---------------------退出写状态处理函数--------------------------
//形参为写状态:0:禁止写,其它:传递给".SetData"的形参
void _ExitWr(struct _QMenuMng *pMng, unsigned char WrState)
{
  //保存退出时,若数据未改变,则需根据退出保存进一步处理
  if((WrState == QMENU_LAYER_WR) && (pMng->OrgAdj == pMng->Para.Adj)){
    if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT_SAVE) //退出时也保存时,调用退出保存
      WrState = QMENU_CFG2_QUIT_SAVE;
    else WrState = 0;//退出不保存
  }
  if(WrState != 0){//允许保存时
    unsigned char Id = QMenuMng_GetId(pMng); //PIC这要才管用
    pMng->pFunAry[Id]->SetData(&pMng->Para, WrState); //写数据    
  }  
  if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT){ //直接退出菜单
    pMng->Layer = QMENU_LAYER_NULL;
    return;
  }
  
  pMng->pFunAry[QMenuMng_GetId(pMng)]->
    GetData(&pMng->Para, QMENU_LAYER_SEL);//更新一下数据获得写结果或更新
  if(pMng->Para.Cfg & QMENU_CFG_WR) //只写时直接退至选择状态
    pMng->Layer = QMENU_LAYER_SEL;
  else
    _EnterRdWr(pMng, 0); //退出到读状态并初始化
}

//---------------------菜单选择状态按键处理函数--------------------------
void _SelKey(struct _QMenuMng *pMng, unsigned char Key)
{
  //响应按键处理
  if(Key & QMENU_MNG_KEY_LONG) return; //不响应长按键
  
  if(Key == (QMENU_MNG_KEY_EXIT | QMENU_MNG_KEY_SHORT)){//短按退出菜单
    pMng->Layer = QMENU_LAYER_NULL;
    return;
  }
  
  if(Key == (QMENU_MNG_KEY_ENTER | QMENU_MNG_KEY_SHORT)){//短按进入菜单
    if(pMng->Para.Cfg & QMENU_CFG_WR){ //直接进入写状态
      pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para, QMENU_LAYER_WR);//写数据准备
      _EnterRdWr(pMng, 1);//进入写状态
    }
    else _EnterRdWr(pMng, 0);//进入读状态
    return;
  }
  //菜单项加减
  if(Key & QMENU_MNG_KEY_UP){//菜单项加
    pMng->Index++;
    if(pMng->Index >= *pMng->pLUT) pMng->Index = 0;//回环了
  }
  else if(Key & QMENU_MNG_KEY_DOWN){//菜单项减
    if(!pMng->Index) pMng->Index = *pMng->pLUT - 1;//回环了
    else pMng->Index--;
  }
  //else return; //保存与退出保持时
  
  QMenuPara_Init(&pMng->Para); //重新初始化
  pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para,
                                          pMng->Layer);//菜单选择状态预读数据准备
}

//---------------------菜单参数显示(读)状态按键处理函数--------------------------
void _RdKey(struct _QMenuMng *pMng, unsigned char Key)
{
  /*/只读时,长保存键,若用户有保存函数,则调用该函数执行保存功能
  if((Key & (QMENU_MNG_KEY_LONG | QMENU_MNG_KEY_ENTER)) ==
     (QMENU_MNG_KEY_LONG | QMENU_MNG_KEY_ENTER)){
    if(pMng->Para.Cfg & QMENU_CFG_RD){//只读时允许进入   
      if(pMng->pFunAry[QMenuMng_GetId(pMng)].SetData != NULL){
        pMng->pFunAry[QMenuMng_GetId(pMng)].SetData(&pMng->Para, QMENU_CFG_WR);//写数据
        pMng->pFunAry[QMenuMng_GetId(pMng)].GetData(&pMng->Para, QMENU_LAYER_SEL);//更新一下数据获得写结果
        pMng->Layer = QMENU_LAYER_SEL;//退到上层菜单
      }
    }
    return;
  }*/ //已用 QMENU_CFG_ADJ_CMD功能代替
  //不响应长按与保持按键
  if(Key & (QMENU_MNG_KEY_LONG | QMENU_MNG_KEY_KEEP)) return;

  if(Key & QMENU_MNG_KEY_ENTER){//短按确认键进入写状态
    if(!(pMng->Para.Cfg & QMENU_CFG_RD)){//非只读时允许进入
      pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para, QMENU_LAYER_WR);//写数据准备
      _EnterRdWr(pMng, 1);//进入写状态
    }
  }
  else if(Key & QMENU_MNG_KEY_EXIT){//短按退出键退出显示状态
    pMng->Layer = QMENU_LAYER_SEL;
  }
  else{ //上下键
    //将逻辑时的上下键解释为翻页显示
    if((pMng->Para.Cfg & QMENU_CFG_ADJ_MASK) == QMENU_CFG_ADJ_LOGIC){
      QMenuAdjLogic_RdPageKey(pMng);
    }
  }
}

//---------------------菜单参数调整(写)状态按键处理函数-------------------------
void _WrKey(struct _QMenuMng *pMng, unsigned char Key)
{
  if(Key & QMENU_MNG_KEY_LONG){//长按键
    if(Key & QMENU_MNG_KEY_ENTER) _ExitWr(pMng, QMENU_LAYER_WR);//长按确认键保存并退出
    return;
  }
  if(Key & QMENU_MNG_KEY_EXIT){//短按退出菜单
    if(Key & QMENU_MNG_KEY_SHORT){
      if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT_SAVE) //退出时也保存
        _ExitWr(pMng, QMENU_CFG2_QUIT_SAVE);
      else _ExitWr(pMng, 0);//退出时不保存
    }
    return;
  }
  
  //密码输入模式时, 在调整最后一位时短按确认键，将触发一次调用SetData
  if((Key & QMENU_MNG_KEY_ENTER) && (pMng->WrIndex == 0) && 
     ((pMng->Para.Cfg & QMENU_CFG_ADJ_MASK) == QMENU_CFG_ADJ_PASS)){
    unsigned char Id = QMenuMng_GetId(pMng); //PIC这要才管用
    pMng->pFunAry[Id]->SetData(&pMng->Para, QMENU_CFG_WR);  
  }
  
  //其它按键根据调整方式决定
  switch(pMng->Para.Cfg & QMENU_CFG_ADJ_MASK){
    case QMENU_CFG_ADJ_PASS:     
    case QMENU_CFG_ADJ_BIT: QMenuAdjBit_Key(pMng, Key); break;
    case QMENU_CFG_ADJ_ALL: QMenuAdjAll_Key(pMng, Key); break;
    case QMENU_CFG_ADJ_LOGIC: QMenuAdjLogic_Key(pMng, Key); break;
    #ifdef SUPPORT_QMENU_ADJ_HEX
      case QMENU_CFG_ADJ_HEX: QMenuAdjHex_Key(pMng, Key); break;
    #endif
    
    //case QMENU_CFG_ADJ_CMD:      //左右按键无效
    default: break;
  }
  
  if(pMng->Para.Cfg & QMENU_CFG_REAL_WR){//实时写时,数据变化写数据
    if(Key & (QMENU_MNG_KEY_UP | QMENU_MNG_KEY_DOWN)){
      unsigned char Id = QMenuMng_GetId(pMng); //PIC这要才管用
      pMng->pFunAry[Id]->SetData(&pMng->Para, QMENU_CFG_REAL_WR); //实时写数据
    }
  }
}

//--------------------------快速任务函数--------------------------
//将此函数放入系统快速进程中,用于处理键值
void QMenuMng_FastTask(struct _QMenuMng *pMng)
{
  unsigned char Key = pMng->Key;
  if(!Key) return; //无键值
  pMng->Key = 0;
  
  unsigned char PrvLayer = pMng->Layer;
  switch(pMng->Layer){
  case QMENU_LAYER_NULL: //不在菜单系统
    if(Key != QMENU_MNG_KEY_ENTER) break; //
    pMng->Layer++; //进入
    Key = 0;//继续往下初始化处理,但非按键
  case QMENU_LAYER_SEL: //菜单系统选择:不响应长按键
    _SelKey(pMng, Key);
    break;
  case QMENU_LAYER_RD: //菜单显示数据状态
   _RdKey(pMng, Key);
    break;
  case QMENU_LAYER_WR: //菜单调整数据状态
   _WrKey(pMng, Key);
    break;
  default: //异常退出
    pMng->Layer = QMENU_LAYER_NULL;
    break;
  }
  if(pMng->Layer != QMENU_LAYER_NULL) QMenuMng_UpdateDisp(pMng);//更新显示
  else if(PrvLayer != QMENU_LAYER_NULL){//退出菜单系统
    QMenuMng_cbExitNotify(pMng->Flag & QMENU_MNG_ID_MASK); //最后退出菜单系统通报
  }
}

//----------------------退出菜单函数--------------------------
//在菜单状态,外部强制退出菜单时调用此函数
void QMenuMng_Exit(struct _QMenuMng *pMng)
{
  if(pMng->Layer == QMENU_LAYER_NULL) return; 
  
  if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT_SAVE){ //退出时也保存(直接调用用户以减少堆栈)
    pMng->pFunAry[QMenuMng_GetId(pMng)]->SetData(&pMng->Para, 
                                            QMENU_CFG2_QUIT_SAVE); //退出时写数据 
    pMng->Layer = QMENU_LAYER_NULL; //退出菜单
  }
  QMenuMng_cbExitNotify(pMng->Flag & QMENU_MNG_ID_MASK); //最后退出菜单系统通报
}  
//----------------------------初始化函数----------------------------
//开机时调用
void QMenuMng_Init(struct _QMenuMng *pMng,
                    unsigned char Id) //分配的ID号
{
  memset(pMng, 0, sizeof(struct _QMenuMng));
  pMng->Flag = Id;
}

/***********************************************************************
		                       QMenuPara相关回调函数实现
***********************************************************************/
#include "struct.h" //struct_get()

//---------------------DataSet实现中置退出菜单函数-----------------------
//仅可用于应用层在实现SetData函数的内部调用(内部不调用退出回调函数以防止嵌套)
void QMenuPara_cbQuitMenuInSet(struct _QMenuPara *pPara)
{
  struct _QMenuMng *pMng = struct_get(pPara, struct _QMenuMng, Para);
  pMng->Layer = QMENU_LAYER_NULL;
}


