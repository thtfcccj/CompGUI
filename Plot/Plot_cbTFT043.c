/*******************************************************************************
              ��ͼ���׼�ӿ�-�ײ���������ӿ�-�ڳ���4.3����ʾ���ϵ�ʵ��
ͨ���˽ӿ�ʹPlotģ������ڶȲ���ʾ��Ӳ��ʵ��
********************************************************************************/

#include <string.h>
#include "Plot_cbHw.h"
#include "TFTDbi.h"
#include "TFTCmd_043.h"


/*******************************************************************************
                              �ⲿ�Դ�ʵ��
********************************************************************************/

//---------------------------���Զ�λĳ��λ����������-------------------------
//���ﷵ����ʾ��ַλ���Ա��´�ʹ��
//->���������Դ������Χ����Ԥ��д�Դ������һ��д�Դ�,�ɷ���NULLָ��
Color_t * Plot_cbAbsLocalArea(unsigned short x, unsigned short y, 
                              unsigned short w, unsigned short h)
{
  //����y�᷶Χ
  TftDbi_WrCmd(TFT_CMD_WR_CASET);
  TftDbi_WrDataS(y);
  TftDbi_WrDataS(h); 
  //����x����Χ
  TftDbi_WrCmd(TFT_CMD_WR_PASET);
  TftDbi_WrDataS(x);
  TftDbi_WrDataS(w); 
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





