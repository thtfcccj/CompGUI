/***********************************************************************

//		             菜单系统之4位数码管时的管理功能-显示部分回调接口
//此模块设计为多例化，单例化时宏实现为直接与Led.h对接
***********************************************************************/
#ifndef _QMENU_MNG_CB_DISP_H
#define	_QMENU_MNG_CB_DISP_H

/***********************************************************************
                    相关配置
***********************************************************************/

//单例化时支持,需在全局里定义
//  #define SUPPORT_QMENU_MNG_SINGLETON

/*********************************************************************
                    相关说明
为防止不同实例时，数码管段码不同(如硬件阵列模组连接IO不同了),这里用
MngId可进行区分
*********************************************************************/
//此模块显示部分独立于系统,需要4个"8.8.8.8"的段码显示,用户需实现列接口
//对8.8.8.8的外部驱动满足:
//1.每个8有独立的显示缓冲,由0~3表示(顺序可指定)
//2.每个8有独立的闪动功能,并用掩码来控制闪动,由位到掩码由应用决定
//3.每个小数点的显示与闪动可独立控制,掩码来控制闪动

#ifdef SUPPORT_QMENU_MNG_SINGLETON //单例化时
  #include "Led.h"
  #include "LedSign.h"
#endif

//---------------------------4位数送显---------------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  //靠右显示,为负时显示负号,指定小数点
  void QMenuMng_cbDispNum4(unsigned char MngId,
                           signed short Data,//要送显的数
                           //0~1bit最少显示位数-1(不含符号位)-1 2bit:是否带符号显示,
                           //3bit:带符号时,正时是否填充0    4~7bit小数点显示掩码
                           unsigned char Flag);
#else //单例化时
  #include "LedNum4.h" //LedNum4_Disp();
  #define QMenuMng_cbDispNum4(mngId, vol, flag)  LedNum4_Disp(vol, flag)
#endif

//---------------------由调整位置得到显示位置掩码----------------------
//QMenuMng归定最右侧位置为Pos0,通过此转换为对应数码管或小数点位置
//unsigned char QMenuMng_cbPos2Mask(unsigned char Pos);
#define QMenuMng_cbPos2Mask(mngId, pos) (1 << pos)

//---------------------由十进制数值得到段码显示----------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
unsigned char QMenuMng_cbVol2Seg(unsigned char MngId, unsigned char Vol);
#else //单例化时
  #define QMenuMng_cbVol2Seg(mngId,vol) LED_SIGN_NumLut[vol]
#endif

//---------------------得到负号的段码表示----------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  unsigned char QMenuMng_cbGetNegaSeg(unsigned char MngId);
#else //单例化时  
  #define QMenuMng_cbGetNegaSeg(mngId) (LED_SIGN__)
#endif
//------------------------更新某个显示缓冲区显示------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbSetSegDisp(unsigned char MngId, 
                             unsigned char Pos, unsigned char Seg);
#else //单例化时
  #define QMenuMng_cbSetSegDisp(pos, seg) do{Led.SegDisp[pos] = seg;}while(0)
#endif
  
//------------------------使掩码对应的数码管闪动------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
void QMenuMng_cbSetSegFlash(unsigned char MngId, 
                            unsigned char Mask);
#else //单例化时
  #define QMenuMng_cbSetSegFlash(mask)  do{Led.SegFlash |= mask;}while(0)
#endif

//----------------------------使所有数码管停闪--------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbClrSegFlash(unsigned char MngId);
#else //单例化时
  #define QMenuMng_cbClrSegFlash()  do{Led.SegFlash &= ~0x0f;}while(0)
#endif
  
//------------------------使掩码对应的小数点显示------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbSetDotDisp(unsigned char MngId, unsigned char Mask);
#else //单例化时
  #define QMenuMng_cbSetDotDisp(mask)  do{Led.DotDisp |= mask;}while(0)
#endif
  
//------------------------使所有小数点停显------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbClrDotDisp(unsigned char MngId);
#else //单例化时  
  #define QMenuMng_cbClrDotDisp()  do{Led.DotDisp &= ~0x0f;}while(0)
#endif
  
//------------------------使掩码对应的小数点闪动------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbSetDotFlash(unsigned char MngId, 
                              unsigned char Mask);
#else //单例化时
  #define QMenuMng_cbSetDotFlash(mask)  do{Led.DotFlash |= mask;}while(0)
#endif
  
//----------------------------使所有小数点停闪----------------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbClrDotFlash(unsigned char MngId);
#else //单例化时
  #define QMenuMng_cbClrDotFlash()  do{Led.DotFlash &= ~0x0f;}while(0)
#endif
  
//------------------------------强制更新以及更新闪动-----------------------
//更新强制标志,同时刷新显示屏
//形参为是否强制在消隐周期
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  void QMenuMng_cbForceFlash(unsigned char MngId, unsigned char IsForceHidden);
#else //单例化时
  #define QMenuMng_cbForceFlash(isforcehidden)\
    do{if(isforcehidden) Led.Flag |= (LED_FLASH_NOW | LED_FLASH_DIS);\
       else Led.Flag |= (LED_FLASH_NOW); Led_Task();}while(0)
#endif
         
//--------------定义在QMenuNote中定义的小数点位置掩码----------------------
#ifndef SUPPORT_QMENU_MNG_SINGLETON //多例化时
  unsigned char QMenuMng_cbGetDotMask(unsigned char MngId); 
#else //单例化时
  QMenuMng_cbGetDotMask(mngId)    LED_SIGN_DOT
#endif
    
#endif //_QMENU_MNG_CB_DISP_H
