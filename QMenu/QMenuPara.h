/***********************************************************************

		             菜单系统之4位数码管时-与用户的交互接口
//由参数与函数接口共同组成
//用户仅能通过此接口，与菜单管理器联系
***********************************************************************/
#ifndef _QMENU_PARA_H
#define	_QMENU_PARA_H

/***********************************************************************
		                           可选配置
***********************************************************************/

//是否支持十六位调整模式,全局里定义
//#define SUPPORT_QMENU_ADJ_HEX 

/***********************************************************************
		             与用户交互的参数接口
***********************************************************************/
struct _QMenuPara{
  signed short Adj;      //当前调整值
  signed short Min;      //调整值的最小值,含  
  signed short Max;      //调整值的最大值,含
  unsigned char Cfg;     //主配置字,每菜单项均需配置,见定义
  unsigned char Cfg2;    //附加配置字,不需要时为0,见定义
};

//主配置字定义为:
#define QMENU_CFG_RD        0x80    //菜单为只读状态,即不能过入第三层
#define QMENU_CFG_WR        0x40    //菜单为只写状态,即直接进入第三层调整
#define QMENU_CFG_REAL_RD   0x20    //菜单显示时需实时读数刷新
#define QMENU_CFG_REAL_WR   0x10    //菜单调整时需实时写刷新,在数值改变时即更新]
#define QMENU_CFG_REAL_REFRESH 0x08    //菜单在显示或调整时，均实时刷新(显负允许时不显正号)

#define QMENU_CFG_ADJ_MASK   0x07  //菜单项的调整方式:
#define QMENU_CFG_ADJ_BIT    0x00  //按位调节模式
#define QMENU_CFG_ADJ_ALL    0x01  //整体调节模式(最大最小值与当前值相同时不调整数值)
#define QMENU_CFG_ADJ_LOGIC  0x02  //逻辑数调节模式
#define QMENU_CFG_ADJ_CMD    0x03  //命令模式,仅允许按确认键
#define QMENU_CFG_ADJ_PASS   0x04  //密码输入模式，同按位调节，但
                                   //最后一位切换时会调用一次WrData(QMENU_CFG_WR)
                                   //写函数实现中比较密码对了可退出菜单并进行相应处理

#ifdef SUPPORT_QMENU_ADJ_HEX
  #define QMENU_CFG_ADJ_HEX    0x05  //十六进制, 调节模式,即每位范围0~F
#endif

//附加配置字定义为:
#define QMENU_CFG2_QUIT_SAVE  0x80    //响应退出时保存,可于不重要参数的保存,或恢复现场
#define QMENU_CFG2_QUIT       0x40    //调整完成后退出菜单标志
#define	QMENU_CFG2_NEGATIVE   0x20    //调整项为负数显示或调整,即可显示负值

#define	QMENU_CFG2_DOT_MASK   0x0f    //最低4位用于控制显示及调整时的小数点位显示
//#define	QMENU_CFG2_DOT0       0x01    //从右向左:第1个小数点显示
#define	QMENU_CFG2_DOT1       0x02    //从右向左:第2个数码管显示
#define	QMENU_CFG2_DOT2       0x04    //从右向左:第3个数码管显示
#define	QMENU_CFG2_DOT3       0x08    //从右向左:第4个数码管显示

/***********************************************************************
		             与用户交互的函数等功能接口
***********************************************************************/

//表述菜单层次的定义:
#define QMENU_LAYER_NULL   0 //不在菜单中
#define QMENU_LAYER_SEL    1 //在选择菜单状态
#define QMENU_LAYER_RD     2 //在查看(读)状态
#define QMENU_LAYER_WR     3 //在修改(写)状态

#include "QMenuPixel.h" //像素化支持结构

struct _QMenuFun{
  #ifdef SUPPORT_QMENU_PIXEL  //像素化支持时
    const struct QMenuPixel_Desc Desc;//描述结构
  #else //4位数码管时:
    unsigned char Note[4];//菜单选择里的提示项(含小数点)
  #endif
  //读取参数至QMenuPara,形参:
  //QMENU_CFG_REAL_RD: 为实时读取状态位置,实时读打开时调用
  //QMENU_CFG_REAL_WR: 为通讯层调用,写缓冲区数据
  //其它定义为菜单层次,表示进入该层菜单时调用,可根据情况进行QMenuPara初始化:
  void (*GetData)(struct _QMenuPara *pPara, unsigned char Type);
  //QMenuPara.Adj完成,保存数据,形参:
  //QMENU_CFG_REAL_WR: 为实时写状态位置,实时写打开且数值变化时调用
  //QMENU_CFG2_QUIT_SAVE: 为退出时保存,退出保存功能打开,在退出强制调用(不管值是否变化)
  //QMENU_LAYER_WR,表示退出写状态时调用,保存键且数值变化时正常写数据
  void (*SetData)(struct _QMenuPara *pPara, unsigned char Type);
};

/***********************************************************************
		                       相关函数接口
***********************************************************************/
//----------------------QMenuPara初始化函数--------------------------
void QMenuPara_Init(struct _QMenuPara *pPara);

//----------------------QMenuPara->SetData空函数----------------------
//菜单项为只读或无事件需要处理时可调用
void QMenuPara_SetNULL(struct _QMenuPara *pPara, unsigned char Type);

/***********************************************************************
		                       相关回调函数
***********************************************************************/

//---------------------DataSet实现中置退出菜单函数-----------------------
//仅可用于应用层在实现SetData函数的内部调用(内部不调用退出回调函数以防止嵌套)
void QMenuPara_cbQuitMenuInSet(struct _QMenuPara *pPara);

#endif


