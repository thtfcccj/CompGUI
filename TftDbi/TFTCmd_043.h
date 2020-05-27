/*******************************************************************************

                  TFT4.3寸显示屏控制器标准驱动程序-指令集定义
//此定义在ILI9488,ILI9486,ILI9481,ST7796S,HX8357D控制器上相同
********************************************************************************/
#ifndef TFT_CMD043_H
#define TFT_CMD043_H

/********************************************************************************
                                 常用操作指令
********************************************************************************/

//写空指令,无参数,主要用于终结内存数据读写
#define TFT_CMD_WR_NOP          0x00  
//写软复位指令,用于复位各寄存器状态到默认值
#define TFT_CMD_WR_SWRESET      0x01  //执行后需等5ms或120ms(睡眠时)
//读软件识别信息,回4个参数: 1无效 2工厂ID 3驱动版本 4驱动ID
#define TFT_CMD_RD_DIDIF      0x04
//读故障与DSI值,回2个参数 1无效 2位组合状态值
#define TFT_CMD_RD_NUMPE       0x05

//RGB I/F模式时读红绿蓝色位 回1个参数
#define TFT_CMD_RD_RED         0x06 //红
#define TFT_CMD_RD_GREEN       0x07 //绿
#define TFT_CMD_RD_BLUE        0x08//蓝
//具体定义为:
#define TFT_RGB_IS_MSB       (1 << 5) //5bit置位为MSB
#define TFT_RGB_IS_LSB       (1 << 1) //1bit置位为LSB
#define TFT_RGB_IS_16        (1 << 0) //置位为18位，复位为16位  

//读显示的电源模式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DPM         0x0a
//第2位组合状态值定义为
#define TFT_DPM_BOOSTER_OK (1 << 7) //BOOSTER电源开
#define TFT_DPM_IDIE_MODE  (1 << 6) //空闲模式
#define TFT_DPM_PARTIAL_ON (1 << 5) //部分显示开
#define TFT_DPM_SLEEP_OUT  (1 << 4) //Sleep Out Mode.
#define TFT_DPM_DISP_NOR   (1 << 3) //正常显示模式
#define TFT_DPM_DISP_ON    (1 << 2) //显示开

//读显示的电源模式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DMADCTL        0x0b
//第2位组合状态值定义为
#define TFT_MAD_PAGE_BT    (1 << 7)  //置位时页(即行数)从下向上
#define TFT_MAD_COL_RL     (1 << 6)  //置位时列数从右到左
#define TFT_MAD_PC_REVERSE (1 << 5)  //置位时行列反转,即横坚显
#define TFT_MAD_LINE_BT    (1 << 4)  //置位时刷新从右到左
#define TFT_MAD_BGR        (1 << 3)  //置位时BGR排列，否则为RGB
#define TFT_MAD_LATCH_RL   (1 << 2)  //置位时显示锁存从右到左

//读显示的像素格式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DCOLMOD        0x0C
//第2位组合状态值定义为
#define TFT_DCOL_DPI_SHIFT   4  //DPI Interface Pixel format
#define TFT_DCOL_DPI_MASK   (7 << TFT_DCOL_DPI_SHIFT)
#define TFT_DCOL_DBI_SHIFT   0  //DPI Interface Pixel format
#define TFT_DCOL_DBI_MASK   (7 << TFT_DCOL_DBI_SHIFT)
//具体定义为:
#define TFT_DCOL_PIXEL_12BIT  3 //12 bit/pixel
#define TFT_DCOL_PIXEL_16BIT  4 //16 bit/pixel
#define TFT_DCOL_PIXEL_18BIT  6 //12 bit/pixel
#define TFT_DCOL_PIXEL_24BIT  7 //16 bit/pixel

//读显示的图像格式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DIM        0x0D
//第2位组合状态值定义为
#define TFT_DIM_V_SCROLL_ON   (1 << 7)  //垂直滚动开
#define TFT_DIM_INVERSION     (1 << 5)  //显示反显
//#define TFT_DIM_DISP_ALL      (1 << 4)  //全显，即白屏->不支持
//#define TFT_DIM_DISP_NULL     (1 << 3)  //全关，即黑屏->不支持
#define TFT_DIM_GAMA_MASK    0X07  //Gamma曲线校准选择，具体为：
#define TFT_DIM_GAMA_GC0     0X00  //Gamma 选GC0
#define TFT_DIM_GAMA_GC1     0X01  //Gamma 选GC1
#define TFT_DIM_GAMA_GC2     0X02  //Gamma 选GC2
#define TFT_DIM_GAMA_GC3     0X03  //Gamma 选GC3

//读显示的信号格式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DSM        0x0E
//第2位组合状态值定义为
//Tearing MODE 开启时将输出一个扫描同步信息号给MCU以提供相关信息。
#define TFT_DSM_TERING_ON      (1 << 7)  //Tearing效果开
#define TFT_DSM_TERING_MODE    (1 << 6)  //Tearing模式选择0或1
#define TFT_DSM_DSI_ERR        (1 << 5)  //

//读自诊断结果,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DSDR        0x0F
//第2位组合状态值定义为
//Tearing MODE 开启时将输出一个扫描同步信息号给MCU以提供相关信息。
#define TFT_DSR_REG_LOAD      (1 << 7)  //Register Loading Detection
#define TFT_DSR_FUNM           (1 << 7)  //Functionality Detection
#define TFT_DSR_CHECKSUM_ERR   (1 << 5)  //校验和一致性

//进入睡眠模式，无形参，120ms后进入
#define TFT_CMD_WR_SLPIN       0x10
//退出睡眠模式，无形参，5ms后退出
#define TFT_CMD_WR_SLPOUT      0x11

//开启局部显示模式，无形参，
#define TFT_CMD_WR_PTLON       0x12
//开启正常显示模式，无形参
#define TFT_CMD_WR_NORON       0x13

//关闭反显，无形参
#define TFT_CMD_WR_INVOFF     0x20
//开启反显，无形参
#define TFT_CMD_WR_INVON      0x21

//全黑显示，无形参
#define TFT_CMD_WR_ALLPOFF    0x22
//全白显示，无形参
#define TFT_CMD_WR_ALLPON     0x23

//GAMA设置 即选择GAMA表,形参跟0-3的位移值,其它无效
#define TFT_CMD_WR_GAMSET     0x26

//关闭显示，无形参
#define TFT_CMD_WR_DISPOFF    0x28
//开启显示，无形参
#define TFT_CMD_WR_DISPON     0x29

//空闲模式关闭，无形参
#define TFT_CMD_WR_IDMOFF      0x38
//空闲模式开启，无形参 空闲模式时RGB颜色只有8位色深
#define TFT_CMD_WR_IDMON      0x39

//关闭Tearing
#define TFT_CMD_WR_TEOFF      0x34
//开启Tearing，无形参
#define TFT_CMD_WR_TEON       0x35

/********************************************************************************
                                 内存操作相关指令
********************************************************************************/
//设置列范围，跟4组8bit形参,前2byte由高到低表示列起始，后2byte由高到低表示列结束
#define TFT_CMD_WR_CASET      0x2A
//设置行范围，跟4组8bit形参,前2byte由高到低表示行起始，后2byte由高到低表示行结束
#define TFT_CMD_WR_PASET      0x2B
//写RAM,写入位置为以前定义的行列范围组成的矩形区,可连续入，多写无效，位数为定义的显示位数
#define TFT_CMD_WR_RAM        0x2C
//读RAM,读入位置为以前定义的行列范围组成的矩形区,可连续出，多读无效，位数为定义的显示位数
#define TFT_CMD_RD_RAM        0x2E

//设置局部显示模式区域,前2byte由高到低表示行起始，后2byte由高到低表示行结束
#define TFT_CMD_WR_PLTAR      0x30
//垂直滚动定义,共6byte:
//  前1-2byte由高到低表示顶部固定区域，前3-4byte由高到低表示垂直滚动起始
//  前5-6byte由高到低表示底部固定区域
#define TFT_CMD_WR_VSCRDEF    0x33

//内存访问控制,控制内读读写时，内存的增长方式(可用于控制坐标0点)，跟1Byte形参
#define TFT_CMD_WR_MADCTL    0x36
//1Byte形参位定义为:
#define TFT_MAC_MY         (1 << 7)//行排列方向
#define TFT_MAC_MX         (1 << 6)//列排列方向
#define TFT_MAC_MV         (1 << 5)//行列选择
#define TFT_MAC_ML         (1 << 5)//LCD vertical refresh direction control
#define TFT_MAC_BGR        (1 << 5)//RGB-BGR ORDER (BGR)
#define TFT_MAC_SS         (1 << 5)//LCD horizontal refresh direction control

//单写滚动光标地址,与33h配合使用，3byte形参：
//前1Byte描述滚动方式 ，前2~3bit描述起始位置
#define TFT_CMD_WR_VSCRSADD   0x37

//接口像素格式,跟1byte形参：
// DBI时同TFT_DCOL_PIXEL定义, DPI时4/6/7表示16/18/24
#define TFT_CMD_WR_COLMOD      0x3a

//继续上次的写内存
#define TFT_CMD_WR_MEM_CONT    0x3C
//继续上次的读内存
#define TFT_CMD_RD_MEM_CONT    0x3E

/********************************************************************************
                                 功能操作指令
********************************************************************************/

//设置撕裂扫描线, 跟3组8bit形参,由高到低表示位置
#define TFT_CMD_WR_MEM_TESL    0x44
//读回设置的撕裂扫描线，返回2Byte 1无效2同上
#define TFT_CMD_RD_MEM_TESL    0x45

//设置灰度级，跟8Byte即255级亮度
#define TFT_CMD_WR_DISBV       0x51
//读取设置的灰度级，返回2Byte 1无效2同上
#define TFT_CMD_RD_DISBV       0x52

//设置显示亮度(即控制LED背光的亮度), 跟1byte形参
#define TFT_CMD_WR_CTRLD       0x53
//1byte形参位定义为:
#define TFT_CTRLD_BCTRL_ON    (1 << 5) //亮度控制模块开启
#define TFT_CTRLD_DD_ON       (1 << 3) //亮度变暗
#define TFT_CTRLD_BL_ON       (1 << 2) //亮度控制开
//读设置的显示亮度，返回2Byte 1无效2同上
#define TFT_CMD_RD_CTRLD       0x54

//写内容自适应亮度控制, 跟1byte形参
#define TFT_CMD_WR_CABC       0x55
//1byte形参定义为:
#define TFT_CABC_OFF         0  //关闭
#define TFT_CABC_USER        1  //User Interface Image
#define TFT_CABC_STILL       2  //Still Picture
#define TFT_CABC_IMAGE       3  //Moving Image
//读内容自适应亮度控制,返回2Byte 1无效2同上
#define TFT_CMD_RD_CABC       0x56
//写内容自适应亮度控制最小亮度, 跟1byte形参计255级
#define TFT_CMD_WR_CABCMB     0x5E
//读内容自适应亮度控制最小亮度，返回2Byte 1无效2同上
#define TFT_CMD_RD_CABCMB     0x5F

//读自动亮度控制自诊断结果，返回2Byte 1无效
#define TFT_CMD_RD_ABCSDR     0x68
//返回第2Byte定义为：
#define TFT_CABCMB_REG_LOAD   (1 << 7) //寄存器装载诊断
#define TFT_CABCMB_FUN_DET    (1 << 6) //功能性诊断

//读黑/白最低位，返回黑白颜色特征的最低位 返回2Byte 1无效2为值
#define TFT_CMD_RD_BWLK     0x70
//读BKx(黑色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Bkx     0x71
//读Bky(黑色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Bky     0x72
//读Wx(白色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Wx      0x73
//读Wy(白色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Wy      0x74
//读红绿最低位, 返回红绿颜色特征的最低位 返回2Byte 1无效2为值:高4bit红，低4bit绿
#define TFT_CMD_RD_RGx    0x75
//读Rx(红色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Rx     0x76
//读Ry(红色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Ry     0x77
//读Gx(绿色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Gx     0x78
//读Gy(绿色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Gy     0x79
//读蓝与Alpha最低位, 返回蓝A特征的最低位 返回2Byte 1无效2为值:高4bit蓝，低4bitA
#define TFT_CMD_RD_BAx    0x7A
//读Bx(蓝色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Bx     0x7B
//读By(蓝色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_By     0x7C
//读Ax(Alpha特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Ax     0x7D
//读By(Alpha特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Ay     0x7E

//读DDB起始, 返回6Byte 1无效2-5为值, 6Byte无效
#define TFT_CMD_RD_DDB_START 0xA1
//继续读DDB, 返回nByte 1无效,其它为DDB值 
#define TFT_CMD_RD_DDB_CONT 0xA8

//读第1个校验和结果, 返回2Byte 1无效,2为值 
#define TFT_CMD_RD_FCS     0xAA
//继续读第二个校验和结果, 返回2Byte 1无效,2为值 
#define TFT_CMD_RD_CCS     0xAF

//读ID1即厂商ID,返回2Byte 1无效,2为值 
#define TFT_CMD_RD_ID1     0xDA
//读ID2即版本号,返回2Byte 1无效,2为值 
#define TFT_CMD_RD_ID2     0xDB
//读ID3即模组ID,返回2Byte 1无效,2为值 
#define TFT_CMD_RD_ID3     0xDC

//写内部时钟(决定刷新速度), 2byte参数
#define TFT_CMD_WR_OSC     0xB0
//第1Byte位定义为：
#define TFT_OSC1_I_UADJ_SHIFT  4  //决定正常与反显时时钟，从30-105HZ越大越快
#define TFT_OSC1_N_UADJ_MASK   0x0f//决定空闲时时钟，从30-105HZ越大越快
//第2Byte位定义为：
#define TFT_OSC2_ON        (1 << 7) //开启

/********************************************************************************
                                底层配置指令
//此类指令显示模组出厂前，部分应已配置好并固化在OTP中???
********************************************************************************/
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
//1byte参数(略),主要用于24bit数据输入方式设置
#define TFT_CMD_WR_IMAGE   0xE9

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

