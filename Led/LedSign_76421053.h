/***************************************************************************
                       数码管字模定义-标准扩展1SEG定义
在DAP3243以及DAP3123里使用
***************************************************************************/

#ifndef __LED_SIGN_76421053_H
#define __LED_SIGN_76421053_H

//段码位置与IO对应关系为:
//        a->7 
//      -------
//      |      | 
// f->0 |      | b->6
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
#define LED_SIGN_2     0xE6
#define LED_SIGN_3     0xF4
#define LED_SIGN_4     0x71
#define LED_SIGN_5     0xB5
#define LED_SIGN_6     0xB7
#define LED_SIGN_7     0xD0
#define LED_SIGN_8     0xf7
#define LED_SIGN_9     0xf5
#define LED_SIGN__     0x20

#define LED_SIGN_A     0xf3
#define LED_SIGN_B     0x37
#define LED_SIGN_C     0x87
#define LED_SIGN_D     0x76
#define LED_SIGN_E     0xA7
#define LED_SIGN_F     0xA3
#define LED_SIGN_G     0x97
#define LED_SIGN_H     0x73
#define LED_SIGN_h     0x33  //小写h
#define LED_SIGN_I     0x03
#define LED_SIGN_J     0x07

#define LED_SIGN_L     0x07
#define LED_SIGN_N     0xD3
#define LED_SIGN_n     0x32  //小写n
#define LED_SIGN_O     0x36
#define LED_SIGN_P     0xE3
#define LED_SIGN_Q     0xf1
#define LED_SIGN_R     0x83
#define LED_SIGN_r     0x22
#define LED_SIGN_S     0xB5
#define LED_SIGN_T     0x27
#define LED_SIGN_U     0x57
#define LED_SIGN_Y     0x75
#define LED_SIGN_Z     0xE6

#define LED_SIGN_DOT   0x08    //段码上的小数点位置

#endif //#define __LED_SIGN_76421053_H