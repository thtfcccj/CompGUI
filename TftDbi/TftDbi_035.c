/*******************************************************************************

                     TFT DBI操作统一接口-035显示屏相关函数实现
此接口与具体使用显示屏控制器无关
********************************************************************************/
#include "TftDbi.h"
#include "TftCmd_035.h"
#include "Delay.h"

#include "TftDbi.h" //驱动与硬件无关
#include "Plot_cbHw.h" //Plot_cbAbsLocalArea

/*******************************************************************************
                           显示屏相关启动代码-用函数时
********************************************************************************/
#ifndef SUPPORT_TFT_DRV_INIT_TBL
//横屏参考：https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html
//----------------------------ST7796S启动代码----------------------------------
static void _HwCfg_ST7796S(void)
{
  //参考：https://wenku.baidu.com/view/e3d53f4c4a73f242336c1eb91a37f111f0850d5d.html
  TftDbi_WrCmd(0xf0); //Command Set Control
  TftDbi_WrData(0xc3);
  
  TftDbi_WrCmd(0xf0);
  TftDbi_WrData(0x96);
  
  TftDbi_WrCmd(TFT_CMD_WR_MADCTL);
  TftDbi_WrData(0x48);
  
  TftDbi_WrCmd(0xB4);   //
  TftDbi_WrData(0x01);
  
  TftDbi_WrCmd(0xB6);  //显示功能控制:
  TftDbi_WrData(0xA0);
  
  TftDbi_WrCmd(0xB7);  //Entry Mode Set
  TftDbi_WrData(0xC6); //0xc0: RGB565时，低位为0, 0x06: 蓝色正常显示
  
  TftDbi_WrCmd(0xe8);  //Display Output Ctrl Adjust 开始调整
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

  TftDbi_WrCmd(0xc5);  //VCOM Control VCOM电压
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
  
  TftDbi_WrCmd(0xf0);//Display Output Ctrl Adjust 结束调整
  TftDbi_WrData(0x3c);
  
  TftDbi_WrCmd(0xf0);
  TftDbi_WrData(0x69);

  TftDbi_WrCmd(0x3A);//TFT_DCOL_PIXEL_16BIT
  TftDbi_WrData(0x55);//‘101’ = 16bit/pixel
  
  #ifdef SUPPORT_TFT_DRV_MV //横屏时
    TftDbi_WrCmd(0x36);
    TftDbi_WrData(0x2C); 
  #else  //竖屏时默认
    
  #endif  
}

//----------------------------HX8357 启动代码----------------------------------
static void _HwCfg_HX8357(void)
{
  //参考: https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html  
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

  TftDbi_WrCmd(0x36);
  #ifdef SUPPORT_TFT_DRV_MV //  横屏时
    TftDbi_WrData(0x2a); 
  #else  //竖屏时
    TftDbi_WrData(0x0a);
  #endif
  
  TftDbi_WrCmd(0x3A);
  TftDbi_WrData(0x05);

  TftDbi_WrCmd(0x2A);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x01);
  TftDbi_WrData(0x3F);

  TftDbi_WrCmd(0x2B);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x00);
  TftDbi_WrData(0x01);
  TftDbi_WrData(0xE0);
  DelayMs(120);

  TftDbi_WrCmd(0x35);
  TftDbi_WrData(0x00);//TE ON

  //TftDbi_WrCmd(0x29); //Display On
  //DelayMs(5);
}

//----------------------------ILI9488 启动代码----------------------------------
static void _HwCfg_ILI9488(void)
{
  //参考：https://bbs.elecfans.com/jishu_476163_1_1.html
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
  #ifdef SUPPORT_TFT_DRV_MV //横屏时
    #ifdef SUPPORT_TFT_DRV_MV_ANTI //反向放置时(下部在左侧)
      TftDbi_WrData(0x2C);
    #else //正向放置时(下部在右侧)
      TftDbi_WrData(0x68); 
    #endif
  #else  //竖屏时
    TftDbi_WrData(0x08); 
  #endif
  
  TftDbi_WrCmd(0x3A);//Interface Mode Control
  TftDbi_WrData(0x55);

  TftDbi_WrCmd(0x21); //显示反转

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
#else
/*******************************************************************************
                           显示屏相关启动代码-用表实现时
********************************************************************************/

//----------------------------ST7796S启动代码----------------------------------
//由原_HwCfg_ST7796S[]手动生成
static const unsigned char _FunData_ST7796S[] = {
  /*0x44*/  0xf0,0xc3,0xf0,0x96,
  /*0x44*/  TFT_CMD_WR_MADCTL, 0x48, 0xB4, 0x01,
  /*0x44*/  0xB6,  /*显示功能控制:*/ 0xA0,0xB7,  /*Entry Mode Set*/0xC6,  //0xc0: RGB565时，低位为0, 0x06: 蓝色正常显示
  /*0x54*/  0xe8,  /*Display Output Ctrl Adjust 开始调整*/0x40, 0x8a, 0x00,
  /*0x55*/  0x00,0x29,0x19,0xa5,
  /*0x11*/  0x33,0xc1,  /*Power Control 2*/0x06,0xc2, //Power Control 3
  /*0x11*/  0xa7,0xc5,  /*VCOM Control VCOM电压*/0x18,0xe0, //Positive Voltage Gamma Control
  /*0x55*/  0xf0,0x09,0x0b,0x06,
  /*0x55*/  0x04,0x15,0x2f,0x54,
  /*0x55*/  0x42,0x3c,0x17,0x14,
  /*0x45*/  0x18,0x1b,0xe1, /*Negative Voltage Gamma Control*/0xf0,
  /*0x55*/  0x09,0x0b,0x06,0x04,
  /*0x55*/  0x03,0x2d,0x43,0x42,
  /*0x55*/  0x3b,0x16,0x14,0x17,
  /*0x11*/  0x1b,0xf0,/*Display Output Ctrl Adjust 结束调整*/0x3c,0xf0,
  /*0x11*/  0x69,
  #ifdef SUPPORT_TFT_DRV_MV //横屏时
    0x36,0x2C, 
  #else  //竖屏时默认
    
  #endif
};
static const unsigned char _FunCode_ST7796S[] = {
  0x44,   0x44,    0x44,    0x54,   
  0x55,   0x11,    0x11,    0x55, 
  0x55,   0x55,    0x45,    0x55, 
  0x55,   0x55,    0x55,    0x11, 
  0x11,  
};

//----------------------------HX8357启动代码----------------------------------
//由原_HwCfg_HX8357[]手动生成
static const unsigned char _FunData_HX8357[] = {
  /*0x22*/ 10,0x11,/*Sleep Out*/120, 0xEE,//Set EQ 
  /*0x55*/ 0x02, 0x01, 0x02, 0x01, 
  /*0x54*/ 0xED,/*Set DIR TIM */0x00, 0x00, 0x9A, 
  /*0x55*/ 0x9A, 0x9B, 0x9B, 0x00, 
  /*0x55*/ 0x00, 0x00, 0x00, 0xAE, 
  /*0x55*/ 0xAE, 0x01, 0xA2, 0x00, 
  /*0x44*/ 0xB4,/*Set RM, DM */0x00,0xC0,  /*Set Panel Driving*/ 0x10, //REV SM GS
  /*0x55*/ 0x3B, /* NL[5:0] */0x00, /*SCN[6:0] */0x02, /*NDL 0 PTS[2:0]*/ 0x11, /*PTG ISC[3:0]*/
  /*0x44*/ 0xC1, 0x10,/*line inversion*/0xC8,/*Set Gamma */0x00,  //KP1,KP0
  /*0x55*/ 0x46,  /*KP3,KP2*/ 0x12,/*KP5,KP4*/ 0x20,/*RP1,RP0*/0x0c,/*VRP0  01*/
  /*0x55*/ 0x00,  /*VRP1*/0x56, /*KN1,KN0*/0x12,  /*KN3,KN2*/0x67,  /*KN5,KN4*/
  /*0x15*/ 0x02,  /*RN1,RN0*/0x00, /*VRN0*/0x0c,  /*VRN1  01*/0xD0,/*Set Power*/
  /*0x15*/ 0x44,/*DDVDH :5.28*/0x42, /*BT VGH:15.84    VGL:-7.92*/ 0x06,/*VREG1  4.625V*/0xD1,/*Set VCOM*/ 
  /*0x45*/ 0x43, /*VCOMH*/0x16,0xD2, 0x04, 
  /*0x51*/ 0x22, 0xD3, 0x04, 0x12, 
  /*0x14*/ 0xD4, 0x07, 0x12, 0xE9, /*Set Panel*/
  /*0x11*/ 0x00,0xC5, /*Set Frame rate*/0x08, /*61.51Hz*/0x36,
  /*0x11*/ 
  #ifdef SUPPORT_TFT_DRV_MV //  横屏时
    0x2a, 
  #else  //竖屏时
    0x0a,
  #endif
           0x3A,0x05,0x2A,
  /*0x55*/ 0x00,0x00,0x01,0x3F,
  /*0x54*/ 0x2B,0x00,0x00,0x01,
  /*0x49*/ 0xE0,120,0x35,0x00,/*TE ON*/
};
static const unsigned char _FunCode_HX8357[] = {
  0x22,   0x55,    0x54,    0x55,   
  0x55,   0x44,    0x44,    0x55, 
  0x45,   0x55,    0x45,    0x15, 
  0x15,   0x45,    0x51,    0x14, 
  0x11,   0x11, 
};

//----------------------------ILI9488启动代码----------------------------------
//由原_HwCfg_ILI9488[]手动生成
static const unsigned char _FunData_ILI9488[] = {
  /*0x54*/ 0xE0, 0x00, 0x0E, 0x15, 
  /*0x55*/ 0x06, 0x13, 0x09, 0x3A,
  /*0x55*/ 0xAC, 0x4F, 0x05, 0x0D, 
  /*0x55*/ 0x0B, 0x33, 0x3B, 0x0F,  
  /*0x54*/ 0xE1, 0x00, 0x0E, 0x16, 
  /*0x55*/ 0x05, 0x13, 0x08, 0x3B,
  /*0x55*/ 0x9A, 0x50, 0x0A, 0x13, 
  /*0x55*/ 0x0F, 0x31, 0x36, 0x0F, 
  /*0x14*/ 0xC0, 0x10, 0x10, 0xC1,
  /*0x51*/ 0x44, 0xC5, 0x00, 0x10, 
  /*0x11*/ 0x80, 0x36,
  #ifdef SUPPORT_TFT_DRV_MV //横屏时
    #ifdef SUPPORT_TFT_DRV_MV_ANTI //反向放置时(下部在左侧)
      0x2C,
    #else //正向放置时(下部在右侧)
      0x68, 
    #endif
  #else  //竖屏时
      0x08, 
  #endif
            0x3A,//Interface Mode Control
  /*0x41*/ 0x55,0x21, /*显示反转*/0XB0,  /*Interface Mode Control */ 0x00, 
  /*0x44*/ 0xB1,/*Frame rate 70HZ*/ 0xB0, 0xB4, 0x02, 
  /*0x14*/ 0xB6, /*RGB/MCU Interface Control*/0x02, 0x22, 0xB7, 
  /*0x51*/ 0xC6, 0XBE,0x00,0x04,
  /*0x55*/ 0xE9, 0x00,0XF7,0xA9, 
  /*0x15*/ 0x51, 0x2C, 0x82,0x11,
  /*0x02*/ 5,
};
static const unsigned char _FunCode_ILI9488[] = {
  0x54,   0x55,    0x54,    0x55,   
  0x54,   0x55,    0x55,    0x55, 
  0x14,   0x51,    0x11,    0x41, 
  0x44,   0x14,    0x51,    0x55, 
  0x15,   0x02, 
};

//----------------------------总的启动代码----------------------------------
static const unsigned char * const _pFunData[] = {//函数的数据
  _FunData_ST7796S,  //0:7796
  _FunData_HX8357,  //1:8357
  _FunData_ILI9488,  //2:9488  
  
};
static const unsigned short _FunSize[] = {//函数内数据个数
  sizeof(_FunData_ST7796S),  //0:7796
  sizeof(_FunData_HX8357),  //1:8357
  sizeof(_FunData_ILI9488),  //2:9488
};

static const unsigned char * const _FunCode[] = {//函数的功能
  _FunCode_ST7796S,  //0:7796
  _FunCode_HX8357,  //1:8357
  _FunCode_ILI9488,  //2:9488
};

//此函数以空间优化为准。
static void _HwCfg(unsigned char Id)
{
  const unsigned char *pData = _pFunData[Id];
  const unsigned char *pCode = _FunCode[Id];
  const unsigned char Count = _FunSize[Id];
  for(unsigned char Pos = 0; Pos < Count; Pos++){
    unsigned char CodeDiffPos = Pos & 0x03;
    unsigned char Data = *pData++;
    switch((*pCode >> (CodeDiffPos * 2)) & 0x03){
      case 0: TftDbi_WrCmd(Data); break;
      case 1: TftDbi_WrData(Data); break; 
      case 2: DelayMs(Data); break; 
      //case 3: DelayUs(Data); break;       
    }
    if(CodeDiffPos == 0x03) pCode++;//下一组
  }
};

#define _HwCfg_ST7796S() do{_HwCfg(0); }while(0)
#define _HwCfg_HX8357()  do{_HwCfg(1); }while(0)
#define _HwCfg_ILI9488() do{_HwCfg(2); }while(0)

#endif  //SUPPORT_TFT_DRV_INIT_TBL
/*******************************************************************************
                           相关函数-普通函数实现
********************************************************************************/

//---------------------------------初始化函数-----------------------------------
//返回负设备有问题
signed char TftDbi_Init(void)
{
  TftDbi.Timer = 0;
  TftDbi_HwInit();
  
  //读软件识别信息,回4个参数: 1无效 2工厂ID 3驱动版本 4驱动ID
  unsigned char ManufacturerID = TftDbi_RdDataCmd(TFT_CMD_RD_DIDIF);
  //unsigned short Info = TftDbi_RdDataS();

  //根据读取的设备ID配置显示屏
  if(ManufacturerID == 0x6B){//0x6b: W350BE024Z实测
    _HwCfg_ST7796S();     
    #ifndef SUPPORT_TFT_DRV_MV //坚屏时
     TftDbi_WrCmd(TFT_CMD_WR_INVON);//开启反显
    #endif
  }
  else if(ManufacturerID == 0x11){//0x11随手写(未验证)
    _HwCfg_HX8357();
  }
  else if(ManufacturerID == 0x22){//0x22随手写(未验证)
    _HwCfg_ILI9488();
    TftDbi_WrCmd(TFT_CMD_WR_INVON);//开启反显
  }
  else if(ManufacturerID == 0x54){//SX035SVGA40P-006实测
    _HwCfg_ILI9488();
    TftDbi_WrCmd(TFT_CMD_WR_INVOFF);//关闭正显
  }
  else{//默认驱动: 实测:JPY3553-4OP(id=0xff)
    _HwCfg_ST7796S(); 
  }
  
  //反显状态(注：不能反反得正)
  signed char DispAnti = TftDbi_cbGetDispAnti(ManufacturerID);
  if(DispAnti < 0){//开启反显时
    TftDbi_WrCmd(TFT_CMD_WR_INVON);
  }
  else if(DispAnti  > 0){//取消反显时
    TftDbi_WrCmd(TFT_CMD_WR_INVOFF);
  }
  
  //最后IC无关初始化
  TftDbi_WrCmd(TFT_CMD_WR_SLPOUT);
  DelayMs(120); //Delay 120ms

  TftDbi_WrCmd(TFT_CMD_WR_DISPON); //Display ON
  //先清屏为指定颜色
  Plot_cbAbsLocalArea(0, 0, TFT_DRV_H_PIXEl, TFT_DRV_V_PIXEl);
  Color_t Color = TftDbi_cbGetInitBg();
  for(unsigned long Ram = 0; Ram < (TFT_DRV_H_PIXEl * TFT_DRV_V_PIXEl); Ram++){
    TftDbi_WrColor(Color);
  }
  
  return 0;
}

//---------------------------------任务函数-----------------------------------
//放16ms进程中扫描
void TftDbi_Task(void)
{
  if(!TftDbi.Timer) return;
  TftDbi.Timer--;
  if(TftDbi.Timer) return;   
  //内存读写时，强制封闭指令
  if((TftDbi.LastCmd == TFT_CMD_RD_RAM) || (TftDbi.LastCmd == TFT_CMD_WR_RAM)) { 
    TftDbi_WrCmd(TFT_CMD_WR_NOP);
    TftDbi_CsDis();
  }
}


