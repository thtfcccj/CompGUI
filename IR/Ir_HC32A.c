/***************************************************************************
              ������ճ�����ж�-��HC32�еĸ߼���ʱ����ʵ��
��ģ��ʵ���뵥Ƭ�����,���ڸ���Ir_HC32������ݽ���,���������Ļص�����
ʹ��Ӳ���̻�A(ƥ��Ĵ���A)
***************************************************************************/

#include "IrDrv.h"
#include "IOCtrl.h"

#include "HC32.h"
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

struct _Ir_Hc32{
  unsigned short IdieWidth;     //������Ir�ز�ʱ������0ʱ��ʾû�иû��弴�����ź�����
  unsigned short WidthAppend;   //���ڲ����ж�Ӱ������ļ�����ʧ
};

struct _Ir_Hc32 Ir_Hc32;

#ifdef IR_HW_DEBUG
  struct {
    unsigned char Pos;
    unsigned short IdieWidth[IR_DRV_CODE_SIZE];
    unsigned short Width[IR_DRV_CODE_SIZE];
    unsigned short Next[IR_DRV_CODE_SIZE];    
  }_Ir_Debug;
#endif

//--------------------------ͨ����ص�����-----------------------------
#if (IR_CH == 0)
  //����Ϊ�����ز���Ӳ������A����4��CHxA �˿��ϲ�����������
  #define _CfgRiseCap() do{IR_TIM->HCPAR =  (1 << 4);}while(0) 
  //����Ϊ�½��ز���Ӳ������A����5��CHxA �˿��ϲ������½���
  #define _CfgFailCap() do{IR_TIM->HCPAR =  (1 << 5);}while(0)
#else           
  //����Ϊ�����ز���Ӳ������A����6��CHxB �˿��ϲ�����������
  #define _CfgRiseCap() do{IR_TIM->HCPAR =  (1 << 6);}while(0) 
  //����Ϊ�½��ز���Ӳ������A����7��CHxB �˿��ϲ������½���
  #define _CfgFailCap() do{IR_TIM->HCPAR =  (1 << 7);}while(0) 
#endif

//---------------------IrDrvӲ����ʼ���ص�����ʵ��-----------------------------
void IrDrv_cbHwInit(void)
{
  CfgIR();  
  
  //1. TIM456 ģ��ʱ��ʹ��
  M0P_SYSCTRL->PERI_CLKEN_f.ADVTIM = 1;
  
  //2.���ö�ʱ�������������(10mS���һ��),��ʱ��ʱ������Ϊ1MHZ��ÿ�μ���ʱ��Ϊ1uS
  IR_TIM->PERAR = IR_PERAR - 1;          //��ʱ����
  IR_TIM->GCONR_f.CKDIV = IR_CKDIV; //��ƵΪ���ã�����Ĭ��(������,A��)
  
  //3.Timerͨ�����벶���������
  #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
    _CfgFailCap();    
  #else
    _CfgRiseCap(); 
  #endif
    
  IR_TIM->PCONR =  (1 << 0);     //�������빦��A(b0) 
  
  //4.Timer3�ж����ȼ��������	
  NVIC_ClearPendingIRQ(IR_TIM_IRQ); //������ж�
  NVIC_EnableIRQ(IR_TIM_IRQ);       //�����ж�
  NVIC_SetPriority(IR_TIM_IRQ, 1);  //�����ȼ�(��ռʽԽСԽ��)
  
  //5.������ʱ��
  IR_TIM->ICONR = (1 << 6) | (1 << 0);//�����ж�ʹ��(b5)�����ƥ���ж�ʹ��A
  IR_TIM->GCONR_f.START = 1;    //������ʱ��
}

//-----------------------�жϴ������----------------------------
void IR_TIM_IRQHandler(void)
{
  //��������жϲ���
  if(IR_TIM->IFR_f.OVFF){ //�����ж���
    IrDrv_IRQ(65535, 0);       //һ�ܽ���
    Ir_Hc32.IdieWidth = 0;     //�л�Ϊʶ��Idieģʽ
    IR_TIM->ICONR_f.INTENOVF = 0;//�ݹر�����ж�(�׸�IR����ʱ�����ܺܳ�(>10ms�Ķ���))
    #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
      _CfgFailCap();    
    #else
      _CfgRiseCap(); 
    #endif
    #ifdef IR_HW_DEBUG
      _Ir_Debug.Pos = 0;
    #endif
  }

  //�����жϲ���
  if(IR_TIM->IFR_f.CMAF){  
    //������ס��ǰ��ʱֵ����λ
    unsigned short WidthAppend = IR_TIM->CNTER; 
    unsigned short Width = IR_TIM->GCMAR; 
    
    do{//ͬ�����ܹر�
      IR_TIM->GCONR_f.START = 0;    //�رն�ʱ��
      IR_TIM->CNTER = 0;
    }while(IR_TIM->CNTER);
    IR_TIM->GCONR_f.START = 1;    //������ʱ��
    #ifdef IR_HW_DEBUG
      _Ir_Debug.Next[_Ir_Debug.Pos] = IR_TIM->CNTER;
    #endif  
 
    //׼���������
    WidthAppend -= Width;//���жϵ���ǰΪ���л��ص�ʱ��,���в���
    Width -= 0;
    Width += Ir_Hc32.WidthAppend; //���ѵı���
    Ir_Hc32.WidthAppend = WidthAppend;
    
    //��⵽��������ʱ(��ǰ��Ir�����)
    if(!Ir_Hc32.IdieWidth){
      Ir_Hc32.IdieWidth = Width;
      #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�����ؼ�����ź�ʱ��
        _CfgRiseCap();//����Ϊ�����ز�����CC1P=0     
      #else
        _CfgFailCap(); 
      #endif 
        
      #ifdef IR_HW_DEBUG
        _Ir_Debug.IdieWidth[_Ir_Debug.Pos] = Width;
      #endif
    }
    //���Ir����ʱ(��ǰ�ڿ������ڼ����)  
    else{ 
      #if(IR_DRV_VALID_LEVEL == 0) //�͵�ƽΪ��Ч�ز�ʱ,��Ϊ�½��ؼ������ź�ʱ��
        _CfgFailCap();    
      #else
        _CfgRiseCap(); 
      #endif
      #ifdef IR_HW_DEBUG
        _Ir_Debug.Width[_Ir_Debug.Pos] = Width;
        if(_Ir_Debug.Pos < (IR_DRV_CODE_SIZE - 1)) _Ir_Debug.Pos++;
      #endif  
      IrDrv_IRQ(Ir_Hc32.IdieWidth, Width);  //���ݴ���  
      Ir_Hc32.IdieWidth = 0;//�л�Ϊʶ���������ģʽ
      if(!IR_TIM->ICONR_f.INTENOVF){//�׸�IR����ʱ,���ܺܳ�(>10ms�Ķ���),�ڼ䲻��λ
        IR_TIM->ICONR_f.INTENOVF = 1;//��������ж�
      }
    }			 		     	    					   
  }
  IR_TIM->ICLR = 0; //`((1 << 6) | (1 << 0));//�������ж��벶���жϱ�־
}
