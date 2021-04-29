/***********************************************************************

		             菜单系统之4位数码管时-与用户的交互接口公共函数实现

***********************************************************************/

#include "QMenuPara.h"

//struct _QMenuPara _Para; //测试占位

//-------------------QMenuPara初始化函数--------------------------
void QMenuPara_Init(struct _QMenuPara *pPara)
{
  pPara->Adj = 0;      //当前调整值
  pPara->Min = 0;      //调整值的最小值,含  
  pPara->Max = 9999;   //调整值的最大值,含
  pPara->Cfg = 0;     //主配置字,每菜单项均需配置,见定义
  pPara->Cfg2 = 0;    //附加配置字,不需要时为0,见定义
}

//-------------------QMenuPara->SetData空函数----------------------
//菜单项为只读或无事件需要处理时可调用
void QMenuPara_SetNULL(struct _QMenuPara *pPara, unsigned char Type)
{

}
