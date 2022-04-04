/*******************************************************************************

                                UI之页面管理实现

********************************************************************************/
#include "UiPage.h"
#include <string.h>

struct _UiPage UiPage; //直接静态定义
/******************************************************************************
                              相关函数实现
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void UiPage_Init(void)
{
  memset(&UiPage, 0, sizeof(struct _UiPage));
}

//---------------------------------刷新函数-----------------------------------
void UiPage_Refresh(void)
{
  UiPage_cbUpdateUi(0x03); 
}
//---------------------------------更新本页名称--------------------------------
void UiPage_UpdateName(const char *pName)
{
  UiPage.pName = pName;
  UiPage_cbUpdateUi(0x03); //更新所有
}

//---------------------------------更新为无页码-----------------------------
//即只有一页，不面要显示页码
void UiPage_UpdateNoPage(void)
{
  UiPage.Count = 0;
  UiPage_cbUpdateUi(0x03); //更新所有
}

//---------------------------------更新为位置-------------------------------
void UiPage_UpdatePos(unsigned short Cur,        //当页起始位置
                      unsigned short Count)      //总行
{
  UiPage.Cur = Cur;
  UiPage.Count = Count;
  UiPage_cbUpdateUi(0x01);
}

//--------------------------------更新为页信息----------------------------------
void UiPage_UpdatePage(unsigned short Start,       //当页起始行
                       unsigned short Count,      //总行
                       unsigned char PageLine)    //每页行数
{
  UiPage.Cur = (Start / PageLine) + 1;
  if(Count == 0) UiPage.Count = 1;
  else UiPage.Count =  ((Count - 1) / PageLine) + 1; 
  UiPage_cbUpdateUi(0x01);  
}





