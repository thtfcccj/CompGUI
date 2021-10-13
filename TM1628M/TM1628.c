/***************************************************************************

                         多例化TM1628驱动实现
TM1628最大工作1MHZ
***************************************************************************/
#include "TM1628.h"
#include <string.h>

unsigned char TM1628_CommBuf[TM1628_COM_BUF];//内建的通讯缓冲区
/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void TM1628_Init(struct _TM1628 *pTM1628, 
                 unsigned char Ch,         //此TM1628通道号
                 unsigned char Gray)       //0~7越高越亮, 8关闭显示
{
  memset(pTM1628, 0, sizeof(struct _TM1628));
  pTM1628->Gray = Gray;
  TM1628_Task(pTM1628);  //开机初始化并更新一次显示
}

//-------------------------按键字节转换为按键ID号----------------------
//不支持多按键模式
#ifdef SUPPORT_TM1628_KEY //支持按键时
static unsigned char _ByteKey2KeyId(struct _TM1628 *pTM1628,
                                     unsigned char KeyCount)
{
  unsigned char KeyId = 0;
  unsigned char KeyByte;
  for(unsigned Pos = 1; Pos <= KeyCount; Pos++){
    KeyByte = TM1628_CommBuf[Pos];
    if(KeyByte & 0x01) break;   //B0按下
    KeyId++;
    if(KeyByte & 0x02) break;   //B1按下
    KeyId++;
    if(KeyByte & 0x08) break;   //B2按下
    KeyId++;
    if(KeyByte & 0x10) break;   //B3按下
    KeyId++;
  }
  return KeyId;
}
#endif

//--------------------------------任务函数-----------------------------
//约10ms调用一次
void TM1628_Task(struct _TM1628 *pTM1628)
{
  unsigned char Ch = pTM1628->Ch;
  const struct _TM1628Static *pStatic = TM1628_cbGetStatic(Ch);
  //周期强制更新与开机/周期初始化以支持热插入
  if(!pTM1628->PeriodInitTimer){
    pTM1628->PeriodInitTimer = TM1628_LED_PERIOD_INIT_OV;
    //显示控制命令
    TM1628_CommBuf[0] = pStatic->DispMode;
    TM1628_cbSendData(Ch, 1);    
    //更新灰度并显示
    if(pTM1628->Gray > 7) TM1628_CommBuf[0] = 0x80;//关闭显示
    else TM1628_CommBuf[0] = 0x88 | pTM1628->Gray;//指令0x80, 开显(B3) + 灰度
    TM1628_cbSendData(Ch, 1);
    pTM1628->DirtyMask = 0xffff; //强制更新显示
  }
  else pTM1628->PeriodInitTimer--;
  
  //按键扫描
  #ifdef SUPPORT_TM1628_KEY //支持按键时
  if(pStatic->KeyByteCount){//本实例有按键时
    if(pTM1628->Flag & TM1628_KEY_PERIOD){//按键扫描周期实时读取数据
      pTM1628->Flag &= ~TM1628_KEY_PERIOD;
      TM1628_CommBuf[0] = 0x42;//读键扫数据
      TM1628_cbFullKey(Ch, pStatic->KeyByteCount);
      pTM1628->Flag &= ~TM1628_KEY_ID_MASK;
      pTM1628->Flag |= _ByteKey2KeyId(pTM1628, pStatic->KeyByteCount);
      TM1628_cbKeyUpdate(pTM1628); //键值更新通报
      return;
    }
    pTM1628->Flag |= TM1628_KEY_PERIOD;//下次为按键周期
  }
  #endif
  //数码管扫描: 策略为：变化时写入
  unsigned short DirtyMask = pTM1628->DirtyMask;
  if(!DirtyMask) return; //没有数据更新

  //暂采用一次性写完策略(代码少但通讯效率低)
  TM1628_CommBuf[0] = 0x40;//写数据到显示寄存器指令(b0:1),自动地址增加(b2)
  TM1628_cbSendData(Ch, 1);
  //写显存
  TM1628_CommBuf[0] = 0xC0;//显示每次从头开始
  if(TM1628_cbIsTest()) 
    memset(&TM1628_CommBuf[1], 0xff, pStatic->LedCount);
  else 
    memcpy(&TM1628_CommBuf[1], pTM1628->DispBuf, pStatic->LedCount);
  TM1628_cbSendData(Ch, 1 + pStatic->LedCount);  
}

//-------------------------------更新数码管显示-------------------------
void TM1628_UpdateDisp(struct _TM1628 *pTM1628,
                       unsigned char UserPos,//内部由TM1628_TO_HW转换
                       unsigned char Seg)   //段码
{
  const struct _TM1628Static *pStatic = TM1628_cbGetStatic(pTM1628->Ch);
  unsigned char HwPos = pStatic->ToHwLut[UserPos];
  if(pTM1628->DispBuf[HwPos] == Seg) return;//未更新
  //更新了
  pTM1628->DispBuf[HwPos] = Seg;
  pTM1628->DirtyMask |= 1 << HwPos;
}

//-------------------------------得到数码管显示-------------------------
unsigned char TM1628_GetDisp(const struct _TM1628 *pTM1628,
                             unsigned char UserPos)
{
  const struct _TM1628Static *pStatic = TM1628_cbGetStatic(pTM1628->Ch);
  return pTM1628->DispBuf[pStatic->ToHwLut[UserPos]];
}

//---------------------------------更新灰度级别-------------------------
void TM1628_UpdateGray(struct _TM1628 *pTM1628,
                       unsigned char Gray)    //0~7越高越亮, 8关闭显示
{
  pTM1628->Gray = Gray;
  pTM1628->PeriodInitTimer = 0;//下次立即生效
}


