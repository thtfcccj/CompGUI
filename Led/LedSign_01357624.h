/***************************************************************************
                       数码管字模定义-标准SEG(一一对应)
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_01357624_H
#define __LED_SIGN_01357624_H

//段码位置与IO对应关系为:
//        a->0 
//      -------
//      |      | 
// f->6 |      | b->1
//      | g->2 |
//      --------   
//      |      | 
// e->7 |      | c->3
//      |      |
//      ------- . dp->4
//        d->5

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0xEB
#define LED_SIGN_1     0x0A
#define LED_SIGN_2     0xA7
#define LED_SIGN_3     0x2F
#define LED_SIGN_4     0x4E
#define LED_SIGN_5     0x6D
#define LED_SIGN_6     0xED
#define LED_SIGN_7     0x0B
#define LED_SIGN_8     0xEF
#define LED_SIGN_9     0x6F
#define LED_SIGN__     0x04

#define LED_SIGN_A     0xCF
#define LED_SIGN_B     0xEC
#define LED_SIGN_C     0xE1
#define LED_SIGN_D     0xAE
#define LED_SIGN_E     0xE5
#define LED_SIGN_F     0xC5
#define LED_SIGN_G     0xE9
#define LED_SIGN_H     0xCE
#define LED_SIGN_h     0xCC  //小写h
#define LED_SIGN_I     0xC0
#define LED_SIGN_J     0x2A
#define LED_SIGN_L     0xE0

#define LED_SIGN_N     0xCB
#define LED_SIGN_n     0x8C  //小写n
#define LED_SIGN_O     0xAC
#define LED_SIGN_P     0xC7
#define LED_SIGN_Q     0x4F
#define LED_SIGN_R     0x84
#define LED_SIGN_S     0x6D
#define LED_SIGN_T     0xE4
#define LED_SIGN_U     0xEA
#define LED_SIGN_Y     0x6E
#define LED_SIGN_Z     0xA7

#define LED_SIGN_DOT   0x10    //段码上的小数点位置

#endif //__LED_SIGN_01357624_H
