/***************************************************************************
                       数码管字模定义-标准扩展1SEG定义
//1.在DAP3301(010表壳半导体型)里使用
***************************************************************************/

#ifndef __LED_SIGN_20643175_H
#define __LED_SIGN_20643175_H

//段码位置与IO对应关系为:
//        a->2 
//      -------
//      |      | 
// f->1 |      | b->0
//      | g->7 |
//      --------   
//      |      | 
// e->3 |      | c->6
//      |      |
//      ------- . dp->5
//        d->4

//------------------------------段码定义-----------------------------
#define LED_SIGN_0     0x5f
#define LED_SIGN_1     0x41
#define LED_SIGN_2     0x9d
#define LED_SIGN_3     0xd5
#define LED_SIGN_4     0xc3
#define LED_SIGN_5     0xd6
#define LED_SIGN_6     0xde
#define LED_SIGN_7     0x45
#define LED_SIGN_8     0xdf
#define LED_SIGN_9     0xd7
#define LED_SIGN__     0x80

#define LED_SIGN_A     0xcf
#define LED_SIGN_B     0xda
#define LED_SIGN_C     0x1e
#define LED_SIGN_D     0xd9
#define LED_SIGN_E     0x9e
#define LED_SIGN_F     0x8e
#define LED_SIGN_G     0x5e
#define LED_SIGN_H     0xcb
#define LED_SIGN_h     0xca  //小写h
#define LED_SIGN_I     0x0a
#define LED_SIGN_J     0x51

#define LED_SIGN_L     0x1a
#define LED_SIGN_N     0x4f
#define LED_SIGN_n     0xc8  //小写n
#define LED_SIGN_O     0xd8
#define LED_SIGN_P     0x8f
#define LED_SIGN_Q     0xc7
#define LED_SIGN_R     0x0e
#define LED_SIGN_r     0x88
#define LED_SIGN_S     0xd6
#define LED_SIGN_T     0x9a
#define LED_SIGN_U     0x5b
#define LED_SIGN_Y     0xd3
#define LED_SIGN_Z     0x9d

#define LED_SIGN_DOT   0x20    //段码上的小数点位置

#endif //#define __LED_SIGN_20643175_H