/*****************************************************************************

		              扫描时置指示灯与获得按键-在8位输出上的实现
//此模块驱动的硬件结构为：
//最大4X4阵列的指示灯与按键
//按键与指示灯行选扫描复用8位输出(如EO8驱动595)输出
//读按键列用IO实现
******************************************************************************/

#ifndef _KEY_LIGHT_H
#define _KEY_LIGHT_H

/*****************************************************************************
                            相关配置
******************************************************************************/

//指示灯与按键复用的扫描总数
#ifndef KEY_LIGHT_SCAN_COUNT     
  #define KEY_LIGHT_SCAN_COUNT     3   
#endif

//指示灯扫描列数 
#ifndef KEY_LIGHT_L_SCAN_COUNT     
  #define KEY_LIGHT_L_SCAN_COUNT   3   
#endif

//按键读列数 
#ifndef KEY_LIGHT_K_SCAN_COUNT     
  #define KEY_LIGHT_K_SCAN_COUNT    3   
#endif

//内部定义:
#ifndef KEY_LIGHT_K_BIT_COUNT //按键位总数，即读键值个数
  #define KEY_LIGHT_K_BIT_COUNT  KEY_LIGHT_SCAN_COUNT //默认为方阵
#endif

/*****************************************************************************
                            相关结构
******************************************************************************/

struct _KeyLight{
  unsigned char Key[KEY_LIGHT_K_SCAN_COUNT];    //当前扫描到的键值，1为有 
  unsigned char Light[KEY_LIGHT_L_SCAN_COUNT];  //指示灯状态，1为亮     
    
  unsigned char ScanPos;  //指扫描位置
};

extern struct _KeyLight KeyLight; //直接单例化


/*****************************************************************************
                            相关函数
******************************************************************************/

//-------------------------初始化函数---------------------------------------
void KeyLight_Init(void);

//-------------------------中断任务函数------------------------------------
//放在1~10ms中断里实现
void KeyLight_IntTask(void);

/*****************************************************************************
                            回调函数接口
******************************************************************************/
#include "IoCtrl.h"  //部分直接IO实现:
//IO初始化                        
//#define KeyLight_cbInitIo()   do{}while(0)                       
//定义(如EO8驱动595)上，指示灯与按键的扫描掩码    
//#define KeyLight_cbScanLut()   {0xnn, 0xnn,0xnn} 
//8位输出上有其它固定位时，该位的值，无时为0
//#define KeyLight_cbGetFixBit()    0
//定义8位输出(如EO8驱动595)上，指示灯位置掩码    
//#define KeyLight_cbLightMask()   0xnn
//定义为指示灯亮  
//#define KeyLight_cbLightOn()   0xnn

//更新输出
//#define KeyLight_cbUpdateOut(out)   do{}while(0)     

//----------------------------得到当前键值----------------------------------
//对应位为1表示按下了
unsigned char KeyLight_cbGetKeyVol(void);

//----------------------------是否测试状态----------------------------------
#include "Test.h"  //直接实现
#define KeyLight_cbIsTest()  Test_IsDoing()


#endif //#ifdef _KEY_LIGHT_H

