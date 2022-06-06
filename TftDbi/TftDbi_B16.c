/*******************************************************************************

         TFT MCU�ӿ�����-TftDbi��غ���ʹ��16λ����ʱ�������ɫ������ʵ��
//��ʵ�ֽ�֧��SUPPORT_RGB_HW_565,���������ʹ����ʾ���������޹�
********************************************************************************/
#include "TftDbi.h"
#include "TftDbi_Bx.h"
#include "Color.h"

#ifdef SUPPORT_RGB_HW_565

//------------------------------ɫϵת���ⲿ����--------------------------------
//RGB666����ɫת��Ϊ��ʾ����ɫϵ��
extern unsigned short  Color_RGB666_2RGB[]; 

//-----------------------------------д��ɫ����--------------------------------
//��ʵ�ֽ�֧��SUPPORT_RGB_HW_565
void TftDbi_WrColor(Color_t Color)
{
  TftDbi_cbWrClkL();  //�͵�ƽʱ������
  unsigned short Data;
  
  //ϵͳ��ɫϵת��Ϊ��ʾ����ɫϵ
  #ifdef SUPPORT_COLOR_RGB666//����ɫ
    if(Color < COLOR_COUNT) Data = Color_RGB666_2RGB[Color];
    else Data = *(TftDrv_pcbGetUserCLut() + (Color - COLOR_COUNT));
  #else 
    Data = Color; //Ĭ���붨��һһ��Ӧ
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
  unsigned short Data = TftDbi_cbDbRd(); //������֮ǰ��������  
  TftDbi_cbWrClkH();  //������֪��������
  
  //TftDbi_cbDelayTrdh(); //Ϊ�¸�����׼��
  //ע�����ָ���֧���������أ���дָ��ʱ�ὫDBתΪ���״̬��
  
  //��ʾ����ɫϵת��Ϊϵͳ��ɫϵ
  #ifdef SUPPORT_COLOR_RGB666
    Color_t Color = Data; //��
  #else
    Color_t Color = Data; //Ĭ���붨��һһ��Ӧ
  #endif
  return Color;
}

#endif //SUPPORT_RGB_HW_565

