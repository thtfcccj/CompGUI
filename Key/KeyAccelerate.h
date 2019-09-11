/***************************************************************************
                       保持键加速功能模块
此模块实现按住+，-键保持键不放，实现加速按键功能，实际上加速就是调用按键
处理函数的频率加大。就会越加越快，直到最快加的速度。
要实现菜单和主界面都有加速功能，该功能就要做在底层，就是在得到按键值之后，
按键处理之前加入该功能
//如果现在按键处理是0.5S执行一次，如果想加快20倍，也就是0.5s/20=25ms
//即最快25MS就执行一次按键。
//#define SUPPORT_KEY_ACCELERATE   //支持加速功能时需包含此模块
***************************************************************************/

#ifndef __KEY_ACCELERTAE_H
#define __KEY_ACCELERTAE_H

/***************************************************************************
                              相关配置
***************************************************************************/



//定义在KeyAccelerate_Task()调用周期内,按键的最大次数
#define KEY_ACCELERATE_MAX     23

//加速功能延时计数值，即判断是连续按后，还要再延时一个时间，才会进入加速
//单位为Task调用时间
#define KEY_ACCELERATE_DELAY   (KEY_ACCELERATE_MAX * 2)

/***************************************************************************
                              相关结构
***************************************************************************/

struct _KeyAccelerate{
  unsigned char Gene;   //加速因子，按键加速频率和该值成正比
  unsigned char Index; //计数器
};

extern struct _KeyAccelerate  KeyAccelerate;

/***************************************************************************
                              相关函数
***************************************************************************/

//--------------------------------初始化函数-----------------------------
void KeyAccelerate_Init(void);

//------------------------------按键加速启动函数-------------------------
//需要按键加速时调用此函数
void KeyAccelerate_Start(void);

//------------------------------按键加速停止函数-------------------------
//按键加速需停止时调用函数
//void KeyAccelerate_Stop(void);
#define KeyAccelerate_Stop() do{KeyAccelerate.Gene = 0;}while(0)

//-------------------------------任务函数-----------------------------
//放入16或32ms进程中
void KeyAccelerate_Task(void);

#endif //#define __KEY_H

