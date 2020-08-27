/*******************************************************************************

           TFT MCU接口驱动-使用TypeB 16位并口时实现-私有接口文件

*******************************************************************************/


/*******************************************************************************
                               底层回调
*******************************************************************************/
#include "IoCtrl.h"

//配置IO(全部配置为输出无效电平，高速时钟)
#define TftDbi_cbIoCfg()     do{CfgLcdIo();}while(0)

//Reset信号(低电平复位)
#define TftDbi_cbStartRst()   do{ClrLcdRst(); }while(0)    //低电平开始复位
#define TftDbi_cbEndRst()     do{SetLcdRst(); }while(0)    //高电平结束

//CS信号
#define TftDbi_cbCsEn()     do{ClrLcdCs();}while(0)       //片选允许(低电平)
#define TftDbi_cbCsDis()    do{SetLcdCs();}while(0)       //片选禁止(高电平)

//RS信号
#define TftDbi_cbRsCmd()    do{ClrLcdRs();}while(0)       //命令模式(低电平)
#define TftDbi_cbRsData()   do{SetLcdRs();}while(0)      //数据模式(高电平)

//写时钟(下降沿数据，上升沿锁存)
#define TftDbi_cbWrClkH()    do{SetLcdWr();}while(0)     //写时钟高
#define TftDbi_cbWrClkL()    do{ClrLcdWr();}while(0)     //写时钟低

//读时钟(下降沿数据，上升沿锁存)
#define TftDbi_cbRdClkH()    do{SetLcdRd();}while(0)     //读时钟高
#define TftDbi_cbRdClkL()    do{ClrLcdRd();}while(0)     //读时钟低

//数据线,16bit
#define TftDbi_cbDbOut()     do{OutLcdDB();}while(0)    //置输出
#define TftDbi_cbDbWr(data)  do{WrLcdDB(data);}while(0) //写数据
#define TftDbi_cbDbIn()      do{InLcdDB();}while(0)     //置输入
#define TftDbi_cbDbRd()      RdLcdDB()                   //读数据

/*******************************************************************************
                              时间控制回调
*******************************************************************************/

#define TftDbi_cbGetUs_HwRst()   (10)            //硬件复位时间 ILI9488>=10us
#define TftDbi_cbGetMs_HwRstWait() (5)           //硬件复位后等待 ILI9488>=5ms

#define TftDbi_cbDelayTchw()   do{}while(0)     //片选取消时间,ILI9488>=0ns 
#define TftDbi_cbDelayTcs()   do{}while(0)     //片选建立时间,ILI9488>=15,读id45,读FM355

#define TftDbi_cbDelayTwrl()   do{}while(0)     //写低电平保持时间,ILI9488>=15ns 
#define TftDbi_cbDelayTwrh()   do{}while(0)      //写高电平保持时间,ILI9488>=15ns

#define TftDbi_cbDelayTrdl()   do{}while(0)     //读低电平保持时间,ILI9488>=45~160ns 
#define TftDbi_cbDelayTrdh()   do{}while(0)     //读高电平保持时间,ILI9488>=90~450ns
                              


