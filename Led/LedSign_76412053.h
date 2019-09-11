/***************************************************************************
                       �������ģ����-��׼��չ1SEG����
1.��DAP3301(BP18���)��ʹ��
***************************************************************************/

#ifndef __LED_SIGN_76412053_H
#define __LED_SIGN_76412053_H

//����λ����IO��Ӧ��ϵΪ:
//        a->7 
//      -------
//      |      | 
// f->0 |      | b->6
//      | g->5 |
//      --------   
//      |      | 
// e->2 |      | c->4
//      |      |
//      ------- . dp->3
//        d->1


//------------------------------���붨��-----------------------------
#define LED_SIGN_0     0xD7
#define LED_SIGN_1     0x50
#define LED_SIGN_2     0xe6
#define LED_SIGN_3     0xF2
#define LED_SIGN_4     0x71
#define LED_SIGN_5     0xb3
#define LED_SIGN_6     0xb7
#define LED_SIGN_7     0xd0
#define LED_SIGN_8     0xf7
#define LED_SIGN_9     0xf3
#define LED_SIGN__     0x20

#define LED_SIGN_A     0xf5
#define LED_SIGN_B     0x37
#define LED_SIGN_C     0x87
#define LED_SIGN_D     0x76
#define LED_SIGN_E     0xa7
#define LED_SIGN_F     0xa5
#define LED_SIGN_G     0x97
#define LED_SIGN_H     0x75
#define LED_SIGN_h     0x35  //Сдh
#define LED_SIGN_I     0x05
#define LED_SIGN_J     0x52

#define LED_SIGN_L     0x07
#define LED_SIGN_N     0xd5
#define LED_SIGN_n     0x34  //Сдn
#define LED_SIGN_O     0x36
#define LED_SIGN_P     0xe5
#define LED_SIGN_Q     0xf1
#define LED_SIGN_R     0x85
#define LED_SIGN_r     0x24
#define LED_SIGN_S     0xb3
#define LED_SIGN_T     0x27
#define LED_SIGN_U     0x57
#define LED_SIGN_Y     0x73
#define LED_SIGN_Z     0xe6

#define LED_SIGN_DOT   0x08    //�����ϵ�С����λ��

#endif //#define __LED_SIGN_76412053_H