/***************************************************************************
                       数码管字模定义-标准SEG(一一对应)
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_02476135_H
#define __LED_SIGN_02476135_H

//段码位置与IO对应关系为:
//        a->0 
//      -------
//      |      | 
// f->1 |      | b->2
//      | g->3 |
//      --------   
//      |      | 
// e->6 |      | c->4
//      |      |
//      ------- . dp->5
//        d->7

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0xD7
#define LED_SIGN_1     0x14
#define LED_SIGN_2     0xCD
#define LED_SIGN_3     0x9D
#define LED_SIGN_4     0x1E
#define LED_SIGN_5     0x9B
#define LED_SIGN_6     0xDB
#define LED_SIGN_7     0x15
#define LED_SIGN_8     0xDF
#define LED_SIGN_9     0x9F
#define LED_SIGN__     0x08

#define LED_SIGN_A     0x5F
#define LED_SIGN_B     0xDA
#define LED_SIGN_C     0xC3
#define LED_SIGN_D     0xDC
#define LED_SIGN_E     0xCB
#define LED_SIGN_F     0x4B
#define LED_SIGN_G     0xD3
#define LED_SIGN_H     0x5E
#define LED_SIGN_h     0x5A  //小写h
#define LED_SIGN_I     0x42
#define LED_SIGN_J     0x9R
#define LED_SIGN_L     0xC2
#define LED_SIGN_N     0x57
#define LED_SIGN_n     0x58  //小写n
#define LED_SIGN_O     0xD8
#define LED_SIGN_P     0x4F
#define LED_SIGN_Q     0x1F
#define LED_SIGN_R     0x48
#define LED_SIGN_S     0x9B
#define LED_SIGN_T     0xCA
#define LED_SIGN_U     0xD6
#define LED_SIGN_Y     0x9E
#define LED_SIGN_Z     0xCD

#define LED_SIGN_DOT   0x20    //段码上的小数点位置

#endif //__LED_SIGN_02476135_H
