/*******************************************************************************

            TFT MCU�ӿ�����-TftDbi��غ���ʹ��8λ����ʱ�������ɫ������ʵ��
//�˽ӿ�֧��RGB565, RGB666,RGB888ɫ�򣬴�ʵ������������ʹ����ʾ���������޹�
********************************************************************************/

#include "TftDbi.h"
#include "TftDbi_Bx.h"
#include "Color.h"

//------------------------------ɫϵת���ⲿ����--------------------------------
//RGB666����ɫת��Ϊ��ʾ����ɫϵ��
extern unsigned short  Color_RGB666_2RGB[]; 

//-----------------------------------д��ɫ����--------------------------------
void TftDbi_WrColor(Color_t Color)
{
  TftDbi_cbWrClkL();  //�͵�ƽʱ׼��������
  #ifndef SUPPORT_RGB_HW_565 //ȫɫ��
    unsigned long Data;
  #else
    unsigned short Data;
  #endif
  
  //ϵͳ��ɫϵת��Ϊ��ʾ����ɫϵ
  #ifdef SUPPORT_COLOR_RGB666//����ɫ
    if(Color < COLOR_COUNT) Data = Color_RGB666_2RGB[Color];
    else Data = *(TftDrv_pcbGetUserCLut() + (Color - COLOR_COUNT));
  #else 
    Data = Color; //�����붨��һһ��Ӧ
  #endif
    
  //16~23b�ͳ�,��ɫ�Ը�λ��ǰ�ͳ�
  #ifndef SUPPORT_RGB_HW_565 //GB666,RGB888��8λ��֧�����3λɫֵ����
    TftDbi_cbDbWr(Data >> 24);  
    TftDbi_cbDelayTwrl();//�ȴ������ȶ�
    TftDbi_cbWrClkH();  //����������
    TftDbi_cbDelayTwrh();  //�ȴ�����
    TftDbi_cbWrClkL();    //�͵�ƽʱ׼��������
  #endif
  //8~15b�ͳ�
  TftDbi_cbDbWr(Data >> 8);  
  TftDbi_cbDelayTwrl();//�ȴ������ȶ�
  TftDbi_cbWrClkH();  //����������
  TftDbi_cbDelayTwrh();  //�ȴ�����
  TftDbi_cbWrClkL();    //�͵�ƽʱ׼��������
  
  //0~7b�ͳ�
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
  unsigned char Data0 = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbWrClkH();  //������֪��������
  
  TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
  TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
  unsigned char Data1 = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbWrClkH();  //������֪��������  
  
  #ifndef SUPPORT_RGB_HW_565 //GB666,RGB888��8λ��֧�����3λɫֵ����
    TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
    TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
    unsigned char Data2 = TftDbi_cbDbRd(); //������֮ǰ��������  
    TftDbi_cbWrClkH();  //������֪��������
    Color_t Color = ((unsigned long)Data0 << 16) | 
                    ((unsigned long)Data1 << 8) | Data2;
  #else
    Color_t Color = ((unsigned short)Data0 << 8) | Data1;
  #endif

  //TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  //ע�����ָ���֧���������أ���дָ��ʱ�ὫDBתΪ���״̬��
  
  //��ʾ����ɫϵת��Ϊϵͳ��ɫϵ
  #ifdef SUPPORT_COLOR_RGB666
    //��
  #endif
  return Color;
}


