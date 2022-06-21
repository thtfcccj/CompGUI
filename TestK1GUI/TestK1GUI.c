/*************************************************************************

                ��һ�Լ��GUIʵ��

**************************************************************************/
#include "TestK1GUI.h"
#include <string.h>

struct _TestK1GUI TestK1GUI;

//----------------------------------��ʼ������--------------------------
void TestK1GUI_Init(void)
{
  memset(&TestK1GUI, 0, sizeof(struct _TestK1GUI));
}

//------------------------------�˳��˵�����----------------------------
void TestK1GUI_QuitMenu(void)
{
  //if(TestK1GUI.eState >= TestK1GUI_eMenuSel)
  TestK1GUI_cbQuitMenu();
  TestK1GUI.QuitIndex = 0;
  TestK1GUI.eState = TestK1GUI_eIdie;
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
  TestK1GUI.eState = TestK1GUI_eMenuSel;//�˵�����
}

//------------------------�˵�ѡ��״̬�л�����---------------------
static void _MenuSelSwitch(void)
{
  if(TestK1GUI.Sel < TestK1GUI_cbGetMaxMenuSel())
    TestK1GUI.Sel++;
  else TestK1GUI.Sel = 0; //�ػ���
}

//--------------------������л���ֵλ��ʼ������---------------------
static void _NumEnterInit(unsigned char DecBit) //��Ӧ�����Ƶ���λ
{
  unsigned char Bit = 1 << (1 + DecBit);
  //�л�ʱ�Զ��߽����
  unsigned short Max = TestK1GUI_cbGetMaxVol(TestK1GUI.Sel);
  unsigned short Vol = TestK1GUI.CurVol;  
  if(Vol > Max) Vol = Max;
  else{
    unsigned short Min = TestK1GUI_cbGetMinVol(TestK1GUI.Sel);
    if(Vol < Min) Vol = Min;
  }
  TestK1GUI.CurVol = Vol;
  TestK1GUI.eState = (enum _TestK1GUI_eState)(DecBit + TestK1GUI_eNumH);//��Ӧ����λ
}

//------------------------��ֵ���ı亯��-------------------------
static void _NumChange(void)
{
  unsigned char BitPos = TestK1GUI.eState - TestK1GUI_eNumH;
  unsigned short Vol = TestK1GUI.CurVol;
  unsigned char BNum = _GetDecBitNum(Vol, BitPos);//��λֵ
  unsigned short DecMask = _DecBit2Mask[BitPos];
  Vol -= DecMask * BNum; //ȥ��λ
  BNum++;//�����ˣ������Ƿ�ػ�
  unsigned short Max = TestK1GUI_cbGetMaxVol(TestK1GUI.Sel);
  if(BNum >= 10){//�ػ���
    if(Max / (DecMask * 10)) BNum = 0; //��λ��ֵʱ�ɵ���
    else{//�޸�λʱ����λֻ��ȡ���
      BNum = _GetDecBitNum(TestK1GUI_cbGetMinVol(TestK1GUI.Sel), BitPos);
    }
  }
  else if((Max / DecMask) < 10){//������������
    if(BNum > _GetDecBitNum(Max, BitPos))
      BNum = _GetDecBitNum(TestK1GUI_cbGetMinVol(TestK1GUI.Sel), BitPos);
  }
  TestK1GUI.CurVol = Vol + DecMask * BNum; //�Ӵ�λ
}

//----------------------------------������----------------------------
//�������16mS��32ms������
void TestK1GUI_Task(void)
{
  //===========================�õ���ǰ����״̬=============================
  unsigned char KeyState = 0; //0��Ч,1�̰� 2����
  //��鰴���İ������ɿ�ʱ��
  if(TestK1GUI_cbIsKeyDown()){//һֱ���ż�ʱ
    if(TestK1GUI.KeyIndex < (TEST_K1_GUI_KEY_OV - 1)) TestK1GUI.KeyIndex++;
    #ifdef SUPPORT_TEST_K1_GUI_ACT_FUN3     //�Լ��ڼ�һֱ ��סΪ����3
      else if(TestK1GUI.KeyIndex == (TEST_K1_GUI_KEY_OV - 1)){
        TestK1GUI.KeyIndex = TEST_K1_GUI_KEY_OV;//����
        if(TestK1GUI.eState == TestK1GUI_eTest) //���빦��3
          TestK1GUI_cbSetCurVol(-3, 0); 
      }
    #endif
    //ֱ���赽����
    if(TestK1GUI.KeyIndex != TEST_K1_GUI_KEY_LONG) return;
    KeyState = 2;//������
  }
  else{//û�����ɿ���
    unsigned char KeyIndex = TestK1GUI.KeyIndex;
    TestK1GUI.KeyIndex = 0;//���¼���
    if((KeyIndex >= TEST_K1_GUI_KEY_VILID) &&  //������Ч��
       (KeyIndex < TEST_K1_GUI_KEY_LONG)){//Ϊ�̰���
       KeyState = 1;//�̰���
    }
    else return; //��Ч
  }
  //������Ч��
  TestK1GUI.QuitIndex = TEST_K1_GUI_QUIT_OV;//��Ч����������
  if(TestK1GUI_cbKeyValidNotify(KeyState)) return; //��Ч����ͨ��

  //===========================��Ч��������=============================
  enum _TestK1GUI_eState ePrv = TestK1GUI.eState;
  switch(TestK1GUI.eState){
    //����״̬��׼��ʶ�𳤶̰�
    case TestK1GUI_eIdie: 
      if(KeyState == 1){ //�̰������Լ�״̬
        TestK1GUI.eState = TestK1GUI_eTest;
        TestK1GUI_cbSetCurVol(-1, 0); 
      }
      else{//����Ϊ�˵�׼��
        TestK1GUI.eState = TestK1GUI_eMenuRdy;
      }
      break;
    case TestK1GUI_eTest: //�Լ�״̬
      if(KeyState == 2)//���������Լ����״̬
        TestK1GUI_cbSetCurVol(-2, 0);             
      else TestK1GUI_cbSetCurVol(-4, 0); //�̰�����4(�˳��Լ�����ָ�)
      break;
    case TestK1GUI_eMenuRdy://�˵�׼��״̬
      if(KeyState == 1){//�̰�����˵�
        TestK1GUI_cbEnterMenu();
        TestK1GUI.Sel = 0;//�Ӳ˵�0��ʼ����ֱ���˳�
        _MenuEnterInit();
      }
      else TestK1GUI_QuitMenu(); //�����˳��˵�
      break;
    case TestK1GUI_eMenuSel: //�˵�ѡ��״̬����
      if(KeyState == 1) _MenuSelSwitch(); //�̰����˵����л�
      else{//������ֵ�Ľ������λ����
        if(TestK1GUI.Sel == 0){//�����˳��˵�
          TestK1GUI_QuitMenu(); 
          return;
        }
        //�ȼ���ԭʼֵ���������λ
        TestK1GUI.CurVol = TestK1GUI_cbGetCurVol(TestK1GUI.Sel); 
        //Ϊֻ��״̬��ָ��
        if(TestK1GUI_cbGetMaskVol(TestK1GUI.Sel) == 0){
          TestK1GUI_cbSetCurVol(TestK1GUI.Sel, 0);//ͨ���û�ִ��
          break;
        }
        unsigned short Mask = TestK1GUI_cbGetMaskVol(TestK1GUI.Sel);
        if(Mask & 0x40) _NumEnterInit(0);//�����λ
        else if(Mask & 0x20) _NumEnterInit(1);//���м�λ
        else _NumEnterInit(2);//ֻ�����λ
      }
    break;
    case TestK1GUI_eNumH: 
    case TestK1GUI_eNumM:      
    case TestK1GUI_eNumL:    
      if(KeyState == 1) _NumChange(); //�̰�����ֵ�������л�
      else{//����ʱ�л���һλ�����λ��������
        unsigned char Pos = TestK1GUI.eState + 1;
        if((Pos > TestK1GUI_eNumL) || //���λ��
          !(TestK1GUI_cbGetMaskVol(TestK1GUI.Sel) & 0x01)){//���λ��ֹ����ʱ
          TestK1GUI_cbSetCurVol(TestK1GUI.Sel, TestK1GUI.CurVol);//����
          _MenuEnterInit(); //���½��˵�
        }
        else{//�л���һλ 
          _NumEnterInit(Pos - TestK1GUI_eNumH); //��һλ��ʼ��
        }
      }
    break;
    default: TestK1GUI_QuitMenu(); break;
  }
  
  TestK1GUI_cbUpdateDisp(ePrv);//������ʾ
}



