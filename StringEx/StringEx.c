/***********************************************************************

                  字符串处理扩展模块
 * 注：strcpy在此函数内调用时,在xc8 1.37中编译器中会链接不过
***********************************************************************/

#include "StringEx.h"
#include <string.h>

//------------------------------得到减正值结果----------------------------------
//Cur >= Count将返回0
unsigned char GetPosEqualU8(unsigned char Cur,
                            unsigned char Count)
{
  if(Cur >= Count) return 0;
  return Count - Cur;
}

//---------------------1字节转换为字符串-显示为最简函数----------------------
//返回结束位置(此位置强制填充结束字符)
char *Value1StringMin(unsigned char Value,
                      char *pString)//接收缓冲
{
  if(Value >= 100){//有3位
    *pString++ = (Value / 100) + '0';
    Value %= 100;
    *pString++ = (Value / 10) + '0';
    Value %= 10;    
  } 
  else if(Value >= 10){//有两位
    *pString++ = (Value / 10) + '0';
    Value %= 10;
  }
  *pString++ = Value + '0';//个位
  *pString = '\0'; //强制填充结束字符
  return pString;
}

//-----------------------转换为字符串-显示为最简函数----------------------
//返回结束位置,最大显示10位
char *Value4StringMin(unsigned long Value,
                      char *pString,//接收缓冲
                      unsigned char Min)//保证的最小位数
{
  //因Value2StringMin显示不全(9999共4位)，这里分段组合
  //9~10位
  if((Value > 99999999) || (Min > 8)){
    unsigned char CurMin;
    if(Min <= 8) CurMin = 0;
    else CurMin = Min - 8;
    pString = Value2StringMin(Value / 100000000, pString, CurMin);
    Value %= 100000000;
    Min = 8;
  }
  
  if((Value >= 10000) || (Min > 4)){//高位
    unsigned char CurMin;
    if(Min <= 4) CurMin = 0;
    else CurMin = Min - 4;
    pString = Value2StringMin(Value / 10000, pString, CurMin);
    Min = 4;
  }
  return Value2StringMin(Value % 10000, pString, Min);//低位
}

//-----------------------转换为字符串-显示为最简函数----------------------
//返回结束位置(此位置强制填充结束字符)
char *Value2StringMin(unsigned short Value,
                      char *pString,//接收缓冲
                      unsigned char Min)//保证的最小位数
{
  /*/短整型时
  //#if(TSIGNED_SIZE >= 2)
    if((Value >= 10000) || (Min >= 5)){
      *pString++ = Value / 10000 + '0';
      Value %= 10000;
	    Min = 10;
    }
    if((Value >= 1000) || (Min >= 4)){
      *pString++ = Value / 1000 + '0';
      Value %= 1000;
	    Min = 10;
    }
  //#endif

  if((Value >= 100) || (Min >= 3)){
    *pString++ = Value / 100 + '0';
    Value %= 100;
	  Min = 10;
  }
  if((Value >= 10) || (Min >= 2)){
    *pString++ = Value / 10 + '0';
    Value %= 10;
	  Min = 10;
  }
  if((Value > 0) || (Min >= 1)){
    *pString++ = Value + '0';
  }*/
  
  //时间换空间方案
  unsigned short Muti = 10000;
  for(unsigned char Pos = 5; Pos > 0; Pos--){
    if((Value >= Muti) || (Min >= Pos)){
      *pString++ = (Value / Muti) + '0';
      Value %= Muti;
	    Min = 10;
    }
    Muti /= 10;
  }

  *pString = '\0'; //强制填充结束字符
  return pString;
}

static const unsigned short _Ov[] = {
  0, 9, 99, 999, 9999,
};

//-----------------------转换为字符串-固定位显示函数----------------------
//返回结束位置
char *Value2StringFix(unsigned short Value,//<0填充0, 超限固定显示显示位数个9,
                      char *pString,//接收缓冲
                      signed char Len)//显示长度, <0时末尾不强制填充结束字符
{
  unsigned short ValidLen = 0 ;
  if(Len < 0) ValidLen = 0 - Len;
  else ValidLen = Len;
  //超限显示
 if(Value > _Ov[ValidLen]) Value = _Ov[ValidLen];
 
  pString += ValidLen; //结束位置
  for(unsigned char Pos = 1; Pos <= ValidLen; Pos++){
    *(pString - Pos) = '0' + (Value % 10);
    Value /= 10;
   }
  if(Len >= 0) *pString = '\0'; //强制填充结束字符
   return pString;
}

//-----------------带标志数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置(此位置强制填充结束字符)
char *pNum2StringFlag(signed short Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //数值显示最短长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag)
{
  unsigned char FullLen;

  if(Flag & 0x80){//带符号时
    if(Value < 0) *pBuf = '-';
    else *pBuf = '+';
    pBuf++;
  }
  if(Value < 0) Value = 0 - Value;  //正值显示
      
  Flag &= 0x07;//小数点了
  
  //有小数点且最小位数不够时
  if(Flag && (Len <= Flag)){
    Len = Flag;//最小位数强制扩展
    if(Value > _Ov[Flag]) Len++; //1以上时,需扩展1位
  }
  
  //带小数点时,先检查数值是否<1,若是,则检查前面补了多少个0.或0.0  
  if(Flag >= Len){ //<0了,0.
    FullLen = Flag - Len;
    *pBuf++ = '0';
    *pBuf++ = '.';
    while(FullLen--) *pBuf++ = '0';//填充无效的0
    pBuf = Value2StringMin(Value,pBuf,Len);
  }
  else{
    //>1了,检查小数点位置,当前位置在小数点左还是右边
    pBuf = Value2StringMin(Value,pBuf,Len);//先填充数据
    if(Flag){ //有小数点时填充小数点位置
      FullLen = Flag;//暂存
      for(; Flag > 0; Flag--) {*pBuf = *(pBuf - 1); pBuf--;}
      *pBuf= '.';
      pBuf += FullLen + 1;//含小数点
    }
  }
  *pBuf = '\0'; //强制填充结束字符
  return pBuf;
}

//-----------------------十进制无符号整数字符转换为整数------------------------
unsigned short DecStr2Us(const char *pDecStr)
{
  unsigned short Us = 0;
  for(unsigned char Pos = 0; Pos < 5; Pos++){
    char Dec = *pDecStr++;
    if((Dec <  '0') || (Dec >  '9')) break;
    Us += Dec - '0';
    Us *= 10;
  }
  return Us;
}

//-------------------------填充十进制8位无符号整数---------------------------
//仅支持1~3位：“0”到“255”字符的转换，需其它字符自动结束。
//返回正: 返回正: 已用字符串长度，0或负字符中的错误位置
signed char FullDecMem2Uc(const char *pDecStr, unsigned char Len,
                            unsigned char *pData) //填充位置,仅支持1字节
{
  if(Len == 0) return -1;//空字符,首字错误
  unsigned char Pos = 0;
  unsigned short Data = 0;
  for(; Pos < Len; Pos++){
    char c = *pDecStr++;
    if((c < '0') || (c > '9')){//字错误
      if(Pos == 0) return -1;//首字错误
      else break; //数据获取完成
    }
    if(Pos >= 3) return -4; //数据超过3位了
    Data = Data * 10 + (c - '0');//取出一位了
  }
  //数据正确了
  if(Data > 255) return -3; //数据超过255了
  *pData = Data; //正确了
  return Pos; //结束位置
}

//-------------------------------字符复制函数-------------------------------
//此函数替换strcpy(),用于返回的是字符结束位置的指针
char *strcpyL(char *pStr, const char *pSub)
{
  do{
    char c = *pSub;
    *pStr = c;
    if(c == 0) break;//结束'\0'.注意结束字符一起copy
    pStr++; pSub++;
  }while(1);
  return pStr;
}

//--------------------------字符复制函数,从右往左copy--------------------------
//此函数替换strcpy(),返回pStr
char *strcpyR(char *pStr, const char *pSub)
{
  unsigned short Len = strlen(pSub) + 1; //含结束字符
  const char *pEndSub = pSub + Len;
  pStr += Len;
  do{
    *pStr = *pEndSub;
    pStr--; pEndSub--;
  }while(pEndSub >= pSub);
  return pStr;
}

//-----------------------------从右往左复制函数-------------------------------
void memcpyR(void *pDest, const void *pSource, unsigned short Len)
{
  //if(Len) return;
  //直接实现用于替换MPLABX 中 memcpy()部分出错问题
  unsigned char *d = (unsigned char *)pDest + (Len - 1); 
  unsigned char *s = (unsigned char *)pSource + (Len - 1);   
  for(; Len > 0; Len--){*d-- = *s--;}
}

//-------------------------------内存复制函数-------------------------------
//此函数替换memcpy(),用于返回的是结束位置的指针
char *memcpyL(char *pStr, const char *pSub, unsigned short Len)
{
  //memcpy(pStr, pSub, Len); //直接用
  //return pStr + Len;  
  
  //直接实现用于替换MPLABX 中 memcpy()部分出错问题
  for(; Len > 0; Len--){  *pStr++ = *pSub++;}
  return pStr;
}

//-----------------------字符转小写函数-------------------------------
char CharToLower(char Str)
{
  if((Str >= 'A') && (Str <= 'Z'))
    return Str - 'A' + 'a';
  return Str;
}

//-------------------------------字符查找函数-------------------------------
//此函数忽略大小写替换strstr(),用于返回的是查找字符结束位置的指针,没找到时为NULL
const char *StrFind(const char *pStr, const char *pSub)
{
  const char *pOrgSub = pSub;
  const char *pOrgStr;
  do{
    //先查找首字母
    char Firt = CharToLower(*pSub);
    do{
      char Str = CharToLower(*pStr++);
      if(Str == '\0') return NULL; //找完了，没找到
      if(Str == Firt) break; //找到首个了
    }while(1);
    
    //一个一个比较后续字母(为提高效率，未调用strcmp)
    pOrgStr = pStr; //需从此位置重新开始找
    pSub++;
    do{
      char Next = CharToLower(*pSub++);
      if(Next == '\0') return pStr; //全部匹配,查找结束
      char Str = CharToLower(*pStr++);
      if(Str == '\0') return NULL; //找完了，没找到
      if(Str != Next){//不匹配, 没找到，继续重头开始找
        pSub = pOrgSub;
        pStr = pOrgStr;
        break; 
      }
    }while(1);
  }while(1);
  return NULL; //防止部分编译器出错(有的加上全警告)
}  

//-------------------------------字符替换函数-------------------------------
//此函数忽略大小写,替换成功返回非0，否则返回0。
signed char StringReplace(char *pStr, const char *pFrom, const char *pTo) 
{
  //先查找字符串
  char *pReplaceEndPos = (char*)StrFind(pStr, pFrom); //此为结束位置
  if(pReplaceEndPos == NULL) return -1; //未找到
  unsigned short FromLen = strlen(pFrom);
  unsigned short ToLen = strlen(pTo);
  if(FromLen > ToLen)//替换后的值往前移动至结束位置
    strcpyL(pReplaceEndPos - (FromLen - ToLen), pReplaceEndPos);
  else if(FromLen < ToLen) //替换后的值往后移动至结束位置(注意应从右向左复制防止覆盖)
    strcpyR(pReplaceEndPos + (ToLen - FromLen), pReplaceEndPos); 
  //else 相等，不用移动
  memcpy(pReplaceEndPos - FromLen, pTo, ToLen); //替换字符串
  return 0; //替换成功
}

//------------------得到空格后的符串函数------------------------
//返回有效的字符，此函数未检查字符串结束
const char *pGetStrSpaceEnd(const char *pStr)
{
  while(*pStr == ' ') pStr++;
  return pStr;
}

//-------------------------清除字符串中的某个特定字符--------------------
void StrClrCar(char *pStr, char C)
{
  char *pDest = pStr;//复制目标
  do{
    char cur = *pStr++;
    if(cur == '\0') break; //结束了
    if(cur != C){//有效字符了
      *pDest++ = cur;
    }
  }while(1);
  *pDest = '\0'; //填充结束字符
}

//-------------------------------得到数字字符长度函数---------------------------
unsigned char StrGetAscNumLen(const char *pStr)
{
  unsigned char Len = 0;
  char Num = *pStr++;
  while((Num >= '0') && (Num <= '9')){
    Len++;
    Num = *pStr++;
  }
  return Len;
}

//----------------------------由字符串得到数值函数---------------------------
//此函数与模块无关，可供外部独立使用
//返回:0:首字不为数字，-1: 数值无效,-2: 数值超限,其它：解了多少字符串
signed char String2Num(const char *pString,
                        unsigned short Max,  //最大值
                        unsigned short *pData) //返回正确时接收的数
{
  const char *pOrgPos = pString;
  unsigned int ID = 0;
  int Valid = 0;  //数值有效标志
  do{
    unsigned char Num = *pString;
    if(Num < '0') break;
    if(Num > '9') break;
    Num -= '0';
    if(!Valid){//找到数,数值有效了
      if(Num == 0) Valid = -1; //忽略前面的0,倍率无效
      else{
        Valid = 1;
        ID = Num;
      }
    }
    else if(Valid == -1){//前面全是0时
      if(Num != 0){
        ID = Num;
        Valid = 1;
      }
    }
    else{
      ID *= 10;  //加倍了
      ID += Num;
    }
    pString++;//下个字符
  }while(ID <= Max);
  
  if(!Valid) return -1;    //数值无效
  if(ID > Max) return -2;    //数值超限
  *pData = ID;
  return pString - pOrgPos;
}



//-----------------------------得到指定字符长度函数---------------------------
unsigned char StrGetCharLen(const char *pStr, char c)
{
  unsigned char Len = 0;
  do{
    char Cur = *pStr++;
    if(Cur == '\0') break;
    if(Cur == c) Len++;
  }while(1);
  return Len;
}

//--------------------------字符串是否为ASC函数--------------------------
//返回非0是，否则返回0
signed char StrIsAsc(const char *pStr)
{
  do{
    char Str = *pStr;
    if(Str == '\0') break;
    else if(Str >= 0x80) return 0; //不是了
    pStr++;
  }while(1);
  
  return 1; //是了
}

//---------------------------扩展字符串cpy函数---------------------------
//给出最高长度值，cpy返回不能超过此值,自动填充结束字符,返回结束字符
char *strcpyEx(char *pNewStr, const char *pOrgStr,
               unsigned char MaxLen)
{
  for(; MaxLen > 0; MaxLen--, pNewStr++, pOrgStr++){
    if(*pOrgStr == '\0') break;//提前结束
    *pNewStr = *pOrgStr;
  }
  //找完了
  *pNewStr = '\0';
  return pNewStr;
}

//给出最高长度值，cpy返回不能超过此值,不填充结束字符,返回下个字符串位置
const char *strcpyEx2(char *pNewStr, const char *pOrgStr,
                       unsigned char MaxLen)
{
  char c;
  for(; MaxLen > 0; MaxLen--, pNewStr++, pOrgStr++){
    c = *pOrgStr;
    *pNewStr = c; //结束字符一起
    if(c == '\0'){//提前结束
      return pOrgStr + 1; //返回下个起始位置
    }
  }
  //超限或达到最大长度了
  //*pNewStr = '\0'; //copy时的结束字符->为防止内存溢出不填充结束字符
  //找到结束位置，下个即为起点
  for(MaxLen = 10; MaxLen > 0; MaxLen--, pOrgStr++){
    if(*pOrgStr == '\0'){//找到了
      break;
    }
  }
  return pOrgStr + 1; //返回下个起始位置
}

//---------------------------扩展长度检查函数---------------------------
//给出最高值，返回不能超过此值
unsigned char strlenEx(const char *pString,unsigned char MaxLen)
{
  for(unsigned char Len = 0; Len < MaxLen; Len++,pString++){
    if(*pString == '\0') return Len;//提前结束
  }
  //找完了
  return MaxLen;
}

//-------------------------标准以对齐填充字符串函数-------------------------
//输入最大长度!=输出长度时调用， 多出的填充空格,返回前部填充空格数量
unsigned char strFullMax(char *pDest, const char *pSource,
                         unsigned char SourceMaxLen,//源字符最大长度
                         unsigned char DestLen, //目标字符长度
                         unsigned char Align)//0:左对齐,1局中对齐，2右对齐
{
  unsigned char Len = strlenEx(pSource, SourceMaxLen);
  *(pDest + DestLen) = '\0';//先填充结束字符
  if(DestLen <= Len){//异常截断处理
    memcpy(pDest, pSource, Len);
    return 0;
  }
  memset(pDest, ' ', DestLen); //先填充目标空格  
  //Align复用作起始位置
  if(Align == 1) Align = (DestLen - Len) >> 1;
  else if(Align == 2) Align = (DestLen - Len);
  else Align = 0;//左对齐
  memcpy(pDest + Align, pSource, Len);
  return Align;
}

//-------------------------以对齐填充字符串函数---------------------------
//多出的填充空格,返回前部填充空格数量
unsigned char strFull(char *pDest, const char *pSource,
                       unsigned char MaxLen, //目标字符长度,即源数据最大长度
                       unsigned char Align)//0:左对齐,1局中对齐，2右对齐
{
  return strFullMax(pDest, pSource, MaxLen, MaxLen, Align);
}

//-----------------------字符串转换为IP4函数------------------------------------
//pStr字样需为:192.168.88.152,填充时高位在前,返回负：没有填充位置
signed char StrToIp4(const char *pStr, unsigned char *pIp4)
{
  unsigned char Pos = 0;
  unsigned char Data = 0;  
  do{
    char Char = *pStr++;
    if(Char == '.'){//下一组了
      pIp4[Pos] = Data;
      Data = 0;
      Pos++;
      if(Pos >= 4) return 0;//正常结束
    }
    else if((Char < '0') || (Char > '9')){//异常或结束了
      pIp4[Pos] = Data;
      if(Pos == 3) return 0;//正常结束
      return -Pos; //异常
    }
    else{
      Data *= 10;
      Data += (Char - '0');
    }
  }while(1);
  return -1;
}

//-----------------------IP4转换为字符串函数------------------------------------
//pStr字样返回为:192.168.88.2,高位在前,返回字符结束位置
char *Ip4ToStr(const unsigned char *pIp4, char *pStr)
{
  unsigned char Pos = 0;
  do{
    pStr = Value2StringMin(*pIp4++, pStr, 1);
    if(Pos >= 3) break;
    *pStr++ = '.';
    Pos++;
  }while(1);
  return pStr;
}

//-----------------------从字符串开头查找指令ID------------------------------
//形参pSize输入时为ppCmdAry阵列大小，若找到了，将返回CmdId
//如:ppCmdAry= "Insert", "Del"
//如：pStr = "Insert data",找到了，则*pSize = 0,返回"Insert"后的指针
//如：pStr = "xxx data"找不到，则返回NULL
const char *pGetCmdIdFromStr(const char *pStr,
                              const char *const* ppCmdAry,
                              unsigned char *pSize)
{
  char CmdCount = *pSize;
  for(unsigned char CmdId = 0; CmdId < CmdCount; CmdId++){
    const char *pCmd = ppCmdAry[CmdId];
    unsigned char Len = strlen(pCmd);
    if((strncmp(pCmd, pStr, Len) != 0)) continue; //不匹配
    *pSize = CmdId;//填充位置
    return pStr + Len; //找到了,至数据位置
  }
  return NULL;//未找到退出
}





