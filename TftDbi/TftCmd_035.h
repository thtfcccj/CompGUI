/*******************************************************************************

                  TFT3.5寸显示屏控制器标准驱动程序-基础指令集定义
//此定义在ILI9488,ILI9486,ILI9481,ST7796S,HX8357D控制器上相同
********************************************************************************/
#ifndef TFT_CMD035_H
#define TFT_CMD035_H

//☆HX(8357D)支持指令(支持功能最多)
//★ILI(9488)支持指令(支持功能次之)
//※ST(7796S)支持指令(支持功能最少)

//上述仅比较命令码部分，以未对具体数据定义进行比较!!!!!!!!!!!
//下述位定义以HX8357D为准

/********************************************************************************
                                 常用操作指令
********************************************************************************/

//☆★※写空指令,无参数,主要用于终结内存数据读写
#define TFT_CMD_WR_NOP          0x00  
//☆★※写软复位指令,用于复位各寄存器状态到默认值
#define TFT_CMD_WR_SWRESET      0x01  //执行后需等5ms或120ms(睡眠时)
//☆★※读软件识别信息,回4个参数: 1无效 2工厂ID 3驱动版本 4驱动ID
#define TFT_CMD_RD_DIDIF      0x04
//☆★※读故障与DSI值,回2个参数 1无效 2位组合状态值
#define TFT_CMD_RD_NUMPE       0x05

//☆ RGB I/F模式时读红绿蓝色位 回1个参
//☆ 红数
#define TFT_CMD_RD_RED         0x06 
//☆ 绿
#define TFT_CMD_RD_GREEN       0x07 
//☆ 蓝
#define TFT_CMD_RD_BLUE        0x08
//具体定义为:
#define TFT_RGB_IS_MSB       (1 << 5) //5bit置位为MSB
#define TFT_RGB_IS_LSB       (1 << 1) //1bit置位为LSB
#define TFT_RGB_IS_16        (1 << 0) //置位为18位，复位为16位  

//★※Read Display Status (09h) //经比对返回定义(暂略)完全不同
#define TFT_CMD_RD_DISP         0x09

//☆★※读显示的电源模式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DPM         0x0a
//第2位组合状态值定义为
#define TFT_DPM_BOOSTER_OK (1 << 7) //BOOSTER电源开
#define TFT_DPM_IDIE_MODE  (1 << 6) //空闲模式
#define TFT_DPM_PARTIAL_ON (1 << 5) //部分显示开
#define TFT_DPM_SLEEP_OUT  (1 << 4) //Sleep Out Mode.
#define TFT_DPM_DISP_NOR   (1 << 3) //正常显示模式
#define TFT_DPM_DISP_ON    (1 << 2) //显示开

//☆★※读显示的电源模式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DMADCTL        0x0b
//第2位组合状态值定义为
#define TFT_MAD_PAGE_BT    (1 << 7)  //置位时页(即行数)从下向上
#define TFT_MAD_COL_RL     (1 << 6)  //置位时列数从右到左
#define TFT_MAD_PC_REVERSE (1 << 5)  //置位时行列反转,即横坚显
#define TFT_MAD_LINE_BT    (1 << 4)  //置位时刷新从右到左
#define TFT_MAD_BGR        (1 << 3)  //置位时BGR排列，否则为RGB
#define TFT_MAD_LATCH_RL   (1 << 2)  //置位时显示锁存从右到左

//☆★※读显示的像素格式,回2个参数 1无效 2位组合状态值:
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

//☆★※读显示的图像格式,回2个参数 1无效 2位组合状态值:
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

//☆★※读显示的信号格式,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DSM        0x0E
//第2位组合状态值定义为
//Tearing MODE 开启时将输出一个扫描同步信息号给MCU以提供相关信息。
#define TFT_DSM_TERING_ON      (1 << 7)  //Tearing效果开
#define TFT_DSM_TERING_MODE    (1 << 6)  //Tearing模式选择0或1
#define TFT_DSM_DSI_ERR        (1 << 5)  //

//☆★※读自诊断结果,回2个参数 1无效 2位组合状态值:
#define TFT_CMD_RD_DSDR        0x0F
//第2位组合状态值定义为
//Tearing MODE 开启时将输出一个扫描同步信息号给MCU以提供相关信息。
#define TFT_DSR_REG_LOAD      (1 << 7)  //Register Loading Detection
#define TFT_DSR_FUNM           (1 << 7)  //Functionality Detection
#define TFT_DSR_CHECKSUM_ERR   (1 << 5)  //校验和一致性

//☆★※进入睡眠模式，无形参，120ms后进入
#define TFT_CMD_WR_SLPIN       0x10
//☆★※退出睡眠模式，无形参，5ms后退出
#define TFT_CMD_WR_SLPOUT      0x11

//☆★※开启局部显示模式，无形参，
#define TFT_CMD_WR_PTLON       0x12
//☆★※开启正常显示模式，无形参
#define TFT_CMD_WR_NORON       0x13

//☆★※关闭反显，无形参
#define TFT_CMD_WR_INVOFF     0x20
//☆★※开启反显，无形参
#define TFT_CMD_WR_INVON      0x21

//☆★全黑显示，无形参
#define TFT_CMD_WR_ALLPOFF    0x22
//☆★全白显示，无形参
#define TFT_CMD_WR_ALLPON     0x23

//☆ GAMA设置 即选择GAMA表,形参跟0-3的位移值,其它无效
#define TFT_CMD_WR_GAMSET     0x26

//☆★※关闭显示，无形参
#define TFT_CMD_WR_DISPOFF    0x28
//☆★※开启显示，无形参
#define TFT_CMD_WR_DISPON     0x29

//☆★※关闭Tearing
#define TFT_CMD_WR_TEOFF      0x34
//☆★※开启Tearing，无形参
#define TFT_CMD_WR_TEON       0x35

//☆★※空闲模式关闭，无形参
#define TFT_CMD_WR_IDMOFF      0x38
//☆★※空闲模式开启，无形参 空闲模式时RGB颜色只有8位色深
#define TFT_CMD_WR_IDMON      0x39

/********************************************************************************
                                 内存操作相关指令
********************************************************************************/
//☆★※设置列范围，跟4组8bit形参,前2byte由高到低表示列起始，后2byte由高到低表示列结束
#define TFT_CMD_WR_CASET      0x2A
//☆★※设置行范围，跟4组8bit形参,前2byte由高到低表示行起始，后2byte由高到低表示行结束
#define TFT_CMD_WR_PASET      0x2B
//☆★※写RAM,写入位置为以前定义的行列范围组成的矩形区,可连续入，多写无效，位数为定义的显示位数
#define TFT_CMD_WR_RAM        0x2C
//☆★※读RAM,读入位置为以前定义的行列范围组成的矩形区,可连续出，多读无效，位数为定义的显示位数
#define TFT_CMD_RD_RAM        0x2E

//☆★※设置局部显示模式区域,前2byte由高到低表示行起始，后2byte由高到低表示行结束
#define TFT_CMD_WR_PLTAR      0x30

//☆★※垂直滚动定义,共6byte:
//  前1-2byte由高到低表示顶部固定区域，前3-4byte由高到低表示垂直滚动起始
//  前5-6byte由高到低表示底部固定区域
#define TFT_CMD_WR_VSCRDEF    0x33

//☆内存访问控制,控制内读读写时，内存的增长方式(可用于控制坐标0点)，跟1Byte形参
#define TFT_CMD_WR_MADCTL    0x36
//1Byte形参位定义为:
#define TFT_MAC_MY         (1 << 7)//行排列方向
#define TFT_MAC_MX         (1 << 6)//列排列方向
#define TFT_MAC_MV         (1 << 5)//行列选择,即模坚屏切换，需配合行列方向进行
#define TFT_MAC_ML         (1 << 4)//LCD vertical refresh direction control
#define TFT_MAC_BGR        (1 << 3)//RGB-BGR ORDER (BGR)
#define TFT_MAC_SS         (1 << 2)//LCD horizontal refresh direction control

//☆单写滚动光标地址,与33h配合使用，3byte形参：
//前1Byte描述滚动方式 ，前2~3bit描述起始位置
#define TFT_CMD_WR_VSCRSADD   0x37

//☆★※接口像素格式,跟1byte形参：
// DBI时同TFT_DCOL_PIXEL定义, DPI时4/6/7表示16/18/24
#define TFT_CMD_WR_COLMOD      0x3a

//☆★※继续上次的写内存
#define TFT_CMD_WR_MEM_CONT    0x3C
//☆★※继续上次的读内存
#define TFT_CMD_RD_MEM_CONT    0x3E

/********************************************************************************
                                 功能操作指令
********************************************************************************/

//☆★※设置撕裂扫描线, 跟3组8bit形参,由高到低表示位置
#define TFT_CMD_WR_MEM_TESL    0x44
//☆★※读回设置的撕裂扫描线，返回2Byte 1无效2同上
#define TFT_CMD_RD_MEM_TESL    0x45

//☆★※设置灰度级，跟8Byte即255级亮度
#define TFT_CMD_WR_DISBV       0x51
//☆★※读取设置的灰度级，返回2Byte 1无效2同上
#define TFT_CMD_RD_DISBV       0x52

//☆★※设置显示亮度(即控制LED背光的亮度), 跟1byte形参
#define TFT_CMD_WR_CTRLD       0x53
//1byte形参位定义为:
#define TFT_CTRLD_BCTRL_ON    (1 << 5) //亮度控制模块开启
#define TFT_CTRLD_DD_ON       (1 << 3) //亮度变暗
#define TFT_CTRLD_BL_ON       (1 << 2) //亮度控制开

//☆★※读设置的显示亮度，返回2Byte 1无效2同上
#define TFT_CMD_RD_CTRLD       0x54

//☆★※写内容自适应亮度控制, 跟1byte形参
#define TFT_CMD_WR_CABC       0x55
//1byte形参定义为:
#define TFT_CABC_OFF         0  //关闭
#define TFT_CABC_USER        1  //User Interface Image
#define TFT_CABC_STILL       2  //Still Picture
#define TFT_CABC_IMAGE       3  //Moving Image

//☆★※读内容自适应亮度控制,返回2Byte 1无效2同上
#define TFT_CMD_RD_CABC       0x56
//☆★※写内容自适应亮度控制最小亮度, 跟1byte形参计255级
#define TFT_CMD_WR_CABCMB     0x5E
//☆★※读内容自适应亮度控制最小亮度，返回2Byte 1无效2同上
#define TFT_CMD_RD_CABCMB     0x5F

//☆★读自动亮度控制自诊断结果，返回2Byte 1无效
#define TFT_CMD_RD_ABCSDR     0x68
//返回第2Byte定义为：
#define TFT_CABCMB_REG_LOAD   (1 << 7) //寄存器装载诊断
#define TFT_CABCMB_FUN_DET    (1 << 6) //功能性诊断

//☆读黑/白最低位，返回黑白颜色特征的最低位 返回2Byte 1无效2为值
#define TFT_CMD_RD_BWLK     0x70
//☆读BKx(黑色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Bkx     0x71
//☆读Bky(黑色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Bky     0x72
//☆读Wx(白色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Wx      0x73
//☆读Wy(白色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Wy      0x74
//☆读红绿最低位, 返回红绿颜色特征的最低位 返回2Byte 1无效2为值:高4bit红，低4bit绿
#define TFT_CMD_RD_RGx    0x75
//☆读Rx(红色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Rx     0x76
//☆读Ry(红色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Ry     0x77
//☆读Gx(绿色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Gx     0x78
//☆读Gy(绿色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Gy     0x79
//☆读蓝与Alpha最低位, 返回蓝A特征的最低位 返回2Byte 1无效2为值:高4bit蓝，低4bitA
#define TFT_CMD_RD_BAx    0x7A
//☆读Bx(蓝色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Bx     0x7B
//☆读By(蓝色特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_By     0x7C
//☆读Ax(Alpha特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Ax     0x7D
//☆读By(Alpha特征)位2-9bit, 返回2Byte 1无效2为值
#define TFT_CMD_RD_Ay     0x7E

//☆读DDB起始, 返回6Byte 1无效2-5为值, 6Byte无效
#define TFT_CMD_RD_DDB_START 0xA1
//☆继续读DDB, 返回nByte 1无效,其它为DDB值 
#define TFT_CMD_RD_DDB_CONT 0xA8

//☆※读第1个校验和结果, 返回2Byte 1无效,2为值 
#define TFT_CMD_RD_FCS     0xAA
//☆※继续读第二个校验和结果, 返回2Byte 1无效,2为值 
#define TFT_CMD_RD_CCS     0xAF

//☆★※读ID1即厂商ID,返回2Byte 1无效,2为值 
#define TFT_CMD_RD_ID1     0xDA
//☆★※读ID2即版本号,返回2Byte 1无效,2为值 
#define TFT_CMD_RD_ID2     0xDB
//☆★※读ID3即模组ID,返回2Byte 1无效,2为值 
#define TFT_CMD_RD_ID3     0xDC

#endif //#ifndef TFT_CMD_H

