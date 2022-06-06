/*******************************************************************************

                  TFT3.5����ʾ����������׼��������-����ָ�����
//�˶�����ILI9488,ILI9486,ILI9481,ST7796S,HX8357D����������ͬ
********************************************************************************/
#ifndef TFT_CMD035_H
#define TFT_CMD035_H

//��HX(8357D)֧��ָ��(֧�ֹ������)
//��ILI(9488)֧��ָ��(֧�ֹ��ܴ�֮)
//��ST(7796S)֧��ָ��(֧�ֹ�������)

//�������Ƚ������벿�֣���δ�Ծ������ݶ�����бȽ�!!!!!!!!!!!
//����λ������HX8357DΪ׼

/********************************************************************************
                                 ���ò���ָ��
********************************************************************************/

//����д��ָ��,�޲���,��Ҫ�����ս��ڴ����ݶ�д
#define TFT_CMD_WR_NOP          0x00  
//����д��λָ��,���ڸ�λ���Ĵ���״̬��Ĭ��ֵ
#define TFT_CMD_WR_SWRESET      0x01  //ִ�к����5ms��120ms(˯��ʱ)
//���������ʶ����Ϣ,��4������: 1��Ч 2����ID 3�����汾 4����ID
#define TFT_CMD_RD_DIDIF      0x04
//������������DSIֵ,��2������ 1��Ч 2λ���״ֵ̬
#define TFT_CMD_RD_NUMPE       0x05

//�� RGB I/Fģʽʱ��������ɫλ ��1����
//�� ����
#define TFT_CMD_RD_RED         0x06 
//�� ��
#define TFT_CMD_RD_GREEN       0x07 
//�� ��
#define TFT_CMD_RD_BLUE        0x08
//���嶨��Ϊ:
#define TFT_RGB_IS_MSB       (1 << 5) //5bit��λΪMSB
#define TFT_RGB_IS_LSB       (1 << 1) //1bit��λΪLSB
#define TFT_RGB_IS_16        (1 << 0) //��λΪ18λ����λΪ16λ  

//���Read Display Status (09h) //���ȶԷ��ض���(����)��ȫ��ͬ
#define TFT_CMD_RD_DISP         0x09

//��������ʾ�ĵ�Դģʽ,��2������ 1��Ч 2λ���״ֵ̬:
#define TFT_CMD_RD_DPM         0x0a
//��2λ���״ֵ̬����Ϊ
#define TFT_DPM_BOOSTER_OK (1 << 7) //BOOSTER��Դ��
#define TFT_DPM_IDIE_MODE  (1 << 6) //����ģʽ
#define TFT_DPM_PARTIAL_ON (1 << 5) //������ʾ��
#define TFT_DPM_SLEEP_OUT  (1 << 4) //Sleep Out Mode.
#define TFT_DPM_DISP_NOR   (1 << 3) //������ʾģʽ
#define TFT_DPM_DISP_ON    (1 << 2) //��ʾ��

//��������ʾ�ĵ�Դģʽ,��2������ 1��Ч 2λ���״ֵ̬:
#define TFT_CMD_RD_DMADCTL        0x0b
//��2λ���״ֵ̬����Ϊ
#define TFT_MAD_PAGE_BT    (1 << 7)  //��λʱҳ(������)��������
#define TFT_MAD_COL_RL     (1 << 6)  //��λʱ�������ҵ���
#define TFT_MAD_PC_REVERSE (1 << 5)  //��λʱ���з�ת,�������
#define TFT_MAD_LINE_BT    (1 << 4)  //��λʱˢ�´��ҵ���
#define TFT_MAD_BGR        (1 << 3)  //��λʱBGR���У�����ΪRGB
#define TFT_MAD_LATCH_RL   (1 << 2)  //��λʱ��ʾ������ҵ���

//��������ʾ�����ظ�ʽ,��2������ 1��Ч 2λ���״ֵ̬:
#define TFT_CMD_RD_DCOLMOD        0x0C
//��2λ���״ֵ̬����Ϊ
#define TFT_DCOL_DPI_SHIFT   4  //DPI Interface Pixel format
#define TFT_DCOL_DPI_MASK   (7 << TFT_DCOL_DPI_SHIFT)
#define TFT_DCOL_DBI_SHIFT   0  //DPI Interface Pixel format
#define TFT_DCOL_DBI_MASK   (7 << TFT_DCOL_DBI_SHIFT)
//���嶨��Ϊ:
#define TFT_DCOL_PIXEL_12BIT  3 //12 bit/pixel
#define TFT_DCOL_PIXEL_16BIT  4 //16 bit/pixel
#define TFT_DCOL_PIXEL_18BIT  6 //12 bit/pixel
#define TFT_DCOL_PIXEL_24BIT  7 //16 bit/pixel

//��������ʾ��ͼ���ʽ,��2������ 1��Ч 2λ���״ֵ̬:
#define TFT_CMD_RD_DIM        0x0D
//��2λ���״ֵ̬����Ϊ
#define TFT_DIM_V_SCROLL_ON   (1 << 7)  //��ֱ������
#define TFT_DIM_INVERSION     (1 << 5)  //��ʾ����
//#define TFT_DIM_DISP_ALL      (1 << 4)  //ȫ�ԣ�������->��֧��
//#define TFT_DIM_DISP_NULL     (1 << 3)  //ȫ�أ�������->��֧��
#define TFT_DIM_GAMA_MASK    0X07  //Gamma����У׼ѡ�񣬾���Ϊ��
#define TFT_DIM_GAMA_GC0     0X00  //Gamma ѡGC0
#define TFT_DIM_GAMA_GC1     0X01  //Gamma ѡGC1
#define TFT_DIM_GAMA_GC2     0X02  //Gamma ѡGC2
#define TFT_DIM_GAMA_GC3     0X03  //Gamma ѡGC3

//��������ʾ���źŸ�ʽ,��2������ 1��Ч 2λ���״ֵ̬:
#define TFT_CMD_RD_DSM        0x0E
//��2λ���״ֵ̬����Ϊ
//Tearing MODE ����ʱ�����һ��ɨ��ͬ����Ϣ�Ÿ�MCU���ṩ�����Ϣ��
#define TFT_DSM_TERING_ON      (1 << 7)  //TearingЧ����
#define TFT_DSM_TERING_MODE    (1 << 6)  //Tearingģʽѡ��0��1
#define TFT_DSM_DSI_ERR        (1 << 5)  //

//����������Ͻ��,��2������ 1��Ч 2λ���״ֵ̬:
#define TFT_CMD_RD_DSDR        0x0F
//��2λ���״ֵ̬����Ϊ
//Tearing MODE ����ʱ�����һ��ɨ��ͬ����Ϣ�Ÿ�MCU���ṩ�����Ϣ��
#define TFT_DSR_REG_LOAD      (1 << 7)  //Register Loading Detection
#define TFT_DSR_FUNM           (1 << 7)  //Functionality Detection
#define TFT_DSR_CHECKSUM_ERR   (1 << 5)  //У���һ����

//��������˯��ģʽ�����βΣ�120ms�����
#define TFT_CMD_WR_SLPIN       0x10
//�����˳�˯��ģʽ�����βΣ�5ms���˳�
#define TFT_CMD_WR_SLPOUT      0x11

//���������ֲ���ʾģʽ�����βΣ�
#define TFT_CMD_WR_PTLON       0x12
//��������������ʾģʽ�����β�
#define TFT_CMD_WR_NORON       0x13

//�����رշ��ԣ����β�
#define TFT_CMD_WR_INVOFF     0x20
//�����������ԣ����β�
#define TFT_CMD_WR_INVON      0x21

//���ȫ����ʾ�����β�
#define TFT_CMD_WR_ALLPOFF    0x22
//���ȫ����ʾ�����β�
#define TFT_CMD_WR_ALLPON     0x23

//�� GAMA���� ��ѡ��GAMA��,�βθ�0-3��λ��ֵ,������Ч
#define TFT_CMD_WR_GAMSET     0x26

//�����ر���ʾ�����β�
#define TFT_CMD_WR_DISPOFF    0x28
//����������ʾ�����β�
#define TFT_CMD_WR_DISPON     0x29

//�����ر�Tearing
#define TFT_CMD_WR_TEOFF      0x34
//��������Tearing�����β�
#define TFT_CMD_WR_TEON       0x35

//��������ģʽ�رգ����β�
#define TFT_CMD_WR_IDMOFF      0x38
//��������ģʽ���������β� ����ģʽʱRGB��ɫֻ��8λɫ��
#define TFT_CMD_WR_IDMON      0x39

/********************************************************************************
                                 �ڴ�������ָ��
********************************************************************************/
//���������з�Χ����4��8bit�β�,ǰ2byte�ɸߵ��ͱ�ʾ����ʼ����2byte�ɸߵ��ͱ�ʾ�н���
#define TFT_CMD_WR_CASET      0x2A
//���������з�Χ����4��8bit�β�,ǰ2byte�ɸߵ��ͱ�ʾ����ʼ����2byte�ɸߵ��ͱ�ʾ�н���
#define TFT_CMD_WR_PASET      0x2B
//����дRAM,д��λ��Ϊ��ǰ��������з�Χ��ɵľ�����,�������룬��д��Ч��λ��Ϊ�������ʾλ��
#define TFT_CMD_WR_RAM        0x2C
//������RAM,����λ��Ϊ��ǰ��������з�Χ��ɵľ�����,���������������Ч��λ��Ϊ�������ʾλ��
#define TFT_CMD_RD_RAM        0x2E

//�������þֲ���ʾģʽ����,ǰ2byte�ɸߵ��ͱ�ʾ����ʼ����2byte�ɸߵ��ͱ�ʾ�н���
#define TFT_CMD_WR_PLTAR      0x30

//������ֱ��������,��6byte:
//  ǰ1-2byte�ɸߵ��ͱ�ʾ�����̶�����ǰ3-4byte�ɸߵ��ͱ�ʾ��ֱ������ʼ
//  ǰ5-6byte�ɸߵ��ͱ�ʾ�ײ��̶�����
#define TFT_CMD_WR_VSCRDEF    0x33

//���ڴ���ʿ���,�����ڶ���дʱ���ڴ��������ʽ(�����ڿ�������0��)����1Byte�β�
#define TFT_CMD_WR_MADCTL    0x36
//1Byte�β�λ����Ϊ:
#define TFT_MAC_MY         (1 << 7)//�����з���
#define TFT_MAC_MX         (1 << 6)//�����з���
#define TFT_MAC_MV         (1 << 5)//����ѡ��,��ģ�����л�����������з������
#define TFT_MAC_ML         (1 << 4)//LCD vertical refresh direction control
#define TFT_MAC_BGR        (1 << 3)//RGB-BGR ORDER (BGR)
#define TFT_MAC_SS         (1 << 2)//LCD horizontal refresh direction control

//�д��������ַ,��33h���ʹ�ã�3byte�βΣ�
//ǰ1Byte����������ʽ ��ǰ2~3bit������ʼλ��
#define TFT_CMD_WR_VSCRSADD   0x37

//�����ӿ����ظ�ʽ,��1byte�βΣ�
// DBIʱͬTFT_DCOL_PIXEL����, DPIʱ4/6/7��ʾ16/18/24
#define TFT_CMD_WR_COLMOD      0x3a

//���������ϴε�д�ڴ�
#define TFT_CMD_WR_MEM_CONT    0x3C
//���������ϴεĶ��ڴ�
#define TFT_CMD_RD_MEM_CONT    0x3E

/********************************************************************************
                                 ���ܲ���ָ��
********************************************************************************/

//��������˺��ɨ����, ��3��8bit�β�,�ɸߵ��ͱ�ʾλ��
#define TFT_CMD_WR_MEM_TESL    0x44
//�����������õ�˺��ɨ���ߣ�����2Byte 1��Ч2ͬ��
#define TFT_CMD_RD_MEM_TESL    0x45

//�������ûҶȼ�����8Byte��255������
#define TFT_CMD_WR_DISBV       0x51
//������ȡ���õĻҶȼ�������2Byte 1��Ч2ͬ��
#define TFT_CMD_RD_DISBV       0x52

//����������ʾ����(������LED���������), ��1byte�β�
#define TFT_CMD_WR_CTRLD       0x53
//1byte�β�λ����Ϊ:
#define TFT_CTRLD_BCTRL_ON    (1 << 5) //���ȿ���ģ�鿪��
#define TFT_CTRLD_DD_ON       (1 << 3) //���ȱ䰵
#define TFT_CTRLD_BL_ON       (1 << 2) //���ȿ��ƿ�

//���������õ���ʾ���ȣ�����2Byte 1��Ч2ͬ��
#define TFT_CMD_RD_CTRLD       0x54

//����д��������Ӧ���ȿ���, ��1byte�β�
#define TFT_CMD_WR_CABC       0x55
//1byte�βζ���Ϊ:
#define TFT_CABC_OFF         0  //�ر�
#define TFT_CABC_USER        1  //User Interface Image
#define TFT_CABC_STILL       2  //Still Picture
#define TFT_CABC_IMAGE       3  //Moving Image

//��������������Ӧ���ȿ���,����2Byte 1��Ч2ͬ��
#define TFT_CMD_RD_CABC       0x56
//����д��������Ӧ���ȿ�����С����, ��1byte�βμ�255��
#define TFT_CMD_WR_CABCMB     0x5E
//��������������Ӧ���ȿ�����С���ȣ�����2Byte 1��Ч2ͬ��
#define TFT_CMD_RD_CABCMB     0x5F

//�����Զ����ȿ�������Ͻ��������2Byte 1��Ч
#define TFT_CMD_RD_ABCSDR     0x68
//���ص�2Byte����Ϊ��
#define TFT_CABCMB_REG_LOAD   (1 << 7) //�Ĵ���װ�����
#define TFT_CABCMB_FUN_DET    (1 << 6) //���������

//�����/�����λ�����غڰ���ɫ���������λ ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_BWLK     0x70
//���BKx(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Bkx     0x71
//���Bky(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Bky     0x72
//���Wx(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Wx      0x73
//���Wy(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Wy      0x74
//����������λ, ���غ�����ɫ���������λ ����2Byte 1��Ч2Ϊֵ:��4bit�죬��4bit��
#define TFT_CMD_RD_RGx    0x75
//���Rx(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Rx     0x76
//���Ry(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Ry     0x77
//���Gx(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Gx     0x78
//���Gy(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Gy     0x79
//�������Alpha���λ, ������A���������λ ����2Byte 1��Ч2Ϊֵ:��4bit������4bitA
#define TFT_CMD_RD_BAx    0x7A
//���Bx(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Bx     0x7B
//���By(��ɫ����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_By     0x7C
//���Ax(Alpha����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Ax     0x7D
//���By(Alpha����)λ2-9bit, ����2Byte 1��Ч2Ϊֵ
#define TFT_CMD_RD_Ay     0x7E

//���DDB��ʼ, ����6Byte 1��Ч2-5Ϊֵ, 6Byte��Ч
#define TFT_CMD_RD_DDB_START 0xA1
//�������DDB, ����nByte 1��Ч,����ΪDDBֵ 
#define TFT_CMD_RD_DDB_CONT 0xA8

//�������1��У��ͽ��, ����2Byte 1��Ч,2Ϊֵ 
#define TFT_CMD_RD_FCS     0xAA
//����������ڶ���У��ͽ��, ����2Byte 1��Ч,2Ϊֵ 
#define TFT_CMD_RD_CCS     0xAF

//������ID1������ID,����2Byte 1��Ч,2Ϊֵ 
#define TFT_CMD_RD_ID1     0xDA
//������ID2���汾��,����2Byte 1��Ч,2Ϊֵ 
#define TFT_CMD_RD_ID2     0xDB
//������ID3��ģ��ID,����2Byte 1��Ч,2Ϊֵ 
#define TFT_CMD_RD_ID3     0xDC

#endif //#ifndef TFT_CMD_H

