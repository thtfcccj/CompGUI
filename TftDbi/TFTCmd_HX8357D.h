/*******************************************************************************

             FT4.3����ʾ����������׼��������-HX8357D��չָ�����
//�˶�����HX8357Dϵ�п�������ʹ��
//����ָ����ʾģ�����ǰ������Ӧ�����úò��̻���OTP��???
********************************************************************************/
#ifndef TFT_CMD_HX8357D_H
#define TFT_CMD_HX8357D_H

//-----------------------------д�ڲ�ʱ��(����ˢ���ٶ�)-------------------
//2byte����
#define TFT_CMD_WR_OSC     0xB0
//��1Byteλ����Ϊ��
#define TFT_OSC1_I_UADJ_SHIFT  4  //���������뷴��ʱʱ�ӣ���30-105HZԽ��Խ��
#define TFT_OSC1_N_UADJ_MASK   0x0f//��������ʱʱ�ӣ���30-105HZԽ��Խ��
//��2Byteλ����Ϊ��
#define TFT_OSC2_ON        (1 << 7) //����

//---------------------------------д��Դ����-----------------------------
//7byte����
#define TFT_CMD_WR_POWER     0xB1
//��1Byteλ����Ϊ��
#define TFT_POWER1_DP_STB    (1 << 0) //��ȴ���ģʽ
//��2Byteλ����Ϊ��
#define TFT_POWER2_BT_MASK   0X03     //VGH��VGL��ѹ�����л���ѹ��·2���������
#define TFT_POWER2_XDK       (1 << 4) //ָ��VSP��ѹ��������ѹ��·����
#define TFT_POWER2_TRI       (1 << 5) //ָ��VSP��ѹ��������ѹ��·����
//��3Byteλ����Ϊ��
#define TFT_POWER3_VRH_MASK  0x3f //ָ��VSPROUT��ѹ������VSPROUT��ѹ����gamma��ѹ
//��4Byteλ����Ϊ��
#define TFT_POWER4_NVRH_MASK  0x3f //ָ��VSNR��ѹ������VSNR��ѹ����gamma��ѹ
//��5Byteλ����Ϊ��
#define TFT_POWER5_GASEN     (1 << 8)//��Դ�ر�ʱ���쳣�ػ����ӹ���
#define TFT_POWER5_AP_MASK   0X07  //������Դ������Ŵ����ĵ�����������·
//��6Byteλ����Ϊ��
#define TFT_POWER6_FS1_SHIFT  4     //����VSP��VSN��ѹ�����Ĺ���Ƶ��
#define TFT_POWER6_FS0_MASK   0X0F  //����VGH/VGL��ѹ�����Ĺ���Ƶ��
//��7Byteλ����Ϊ��
#define TFT_POWER7_VCOMG_ON    (1 << 6)//ָ����ѹ��·3�Ŀ�/�ؿ����Բ���VSN��ѹ
#define TFT_POWER7_PON      (1 << 4)//ָ��VGH��VGL��ѹ��������ѹ��·2�Ŀ�/�ؿ���
#define TFT_POWER7_DK_OFF   (1 << 3)//ָ����ѹ��·1�Ŀ�/�ؿ����Բ���VSP��ѹ
#define TFT_POWER7_STB      (1 << 0)//����ģʽ,���������

//---------------------------------д��ʾ����-----------------------------
//5byte����,��Ҫ������������ʾ����ʾ
#define TFT_CMD_WR_DISPLAY     0xB2
//��1Byteλ����Ϊ��
#define TFT_DISPLAY1_ISC0_MASK   0X0F //�ڷ���ʾ�����У���REFΪ��1��ʱ��ָ������������ɨ������
//��2Byteλ����Ϊ��
#define TFT_DISPLAY2_REF   (1 << 0)//ˢ�²���ģʽ����λ�ķ���ʾ����ʾ,1����
#define TFT_DISPLAY2_PTG   (1 << 1)//ָ�����������ڷ���ʾ�����ɨ��ģʽ,1�̶�VGL
#define TFT_DISPLAY2_PT0   (1 << 6)//ָ��������ʾģʽ�µķ���ʾ����Դ���
#define TFT_DISPLAY2_PT1   (1 << 7)//ָ��������ʾģʽ�µķ���ʾ����Դ���
//��3Byteλ����Ϊ��
#define TFT_DISPLAY3_NL_MASK   0X3F //����ÿ��8������LCD������
//��4Byteλ����Ϊ��
#define TFT_DISPLAY4_SCN_MASK 0X7F //ָ������������ʼɨ�������
//��5Byteλ����Ϊ��
#define TFT_DISPLAY5_D0    (1 << 2) //��ʾ�ر�ʱ��1�ɲ������ԣ�0���ɲ���������
#define TFT_DISPLAY5_D1    (1 << 3)//1��ʾ�򿪣�����0�ر�
#define TFT_DISPLAY5_DTE   (1 << 4)//0ʱ�̶���VGL��,��������
#define TFT_DISPLAY5_GON   (1 << 5)//0ʱ�̶�VGH��,��������

//---------------------------------дRGB�ӿ�ģʽ----------------------------
//4byte����
#define TFT_CMD_WR_RGBIF     0xB3
//��1Byteλ����Ϊ��
#define TFT_RGBIF1_DM     (1 << 0)//����ѡ����ʾ����ģʽ��0�ڲ�ʱ�ӣ�1DPI�ӿ�
#define TFT_RGBIF1_RM     (1 << 1)//RAM���ʷ�ʽ: 0ʱ��DBI�ӿڣ�1ʱ��DPI�ӿ� 
#define TFT_RGBIF1_EPF0   (1 << 4)//65Kɫʱ��RGB���ģʽ,Ĭ��RGB565
#define TFT_RGBIF1_EPF1   (1 << 5)//65Kɫʱ��RGB���ģʽ,Ĭ��RGB565
#define TFT_RGBIF1_BYPASS (1 << 6)//RGB�ӿ�ʱѡ����ʾ����·����0�ڴ棬1��ʾ
#define TFT_RGBIF1_SDO_EN (1 << 7)//SDO��������
//��2Byteλ����Ϊ��
#define TFT_RGBIF2_EPL     (1 << 0)//��RGB�ӿ�ģʽ��ָ�����ùܽŵļ���
#define TFT_RGBIF2_VSPL    (1 << 1)//VSYNC���ŵļ���, 0���٣�1����
#define TFT_RGBIF2_HSPL    (1 << 2)//HSYNC���ŵļ���, 0���٣�1����
#define TFT_RGBIF2_DPL     (1 << 3)//DOTCLK pin�ļ��ԣ� 0�����أ�1�½���
//��3Byteλ����Ϊ��
#define TFT_RGBIF3_HBP_MASK 0X3F //���ô�HSYNC�ź�����Ч�߽�ʱ��Ч���ظ���
#define TFT_RGBIF3_RCM     (1 << 7)//RGBģʽʱ��0VS+HS+DE,1S+HS
//��4Byteλ����Ϊ��
#define TFT_RGBIF4_VBP_MASK 0X3F //���ô�VSYNC�ź�����Ч�߽�ʱ��Ч���ظ���

//------------------------------������ʾ���ڶ�ʱ��---------------------------
//7byte����
#define TFT_CMD_WR_CYC     0xB4
//��1Byteλ����Ϊ��
#define TFT_CYC1_N_NW0   (1 << 0)//������/����ģʽ��ָ��LCD������ת����
#define TFT_CYC1_N_NW1   (1 << 1)//������/����ģʽ��ָ��LCD������ת����
#define TFT_CYC1_I_NW0   (1 << 4)//�ڿ���/���ֿ���ģʽ��ָ��LCD������ת����
#define TFT_CYC1_I_NW1   (1 << 5)//�ڿ���/���ֿ���ģʽ��ָ��LCD������ת����
#define TFT_CYC1_ZIV     (1 << 8)//����Z��תģʽ,1��ת
//��2Byteλ����Ϊ��
#define TFT_CYC2_RTN_MASK  0X7F  //Ϊ�ڲ�����ָ��һ��ʱ������, =1/�ڲ�����ʱ��Ƶ�ʣ�fosc��
//��3Byteλ����Ϊ��
#define TFT_CYC3_DIV0     (1 << 0)//ʹ���ڲ�ʱ�ӽ�����ʾ����ʱ�ķ�Ƶ 1<<DIV01
#define TFT_CYC3_DIV1     (1 << 1)//ʹ���ڲ�ʱ�ӽ�����ʾ����ʱ�ķ�Ƶ 1<<DIV01
#define TFT_CYC3_OSC_DIV2 (1 << 3)//�ڲ�ʱ��1��Ƶ
//��4Byteλ����Ϊ��
#define TFT_CYC4_N_NUM_MASK  0XFF//������/����ģʽһ֡�Ŀհ��������ظ���
//��5Byteλ����Ϊ��
#define TFT_CYC5_I_NUM_MASK  0XFF//ָ�����У�8ɫ��ʱһ֡�հ������е�����
//��6Byteλ����Ϊ��
#define TFT_CYC6_GDON        0XFF //ָ���������������ڵ���Ч�������ʼʱ��
//��7Byteλ����Ϊ��
#define TFT_CYC7_GDOF        0XFF//ָ���������������е����������ʱ��

//------------------------------���õ�Դ��ѹ����---------------------------
//3byte����
#define TFT_CMD_WR_BGP   0xB5
//��1Byteλ����Ϊ��
#define TFT_BGP1_VREF_MASK  0X0F  //ָ��VREF��ѹ��������
//��2Byteλ����Ϊ��
#define TFT_BGP2_nVREF_MASK  0X07  //ָ��nVREF��ѹ��������
//��3Byteλ����Ϊ��
#define TFT_BGP3_VVDHS_MASK  0X07  //ָ��VSP/VSN��ѹ��������
#define TFT_BGP3_VPP_SHIFT   5     //ָ��OTP VPP��ѹ��������
#define TFT_BGP3_VPP_MASK    0XE0

//------------------------------����VCOM��ѹ---------------------------
//2byte����
#define TFT_CMD_WR_VCOM   0xB6
//��1Byteλ����Ϊ��
#define TFT_VCOM1_VCOM_MASK      0X7F  //DC VCOM��ѹ���ã�������
#define TFT_VCOM1_OTP_TIME_MASK  0X07  //VCOM OTP���ʱ�����ã�������

//------------------------------����OTP---------------------------
//5byte����(��)
#define TFT_CMD_WR_OTP   0xB7

//------------------------------�����ⲿ��չָ��---------------------------
//3byte����(��)
#define TFT_CMD_WR_EXTC   0xB9
//3byte����,ǰ�������ֽڶ���Ϊ:
#define TFT_EXTC1_PRE0    0XFF
#define TFT_EXTC2_PRE1    0X83
//��3byte����������ֹ�뿪��
#define TFT_EXTC_EN_ARY   0x57
#define TFT_EXTC_DIS_ARY  0x47

//------------------------------����STBA---------------------------
//6byte����
#define TFT_CMD_WR_STBA   0xC0
//��1Byte
#define TFT_STBA1_NP_OPON_MASK  0XFF//ָ������ģʽ�µ���ЧԴ��������
//��2Byte
#define TFT_STBA2_IP_OPON_MASK  0XFF//ָ������ģʽ�µ���ЧԴ��������
//��3-5Byte STBA Դƫѹ����΢������
#define TFT_STBA3_STBA_H_MASK  0XFF
#define TFT_STBA4_STBA_M_MASK  0XFF
#define TFT_STBA5_STBA_L_MASK  0XFF
//��6Byte STBA Դƫѹ����΢������
#define TFT_STBA6_GENON_MASK  0XFF//GAMA��������ʱ��

//------------------------------����DGC---------------------------
//��GAMAУ׼���߲��ұ�100byte����
#define TFT_CMD_WR_DGC   0xC1
#define TFT_DGC1_EN      0X80  //����GAMA����
//��Ӧ�ֽ���ֹ��Ϣ��ÿ��33byte
#define TFT_DGC_R_START  2
#define TFT_DGC_G_START  35
#define TFT_DGC_B_START  68
//------------------------------����ID��---------------------------
//4byte����,ǰ3�ɸߵ���ID,4ΪдOTPʱ��
#define TFT_CMD_WR_ID   0xC3

//------------------------------����DDB---------------------------
//4byte����,�ɸߵ���DDB
#define TFT_CMD_WR_DDB   0xC4

//------------------------------����CABC---------------------------
//����PWM���Ʊ������ȼ����أ� 2byte����(��)
#define TFT_CMD_WR_CABC_REL   0xC9

//------------------------------����PANNEL---------------------------
//��������������ԣ� 1byte����
#define TFT_CMD_WR_PANNEL   0xCC

//------------------------------����GAMA����---------------------------
//34byte����
#define TFT_CMD_WR_GAMA   0xE0
//ǰ��������VRP(ǰ16byte)��VRN(ǰ16byte) 
#define TFT_GAMA_VRP_START  0
#define TFT_GAMA_VRN_START  16
//��2Byte��

//------------------------------����ͼ���ʽ---------------------------
//��1byte����(��),��Ҫ����24bit�������뷽ʽ����
#define TFT_CMD_WR_IMAGE   0xE9
//��Ȩ֧��D0: DB_EN

//------------------------------����ͼ���ʽ---------------------------
//1byte����
#define TFT_CMD_WR_MESSI   0xEA
//��1Byte
#define TFT_MESSI_ENB    (1 << 0)//��������
#define TFT_SPI_DCX_SEL  (1 << 1)//4��SPI�ӿ� 0:8th SCL, 1:1st SCL

//--------------------------------������ɫ-----------------------------
//15byte����,��ϰ�10bit����, ����
//Bkx[9:0]��Bky[9:0]: Wx[9:0]��Wy[9:0]: Rx[9:0]��Ry[9:0]:
//Gx[9:0]��Gy[9:0]: Bx[9:0]��By[9:0]: Ax[9:0]��Ay[9:0]:
#define TFT_CMD_WR_COLOR   0xEB

//------------------------------����SPI�������ַ---------------------------
//Ϊ�û�������������SPI��ȡ�����ַ, 1byte����
#define TFT_CMD_WR_SPI_CMD_BASE   0xEA

//------------------------------��SPI��������--------------------------
//����:"Ϊ�û�������������SPI��ȡ�����ַ"��ʼ�����ݣ��׸�����Ч����������
#define TFT_CMD_RD_SPI_CMD_DATA   0xFF

//------------------------------��HIMAX_ID--------------------------
//����HIMAX_ID��1��Ч2Ϊֵ
#define TFT_CMD_RD_HIMAX_ID   0XD0


#endif //#ifndef TFT_CMD_H

