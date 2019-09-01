#include <windows.h>
#include <stdio.h>
 
#include "cneuronet.h"
#include "cneuronettrainer.h"
#include "idatastream.h"

/*
// https://proglib.io/p/neural-nets-guide/
*/

//----------------------------------------------------------------------------------------------------
//главная функция программы
//----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevstance,LPSTR lpstrCmdLine,int nCmdShow)
{
 try
 {
  std::shared_ptr<CNeuroNet> cNeuroNet_Ptr(new CNeuroNet);
  std::unique_ptr<CNeuroNetTrainer> cNeuroNetTrainer_Ptr(new CNeuroNetTrainer);
  std::vector<size_t> neuron_in_layers(3);
  neuron_in_layers[0]=10;//входной слой
  neuron_in_layers[1]=5;
  neuron_in_layers[2]=10;//выходной слой
  //создаём нейросеть
  cNeuroNet_Ptr->Create(neuron_in_layers);
  //подключаем класс для тренировки нейросети
  cNeuroNetTrainer_Ptr->Connect(cNeuroNet_Ptr);
  //создаём образы для обучения
  std::vector<std::pair<CVector,CVector>> image(2);
  size_t i=0;

  //первый образ
  image[i]=std::make_pair<CVector,CVector>(CVector(10),CVector(10));
  //входной вектор
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
  //выходной вектор
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
  
  //второй образ
  i++;
  image[i]=std::make_pair<CVector,CVector>(CVector(10),CVector(10));
  //входной вектор
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
  //выходной вектор
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
  //обучаем нейросеть
  cNeuroNet_Ptr->Reset();

  const double speed=0.25;//скорость обучения
  const double max_cost=0.001;//ошибка обучения для всех образов (сумма квадратов ошибки)
  const size_t max_iteration=100000;//максимальное количество итераций обучения
  double cost=cNeuroNetTrainer_Ptr->Training(image,speed,max_cost,max_iteration);

  char str[255];
  sprintf(str,"Достигнута точность обучения:%f",cost);
  MessageBox(NULL,str,"",MB_OK);

  //вычисляем нейросеть
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

  cNeuroNet_Ptr->SetInput(cVector_Input);
  cNeuroNet_Ptr->Forward();
  cNeuroNet_Ptr->GetOutput(cVector_Output);

  std::string out;
  //выведем результат
  for(size_t y=0;y<cVector_Output.GetSize();y++)
  {  
   
   sprintf(str,"V[%i]:%f ",y,cVector_Output.GetElement(y));
   out+=str;
  }
  MessageBox(NULL,out.c_str(),"Результат",MB_OK);

  cNeuroNet_Ptr->Export("net.txt");
  //сохраним нейросеть
  std::unique_ptr<IDataStream> iDataStream_Ptr(IDataStream::CreateNewDataStreamFile("neuronet.net",true));
  cNeuroNet_Ptr->Save(iDataStream_Ptr.get());
 }
 catch(const char *text)
 {
  MessageBox(NULL,text,"Ошибка",MB_OK);
 }
 
 exit(EXIT_SUCCESS);
}
 
