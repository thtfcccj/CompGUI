/***********************************************************************
		         �˵�ϵͳ֮4λ�����-ʮ�����Ƶ���ģʽʵ��

//WrIndexλ��ֵ�ֱ�Ϊ:0,1,2,3, ��0Ϊ��ʮ������λ
//�Ծ͵���λ�ֱ�Ϊ:   0,1,2,3, ���Ӹߵ���λ����
//ÿһ��ֵ�Ŀɵ��ڷ�Χ��Max��Min����,Min>=Maxʱ���ɵ�
//��ģ��ΪQMenuMng���ڲ�ʵ��,�Ƕ���ʹ��ģ��
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

//-----------------------�õ��¸��ɵ���λ---------------------------
//>= 4��δ�ҵ�
static unsigned char _GetNextWrIndex(struct _QMenuMng *pMng, 
                                       unsigned char NextIndex)
{
  unsigned short Mask = 0x0f << ((3 - NextIndex) * 4);//���λΪ0
  for(; NextIndex < 4; NextIndex++, Mask >>= 4){
    if((Mask & pMng->Para.Max) > (Mask & pMng->Para.Min))
      break;
  }
  return NextIndex;
}

//-----------------------��ʼ������---------------------------
void QMenuAdjHex_Init(struct _QMenuMng *pMng)
{
  pMng->WrIndex = _GetNextWrIndex(pMng, 0);
}

//-------------------------����������---------------------------
void QMenuAdjHex_Key(struct _QMenuMng *pMng, 
                       unsigned char Key)
{
  unsigned char Data;
  
  //ȷ�ϼ��л�����λ, pMng->WrIndex��ʾ��ǰ��������ֵλ
  if(Key & QMENU_MNG_KEY_ENTER){
    Data = _GetNextWrIndex(pMng, pMng->WrIndex + 1);
    if(Data >= 4) Data = _GetNextWrIndex(pMng, 0); //�ػ���
    pMng->WrIndex = Data;
    
    pMng->Flag |= QMENU_MNG_FLASH_HIDDEN; //�л�ʱ���л�λ����
    return;
  }
  //��Χ�����������Сֵ����
  Data = (3 - pMng->WrIndex) * 4;
  unsigned short Mask = 0x0f << Data; 
  unsigned short Hex = pMng->Para.Adj & Mask;
  unsigned short Min = pMng->Para.Min & Mask;
  unsigned short Max = pMng->Para.Max & Mask;
  pMng->Para.Adj &= ~Mask;  
  unsigned short Add =  1 << Data;   
  //�ϼ�����λ+1,
  if(Key & QMENU_MNG_KEY_UP){
    if(Hex == Max) Hex = Min;
    else Hex += Add;
  }
  else{//�¼�����λ-1
    if(Hex == Min) Hex = Max;
    else Hex -= Add;
  }
  pMng->Para.Adj |= Hex;
}

//----------------------��ʾ���º���---------------------------
void QMenuAdjHex_Disp(struct _QMenuMng *pMng)
{
  
  unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;  
  
  //��������������,��С������
  unsigned short Data = pMng->Para.Adj;
  for(unsigned char Pos = 0; Pos < 4; Pos++, Data >>= 4){
    QMenuMng_cbSetSegDisp(MngId, 
                          Pos, 
                          QMenuMng_cbVol2Seg(MngId, Data & 0x0f));     
  }

  if(pMng->Layer == QMENU_LAYER_WR)//д״̬�õ���λ����
    QMenuMng_cbSetSegFlash(MngId, QMenuMng_cbPos2Mask(MngId, 3 - pMng->WrIndex));
}

