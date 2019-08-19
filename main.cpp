#include <windows.h>
#include <stdio.h>
 
#include "cneuronet.h"

/*
double GetRandValue(double max_value)
{
 return((static_cast<double>(rand())*max_value)/static_cast<double>(RAND_MAX));
}
 
double NeuronFunction(double value)
{
 return(1.0/(1.0+exp(-value)));
}
 
double NeuronFunctionDifferencial(double value)
{
 return((1.0-NeuronFunction(value))*NeuronFunction(value));
}
 
 
// https://proglib.io/p/neural-nets-guide/
 
*/

//----------------------------------------------------------------------------------------------------
//������� ������� ���������
//----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevstance,LPSTR lpstrCmdLine,int nCmdShow)
{
 try
 {
  CNeuroNet cNeuroNet;
  std::vector<size_t> neuron_in_layers(3);
  neuron_in_layers[0]=10;//������� ����
  neuron_in_layers[1]=10;
  neuron_in_layers[2]=10;//�������� ����
  //������ ���������
  cNeuroNet.Create(neuron_in_layers);
  //������ ������ ��� ��������
  std::vector<std::pair<CVector,CVector>> image(2);
  size_t i=0;

  //������ �����
  image[i]=std::make_pair<CVector,CVector>(CVector(10),CVector(10));
  //������� ������
  image[i].first.SetElement(0,0);
  image[i].first.SetElement(1,1);
  image[i].first.SetElement(2,2);
  image[i].first.SetElement(3,3);
  image[i].first.SetElement(4,4);
  image[i].first.SetElement(5,5);
  image[i].first.SetElement(6,6);
  image[i].first.SetElement(7,7);
  image[i].first.SetElement(8,8);
  image[i].first.SetElement(9,9);
  //�������� ������
  image[i].second.SetElement(0,0.0);
  image[i].second.SetElement(1,0.1);
  image[i].second.SetElement(2,0.2);
  image[i].second.SetElement(3,0.3);
  image[i].second.SetElement(4,0.4);
  image[i].second.SetElement(5,0.5);
  image[i].second.SetElement(6,0.6);
  image[i].second.SetElement(7,0.7);
  image[i].second.SetElement(8,0.8);
  image[i].second.SetElement(9,0.9);
  
  //������ �����
  i++;
  image[i]=std::make_pair<CVector,CVector>(CVector(10),CVector(10));
  //������� ������
  image[i].first.SetElement(0,9);
  image[i].first.SetElement(1,8);
  image[i].first.SetElement(2,7);
  image[i].first.SetElement(3,6);
  image[i].first.SetElement(4,5);
  image[i].first.SetElement(5,4);
  image[i].first.SetElement(6,3);
  image[i].first.SetElement(7,2);
  image[i].first.SetElement(8,1);
  image[i].first.SetElement(9,0);
  //�������� ������
  image[i].second.SetElement(0,0.9);
  image[i].second.SetElement(1,0.8);
  image[i].second.SetElement(2,0.7);
  image[i].second.SetElement(3,0.6);
  image[i].second.SetElement(4,0.5);
  image[i].second.SetElement(5,0.4);
  image[i].second.SetElement(6,0.3);
  image[i].second.SetElement(7,0.2);
  image[i].second.SetElement(8,0.1);
  image[i].second.SetElement(9,0.0);
  //������� ���������
  cNeuroNet.Reset();
  const double speed=0.25;//�������� ��������
  const double max_cost=0.001;//������ �������� ��� ���� ������� (����� ��������� ������)
  const size_t max_iteration=100000;//������������ ���������� �������� ��������
  double cost=cNeuroNet.Training(image,speed,max_cost,max_iteration);

  char str[255];
  sprintf(str,"���������� �������� ��������:%f",cost);
  MessageBox(NULL,str,"",MB_OK);

  //��������� ���������
  CVector cVector_Input(10);
  cVector_Input.SetElement(0,0);
  cVector_Input.SetElement(1,1);
  cVector_Input.SetElement(2,2);
  cVector_Input.SetElement(3,3);
  cVector_Input.SetElement(4,4);
  cVector_Input.SetElement(5,5);
  cVector_Input.SetElement(6,6);
  cVector_Input.SetElement(7,7);
  cVector_Input.SetElement(8,8);
  cVector_Input.SetElement(9,9);

  CVector cVector_Output(10);

  cNeuroNet.GetAnswer(cVector_Input,cVector_Output);

  std::string out;
  //������� ���������
  for(size_t y=0;y<cVector_Output.GetSize();y++)
  {  
   
   sprintf(str,"V[%i]:%f ",y,cVector_Output.GetElement(y));
   out+=str;
  }
  MessageBox(NULL,out.c_str(),"���������",MB_OK);
 }
 catch(const char *text)
 {
  MessageBox(NULL,text,"������",MB_OK);
 }
 exit(EXIT_SUCCESS);
}
 
