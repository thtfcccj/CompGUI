/*****************************************************************************

//		        按键模块私有部分-在使用QGUI时的实现
//QTGUI固定支持4个按键,键值需转换为QGUI支持的格式
****************************************************************************/
#ifndef __KEY_Q_GUI_H
#define __KEY_Q_GUI_H

/*******************************************************************************
					                            配置区
********************************************************************************/
//键值ID方式
#define     SUPPORT_KEY_MASK                                 

//定义有效按键检查次数，防止按键干扰或无按键到按键时防止抖动
#define			KEY_VALID_COUNT			4
//长按时间检测次数,此值影响长按时间判断		
#define			KEY_TIMER_LONG			100
//保持键时间间隔默认次数,此值影响调整项增减快慢
#define			KEY_TIMER_HODE			50

//定义原始键值的最大范围，原始键值范围在0-此值之间
#define			KEY_COUNT	     4

#endif

