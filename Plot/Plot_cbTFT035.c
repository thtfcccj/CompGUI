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
  TftDbi_CsDis();  
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
  TftDbi_WrCmd(TFT_CMD_RD_RAM);       //���Դ�
  Color_t Color =  TftDbi_RdColor();  //��1�����Դ���һλ
  TftDbi_WrCmd(TFT_CMD_WR_RAM);//Ԥ��Ϊ����д״̬׼��д������
}




