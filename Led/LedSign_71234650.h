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
#define LED_SIGN_0     0x3F
#define LED_SIGN_1     0x06
#define LED_SIGN_2     0x5B
#define LED_SIGN_3     0x4F
#define LED_SIGN_4     0x66
#define LED_SIGN_5     0x6D
#define LED_SIGN_6     0x7D
#define LED_SIGN_7     0x07
#define LED_SIGN_8     0x7F
#define LED_SIGN_9     0x6F
#define LED_SIGN__     0x40

#define LED_SIGN_A     0x77
#define LED_SIGN_B     0x7C
#define LED_SIGN_C     0x39
#define LED_SIGN_D     0x5E
#define LED_SIGN_E     0x79
#define LED_SIGN_F     0x71
#define LED_SIGN_G     0x3D
#define LED_SIGN_H     0x76
#define LED_SIGN_h     0x74  //小写h
#define LED_SIGN_I     0x30
#define LED_SIGN_J     0x0E
#define LED_SIGN_L     0x38
#define LED_SIGN_N     0x37
#define LED_SIGN_n     0x54  //小写n
#define LED_SIGN_O     0x5C
#define LED_SIGN_P     0x73
#define LED_SIGN_Q     0x67
#define LED_SIGN_R     0x31
#define LED_SIGN_S     0x6D
#define LED_SIGN_T     0x78
#define LED_SIGN_U     0x3E
#define LED_SIGN_Y     0x6E
#define LED_SIGN_Z     0x5B

#define LED_SIGN_DOT   0x80    //段码上的小数点位置

#endif //__LED_SIGN_71234650_H
