/*******************************************************************************

                         显示屏测试功能实现
暂仅支持标准色测试
*******************************************************************************/

#include "TftTest.h"
#include "Plot.h"
#include "Plot_cbHw.h"//TFT_DRV_H_PIXEl TFT_DRV_V_PIXEl


struct _TftTest TftTest;

/*******************************************************************************
                            相关函数实现
*******************************************************************************/

//--------------------------色彩测试时标准颜色值------------------------------
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

//--------------------------------刷新函数------------------------------
static void _Refresh(void)
{
  //根据定时值与测试类型填充屏幕
  
  //色彩测试:
  if(!(TftTest.Timer % (TFT_TEST_OV / sizeof(_Color)))){//颜色变化切换
    unsigned char Id = TftTest.Timer / (TFT_TEST_OV / sizeof(_Color));
    if(Id >= sizeof(_Color)) Plot_SetBrushColor(RGB000000);
    else Plot_SetBrushColor(_Color[Id]);
    Plot_FullRect(0, 0, TFT_DRV_H_PIXEl, TFT_DRV_V_PIXEl);
  }  
}

//---------------------------------任务函数------------------------------
//1s更新一次
void TftTest_Task(void)
{
  if(!TftTest.Timer) return; 
  if(TftTest.Timer & 0xe0){ //暂停模式时计时
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

//----------------------------启动测试-------------------------------
//测试中调用将一直当前显示7s,否则开始测试
void TftTest_Start(unsigned char Type) //测试类型,见说明
{
  if(!TftTest.Timer || (Type != TftTest.Type)){//启动测试
    TftTest.Timer = TFT_TEST_OV;
    TftTest.Type = Type;
    _Refresh();    
  }
  else{//暂停显示
    TftTest.Timer |= 0xe0;
  }
}

//-------------------------退出测试------------------------------
void TftTest_Exit(void)
{
  TftTest.Timer = 0;
  TftTest_cbExitNotify();  //退出通报
}


