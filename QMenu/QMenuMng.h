/***********************************************************************

//		             菜单系统之4位数码管时的管理功能实现
//此模块设计为多例化，单例化时宏实现为直接与Led.h对接
//此多例化仅为兼容单例，若MCU堆栈等资源受限，可使用此模块对应的单例模块
***********************************************************************/
#ifndef _QMENU_MNG_H
#define	_QMENU_MNG_H

/***********************************************************************
                            说明
***********************************************************************/
//菜单共分三层:
//第一层为菜单选择,显示一组操作字符提示,此时可选择进入或退出,
//第二层为参数显示,可一次性读入显示值,也可实时更新显示值
//第三层为参数调节:当第二层非只读时,按确认键进入
//第二,第三层根据情况可有可无
//菜单系统仅响应进入,退出,上,下键的短按,长按,保持时响应

/***********************************************************************
                    相关配置
***********************************************************************/

//支持像素化时定义，否则默认支持4位数码管, 需在全局里定义
//#define SUPPORT_QMENU_PIXEL   //支持像素化时

//支持其它显示映射方式时(含像素化)在QMenuMng_cbDispOther.h里，由用户自定义
//#define SUPPORT_QMENU_MAP_DISP_OTHER 


/***********************************************************************
                    相关结构-菜单管理器
//与用户交互部见在QMenu.h里定义
***********************************************************************/

#include "QMenuPara.h"
//用于菜单系统运行管理,用户禁止直接访问
struct _QMenuMng{
  //当前所处层,见定义0:不在菜单,否则在1,2,3菜单中
  unsigned char Layer;
  //当前处于第几个菜单位置
  unsigned char Index;
  //缓冲的最后键值,用于减少系统嵌套调用
  unsigned char Key;
  //相关标志,见定义
  unsigned char Flag;
  
  //参数调整时,参数的内部变量:
  unsigned char WrIndex;//调整数值时的位置
  unsigned char WrMaxPos;//调整数值时的最大范围,含
  signed short OrgAdj;  //进入调用时的调整项原始值
  
   //用户挂接的菜单查找表,首位为查找表大小. 可使用此机制实现权限访问
  const unsigned char *pLUT;  
  //用户交互接口阵列
  const struct _QMenuFun *const *pFunAry;
  //正在调整的参数
  struct _QMenuPara Para;  
};

//其中,菜单层次见QMenu中的层次定义:

//相关标志定义为:
#define QMENU_MNG_ID_MASK   0x3F     //多组显示时,管理器ID号

#define QMENU_MNG_REFRESH_NOW 0x40   //立即刷新标志

//闪动位强制消隐显示标志,否则按键将使闪动位强制显示,此标志在调用DISP后自动取消
#define QMENU_MNG_FLASH_HIDDEN   0x80  

/***********************************************************************
                        相关函数
***********************************************************************/

//--------------------------任务函数--------------------------
//将此函数放入系统250mS进程中,用于更新实时值的显示
void QMenuMng_Task(struct _QMenuMng *pMng);

//----------------------更新菜单显示函数--------------------------
//调用此函数主动更新界面显示(点动，不可周期调用)
void QMenuMng_UpdateDisp(struct _QMenuMng *pMng);

//------------------------重新装载用户数据------------------------
void QMenuMng_ReloadPara(struct _QMenuMng *pMng);

//----------------------进入菜单初始化函数--------------------------
//由主界面准备进入菜单时调用挂接的菜单查找表并完成内部初始化
//pLUT: pFunAry的查找表项，首位为个数，后跟对应项ID
//还需调用QMenuMng_Key(QMENU_MNG_KEY_ENTER)才能正式进入菜单
void QMenuMng_EnterInit(struct _QMenuMng *pMng,
                        const struct _QMenuFun *const *pFunAry,//不能为NULL
                        const unsigned char *pLUT);//不能为NULL

//----------------------------退出菜单函数--------------------------
//在菜单状态,外部强制退出菜单时调用此函数
void QMenuMng_Exit(struct _QMenuMng *pMng);
                  
//----------------------------初始化函数----------------------------
//开机时调用
void QMenuMng_Init(struct _QMenuMng *pMng,
                    unsigned char Id); //分配的ID号

//----------------------接键处理函数--------------------------
//形参Key按位定义为:
#define QMENU_MNG_KEY_MASK   0x0f  //键值定义掩码:
#define QMENU_MNG_KEY_MODE   0x70  //按键方式掩码
#define QMENU_MNG_KEY_UP     0x01  //上键
#define QMENU_MNG_KEY_DOWN   0x02  //下键
#define QMENU_MNG_KEY_ENTER  0x04  //进入键
#define QMENU_MNG_KEY_EXIT   0x08  //退出键

#define QMENU_MNG_KEY_TYPE_MASK 0x70//按键类型：
#define QMENU_MNG_KEY_SHORT  0x10  //短按键
#define QMENU_MNG_KEY_LONG   0x20  //长按键
#define QMENU_MNG_KEY_KEEP   0x40  //保持按键(按住不放)

#define QMENU_MNG_KEY_USER   0x80  //用户保留功能(内部不使用)

//void QMenuMng_Key(unsigned char Key);
#define QMenuMng_Key(pmng, key) do{(pmng)->Key = key;}while(0)

//--------------------------快速任务函数--------------------------
//将此函数放入系统快速进程中,用于处理键值
void QMenuMng_FastTask(struct _QMenuMng *pMng);

//------------------------获得正在调整项菜单指针阵列------------------------
//可用于判断当前位置以显示对应图标提示等,菜单内才可调用
#define QMenuMng_pGetCurFunAry(mng) ((mng)->pFunAry)

//------------------------获得正在调整项菜单指针------------------------
//可用于判断当前位置以显示对应图标提示等,菜单内才可调用
#define QMenuMng_pGetCurFun(mng) ((mng)->pFunAry[QMenuMng_GetId(mng)])

//-------------------------得到当前菜单ID号--------------------
unsigned char QMenuMng_GetId(const struct _QMenuMng *pMng);

//-------------------------得到调整前值-----------------------
//signed short QMenuMng_GetOrgAdj(struct _QMenuMng *pMng);
#define QMenuMng_GetOrgAdj(mng) ((mng)->OrgAdj)

//-------------------------得到当前调整位------------------------
//unsigned char QMenuMng_GetWrIndex(struct _QMenuMng *pMng);
#define QMenuMng_GetWrIndex(mng) ((mng)->WrIndex)

//------------------判断是否在实时显示状态------------------------
signed char QMenuMng_IsRealDisp(struct _QMenuMng *pMng);

//---------------------------得到在调整值------------------------
//正在调整值在菜单未切换时不会改变
//unsigned short QMenuMng_GetAdj(struct _QMenuMng *pMng);
#define QMenuMng_GetAdj(mng) ((mng)->Para.Adj)

//---------------------------得到菜单层次------------------------
//正在调整值在菜单未切换时不会改变
//unsigned char QMenuMng_GetLayer(struct _QMenuMng *pMng);
#define QMenuMng_GetLayer(mng) ((mng)->Layer)

/***********************************************************************
                              回调函数
***********************************************************************/
#if defined(SUPPORT_QMENU_MAP_DISP_OTHER) || defined(SUPPORT_QMENU_PIXEL) 
  #include "QMenuMng_cbDispOther.h"
#else
  #include "QMenuMng_cbDisp.h"
#endif

//---------------------------退出菜单系统通报-----------------------
void QMenuMng_cbExitNotify(unsigned char MngId);


#endif //_QMENU_MNG_H
