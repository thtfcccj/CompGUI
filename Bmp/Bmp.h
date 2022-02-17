/*******************************************************************************
                         BMP格式处理模块
此模块负责BMP格式及类似非压缩图像格式的绘制
BMP文件的图像深度可选lbit、4bit、8bit及24bit。
BMP文件存储数据时，图像的扫描方式是按从左到右、从下到上的顺序。
********************************************************************************/
#ifndef _BMP_H
#define _BMP_H

/*******************************************************************************
                               相关结构
********************************************************************************/

//-------------------------------BMP文件格式定义-------------------------------
//u16,u32时，以低位在前，高位在后方式存储，为方便使用，全部为8位对齐,
//命名规则参考：https://blog.csdn.net/u012877472/article/details/50272771
//RLE压缩算法参考：https://blog.csdn.net/u010707515/article/details/41970497
//下述格式以实际对照PS CS6版生成的bit4位文件比较通过！

//bmp文件头（bmp file header）,一共14字节:
struct _Bmp_FileHeader{ 
  unsigned char bfType[2];     //2字节，文件类型；
  unsigned char bfSize[4]：     //4字节，文件大小；
  unsigned char bfReserved1[2]：//2字节，保留，必须设置为0；
  unsigned char bfReserved2[2]：//2字节，保留，必须设置为0；
  unsigned char bfOffBits[4]：  //4字节，从头到位图数据的偏移；
};

//位图信息头（bitmap information）,一共40字节:
struct _Bmp_Infomation{ 
  unsigned char biSize[4];           //4字节，信息头的大小，即40；
  unsigned char biWidth[4];          //4字节，以像素为单位说明图像的宽度；
  unsigned char biHeight[4];         //4字节，以像素为单位说明图像的高度，同时如果为正，说明位图倒立（即数据表示从图像的左下角到右上角），如果为负说明正向；
  unsigned char biPlanes[2];         //2字节，为目标设备说明颜色平面数，总被设置为1；
  unsigned char biBitCount[2];       //2字节，说明比特数/像素数，值有1、2、4、8、16、24、32；
  unsigned char biCompression[4];    //4字节，说明图像的压缩类型，最常用的就是0（BI_RGB），表示不压缩；
  unsigned char biSizeImages[4];     //4字节，说明位图数据的大小，当用BI_RGB格式时，可以设置为0；
  unsigned char biXPelsPerMeter[4];  //表示水平分辨率，单位是像素/米，有符号整数；
  unsigned char biYPelsPerMeter[4];  //表示垂直分辨率，单位是像素/米，有符号整数；
  unsigned char biClrUsed[4];        //说明位图使用的调色板中的颜色索引数，为0说明使用所有；
  unsigned char biClrImportant[4];    //说明对图像显示有重要影响的颜色索引数，为0说明都重要
};

//关于调色板（Color Palette）
//调色板是可选的,每4个字节表示1种颜色（RGB加上Alpha值）
struct _Bmp_ColorPalette{ 
  unsigned char B;//蓝在前
  unsigned char G;
  unsigned char R;
  unsigned char A;//Alpha在后
};

//关于位图数据
//如果是24位色图，按照BGR的顺序排列，32位色图按照BGRAlpha排列
//BMP图像要求每行的数据的长度必须是4的倍数,如果不够需要进行比特填充（以0填充）

//总的文件头为
struct _Bmp_Header{
  struct _Bmp_FileHeader; //文件头14字节
  struct _Bmp_Infomation Infomation//位图信息头40字节
  //后续数据说明:
  //struct _Bmp_ColorPalette[0~biBitCount];  //可选调色板
  //unsigned char Data[];  //余于为位图数据,每行数据需以4的倍数对齐
};

//-------------------------------BMP图像压缩数据头定义-------------------------------
//总的文件头为14+40=54字节，嵌入式系统固定保存时，只需其必要的描述信息即可,以节省空间:
//以RGB 16位索引图像，16位索引色深时,空间对比:
//  原BMP格式数据头长度=14+40+16*4= 108字节
//  ZipBMP格式数据头长度=6+16*2= 38字节, 节省70字节
struct _Bmp_ZipHeader{//为便于提取与转换,以BMP格式信息出现顺序排列
  unsigned char biWidth[2];    //2字节，像素宽度,原位置0x12-0x15
  unsigned char biHeight[2];   //2字节，像素高度，固定取正，原位置0x16-0x19
  unsigned char biBitCountInfo; //1字节组合表示： b0~b4为比特数/像素数，值有1、2、4、8、16、24、32；原位置0x1C-0x1D
                                //b5~b7: 根据不同色深表示为：
                                //  像素数为1,2,4,8时：若带调色板，则b7置位表示使用自定义RgbHwType_t调整标，否则为标准
                                //  像素数为4、8时：b5=1表示为RLE方式压缩：即像素数为4时为BI_RLE4压缩 8时为BI_RLE8压缩
                                //  像素数为16时：b5~b7=0为X1R5G5B5,1为A1R5G5B5，2为R5G6B5，3为X4R4G4B4,4为A4R4G4B4
                                //  像素数为32时：b5=0表示X8R8G8B8， b5=1表示A8R8G8B8 
  unsigned char biClrUsed;     //使用的调色板中的颜色索引数，0时使用所有；原位置0x2e-0x31
  //后续数据说明:
  //struct _Bmp_ColorPalette[0~biBitCount] or RgbHwType_t [0~biBitCount];;  //可选调色板
  //  //可选嵌入式硬件相关调色板,直接对应以提高解码效率与存储空间(暂不支持)
  //unsigned char Data[];        //余下为位图数据，这里
  };

