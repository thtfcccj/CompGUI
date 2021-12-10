/***************************************************************************
                       数码管字模定义-标准SEG(一一对应)
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_71234650_H
#define __LED_SIGN_71234650_H

//段码位置与IO对应关系为:
//        a->7 
//      -------
//      |      | 
// f->6 |      | b->1
//      | g->5 |
//      --------   
//      |      | 
// e->4 |      | c->2
//      |      |
//      ------- . dp->0
//        d->3

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0xDE
#define LED_SIGN_1     0x06
#define LED_SIGN_2     0xBA
#define LED_SIGN_3     0xAE
#define LED_SIGN_4     0x66
#define LED_SIGN_5     0xEC
#define LED_SIGN_6     0xFC
#define LED_SIGN_7     0x86
#define LED_SIGN_8     0xFE
#define LED_SIGN_9     0xEE
#define LED_SIGN__     0x20

#define LED_SIGN_A     0xF6
#define LED_SIGN_B     0x7C
#define LED_SIGN_C     0xD8
#define LED_SIGN_D     0x3E
#define LED_SIGN_E     0xF8
#define LED_SIGN_F     0xF0
#define LED_SIGN_G     0xDC
#define LED_SIGN_H     0x76
#define LED_SIGN_h     0x74  //小写h
#define LED_SIGN_I     0x50
#define LED_SIGN_i     0x10 //小写i
#define LED_SIGN_J     0x0E
#define LED_SIGN_L     0x58
#define LED_SIGN_N     0xD6
#define LED_SIGN_n     0x34  //小写n
#define LED_SIGN_O     0x3C
#define LED_SIGN_P     0xF2
#define LED_SIGN_Q     0xE6
#define LED_SIGN_R     0xD0
#define LED_SIGN_r     0x30  //小写r
#define LED_SIGN_S     0xEC
#define LED_SIGN_T     0x78
#define LED_SIGN_U     0x5E
#define LED_SIGN_Y     0x6E
#define LED_SIGN_Z     0xBA

#define LED_SIGN_DOT   0x01    //段码上的小数点位置

#endif //__LED_SIGN_71234650_H
