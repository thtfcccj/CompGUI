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
//��ʵ�����������ͬ���ܲ�ͬ��ʵ���ȡ���ԣ�����
Color_t TftDbi_RdColor(void)
{
  TftDbi_cbDbIn();   //Ĭ��Ϊ���״̬��ת��Ϊ����
  
  TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
  TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
  unsigned char R = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbRdClkH();  //������֪��������
  
  TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
  TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
  unsigned char G = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbRdClkH();  //������֪��������  
  
  TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  TftDbi_cbRdClkL();  //�͵�ƽʱ׼������ 
  TftDbi_cbDelayTrdl();//�ȴ����ݼ�����ȶ�
  unsigned char B = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbRdClkH();  //������֪��������  
  
  #ifndef SUPPORT_RGB_HW_565 //GB666,RGB888��8λ��֧�����3λɫֵ����
    Color_t Color = ((unsigned long)R << 16) | 
                    ((unsigned long)G << 8) | B;
  #else
    Color_t Color = (unsigned short)(R & 0xF1) << 11;
    Color |= (unsigned short)(G & 0xF3) << 5;
    Color |= (unsigned short)(B & 0xF1) >> 3;    
  #endif

  //TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  //ע�����ָ���֧���������أ���дָ��ʱ�ὫDBתΪ���״̬��
  
  //��ʾ����ɫϵת��Ϊϵͳ��ɫϵ
  #ifdef SUPPORT_COLOR_RGB666
    //��
  #endif
  return Color;
}


