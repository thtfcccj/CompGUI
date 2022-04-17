/*******************************************************************************

                             GIF��ʽ����ʵ��
//�ο���ʾ����https://blog.csdn.net/liukaixiansheng/article/details/84793718
********************************************************************************/

#include "DecodeGIF.h"
#include "Adler32.h" //Adler32ѹ����У�鷽ʽ
#include "struct.h" //struct_get()
#include <string.h>

//--------------------------�������ݻ���outʹ������---------------------------
//3�� out->U8Para   ������¼ɫ��bpp
//4�� out->U16Para  ����������¼һ����������С

//-----------------------------------����ʾ��---------------------------------
//abcbabbacѹ����Ϊ��a-b-c-b-27-30-c   
//�� out header dict  ���   
//a  a   a                   --->��ʼ��ʱ
//b  b   b    27=ab   a  b  //���=�ϴ��� | ����curS(�ɱ��β����)
//c  c   c    28=bc   b  c 
//b  b   b    29=cb   c  b 
//27 ab  a    30=ba   b  a 
//30 ba  b    31=abb  27 b
//c  c   c    32=bac  30 c

//���������Ӧ_Decode()��
//  ��   ��Ӧ  CurCode
//  out  ��Ӧ  _Out4Dict()����
//header ��Ӧ  _Out4Dict()��������ֵ
//���[1]��Ӧ pDecode->DictId[]
//���[2]��Ӧ pDecode->DictChar[]
//�ϴ��� ��Ӧ PrvCode, ��out�ֵ��Ӧ��ID

//����ַ�ͨ���ݹ�ɵõ�

/*******************************************************************************
                           ��غ���ʵ��
********************************************************************************/

//-------------------------���ݺ���������-----------------------------------
//�����õ��˶�������
static brsize_t _LaterPro(struct  _winWriter *out)
{
  struct _DecodeGIF *pDecode = struct_get(out, struct _DecodeGIF, wOut);
  pDecode->cbOutLine(out);//����һ������
  unsigned short length = out->U16Para; //һ������ռλ  
  out->OutedSize += length;
  return length;   //��һ�����ݱ�������
}

//--------------------------ͨ���ֵ�����ַ�---------------------------------
//����<0�쳣�����򷵻��ҵ�����ַ����׸��ַ�
static unsigned short _Out4Dict(struct _DecodeGIF *pDecode,
                              unsigned short DictId, //Ҫ���ұ����
                              unsigned short DictLen)//��ǰ����
{

  winWriter_t *out = &pDecode->wOut;
  unsigned short IdStart = ((unsigned short)1 << pDecode->wOut.U8Para);
  
  //ֱ������������
  if(DictId < IdStart){
    *(out->data + out->start++) = DictId; //���ǰ׺
    //�����Ȼص����
    if((out->capability - out->start) < out->U16Para) 
      winWriter_OutData(out);
    return DictId;
  }
  
  //��ݹ���,Ϊ����ʣ�ÿ���ҵ�������ַ�Ҳ����
  unsigned short Deep = DECODER_STRING_SIZE - 1;
  do{
    //��������ѹ�����һ���ַ�
    pDecode->AntiChar[Deep--] = pDecode->DictChar[DictId];
    if(Deep == 0) return -1;//�ַ�̫�������岻����
    DictId = pDecode->DictId[DictId];
    if(DictId >= DictLen) return -1;//�ֵ����
    //�ҵ�ֱ����(���������ַ�),������
    if(DictId < IdStart){
      pDecode->AntiChar[Deep] = DictId;
      break;
    }
  }while(1);
  //�����򻺳���ַ����
  winWriter_Copy(out, &pDecode->AntiChar[Deep], 
                 DECODER_STRING_SIZE - Deep, out->U16Para);
  return DictId;
}

//-----------------------------����ʵ��-----------------------------------
//�ݽ�֧������ɫ,����ɨ��Ĭ��ѹ����ʽ���Զ��˲�����ʽ
static signed char _Decode(struct _DecodeGIF *pDecode)
{
  bReader_t *reader = &pDecode->Reader;
  winWriter_t *out = &pDecode->wOut;
  
  //��ʼ���ֵ�
  unsigned char BitSpace = out->U8Para; //���ڵ�����ռλ
  unsigned short clrCode = (unsigned short)1 << BitSpace;//������=ԭʼ���ݳ���+1
  unsigned short endCode = clrCode + 1;//�������Ϊ������+1
  unsigned short DictInPos = endCode + 1;//�ֵ����λ�ó�ʼ��Ϊ�������+1

  BitSpace++;//�Ѿ���ԭ���ݶ��һλ�ˡ�

  signed char Bit = 0;
  unsigned short PrvCode = 0xffff; 
  unsigned short CurCode;   
  unsigned short CurS;
  do{
    //���������
    Bit -= BitSpace; //�´���ʼλ��
    if(Bit < BitSpace){//�����ͻ���
      if(reader->bitsize <= reader->bp) //������(û�����ַ��Ƿ��쳣?)
        return -1;
      bReader_2BufferB32(reader); //����32λ
      Bit = 32;// - (reader->bp & 0x07);
    }
    //��ֵ
    CurCode = bReader_RdB(reader, BitSpace); 
    //�����ַ�����:
    if(CurCode == endCode) break;//������
    if(CurCode == clrCode){//���ֵ�
      DictInPos = endCode + 1;//�ֵ����λ�ó�ʼ��Ϊ�������+1
      PrvCode = 0xffff;  //���¿�ʼ
      continue;
    }

    if(PrvCode != 0xffff){//���״�ʱ
      pDecode->DictId[DictInPos] = PrvCode;
      pDecode->DictChar[DictInPos] = CurS;//Ԥ����
      DictInPos++; //������
      if(DictInPos >= DECODER_DICT_SIZE) return -1;//����������
      //λ���ܵ�����
      if(DictInPos >= ((unsigned short)1 << BitSpace)) BitSpace++;
    }
    //������
    if(CurCode < DictInPos){ 
      CurS = _Out4Dict(pDecode, CurCode, DictInPos);
      if(CurS >= 256) return -2;//δ�ҵ��ַ��������쳣
    }
    else return -1;//�ֵ�δ�����������쳣
    pDecode->DictChar[DictInPos - 1] = CurS; //��ʽ����
    
    //��׺��ǰ׺
    PrvCode = CurCode;

  }while(1);
  
  return 0;
}

//-----------------------------Ĭ��OutLine-----------------------------------
static void _cbOutLine(const struct  _winWriter *out){(void)out;}

//-----------------------------GIF������ʵ��-----------------------------------
//�ݽ�֧������ɫ,����ɨ��Ĭ��ѹ����ʽ���Զ��˲�����ʽ
signed char DecodeGIF(struct _DecodeGIF *pDecode,//�����ʼ��
                        DecodeGIF_cbOutLine_t cbOutLine,//�ص��������
                        unsigned char bbp,//ɫ��,1,2,4,8����ɫ
                        unsigned short w, //ͼ����
                        unsigned short h,  //ͼ��߶�
                        const unsigned char *idat, //gifͼ��ѹ������
                        unsigned long idatLen)    //gifͼ�����ݳ���
{
  //if(pDecode == NULL) return -1;//�쳣
  //У��������ȷ��
  if(*idat++ != bbp) return -1;//�����쳣
  idatLen--;  
  
  if(cbOutLine == NULL) pDecode->cbOutLine = _cbOutLine;//��ֹ�쳣
  else pDecode->cbOutLine = cbOutLine;  

  //�õ�ÿ��ʵ��ռ���ֽ���
  unsigned short Line = w;//((w  >> 3) * bbp); //��������ռλ
  //Line += ((w & 0x07) * bbp + 7) >> 3; //ĩβ����δ���벿��ռ1�ֽ�
  
  //��ʼ��winWriter
  winWriter_t *out = &pDecode->wOut;
  winWriter_Clr(out);
  out->LaterPro = _LaterPro; //���ڴ�����
  
  out->U8Para = bbp;  //������¼ɫ��bpp
  out->U16Para = Line; //������¼һ�д�С
  out->data = pDecode->OutData;
  out->capability = DECODER_GIF_DATA_BUF_SIZE;
  out->MaxOutSize = Line * h;

  //��ʼ��reader
  bReader_t *reader = &pDecode->Reader;
  bReader_Init(reader, idat, idatLen);
  
  //��ѹ������
  signed char Resume = _Decode(pDecode);
  if(Resume) return Resume;//�쳣
  
  //δ���ͼ���������
  while(out->start){
    if(winWriter_OutData(out) < 0) return -1;//����쳣
  }
  
  return 0;
}
