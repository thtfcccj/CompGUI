/***************************************************************************
                       独立IO按键实现模块
***************************************************************************/

#include "LedDrv_Spi.h"
#include "GuiSpi.h"     //Spi的GUI接口
#include "LedSign.h"    //LED_SIGN_DOT
#include <string.h>

struct _LedDrv_Spi  LedDrv_Spi;

/***************************************************************************
                              相关函数实现
***************************************************************************/

//-----------------------------初始化函数-----------------------------
void LedDrv_Spi_Init(void)
{
  GuiSpi_Init();
  memset(&LedDrv_Spi, 0, sizeof(struct _LedDrv_Spi));
  //初始化显示板
  GuiSpi.CmdBuf[0] = 0;      //功能码
  GuiSpi.DataBuf[0] = 0x1f;  //用于控制每个数码管是否显示
  GuiSpi.DataBuf[1] = 0xff;    //节能开关及总节能模式总选择,不使用节能  
  GuiSpi.Cmd.CmdSize = 1;    //此协议中固定1个
  GuiSpi.Cmd.DataSize = 2;
  GuiSpi.Cmd.Cfg = GUI_SPI_DEV_CMD_DUPLEX; //启用全双工操作
  GuiSpiDev_Restart(&GuiSpiDev ,&GuiSpi.Cmd); //启动发送
}

//--------------------------更新某个数码管函数---------------------------
void LedDrv_Spi_SetLed(unsigned char Pos,     //数码管位置
                       unsigned char SetDot)//带LED_SIGN_DOT的显示段码值
{
  if(Pos >= LED_DRV_SPI_SEG_COUNT) return; //超限
  LedDrv_Spi.Buf[LED_DRV_SPI_ADR_SEG + Pos] = SetDot & (~LED_SIGN_DOT);//取Seg
  if(SetDot & LED_SIGN_DOT)//带小数点了
    LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] |= 1 << Pos;
  else
   LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] &= ~(1 << Pos);   
  
  LedDrv_Spi_SetUpdate();//置更新标志
}

//--------------------------得到某个数码管函数---------------------------
//返回带LED_SIGN_DOT的显示段码值
unsigned char LedDrv_Spi_GetLed(unsigned char Pos)     //数码管位置
{
  unsigned char SegPos;
  if(LedDrv_Spi.Buf[LED_DRV_SPI_ADR_DOT] & (1 << Pos))
    SegPos = LED_SIGN_DOT;
  else SegPos = 0;
  return SegPos | LedDrv_Spi.Buf[LED_DRV_SPI_ADR_SEG + Pos];
}

//----------------------------快速任务函数-----------------------------
//放入快速进程中扫描
void LedDrv_Spi_FastTask(void)
{
  GuiSpi_FastTask();  //先处理SPI
  
  if(!GuiSpi_IsIdie()) return;    //SPI处理数据过程中

  switch(LedDrv_Spi.eState){
    case eLedDrv_Spi_Idie://空闲模式,检查是否有按键中断,否则可发送数据
      if(!GuiSpi_IsCs()) //等待过程中,CS拉低有中断了
        LedDrv_Spi.eState = eLedDrv_Spi_Wait;
      //有数据要更新了!
      else if(LedDrv_Spi.Buf[LED_DRV_SPI_ADR_STATE] & LED_DRV_SPI_UPDATE){
        //发送数据
        GuiSpi.CmdBuf[0] = LED_DRV_SPI_ADR_START;  //功能码
        if(LedDrv_Spi_cbIsTest()){//写测试数据
          memset(GuiSpi.DataBuf, 0xff, LED_DRV_SPI_ADR_COUNT);
          GuiSpi.DataBuf[1] = LedDrv_Spi.Buf[1];//状态码
        }
        else{ //数据
          memcpy(GuiSpi.DataBuf, LedDrv_Spi.Buf, LED_DRV_SPI_ADR_COUNT);
        }
        GuiSpi.Cmd.DataSize = LED_DRV_SPI_ADR_COUNT;
        GuiSpi.Cmd.Cfg = GUI_SPI_DEV_CMD_DUPLEX; //启用全双工操作
        GuiSpiDev_Restart(&GuiSpiDev ,&GuiSpi.Cmd); //启动发送
        
        LedDrv_Spi.eState = eLedDrv_Spi_Doing;   //发送过程中
        //立即取消,以允许重写入
        LedDrv_Spi.Buf[LED_DRV_SPI_ADR_STATE] &= ~LED_DRV_SPI_UPDATE;
      }
      break;
    case eLedDrv_Spi_Wait://有中断了,等待CS为高
      if(!GuiSpi_IsCs()) break; //还在低电平中
      //准备接收按键码
      LedDrv_Spi_SetUpdate();//置更新标志
      LedDrv_Spi.eState = eLedDrv_Spi_Idie;
      break;
    case eLedDrv_Spi_Doing:{//发送过程完成后,检查是否有按键值,有则回调
      unsigned char Key = GuiSpi.DataBuf[0];
      if(Key) LedDrv_Spi_cbKeyNotify(Key);
      //继续退出
    }
    default:
      LedDrv_Spi.eState = eLedDrv_Spi_Idie;
      break;
  }
}
                               
                               

