/***************************************************************************
                       �������ģ����-��׼��չ1SEG����
//1.��DAP3301(010���ͨ����)��ʹ��
***************************************************************************/

#ifndef __LED_SIGN_27510463_H
#define __LED_SIGN_27510463_H

//����λ����IO��Ӧ��ϵΪ:
//        a->2 
//      -------
//      |      | 
// f->4 |      | b->7
//      | g->6 |
//      --------   
//      |      | 
// e->0 |      | c->5
//      |      |
//      ------- . dp->3
//        d->1

//------------------------------���붨��-----------------------------
#define LED_SIGN_0     0xB7
#define LED_SIGN_1     0xA0
#define LED_SIGN_2     0xC7
#define LED_SIGN_3     0xE6
#define LED_SIGN_4     0xF0
#define LED_SIGN_5     0x76
#define LED_SIGN_6     0x77
#define LED_SIGN_7     0xA4
#define LED_SIGN_8     0xf7
#define LED_SIGN_9     0xf6
#define LED_SIGN__     0x40

#define LED_SIGN_A     0xf5
#define LED_SIGN_B     0x73
#define LED_SIGN_C     0x17
#define LED_SIGN_D     0xe3
#define LED_SIGN_E     0x57
#define LED_SIGN_F     0x55
#define LED_SIGN_G     0x37
#define LED_SIGN_H     0xf1
#define LED_SIGN_h     0x71  //Сдh
#define LED_SIGN_I     0x11
#define LED_SIGN_J     0xa2

#define LED_SIGN_L     0x13
#define LED_SIGN_N     0xb5
#define LED_SIGN_n     0x61  //Сдn
#define LED_SIGN_O     0x63
#define LED_SIGN_P     0xd5
#define LED_SIGN_Q     0xf4
#define LED_SIGN_R     0x15
#define LED_SIGN_r     0x41
#define LED_SIGN_S     0x76
#define LED_SIGN_T     0x53
#define LED_SIGN_U     0xb3
#define LED_SIGN_Y     0xf2
#define LED_SIGN_Z     0xc8

#define LED_SIGN_DOT   0x08    //�����ϵ�С����λ��

#endif //#define __LED_SIGN_27510463_H