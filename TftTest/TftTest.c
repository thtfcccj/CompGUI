/*******************************************************************************

                         ��ʾ�����Թ���ʵ��
�ݽ�֧�ֱ�׼ɫ����
*******************************************************************************/

#include "TftTest.h"
#include "Plot.h"
#include "Plot_cbHw.h"//TFT_DRV_H_PIXEl TFT_DRV_V_PIXEl


struct _TftTest TftTest;

/*******************************************************************************
                            ��غ���ʵ��
*******************************************************************************/

//--------------------------ɫ�ʲ���ʱ��׼��ɫֵ------------------------------
static const Color_t _Color[] = {
  RGB000000, 
  RGBFFFFFF,
  RGBFF00FF,   
  RGB00FFFF, 
  RGBFFFF00, 
  RGB0000FF, 
  RGB00FF00, 
  RGBFF0000, 
};

//--------------------------------ˢ�º���------------------------------
static void _Refresh(void)
{
  //���ݶ�ʱֵ��������������Ļ
  
  //ɫ�ʲ���:
  if(!(TftTest.Timer % (TFT_TEST_OV / sizeof(_Color)))){//��ɫ�仯�л�
    unsigned char Id = TftTest.Timer / (TFT_TEST_OV / sizeof(_Color));
    if(Id >= sizeof(_Color)) Plot_SetBrushColor(RGB000000);
    else Plot_SetBrushColor(_Color[Id]);
    Plot_FullRect(0, 0, TFT_DRV_H_PIXEl, TFT_DRV_V_PIXEl);
  }  
}

//---------------------------------������------------------------------
//1s����һ��
void TftTest_Task(void)
{
  if(!TftTest.Timer) return; 
  if(TftTest.Timer & 0xe0){ //��ͣģʽʱ��ʱ
    TftTest.Timer -= 0x20;
    return;
  }
  
  TftTest.Timer--;
  if(!TftTest.Timer){
    TftTest_Exit();
    return;   
  }
  _Refresh();
}

//----------------------------��������-------------------------------
//�����е��ý�һֱ��ǰ��ʾ7s,����ʼ����
void TftTest_Start(unsigned char Type) //��������,��˵��
{
  if(!TftTest.Timer || (Type != TftTest.Type)){//��������
    TftTest.Timer = TFT_TEST_OV;
    TftTest.Type = Type;
    _Refresh();    
  }
  else{//��ͣ��ʾ
    TftTest.Timer |= 0xe0;
  }
}

//-------------------------�˳�����------------------------------
void TftTest_Exit(void)
{
  TftTest.Timer = 0;
  TftTest_cbExitNotify();  //�˳�ͨ��
}


