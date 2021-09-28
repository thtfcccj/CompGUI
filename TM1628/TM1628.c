/***************************************************************************

                  TM1628����ʵ��
TM1628�����1MHZ
***************************************************************************/
#include "TM1628.h"
#include <string.h>

struct _TM1628  TM1628;//ֱ�ӵ�����
static const unsigned char _ToHwLut[] = TM1628_TO_HW;

/***************************************************************************
                              ��غ���
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void TM1628_Init(unsigned char Gray)    //0~7Խ��Խ��, 8�ر���ʾ
{
  TM1628_cbHwInit();
  memset(&TM1628, 0, sizeof(struct _TM1628));
  
  //��ʾ��������
  TM1628.CommBuf[0] = TM1628_DISP_MODE;
  TM1628_cbSendData(1);
  
  TM1628.DirtyMask = 0xffff; //��������
  TM1628_Task();  //����һ����ʾ
  
  TM1628_UpdateGray(Gray);//���»ҶȲ���ʾ
}

//-------------------------�����ֽ�ת��Ϊ����ID��----------------------
//��֧�ֶఴ��ģʽ
#ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
static unsigned char _ByteKey2KeyId(void)
{
  unsigned char KeyId = 0;
  unsigned char KeyByte;
  for(unsigned Pos = 1; Pos <= SUPPORT_TM1628_KEY; Pos++){
    KeyByte = TM1628.CommBuf[Pos];
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
void TM1628_Task(void)
{
  //����ɨ��
  #ifdef SUPPORT_TM1628_KEY //֧�ְ���ʱ
    if(TM1628.Flag & TM1628_KEY_PERIOD){//����ɨ������ʵʱ��ȡ����
      TM1628.Flag &= ~TM1628_KEY_PERIOD;
      TM1628.CommBuf[0] = 0x42;//����ɨ����
      TM1628_cbFullKey(SUPPORT_TM1628_KEY);
      TM1628.Flag &= ~TM1628_KEY_ID_MASK;
      TM1628.Flag |= _ByteKey2KeyId();
      return;
    }
    TM1628.Flag |= TM1628_KEY_PERIOD;//�´�Ϊ��������
  #endif
  //�����ɨ��: ����Ϊ����
  unsigned short DirtyMask = TM1628.DirtyMask;
  if(!DirtyMask) return; //û�����ݸ���

  //�ݲ���һ����д�����(�����ٵ�ͨѶЧ�ʵ�)
  TM1628.CommBuf[0] = 0x40;//д���ݵ���ʾ�Ĵ���ָ��(b0:1),�Զ���ַ����(b2)
  TM1628_cbSendData(1);
  //д��ʾ��
  TM1628.CommBuf[0] = 0xC0;//��ʾÿ�δ�ͷ��ʼ
  if(TM1628_cbIsTest()) 
    memset(&TM1628.CommBuf[1], 0xff, TM1628_LED_COUNT);
  else 
    memcpy(&TM1628.CommBuf[1], TM1628.DispBuf, TM1628_LED_COUNT);
  TM1628_cbSendData(1 + TM1628_LED_COUNT);  
}

//-------------------------------�����������ʾ-------------------------
void TM1628_UpdateDisp(unsigned char UserPos,//�ڲ���TM1628_TO_HWת��
                       unsigned char Seg)   //����
{
  unsigned char HwPos = _ToHwLut[UserPos];
  if(TM1628.DispBuf[HwPos] == Seg) return;//δ����
  //������
  TM1628.DispBuf[HwPos] = Seg;
  TM1628.DirtyMask |= 1 << HwPos;
}

//-------------------------------�õ��������ʾ-------------------------
unsigned char TM1628_GetDisp(unsigned char UserPos)
{
  return TM1628.DispBuf[_ToHwLut[UserPos]];
}

//---------------------------------���»Ҷȼ���-------------------------
void TM1628_UpdateGray(unsigned char Gray)    //0~7Խ��Խ��, 8�ر���ʾ
{
  if(Gray > 7) TM1628.CommBuf[0] = 0x80;//�ر���ʾ
  else TM1628.CommBuf[0] = 0x88 | Gray;//ָ��0x80, ����(B3) + �Ҷ�
  TM1628_cbSendData(1);
}
