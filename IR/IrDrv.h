/***************************************************************************
                       红外接收通用驱动程序
此模块负责处理红外接收按键,并支持按键对码
此模块独立于硬件及遥控板
***************************************************************************/

#ifndef __IR_DRV_H
#define __IR_DRV_H

/***************************************************************************
                              相关说明
***************************************************************************/
//根据资料分析及实测:
//1.每个码值由多个位组成,最多位为NEC协义,为16个正码+16个补码,故允许最多为32bit
//2.所有码值均由一个脉冲起始码组成,故程序中将此码值作为起始,不计入信息中
//3.每个位的无信号周期不超过5mS(现有资料NEC的,最大2.25ms),故置定时器时,
//  应作为一个码值的结束标志
//4.位的定义基本有两种方式 
//  (1):每个周期以空闲宽度区为1或0,有信息时间固定,如NEC,国内常用彩电等
//  (2):每个周期宽度固定,以信号宽度作为高电电平依据
//注:
//1.对码不由此模块实现,此模块仅给出新键信息
//2.新按键无长按与保持按检测
/***************************************************************************
                              相关配置
***************************************************************************/

//支持对码时,在全局里定义，支持时，键值保存EEPROM
//#define SUPPORT_IR_KEY_MATCH 

//指定红外芯片(HS0038B等)空闲时有信号载波(在硬件实现中会用到) 
#ifndef IR_DRV_VALID_LEVEL
  #define IR_DRV_VALID_LEVEL    0  //0低电平为有效载波，1高电平，其它无效  
#endif

//定义按键个数,需<=63
#ifndef IR_DRV_KEY_COUNT
  #define IR_DRV_KEY_COUNT          4
#endif

//定义码值的位数大小
#ifndef IR_DRV_CODE_SIZE
  #define IR_DRV_CODE_SIZE          32
#endif

//定义长按键检测时的连续时间, 在此时间内又收到码值了,认为用户没有放开,
//Task调用周期为单位
#ifndef IR_DRV_KEEP_COUNT
  #define IR_DRV_KEEP_COUNT          13    //200ms为周期
#endif

//定义长按键检测时间,Task 为单位
#ifndef IR_DRV_KEY_LONG_COUNT
  #define IR_DRV_KEY_LONG_COUNT      62  
#endif

//定义保持按键间的发送间隔时间,Task调用周期为单位
#ifndef IR_DRV_KEY_HOLD_SPACE
  #define IR_DRV_KEY_HOLD_SPACE      16
#endif

/***************************************************************************
                              相关结构
***************************************************************************/

//红外接收的一个码值信息,即描述一个按键的波形信息
struct _IrDrvCode{
  //码值总体描述: 因不强调信号的频率,故这里不将脉冲宽度作为信息存储
  //unsigned short IdieWidth;    //无信号时的标准宽度
  //unsigned short IrWidth;      //有信号时的标准宽度
  unsigned char  Count;         //码值个数
  
  //位描述:->因不强制脉宽时间,
  //无信号时的脉冲宽度掩码,0:标准宽,1:加宽
  unsigned char IdieWidthMask[IR_DRV_CODE_SIZE / 8]; 
  //有信号时的脉冲宽度掩码,0:标准宽,1:加宽
  unsigned char IrWidthMask[IR_DRV_CODE_SIZE / 8];  
};

//红外按收信息位定义
struct _IrDrvInfo{
  struct _IrDrvCode Code[IR_DRV_KEY_COUNT]; //每个按键对应的红外码值
};

//运行状态机:
enum _IrDrvState{
 eIrDrvState_Idie      = 0,    //没有检测到信号,空闲状态
 eIrDrvState_Rdy       = 1,    //检测到起动信号(即长的空闲期),准备收数据状态
 eIrDrvState_SampIrW   = 2,    //Ir宽度采集信号过程中
 eIrDrvState_SampIdieW = 3,    //无Ir宽度采集信号过程中
 eIrDrvState_Doing     = 4,    //采集信号过程中
 eIrDrvState_Final     = 5,    //采集信号过程完成,准备让用户收数据
};

struct _IrDrv{
  //用于先缓存完一个码值后,再在普通进程中处理是那个码值
  struct _IrDrvCode Buf; 
  unsigned short IdieWidth;    //无信号时的标准宽度
  unsigned short IrWidth;      //有信号时的标准宽度
  
  //用于缓冲码值,若Buf值与Info值不同,则将Buf缓冲
  struct _IrDrvCode CodeIdent; 
  //键值信息
  unsigned char PrvKey;        //上次按键值
  //unsigned char KeyIndex;      //相同按键时的重复次数
  unsigned char KeyKeepIndex;  //用户连接按键计数器
  unsigned char KeyHoldIndex;  //保持按键计数器
  //相关标志,见定义
  enum _IrDrvState eState;    //运行状态机      
};
//因每个系统中存在一个红线接收,故直接实例化
extern struct _IrDrv  IrDrv; 

/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数---------------------------------
void IrDrv_Init(void);

//-------------------------------普通任务函数-----------------------------
//放入16ms进程中扫描
void IrDrv_Task(void);

//-------------------------------快速任务函数-----------------------------
//放入快进程中扫描
void IrDrv_FastTask(void);

//---------------------------中断处理函数-----------------------------
//放入捕获中断中,在有信号周期结束(即处在空闲周期)时调用此函数
//注:规定当IdieWidth>=65535时表示当前码值检测结束
void IrDrv_IRQ(unsigned short IdieWidth,  //无信号周期的脉冲宽度,>=0 
               unsigned short IrWidth);   //有信号周期的脉冲宽度,>=0

/***************************************************************************
                         回调函数
***************************************************************************/

//---------------------------按键通报函数-----------------------------
//KeyState定义为： b7:6: 按键通报状态:0短按,1长按,2保持，3释放(即为空)
//                 b5:0: 当前键值ID
void IrDrv_cbKeyNotify(unsigned char KeyState);

//---------------------------硬件初始化回调-------------------------------
void IrDrv_cbHwInit(void);

//------------------------------信息位定义-------------------------------
extern const struct _IrDrvInfo cbIrDrvInfoDefault;//需实现默认对码值

#ifdef SUPPORT_IR_KEY_MATCH    //支持对码时
  extern struct _IrDrvInfo cbIrDrvInfo;//实现为变量(需实现新键值处理及读写EEPROM)
#else
  #define cbIrDrvInfo  cbIrDrvInfoDefault //实现为默认Info值
#endif

//-------------------新码值通报函数------------------------------
//识别到对码不对的键值时,通报新键值(IrDrv.CodeIdent为当前键值)
//可用于用户进入对码状态或保存键值
#ifdef SUPPORT_IR_KEY_MATCH    //支持对码时有效
  void IrDrv_cbNewKeyNotify(void);
#else
  #define IrDrv_cbNewKeyNotify() do{}while(0)
#endif

//--------------------新键值切换通报函数------------------------------
//识别到多次相同键值无效按键后,检测到不一样时通报
#ifdef SUPPORT_IR_KEY_MATCH    //支持对码时有效
  void IrDrv_cbNewKeySwitchNotify(void);
#else
  #define IrDrv_cbNewKeySwitchNotify() do{}while(0)
#endif

#endif //#define __IR_DRV_H

