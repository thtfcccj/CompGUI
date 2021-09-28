/***************************************************************************

              ������ճ�����ж�-��ASM32�еĸ߼���ʱ����ʵ��
��ģ��ʵ���뵥Ƭ�����,���ڸ���Ir_ASM32������ݽ���,���������Ļص�����
***************************************************************************/
#include "IrDrv.h"
#include "IOCtrl.h"

#include "ASM32.h"
#include "Delay.h"

/***************************************************************************
                            �ڲ�����
****************************************************************************/
//IoCtrl���趨��:

//#define IR_TIM        M0P_TIM4    //ָ����ʱ��
//#define IR_CH         0            //ͨ����, Aͨ��ʱΪ0, Bͨ��ʱΪ1
//��ʱ����أ��������Ƶ�����ֵ���Ա�֤���ʱ��Ϊ10ms
//#define IR_CKDIV      3           //��Ƶֵ,<4����λֵ,5:64��,6:256��,7:1024��
//#define IR_PERAR      30000      //�������ֵ
//�ж����: 
//#define IR_TIM_IRQHandler TIM4_IRQHandler //�����жϺ�����
//#define IR_TIM_IRQ  TIM4_IRQn             //�жϱ��



/***************************************************************************
                          ��غ���ʵ��
****************************************************************************/

struct _Ir_ASM32{
  unsigned short IdieWidth;     //������Ir�ز�ʱ������0ʱ��ʾû�иû��弴�����ź�����
  unsigned short WidthAppend;   //���ڲ����ж�Ӱ������ļ�����ʧ
};

struct _Ir_ASM32 Ir_ASM32;

#ifdef IR_HW_DEBUG
  struct {
    unsigned char Pos;
    unsigned short IdieWidth[IR_DRV_CODE_SIZE];
    unsigned short Width[IR_DRV_CODE_SIZE];
    unsigned short Next[IR_DRV_CODE_SIZE];    
  }_Ir_Debug;
#endif

//---------------------IrDrvӲ����ʼ���ص�����ʵ��-----------------------------
void IrDrv_cbHwInit(void)
{
  CfgIR();  //��IO�� 1. ģ��ʱ��ʹ��

  //2.���ö�ʱ�������������(10mS���һ��),��ʱ��ʱ������Ϊ1MHZ��ÿ�μ���ʱ��Ϊ1uS
  IR_TIM->ARR = IR_PERAR - 1;          //��ʱ����
  IR_TIM->PSC = IR_CKDIV; //��ƵΪ���� = fCK_PSC/( PSC[15:0]+1)
  
  //3.Timerͨ�����벶���������
  #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
    IR_CfgFailCap();    //����Ϊ�½��ز���,�����˲�������ͨ����,��ʹ��
  #else
    IR_CfgRiseCap();    ///����Ϊ�����ز���,�����˲�������ͨ����,��ʹ��
  #endif
  
  //4.Timer3�ж����ȼ��������	
  NVIC_ClearPendingIRQ(IR_TIM_IRQ); //������ж�
  NVIC_EnableIRQ(IR_TIM_IRQ);       //�����ж�
  NVIC_SetPriority(IR_TIM_IRQ, 1);  //�����ȼ�(��ռʽԽСԽ��)
  
  //5.������ʱ��
  IR_TIM->DIER = (1 << (1 + IR_CH)) | TIM_DIER_UIE;//�����ж�ʹ��(b0)�����ƥ���ж�ʹ��
  IR_TIM->CR1 |= TIM_CR1_CEN;    //������ʱ��
}

//-----------------------�жϴ������----------------------------
void IR_TIM_IRQHandler(void)
{
  //��������жϲ���
  if((IR_TIM->DIER & TIM_DIER_UIE) && (IR_TIM->SR & TIM_SR_UIF)){ //�����ж���
    IR_TIM->SR &= ~TIM_SR_UIF;
    IrDrv_IRQ(65535, 0);       //һ�ܽ���
    Ir_ASM32.IdieWidth = 0;     //�л�Ϊʶ��Idieģʽ
    IR_TIM->DIER &= ~TIM_DIER_UIE;//�ݹر�����ж�(�׸�IR����ʱ�����ܺܳ�(>10ms�Ķ���))
    #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
      IR_CfgFailCap();    
    #else
      IR_CfgRiseCap(); 
    #endif
    #ifdef IR_HW_DEBUG
      _Ir_Debug.Pos = 0;
    #endif
  }
  
  

  //�����жϲ���
  if(IR_TIM->SR & (1 << (1 + IR_CH))){  
    //������ס��ǰ��ʱֵ����λ
    unsigned short WidthAppend = IR_TIM->CNT; 
    unsigned short Width = IR_TIM->IR_CCR; 
    IR_TIM->SR &= ~(1 << (1 + IR_CH));
    
    //do{//ͬ�����ܹر�
    //  IR_TIM->CR1 &= ~TIM_CR1_CEN;    //�ȹرն�ʱ��
    //  IR_TIM->CNT = 0;
    //}while(IR_TIM->CNT);
    //IR_TIM->CR1 |= TIM_CR1_CEN;    //�ٿ�����ʱ��
    IR_TIM->CNT = 0;
    #ifdef IR_HW_DEBUG
      _Ir_Debug.Next[_Ir_Debug.Pos] = IR_TIM->CNT;
    #endif  
 
    //׼���������
    WidthAppend -= Width;//���жϵ���ǰΪ���л��ص�ʱ��,���в���
    Width -= 0;
    Width += Ir_ASM32.WidthAppend; //���ѵı���
    Ir_ASM32.WidthAppend = WidthAppend;
    
    //��⵽��������ʱ(��ǰ��Ir�����)
    if(!Ir_ASM32.IdieWidth){
      Ir_ASM32.IdieWidth = Width;
      #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�����ؼ�����ź�ʱ��
        IR_CfgRiseCap();//����Ϊ�����ز�����CC1P=0     
      #else
        IR_CfgFailCap(); 
      #endif 
        
      #ifdef IR_HW_DEBUG
        _Ir_Debug.IdieWidth[_Ir_Debug.Pos] = Width;
      #endif
    }
    //���Ir����ʱ(��ǰ�ڿ������ڼ����)  
    else{ 
      #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
        IR_CfgFailCap();    
      #else
        IR_CfgRiseCap(); 
      #endif
      #ifdef IR_HW_DEBUG
        _Ir_Debug.Width[_Ir_Debug.Pos] = Width;
        if(_Ir_Debug.Pos < (IR_DRV_CODE_SIZE - 1)) _Ir_Debug.Pos++;
      #endif  
      IrDrv_IRQ(Ir_ASM32.IdieWidth, Width);  //���ݴ���  
      Ir_ASM32.IdieWidth = 0;//�л�Ϊʶ���������ģʽ
      if(!(IR_TIM->SR & TIM_SR_UIF)){//�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���),�ڼ䲻��λ
        IR_TIM->DIER |= TIM_DIER_UIE;//��������ж�
      }
    }			 		     	    					   
  }
  IR_TIM->SR = 0; //�������ж��벶���жϱ�־
}
