/*******************************************************************************

                                UI֮ҳ�����ʵ��

********************************************************************************/
#include "UiPage.h"
#include <string.h>

struct _UiPage UiPage; //ֱ�Ӿ�̬����
/******************************************************************************
                              ��غ���ʵ��
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void UiPage_Init(void)
{
  memset(&UiPage, 0, sizeof(struct _UiPage));
}

//---------------------------------ˢ�º���-----------------------------------
void UiPage_Refresh(void)
{
  UiPage_cbUpdateUi(0x03); 
}
//---------------------------------���±�ҳ����--------------------------------
void UiPage_UpdateName(const char *pName)
{
  UiPage.pName = pName;
  UiPage_cbUpdateUi(0x03); //��������
}

//---------------------------------����Ϊ��ҳ��-----------------------------
//��ֻ��һҳ������Ҫ��ʾҳ��
void UiPage_UpdateNoPage(void)
{
  UiPage.Count = 0;
  UiPage_cbUpdateUi(0x03); //��������
}

//---------------------------------����Ϊλ��-------------------------------
void UiPage_UpdatePos(unsigned short Cur,        //��ҳ��ʼλ��
                      unsigned short Count)      //����
{
  UiPage.Cur = Cur;
  UiPage.Count = Count;
  UiPage_cbUpdateUi(0x01);
}

//--------------------------------����Ϊҳ��Ϣ----------------------------------
void UiPage_UpdatePage(unsigned short Start,       //��ҳ��ʼ��
                       unsigned short Count,      //����
                       unsigned char PageLine)    //ÿҳ����
{
  UiPage.Cur = (Start / PageLine) + 1;
  if(Count == 0) UiPage.Count = 1;
  else UiPage.Count =  ((Count - 1) / PageLine) + 1; 
  UiPage_cbUpdateUi(0x01);  
}





