/*******************************************************************************

                     TFT DBI����ͳһ�ӿ�-035��ʾ����غ���ʵ��
�˽ӿ������ʹ����ʾ���������޹�
********************************************************************************/
#include "TftDbi.h"
#include "TftCmd_035.h"
#include "Delay.h"

#include "TftDbi.h" //������Ӳ���޹�

/*******************************************************************************
                           Ӳ����غ���
********************************************************************************/
//�����ο���https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html

//----------------------------ST7796S��������----------------------------------
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

  #ifdef SUPPORT_TFT_DRV_MV //����ʱ
    TftDbi_WrCmd(0x36);
    TftDbi_WrData(0x2C); 
  #else  //����ʱĬ��
    
  #endif
  
  TftDbi_WrCmd(0x3A);//TFT_DCOL_PIXEL_16BIT
  TftDbi_WrData(0x55);//��101�� = 16bit/pixel
}

//----------------------------HX8357 ��������----------------------------------
static void _HwCfg_HX8357(void)
{
  //�ο�: https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html  
  //      https://bbs.csdn.net/topics/380164559
  DelayMs(10);
  TftDbi_WrCmd(0x11);//Sleep Out 
  DelayMs(120); 

  TftDbi_WrCmd(0xEE);//Set EQ 
  TftDbi_WrData(0x02); 
  TftDbi_WrData(0x01); 
  TftDbi_WrData(0x02); 
  TftDbi_WrData(0x01); 

  TftDbi_WrCmd(0xED);//Set DIR TIM 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x9A); 
  TftDbi_WrData(0x9A); 
  TftDbi_WrData(0x9B); 
  TftDbi_WrData(0x9B); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0xAE); 
  TftDbi_WrData(0xAE); 
  TftDbi_WrData(0x01); 
  TftDbi_WrData(0xA2); 
  TftDbi_WrData(0x00); 

  TftDbi_WrCmd(0xB4);//Set RM, DM 
  TftDbi_WrData(0x00);// 

  TftDbi_WrCmd(0xC0);  //Set Panel Driving
  TftDbi_WrData(0x10); //REV SM GS
  TftDbi_WrData(0x3B); // NL[5:0] 
  TftDbi_WrData(0x00); //SCN[6:0] 
  TftDbi_WrData(0x02); //NDL 0 PTS[2:0]
  TftDbi_WrData(0x11); //PTG ISC[3:0] 

  TftDbi_WrCmd(0xC1);//
  TftDbi_WrData(0x10);//line inversion

  TftDbi_WrCmd(0xC8);//Set Gamma 
  TftDbi_WrData(0x00);  //KP1,KP0
  TftDbi_WrData(0x46);  //KP3,KP2
  TftDbi_WrData(0x12);  //KP5,KP4
  TftDbi_WrData(0x20);  //RP1,RP0
  TftDbi_WrData(0x0c);  //VRP0  01
  TftDbi_WrData(0x00);  //VRP1
  TftDbi_WrData(0x56);  //KN1,KN0
  TftDbi_WrData(0x12);  //KN3,KN2
  TftDbi_WrData(0x67);  //KN5,KN4
  TftDbi_WrData(0x02);  //RN1,RN0
  TftDbi_WrData(0x00);  //VRN0
  TftDbi_WrData(0x0c);  //VRN1  01

  TftDbi_WrCmd(0xD0);//Set Power 
  TftDbi_WrData(0x44);//DDVDH :5.28
  TftDbi_WrData(0x42); // BT VGH:15.84    VGL:-7.92
  TftDbi_WrData(0x06);//VREG1  4.625V

  TftDbi_WrCmd(0xD1);//Set VCOM 
  TftDbi_WrData(0x43); //VCOMH
  TftDbi_WrData(0x16);

  TftDbi_WrCmd(0xD2); 
  TftDbi_WrData(0x04); 
  TftDbi_WrData(0x22); //12

  TftDbi_WrCmd(0xD3); 
  TftDbi_WrData(0x04); 
  TftDbi_WrData(0x12); 

  TftDbi_WrCmd(0xD4); 
  TftDbi_WrData(0x07); 
  TftDbi_WrData(0x12); 

  TftDbi_WrCmd(0xE9); //Set Panel
  TftDbi_WrData(0x00);

  TftDbi_WrCmd(0xC5); //Set Frame rate
  TftDbi_WrData(0x08);  //61.51Hz

  TftDbi_WrCmd(0X0036);
  #ifdef SUPPORT_TFT_DRV_MV //  ����ʱ
    TftDbi_WrData(0X002a); 
  #else  //����ʱ
    TftDbi_WrData(0X000a);
  #endif
  
  TftDbi_WrCmd(0X003A);
  TftDbi_WrData(0X0005);

  TftDbi_WrCmd(0X002A);
  TftDbi_WrData(0X0000);
  TftDbi_WrData(0X0000);
  TftDbi_WrData(0X0001);
  TftDbi_WrData(0X003F);

  TftDbi_WrCmd(0X002B);
  TftDbi_WrData(0X0000);
  TftDbi_WrData(0X0000);
  TftDbi_WrData(0X0001);
  TftDbi_WrData(0X00E0);
  DelayMs(120);


  TftDbi_WrCmd(0x35);
  TftDbi_WrData(0x00);//TE ON

  //TftDbi_WrCmd(0x29); //Display On
  //DelayMs(5);
}

//----------------------------ILI9488 ��������----------------------------------
static void _HwCfg_ILI9488(void)
{
  //�ο���https://bbs.elecfans.com/jishu_476163_1_1.html
  TftDbi_WrCmd(0xE0); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x0E); 
  TftDbi_WrData(0x15); 
  TftDbi_WrData(0x06); 
  TftDbi_WrData(0x13); 
  TftDbi_WrData(0x09); 
  TftDbi_WrData(0x3A); 
  TftDbi_WrData(0xAC); 
  TftDbi_WrData(0x4F); 
  TftDbi_WrData(0x05); 
  TftDbi_WrData(0x0D); 
  TftDbi_WrData(0x0B); 
  TftDbi_WrData(0x33); 
  TftDbi_WrData(0x3B); 
  TftDbi_WrData(0x0F);  
   
  TftDbi_WrCmd(0xE1); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x0E); 
  TftDbi_WrData(0x16); 
  TftDbi_WrData(0x05); 
  TftDbi_WrData(0x13); 
  TftDbi_WrData(0x08); 
  TftDbi_WrData(0x3B); 
  TftDbi_WrData(0x9A); 
  TftDbi_WrData(0x50); 
  TftDbi_WrData(0x0A); 
  TftDbi_WrData(0x13); 
  TftDbi_WrData(0x0F); 
  TftDbi_WrData(0x31); 
  TftDbi_WrData(0x36); 
  TftDbi_WrData(0x0F); 

  TftDbi_WrCmd(0xC0); 
  TftDbi_WrData(0x10); 
  TftDbi_WrData(0x10); 
   
  TftDbi_WrCmd(0xC1); 
  TftDbi_WrData(0x44); 

  TftDbi_WrCmd(0xC5); 
  TftDbi_WrData(0x00); 
  TftDbi_WrData(0x10); 
  TftDbi_WrData(0x80); 

  TftDbi_WrCmd(0x36);
  #ifdef SUPPORT_TFT_DRV_MV //����ʱ
    TftDbi_WrData(0x2C); 
  #else  //����ʱ
    TftDbi_WrData(0x08); 
  #endif
  
  TftDbi_WrCmd(0x3A);//Interface Mode Control
  TftDbi_WrData(0x55);

  TftDbi_WrCmd(0x21); 

  TftDbi_WrCmd(0XB0);  //Interface Mode Control  
  TftDbi_WrData(0x00); 
  TftDbi_WrCmd(0xB1);   //Frame rate 70HZ  
  TftDbi_WrData(0xB0); 

  TftDbi_WrCmd(0xB4); 
  TftDbi_WrData(0x02);   
  TftDbi_WrCmd(0xB6); //RGB/MCU Interface Control
  TftDbi_WrData(0x02); 
  TftDbi_WrData(0x22); 

  TftDbi_WrCmd(0xB7); 
  TftDbi_WrData(0xC6); 

  TftDbi_WrCmd(0XBE);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x04);

  TftDbi_WrCmd(0xE9); 
  TftDbi_WrData(0x00);
   
  TftDbi_WrCmd(0XF7);    
  TftDbi_WrData(0xA9); 
  TftDbi_WrData(0x51); 
  TftDbi_WrData(0x2C); 
  TftDbi_WrData(0x82);

  TftDbi_WrCmd(0x11);

  DelayMs(5);

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
  unsigned char ManufacturerID = TftDbi_RdDataCmd(TFT_CMD_RD_DIDIF);
  //unsigned short Info = TftDbi_RdDataS();

  //���ݶ�ȡ���豸ID������ʾ��
  if(ManufacturerID == 0x6B){//0x6b: W350BE024Zʵ��
    _HwCfg_ST7796S();     
    TftDbi_WrCmd(TFT_CMD_WR_INVON);//��������(��������)
  }
  else if(ManufacturerID == 0x11){//0x11����д(δ��֤)
    _HwCfg_HX8357();
  }
  else if(ManufacturerID == 0x22){//0x22����д(δ��֤)
    _HwCfg_ILI9488();
    TftDbi_WrCmd(TFT_CMD_WR_INVON);//��������(��������)
  }
  else if(ManufacturerID == 0x54){//SX035SVGA40P-006ʵ��
    _HwCfg_ILI9488();
    TftDbi_WrCmd(TFT_CMD_WR_INVOFF);//��������(�޴˾ֻ��Ƿ���)
  }
  else{//Ĭ������: ʵ��:JPY3553-4OP(id=0xff)
    _HwCfg_ST7796S(); 
  }
  
  //���IC�޹س�ʼ��
  TftDbi_WrCmd(TFT_CMD_WR_SLPOUT);
  DelayMs(120); //Delay 120ms

  TftDbi_WrCmd(TFT_CMD_WR_DISPON); //Display ON
  //������Ϊ��ɫ
  TftDbi_WrCmd(TFT_CMD_WR_RAM); 
  for(unsigned long Ram = 0; Ram < (TFT_DRV_H_PIXEl * TFT_DRV_V_PIXEl); Ram++){
    TftDbi_WrColor(0);
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


