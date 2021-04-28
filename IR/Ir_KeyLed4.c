/***************************************************************************

      ������ճ���֮����-��4����ֵ��4λ�����Ӧ��ʱ��ʵ��

***************************************************************************/
#ifdef SUPPORT_IR_KEY_MATCH    //֧�ֶ���ʱ

#include "Ir_KeyLed4.h"
#include "IrDrv.h"

#include "InfoBase.h"
#include "Eeprom.h"

#include "Led.h"
#include "LedSign.h"

#include <string.h>

struct _Ir_KeyLed4 Ir_KeyLed4; 

struct _IrDrvInfo cbIrDrvInfo;//ʵ��Ϊ����

//-----------------------------��ʼ������-----------------------------
void Ir_KeyLed4_Init(signed char Inited)
{
  memset(&Ir_KeyLed4, 0, sizeof(Ir_KeyLed4));
  //��ʼ��cbIrDrvInfo:
  if(!Inited){
    memcpy(&cbIrDrvInfo, &cbIrDrvInfoDefault, sizeof(struct _IrDrvInfo));
    Eeprom_Wr(IrDev_GetInfoBase(),
              &cbIrDrvInfo,
              sizeof(struct _IrDrvInfo));    
  }
  else{
    Eeprom_Rd(IrDev_GetInfoBase(),
              &cbIrDrvInfo,
              sizeof(struct _IrDrvInfo));
  }
  
}

//��������ʱ����ʾ���Ŷ���
static const unsigned char _DispLut[][4] = {
  {0,          LED_SIGN_P, LED_SIGN_U            },   //UP
  {LED_SIGN_N, LED_SIGN_U, LED_SIGN_O, LED_SIGN_D},   //DOWN 
  {LED_SIGN_R, LED_SIGN_T, LED_SIGN_N, LED_SIGN_E},   //ENTER
  {LED_SIGN_T, LED_SIGN_I, LED_SIGN_U, LED_SIGN_Q},   //QUIT
  {LED_SIGN_E, LED_SIGN_U, LED_SIGN_A, LED_SIGN_S},   //SAVE
};

//------------------------���½�����ʾ����-----------------------------
static void _UpDateDisp(void)
{
  if(!Ir_KeyLed4.IdentState) return;  //û��ʶ��״̬
  unsigned char Pos;
  //if(Ir_KeyLed4.IdentState > Ir_KeyLed4_KEY_COUNT) Pos = 4; //����״̬��
  //else Pos = Ir_KeyLed4.IdentState - 1;
  Pos = Ir_KeyLed4.IdentState - 1;
  memcpy(Led.SegDisp, _DispLut[Pos], 4);
  Led.SegFlash = 0x0f; //������ʾ
}

//------------------------------�˲�������-------------------------------
//��ֹ�����ź�Ӱ��ʶ��,����1:�쳣�ź�
unsigned char _IrFilter(void)
{
  if(IrDrv.CodeIdent.Count < 8) return 1; //̫����
  
  //������ź�ʱ�������������Ƿ�ȫΪ0��ȫΪ1
  unsigned char IdieCount = 0;
  for(unsigned char i = 0; i < IrDrv.CodeIdent.Count; i++){
    if(IrDrv.CodeIdent.IdieWidthMask[i >> 3] & (1 << (i & 0x07)))
      IdieCount++;
  }
  unsigned char WidthCount = 0;
  for(unsigned char i = 0; i < IrDrv.CodeIdent.Count; i++){
    if(IrDrv.CodeIdent.IrWidthMask[i >> 3] & (1 << (i & 0x07)))
      WidthCount++;
  }
  //��������Ϊ0��ȫ��
  if((!IdieCount) || (IdieCount >= IrDrv.CodeIdent.Count)){
    //IR���ڲ���ȫ��ͬ
    if((!WidthCount) || (WidthCount >= IrDrv.CodeIdent.Count))
      return 1;
  } 
  //�ź�����Ϊ0��ȫ�� 
  if((!WidthCount) || (WidthCount >= IrDrv.CodeIdent.Count)){
    //����IR���ڲ���ȫ��ͬ
    if((!IdieCount) || (IdieCount >= IrDrv.CodeIdent.Count))
      return 1;
  }
  return 0; //�����ȷ
}
//-------------------------------������-------------------------------
//����256mS������ɨ��
void Ir_KeyLed4_Task(void)
{
  //==============��������ʶ��ģʽ��������===================
  if(!Ir_KeyLed4.IdentState){
    if(Ir_KeyLed4.KeyCount >= IR_KEY_LED4_IDENT_ENTER_COUNT){//��������
      //����ʶ��ģʽ��ʼ��
      Ir_KeyLed4.IdentState = 1;
      memset(&cbIrDrvInfo, 0, sizeof(struct _IrDrvInfo)); //���ԭ�е���Ϣ
      Led_Init();//���³�ʼ����ʾ
      _UpDateDisp();//�ֶ�������ʾ
      Ir_KeyLed4.Timer = IR_KEY_LED4_IDENT_EXIT_COUNT; //�����״μ�ʱ
      Ir_KeyLed4.Flag &= ~IR_KEY_LED4_REPEAT; 
    }
    return;
  }
  
  //===============�˳�ʶ���г�ʱ�˳�����=======================
  if(Ir_KeyLed4.Timer){
    Ir_KeyLed4.Timer--;
    if(Ir_KeyLed4.Timer) return;//ʱ��δ��
  }
  //��ʱ,�˳�ʶ��ģʽ
   Eeprom_Rd(IrDev_GetInfoBase(),&cbIrDrvInfo, sizeof(struct _IrDrvInfo));//�ָ���ֵ
   Ir_KeyLed4.IdentState = 0;//
   Ir_KeyLed4.KeyCount = 0; //
   Ir_KeyLed4_cbLedInit();  //�ָ�KEY_LED4�ֳ�
   
   //while(1);//Reset(); //��λ����       
   return; //Ϊ��ֹ�й��ɵĺ����źŽ���,��Ϊ����
}

//-----------------------------����������---------------------------
//��ʶ��״̬���ô˺���������
void Ir_KeyLed4_Key(unsigned char Key)
{
  if(Ir_KeyLed4.IdentState > IR_KEY_LED4_KEY_COUNT){ //����״̬��
    if(Key & 0x04){//����Ϊȷ�ϼ���
      //������ֵ
      Eeprom_Wr(IrDev_GetInfoBase(), &cbIrDrvInfo, sizeof(struct _IrDrvInfo));       
      //void(*Reset)(void) = 0;
      while(1);//Reset(); //��λ����    
    }
  }
}

/***************************************************************************
                          IrDrv��ػص�����ʵ��
***************************************************************************/

//---------------------------����ֵͨ������--------------------------------
//ʶ�𵽶��벻�Եļ�ֵʱ,ͨ���¼�ֵ
//�������û��������״̬�򱣴��ֵ
void IrDrv_cbNewKeyNotify(void)
{
  //�жϽ���ʶ��״̬
  if(!Ir_KeyLed4.IdentState){
   if(_IrFilter())//�쳣�˳�
      Ir_KeyLed4.KeyCount = 0; //���¿�ʼ
    else Ir_KeyLed4.KeyCount++;
    return;
  }
  //�û������ȷ��
  if(Ir_KeyLed4.IdentState > IR_KEY_LED4_KEY_COUNT) return;

  //=================ʶ���ֵ������=========================
  if(Ir_KeyLed4.Flag & IR_KEY_LED4_REPEAT){//���ٴΰ�����
    memcpy(&cbIrDrvInfo.Code[Ir_KeyLed4.IdentState - 1],//��ʱ���±���
           &IrDrv.CodeIdent, 
           sizeof(struct _IrDrvCode));
    Ir_KeyLed4.IdentState++; //�¸�����
    _UpDateDisp();//������ʾ
    Ir_KeyLed4.Flag &= ~IR_KEY_LED4_REPEAT;
  }
  else{//����һ��
    Ir_KeyLed4.Flag |= IR_KEY_LED4_REPEAT;
  }
  //�嶨ʱ��
  Ir_KeyLed4.Timer = IR_KEY_LED4_IDENT_EXIT_COUNT; 
}

//--------------------�¼�ֵ�л�ͨ������------------------------------
//ʶ�𵽶����ͬ��ֵ��Ч������,��⵽��һ��ʱͨ��
void IrDrv_cbNewKeySwitchNotify(void)
{
  Ir_KeyLed4.Flag &= ~IR_KEY_LED4_REPEAT;
  Ir_KeyLed4.KeyCount = 0;
}

#endif //#ifdef SUPPORT_IR_KEY_MATCH    //֧�ֶ���ʱ
