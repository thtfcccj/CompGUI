/*******************************************************************************

          ePic��ʽͼ����ģ��-����BMPͼ���ѹ������ͼʵ��

********************************************************************************/

#include "ePic.h"

#include "Plot.h"
#include "Plot_cbHw.h"//�ײ��������
#include <string.h>

//-----------------------------��PNGͼ-----------------------------------
//����ȡģw*h��λ,�˺�����������ɫ��
//�ݽ�֧��8B����
signed char ePic_PlotPNG(u16 x,u16 y)
{
  //�����Կռ任ʱ��
  unsigned short w = ePicBuf.Header.w;
  unsigned short h = ePicBuf.Header.h;  
  unsigned char bitSize = ePicBuf.Header.DeepInfo;//ɫ���֧��1��2��4��8ֵ
  unsigned char mapSize =  ePicBuf.Header.PaletteCount;
  const unsigned char *map = ePicBuf.pNextData;
  const unsigned char *data = map + sizeof(Color_t) * mapSize;
  
  //����ͷ�� ѹ����Ϣ�̶�Ϊ0x78, 0xda,����λΪѹ�����
  unsigned char Data = *data++; //ȡλ����
  //RFC-1950 CMF: 0-3b:CM=8��ʾdeflateѹ������, 4~7CINFOΪ7��ʾ���32K����
  if(Data != 0x78) return -1;//��֧�ֵĸ�ʽ
  Data = *data++; //ȡλ����
  //FLG��: 0~4b: FCHECK:��CMF��FLG��У��λ,b5:��Ԥ���ֵ�  b6-7ѹ���ȼ�
  //���b5=1��Ԥ���ֵ䣬ѹ���ȼ����L3
  if(Data != 0xDA) return -1;//��֧�ֵĸ�ʽ
  //��b5=1��Ԥ���ֵ䣬���DICTID������4�ֽ����
  
  Data = *data++; //ȡλѹ�����

  //δ�꣬������Ҫ���ӣ�
  
  
  
  return 0;
}
