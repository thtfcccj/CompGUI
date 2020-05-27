/*******************************************************************************
                        绘图库标准接口-底层操作函数接口
通过此接口使Plot模块独立于度层显示屏硬件实现
********************************************************************************/
#ifndef _PLOT_CB_HW_H
#define _PLOT_CB_HW_H

/*******************************************************************************
                              外部显存实现
********************************************************************************/
#ifdef SUPPORT_EX_VFB    //外部显存实现
#include "TftDbi.h"      //通过DBI接口操作显示屏显存

//---------------------------绝对定位某个位置与区域函数-------------------------
//这里返回显示地址位置以便下次使用
//->这里设置显存操作范围，并预置写显存命令，下一步写显存,返回NULL指针
Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
                              unsigned short w, unsigned short h);

//---------------------------到下一行起始-------------------------------------
//本行显示完成定位到下一行，返回显示地址位置以便下次使用
//void Plot_cbToNextRowStart(Color_t *pColor, unsigned short NextPosInfo);
//->这里因为连续写内存模式将自动指向下一个，故此函数暂不需实现
#define Plot_cbToNextRowStart(pcolor, nextPosInfo)  ((pcolor) + (nextPosInfo))  

//----------------------------设置当前位置颜色--------------------------------
//同时指下一需写入的位置
//void Plot_cbSetCurColor(Color_t *pColor, Color_t Color);
//->因已提前预置读写显存命令，这里直接写数据到显存即可。
#define Plot_cbSetCurColor(pcolor, cc) do{TftDbi_WrColor(cc); (pcolor)++; }while(0)

//----------------------------更新至下一位置--------------------------------
//不写入颜色，仅将下次写入位置移至下一个点
//void Plot_cbUpdateNext(Color_t *pColor);
//不写入时，内置显示无法到下一点，这里需实现为：读此点以到下一点后恢复写显存命令
extern void Plot_cbUpdateNextFun(void);
#define Plot_cbUpdateNext(pcolor) do{Plot_cbUpdateNextFun(); (pcolor)++; }while(0)

/*******************************************************************************
                        默认使用内部显存实现
********************************************************************************/
#else //内置显存:
#include "TftDrv.h"
//---------------------------绝对定位某个位置与区域函数-------------------------
//这里返回显示地址位置以便下次使用
//Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
//                              unsigned short w, unsigned short h);
#define Plot_cbAbsLocalArea(xX, yY, wW, hH)  (&TftDrv_Buf[yY][xX])  

//---------------------------到下一行起始-------------------------------------
//本行显示完成定位到下一行，返回显示地址位置以便下次使用
//void Plot_cbToNextRowStart(Color_t *pColor, unsigned short NextPosInfo);
#define Plot_cbToNextRowStart(pcolor, nextPosInfo)  ((pcolor) + (nextPosInfo))  

//----------------------------设置当前位置颜色--------------------------------
//同时指下一需写入的位置
//void Plot_cbSetCurColor(Color_t *pColor, Color_t Color);
#define Plot_cbSetCurColor(pcolor, cc)  do{*(pcolor) = cc; (pcolor)++; }while(0)

//----------------------------更新至下一位置--------------------------------
//不写入颜色，仅将下次写入位置移至下一个点
//Color_t *pColor Plot_cbUpdateNext(Color_t *pColor);
#define Plot_cbUpdateNext(pcolor)  do{(pcolor)++; }while(0)


#endif 
#endif //_PLOT_CB_HW_H


