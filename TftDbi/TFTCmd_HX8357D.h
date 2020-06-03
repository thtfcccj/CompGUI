/*******************************************************************************

             FT4.3寸显示屏控制器标准驱动程序-HX8357D扩展指令集定义
//此定义在HX8357D系列控制器上使用
//此类指令显示模组出厂前，部分应已配置好并固化在OTP中???
********************************************************************************/
#ifndef TFT_CMD_HX8357D_H
#define TFT_CMD_HX8357D_H

//-----------------------------写内部时钟(决定刷新速度)-------------------
//2byte参数
#define TFT_CMD_WR_OSC     0xB0
//第1Byte位定义为：
#define TFT_OSC1_I_UADJ_SHIFT  4  //决定正常与反显时时钟，从30-105HZ越大越快
#define TFT_OSC1_N_UADJ_MASK   0x0f//决定空闲时时钟，从30-105HZ越大越快
//第2Byte位定义为：
#define TFT_OSC2_ON        (1 << 7) //开启

//---------------------------------写电源控制-----------------------------
//7byte参数
#define TFT_CMD_WR_POWER     0xB1
//第1Byte位定义为：
#define TFT_POWER1_DP_STB    (1 << 0) //深度待机模式
//第2Byte位定义为：
#define TFT_POWER2_BT_MASK   0X03     //VGH和VGL电压产生切换升压电路2的输出因数
#define TFT_POWER2_XDK       (1 << 4) //指定VSP电压产生的升压电路比率
#define TFT_POWER2_TRI       (1 << 5) //指定VSP电压产生的升压电路比率
//第3Byte位定义为：
#define TFT_POWER3_VRH_MASK  0x3f //指定VSPROUT电压调整。VSPROUT电压用于gamma电压
//第4Byte位定义为：
#define TFT_POWER4_NVRH_MASK  0x3f //指定VSNR电压调整。VSNR电压用于gamma电压
//第5Byte位定义为：
#define TFT_POWER5_GASEN     (1 << 8)//电源关闭时的异常关机监视功能
#define TFT_POWER5_AP_MASK   0X07  //调整电源中运算放大器的电流驱动量电路
//第6Byte位定义为：
#define TFT_POWER6_FS1_SHIFT  4     //设置VSP和VSN电压产生的工作频率
#define TFT_POWER6_FS0_MASK   0X0F  //设置VGH/VGL电压产生的工作频率
//第7Byte位定义为：
#define TFT_POWER7_VCOMG_ON    (1 << 6)//指定升压电路3的开/关控制以产生VSN电压
#define TFT_POWER7_PON      (1 << 4)//指定VGH、VGL电压产生的升压电路2的开/关控制
#define TFT_POWER7_DK_OFF   (1 << 3)//指定升压电路1的开/关控制以产生VSP电压
#define TFT_POWER7_STB      (1 << 0)//待机模式,不允许操作

//---------------------------------写显示控制-----------------------------
//5byte参数,主要用于正常让显示屏显示
#define TFT_CMD_WR_DISPLAY     0xB2
//第1Byte位定义为：
#define TFT_DISPLAY1_ISC0_MASK   0X0F //在非显示区域中，当REF为“1”时，指定门驱动器的扫描周期
//第2Byte位定义为：
#define TFT_DISPLAY2_REF   (1 << 0)//刷新部分模式启用位的非显示区显示,1允许
#define TFT_DISPLAY2_PTG   (1 << 1)//指定门驱动器在非显示区域的扫描模式,1固定VGL
#define TFT_DISPLAY2_PT0   (1 << 6)//指定部分显示模式下的非显示区域源输出
#define TFT_DISPLAY2_PT1   (1 << 7)//指定部分显示模式下的非显示区域源输出
//第3Byte位定义为：
#define TFT_DISPLAY3_NL_MASK   0X3F //设置每隔8行驱动LCD的行数
//第4Byte位定义为：
#define TFT_DISPLAY4_SCN_MASK 0X7F //指定门驱动器开始扫描的门线
//第5Byte位定义为：
#define TFT_DISPLAY5_D0    (1 << 2) //显示关闭时：1可操作关显，0不可操作并关显
#define TFT_DISPLAY5_D1    (1 << 3)//1显示打开，否则0关闭
#define TFT_DISPLAY5_DTE   (1 << 4)//0时固定到VGL开,否则正常
#define TFT_DISPLAY5_GON   (1 << 5)//0时固定VGH开,否则正常

//---------------------------------写RGB接口模式----------------------------
//4byte参数
#define TFT_CMD_WR_RGBIF     0xB3
//第1Byte位定义为：
#define TFT_RGBIF1_DM     (1 << 0)//用于选择显示操作模式。0内部时钟，1DPI接口
#define TFT_RGBIF1_RM     (1 << 1)//RAM访问方式: 0时用DBI接口，1时用DPI接口 
#define TFT_RGBIF1_EPF0   (1 << 4)//65K色时，RGB组合模式,默认RGB565
#define TFT_RGBIF1_EPF1   (1 << 5)//65K色时，RGB组合模式,默认RGB565
#define TFT_RGBIF1_BYPASS (1 << 6)//RGB接口时选择显示数据路径，0内存，1显示
#define TFT_RGBIF1_SDO_EN (1 << 7)//SDO引脚允许
//第2Byte位定义为：
#define TFT_RGBIF2_EPL     (1 << 0)//在RGB接口模式下指定启用管脚的极性
#define TFT_RGBIF2_VSPL    (1 << 1)//VSYNC引脚的极性, 0低速，1高速
#define TFT_RGBIF2_HSPL    (1 << 2)//HSYNC引脚的极性, 0低速，1高速
#define TFT_RGBIF2_DPL     (1 << 3)//DOTCLK pin的极性， 0上升沿，1下降沿
//第3Byte位定义为：
#define TFT_RGBIF3_HBP_MASK 0X3F //设置从HSYNC信号在无效边界时无效像素个数
#define TFT_RGBIF3_RCM     (1 << 7)//RGB模式时：0VS+HS+DE,1S+HS
//第4Byte位定义为：
#define TFT_RGBIF4_VBP_MASK 0X3F //设置从VSYNC信号在无效边界时无效像素个数

//------------------------------设置显示周期定时器---------------------------
//7byte参数
#define TFT_CMD_WR_CYC     0xB4
//第1Byte位定义为：
#define TFT_CYC1_N_NW0   (1 << 0)//在正常/部分模式下指定LCD驱动反转类型
#define TFT_CYC1_N_NW1   (1 << 1)//在正常/部分模式下指定LCD驱动反转类型
#define TFT_CYC1_I_NW0   (1 << 4)//在空闲/部分空闲模式下指定LCD驱动反转类型
#define TFT_CYC1_I_NW1   (1 << 5)//在空闲/部分空闲模式下指定LCD驱动反转类型
#define TFT_CYC1_ZIV     (1 << 8)//设置Z反转模式,1反转
//第2Byte位定义为：
#define TFT_CYC2_RTN_MASK  0X7F  //为内部操作指定一行时钟周期, =1/内部工作时钟频率（fosc）
//第3Byte位定义为：
#define TFT_CYC3_DIV0     (1 << 0)//使用内部时钟进行显示操作时的分频 1<<DIV01
#define TFT_CYC3_DIV1     (1 << 1)//使用内部时钟进行显示操作时的分频 1<<DIV01
#define TFT_CYC3_OSC_DIV2 (1 << 3)//内部时钟1分频
//第4Byte位定义为：
#define TFT_CYC4_N_NUM_MASK  0XFF//在正常/部分模式一帧的空白区域像素个数
//第5Byte位定义为：
#define TFT_CYC5_I_NUM_MASK  0XFF//指定空闲（8色）时一帧空白区域中的行数
//第6Byte位定义为：
#define TFT_CYC6_GDON        0XFF //指定单线驱动周期内的有效门输出开始时间
//第7Byte位定义为：
#define TFT_CYC7_GDOF        0XFF//指定单线驱动周期中的门输出结束时间

//------------------------------设置电源电压操作---------------------------
//3byte参数
#define TFT_CMD_WR_BGP   0xB5
//第1Byte位定义为：
#define TFT_BGP1_VREF_MASK  0X0F  //指定VREF电压，见资料
//第2Byte位定义为：
#define TFT_BGP2_nVREF_MASK  0X07  //指定nVREF电压，见资料
//第3Byte位定义为：
#define TFT_BGP3_VVDHS_MASK  0X07  //指定VSP/VSN电压，见资料
#define TFT_BGP3_VPP_SHIFT   5     //指定OTP VPP电压，见资料
#define TFT_BGP3_VPP_MASK    0XE0

//------------------------------设置VCOM电压---------------------------
//2byte参数
#define TFT_CMD_WR_VCOM   0xB6
//第1Byte位定义为：
#define TFT_VCOM1_VCOM_MASK      0X7F  //DC VCOM电压设置，见资料
#define TFT_VCOM1_OTP_TIME_MASK  0X07  //VCOM OTP编程时间设置，见资料

//------------------------------设置OTP---------------------------
//5byte参数(略)
#define TFT_CMD_WR_OTP   0xB7

//------------------------------设置外部扩展指令---------------------------
//3byte参数(略)
#define TFT_CMD_WR_EXTC   0xB9
//3byte参数,前两引导字节定义为:
#define TFT_EXTC1_PRE0    0XFF
#define TFT_EXTC2_PRE1    0X83
//第3byte参数决定禁止与开启
#define TFT_EXTC_EN_ARY   0x57
#define TFT_EXTC_DIS_ARY  0x47

//------------------------------设置STBA---------------------------
//6byte参数
#define TFT_CMD_WR_STBA   0xC0
//第1Byte
#define TFT_STBA1_NP_OPON_MASK  0XFF//指定正常模式下的有效源操作周期
//第2Byte
#define TFT_STBA2_IP_OPON_MASK  0XFF//指定空闲模式下的有效源操作周期
//第3-5Byte STBA 源偏压电流微调设置
#define TFT_STBA3_STBA_H_MASK  0XFF
#define TFT_STBA4_STBA_M_MASK  0XFF
#define TFT_STBA5_STBA_L_MASK  0XFF
//第6Byte STBA 源偏压电流微调设置
#define TFT_STBA6_GENON_MASK  0XFF//GAMA控制周期时间

//------------------------------设置DGC---------------------------
//即GAMA校准曲线查找表，100byte参数
#define TFT_CMD_WR_DGC   0xC1
#define TFT_DGC1_EN      0X80  //开启GAMA样正
//对应字节起止信息，每个33byte
#define TFT_DGC_R_START  2
#define TFT_DGC_G_START  35
#define TFT_DGC_B_START  68
//------------------------------设置ID号---------------------------
//4byte参数,前3由高到低ID,4为写OTP时间
#define TFT_CMD_WR_ID   0xC3

//------------------------------设置DDB---------------------------
//4byte参数,由高到低DDB
#define TFT_CMD_WR_DDB   0xC4

//------------------------------设置CABC---------------------------
//用于PWM控制背光亮度及开关， 2byte参数(略)
#define TFT_CMD_WR_CABC_REL   0xC9

//------------------------------设置PANNEL---------------------------
//用于设置面板特性， 1byte参数
#define TFT_CMD_WR_PANNEL   0xCC

//------------------------------设置GAMA曲线---------------------------
//34byte参数
#define TFT_CMD_WR_GAMA   0xE0
//前用于设置VRP(前16byte)与VRN(前16byte) 
#define TFT_GAMA_VRP_START  0
#define TFT_GAMA_VRN_START  16
//后2Byte略

//------------------------------设置图像格式---------------------------
//★1byte参数(略),主要用于24bit数据输入方式设置
#define TFT_CMD_WR_IMAGE   0xE9
//★权支持D0: DB_EN

//------------------------------设置图像格式---------------------------
//1byte参数
#define TFT_CMD_WR_MESSI   0xEA
//第1Byte
#define TFT_MESSI_ENB    (1 << 0)//允许命令
#define TFT_SPI_DCX_SEL  (1 << 1)//4线SPI接口 0:8th SCL, 1:1st SCL

//--------------------------------设置颜色-----------------------------
//15byte参数,组合按10bit操例, 含：
//Bkx[9:0]，Bky[9:0]: Wx[9:0]，Wy[9:0]: Rx[9:0]，Ry[9:0]:
//Gx[9:0]，Gy[9:0]: Bx[9:0]，By[9:0]: Ax[9:0]，Ay[9:0]:
#define TFT_CMD_WR_COLOR   0xEB

//------------------------------设置SPI读命令基址---------------------------
//为用户定义命令设置SPI读取命令基址, 1byte参数
#define TFT_CMD_WR_SPI_CMD_BASE   0xEA

//------------------------------读SPI命令数据--------------------------
//返回:"为用户定义命令设置SPI读取命令基址"开始的数据，首个即有效，个数不限
#define TFT_CMD_RD_SPI_CMD_DATA   0xFF

//------------------------------读HIMAX_ID--------------------------
//返回HIMAX_ID，1无效2为值
#define TFT_CMD_RD_HIMAX_ID   0XD0


#endif //#ifndef TFT_CMD_H

