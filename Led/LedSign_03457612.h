/***************************************************************************
                       数码管字模定义-标准SEG(一一对应)
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_03457612_H
#define __LED_SIGN_03457612_H

//段码位置与IO对应关系为:
//        a->0 
//      -------
//      |      | 
// f->6 |      | b->3
//      | g->1 |
//      --------   
//      |      | 
// e->7 |      | c->4
//      |      |
//      ------- . dp->2
//        d->5

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0xF9
#define LED_SIGN_1     0x18
#define LED_SIGN_2     0xAB
#define LED_SIGN_3     0x3B
#define LED_SIGN_4     0x5A
#define LED_SIGN_5     0x73
#define LED_SIGN_6     0xF3
#define LED_SIGN_7     0x19
#define LED_SIGN_8     0xFB
#define LED_SIGN_9     0x9B
#define LED_SIGN__     0x02

#define LED_SIGN_A     0xDB
#define LED_SIGN_B     0xF2
#define LED_SIGN_C     0xE1
#define LED_SIGN_D     0xBA
#define LED_SIGN_E     0xE3
#define LED_SIGN_F     0xC3
#define LED_SIGN_G     0xF1
#define LED_SIGN_H     0xDA
#define LED_SIGN_h     0xD2  //小写h
#define LED_SIGN_I     0xC0
#define LED_SIGN_J     0x38
#define LED_SIGN_L     0xE0
#define LED_SIGN_N     0xD9
#define LED_SIGN_n     0x92  //小写n
#define LED_SIGN_O     0xB2
#define LED_SIGN_P     0xCB
#define LED_SIGN_Q     0x5B
#define LED_SIGN_R     0x82
#define LED_SIGN_S     0x73
#define LED_SIGN_T     0xE2
#define LED_SIGN_U     0xF8
#define LED_SIGN_Y     0x7A
#define LED_SIGN_Z     0xAB

#define LED_SIGN_DOT   0x04    //段码上的小数点位置

#endif //__LED_SIGN_03457612_H
