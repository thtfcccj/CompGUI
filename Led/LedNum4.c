/***************************************************************************
                       4λ�������ʾ��غ���ʵ��
***************************************************************************/

#include "LedNum4.h"
#include "Led.h"
#include "LedSign.h"

#include <string.h>
//-----------------------4λ�����������ʾ0-9��ģ----------------------

__flash const unsigned char LED_SIGN_NumLut[10] = \
{LED_SIGN_0,LED_SIGN_1,LED_SIGN_2,LED_SIGN_3,LED_SIGN_4,
 LED_SIGN_5,LED_SIGN_6,LED_SIGN_7,LED_SIGN_8,LED_SIGN_9};

//----------------------4λ����ʾ����-------------------------------
//������ʾ,Ϊ��ʱ��ʾ����,ָ��С����
void LedNum4_Disp(signed short Data,
                  //0~1bit������ʾλ��-1(��������λ)-1 2bit:�Ƿ��������ʾ, 
                  //3bit:������ʱ,��ʱ�Ƿ����0    4~7bitС������ʾ����
                  unsigned char Flag) 
{
  //���С��������
  Led.DotDisp &= ~0x0f;
  Led.DotDisp |= Flag >> 4;
  
  //�������ʾ
  unsigned char Pos = 0;  
  for(; Pos < 4; Pos++) Led.SegDisp[Pos] = 0;
  
  //�����������ʾ
  unsigned short Value;
  Pos = (Flag & 0x03) + 1; //���λ,���==4
  
  if(Data < 0){//��ֵʱ
    if(Flag & 0x04){//������ʾ��ֵʱ
      if(Pos == 4) Pos = 3;//��ȡ��ʾ
      Led.SegDisp[Pos] = LED_SIGN__;//����  
      if(Data < -999) Data = -999; //��������(��δ֧�ֽ�ȡ��ʾ)          
      Value = (unsigned short)(0 - Data); //��Ϊ��
    }
    else Value = 0;//��������ʾ��ʱ,��ʾ0
  }
  else{//��ֵʱ
    Value = (unsigned short)Data;
    //��������:
    if(Pos == 4){//4λ��ʾʱ
      if(Value > 9999) Value = 9999;//�����̽�ȡ��ʾ
    }
    else{//3λ������ʱ
      if((Pos == 3) && (Value > 999)) Value = 999;//3λ��ʾʱ�������̽�ȡ��ʾ
      else if((Pos == 2) && (Value > 99)) Value = 99;//2λ��ʾʱ�������̽�ȡ��ʾ
      if((Flag & 0x0c) == 0x0c) //������ʾ��ʱ
        Led.SegDisp[Pos] = LED_SIGN_0;//��һλ���0��ʾ��
    }
  }
  
  //�����ֵ,�����λ��ʼ
  for(unsigned char i = 0; i < Pos; i++){
      Led.SegDisp[i] = LED_SIGN_NumLut[Value % 10];
      Value /= 10;
  }
}


//----------------------3λ����ʾ����-------------------------------
//����λ���ܣ�3λ������ʾ,Ϊ��ʱ��ʾ����,ָ��С����
void LedNum3_Disp(signed short Data,
                  //0~1bit������ʾλ��-1(��������λ) 2bit:�Ƿ��������ʾ, 
                  //3bit:������ʱ,��ʱ�Ƿ����0    4~6bitС������ʾ����
                  unsigned char Flag) 
{
  //���С��������
  Led.DotDisp &= ~0x07;
  Led.DotDisp |= (Flag >> 4) & 0x07;
  
  //�������3λ��ʾ
  unsigned char Pos = 0;  
  for(; Pos < 3; Pos++) Led.SegDisp[Pos] = 0;
  
  //��������
  if(Data > 999) Data = 999;
  else if(Data < -999) Data = -999;
    
  //�����������ʾ
  unsigned short Value;
  Pos = (Flag & 0x03) + 1; //���λ
  if(Pos > 3) Pos = 3; //������
  
  if(Data < 0){//��ֵʱ
    if(Flag & 0x04){//������ʾ��ֵʱ
      Led.SegDisp[Pos] = LED_SIGN__;//����  
      if(Data < -99) Data = -99; //��������          
      Value = (unsigned short)(0 - Data); //��Ϊ��
    }
    else Value = 0;//��������ʾ��ʱ,��ʾ0
  }
  else{
    Value = (unsigned short)Data;
    if(((Flag & 0x0c) == 0x0c)) 
      Led.SegDisp[Pos] = LED_SIGN_0;//�޸�ʱ���0
    else if((Flag & 0x04)){//����ռһλʱ
      if(Data > 99) Pos++; //������,����λ������ʾ���λֵ
    }
  }
  
  //�����ֵ,�����λ��ʼ
  for(unsigned char i = 0; i < Pos; i++){
      Led.SegDisp[i] = LED_SIGN_NumLut[Value % 10];
      Value /= 10;
  }
}

//----------------------4λ����ʾErrX����-------------------------------
void LedNum4_ErrX(unsigned char ErrX) //������
{
  Led.SegDisp[0] = LED_SIGN_NumLut[ErrX];
  Led.SegDisp[1] = LED_SIGN_R;
  Led.SegDisp[2] = LED_SIGN_R;
  Led.SegDisp[3] = LED_SIGN_E;
  Led.SegFlash |= 0x0f;  //������ʾ
}

//---------------------------ѹ��ʮ������ֵ��ʾ-------------------------------
//��Led.SegDisp�е���ѹ����ʾ����000��ʾΪ0, 00.1��ʾΪ0.1
//05.00��ʾΪ5.00��С����λ���ڱ�ʾ���ȣ��ʲ�ѹ����ʾ,Ϊ��ֵʱ����ѹ����ʾ
void LedNum4_NumDecZip(void) 
{
  //�Ӹ�λSegDisp��ʼ��飬������Ϊ0����С������ȥ������ʾ
  for(unsigned char Pos = 3; Pos > 0; Pos--){//������ʾһλ
    if(Led.DotDisp & (1 << Pos)) break; //���ҵ�С���㼴������
    unsigned char Disp = Led.SegDisp[Pos];
    if(Disp == LED_SIGN_0) Led.SegDisp[Pos] = 0;//ȥ����ʾ
    else if(Disp != 0) break;//����ʾʱ(�縺/��ֵ/����)��������
  }
}




