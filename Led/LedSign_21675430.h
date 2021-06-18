/***************************************************************************
                       �������ģ����-��׼SEG(һһ��Ӧ)
//��Ҫ����ʾ����ʹ��
***************************************************************************/

#ifndef __LED_SIGN_21675430_H
#define __LED_SIGN_21675430_H

//����λ����IO��Ӧ��ϵΪ:
//        a->2 
//      -------
//      |      | 
// f->4 |      | b->1
//      | g->3 |
//      --------   
//      |      | 
// e->5 |      | c->6
//      |      |
//      ------- . dp->0
//        d->7

//------------------------------���붨��-----------------------------
#define LED_SIGN_0     0xF6
#define LED_SIGN_1     0x42
#define LED_SIGN_2     0xA3
#define LED_SIGN_3     0xCE
#define LED_SIGN_4     0x5A
#define LED_SIGN_5     0xDC
#define LED_SIGN_6     0xFC
#define LED_SIGN_7     0x46
#define LED_SIGN_8     0xFE
#define LED_SIGN_9     0xDE
#define LED_SIGN__     0x08

#define LED_SIGN_A     0x7E
#define LED_SIGN_B     0xF8
#define LED_SIGN_C     0xB4
#define LED_SIGN_D     0xEA
#define LED_SIGN_E     0xBC
#define LED_SIGN_F     0x3C
#define LED_SIGN_G     0xF4
#define LED_SIGN_H     0x7A
#define LED_SIGN_h     0x78  //Сдh
#define LED_SIGN_I     0x30
#define LED_SIGN_J     0xC2
#define LED_SIGN_L     0xB0
#define LED_SIGN_N     0x76
#define LED_SIGN_n     0x68  //Сдn
#define LED_SIGN_O     0xE8
#define LED_SIGN_P     0x3E
#define LED_SIGN_Q     0x5E
#define LED_SIGN_R     0x28
#define LED_SIGN_S     0xDC
#define LED_SIGN_T     0xB8
#define LED_SIGN_U     0xF2
#define LED_SIGN_Y     0xDA
#define LED_SIGN_Z     0xA3

#define LED_SIGN_DOT   0x01    //�����ϵ�С����λ��

#endif //__LED_SIGN_21675430_H
