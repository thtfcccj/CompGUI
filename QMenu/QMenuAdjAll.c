/***********************************************************************
		             �˵�ϵͳ֮4λ�����-�������ģʽʵ��
//��ģ��ΪQMenuMng���ڲ�ʵ��,�Ƕ���ʹ��ģ��
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

//-------------------------����������---------------------------
void QMenuAdjAll_Key(struct _QMenuMng *pMng, unsigned char Key)
{
  //ȷ�ϼ���Ч
  if(Key & QMENU_MNG_KEY_ENTER) return;
  //�ϼ�����
  if(Key & QMENU_MNG_KEY_UP){
    if(pMng->Para.Adj < pMng->Para.Max) pMng->Para.Adj++;
    else pMng->Para.Adj = pMng->Para.Min;//�ػ���
    return;
  }
  //�¼�����
  if(pMng->Para.Adj > pMng->Para.Min) pMng->Para.Adj--;
  else pMng->Para.Adj = pMng->Para.Max;//�ػ���
}

//----------------------��ʾ���º���---------------------------
void QMenuAdjAll_Disp(struct _QMenuMng *pMng)
{
  QMenuAdj_DispVol(pMng); //�ȸ���ֵ��ʾ
  if(pMng->Layer == QMENU_LAYER_WR){//д״̬��ȫ������λ����
    QMenuMng_cbSetSegFlash(pMng->Flag & QMENU_MNG_ID_MASK, 0x0f);
    QMenuMng_cbSetDotFlash(pMng->Flag & QMENU_MNG_ID_MASK,
                           pMng->Para.Cfg2 & 0x0f);//��С����һ����Ϊ����
  }
}

