/***************************************************************************

                  TM1628驱动实现
TM1628最大工作1MHZ
***************************************************************************/
#include "TM1628.h"
#include <string.h>

struct _TM1628  TM1628;//直接单例化
static const unsigned char _ToHwLut[] = TM1628_TO_HW;

/***************************************************************************
                              相关函数
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void TM1628_Init(unsigned char Gray)    //0~7越高越亮, 8关闭显示
{
  TM1628_cbHwInit();
  memset(&TM1628, 0, sizeof(struct _TM1628));
  
  //显示控制命令
  TM1628.CommBuf[0] = TM1628_DISP_MODE;
  TM1628_cbSendData(1);
  
  TM1628.DirtyMask = 0xffff; //更新数据
  TM1628_Task();  //更新一次显示
  
  TM1628_UpdateGray(Gray);//更新灰度并显示
}

//-------------------------按键字节转换为按键ID号----------------------
//不支持多按键模式
#ifdef SUPPORT_TM1628_KEY //支持按键时
static unsigned char _ByteKey2KeyId(void)
{
  unsigned char KeyId = 0;
  unsigned char KeyByte;
  for(unsigned Pos = 1; Pos <= SUPPORT_TM1628_KEY; Pos++){
    KeyByte = TM1628.CommBuf[Pos];
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
void TM1628_Task(void)
{
  //按键扫描
  #ifdef SUPPORT_TM1628_KEY //支持按键时
    if(TM1628.Flag & TM1628_KEY_PERIOD){//按键扫描周期实时读取数据
      TM1628.Flag &= ~TM1628_KEY_PERIOD;
      TM1628.CommBuf[0] = 0x42;//读键扫数据
      TM1628_cbFullKey(SUPPORT_TM1628_KEY);
      TM1628.Flag &= ~TM1628_KEY_ID_MASK;
      TM1628.Flag |= _ByteKey2KeyId();
      return;
    }
    TM1628.Flag |= TM1628_KEY_PERIOD;//下次为按键周期
  #endif
  //数码管扫描: 策略为：若
  unsigned short DirtyMask = TM1628.DirtyMask;
  if(!DirtyMask) return; //没有数据更新

  //暂采用一次性写完策略(代码少但通讯效率低)
  TM1628.CommBuf[0] = 0x40;//写数据到显示寄存器指令(b0:1),自动地址增加(b2)
  TM1628_cbSendData(1);
  //写显示存
  TM1628.CommBuf[0] = 0xC0;//显示每次从头开始
  if(TM1628_cbIsTest()) 
    memset(&TM1628.CommBuf[1], 0xff, TM1628_LED_COUNT);
  else 
    memcpy(&TM1628.CommBuf[1], TM1628.DispBuf, TM1628_LED_COUNT);
  TM1628_cbSendData(1 + TM1628_LED_COUNT);  
}

//-------------------------------更新数码管显示-------------------------
void TM1628_UpdateDisp(unsigned char UserPos,//内部由TM1628_TO_HW转换
                       unsigned char Seg)   //段码
{
  unsigned char HwPos = _ToHwLut[UserPos];
  if(TM1628.DispBuf[HwPos] == Seg) return;//未更新
  //更新了
  TM1628.DispBuf[HwPos] = Seg;
  TM1628.DirtyMask |= 1 << HwPos;
}

//-------------------------------得到数码管显示-------------------------
unsigned char TM1628_GetDisp(unsigned char UserPos)
{
  return TM1628.DispBuf[_ToHwLut[UserPos]];
}

//---------------------------------更新灰度级别-------------------------
void TM1628_UpdateGray(unsigned char Gray)    //0~7越高越亮, 8关闭显示
{
  if(Gray > 7) TM1628.CommBuf[0] = 0x80;//关闭显示
  else TM1628.CommBuf[0] = 0x88 | Gray;//指令0x80, 开显(B3) + 灰度
  TM1628_cbSendData(1);
}
