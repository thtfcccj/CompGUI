/***************************************************************************
                       保持键加速功能模块
此模块实现按住某个键保持不放，实现加速按键功能，实际上加速就是调用按键
处理函数的频率加大。就会越加越快，直到最快加的速度。
要实现菜单和主界面都有加速功能，该功能就要做在底层，就是在得到按键值之后，
按键处理之前加入该功能
//如果现在按键处理是0.5S执行一次，如果想加快20倍，也就是0.5s/20=25ms
//即最快25ms就执行一次按键。
***************************************************************************/
#ifndef __KEY_ACCELERTAE_H
#define __KEY_ACCELERTAE_H

/***************************************************************************
                              相关配置
***************************************************************************/

//#define SUPPORT_KEY_ACCELERATE   //若为可选支持加速功能时建议需包含此模块时的宏

//定义在KeyAccelerate_Task()调用周期内,按键的最大次数
#ifndef KEY_ACCELERATE_MAX
  #define KEY_ACCELERATE_MAX     23
#endif

//加速功能延时计数值，即判断是连续按后，还要再延时一个时间，才会进入加速
//单位为Task调用时间
#ifndef KEY_ACCELERATE_DELAY
  #define KEY_ACCELERATE_DELAY   (KEY_ACCELERATE_MAX * 2)
#endif

//定义用户键值单位
//单位为Task调用时间
#ifndef KeyAccelerate_KeyType_t
  #define KeyAccelerate_KeyType_t unsigned char
#endif

/***************************************************************************
                              相关结构
***************************************************************************/

struct _KeyAccelerate{
  unsigned char Gene;   //加速因子，按键加速频率和该值成正比
  unsigned char Index; //计数器
  KeyAccelerate_KeyType_t Key;//当前正在被加速的按键
};

extern struct _KeyAccelerate  KeyAccelerate;

/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数----------------------------------
void KeyAccelerate_Init(void);

//-------------------------按键加速启动函数--------------------------------
//检查到为需要加速的保持按键时，调用此函数实现加速
void KeyAccelerate_Start(KeyAccelerate_KeyType_t Key);//键值

//------------------------按键加速停止函数-------------------------------
//用户按键取消时调用此函数停止
//void KeyAccelerate_Stop(void);
#define KeyAccelerate_Stop() do{KeyAccelerate.Gene = 0;}while(0)

//-------------------------------任务函数-----------------------------
//放入16或32ms进程中
void KeyAccelerate_Task(void);

/***************************************************************************
                            回调函数
***************************************************************************/

//-------------------------加速键处理函数-------------------------------
void KeyAccelerate_cbKey(KeyAccelerate_KeyType_t Key);

#endif //#define __KEY_H

