/***************************************************************************
                       数码管字模定义-76543210SEG
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_76543210_H
#define __LED_SIGN_76543210_H

//段码位置与IO对应关系为:
//        a->7 
//      -------
//      |      | 
// f->2 |      | b->6
//      | g->1 |
//      --------   
//      |      | 
// e->3|       | c->5
//      |      |
//      ------- . dp->0
//        d->4

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0xFC
#define LED_SIGN_1     0x60
#define LED_SIGN_2     0xDA
#define LED_SIGN_3     0xF2
#define LED_SIGN_4     0x66
#define LED_SIGN_5     0xB6
#define LED_SIGN_6     0xBE
#define LED_SIGN_7     0xE0
#define LED_SIGN_8     0xFE
#define LED_SIGN_9     0xF6
#define LED_SIGN__     0x02

#define LED_SIGN_A     0xEE
#define LED_SIGN_B     0x3E
#define LED_SIGN_C     0x9C
#define LED_SIGN_D     0x7A
#define LED_SIGN_E     0x9E
#define LED_SIGN_F     0x8E
#define LED_SIGN_G     0xBC
#define LED_SIGN_H     0x6E
#define LED_SIGN_h     0x2E  //小写h
#define LED_SIGN_I     0x0C
#define LED_SIGN_J     0x70
#define LED_SIGN_L     0x1C
#define LED_SIGN_N     0xEC
#define LED_SIGN_n     0x2A  //小写n
#define LED_SIGN_O     0x3A
#define LED_SIGN_P     0xCE
#define LED_SIGN_Q     0xE6
#define LED_SIGN_R     0x0A
#define LED_SIGN_S     0xB6
#define LED_SIGN_T     0x1E
#define LED_SIGN_U     0x7C
#define LED_SIGN_Y     0x76
#define LED_SIGN_Z     0xDA

#define LED_SIGN_DOT   0x01    //段码上的小数点位置

#endif //__LED_SIGN_76543210_H
