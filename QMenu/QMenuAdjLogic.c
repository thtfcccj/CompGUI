/***********************************************************************
		             �˵�ϵͳ֮4λ�����-�߼�������ģʽʵ��
//���λ�߼�ֵ,λ�ڸ�λ��ʾ
//WrIndexλ��ֵ�ֱ�Ϊ:3,2,1,0, 7,6,5,4
//�Ծ͵���λ�ֱ�Ϊ:   0,1,2,3, 4,5,6,7
//�߼����Ŀɵ���λ��Maxλ����,��1ʱ��ʾ�ɵ���,����ֻ��
//��ģ��ΪQMenuMng���ڲ�ʵ��,�Ƕ���ʹ��ģ��
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

//-----------------------�õ��¸��ɵ���λ---------------------------
//>= 8��δ�ҵ�
static unsigned char _GetNextWrIndex(struct _QMenuMng *pMng, 
                                     unsigned char NextIndex)
{
  for(; NextIndex < 8; NextIndex++)
    if((1 << NextIndex) & pMng->Para.Max) break;
  return NextIndex;
}

//-----------------------��ʼ������---------------------------
void QMenuAdjLogic_Init(struct _QMenuMng *pMng)
{
  pMng->WrIndex = _GetNextWrIndex(pMng, 0);
}

//-------------------------����������---------------------------
void QMenuAdjLogic_Key(struct _QMenuMng *pMng, 
                       unsigned char Key)
{
  unsigned char Data;
  
  //ȷ�ϼ��л�����λ, pMng->WrIndex��ʾ��ǰ��������ֵλ
  if(Key & QMENU_MNG_KEY_ENTER){
    Data = _GetNextWrIndex(pMng, pMng->WrIndex + 1);
    if(Data >= 8) Data = _GetNextWrIndex(pMng, 0); //�ػ���
    pMng->WrIndex = Data;
    
    pMng->Flag |= QMENU_MNG_FLASH_HIDDEN; //�л�ʱ���л�λ����
    return;
  }
  
  //���¼��л��߼�
  Data = 1 << pMng->WrIndex;
  if(pMng->Para.Adj & Data) pMng->Para.Adj &= ~Data;
  else pMng->Para.Adj |= Data;
}

//-----------------������ʱ�����¼���ҳ������---------------------------
void QMenuAdjLogic_RdPageKey(struct _QMenuMng *pMng)
{
  unsigned char Data = _GetNextWrIndex(pMng, pMng->WrIndex + 4);
  if(Data >= 8) Data = _GetNextWrIndex(pMng,0); //�ػ���
  pMng->WrIndex = Data;
}

//----------------------��ʾ���º���---------------------------
void QMenuAdjLogic_Disp(struct _QMenuMng *pMng)
{
  unsigned char Pos,IsHi;
  unsigned char Data = pMng->Para.Adj;
  
  if(pMng->WrIndex >= 4) Data >>= 4;//��ʾ��λֵ������
    unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;
  
  //����С���������
  for(Pos = 0; Pos < 4; Pos++){ //�ж���ֵ
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
  QMenuMng_cbSetDotDisp(MngId, IsHi); //��С�����ʾ�ڵ�2ҳ
  
  if(pMng->Layer == QMENU_LAYER_WR)//д״̬�õ���λ����
    QMenuMng_cbSetSegFlash(MngId, QMenuMng_cbPos2Mask(MngId, 3 - Pos));
}

