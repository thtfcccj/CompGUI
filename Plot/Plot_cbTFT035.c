/*******************************************************************************
          ��ͼ���׼�ӿ�-�ײ���������ӿ�-�ڳ���3.5������GRAM��ʾ���ϵ�ʵ��
ͨ���˽ӿ�ʹPlotģ������ڶȲ���ʾ��Ӳ��ʵ��
********************************************************************************/

#include <string.h>
#include "Plot_cbHw.h"
#include "TFTDbi.h"
#include "TFTCmd_035.h"

/*******************************************************************************
                              �ⲿ�Դ�ʵ��
********************************************************************************/

//---------------------------���Զ�λĳ��λ����������-------------------------
//���ﷵ����ʾ��ַλ���Ա��´�ʹ��
//->���������Դ������Χ����Ԥ��д�Դ������һ��д�Դ�,�ɷ���NULLָ��
Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
                              unsigned short w, unsigned short h)
{
  //����x�᷶Χ
  TftDbi_WrCmd(TFT_CMD_WR_CASET);
  TftDbi_WrDataS(x);
  TftDbi_WrDataS(x+w-1); 
  //TftDbi_CsDis();  
  //����y�᷶Χ
  TftDbi_WrCmd(TFT_CMD_WR_PASET);
  TftDbi_WrDataS(y);
  TftDbi_WrDataS(y+h-1); 
  //Ԥ��Ϊ����д״̬׼��д������   
  TftDbi_WrCmd(TFT_CMD_WR_RAM);  
  return (Color_t *)NULL;
}

//----------------------------��������һλ��--------------------------------
//���˵��Ե���һ���ָ�д�Դ�����
void Plot_cbUpdateNextFun(void)
{
  //��д��ʾָ��Ὣλ������x,y�Ὺʼ,ʵ�ⲻ����
  //���̶�дָ��ʵ���д������أ���Ҳ�����У��ݷ�ʵ�ִ˹���
  
  //�Ҵ˻����ڱ���ǰ���ı�ʱ���ϴ�ǰ���ᱻ��Ϊ�Ǳ������±�������ˢ�£�
  //�ʾ�����ʵ��Ҳֻ��Ӧ���ڹ̶��л��޳���,����ÿ����ˢ�±���ʱ��ʹ��
  
  //TftDbi_WrCmd(TFT_CMD_RD_MEM_CONT);       //���Դ�
  //Color_t Color =  TftDbi_RdColor();  //��1�����Դ���һλ
  //TftDbi_WrCmd(TFT_CMD_WR_MEM_CONT);//Ԥ��Ϊ����д״̬׼��д������
}

//---------------------------�����Զ�λĳ��λ����������------------------------
//�ж���ɫ����ʱ��ʵ��
Color_t * Plot_cbAbsLocalAreaRd(unsigned short x, unsigned short y, 
                                unsigned short w, unsigned short h)
{
  //����x�᷶Χ
  TftDbi_WrCmd(TFT_CMD_WR_CASET);
  TftDbi_WrDataS(x);
  TftDbi_WrDataS(x+w-1); 
  //TftDbi_CsDis();  
  //����y�᷶Χ
  TftDbi_WrCmd(TFT_CMD_WR_PASET);
  TftDbi_WrDataS(y);
  TftDbi_WrDataS(y+h-1); 
  
  //Ԥ��Ϊ������״̬׼����������   
  TftDbi_WrCmd(TFT_CMD_RD_RAM); 
  TftDbi_RdData(); //�׸���Ч
  return (Color_t *)NULL;
}


                              


