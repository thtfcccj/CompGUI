/*************************************************************************

                һ��+4��GUI����ʵ��

**************************************************************************/

#include "Lt4K1Gui.h"
#include <string.h>

struct _Lt4K1Gui Lt4K1Gui;

//----------------------------------��ʼ������--------------------------
void Lt4K1Gui_Init(void)
{
  memset(&Lt4K1Gui, 0, sizeof(struct _Lt4K1Gui));
}

//--------------------------ָʾ��������-------------------------
//����256mS������
void Lt4K1Gui_LedTask(void)
{
  if(Lt4K1Gui.eState < Lt4K1Gui_eMenuSel) return;//�����ܿ�״̬
  
  //����ֻ����ǰ������ָʾ�Ƶ���ʾ����
  unsigned char IsOn; //��ǰ�Ƿ���ʾ
  if(!Lt4K1Gui.LtIndex){//�¸����ڿ�ʼ�ˣ��׸�������
    IsOn = 1;
    if(Lt4K1Gui.LtCount) //��0ʱ��
       Lt4K1Gui.LtIndex = (Lt4K1Gui.LtCount + LT4K1_GUI_LED_IDIE) * 2;
    else Lt4K1Gui.LtIndex = LT4K1_GUI_LED_IDIE * 4; //0ʱ��˫����
  }
  else{
    Lt4K1Gui.LtIndex--;
    if(Lt4K1Gui.LtIndex >= (LT4K1_GUI_LED_IDIE * 2)){//ָʾ����
      if(Lt4K1Gui.LtCount){ //��0ʱ��
        if(Lt4K1Gui.LtIndex & 0x01) IsOn = 0;//����������
        else IsOn = 1; //˫������
      }
      else IsOn = 1; //0ʱ������ȫ��
    }
    else IsOn = 0;//ͣ��������
  }
  unsigned char Mask = 1 << (Lt4K1Gui.eState - Lt4K1Gui_eMenuSel);  
  if(IsOn) Lt4K1Gui_cbOnLight(Mask);
  else  Lt4K1Gui_cbOffLight(Mask);
}

//------------------------------�˳��˵�����----------------------------
void Lt4K1Gui_QuitMenu(void)
{
  if(Lt4K1Gui.eState >= Lt4K1Gui_eMenuSel)
    Lt4K1Gui_cbQuitGUI();
  Lt4K1Gui.eState = Lt4K1Gui_eIdie;
}

//-----------------------------�õ�ʮ����λ��ֵ------------------------
static const unsigned short _DecBit2Mask[] = {100,10,1};
static unsigned char _GetDecBitNum(unsigned short Num,
                                   unsigned char DecBit)//֧��0,1,2
{
  unsigned char BitNum = Num / _DecBit2Mask[DecBit]; //��Ӧλ�������
  return BitNum % 10;//�õ���
}

//------------------------�˵�ѡ��״̬�����ʼ������---------------------
static void _MenuEnterInit(void)
{
  Lt4K1Gui_cbOnLight(0xff); //��ֵȫ��,
  Lt4K1Gui.Sel = 1;//�Ӳ˵�1��ʼ(���������˵�һ��һ����ʾ����)
  Lt4K1Gui.LtCount = 1;
  Lt4K1Gui.LtIndex = 0; //���³�ʼ��
  Lt4K1Gui.eState = Lt4K1Gui_eMenuSel;//�˵�����
}

//------------------------�˵�ѡ��״̬�л�����---------------------
static void _MenuSelSwitch(void)
{
  if(Lt4K1Gui.Sel < Lt4K1Gui_cbGetMaxMenuSel())
    Lt4K1Gui.Sel++;
  else Lt4K1Gui.Sel = 1; //�ػ���
  Lt4K1Gui.LtCount = Lt4K1Gui.Sel;//������������
  Lt4K1Gui.LtIndex = 0; //��ͷ����
}

//--------------------������л���ֵλ��ʼ������---------------------
static void _NumEnterInit(unsigned char DecBit) //��Ӧ�����Ƶ���λ
{
  unsigned char Bit = 1 << (1 + DecBit);
  Lt4K1Gui_cbOnLight(Bit); //����λ��
  Lt4K1Gui_cbOffLight(~Bit); //����ȫ��
  Lt4K1Gui.LtCount = _GetDecBitNum(Lt4K1Gui.CurVol, DecBit);//��λֵ
  Lt4K1Gui.LtIndex = 0; //���³�ʼ��
  Lt4K1Gui.eState = (enum _Lt4K1Gui_eState)(DecBit + Lt4K1Gui_eNumH);//��Ӧ����λ
}

//------------------------��ֵ���ı亯��-------------------------
static void _NumChange(void)
{
  unsigned char BitPos = Lt4K1Gui.eState - Lt4K1Gui_eNumH;
  unsigned short Vol = Lt4K1Gui.CurVol;
  unsigned char BNum = _GetDecBitNum(Vol, BitPos);//��λֵ
  Vol -= _DecBit2Mask[BitPos] * BNum; //ȥ��λ
  BNum++;//�����ˣ������Ƿ�ػ�
  if(BNum > _GetDecBitNum(Lt4K1Gui_cbGetMaxVol(Lt4K1Gui.Sel), BitPos))
    BNum = _GetDecBitNum(Lt4K1Gui_cbGetMinVol(Lt4K1Gui.Sel), BitPos);
  Lt4K1Gui.CurVol = Vol + _DecBit2Mask[BitPos] * BNum; //�Ӵ�λ
  Lt4K1Gui.LtCount = BNum;//������������
  Lt4K1Gui.LtIndex = 0; //��ͷ����  
}

//----------------------------------������----------------------------
//�������64mS������
void Lt4K1Gui_Task(void)
{
  //===========================�õ���ǰ����״̬=============================
  unsigned char KeyState = 0; //0��Ч,1�̰� 2����
  //��鰴���İ������ɿ�ʱ��
  if(Lt4K1Gui_cbIsKeyDown()){//һֱ���ż�ʱ
    if(Lt4K1Gui.KeyIndex < 255) Lt4K1Gui.KeyIndex++;
  }
  else{//û�����ɿ���
    if((Lt4K1Gui.KeyIndex >= LT4K1_GUI_KEY_VILID) &&  //������Ч��
       (Lt4K1Gui.KeyIndex < (LT4K1_GUI_KEY_LONG * 3))){//����ʱ�䲻�ܹ���
      if(Lt4K1Gui.KeyIndex < LT4K1_GUI_KEY_LONG) KeyState = 1;//�̰�
      else  KeyState = 2;//����
      Lt4K1Gui.QuitIndex = LT4K1_GUI_QUIT_OV;//��Ч����������
      Lt4K1Gui_cbKeyValidNotify(KeyState); //��Ч����ͨ��
    }
    //else KeyState = 0;
    Lt4K1Gui.KeyIndex = 0;//���¼���
  }
  
  //=========================�Զ��˳��˵�����===============================
  unsigned char ePrvState = Lt4K1Gui.eState;
  if(Lt4K1Gui.QuitIndex){
    Lt4K1Gui.QuitIndex--;
    if(!Lt4K1Gui.QuitIndex)  Lt4K1Gui_QuitMenu(); //ʱ�䵽���˳��˵�
  }
  else return; //û�ڲ˵���
  if(KeyState == 0) return; //��ǰ�ް���

  //===========================��Ч��������=============================
  switch(Lt4K1Gui.eState){
    //����״̬��׼��ʶ�𳤰�
    case Lt4K1Gui_eIdie: 
      Lt4K1Gui.eState = (enum _Lt4K1Gui_eState)KeyState;//1,2��Ӧ
      Lt4K1Gui_cbSetCurVol(0 - KeyState, 0);
      break;
    case Lt4K1Gui_eFun0: //����0״̬
      if(KeyState == 2){//��������˵�
        Lt4K1Gui_cbEnterGUI();
        _MenuEnterInit();
      }
      else Lt4K1Gui_QuitMenu(); //�̰��˳��˵�
      break;
    case Lt4K1Gui_eFun1://����1״̬����ֱ���˳��˵�    
      Lt4K1Gui_QuitMenu(); 
      break;
    case Lt4K1Gui_eMenuSel: //�˵�ѡ��״̬����
      if(KeyState == 1) _MenuSelSwitch(); //�̰����˵����л�
      else{//������ֵ�Ľ������λ����
        Lt4K1Gui_cbGetCurVol(Lt4K1Gui.Sel); //����ԭʼֵ  
        _NumEnterInit(0);
      }
    break;
    case Lt4K1Gui_eNumH: 
    case Lt4K1Gui_eNumM:      
    case Lt4K1Gui_eNumL:    
      if(KeyState == 1) _NumChange(); //�̰�����ֵ�������л�
      else{//����ʱ�л���һλ�����λ��������
        unsigned char Pos = Lt4K1Gui.eState + 1;
        if(Pos > Lt4K1Gui_eNumL){//���λ��
          Lt4K1Gui_cbSetCurVol(Lt4K1Gui.Sel, Lt4K1Gui.CurVol);//����
          _MenuEnterInit(); //���½��˵�
        }
        else{//�л���һλ 
          _NumEnterInit(Pos - Lt4K1Gui_eNumH); //��һλ��ʼ��
        }
      }
    default: Lt4K1Gui_QuitMenu(); break;
  }
}



