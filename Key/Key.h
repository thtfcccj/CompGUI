/*****************************************************************************

//		               按键模块标准接口
//两种采集方式：
//SUPPORT_KEY_ID方式获得唯一键值，不能识别组合键
//SUPPORT_KEY_MASK方式获得每个键的按下状态，可识别组合键
****************************************************************************/
#ifndef __KEY_H
#define __KEY_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*******************************************************************************
					                  全局定义说明
********************************************************************************/

//是否支持硬按键功能,不支持是不含此模块(如红外等可以不支持)
//#define SUPPORT_KEY

//通报按键为独立，否则分别通报,定义此项后，键值为原始，并应确保键值<64个
//#define SUPPORT_KEY_NOTIFY_ALONE

/*******************************************************************************
					                  内部配置区
********************************************************************************/
#ifdef SUPPORT_QGUI   //使用QGUI时专用实现
  #include "Key_QGUI.h"
#else
  #include "KeyDef.h"        //用户的私有实现
#endif

#ifdef SUPPORT_KEY_ID 
  #define KeySize_t    unsigned char  //不需定义键值
  #ifndef KEY_INVALID_KEY
    #define KEY_INVALID_KEY    0xff         //定义无效键值
  #endif
#endif

#ifdef SUPPORT_KEY_MASK //掩码时，不定义默认只响应5个按键
  #ifndef KeySize_t
    #define KeySize_t    unsigned char
  #endif
  #ifndef KEY_INVALID_KEY
    #define KEY_INVALID_KEY    0         //定义无效键值
  #endif
#endif

/*******************************************************************************
					                     相关结构
*******************************************************************************/
//按键管理器
struct _Key{
  KeySize_t KeyId;         //按键值ID 
  unsigned char Index;		//按键计数器
};

extern struct _Key Key;

/*******************************************************************************
					                     相关函数
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void Key_Init(void);

//------------------------------------任务函数----------------------------------
//每8ms调用一次以获取键值
void Key_Task(void);

//----------------------------------获得最后一次键值函数------------------------
#define Key_GetKeyId()   (Key.KeyId)


/*******************************************************************************
					                    回调函数-底层
*******************************************************************************/
//--------------------------------初始始IO--------------------------------------
#include "IoCtrl.h" //直接实现
#define Key_cbCfgIO()       CfgKey()

//---------------------------------得到原始键值函数-----------------------------
//SUPPORT_KEY_ID时需实现
unsigned char Key_cbGetOrgKeyId(void);

//-----------------------------判断对应键是否按下-----------------------------
//SUPPORT_KEY_MASK时需实现
unsigned char Key_cbIsKeySel(unsigned char KeySel);

/*******************************************************************************
					                    回调函数-上层
*******************************************************************************/

//----------------------------由物理键值ID得到键值-----------------------------
//SUPPORT_KEY_ID时需实现
#ifdef SUPPORT_KEY_NOTIFY_ALONE //独立ID时，在用户层转换
  #define Key_cbKeyId2Key(keyId)  (keyId)
#else //在此层实现
  KeySize_t Key_cbKeyId2Key(KeySize_t KeyId);
#endif

//-------------------------------按键单个通报函数---------------------------------
//用回调的方式实现按键处理
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  //KeyState定义为： b7:6: 按键通报状态:0短按,1长按,2保持，3启动(0xc0)或释放(0xff)
  //                 b5:0: 当前键值ID
  void Key_cbNotify(KeySize_t KeyState);
#endif

//-------------------------------短按键通报函数---------------------------------
//用回调的方式实现按键处理
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbShortNotify(key)  Key_cbNotify(0x00 | (key))
#else
  void Key_cbShortNotify(KeySize_t Key);
#endif 

//-------------------------------长按键通报函数---------------------------------
//不支持时可实现为空
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbLongNotify(key)  Key_cbNotify(0x40 | (key))
#else
  void Key_cbLongNotify(KeySize_t Key);
#endif 

//-------------------------------保持按键通报函数------------------------------
//不支持时可实现为空
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbKeepNotify(key)  Key_cbNotify(0x80 | (key))
#else
  void Key_cbKeepNotify(KeySize_t Key);
#endif 

//------------------------------松开按键通报函数------------------------------
//可用于支持按键加速，不支持时可实现为空
#ifdef SUPPORT_KEY_NOTIFY_ALONE
  #define Key_cbRlsNotify()  Key_cbNotify(0xff)
#else
  void Key_cbRlsNotify(void);
#endif 
#endif





