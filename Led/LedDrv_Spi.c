/***************************************************************************
                       ����IO����ʵ��ģ��
***************************************************************************/

#include "LedDrv_Spi.h"
#include "GuiSpi.h"     //Spi��GUI�ӿ�
#include "LedSign.h"    //LED_SIGN_DOT
#include <string.h>

struct _LedDrv_Spi  LedDrv_Spi;

/***************************************************************************
                              ��غ���ʵ��
***************************************************************************/

//-----------------------------��ʼ������-----------------------------
void LedDrv_Spi_Init(void)
{
  GuiSpi_Init();
  memset(&LedDrv_Spi, 0, sizeof(struct _LedDrv_Spi));
  //��ʼ����ʾ��
  GuiSpi.CmdBuf[0] = 0;      //������
  GuiSpi.DataBuf[0] = 0x1f;  //���ڿ���ÿ��������Ƿ���ʾ
  GuiSpi.DataBuf[1] = 0xff;    //���ܿ��ؼ��ܽ���ģʽ��ѡ��,��ʹ�ý���  
  GuiSpi.Cmd.CmdSize = 1;    //��Э���й̶�1��
  GuiSpi.Cmd.DataSize = 2;
  GuiSpi.Cmd.Cfg = GUI_SPI_DEV_CMD_DUPLEX; //����ȫ˫������
  GuiSpiDev_Restart(&GuiSpiDev ,&GuiSpi.Cmd); //��������
}

//--------------------------����ĳ������ܺ���---------------------------
void LedDrv_Spi_SetLed(unsigned char Pos,     //�����λ��
                       unsigned char SetDot)//��LED_SIGN_DOT����ʾ����ֵ
{
  if(Pos >= LED_DRV_SPI_SEG_COUNT) return; //����
  LedDrv_Spi.Buf[LED_DRV_SPI_ADR_SEG + Pos] = SetDot & (~LED_SIGN_DOT);//ȡSeg
  if(SetDot & LED_SIGN_DOT)//��С������
    LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] |= 1 << Pos;
  else
   LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] &= ~(1 << Pos);   
  
  LedDrv_Spi_SetUpdate();//�ø��±�־
}

//--------------------------�õ�ĳ������ܺ���---------------------------
//���ش�LED_SIGN_DOT����ʾ����ֵ
unsigned char LedDrv_Spi_GetLed(unsigned char Pos)     //�����λ��
{
  unsigned char SegPos;
  if(LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] & (1 << Pos))
    SegPos = LED_SIGN_DOT;
  else SegPos = 0;
  return SegPos | LedDrv_Spi.Buf[LED_DRV_SPI_ADR_SEG + Pos];
}

//----------------------------����������-----------------------------
//������ٽ�����ɨ��
void LedDrv_Spi_FastTask(void)
{
  GuiSpi_FastTask();  //�ȴ���SPI
  
  if(!GuiSpi_IsIdie()) return;    //SPI�������ݹ�����

  switch(LedDrv_Spi.eState){
    case eLedDrv_Spi_Idie://����ģʽ,����Ƿ��а����ж�,����ɷ�������
      if(!GuiSpi_IsCs()) //�ȴ�������,CS�������ж���
        LedDrv_Spi.eState = eLedDrv_Spi_Wait;
      //������Ҫ������!
      else if(LedDrv_Spi.Buf[LED_DRV_SPI_ADR_STATE] & LED_DRV_SPI_UPDATE){
        //��������
        GuiSpi.CmdBuf[0] = LED_DRV_SPI_ADR_START;  //������
        if(LedDrv_Spi_cbIsTest()){//д��������
          memset(GuiSpi.DataBuf, 0xff, LED_DRV_SPI_ADR_COUNT);
          GuiSpi.DataBuf[1] = LedDrv_Spi.Buf[1];//״̬��
        }
        else{ //����
          memcpy(GuiSpi.DataBuf, LedDrv_Spi.Buf, LED_DRV_SPI_ADR_COUNT);
        }
        GuiSpi.Cmd.DataSize = LED_DRV_SPI_ADR_COUNT;
        GuiSpi.Cmd.Cfg = GUI_SPI_DEV_CMD_DUPLEX; //����ȫ˫������
        GuiSpiDev_Restart(&GuiSpiDev ,&GuiSpi.Cmd); //��������
        
        LedDrv_Spi.eState = eLedDrv_Spi_Doing;   //���͹�����
        //����ȡ��,��������д��
        LedDrv_Spi.Buf[LED_DRV_SPI_ADR_STATE] &= ~LED_DRV_SPI_UPDATE;
      }
      break;
    case eLedDrv_Spi_Wait://���ж���,�ȴ�CSΪ��
      if(!GuiSpi_IsCs()) break; //���ڵ͵�ƽ��
      //׼�����հ�����
      LedDrv_Spi_SetUpdate();//�ø��±�־
      LedDrv_Spi.eState = eLedDrv_Spi_Idie;
      break;
    case eLedDrv_Spi_Doing:{//���͹�����ɺ�,����Ƿ��а���ֵ,����ص�
      unsigned char Key = GuiSpi.DataBuf[0];
      if(Key) LedDrv_Spi_cbKeyNotify(Key);
      //�����˳�
    }
    default:
      LedDrv_Spi.eState = eLedDrv_Spi_Idie;
      break;
  }
}
                               
                               

