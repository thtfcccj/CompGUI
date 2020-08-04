/***********************************************************************
		             �˵�ϵͳ֮4λ�����-��������ʵ��
//��ģ��Ϊ���е���ʵ�ֵ��ܽӿ�
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"
#include "QMenuMath.h"

//----------------------����������λ�ú���---------------------------
//���ھ�����ʾ����λ
void QMenuAdj_UpdateMaxPos(struct _QMenuMng *pMng)
{
  
  unsigned char Pos;
  //�Ծ���ֵ��Ϊ����
  unsigned short Max,Min; 
  Max = QMenuMath_Ss2Su(pMng->Para.Max);
  Min = QMenuMath_Ss2Su(pMng->Para.Min);
  if(Min > Max) Max = Min;
  //Min = QMenuMath_Ss2Su(pMng->Para.Adj); 
  //if(Min > Max) Max = Min;
  
  if(Max >= 1000) Pos = 3; //���4λ
  else{
    if(Max >= 100) Pos = 2;
    else if(Max >= 10) Pos = 1;
    else Pos = 0;
    if(pMng->Para.Cfg2 & QMENU_CFG2_NEGATIVE) Pos++;//��������
  }
  pMng->WrMaxPos = Pos;
}


//---------------------�����趨����ֵ����-----------------------
void QMenuAdj_SetValue(struct _QMenuMng *pMng, signed short Value)
{
  if(Value < pMng->Para.Min) Value = pMng->Para.Min;
  else if(Value > pMng->Para.Max) Value = pMng->Para.Max;
  pMng->Para.Adj = Value;          
}

//----------------------��ʾ��ֵ����---------------------------
//��������ֵ��ʾ,������С����,����λ����

void QMenuAdj_DispVol(struct _QMenuMng *pMng)
{
  unsigned char Flag = pMng->WrMaxPos; //С����
  if(pMng->Para.Cfg2 & QMENU_CFG2_NEGATIVE){
    Flag--;//����λС���㲻��
    Flag |= 0x04;
    //����ģʽʱ,����ҲҪ������(ʵʱˢ��ģʽ����)
    if((pMng->Layer == QMENU_LAYER_WR) && !(pMng->Para.Cfg & QMENU_CFG_REAL_REFRESH))
      Flag |= 0x08;      
  }
  //ר������
  QMenuMng_cbDispNum4(pMng->Flag & QMENU_MNG_ID_MASK,
                      pMng->Para.Adj, Flag);
}

