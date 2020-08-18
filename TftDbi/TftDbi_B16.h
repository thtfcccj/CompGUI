/*******************************************************************************

           TFT MCU�ӿ�����-ʹ��TypeB 16λ����ʱʵ��-˽�нӿ��ļ�

*******************************************************************************/


/*******************************************************************************
                               �ײ�ص�
*******************************************************************************/
#include "IoCtrl.h"

//����IO(ȫ������Ϊ�����Ч��ƽ������ʱ��)
#define TftDbi_cbIoCfg()     do{CfgLcdIo();}while(0)

//Reset�ź�(�͵�ƽ��λ)
#define TftDbi_cbStartRst()   do{ClrLcdRst(); }while(0)    //�͵�ƽ��ʼ��λ
#define TftDbi_cbEndRst()     do{SetLcdRst(); }while(0)    //�ߵ�ƽ����

//CS�ź�
#define TftDbi_cbCsEn()     do{ClrLcdCs();}while(0)       //Ƭѡ����(�͵�ƽ)
#define TftDbi_cbCsDis()    do{SetLcdCs();}while(0)       //Ƭѡ��ֹ(�ߵ�ƽ)

//RS�ź�
#define TftDbi_cbRsCmd()    do{ClrLcdRs();}while(0)       //����ģʽ(�͵�ƽ)
#define TftDbi_cbRsData()   do{SetLcdRs();}while(0)      //����ģʽ(�ߵ�ƽ)

//дʱ��(�½������ݣ�����������)
#define TftDbi_cbWrClkH()    do{SetLcdWr();}while(0)     //дʱ�Ӹ�
#define TftDbi_cbWrClkL()    do{ClrLcdWr();}while(0)     //дʱ�ӵ�

//��ʱ��(�½������ݣ�����������)
#define TftDbi_cbRdClkH()    do{SetLcdRd();}while(0)     //��ʱ�Ӹ�
#define TftDbi_cbRdClkL()    do{ClrLcdRd();}while(0)     //��ʱ�ӵ�

//������,16bit
#define TftDbi_cbDbOut()     do{OutLcdDB();}while(0)    //�����
#define TftDbi_cbDbWr(data)  do{WrLcdDB(data);}while(0) //д����
#define TftDbi_cbDbIn()      do{InLcdDB();}while(0)     //������
#define TftDbi_cbDbRd()      RdLcdDB()                   //������

/*******************************************************************************
                              ʱ����ƻص�
*******************************************************************************/

#define TftDbi_cbGetUs_HwRst()   (10)            //Ӳ����λʱ�� ILI9488>=10us
#define TftDbi_cbGetMs_HwRstWait() (5)           //Ӳ����λ��ȴ� ILI9488>=5ms

#define TftDbi_cbDelayTchw()   do{}while(0)     //Ƭѡȡ��ʱ��,ILI9488>=0ns 
#define TftDbi_cbDelayTcs()   do{}while(0)     //Ƭѡ����ʱ��,ILI9488>=15,��id45,��FM355

#define TftDbi_cbDelayTwrl()   do{}while(0)     //д�͵�ƽ����ʱ��,ILI9488>=15ns 
#define TftDbi_cbDelayTwrh()   do{}while(0)      //д�ߵ�ƽ����ʱ��,ILI9488>=15ns

#define TftDbi_cbDelayTrdl()   do{}while(0)     //���͵�ƽ����ʱ��,ILI9488>=45~160ns 
#define TftDbi_cbDelayTrdh()   do{}while(0)     //���ߵ�ƽ����ʱ��,ILI9488>=90~450ns
                              


