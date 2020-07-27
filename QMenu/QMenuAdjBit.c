/***********************************************************************
		             �˵�ϵͳ֮4λ�����-��λ����ʵ��
//��ģ��ΪQMenuMng���ڲ�ʵ��,�Ƕ���ʹ��ģ��
***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"
#include "QMenuMath.h"

//----------------�õ�ָ��λ����ֵ����------------------------
unsigned char _GetDigitalInPos(unsigned short Value,
                               unsigned char Pos)
{
  while(Pos--){
    Value /= 10;
  }
  return Value % 10;
}

//-----------------------��ʼ������---------------------------
void QMenuAdjBit_Init(struct _QMenuMng *pMng)
{
  QMenuAdj_UpdateMaxPos(pMng);
  pMng->WrIndex = pMng->WrMaxPos; //��������λ
}

//�ɵ�����õ�����
static __flash const unsigned short _Bit2Muti[4] = {1,10,100,1000};

//-------------------------����������---------------------------
void QMenuAdjBit_Key(struct _QMenuMng *pMng, unsigned char Key)
{
  unsigned short Value, Muti;
  unsigned char Vol;
  
  //ȷ�ϼ��ڵ���λ���л�
  if(Key & QMENU_MNG_KEY_ENTER){
    if(pMng->WrIndex) pMng->WrIndex--;
    else pMng->WrIndex = pMng->WrMaxPos; //�ػ������λ��
    pMng->Flag |= QMENU_MNG_FLASH_HIDDEN; //�л�ʱ���л�λ����
    return;
  }
  //���¼�����λ����
  if((pMng->Para.Cfg2 & QMENU_CFG2_NEGATIVE) && (pMng->WrIndex == pMng->WrMaxPos)){
    pMng->Para.Adj = 0 - pMng->Para.Adj; //��ת
    return;
  }
  //���¼�����
  Value = QMenuMath_Ss2Su(pMng->Para.Adj);
  Muti = _Bit2Muti[pMng->WrIndex];//������
  Vol = _GetDigitalInPos(Value,pMng->WrIndex);  //�õ�ָ��λ��ֵ
  Value -= Muti * Vol; //ȥ����λ��
  if(Key & QMENU_MNG_KEY_UP){
    if(Vol < 9) Vol++;
    else Vol = 0;
  }
  else{
    if(Vol) Vol--;
    else Vol =  9;
  }
  Value += Vol * Muti; //�µĸ�λ��ֵ
  
  //�趨����ֵ
  if(pMng->Para.Adj < 0) QMenuAdj_SetValue(pMng, 0 - Value);
  else QMenuAdj_SetValue(pMng, Value);
}

//----------------------��ʾ���º���---------------------------
void QMenuAdjBit_Disp(struct _QMenuMng *pMng)
{
  QMenuAdj_DispVol(pMng); //�ȸ���ֵ��ʾ
  if(pMng->Layer == QMENU_LAYER_WR){//д״̬�����ڵ���λ����
    unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;
    QMenuMng_cbSetSegFlash(MngId, 
                           QMenuMng_cbPos2Mask(MngId, pMng->WrIndex));
  }
}

