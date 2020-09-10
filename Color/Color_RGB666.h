/*******************************************************************************

              彩色显示屏时,标准颜色-在使用RGB6*6*6色时的标准颜色定义
//参考https://wenku.baidu.com/view/d9e7d417c5da50e2524d7fbe.html方案，但ID不同
********************************************************************************/

#ifndef _COLOR_RGB666_H
#define _COLOR_RGB666_H

//----------------------------6*6*6标准标准颜色索引值定义------------------------
typedef unsigned char Color_t; //定义颜色类型占位
#define COLOR_COUNT    (6 * 6 * 6) //支持的颜色数量

//----------------------------6*6*6标准标准颜色索引值定义------------------------
//==================1组(0-36)=====================
#define RGB000000    (0 + 0)        
#define RGB000033    (0 + 1)
#define RGB000066    (0 + 2)        
#define RGB000099    (0 + 3)
#define RGB0000CC    (0 + 4)        
#define RGB0000FF    (0 + 5)
#define RGB003300    (0 + 6) //-----
#define RGB003333    (0 + 7)        
#define RGB003366    (0 + 8)
#define RGB003399    (0 + 9)        
#define RGB0033CC    (0 + 10) 
#define RGB0033FF    (0 + 11)//-----
#define RGB006600    (0 + 12)
#define RGB006633    (0 + 13)        
#define RGB006666    (0 + 14)
#define RGB006699    (0 + 15)        
#define RGB0066CC    (0 + 16) 
#define RGB0066FF    (0 + 17)//-----
#define RGB009900    (0 + 18)
#define RGB009933    (0 + 19)        
#define RGB009966    (0 + 20)
#define RGB009999    (0 + 21)        
#define RGB0099CC    (0 + 22) 
#define RGB0099FF    (0 + 23)//-----
#define RGB00CC00    (0 + 24)
#define RGB00CC33    (0 + 25)        
#define RGB00CC66    (0 + 26)
#define RGB00CC99    (0 + 27)        
#define RGB00CCCC    (0 + 28) 
#define RGB00CCFF    (0 + 29)//-----
#define RGB00FF00    (0 + 30)
#define RGB00FF33    (0 + 31)        
#define RGB00FF66    (0 + 32)
#define RGB00FF99    (0 + 33)        
#define RGB00FFCC    (0 + 34) 
#define RGB00FFFF    (0 + 35)//-----
//==================2组(36-71)=====================
#define RGB330000    (36 + 0)        
#define RGB330033    (36 + 1)
#define RGB330066    (36 + 2)        
#define RGB330099    (36 + 3)
#define RGB3300CC    (36 + 4)        
#define RGB3300FF    (36 + 5)
#define RGB333300    (36 + 6) //-----
#define RGB333333    (36 + 7)        
#define RGB333366    (36 + 8)
#define RGB333399    (36 + 9)        
#define RGB3333CC    (36 + 10) 
#define RGB3333FF    (36 + 11)//-----
#define RGB336600    (36 + 12)
#define RGB336633    (36 + 13)        
#define RGB336666    (36 + 14)
#define RGB336699    (36 + 15)        
#define RGB3366CC    (36 + 16) 
#define RGB3366FF    (36 + 17)//-----
#define RGB339900    (36 + 18)
#define RGB339933    (36 + 19)        
#define RGB339966    (36 + 20)
#define RGB339999    (36 + 21)        
#define RGB3399CC    (36 + 22) 
#define RGB3399FF    (36 + 23)//-----
#define RGB33CC00    (36 + 24)
#define RGB33CC33    (36 + 25)        
#define RGB33CC66    (36 + 26)
#define RGB33CC99    (36 + 27)        
#define RGB33CCCC    (36 + 28) 
#define RGB33CCFF    (36 + 29)//-----
#define RGB33FF00    (36 + 30)
#define RGB33FF33    (36 + 31)        
#define RGB33FF66    (36 + 32)
#define RGB33FF99    (36 + 33)        
#define RGB33FFCC    (36 + 34) 
#define RGB33FFFF    (36 + 35)//-----
//==================3组(72-106)=====================
#define RGB660000    (72 + 0)        
#define RGB660033    (72 + 1)
#define RGB660066    (72 + 2)        
#define RGB660099    (72 + 3)
#define RGB6600CC    (72 + 4)        
#define RGB6600FF    (72 + 5)
#define RGB663300    (72 + 6) //-----
#define RGB663333    (72 + 7)        
#define RGB663366    (72 + 8)
#define RGB663399    (72 + 9)        
#define RGB6633CC    (72 + 10) 
#define RGB6633FF    (72 + 11)//-----
#define RGB666600    (72 + 12)
#define RGB666633    (72 + 13)        
#define RGB666666    (72 + 14)
#define RGB666699    (72 + 15)        
#define RGB6666CC    (72 + 16) 
#define RGB6666FF    (72 + 17)//-----
#define RGB669900    (72 + 18)
#define RGB669933    (72 + 19)        
#define RGB669966    (72 + 20)
#define RGB669999    (72 + 21)        
#define RGB6699CC    (72 + 22) 
#define RGB6699FF    (72 + 23)//-----
#define RGB66CC00    (72 + 24)
#define RGB66CC33    (72 + 25)        
#define RGB66CC66    (72 + 26)
#define RGB66CC99    (72 + 27)        
#define RGB66CCCC    (72 + 28) 
#define RGB66CCFF    (72 + 29)//-----
#define RGB66FF00    (72 + 30)
#define RGB66FF33    (72 + 31)        
#define RGB66FF66    (72 + 32)
#define RGB66FF99    (72 + 33)        
#define RGB66FFCC    (72 + 34) 
#define RGB66FFFF    (72 + 35)//-----
//==================4组(108-143)=====================
#define RGB990000    (108 + 0)        
#define RGB990033    (108 + 1)
#define RGB990066    (108 + 2)        
#define RGB990099    (108 + 3)
#define RGB9900CC    (108 + 4)        
#define RGB9900FF    (108 + 5)
#define RGB993300    (108 + 6) //-----
#define RGB993333    (108 + 7)        
#define RGB993366    (108 + 8)
#define RGB993399    (108 + 9)        
#define RGB9933CC    (108 + 10) 
#define RGB9933FF    (108 + 11)//-----
#define RGB996600    (108 + 12)
#define RGB996633    (108 + 13)        
#define RGB996666    (108 + 14)
#define RGB996699    (108 + 15)        
#define RGB9966CC    (108 + 16) 
#define RGB9966FF    (108 + 17)//-----
#define RGB999900    (108 + 18)
#define RGB999933    (108 + 19)        
#define RGB999966    (108 + 20)
#define RGB999999    (108 + 21)        
#define RGB9999CC    (108 + 22) 
#define RGB9999FF    (108 + 23)//-----
#define RGB99CC00    (108 + 24)
#define RGB99CC33    (108 + 25)        
#define RGB99CC66    (108 + 26)
#define RGB99CC99    (108 + 27)        
#define RGB99CCCC    (108 + 28) 
#define RGB99CCFF    (108 + 29)//-----
#define RGB99FF00    (108 + 30)
#define RGB99FF33    (108 + 31)        
#define RGB99FF66    (108 + 32)
#define RGB99FF99    (108 + 33)        
#define RGB99FFCC    (108 + 34) 
#define RGB99FFFF    (108 + 35)//-----
//==================5组(144-180)=====================
#define RGBCC0000    (144 + 0)        
#define RGBCC0033    (144 + 1)
#define RGBCC0066    (144 + 2)        
#define RGBCC0099    (144 + 3)
#define RGBCC00CC    (144 + 4)        
#define RGBCC00FF    (144 + 5)
#define RGBCC3300    (144 + 6) //-----
#define RGBCC3333    (144 + 7)        
#define RGBCC3366    (144 + 8)
#define RGBCC3399    (144 + 9)        
#define RGBCC33CC    (144 + 10) 
#define RGBCC33FF    (144 + 11)//-----
#define RGBCC6600    (144 + 12)
#define RGBCC6633    (144 + 13)        
#define RGBCC6666    (144 + 14)
#define RGBCC6699    (144 + 15)        
#define RGBCC66CC    (144 + 16) 
#define RGBCC66FF    (144 + 17)//-----
#define RGBCC9900    (144 + 18)
#define RGBCC9933    (144 + 19)        
#define RGBCC9966    (144 + 20)
#define RGBCC9999    (144 + 21)        
#define RGBCC99CC    (144 + 22) 
#define RGBCC99FF    (144 + 23)//-----
#define RGBCCCC00    (144 + 24)
#define RGBCCCC33    (144 + 25)        
#define RGBCCCC66    (144 + 26)
#define RGBCCCC99    (144 + 27)        
#define RGBCCCCCC    (144 + 28) 
#define RGBCCCCFF    (144 + 29)//-----
#define RGBCCFF00    (144 + 30)
#define RGBCCFF33    (144 + 31)        
#define RGBCCFF66    (144 + 32)
#define RGBCCFF99    (144 + 33)        
#define RGBCCFFCC    (144 + 34) 
#define RGBCCFFFF    (144 + 35)//-----
//==================6组(180-216)=====================
#define RGBFF0000    (180 + 0)        
#define RGBFF0033    (180 + 1)
#define RGBFF0066    (180 + 2)        
#define RGBFF0099    (180 + 3)
#define RGBFF00CC    (180 + 4)        
#define RGBFF00FF    (180 + 5)
#define RGBFF3300    (180 + 6) //-----
#define RGBFF3333    (180 + 7)        
#define RGBFF3366    (180 + 8)
#define RGBFF3399    (180 + 9)        
#define RGBFF33CC    (180 + 10) 
#define RGBFF33FF    (180 + 11)//-----
#define RGBFF6600    (180 + 12)
#define RGBFF6633    (180 + 13)        
#define RGBFF6666    (180 + 14)
#define RGBFF6699    (180 + 15)        
#define RGBFF66CC    (180 + 16) 
#define RGBFF66FF    (180 + 17)//-----
#define RGBFF9900    (180 + 18)
#define RGBFF9933    (180 + 19)        
#define RGBFF9966    (180 + 20)
#define RGBFF9999    (180 + 21)        
#define RGBFF99CC    (180 + 22) 
#define RGBFF99FF    (180 + 23)//-----
#define RGBFFCC00    (180 + 24)
#define RGBFFCC33    (180 + 25)        
#define RGBFFCC66    (180 + 26)
#define RGBFFCC99    (180 + 27)        
#define RGBFFCCCC    (180 + 28) 
#define RGBFFCCFF    (180 + 29)//-----
#define RGBFFFF00    (180 + 30)
#define RGBFFFF33    (180 + 31)        
#define RGBFFFF66    (180 + 32)
#define RGBFFFF99    (180 + 33)        
#define RGBFFFFCC    (180 + 34) 
#define RGBFFFFFF    (180 + 35)//-----


#endif //#define _COLOR_RGB666_H
