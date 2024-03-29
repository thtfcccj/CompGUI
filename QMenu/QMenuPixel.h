/***********************************************************************

		             菜单系统之像素像实现相关
//此模块在支持像素化时包含
***********************************************************************/
#ifndef _QMENU_PIXEL_H
#define	_QMENU_PIXEL_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
#ifdef SUPPORT_QMENU_PIXEL  //像素化支持时

/***********************************************************************
                        像素化支持说明
***********************************************************************/
//支持像素方式显示(用于小型点阵屏，如128*64OLED)时,需借助外部显示实现为:
//两行显示，分别为：
//  第一行：菜单及子菜单字符提示
//  第二行：具体数值，此栏的调整方式保持原有数码管风格但可着重
//两行正在调整项可通过着重或加大显示方式显示提示。
//以128*64为例，可使用区域为128*48，即中文字符8*3：
//菜单模式时，行1加大1倍显示(占两行两列), 行2提示当前值
//调整模式时：行2加大1倍显示(占两行两列)，行1提示如单位等
//模式:            数值模式      位调整模式(列出8位逻辑) 十六进制模式     
//选择时行1：    |高报设置|         /其它参数|           |输出逻辑|    加大一倍显示
//选择时行2:      25.0%LEL     ■□□■□□□□□□□□    0000        正常显示

//调整时行1：  |高报设置%LEL|     /->小数点位置|         |输出逻辑|    正常显示
//调整时行2:       25.0             ■□□■                0000          加大一倍显示

/***********************************************************************
                        QMenuPara里的PixelDesc
***********************************************************************/
//主结构:
struct QMenuPixel_Desc{
  const char *pMenuName;    //菜单名称
  const void *pAppendDesc;  //与调整结构相关的附加信息指针,见定义(可为NULL)                       
};

//AdjBit与AdjAll时信息描述定义:
struct QMenuPixel_DescBitAll{
  const char *pSuffix;   //显示模式时值后缀，如单位(25.0%LEL),NULL时无
};

//AdjLogic与AdjHex时，信息描述结构定义:
//以字符串阵列（const char *const *）方式组织，每个字符串为可解析结构
//可解析结构定义为:
//以数字开始,表示位描述，  模板为：    n※x>名称：%: 
// ● n： 表示对应位用此字符串替换,可以多个，如01表示B0与B1位
// ● ※x: 为变量替换字符， 无变量时无此位，多个变量时，依次排列，※定义为:
//     ■ B： 位开关： 0关1开,x不需要  
//     ■ A： 位开关： 开1关,x不需要  
//     ■ Sx：状态查找, x表示此阵列下的ID号为阵列字符查找表
// ● >：后跟菜单名称
// ● %？表示显示为变量:
//  ■ %：此字符只有一个变量
//  ■ %1,多个变量时，第1个变量
//示例(假定为0b0011)：
// ■  "3A>反浓度: %";    //显示为：“反浓度: 关闭”,即B2为反浓度位,0时关闭
// ■  "2B>超量程: %";    //显示为：“超量程: 开启”,即B2为超量程位，0时开启
// ■  "01S1>工作状态: %";//显示为：“工作状态: 开启”,即B0:1为状态0b11

/***********************************************************************
    QMenuMng里,像素化专用回调函数声明(原回调函数无说明依然有效)
***********************************************************************/
struct _QMenuMng;

//---------------------菜单调整模式时的显示---------------------------
void QMenuMng_cbUpdateDispMenuSel(const struct _QMenuMng *pMng);



#endif //SUPPORT_QMENU_PIXEL
#endif //_QMENU_PIXEL_H
