/*******************************************************************************

                     TFT DBI����ͳһ�ӿ�-035��ʾ����غ���ʵ��
�˽ӿ������ʹ����ʾ���������޹�
********************************************************************************/
#include "TftDbi.h"
#include "TftCmd_035.h"
#include "Delay.h"

#include "TftDbi_B16.h" //��ʵ����أ���ʼΪ16λ����ʱʵ��

/*******************************************************************************
                           Ӳ����غ���
********************************************************************************/

//----------------------------ST7796SӲ������----------------------------------
static void _HwCfg_ST7796S(void)
{
  //�ο���https://wenku.baidu.com/view/e3d53f4c4a73f242336c1eb91a37f111f0850d5d.html
  TftDbi_WrCmd(0xf0); //Command Set Control
  TftDbi_WrData(0xc3);
  
  TftDbi_WrCmd(0xf0);
  TftDbi_WrData(0x96);
  
  TftDbi_WrCmd(TFT_CMD_WR_MADCTL);
  TftDbi_WrData(0x48);
  
  TftDbi_WrCmd(0xB4);   //
  TftDbi_WrData(0x01);
  
  TftDbi_WrCmd(0xB6);  //��ʾ���ܿ���:
  TftDbi_WrData(0xA0);
  
  TftDbi_WrCmd(0xB7);  //Entry Mode Set
  TftDbi_WrData(0xC6); //0xc0: RGB565ʱ����λΪ0, 0x06: ��ɫ������ʾ
  
  TftDbi_WrCmd(0xe8);  //Display Output Ctrl Adjust ��ʼ����
  TftDbi_WrData(0x40);
  TftDbi_WrData(0x8a);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x29);
  TftDbi_WrData(0x19);
  TftDbi_WrData(0xa5);
  TftDbi_WrData(0x33);
  
  TftDbi_WrCmd(0xc1);  //Power Control 2
  TftDbi_WrData(0x06);

  TftDbi_WrCmd(0xc2); //Power Control 3
  TftDbi_WrData(0xa7);

  TftDbi_WrCmd(0xc5);  //VCOM Control VCOM��ѹ
  TftDbi_WrData(0x18);

  TftDbi_WrCmd(0xe0); //Positive Voltage Gamma Control
  TftDbi_WrData(0xf0);
  TftDbi_WrData(0x09);
  TftDbi_WrData(0x0b);
  TftDbi_WrData(0x06);
  TftDbi_WrData(0x04);
  TftDbi_WrData(0x15);
  TftDbi_WrData(0x2f);
  TftDbi_WrData(0x54);
  TftDbi_WrData(0x42);
  TftDbi_WrData(0x3c);
  TftDbi_WrData(0x17);
  TftDbi_WrData(0x14);
  TftDbi_WrData(0x18);
  TftDbi_WrData(0x1b);

  TftDbi_WrCmd(0xe1); //Negative Voltage Gamma Control
  TftDbi_WrData(0xf0);
  TftDbi_WrData(0x09);
  TftDbi_WrData(0x0b);
  TftDbi_WrData(0x06);
  TftDbi_WrData(0x04);
  TftDbi_WrData(0x03);
  TftDbi_WrData(0x2d);
  TftDbi_WrData(0x43);
  TftDbi_WrData(0x42);
  TftDbi_WrData(0x3b);
  TftDbi_WrData(0x16);
  TftDbi_WrData(0x14);
  TftDbi_WrData(0x17);
  TftDbi_WrData(0x1b);
  
  TftDbi_WrCmd(0xf0);//Display Output Ctrl Adjust ��������
  TftDbi_WrData(0x3c);
  
  TftDbi_WrCmd(0xf0);
  TftDbi_WrData(0x69);

  TftDbi_WrCmd(0x3A);//TFT_DCOL_PIXEL_16BIT
  TftDbi_WrData(0x55);//��101�� = 16bit/pixel
  TftDbi_WrCmd(TFT_CMD_WR_INVON);//��������(��������)
}

//----------------------------HX8357Ӳ������----------------------------------
static void _HwCfg_HX8357(void)
{
  //�ο�: https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html  

}
/*******************************************************************************
                           ��غ���-��ͨ����ʵ��
********************************************************************************/

//---------------------------------��ʼ������-----------------------------------
//���ظ��豸������
signed char TftDbi_Init(void)
{
  TftDbi.Timer = 0;
  TftDbi_HwInit();
  
  //�����ʶ����Ϣ,��4������: 1��Ч 2����ID 3�����汾 4����ID
  unsigned char Id = TftDbi_RdDataCmd(TFT_CMD_RD_DIDIF);
  //unsigned short Info = TftDbi_RdDataS();

  //���ݶ�ȡ���豸ID������ʾ��
  if(Id == 0x6B)_HwCfg_ST7796S();
  else _HwCfg_ST7796S();
  
  //���IC�޹س�ʼ��
  TftDbi_WrCmd(TFT_CMD_WR_SLPOUT);
  DelayMs(120); //Delay 120ms

  TftDbi_WrCmd(TFT_CMD_WR_DISPON); //Display ON
  //������Ϊ��ɫ
  TftDbi_WrCmd(TFT_CMD_WR_RAM); 
  for(unsigned long Ram = 0; Ram < (TFT_DRV_H_PIXEl * TFT_DRV_V_PIXEl); Ram++){
    TftDbi_B16_WrColor(0);
  }
  
  return 0;
}

//---------------------------------������-----------------------------------
//��16ms������ɨ��
void TftDbi_Task(void)
{
  if(!TftDbi.Timer) return;
  TftDbi.Timer--;
  if(TftDbi.Timer) return;   
  //�ڴ��дʱ��ǿ�Ʒ��ָ��
  if((TftDbi.LastCmd == TFT_CMD_RD_RAM) || (TftDbi.LastCmd == TFT_CMD_WR_RAM)) { 
    TftDbi_WrCmd(TFT_CMD_WR_NOP);
    TftDbi_CsDis();
  }
}


