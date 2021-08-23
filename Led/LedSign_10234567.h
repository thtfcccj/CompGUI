/***************************************************************************
                       �������ģ����-��׼SEG(һһ��Ӧ)
//��Ҫ����ʾ����ʹ��
***************************************************************************/

#ifndef __LED_SIGN_10234567_H
#define __LED_SIGN_10234567_H

//����λ����IO��Ӧ��ϵΪ:
//        a->1 
//      -------
//      |      | 
// f->5 |      | b->0
//      | g->6 |
//      --------   
//      |      | 
// e->4 |      | c->2
//      |      |
//      ------- . dp->7
//        d->3

//------------------------------���붨��-----------------------------
#define LED_SIGN_0     0x3F
#define LED_SIGN_1     0x05
#define LED_SIGN_2     0x5B
#define LED_SIGN_3     0x4F
#define LED_SIGN_4     0x65
#define LED_SIGN_5     0x6E
#define LED_SIGN_6     0x7E
#define LED_SIGN_7     0x07
#define LED_SIGN_8     0x7F
#define LED_SIGN_9     0x6F
#define LED_SIGN__     0x40

#define LED_SIGN_A     0x77
#define LED_SIGN_B     0x7C
#define LED_SIGN_C     0x3A
#define LED_SIGN_D     0x5D
#define LED_SIGN_E     0x7A
#define LED_SIGN_F     0x72
#define LED_SIGN_G     0x3E
#define LED_SIGN_H     0x75
#define LED_SIGN_h     0x74  //Сдh
#define LED_SIGN_I     0x30
#define LED_SIGN_J     0x0D
#define LED_SIGN_L     0x38
#define LED_SIGN_N     0x37
#define LED_SIGN_n     0x54  //Сдn
#define LED_SIGN_O     0x5C
#define LED_SIGN_P     0x73
#define LED_SIGN_Q     0x67
#define LED_SIGN_R     0x32
#define LED_SIGN_S     0x6E
#define LED_SIGN_T     0x78
#define LED_SIGN_U     0x3D
#define LED_SIGN_Y     0x6D
#define LED_SIGN_Z     0x5B

#define LED_SIGN_DOT   0x80    //�����ϵ�С����λ��

#endif //__LED_SIGN_01234567_H
