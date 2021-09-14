/***************************************************************************

                       数码管显示驱动程序-在使用1652时的实现

***************************************************************************/
#include "LedDrv1652.h"
#include <string.h>

struct _LedDrv1652  LedDrv1652;

static const unsigned char _ToHwLut[LED_DRV_LED_COUNT] = LED1652_TO_HW_LUT;

//独立快速更新时，定义上次值
#ifdef LED1652_FAST_BIT 
  static unsigned char _PrvFastVol;
#endif

/***************************************************************************
                    通讯函数-通用USART实现
***************************************************************************/
#ifdef SUPPORT_LED_DRV_1652_USART

//--------------------Usart结束中断处理函数定义--------------------------------
//形参为返struct _UsartDev指针
static signed char _UsartDevInt(void *pv)
{
  LedDrv1652.Timer = 3; //通讯完成,但需间隔3ms时间
  return 0;
}

//----------------------------发送数据---------------------------------------
static void LedDrv1652_cbSendStart(unsigned char SendSize)
{
  UsartDev_SendStart(LedDrv1652_cbpGetUsartDev(), //所在设备
                     LedDrv1652.CommBuf,                    //发送缓冲区
                     0x8000 | SendSize,//发送缓冲区大小,发送完中断
                     _UsartDevInt);               //发送回调函数
}
#endif

/***************************************************************************
                              相关函数实现
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void LedDrv1652_Init(void)
{
  memset(&LedDrv1652, 0, sizeof(struct _LedDrv1652));
  //配置1652
  LedDrv1652.CommBuf[0] = 0x18;//显示控制命令
  LedDrv1652.CommBuf[1] = LED1652_CFG;
  LedDrv1652_cbHwInit();
  LedDrv1652_cbSendStart(2);
  LedDrv1652.UpdateMask = 0xff;//首次更新所有,同时置通讯中标志
  LedDrv1652.Timer = 3; //通讯中，需间隔3ms时间
}

//-------------------------------任务函数-----------------------------
//放入进程或中断中扫描,扫描率决定显示情况
void LedDrv1652_Task(void)
{
  if(LedDrv1652.Timer){//通讯等待过程中 
    LedDrv1652.Timer--;
    return; 
  }
  if(LedDrv1652.UpdateMask == 0){//无需更新时
    return; 
  }
  
  //测试模式更新
  if(LedDrv1652_cbIsTest()){
    memset(&LedDrv1652.CommBuf[1], 0xff, LED_DRV_LED_COUNT);
    #ifdef LED1652_RSV_MASK//有保留位时保留原值
      LedDrv1652.CommBuf[LED1652_RSV_POS + 1] &= ~LED1652_RSV_MASK;  
      LedDrv1652.CommBuf[LED1652_RSV_POS + 1] |= 
        LedDrv1652.Buf[_ToHwLut[LED1652_RSV_POS]] & LED1652_RSV_MASK;       
    #endif
  }
  //独立快速更新: 如指示灯等快速更新
  #ifdef LED1652_FAST_BIT 
  else if(LedDrv1652.UpdateMask == (1 << LED1652_FAST_BIT)){
    LedDrv1652.UpdateMask =  (1 << LED1652_FAST_BIT);//  清走缓冲
    unsigned char Cur = LedDrv1652.Buf[_ToHwLut[LED1652_FAST_BIT]];
    if(_PrvFastVol == Cur) return;//并没有改变
    _PrvFastVol = Cur;
    LedDrv1652.CommBuf[LED1652_FAST_BIT + 1] = Cur;
    LedDrv1652.CommBuf[0] = 0x08 | LED1652_FAST_BIT;//显示地址命令
    LedDrv1652.Timer = 255; //通讯中
    LedDrv1652_cbSendStart(2);
    return;
  }
  #endif
  //其它更新：暂使用直接全部更新策略以节省代码
  else{   
    LedDrv1652.CommBuf[0] = 0x08;//显示地址命令
    LedDrv1652.CommBuf[1] = LedDrv1652.Buf[_ToHwLut[0]];
    LedDrv1652.CommBuf[2] = LedDrv1652.Buf[_ToHwLut[1]];
    LedDrv1652.CommBuf[3] = LedDrv1652.Buf[_ToHwLut[2]];
    LedDrv1652.CommBuf[4] = LedDrv1652.Buf[_ToHwLut[3]];
    LedDrv1652.CommBuf[5] = LedDrv1652.Buf[_ToHwLut[4]];
    #if LED_DRV_LED_COUNT >= 6
      LedDrv1652.CommBuf[6] = LedDrv1652[_ToHwLut[5]];
    #endif
      
  }
  LedDrv1652.CommBuf[0] = 0x08;//显示地址命令  
  LedDrv1652.UpdateMask = 0;//  清走缓冲
  LedDrv1652.Timer = 255; //通讯中
  LedDrv1652_cbSendStart(LED_DRV_LED_COUNT + 1);
}


