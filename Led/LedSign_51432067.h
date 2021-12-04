/***************************************************************************
                       数码管字模定义-标准SEG(一一对应)
//主要在显示板上使用
***************************************************************************/

#ifndef __LED_SIGN_51432067_H
#define __LED_SIGN_51432067_H

//段码位置与IO对应关系为:
//        a->5 
//      -------
//      |      | 
// f->0 |      | b->1
//      | g->6 |
//      --------   
//      |      | 
// e->2 |      | c->4
//      |      |
//      ------- . dp->7
//        d->3

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0x3F
#define LED_SIGN_1     0x12
#define LED_SIGN_2     0x6E
#define LED_SIGN_3     0x7A
#define LED_SIGN_4     0x53
#define LED_SIGN_5     0x79
#define LED_SIGN_6     0x7D
#define LED_SIGN_7     0x32
#define LED_SIGN_8     0x7F
#define LED_SIGN_9     0x7B
#define LED_SIGN__     0x40

#define LED_SIGN_A     0x77
#define LED_SIGN_B     0x5D
#define LED_SIGN_C     0x2D
#define LED_SIGN_D     0x5E
#define LED_SIGN_E     0x6D
#define LED_SIGN_F     0x65
#define LED_SIGN_G     0x3D
#define LED_SIGN_H     0x5F
#define LED_SIGN_h     0x55  //小写h
#define LED_SIGN_I     0x05
#define LED_SIGN_J     0x1A
#define LED_SIGN_L     0x0D
#define LED_SIGN_N     0x37
#define LED_SIGN_n     0x54  //小写n
#define LED_SIGN_O     0x5C
#define LED_SIGN_P     0x57
#define LED_SIGN_Q     0x73
#define LED_SIGN_R     0x44
#define LED_SIGN_S     0x79
#define LED_SIGN_T     0x4D
#define LED_SIGN_U     0x1F
#define LED_SIGN_Y     0x5B
#define LED_SIGN_Z     0x6E

#define LED_SIGN_DOT   0x80    //段码上的小数点位置

#endif //__LED_SIGN_51432650_H
