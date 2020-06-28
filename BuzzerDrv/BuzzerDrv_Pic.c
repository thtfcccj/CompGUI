/***************************************************************************

          频率可控蜂鸣器模块底层驱动接口-在PIC单片机CCP1+CCP2中,用PWM时的实现
//使用双向驱动以提高声压等级:即V-G,G-V交换
****************************************************************************/

#include <pic.h>
#include <string.h>
#include "PicBit.h"
#include "IoCtrl.h"

#include "BuzzerDrv.h"
#include "string_P.h"

/***************************************************************************
                            内部配置
****************************************************************************/

//定义MCU的工作频率
#ifndef _MCU_HZ
  #define _MCU_HZ         8000000   //定义MCU的工作频率
#endif

//定义使用的定时器,PWM模式可使用(用8位模式)2/4/6定时器中的一个,不定义时使用定时器2
//#define BUZZER_USE_TIMER_4 //使用定时器4时定义
//#define BUZZER_USE_TIMER_6 //使用定时器6时定义

/***************************************************************************
                           内部配置后的宏转义
****************************************************************************/

//------------------------关于定时器2寄存器的转义------------------------
#ifndef BUZZER_USE_TIMER_4
#ifndef BUZZER_USE_TIMER_6
  #define TbCON        T2CON       //控制
  #define PRb          PR2         //分频
  //选择定时器2为CCP1，CCP2的PWM定时器(0b11无效)
  #define _SelTimer()  do{CCPTMRS0 = 0xf0;}while(0)
#endif //#ifndef BUZZER_USE_TIMER_4
#endif //#ifndef BUZZER_USE_TIMER_6

//------------------------关于定时器4寄存器的转义------------------------
#ifdef BUZZER_USE_TIMER_4
  #define TbCON        T4CON       //控制
  #define PRb          PR4         //分频
  //选择定时器4为CCP1，CCP2的PWM定时器(0b11无效)
  #define _SelTimer()  do{CCPTMRS0 = 0xf6;}while(0)
#endif //#ifndef BUZZER_USE_TIMER_4

//------------------------关于定时器6寄存器的转义------------------------
#ifdef BUZZER_USE_TIMER_6
  #define TbCON        T6CON       //控制
  #define PRb          PR6         //分频
  //选择定时器4为CCP1，CCP2的PWM定时器(0b11无效)
  #define _SelTimer()  do{CCPTMRS0 = 0xfa;}while(0)
#endif //#ifndef BUZZER_USE_TIMER_6

/***************************************************************************
                          相关函数实现
****************************************************************************/

//------------------------------初始化函数---------------------------------
void BuzzerDrv_Init(void)
{
   //输出频率 = _MCU_HZ / ((预分频值(4bit) * (分频器值(8bit))
   //配置说明:
   //  1.P1A，P2A分别输出高低电平以实现双向驱动
   //  2.CCP1，CCP2的值为定时值的一并以实现50%占空比
   //  3.通过配置定时器溢出周期为需要的频率，以定现蜂鸣器的频率控制

   //使用定时8bit比较 * 4bit后分频实现,默认配置为4KHz周期
   TbCON = 0; //先关闭

   CCP1CON  = 0;//PICB_CCP1M1 | PICB_CCP1M2 | PICB_CCP1M3;//PWM模式P1A出，低电平有效。占空比高两位
   CCP2CON  = 0;//PICB_CCP2M2 | PICB_CCP2M3;//PWM模式P2A出，高电平有效。占空比高两位
   
   _SelTimer(); //选择定时器
   CfgBuzzerIo();//PWM对应引脚设置为输出
}

//各分频器对应的最低蜂鸣器频率值
static const __flash unsigned short _PsForMinHz[] = {
  (unsigned long )_MCU_HZ >> (8 + 0),   //1分频
  (unsigned long )_MCU_HZ >> (8 + 2),   //4分频
  (unsigned long )_MCU_HZ >> (8 + 4),   //16分频
  (unsigned long )_MCU_HZ >> (8 + 6),   //64分频
};

//分频器下标对应的分频数
static const __flash unsigned long _Ps2Div[] = {
  1,     //1分频
  4,     //4分频
  16,    //16分频
  64,    //64分频
};

//-----------------------------启动函数-------------------------------------
//形参为频率,0表示暂时不响
//注：频率范围与分辨率与硬件相关,注意分辨率的不连续性对声音的影响
void BuzzerDrv_Start(unsigned short Hz)
{
  if(Hz == 0){//暂时不响
    BuzzerDrv_Stop();
    return;
  }
  //输出频率 = _MCU_HZ / ((预分频值(4bit) * (分频器值(8bit))

  //计算预分频值
  unsigned char Ps;
  for(Ps = 0; Ps < 4; Ps++){
    if(Hz > _PsForMinHz[Ps]) break;
  }

  //计算并填充定时器值,并开启定时器
  unsigned char Pr = (unsigned long )_MCU_HZ / (Hz * _Ps2Div[Ps]);
  PRb = Pr;
  //分频值设为50%
  Pr >>= 1;
  CCPR1L = Pr;
  CCPR2L = Pr;
  CCP1CON  = PICB_CCP1M1 | PICB_CCP1M2 | PICB_CCP1M3;//PWM模式P1A出，低电平有效。占空比高两位
  CCP2CON  = PICB_CCP2M2 | PICB_CCP2M3;//PWM模式P2A出，高电平有效。占空比高两位
  TbCON = PICB_TMR2ON | Ps; //使用最高Fosc/4(8MHZ)以实现频率的细微调整
  
  //注：4KHZ附近的实际频率情况为：
  //	116	4310.344828
  //	117	4273.504274
  //	118	4237.288136
  //	119	4201.680672
  //	120	4166.666667
  //	121	4132.231405
  //	122	4098.360656
  //	123	4065.04065
  //	124	4032.258065
  //	125	4000
  //	126	3968.253968
  //	127	3937.007874
  //	128	3906.25
  //	129	3875.968992
  //	130	3846.153846
  //	131	3816.793893
  //	132	3787.878788
}

//-----------------------------停止函数-------------------------------------
void BuzzerDrv_Stop(void)
{
  TbCON = 0;//关闭定时器
  CCP1CON  = 0;
  CCP2CON  = 0;
  ClrBuzzerIo();//驱动为低电平无电流输出
}