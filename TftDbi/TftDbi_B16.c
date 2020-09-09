/*******************************************************************************

                    TFT MCU�ӿ�����-ʹ��TypeB 16λ����ʱʵ��
�˽ӿ������ʹ����ʾ���������޹�
********************************************************************************/

#include "TftDbi.h"
#include "TftDbi_B16.h"
#include "Color.h"

//--------------------------------Ӳ����ʼ������------------------------------
void TftDbi_HwInit(void)
{
  TftDbi_cbIoCfg();
  //Ӳ����λ
  TftDbi_cbStartRst();
  TftDbi_cbDelayHwRst();
  TftDbi_cbEndRst();
  TftDbi_cbDelayHwRstFinal();  //�ȴ���λ���
  //��ʼ���Ĵ������ڴ�
}

//---------------------------------дָ���----------------------------------
//ָ��̶�Ϊ1��Byte
void TftDbi_WrCmd(unsigned char Cmd)
{
  TftDbi_cbCsEn();
  TftDbi_cbDelayTcs(); //Ƭѡ����ʱ��
  TftDbi_cbRsCmd();
  TftDbi_cbDbOut();   //�����
  TftDbi_cbWrClkL();  //�͵�ƽʱ������
  TftDbi_cbDbWr(Cmd);
  //�ȴ��ڼ临λ�ṹ  
  TftDbi_cbDelayTwrl();//�ȴ������ȶ�
  TftDbi.LastCmd = Cmd;
  TftDbi.Timer = TFT_DRV_TIM_OV;
  TftDbi_cbWrClkH();  //����������
  TftDbi_cbDelayTwrh();  //�ȴ�����
  
  TftDbi_cbRsData(); //���ָ�����״̬,Ƭѡ����
}

//----------------------------------Ƭѡ��ֹ-----------------------------------
//��дָ��ʱ�Զ�Ƭѡ����������ֶ���ֹ������ȶ��ԡ�
void TftDbi_CsDis(void)
{
  TftDbi_cbCsDis();  
  TftDbi_cbDelayTchw(); //�ߵ�ƽ���ʱ��
}

//------------------------------------д���ݺ���-------------------------------
void TftDbi_WrData(unsigned char Data)
{
  //Ĭ�������״̬���������
  TftDbi_cbWrClkL();  //�͵�ƽʱ������
  TftDbi_cbDbWr(Data);  
  TftDbi_cbDelayTwrl();//�ȴ������ȶ�
  TftDbi_cbWrClkH();  //����������
  TftDbi_cbDelayTwrh();  //�ȴ�����  
}

//------------------------------------�����ݺ���-------------------------------
//ע������ָ��������׸�����Ϊ��Чֵ
unsigned char TftDbi_RdData(void)
{
  TftDbi_cbDbIn();   //Ĭ��Ϊ���״̬��ת��Ϊ����
  TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
  TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
  unsigned char Data = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbRdClkH();  //������֪��������
  
  TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  //ע�����ָ���֧���������أ���дָ��ʱ�ὫDBתΪ���״̬��
  
  return Data;
}

extern unsigned long  Color_RGB666_2RGB[];


//-----------------------------ֱ��д16λ��ɫ-----------------------------------
//�ײ�ʵ�����
void TftDbi_B16_WrColor(unsigned short ColorB16)
{
  TftDbi_cbWrClkL();  //�͵�ƽʱ������
  TftDbi_cbDbWr(ColorB16);  
  TftDbi_cbDelayTwrl();//�ȴ������ȶ�
  TftDbi_cbWrClkH();  //����������
  TftDbi_cbDelayTwrh();  //�ȴ����� 
}

//-----------------------------------д��ɫ����--------------------------------
void TftDbi_WrColor(Color_t Color)
{
  TftDbi_cbWrClkL();  //�͵�ƽʱ������
  //����ɫת��
  #ifdef SUPPORT_COLOR_RGB666
    unsigned long RGB24;
    if(Color < COLOR_COUNT) RGB24 = Color_RGB666_2RGB[Color];
    else RGB24 = *(TftDrv_pcbGetUserCLut() + (Color - COLOR_COUNT));
    //ת��Ϊ565ģʽ
    unsigned short Data = ((RGB24 >> 3) & 0x001f) |            /*B*/
                            ((RGB24 >> (8 + 2 - 5)) & 0x07E0) |  /*G*/
                            ((RGB24 >> (16 + 3 - 11)) & 0xF800); /*R*/
  #else 
    unsigned short Data = Color;
  #endif
  
  TftDbi_cbDbWr(Data);  
  TftDbi_cbDelayTwrl();//�ȴ������ȶ�
  TftDbi_cbWrClkH();  //����������
  TftDbi_cbDelayTwrh();  //�ȴ�����  
}

//-----------------------------------����ɫ����--------------------------------
//��״̬����Ϊ:0��������������ʼ������������
Color_t TftDbi_RdColor(void)
{
  TftDbi_cbDbIn();   //Ĭ��Ϊ���״̬��ת��Ϊ����
  TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
  TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
  Color_t Data = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbWrClkH();  //������֪��������
  
  //TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  //ע�����ָ���֧���������أ���дָ��ʱ�ὫDBתΪ���״̬��

  return Data;
}


