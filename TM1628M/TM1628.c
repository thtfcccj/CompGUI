/***************************************************************************

                         ������TM1628����ʵ��
TM1628�����1MHZ
***************************************************************************/
#include "TM1628.h"
#include <string.h>

unsigned char TM1628_CommBuf[TM1628_COM_BUF];//�ڽ���ͨѶ������
/***************************************************************************
                              ��غ���
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void TM1628_Init(struct _TM1628 *pTM1628, 
                 unsigned char Ch,         //��TM1628ͨ����
                 unsigned char Gray)       //0~7Խ��Խ��, 8�ر���ʾ
{
  memset(pTM1628, 0, sizeof(struct _TM1628));
  pTM1628->Gray = Gray;
  TM1628_Task(pTM1628);  //������ʼ��������һ����ʾ
}

//-------------------------�����ֽ�ת��Ϊ����ID��----------------------
//��֧�ֶఴ��ģʽ
#ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
static unsigned char _ByteKey2KeyId(struct _TM1628 *pTM1628,
                                     unsigned char KeyCount)
{
  unsigned char KeyId = 0;
  unsigned char KeyByte;
  for(unsigned Pos = 1; Pos <= KeyCount; Pos++){
    KeyByte = TM1628_CommBuf[Pos];
    if(KeyByte & 0x01) break;   //B0����
    KeyId++;
    if(KeyByte & 0x02) break;   //B1����
    KeyId++;
    if(KeyByte & 0x08) break;   //B2����
    KeyId++;
    if(KeyByte & 0x10) break;   //B3����
    KeyId++;
  }
  return KeyId;
}
#endif

//--------------------------------������-----------------------------
//Լ10ms����һ��
void TM1628_Task(struct _TM1628 *pTM1628)
{
  unsigned char Ch = pTM1628->Ch;
  const struct _TM1628Static *pStatic = TM1628_cbGetStatic(Ch);
  //����ǿ�Ƹ����뿪��/���ڳ�ʼ����֧���Ȳ���
  if(!pTM1628->PeriodInitTimer){
    pTM1628->PeriodInitTimer = TM1628_LED_PERIOD_INIT_OV;
    //��ʾ��������
    TM1628_CommBuf[0] = pStatic->DispMode;
    TM1628_cbSendData(Ch, 1);    
    //���»ҶȲ���ʾ
    if(pTM1628->Gray > 7) TM1628_CommBuf[0] = 0x80;//�ر���ʾ
    else TM1628_CommBuf[0] = 0x88 | pTM1628->Gray;//ָ��0x80, ����(B3) + �Ҷ�
    TM1628_cbSendData(Ch, 1);
    pTM1628->DirtyMask = 0xffff; //ǿ�Ƹ�����ʾ
  }
  else pTM1628->PeriodInitTimer--;
  
  //����ɨ��
  #ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
  if(pStatic->KeyByteCount){//��ʵ���а���ʱ
    if(pTM1628->Flag & TM1628_KEY_PERIOD){//����ɨ������ʵʱ��ȡ����
      pTM1628->Flag &= ~TM1628_KEY_PERIOD;
      TM1628_CommBuf[0] = 0x42;//����ɨ����
      TM1628_cbFullKey(Ch, pStatic->KeyByteCount);
      pTM1628->Flag &= ~TM1628_KEY_ID_MASK;
      pTM1628->Flag |= _ByteKey2KeyId(pTM1628, pStatic->KeyByteCount);
      TM1628_cbKeyUpdate(pTM1628); //��ֵ����ͨ��
      return;
    }
    pTM1628->Flag |= TM1628_KEY_PERIOD;//�´�Ϊ��������
  }
  #endif
  //�����ɨ��: ����Ϊ���仯ʱд��
  unsigned short DirtyMask = pTM1628->DirtyMask;
  if(!DirtyMask) return; //û�����ݸ���

  //�ݲ���һ����д�����(�����ٵ�ͨѶЧ�ʵ�)
  TM1628_CommBuf[0] = 0x40;//д���ݵ���ʾ�Ĵ���ָ��(b0:1),�Զ���ַ����(b2)
  TM1628_cbSendData(Ch, 1);
  //д�Դ�
  TM1628_CommBuf[0] = 0xC0;//��ʾÿ�δ�ͷ��ʼ
  if(TM1628_cbIsTest()) 
    memset(&TM1628_CommBuf[1], 0xff, pStatic->LedCount);
  else 
    memcpy(&TM1628_CommBuf[1], pTM1628->DispBuf, pStatic->LedCount);
  TM1628_cbSendData(Ch, 1 + pStatic->LedCount);  
}

//-------------------------------�����������ʾ-------------------------
void TM1628_UpdateDisp(struct _TM1628 *pTM1628,
                       unsigned char UserPos,//�ڲ���TM1628_TO_HWת��
                       unsigned char Seg)   //����
{
  const struct _TM1628Static *pStatic = TM1628_cbGetStatic(pTM1628->Ch);
  unsigned char HwPos = pStatic->ToHwLut[UserPos];
  if(pTM1628->DispBuf[HwPos] == Seg) return;//δ����
  //������
  pTM1628->DispBuf[HwPos] = Seg;
  pTM1628->DirtyMask |= 1 << HwPos;
}

//-------------------------------�õ��������ʾ-------------------------
unsigned char TM1628_GetDisp(const struct _TM1628 *pTM1628,
                             unsigned char UserPos)
{
  const struct _TM1628Static *pStatic = TM1628_cbGetStatic(pTM1628->Ch);
  return pTM1628->DispBuf[pStatic->ToHwLut[UserPos]];
}

//---------------------------------���»Ҷȼ���-------------------------
void TM1628_UpdateGray(struct _TM1628 *pTM1628,
                       unsigned char Gray)    //0~7Խ��Խ��, 8�ر���ʾ
{
  pTM1628->Gray = Gray;
  pTM1628->PeriodInitTimer = 0;//�´�������Ч
}


