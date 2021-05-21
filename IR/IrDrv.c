/***************************************************************************

                       �������ͨ����������ʵ��

***************************************************************************/
#include "IrDrv.h"
#include <string.h>

struct _IrDrv  IrDrv;

/***************************************************************************
                              ��غ���ʵ��
***************************************************************************/
//-----------------------------��ʼ������-----------------------------
void IrDrv_Init(void)
{
  memset(&IrDrv, 0, sizeof(IrDrv));
  IrDrv.PrvKey = 0xff;
  IrDrv_cbHwInit();//��ʼ��Ӳ��
}

//-------------------------------��ͨ������-----------------------------
//����16ms������ɨ��
void IrDrv_Task(void)
{
  if(!IrDrv.KeyKeepIndex) return;
  //���Ӱ�������
  IrDrv.KeyKeepIndex--;  
  if(IrDrv.PrvKey >= IR_DRV_KEY_COUNT) return; //�¼�ֵ��������  
  
  if(!IrDrv.KeyKeepIndex){//�û�ȡ�������Ӽ���
    //�ͷ�ʱ���̰���
    if(IrDrv.KeyHoldIndex < IR_DRV_KEY_LONG_COUNT) 
      IrDrv_cbKeyNotify((0 << 6) | IrDrv.PrvKey);
    else IrDrv_cbKeyNotify(0xff); //�ް���ͨ��
    if(IrDrv.PrvKey < IR_DRV_KEY_COUNT){//��Ч��ʱֵʱ��λ(ʶ����Ч��ֵ�л�ʱ��λ)
      IrDrv.PrvKey = 0xff;
    }
    return;
  }
  //������������,���ְ�����
  IrDrv.KeyHoldIndex++;
  if(IrDrv.KeyHoldIndex == IR_DRV_KEY_LONG_COUNT)
    IrDrv_cbKeyNotify((1 << 6) | IrDrv.PrvKey);
  else if(IrDrv.KeyHoldIndex == (IR_DRV_KEY_LONG_COUNT + IR_DRV_KEY_HOLD_SPACE)){
    IrDrv_cbKeyNotify((2 << 6) | IrDrv.PrvKey);
    IrDrv.KeyHoldIndex = IR_DRV_KEY_LONG_COUNT;
  }
}

//-------------------------------����������-----------------------------
//����������ɨ��
void IrDrv_FastTask(void)
{
  if(IrDrv.eState != eIrDrvState_Final) return;

  //�����յ���ʱ,ǿ����Ϊ���ϴΰ������ظ���
  if(IrDrv.KeyKeepIndex){
    IrDrv.KeyKeepIndex = IR_DRV_KEEP_COUNT;    
    IrDrv.eState = eIrDrvState_Rdy; //��׼�������¸�������
    return;
  } 
  IrDrv.KeyKeepIndex = IR_DRV_KEEP_COUNT; 
  
  //��ֵ���
  unsigned char Key = 0;  
  for(; Key < IR_DRV_KEY_COUNT; Key++){
    if(!memcmp(&IrDrv.Buf, &cbIrDrvInfo.Code[Key], sizeof(struct _IrDrvCode)))//��ͬ��
      break;
  }
  //��������
  if(Key != IrDrv.PrvKey){//���ΰ�����ͬ,��ʼ��
    IrDrv.PrvKey = Key;
    IrDrv.KeyHoldIndex = 0;
    if(Key >= IR_DRV_KEY_COUNT){//��ס�¼�ֵ���ڶ���
      if(IrDrv.PrvKey == IR_DRV_KEY_COUNT)//�¼�ֵ�л�ͨ��
        IrDrv_cbNewKeySwitchNotify();       
      memcpy(&IrDrv.CodeIdent, &IrDrv.Buf, sizeof(struct _IrDrvCode));
    }
  }
  else if(Key >= IR_DRV_KEY_COUNT){//���ΰ�����ͬ��,����¼�ֵ�Ƿ���ͬ
    if(!memcmp(&IrDrv.CodeIdent, &IrDrv.Buf, sizeof(struct _IrDrvCode))){
      IrDrv_cbNewKeyNotify(); //�¼�ֵͨ��
    }
    else{ //��Ч��ֵ�л�ʱ��λ
      IrDrv.PrvKey = 0xff;
    }
  }
  
  IrDrv.eState = eIrDrvState_Rdy; //��׼�������¸�������
  memset(&IrDrv.Buf, 0, sizeof(IrDrv.Buf));//����MCU�ж�����Ч,��ǰ��
}

//------------------------�����⺯��-----------------------------
//���׼��ȱȽ�,����Ǹߵ绹�ǵ͵�ƽ,���Ǳ�׼�ؿ�ȴ���
//���ظ�ֵ:��׼��ȴ���,��ֵ,�ȱ�׼��ȿ�, 0:���׼�����ͬ
static signed char _GetWidthInfo(unsigned short StdWidth,  //��׼���
                                 unsigned short Width)     //����Ŀ��
{
  if(Width < StdWidth){
    if(Width < (StdWidth - (StdWidth >> 2))) //С��3/4���ʱ,��Ϊ��׼��ȴ���
      return -1;
    //else return 0; //3/4��1֮��,��Ϊ�Ǳ�׼���
  }
  else{
    if(Width > (StdWidth + (StdWidth >> 2))) //����5/4���ʱ,��Ϊ�ǿ��ƽ
      return 1;
    //else return 0; //4/4��1֮��,��Ϊ�Ǳ�׼���
  }
  return 0;
}

//-----------------------------����������---------------------------------
//���׼��ȱȽ�,����Ǹߵ绹�ǵ͵�ƽ,���Ǳ�׼�ؿ�ȴ���
//���ظ�ֵ:��׼��ȴ���,��ֵ,�ȱ�׼��ȿ�, 0:���׼�����ͬ
static void _SetWidthInfo(unsigned char *pWidthMask,  //����������ʼλ��
                          unsigned char Pos,          //�������λ��
                          signed char Result)       //_GetWidthInfo������Ϣ
{
  if(Result < 0){//��ת��Ϣ
    //����ȫ����λ,��⵽ʱ��λ
    memset(pWidthMask, 0xff, IR_DRV_CODE_SIZE / 8); 
    
    //ע: ��һ������ȫ�ǿ��ƽ,����ǰ������Ӧ��0xff������0
    Result = 0;
  }
  pWidthMask += Pos >> 3;
  Pos = 1 << (Pos & 0x07); 
  if(Result) *pWidthMask |= Pos;
  else *pWidthMask &= ~Pos;
}

//---------------------------------�жϴ�����------------------------------
//���벶���ж���,�����ź����ڽ���(�����ڿ�������)ʱ���ô˺���
//ע:�涨��IdieWidth>=65535ʱ��ʾ��ǰ��ֵ������
void IrDrv_IRQ(unsigned short IdieWidth,  //���ź����ڵ�������,>=0
               unsigned short IrWidth)   //���ź����ڵ�������,>=0
{
  //�źŽ�����
  if(IdieWidth == 65535){ 
    if(IrDrv.eState >= eIrDrvState_SampIdieW) //����ʱ(����ң�ذ�ֻ��һ��Ir��ʾ�ظ���)
       IrDrv.eState = eIrDrvState_Final; 
    else{// ׼��������ֵ
      IrDrv.eState = eIrDrvState_Rdy;
    }
    return;
  }
  //����״̬����������
  switch(IrDrv.eState){
    case eIrDrvState_Rdy://��׼������ʼ,׼���ɼ�Ir���
      IrDrv.eState = eIrDrvState_SampIrW;
      //memset(&IrDrv.Buf, 0, sizeof(IrDrv.Buf));//PIC�ж�����Ч,��ǰ��
      IrDrv.eState = eIrDrvState_SampIrW;
      break;
    case eIrDrvState_SampIrW://�ɼ�Ir���,׼���ɼ���Ir���
      IrDrv.IrWidth = IrWidth;
      IrDrv.eState = eIrDrvState_SampIdieW;
      break;
    case eIrDrvState_SampIdieW://�ɼ���Ir��Ȳ���ʼ��������
      IrDrv.IdieWidth = IdieWidth;
      IrDrv.eState = eIrDrvState_Doing;
      //����break;
    case eIrDrvState_Doing:{//�ɼ���Ir��Ȳ���ʼ��������
      if(IrDrv.Buf.Count >= IR_DRV_CODE_SIZE) return; //������,����Ķ���
      //������п��
      signed char Result = _GetWidthInfo(IrDrv.IdieWidth, IdieWidth);
      if(Result < 0) IrDrv.IdieWidth = IdieWidth; //���¸��¿���ʱ���  
      _SetWidthInfo(IrDrv.Buf.IdieWidthMask, IrDrv.Buf.Count, Result);
      //��������Ϣʱ�Ŀ��
      Result = _GetWidthInfo(IrDrv.IrWidth, IrWidth);
      if(Result < 0) IrDrv.IrWidth = IrWidth; //���¸��¿���ʱ���
      _SetWidthInfo(IrDrv.Buf.IrWidthMask, IrDrv.Buf.Count, Result);
      IrDrv.Buf.Count++; //����һλ��
      break;
    }      
    //case eIrDrvState_Idie; //��û��ʼ��
    //case eIrDrvState_Final //��û�����
    default:break;
  }
}
