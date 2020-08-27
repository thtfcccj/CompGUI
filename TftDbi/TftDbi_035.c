/*******************************************************************************

                     TFT DBI����ͳһ�ӿ�-035��ʾ����غ���ʵ��
�˽ӿ������ʹ����ʾ���������޹�
********************************************************************************/
#include "TftDbi.h"
#include "TftCmd_035.h"

/*******************************************************************************
                                  ��غ���-��ͨ����ʵ��
********************************************************************************/
//---------------------------------��ʼ������-----------------------------------
//���ظ��豸������
signed char TftDbi_Init(void)
{
  TftDbi.Timer = 0;
  TftDbi_HwInit();
  
  //�����ʶ����Ϣ,��4������: 1��Ч 2����ID 3�����汾 4����ID
  unsigned char Id = TftDbi_RdDataCmd(TFT_CMD_RD_DIDIF);
  unsigned short Info = TftDbi_RdDataS();

  if((Id == 0) || (Id == 0xff)) return -1; //�豸����
  if((Info == 0) || (Info == 0xffff)) return -2; //�豸��Ϣ����
  
  //035��ʾ����ʼ��
  //�ɲο�: https://wenku.baidu.com/view/dd4f6885f46527d3250ce091.html  
  TftDbi_WrCmd(TFT_CMD_WR_DISPON); //������ʾ
  TftDbi_WrCmd(TFT_CMD_WR_ALLPON); //ȫ����ʾ����
  TftDbi_WrCmd(TFT_CMD_WR_ALLPOFF); //ȫ����ʾ����   

  return 0;
}

//---------------------------------������-----------------------------------
//��16ms������ɨ��
void TftDbi_Task(void)
{
  if(!TftDbi.Timer) return;
  TftDbi.Timer--;
  if(TftDbi.Timer) return;   
  //�ڴ��дʱ��ǿ�Ʒ��ָ��
  if((TftDbi.LastCmd == TFT_CMD_RD_RAM) || (TftDbi.LastCmd == TFT_CMD_WR_RAM)) { 
    TftDbi_WrCmd(TFT_CMD_WR_NOP);
    TftDbi_CsDis();
  }
}


