/***********************************************************************

//		             �˵�ϵͳ֮4λ�����ʱ�Ĺ�����ʵ��

***********************************************************************/

#include "QMenuMng.h"
#include "QMenuAdj.h"

#include <string.h>

//struct _QMenuMng Mng;  //ռλ����

/***********************************************************************
                    ��غ���ʵ��
***********************************************************************/

//------------------�ж��Ƿ���ʵʱ��ʾ״̬------------------------
signed char QMenuMng_IsRealDisp(struct _QMenuMng *pMng)
{
  if((pMng->Layer == QMENU_LAYER_RD) && 
    (pMng->Para.Cfg & (QMENU_CFG_REAL_RD | QMENU_CFG_REAL_REFRESH))) 
    return -1;
  if((pMng->Layer == QMENU_LAYER_WR) && (pMng->Para.Cfg & QMENU_CFG_REAL_REFRESH)) 
    return -2; 
  return 0;
}

//------------------------����װ���û�����------------------------
void QMenuMng_ReloadPara(struct _QMenuMng *pMng)
{
  QMenuPara_Init(&pMng->Para); //���³�ʼ��
  pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para,
                                               pMng->Layer);//�˵����
  pMng->Flag |= QMENU_MNG_REFRESH_NOW;
}

//--------------------------������--------------------------
//���˺�������ϵͳ250mS������,���ڸ���ʵʱֵ����ʾ
void QMenuMng_Task(struct _QMenuMng *pMng)
{
  if(QMenuMng_IsRealDisp(pMng)){
    //��ʼʵʱ����
    pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para, QMENU_CFG_REAL_RD); 
    QMenuMng_UpdateDisp(pMng); //������ʾ
  }
  //������
  else if(pMng->Flag & QMENU_MNG_REFRESH_NOW){
    pMng->Flag &= ~QMENU_MNG_REFRESH_NOW;
    QMenuMng_UpdateDisp(pMng); //������ʾ
  }
}

//----------------------����˵���ʼ������--------------------------
//��������׼������˵�ʱ���ùҽӵĲ˵����ұ�����ڲ���ʼ��
void QMenuMng_EnterInit(struct _QMenuMng *pMng, 
                        const struct _QMenuFun * const *pFunAry,//����ΪNULL
                        const unsigned char *pLUT)//����ΪNULL
{
  unsigned char Id = pMng->Flag & QMENU_MNG_ID_MASK;
  memset(pMng, 0, sizeof(struct _QMenuMng));
  pMng->Flag = Id;
  pMng->pFunAry = pFunAry;
  pMng->pLUT = pLUT;
}

//-------------------------�õ���ǰ�˵�ID��--------------------
unsigned char QMenuMng_GetId(const struct _QMenuMng *pMng)
{
  return  *(pMng->pLUT + pMng->Index + 1);
}

//----------------------���²˵���ʾ����--------------------------
//���ô˺����������½�����ʾ
void QMenuMng_UpdateDisp(struct _QMenuMng *pMng)
{
  unsigned char MngId = pMng->Flag & QMENU_MNG_ID_MASK;
  
  //ͳһ��ʼ����ʾ����(��������ܵ���ʾ)
  QMenuMng_cbClrSegFlash(MngId);
  QMenuMng_cbClrDotFlash(MngId);
  QMenuMng_cbClrDotDisp(MngId);  
  
  if(pMng->Layer == QMENU_LAYER_SEL){//ֱ���ò˵�����Ÿ�����ʾ(��С����)
    #ifdef SUPPORT_QMENU_PIXEL   //֧�����ػ�ʱ�����ⲿʵ��
      QMenuMng_cbUpdateDispMenuSel(pMng);
    #else //4λ�����ʱ�����θ��²˵���ʾ����ʾ
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
    //���ݵ�����ʽ�������ģ����ʾ
    switch(pMng->Para.Cfg & QMENU_CFG_ADJ_MASK){
    case QMENU_CFG_ADJ_BIT: 
    case QMENU_CFG_ADJ_PASS:     
      QMenuAdjBit_Disp(pMng); 
      QMenuMng_cbSetDotDisp(MngId, pMng->Para.Cfg2 & QMENU_CFG2_DOT_MASK);//�ֶ�����С����
      break; 
    case QMENU_CFG_ADJ_ALL: 
    case QMENU_CFG_ADJ_CMD:       
      QMenuAdjAll_Disp(pMng); 
      QMenuMng_cbSetDotDisp(MngId, pMng->Para.Cfg2 & QMENU_CFG2_DOT_MASK);//�ֶ�����С����
      break;
    #ifdef SUPPORT_QMENU_ADJ_HEX
      case QMENU_CFG_ADJ_HEX: QMenuAdjHex_Disp(pMng); 
      QMenuMng_cbSetDotDisp(MngId, pMng->Para.Cfg2 & QMENU_CFG2_DOT_MASK);//�ֶ�����С����
      break;
    #endif
    //case QMENU_CFG_ADJ_LOGIC:
    default:QMenuAdjLogic_Disp(pMng); break;
    }
  }
  QMenuMng_cbForceFlash(MngId, pMng->Flag & QMENU_MNG_FLASH_HIDDEN); //ǿ�Ƹ���
  pMng->Flag &= ~QMENU_MNG_FLASH_HIDDEN;
}

/***********************************************************************
                    ����������غ���ʵ��
***********************************************************************/

//---------------------�����д״̬��ʼ������--------------------------
void _EnterRdWr(struct _QMenuMng *pMng, unsigned char IsWr)
{
  if(IsWr){
    pMng->Layer = QMENU_LAYER_WR;
  }
  else pMng->Layer = QMENU_LAYER_RD;
  
  pMng->WrIndex = 0;
  pMng->OrgAdj = pMng->Para.Adj;
  
  //���ݵ�����ʽ��ʼ�����ģ��
  switch(pMng->Para.Cfg & QMENU_CFG_ADJ_MASK){
    case QMENU_CFG_ADJ_PASS:    
    case QMENU_CFG_ADJ_BIT: QMenuAdjBit_Init(pMng); break;
    case QMENU_CFG_ADJ_CMD: pMng->OrgAdj++;//������ģʽ��д
    case QMENU_CFG_ADJ_ALL: QMenuAdjAll_Init(pMng); break;
    #ifdef SUPPORT_QMENU_ADJ_HEX
      case QMENU_CFG_ADJ_HEX: QMenuAdjHex_Init(pMng); break;
    #endif
    //case QMENU_CFG_ADJ_LOGIC:
    default:QMenuAdjLogic_Init(pMng); break;
  }
}
//---------------------�˳�д״̬������--------------------------
//�β�Ϊд״̬:0:��ֹд,����:���ݸ�".SetData"���β�
void _ExitWr(struct _QMenuMng *pMng, unsigned char WrState)
{
  //�����˳�ʱ,������δ�ı�,��������˳������һ������
  if((WrState == QMENU_LAYER_WR) && (pMng->OrgAdj == pMng->Para.Adj)){
    if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT_SAVE) //�˳�ʱҲ����ʱ,�����˳�����
      WrState = QMENU_CFG2_QUIT_SAVE;
    else WrState = 0;//�˳�������
  }
  if(WrState != 0){//������ʱ
    unsigned char Id = QMenuMng_GetId(pMng); //PIC��Ҫ�Ź���
    pMng->pFunAry[Id]->SetData(&pMng->Para, WrState); //д����    
  }  
  if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT){ //ֱ���˳��˵�
    pMng->Layer = QMENU_LAYER_NULL;
    return;
  }
  
  pMng->pFunAry[QMenuMng_GetId(pMng)]->
    GetData(&pMng->Para, QMENU_LAYER_SEL);//����һ�����ݻ��д��������
  if(pMng->Para.Cfg & QMENU_CFG_WR) //ֻдʱֱ������ѡ��״̬
    pMng->Layer = QMENU_LAYER_SEL;
  else
    _EnterRdWr(pMng, 0); //�˳�����״̬����ʼ��
}

//---------------------�˵�ѡ��״̬����������--------------------------
void _SelKey(struct _QMenuMng *pMng, unsigned char Key)
{
  //��Ӧ��������
  if(Key & QMENU_MNG_KEY_LONG) return; //����Ӧ������
  
  if(Key == (QMENU_MNG_KEY_EXIT | QMENU_MNG_KEY_SHORT)){//�̰��˳��˵�
    pMng->Layer = QMENU_LAYER_NULL;
    return;
  }
  
  if(Key == (QMENU_MNG_KEY_ENTER | QMENU_MNG_KEY_SHORT)){//�̰�����˵�
    if(pMng->Para.Cfg & QMENU_CFG_WR){ //ֱ�ӽ���д״̬
      pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para, QMENU_LAYER_WR);//д����׼��
      _EnterRdWr(pMng, 1);//����д״̬
    }
    else _EnterRdWr(pMng, 0);//�����״̬
    return;
  }
  //�˵���Ӽ�
  if(Key & QMENU_MNG_KEY_UP){//�˵����
    pMng->Index++;
    if(pMng->Index >= *pMng->pLUT) pMng->Index = 0;//�ػ���
  }
  else if(Key & QMENU_MNG_KEY_DOWN){//�˵����
    if(!pMng->Index) pMng->Index = *pMng->pLUT - 1;//�ػ���
    else pMng->Index--;
  }
  //else return; //�������˳�����ʱ
  
  QMenuPara_Init(&pMng->Para); //���³�ʼ��
  pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para,
                                          pMng->Layer);//�˵�ѡ��״̬Ԥ������׼��
}

//---------------------�˵�������ʾ(��)״̬����������--------------------------
void _RdKey(struct _QMenuMng *pMng, unsigned char Key)
{
  /*/ֻ��ʱ,�������,���û��б��溯��,����øú���ִ�б��湦��
  if((Key & (QMENU_MNG_KEY_LONG | QMENU_MNG_KEY_ENTER)) ==
     (QMENU_MNG_KEY_LONG | QMENU_MNG_KEY_ENTER)){
    if(pMng->Para.Cfg & QMENU_CFG_RD){//ֻ��ʱ�������   
      if(pMng->pFunAry[QMenuMng_GetId(pMng)].SetData != NULL){
        pMng->pFunAry[QMenuMng_GetId(pMng)].SetData(&pMng->Para, QMENU_CFG_WR);//д����
        pMng->pFunAry[QMenuMng_GetId(pMng)].GetData(&pMng->Para, QMENU_LAYER_SEL);//����һ�����ݻ��д���
        pMng->Layer = QMENU_LAYER_SEL;//�˵��ϲ�˵�
      }
    }
    return;
  }*/ //���� QMENU_CFG_ADJ_CMD���ܴ���
  //����Ӧ�����뱣�ְ���
  if(Key & (QMENU_MNG_KEY_LONG | QMENU_MNG_KEY_KEEP)) return;

  if(Key & QMENU_MNG_KEY_ENTER){//�̰�ȷ�ϼ�����д״̬
    if(!(pMng->Para.Cfg & QMENU_CFG_RD)){//��ֻ��ʱ�������
      pMng->pFunAry[QMenuMng_GetId(pMng)]->GetData(&pMng->Para, QMENU_LAYER_WR);//д����׼��
      _EnterRdWr(pMng, 1);//����д״̬
    }
  }
  else if(Key & QMENU_MNG_KEY_EXIT){//�̰��˳����˳���ʾ״̬
    pMng->Layer = QMENU_LAYER_SEL;
  }
  else{ //���¼�
    //���߼�ʱ�����¼�����Ϊ��ҳ��ʾ
    if((pMng->Para.Cfg & QMENU_CFG_ADJ_MASK) == QMENU_CFG_ADJ_LOGIC){
      QMenuAdjLogic_RdPageKey(pMng);
    }
  }
}

//---------------------�˵���������(д)״̬����������-------------------------
void _WrKey(struct _QMenuMng *pMng, unsigned char Key)
{
  if(Key & QMENU_MNG_KEY_LONG){//������
    if(Key & QMENU_MNG_KEY_ENTER) _ExitWr(pMng, QMENU_LAYER_WR);//����ȷ�ϼ����沢�˳�
    return;
  }
  if(Key & QMENU_MNG_KEY_EXIT){//�̰��˳��˵�
    if(Key & QMENU_MNG_KEY_SHORT){
      if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT_SAVE) //�˳�ʱҲ����
        _ExitWr(pMng, QMENU_CFG2_QUIT_SAVE);
      else _ExitWr(pMng, 0);//�˳�ʱ������
    }
    return;
  }
  
  //��������ģʽʱ, �ڵ������һλʱ�̰�ȷ�ϼ���������һ�ε���SetData
  if((Key & QMENU_MNG_KEY_ENTER) && (pMng->WrIndex == 0) && 
     ((pMng->Para.Cfg & QMENU_CFG_ADJ_MASK) == QMENU_CFG_ADJ_PASS)){
    unsigned char Id = QMenuMng_GetId(pMng); //PIC��Ҫ�Ź���
    pMng->pFunAry[Id]->SetData(&pMng->Para, QMENU_CFG_WR);  
  }
  
  //�����������ݵ�����ʽ����
  switch(pMng->Para.Cfg & QMENU_CFG_ADJ_MASK){
    case QMENU_CFG_ADJ_PASS:     
    case QMENU_CFG_ADJ_BIT: QMenuAdjBit_Key(pMng, Key); break;
    case QMENU_CFG_ADJ_ALL: QMenuAdjAll_Key(pMng, Key); break;
    case QMENU_CFG_ADJ_LOGIC: QMenuAdjLogic_Key(pMng, Key); break;
    #ifdef SUPPORT_QMENU_ADJ_HEX
      case QMENU_CFG_ADJ_HEX: QMenuAdjHex_Key(pMng, Key); break;
    #endif
    
    //case QMENU_CFG_ADJ_CMD:      //���Ұ�����Ч
    default: break;
  }
  
  if(pMng->Para.Cfg & QMENU_CFG_REAL_WR){//ʵʱдʱ,���ݱ仯д����
    if(Key & (QMENU_MNG_KEY_UP | QMENU_MNG_KEY_DOWN)){
      unsigned char Id = QMenuMng_GetId(pMng); //PIC��Ҫ�Ź���
      pMng->pFunAry[Id]->SetData(&pMng->Para, QMENU_CFG_REAL_WR); //ʵʱд����
    }
  }
}

//--------------------------����������--------------------------
//���˺�������ϵͳ���ٽ�����,���ڴ����ֵ
void QMenuMng_FastTask(struct _QMenuMng *pMng)
{
  unsigned char Key = pMng->Key;
  if(!Key) return; //�޼�ֵ
  pMng->Key = 0;
  
  unsigned char PrvLayer = pMng->Layer;
  switch(pMng->Layer){
  case QMENU_LAYER_NULL: //���ڲ˵�ϵͳ
    if(Key != QMENU_MNG_KEY_ENTER) break; //
    pMng->Layer++; //����
    Key = 0;//�������³�ʼ������,���ǰ���
  case QMENU_LAYER_SEL: //�˵�ϵͳѡ��:����Ӧ������
    _SelKey(pMng, Key);
    break;
  case QMENU_LAYER_RD: //�˵���ʾ����״̬
   _RdKey(pMng, Key);
    break;
  case QMENU_LAYER_WR: //�˵���������״̬
   _WrKey(pMng, Key);
    break;
  default: //�쳣�˳�
    pMng->Layer = QMENU_LAYER_NULL;
    break;
  }
  if(pMng->Layer != QMENU_LAYER_NULL) QMenuMng_UpdateDisp(pMng);//������ʾ
  else if(PrvLayer != QMENU_LAYER_NULL){//�˳��˵�ϵͳ
    QMenuMng_cbExitNotify(pMng->Flag & QMENU_MNG_ID_MASK); //����˳��˵�ϵͳͨ��
  }
}

//----------------------�˳��˵�����--------------------------
//�ڲ˵�״̬,�ⲿǿ���˳��˵�ʱ���ô˺���
void QMenuMng_Exit(struct _QMenuMng *pMng)
{
  if(pMng->Layer == QMENU_LAYER_NULL) return; 
  
  if(pMng->Para.Cfg2 & QMENU_CFG2_QUIT_SAVE){ //�˳�ʱҲ����(ֱ�ӵ����û��Լ��ٶ�ջ)
    pMng->pFunAry[QMenuMng_GetId(pMng)]->SetData(&pMng->Para, 
                                            QMENU_CFG2_QUIT_SAVE); //�˳�ʱд���� 
    pMng->Layer = QMENU_LAYER_NULL; //�˳��˵�
  }
  QMenuMng_cbExitNotify(pMng->Flag & QMENU_MNG_ID_MASK); //����˳��˵�ϵͳͨ��
}  
//----------------------------��ʼ������----------------------------
//����ʱ����
void QMenuMng_Init(struct _QMenuMng *pMng,
                    unsigned char Id) //�����ID��
{
  memset(pMng, 0, sizeof(struct _QMenuMng));
  pMng->Flag = Id;
}

/***********************************************************************
		                       QMenuPara��ػص�����ʵ��
***********************************************************************/
#include "struct.h" //struct_get()

//---------------------DataSetʵ�������˳��˵�����-----------------------
//��������Ӧ�ò���ʵ��SetData�������ڲ�����(�ڲ��������˳��ص������Է�ֹǶ��)
void QMenuPara_cbQuitMenuInSet(struct _QMenuPara *pPara)
{
  struct _QMenuMng *pMng = struct_get(pPara, struct _QMenuMng, Para);
  pMng->Layer = QMENU_LAYER_NULL;
}


