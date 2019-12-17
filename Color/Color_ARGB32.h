/*******************************************************************************

              彩色显示屏时,标准颜色-在使用ARGB8*8*8*8色时的标准颜色定义
//参考https://wenku.baidu.com/view/d9e7d417c5da50e2524d7fbe.html方案，但ID不同
********************************************************************************/

#ifndef _COLOR_ARGB32_H
#define _COLOR_ARGB32_H

//----------------------------8*8*8*8标准标准颜色索引值定义---------------------
typedef unsigned long Color_t; //定义颜色类型占位
#define COLOR_COUNT    0xffffffff //支持的颜色数量

//----------------------------6*6*6标准标准颜色索引值转换-----------------------
//==================1组(1-36=====================
#define RGB000000    0x00000000        
#define RGB000033    0x00000033
#define RGB000066    0x00000066        
#define RGB000099    0x00000099
#define RGB0000CC    0x000000CC        
#define RGB0000FF    0x000000FF
#define RGB003300    0x00003300//-----
#define RGB003333    0x00003333        
#define RGB003366    0x00003366
#define RGB003399    0x00003399        
#define RGB0033CC    0x000033CC 
#define RGB0033FF    0x000033FF//-----
#define RGB006600    0x00006600
#define RGB006633    0x00006633        
#define RGB006666    0x00006666
#define RGB006699    0x00006699        
#define RGB0066CC    0x000066CC 
#define RGB0066FF    0x000066FF//-----
#define RGB009900    0x00009900
#define RGB009933    0x00009933        
#define RGB009966    0x00009966
#define RGB009999    0x00009999        
#define RGB0099CC    0x000099CC 
#define RGB0099FF    0x000099FF//-----
#define RGB00CC00    0x0000CC00
#define RGB00CC33    0x0000CC33        
#define RGB00CC66    0x0000CC66
#define RGB00CC99    0x0000CC99        
#define RGB00CCCC    0x0000CCCC 
#define RGB00CCFF    0x0000CCFF//-----
#define RGB00FF00    0x0000FF00
#define RGB00FF33    0x0000FF33        
#define RGB00FF66    0x0000FF66
#define RGB00FF99    0x0000FF99       
#define RGB00FFCC    0x0000FFCC 
#define RGB00FFFF    0x0000FFFF//-----
//==================2组(36-72=====================
#define RGB330000    0x00330000        
#define RGB330033    0x00330033
#define RGB330066    0x00330066        
#define RGB330099    0x00330099
#define RGB3300CC    0x003300CC        
#define RGB3300FF    0x003300FF
#define RGB333300    0x00333300 //-----
#define RGB333333    0x00333333//0x003308        
#define RGB333366    0x00333366
#define RGB333399    0x00333399        
#define RGB3333CC    0x003333CC 
#define RGB3333FF    0x003333FF//-----
#define RGB336600    0x00336600
#define RGB336633    0x00336633        
#define RGB336666    0x00336666
#define RGB336699    0x00336699       
#define RGB3366CC    0x003366CC 
#define RGB3366FF    0x003366FF//-----
#define RGB339900    0x00339900
#define RGB339933    0x00339933        
#define RGB339966    0x00339966
#define RGB339999    0x00339999        
#define RGB3399CC    0x003399CC 
#define RGB3399FF    0x003399FF//-----
#define RGB33CC00    0x0033CC00
#define RGB33CC33    0x0033CC33        
#define RGB33CC66    0x0033CC66
#define RGB33CC99    0x0033CC99        
#define RGB33CCCC    0x0033CCCC 
#define RGB33CCFF    0x0033CCFF//-----
#define RGB33FF00    0x0033FF00
#define RGB33FF33    0x0033FF33        
#define RGB33FF66    0x0033FF66
#define RGB33FF99    0x0033FF99        
#define RGB33FFCC    0x0033FFCC 
#define RGB33FFFF    0x0033FFFF//-----
//==================3组(73-108=====================
#define RGB660000    0x00660000        
#define RGB660033    0x00660033
#define RGB660066    0x00660066        
#define RGB660099    0x00660099
#define RGB6600CC    0x006600CC        
#define RGB6600FF    0x006600FF
#define RGB663300    0x00663300 //-----
#define RGB663333    0x00663333        
#define RGB663366    0x00663366
#define RGB663399    0x00663399        
#define RGB6633CC    0x006633CC 
#define RGB6633FF    0x006633FF//-----
#define RGB666600    0x00666600
#define RGB666633    0x00666633        
#define RGB666666    0x00666666
#define RGB666699    0x00666699        
#define RGB6666CC    0x006666CC 
#define RGB6666FF    0x006666FF//-----
#define RGB669900    0x00669900
#define RGB669933    0x00669933        
#define RGB669966    0x00669966
#define RGB669999    0x00669999        
#define RGB6699CC    0x006699CC 
#define RGB6699FF    0x006699FF//-----
#define RGB66CC00    0x0066CC00
#define RGB66CC33    0x0066CC33        
#define RGB66CC66    0x0066CC66
#define RGB66CC99    0x0066CC99        
#define RGB66CCCC    0x0066CCCC 
#define RGB66CCFF    0x0066CCFF//-----
#define RGB66FF00    0x0066FF00
#define RGB66FF33    0x0066FF33        
#define RGB66FF66    0x0066FF66
#define RGB66FF99    0x0066FF99        
#define RGB66FFCC    0x0066FFCC 
#define RGB66FFFF    0x0066FFFF//-----
//==================4组(109-144=====================
#define RGB990000    0x00990000        
#define RGB990033    0x00990033
#define RGB990066    0x00990066        
#define RGB990099    0x00990099
#define RGB9900CC    0x009900CC        
#define RGB9900FF    0x009900FF
#define RGB993300    0x00993300 //-----
#define RGB993333    0x00993333        
#define RGB993366    0x00993366
#define RGB993399    0x00993399        
#define RGB9933CC    0x009933CC 
#define RGB9933FF    0x009933FF//-----
#define RGB996600    0x00996600
#define RGB996633    0x00996633        
#define RGB996666    0x00996666
#define RGB996699    0x00996699        
#define RGB9966CC    0x009966CC 
#define RGB9966FF    0x009966FF//-----
#define RGB999900    0x00999900
#define RGB999933    0x00999933        
#define RGB999966    0x00999966
#define RGB999999    0x00999999        
#define RGB9999CC    0x009999CC 
#define RGB9999FF    0x009999FF//-----
#define RGB99CC00    0x0099CC00
#define RGB99CC33    0x0099CC33        
#define RGB99CC66    0x0099CC66
#define RGB99CC99    0x0099CC99        
#define RGB99CCCC    0x0099CCCC 
#define RGB99CCFF    0x0099CCFF//-----
#define RGB99FF00    0x0099FF00
#define RGB99FF33    0x0099FF33        
#define RGB99FF66    0x0099FF66
#define RGB99FF99    0x0099FF99        
#define RGB99FFCC    0x0099FFCC 
#define RGB99FFFF    0x0099FFFF//-----
//==================5组(144-180=====================
#define RGBCC0000    0x00cc0000        
#define RGBCC0033    0x00cc0033
#define RGBCC0066    0x00cc0066        
#define RGBCC0099    0x00cc0099
#define RGBCC00CC    0x00cc00CC        
#define RGBCC00FF    0x00cc00FF
#define RGBCC3300    0x00cc3300 //-----
#define RGBCC3333    0x00cc3333        
#define RGBCC3366    0x00cc3366
#define RGBCC3399    0x00cc3399        
#define RGBCC33CC    0x00cc33CC 
#define RGBCC33FF    0x00cc33FF//-----
#define RGBCC6600    0x00cc6600
#define RGBCC6633    0x00cc6633        
#define RGBCC6666    0x00cc6666
#define RGBCC6699    0x00cc6699        
#define RGBCC66CC    0x00cc66CC 
#define RGBCC66FF    0x00cc66FF//-----
#define RGBCC9900    0x00cc9900
#define RGBCC9933    0x00cc9933        
#define RGBCC9966    0x00cc9966
#define RGBCC9999    0x00cc9999        
#define RGBCC99CC    0x00cc99CC
#define RGBCC99FF    0x00cc99FF//-----
#define RGBCCCC00    0x00ccCC00
#define RGBCCCC33    0x00ccCC33        
#define RGBCCCC66    0x00ccCC66
#define RGBCCCC99    0x00ccCC99        
#define RGBCCCCCC    0x00ccCCCC 
#define RGBCCCCFF    0x00ccCCFF//-----
#define RGBCCFF00    0x00ccFF00
#define RGBCCFF33    0x00ccFF33        
#define RGBCCFF66    0x00ccFF66
#define RGBCCFF99    0x00ccFF99        
#define RGBCCFFCC    0x00ccFFCC 
#define RGBCCFFFF    0x00ccFFFF//-----
//==================6组(181-216=====================
#define RGBFF0000    0x00ff0000        
#define RGBFF0033    0x00ff0033
#define RGBFF0066    0x00ff0066        
#define RGBFF0099    0x00ff0099
#define RGBFF00CC    0x00ff00CC        
#define RGBFF00FF    0x00ff00FF
#define RGBFF3300    0x00ff3300 //-----
#define RGBFF3333    0x00ff3333       
#define RGBFF3366    0x00ff3366
#define RGBFF3399    0x00ff3399        
#define RGBFF33CC    0x00ff33CC 
#define RGBFF33FF    0x00ff33FF//-----
#define RGBFF6600    0x00ff6600
#define RGBFF6633    0x00ff6633        
#define RGBFF6666    0x00ff6666
#define RGBFF6699    0x00ff6699        
#define RGBFF66CC    0x00ff66CC 
#define RGBFF66FF    0x00ff66FF//-----
#define RGBFF9900    0x00ff9900
#define RGBFF9933    0x00ff9933       
#define RGBFF9966    0x00ff9966
#define RGBFF9999    0x00ff9999        
#define RGBFF99CC    0x00ff99CC 
#define RGBFF99FF    0x00ff99FF//-----
#define RGBFFCC00    0x00ffCC00
#define RGBFFCC33    0x00ffCC33        
#define RGBFFCC66    0x00ffCC66
#define RGBFFCC99    0x00ffCC99        
#define RGBFFCCCC    0x00ffCCCC 
#define RGBFFCCFF    0x00ffCCFF//-----
#define RGBFFFF00    0x00ffFF00
#define RGBFFFF33    0x00ffFF33        
#define RGBFFFF66    0x00ffFF66
#define RGBFFFF99    0x00ffFF99        
#define RGBFFFFCC    0x00ffFFCC 
#define RGBFFFFFF    0x00ffFFFF//-----


#endif //#define _COLOR_RGB666_H
