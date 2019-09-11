/***************************************************************************
                       数码管字模定义-标准SEG(一一对应)
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_06421753_H
#define __LED_SIGN_06421753_H

//段码位置与IO对应关系为:
//        a->0 
//      -------
//      |      | 
// f->7 |      | b->6
//      | g->5 |
//      --------   
//      |      | 
// e->1 |      | c->4
//      |      |
//      ------- . dp->3
//        d->2

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0xD7
#define LED_SIGN_1     0x50
#define LED_SIGN_2     0x67
#define LED_SIGN_3     0x75
#define LED_SIGN_4     0xF0
#define LED_SIGN_5     0xB5
#define LED_SIGN_6     0xB7
#define LED_SIGN_7     0x51
#define LED_SIGN_8     0xF7
#define LED_SIGN_9     0xF5
#define LED_SIGN__     0x20

#define LED_SIGN_A     0xF3
#define LED_SIGN_B     0xB6
#define LED_SIGN_C     0x87
#define LED_SIGN_D     0x76
#define LED_SIGN_E     0xA7
#define LED_SIGN_F     0xA3
#define LED_SIGN_G     0x97
#define LED_SIGN_H     0xF2

#define LED_SIGN_h     0xB2  //小写h
#define LED_SIGN_I     0x82
#define LED_SIGN_J     0x54
#define LED_SIGN_L     0x86
#define LED_SIGN_N     0xD3
#define LED_SIGN_n     0x32  //小写n
#define LED_SIGN_O     0x36
#define LED_SIGN_P     0xE3
#define LED_SIGN_Q     0xF1
#define LED_SIGN_R     0x22
#define LED_SIGN_S     0xB5
#define LED_SIGN_T     0xA6
#define LED_SIGN_U     0xD6
#define LED_SIGN_Y     0xF4
#define LED_SIGN_Z     0x67

#define LED_SIGN_DOT   0x08    //段码上的小数点位置

#endif //__LED_SIGN_06421753_H
