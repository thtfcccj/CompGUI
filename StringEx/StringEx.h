/***********************************************************************

                  字符串处理扩展模块

***********************************************************************/
#ifndef __STRING_EX_H
#define	__STRING_EX_H

/******************************************************************************
		                            相关函数
******************************************************************************/

//------------------------------得到减正值结果----------------------------------
//Cur >= Count将返回0
unsigned char GetPosEqualU8(unsigned char Cur,
                             unsigned char Count);

//---------------------1字节转换为字符串-显示为最简函数----------------------
//返回结束位置(此位置强制填充结束字符)
char *Value1StringMin(unsigned char Value,
                      char *pString);//接收缓冲

//-----------------------转换为字符串-显示为最简函数----------------------
//返回结束位置,最大显示10位
char *Value4StringMin(unsigned long Value,
                      char *pString,//接收缓冲
                      unsigned char Min);//保证的最小位数

//-----------------------转换为字符串-显示为最简函数----------------------
//返回结束位置(此位置强制填充结束字符)
char *Value2StringMin(unsigned short Value,
                      char *pString,//接收缓冲
                      unsigned char Min);//保证的最小位数

//-----------------------转换为字符串-固定位显示函数----------------------
//返回结束位置
char *Value2StringFix(unsigned short Value,//<0填充0, 超限固定显示显示位数个9,
                      char *pString,//接收缓冲
                      signed char Len);//显示长度, <0时末尾不强制填充结束字符

//-----------------带标志数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置(此位置强制填充结束字符)
char *pNum2StringFlag(signed short Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //数值显示最短长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag);

//-----------------------十进制无符号整数字符转换为整数---------------------
unsigned short DecStr2Us(const char *pDecStr);

//-------------------------填充十进制8位无符号整数---------------------------
//仅支持1~3位：“0”到“255”字符的转换，需其它字符自动结束。
//返回正: 已用字符串长度，0或负字符中的错误位置
signed char FullDecMem2Uc(const char *pDecStr, unsigned char Len,
                            unsigned char *pData); //填充位置,仅支持1字节

//-------------------------------字符复制函数-------------------------------
//此函数替换strcpy(),用于返回的是字符结束位置的指针
char *strcpyL(char *pStr, const char *pSub);

//--------------------------字符复制函数,从右往左copy--------------------------
//此函数替换strcpy(),返回pStr
char *strcpyR(char *pStr, const char *pSub);

//-----------------------------从右往左复制函数-------------------------------
void memcpyR(void *pDest, const void *pSource, unsigned short Len);

//-------------------------------内存复制函数-------------------------------
//此函数替换memcpy(),用于返回的是结束位置的指针
char *memcpyL(char *pStr, const char *pSub, unsigned short Len);

//-----------------------字符转小写函数-------------------------------
char CharToLower(char Str);

//-------------------------------字符查找函数-------------------------------
//此函数忽略大小写替换strstr(),
//返回的是查找字符  "结束位置指针",没找到时为NULL
const char *StrFind(const char *pStr, const char *pSub);

//-------------------------------字符替换函数-------------------------------
//此函数忽略大小写,替换成功返回非0，否则返回0。
signed char StringReplace(char *pStr, const char *pFrom, const char *pTo);

//----------------------------得到空格后的符串函数------------------------
//返回有效的字符，此函数未检查字符串结束
const char *pGetStrSpaceEnd(const char *pStr);

//-----------------------------删除字符串中的某个特定字符-------------------
void StrClrCar(char *pStr, char C);

//-------------------------得到数字字符长度函数--------------------
unsigned char StrGetAscNumLen(const char *pStr);

//----------------------------由字符串得到数值函数---------------------------
//此函数与模块无关，可供外部独立使用
//返回:0:首字不为数字，-1: 数值无效,-2: 数值超限,其它：解了多少字符串
signed char String2Num(const char *pString,
                        unsigned short Max,  //最大值
                        unsigned short *pData); //返回正确时接收的数

//-----------------------------提指定字符长度函数---------------------------
unsigned char StrGetCharLen(const char *pStr, char c);

//--------------------------字符串是否为ASC函数--------------------------
//返回非0是，否则返回0
signed char StrIsAsc(const char *pStr);

//---------------------------扩展字符串cpy函数---------------------------
//给出最高长度值，cpy返回不能超过此值,自动填充结束字符,返回结束字符
char *strcpyEx(char *pNewStr, const char *pOrgStr,
               unsigned char MaxLen);

//给出最高长度值，cpy返回不能超过此值,不填充结束字符,返回下个字符串位置
const char *strcpyEx2(char *pNewStr, const char *pOrgStr,
                       unsigned char MaxLen);

//---------------------------扩展长度检查函数---------------------------
//给出最高值，返回不能超过此值
unsigned char strlenEx(const char *pString,unsigned char MaxLen);

//-------------------------标准以对齐填充字符串函数-------------------------
//输入最大长度!=输出长度时调用， 多出的填充空格,返回前部填充空格数量
unsigned char strFullMax(char *pDest, const char *pSource,
                         unsigned char SourceMaxLen,//源字符最大长度
                         unsigned char DestLen, //目标字符长度
                         unsigned char Align);//0:左对齐,1局中对齐，2右对齐

//-------------------------以对齐填充字符串函数---------------------------
//多出的填充空格,返回前部填充空格数量
unsigned char strFull(char *pDest, const char *pSource,
                       unsigned char DestLen, //目标字符长度,即源数据最大长度
                       unsigned char Align);//0:左对齐,1局中对齐，2右对齐

//-----------------------字符串转换为IP4函数------------------------------------
//pStr字样需为:192.168.88.152,填充时高位在前,返回负：没有填充位置
signed char StrToIp4(const char *pStr, unsigned char *pIp4);

//-----------------------IP4转换为字符串函数------------------------------------
//pStr字样返回为:192.168.88.2,高位在前,返回字符结束位置
char *Ip4ToStr(const unsigned char *pIp4, char *pStr);

//-----------------------从字符串开头查找指令ID------------------------------
//形参pSize输入时为ppCmdAry阵列大小，若找到了，将返回CmdId
//如:ppCmdAry= "Insert", "Del"
//如：pStr = "Insert data",找到了，则*pSize = 0,返回"Insert"后的指针
//如：pStr = "xxx data"找不到，则返回NULL
const char *pGetCmdIdFromStr(const char *pStr,
                              const char *const* ppCmdAry,
                              unsigned char *pSize);

#endif




