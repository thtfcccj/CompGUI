/***********************************************************************

                  字符串处理扩展模块

***********************************************************************/
#ifndef __STRING_EX_H
#define	__STRING_EX_H

/******************************************************************************
		                            相关函数
******************************************************************************/

//-----------------------转换为字符串-显示为最简函数----------------------
//返回结束位置,最大支持10^8数显示(超过显示99999999)
char *Value4StringMin(unsigned long Value,
                      char *pString,//接收缓冲
                      unsigned char Min);//保证的最小位数

//-----------------------转换为字符串-显示为最简函数----------------------
//返回结束位置
char *Value2StringMin(signed short Value,
                      char *pString,//接收缓冲
                      unsigned char Min);//保证的最小位数

//-----------------------转换为字符串-固定位显示函数----------------------
//<0填充0, 超限固定显示显示位数个9, 返回结束位置
char *Value2StringFix(unsigned short Value,
                      char *pString,//接收缓冲
                      unsigned char Len);//显示长度

//-----------------带标志数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置
char *pNum2StringFlag(signed short Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //数值显示最短长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag);

//-------------------------------字符复制函数-------------------------------
//此函数替换strcpy(),用于返回的是字符结束位置的指针
char *strcpyL(char *pStr, const char *pSub);

//-------------------------------内存复制函数-------------------------------
//此函数替换memcpy(),用于返回的是结束位置的指针
char *memcpyL(char *pStr, const char *pSub, unsigned short Len);
        
//-----------------------字符转小写函数-------------------------------
char CharToLower(char Str);

//-------------------------------字符查找函数-------------------------------
//此函数忽略大小写替换strstr(),返回的是查找字符结束位置的指针,没找到时为NULL
char *StrFind(char *pStr, const char *pSub);

//-------------------------------字符替换函数-------------------------------
//此函数忽略大小写,替换成功返回非0，否则返回0。
signed char StringReplace(char *pStr, const char *pFrom, const char *pTo);

//---------------将字符串里所有":"前大写字母转换为小写字母函数--------------------
//void StrToSmallEndColon(char *pStr);

//----------------------------去掉字符串前的空格字符------------------------
//返回有效的字符，此函数未检查字符串结束
char *StrRemoveSpace(char *pStr);

//-------------------------------得到数字字符长度函数---------------------------
unsigned char StrGetAscNumLen(const char *pStr);

//-----------------------------提指定字符长度函数---------------------------
unsigned char StrGetCharLen(const char *pStr, char c);


//----------------------------ASC数字转换为Bcd数字函数--------------------------
//从最高位开始，若长度若为奇数，则首位Bcd码填充0
//调用此函数需确保长度范围内为asc数字
void AscNumToBcd(const char *pStr, 
                 unsigned char Len,                
                 unsigned char *pBcd);

//----------------------------Bcd数字转换为ASC数字函数--------------------------
//返回字符结束位置
char *BcdNumToAsc(const unsigned char *pBcd,
                 unsigned char Len,                
                 char *pStr);

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

/******************************************************************************
		                            相关回调函数
******************************************************************************/

//----------------------------得到字符串替换需要的缓冲区------------------------
//调用StringReplace()时需实现，注意系统可能出现的最大字符串大小;
char *StringEx_pcbGetReplaceBuf(void);

#endif




