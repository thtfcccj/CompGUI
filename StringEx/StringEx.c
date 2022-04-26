/***********************************************************************

                  �ַ���������չģ��
 * ע��strcpy�ڴ˺����ڵ���ʱ,��xc8 1.37�б������л����Ӳ���
***********************************************************************/

#include "StringEx.h"
#include <string.h>

//------------------------------�õ�����ֵ���----------------------------------
//Cur >= Count������0
unsigned char GetPosEqualU8(unsigned char Cur,
                            unsigned char Count)
{
  if(Cur >= Count) return 0;
  return Count - Cur;
}

//-----------------------ת��Ϊ�ַ���-��ʾΪ�����----------------------
//���ؽ���λ��,���֧��10^8����ʾ(������ʾ99999999)
char *Value4StringMin(unsigned long Value,
                      char *pString,//���ջ���
                      unsigned char Min)//��֤����Сλ��
{
  if((Value >= 10000) || (Min >= 5)){//��λ
    if(Value > 99999999) Value = 99999999;
    unsigned char CurMin;
    if(Min <= 4) CurMin = 0;
    else CurMin = Min - 4;
    pString = Value2StringMin(Value / 10000, pString, CurMin);
    Min = 4;
  }
  return Value2StringMin(Value % 10000, pString, Min);//��λ
}

//-----------------------ת��Ϊ�ַ���-��ʾΪ�����----------------------
//���ؽ���λ��(��λ��ǿ���������ַ�)
char *Value2StringMin(signed short Value,
                      char *pString,//���ջ���
                      unsigned char Min)//��֤����Сλ��
{
  //������ʱ
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
  }
  
  *pString = '\0'; //ǿ���������ַ�
  return pString;
}

static const unsigned short _Ov[] = {
  0, 9, 99, 999, 9999,
};

//-----------------------ת��Ϊ�ַ���-�̶�λ��ʾ����----------------------
//���ؽ���λ��
char *Value2StringFix(unsigned short Value,//<0���0, ���޹̶���ʾ��ʾλ����9,
                      char *pString,//���ջ���
                      signed char Len)//��ʾ����, <0ʱĩβ��ǿ���������ַ�
{
  unsigned short ValidLen = 0 ;
  if(Len < 0) ValidLen = 0 - Len;
  else ValidLen = Len;
  //������ʾ
 if(Value > _Ov[ValidLen]) Value = _Ov[ValidLen];
 
  pString += ValidLen; //����λ��
  for(unsigned char Pos = 1; Pos <= ValidLen; Pos++){
    *(pString - Pos) = '0' + (Value % 10);
    Value /= 10;
   }
  if(Len >= 0) *pString = '\0'; //ǿ���������ַ�
   return pString;
}

//-----------------����־��ת��Ϊ�ַ�������------------------
//����������ĩδ�û�����λ��(��λ��ǿ���������ַ�)
char *pNum2StringFlag(signed short Value,   //��ǰ��ֵ
                      char *pBuf,    //���ջ�����
                      unsigned char Len,    //��ֵ��ʾ��̳���
                      //��־,����Ϊ����3bit:С����λ��,0x80:��ʾ������
                      unsigned char Flag)
{
  unsigned char FullLen;

  if(Flag & 0x80){//������ʱ
    if(Value < 0) *pBuf = '-';
    else *pBuf = '+';
    pBuf++;
  }
  if(Value < 0) Value = 0 - Value;  //��ֵ��ʾ
      
  //��С����ʱ,�ȼ����ֵ�Ƿ�<1,����,����ǰ�油�˶��ٸ�0.��0.0
  Flag &= 0x07;
  if(Flag >= Len){ //<0��,0.
    FullLen = Flag - Len;
    *pBuf++ = '0';
    *pBuf++ = '.'; 
    while(FullLen--) *pBuf++ = '0';//�����Ч��0
    pBuf = Value2StringMin(Value,pBuf,Len);
  }
  else{
    //>1��,���С����λ��,��ǰλ����С���������ұ�
    pBuf = Value2StringMin(Value,pBuf,Len);//���������
    if(Flag){ //��С����ʱ���С����λ��
      FullLen = Flag;//�ݴ�
      for(; Flag > 0; Flag--) {*pBuf = *(pBuf - 1); pBuf--;}
      *pBuf= '.';
      pBuf += FullLen + 1;//��С����
    }
  }
  *pBuf = '\0'; //ǿ���������ַ�
  return pBuf;
}

//-----------------------ʮ�����޷��������ַ�ת��Ϊ����------------------------
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

//-------------------------------�ַ����ƺ���-------------------------------
//�˺����滻strcpy(),���ڷ��ص����ַ�����λ�õ�ָ��
char *strcpyL(char *pStr, const char *pSub)
{
  do{
    char c = *pSub;
    *pStr = c;
    if(c == 0) break;//����'\0'.ע������ַ�һ��copy
    pStr++; pSub++;
  }while(1);
  return pStr;
}

//--------------------------�ַ����ƺ���,��������copy--------------------------
//�˺����滻strcpy(),����pStr
char *strcpyR(char *pStr, const char *pSub)
{
  unsigned short Len = strlen(pSub) + 1; //�������ַ�
  const char *pEndSub = pSub + Len;
  pStr += Len;
  do{
    *pStr = *pEndSub;
    pStr--; pEndSub--;
  }while(pEndSub >= pSub);
  return pStr;
}

//-------------------------------�ڴ渴�ƺ���-------------------------------
//�˺����滻memcpy(),���ڷ��ص��ǽ���λ�õ�ָ��
char *memcpyL(char *pStr, const char *pSub, unsigned short Len)
{
  //memcpy(pStr, pSub, Len); //ֱ����
  //return pStr + Len;  
  
  //ֱ��ʵ�������滻MPLABX �� memcpy()���ֳ�������
  for(; Len > 0; Len--){  *pStr++ = *pSub++;}
  return pStr;
}

//-----------------------�ַ�תСд����-------------------------------
char CharToLower(char Str)
{
  if((Str >= 'A') && (Str <= 'Z'))
    return Str - 'A' + 'a';
  return Str;
}

//-------------------------------�ַ����Һ���-------------------------------
//�˺������Դ�Сд�滻strstr(),���ڷ��ص��ǲ����ַ�����λ�õ�ָ��,û�ҵ�ʱΪNULL
const char *StrFind(const char *pStr, const char *pSub)
{
  const char *pOrgSub = pSub;
  const char *pOrgStr;
  do{
    //�Ȳ�������ĸ
    char Firt = CharToLower(*pSub);
    do{
      char Str = CharToLower(*pStr++);
      if(Str == '\0') return NULL; //�����ˣ�û�ҵ�
      if(Str == Firt) break; //�ҵ��׸���
    }while(1);
    
    //һ��һ���ȽϺ�����ĸ(Ϊ���Ч�ʣ�δ����strcmp)
    pOrgStr = pStr; //��Ӵ�λ�����¿�ʼ��
    pSub++;
    do{
      char Next = CharToLower(*pSub++);
      if(Next == '\0') return pStr; //ȫ��ƥ��,���ҽ���
      char Str = CharToLower(*pStr++);
      if(Str == '\0') return NULL; //�����ˣ�û�ҵ�
      if(Str != Next){//��ƥ��, û�ҵ���������ͷ��ʼ��
        pSub = pOrgSub;
        pStr = pOrgStr;
        break; 
      }
    }while(1);
  }while(1);
  return NULL; //��ֹ���ֱ���������(�еļ���ȫ����)
}  

//-------------------------------�ַ��滻����-------------------------------
//�˺������Դ�Сд,�滻�ɹ����ط�0�����򷵻�0��
signed char StringReplace(char *pStr, const char *pFrom, const char *pTo) 
{
  //�Ȳ����ַ���
  char *pReplaceEndPos = (char*)StrFind(pStr, pFrom); //��Ϊ����λ��
  if(pReplaceEndPos == NULL) return -1; //δ�ҵ�
  unsigned short FromLen = strlen(pFrom);
  unsigned short ToLen = strlen(pTo);
  if(FromLen > ToLen)//�滻���ֵ��ǰ�ƶ�������λ��
    strcpyL(pReplaceEndPos - (FromLen - ToLen), pReplaceEndPos);
  else if(FromLen < ToLen) //�滻���ֵ�����ƶ�������λ��(ע��Ӧ���������Ʒ�ֹ����)
    strcpyR(pReplaceEndPos + (ToLen - FromLen), pReplaceEndPos); 
  //else ��ȣ������ƶ�
  memcpy(pReplaceEndPos - FromLen, pTo, ToLen); //�滻�ַ���
  return 0; //�滻�ɹ�
}

/*/---------------���ַ���������":"ǰ��д��ĸת��ΪСд��ĸ����--------------------
void StrToSmallEndColon(char *pStr)
{
  do{
    char Str = *pStr;
    if(Str == '\0') break;
    else if(Str == ':') break; 
    *pStr = CharToLower(Str);
    pStr++;
  }while(1);
}*/

//------------------�õ��ո��ķ�������------------------------
//������Ч���ַ����˺���δ����ַ�������
const char *pGetStrSpaceEnd(const char *pStr)
{
  while(*pStr == ' ') pStr++;
  return pStr;
}

//-------------------------------�õ������ַ����Ⱥ���---------------------------
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

//----------------------------���ַ����õ���ֵ����---------------------------
//�˺�����ģ���޹أ��ɹ��ⲿ����ʹ��
//����:0:���ֲ�Ϊ���֣�-1: ��ֵ��Ч,-2: ��ֵ����,���������˶����ַ���
signed char String2Num(const char *pString,
                        unsigned short Max,  //���ֵ
                        unsigned short *pData) //������ȷʱ���յ���
{
  const char *pOrgPos = pString;
  unsigned int ID = 0;
  int Valid = 0;  //��ֵ��Ч��־
  do{
    unsigned char Num = *pString;
    if(Num < '0') break;
    if(Num > '9') break;
    Num -= '0';
    if(!Valid){//�ҵ���,��ֵ��Ч��
      if(Num == 0) Valid = -1; //����ǰ���0,������Ч
      else{
        Valid = 1;
        ID = Num;
      }
    }
    else if(Valid == -1){//ǰ��ȫ��0ʱ
      if(Num != 0){
        ID = Num;
        Valid = 1;
      }
    }
    else{
      ID *= 10;  //�ӱ���
      ID += Num;
    }
    pString++;//�¸��ַ�
  }while(ID <= Max);
  
  if(!Valid) return -1;    //��ֵ��Ч
  if(ID > Max) return -2;    //��ֵ����
  *pData = ID;
  return pString - pOrgPos;
}



//-----------------------------�õ�ָ���ַ����Ⱥ���---------------------------
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

//--------------------------�ַ����Ƿ�ΪASC����--------------------------
//���ط�0�ǣ����򷵻�0
signed char StrIsAsc(const char *pStr)
{
  do{
    char Str = *pStr;
    if(Str == '\0') break;
    else if(Str >= 0x80) return 0; //������
    pStr++;
  }while(1);
  
  return 1; //����
}

//---------------------------��չ�ַ���cpy����---------------------------
//������߳���ֵ��cpy���ز��ܳ�����ֵ,�Զ��������ַ�,���ؽ����ַ�
char *strcpyEx(char *pNewStr, const char *pOrgStr,
               unsigned char MaxLen)
{
  for(; MaxLen > 0; MaxLen--, pNewStr++, pOrgStr++){
    if(*pOrgStr == '\0') break;//��ǰ����
    *pNewStr = *pOrgStr;
  }
  //������
  *pNewStr = '\0';
  return pNewStr;
}

//������߳���ֵ��cpy���ز��ܳ�����ֵ,���������ַ�,�����¸��ַ���λ��
const char *strcpyEx2(char *pNewStr, const char *pOrgStr,
                       unsigned char MaxLen)
{
  char c;
  for(; MaxLen > 0; MaxLen--, pNewStr++, pOrgStr++){
    c = *pOrgStr;
    *pNewStr = c; //�����ַ�һ��
    if(c == '\0'){//��ǰ����
      return pOrgStr + 1; //�����¸���ʼλ��
    }
  }
  //���޻�ﵽ��󳤶���
  //*pNewStr = '\0'; //copyʱ�Ľ����ַ�->Ϊ��ֹ�ڴ�������������ַ�
  //�ҵ�����λ�ã��¸���Ϊ���
  for(MaxLen = 10; MaxLen > 0; MaxLen--, pOrgStr++){
    if(*pOrgStr == '\0'){//�ҵ���
      break;
    }
  }
  return pOrgStr + 1; //�����¸���ʼλ��
}

//---------------------------��չ���ȼ�麯��---------------------------
//�������ֵ�����ز��ܳ�����ֵ
unsigned char strlenEx(const char *pString,unsigned char MaxLen)
{
  for(unsigned char Len = 0; Len < MaxLen; Len++,pString++){
    if(*pString == '\0') return Len;//��ǰ����
  }
  //������
  return MaxLen;
}

//-------------------------��׼�Զ�������ַ�������-------------------------
//������󳤶�!=�������ʱ���ã� ��������ո�,����ǰ�����ո�����
unsigned char strFullMax(char *pDest, const char *pSource,
                         unsigned char SourceMaxLen,//Դ�ַ���󳤶�
                         unsigned char DestLen, //Ŀ���ַ�����
                         unsigned char Align)//0:�����,1���ж��룬2�Ҷ���
{
  unsigned char Len = strlenEx(pSource, SourceMaxLen);
  if(DestLen <= Len){//�쳣�ضϴ���
    memcpy(pDest, pSource, Len);
    return 0;
  }
  memset(pDest, ' ', DestLen); //�����Ŀ��ո�  
  //Align��������ʼλ��
  if(Align == 1) Align = (DestLen - Len) >> 1;
  else if(Align == 2) Align = (DestLen - Len);
  else Align = 0;//�����
  memcpy(pDest + Align, pSource, Len);
  return Align;
}

//-------------------------�Զ�������ַ�������---------------------------
//��������ո�,����ǰ�����ո�����
unsigned char strFull(char *pDest, const char *pSource,
                       unsigned char MaxLen, //Ŀ���ַ�����,��Դ������󳤶�
                       unsigned char Align)//0:�����,1���ж��룬2�Ҷ���
{
  return strFullMax(pDest, pSource, MaxLen, MaxLen, Align);
}

//-----------------------�ַ���ת��ΪIP4����------------------------------------
//pStr������Ϊ:192.168.88.152,���ʱ��λ��ǰ,���ظ���û�����λ��
signed char StrToIp4(const char *pStr, unsigned char *pIp4)
{
  unsigned char Pos = 0;
  unsigned char Data = 0;  
  do{
    char Char = *pStr++;
    if(Char == '.'){//��һ����
      pIp4[Pos] = Data;
      Data = 0;
      Pos++;
      if(Pos >= 4) return 0;//��������
    }
    else if((Char < '0') || (Char > '9')){//�쳣�������
      pIp4[Pos] = Data;
      if(Pos == 3) return 0;//��������
      return -Pos; //�쳣
    }
    else{
      Data *= 10;
      Data += (Char - '0');
    }
  }while(1);
  return -1;
}

//-----------------------IP4ת��Ϊ�ַ�������------------------------------------
//pStr��������Ϊ:192.168.88.2,��λ��ǰ,�����ַ�����λ��
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



