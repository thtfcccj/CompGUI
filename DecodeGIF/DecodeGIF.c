/*******************************************************************************

                             GIF格式解码实现
//参考与示例：https://blog.csdn.net/liukaixiansheng/article/details/84793718
********************************************************************************/

#include "DecodeGIF.h"
#include "Adler32.h" //Adler32压缩包校验方式
#include "struct.h" //struct_get()
#include <string.h>

//--------------------------接收数据缓冲out使用声明---------------------------
//3： out->U8Para   用作记录色深bpp
//4： out->U16Para  用作有作记录一行数据量大小

//-----------------------------------解码示例---------------------------------
//abcbabbac压缩后为：a-b-c-b-27-30-c   
//流 out header dict  存放   
//a  a   a                   --->初始化时
//b  b   b    27=ab   a  b  //存放=上次流 | 本次curS(由本次查表获得)
//c  c   c    28=bc   b  c 
//b  b   b    29=cb   c  b 
//27 ab  a    30=ba   b  a 
//30 ba  b    31=abb  27 b
//c  c   c    32=bac  30 c

//上述标题对应_Decode()中
//  流   对应  CurCode
//  out  对应  _Out4Dict()函数
//header 对应  _Out4Dict()函数返回值
//存放[1]对应 pDecode->DictId[]
//存放[2]对应 pDecode->DictChar[]
//上次流 对应 PrvCode, 即out字典对应的ID

//输出字符通过递归可得到

/*******************************************************************************
                           相关函数实现
********************************************************************************/

//-------------------------数据后续处理函数-----------------------------------
//返回用掉了多少数据
static brsize_t _LaterPro(struct  _winWriter *out)
{
  struct _DecodeGIF *pDecode = struct_get(out, struct _DecodeGIF, wOut);
  pDecode->cbOutLine(out);//绘制一行数据
  unsigned short length = out->U16Para; //一行数据占位  
  out->OutedSize += length;
  return length;   //第一行数据被绘制了
}

//--------------------------通过字典输出字符---------------------------------
//返回<0异常，否则返回找到输出字符的首个字符
static unsigned short _Out4Dict(struct _DecodeGIF *pDecode,
                              unsigned short DictId, //要查找表的字
                              unsigned short DictLen)//当前表长度
{

  winWriter_t *out = &pDecode->wOut;
  unsigned short IdStart = ((unsigned short)1 << pDecode->wOut.U8Para);
  
  //直接数，不需查表
  if(DictId < IdStart){
    *(out->data + out->start++) = DictId; //输出前缀
    //满了先回调输出
    if((out->capability - out->start) < out->U16Para) 
      winWriter_OutData(out);
    return DictId;
  }
  
  //需递归查表,为提高率，每次找到的最低字符也缓存
  unsigned short Deep = DECODER_STRING_SIZE - 1;
  do{
    //反向依次压入最后一个字符
    pDecode->AntiChar[Deep--] = pDecode->DictChar[DictId];
    if(Deep == 0) return -1;//字符太长，缓冲不够了
    DictId = pDecode->DictId[DictId];
    if(DictId >= DictLen) return -1;//字典出错
    //找到直接数(至少两个字符),结束了
    if(DictId < IdStart){
      pDecode->AntiChar[Deep] = DictId;
      break;
    }
  }while(1);
  //将反向缓冲的字符输出
  winWriter_Copy(out, &pDecode->AntiChar[Deep], 
                 DECODER_STRING_SIZE - Deep, out->U16Para);
  return DictId;
}

//-----------------------------译码实现-----------------------------------
//暂仅支持索引色,逐行扫，默认压缩方式与自动滤波器格式
static signed char _Decode(struct _DecodeGIF *pDecode)
{
  bReader_t *reader = &pDecode->Reader;
  winWriter_t *out = &pDecode->wOut;
  
  //初始化字典
  unsigned char BitSpace = out->U8Para; //现在的数据占位
  unsigned short clrCode = (unsigned short)1 << BitSpace;//清除标记=原始数据长度+1
  unsigned short endCode = clrCode + 1;//结束标记为清除标记+1
  unsigned short DictInPos = endCode + 1;//字典插入位置初始化为结束标记+1

  BitSpace++;//已经比原数据多出一位了。

  signed char Bit = 0;
  unsigned short PrvCode = 0xffff; 
  unsigned short CurCode;   
  unsigned short CurS;
  do{
    //缓冲比特流
    Bit -= BitSpace; //下次起始位置
    if(Bit < BitSpace){//不够就缓冲
      if(reader->bitsize <= reader->bp) //读完了(没结束字符是否异常?)
        return -1;
      bReader_2BufferB32(reader); //最多读32位
      Bit = 32;// - (reader->bp & 0x07);
    }
    //读值
    CurCode = bReader_RdB(reader, BitSpace); 
    //特殊字符处理:
    if(CurCode == endCode) break;//结束了
    if(CurCode == clrCode){//清字典
      DictInPos = endCode + 1;//字典插入位置初始化为结束标记+1
      PrvCode = 0xffff;  //重新开始
      continue;
    }

    if(PrvCode != 0xffff){//非首次时
      pDecode->DictId[DictInPos] = PrvCode;
      pDecode->DictChar[DictInPos] = CurS;//预建立
      DictInPos++; //扩容了
      if(DictInPos >= DECODER_DICT_SIZE) return -1;//超过容量了
      //位可能的扩流
      if(DictInPos >= ((unsigned short)1 << BitSpace)) BitSpace++;
    }
    //输出结果
    if(CurCode < DictInPos){ 
      CurS = _Out4Dict(pDecode, CurCode, DictInPos);
      if(CurS >= 256) return -2;//未找到字符，数据异常
    }
    else return -1;//字典未建立，数据异常
    pDecode->DictChar[DictInPos - 1] = CurS; //正式建立
    
    //后缀做前缀
    PrvCode = CurCode;

  }while(1);
  
  return 0;
}

//-----------------------------默认OutLine-----------------------------------
static void _cbOutLine(const struct  _winWriter *out){(void)out;}

//-----------------------------GIF译码器实现-----------------------------------
//暂仅支持索引色,逐行扫，默认压缩方式与自动滤波器格式
signed char DecodeGIF(struct _DecodeGIF *pDecode,//无需初始化
                        DecodeGIF_cbOutLine_t cbOutLine,//回调输出函数
                        unsigned char bbp,//色深,1,2,4,8索引色
                        unsigned short w, //图像宽度
                        unsigned short h,  //图像高度
                        const unsigned char *idat, //gif图像压缩数据
                        unsigned long idatLen)    //gif图像数据长度
{
  //if(pDecode == NULL) return -1;//异常
  //校验数据正确性
  if(*idat++ != bbp) return -1;//数据异常
  idatLen--;  
  
  if(cbOutLine == NULL) pDecode->cbOutLine = _cbOutLine;//防止异常
  else pDecode->cbOutLine = cbOutLine;  

  //得到每行实际占用字节数
  unsigned short Line = w;//((w  >> 3) * bbp); //整数部分占位
  //Line += ((w & 0x07) * bbp + 7) >> 3; //末尾像素未对齐部分占1字节
  
  //初始化winWriter
  winWriter_t *out = &pDecode->wOut;
  winWriter_Clr(out);
  out->LaterPro = _LaterPro; //窗口处理函数
  
  out->U8Para = bbp;  //用作记录色深bpp
  out->U16Para = Line; //有作记录一行大小
  out->data = pDecode->OutData;
  out->capability = DECODER_GIF_DATA_BUF_SIZE;
  out->MaxOutSize = Line * h;

  //初始化reader
  bReader_t *reader = &pDecode->Reader;
  bReader_Init(reader, idat, idatLen);
  
  //解压缩数据
  signed char Resume = _Decode(pDecode);
  if(Resume) return Resume;//异常
  
  //未完成图像数据输出
  while(out->start){
    if(winWriter_OutData(out) < 0) return -1;//输出异常
  }
  
  return 0;
}
