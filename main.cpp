#include <windows.h>
#include <stdio.h>
 
#include "cmatrix.h"
#include <stdint.h>
#include <math.h>
#include <vector>
 
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
 
//----------------------------------------------------------------------------------------------------
//главная функция программы
//----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevstance,LPSTR lpstrCmdLine,int nCmdShow)
{
 try
 {
  printf("Start.\r\n");
   
  static const uint32_t layers=3;//число активных слоёв
  static const uint32_t neuron_in_layers[layers]={10,10,10};//количество нейронов в слоях
   
  //слой 0 - входной, слой layers-1 - выходной
  CMatrix cMatrix_W[layers-1];//веса
  CMatrix cMatrix_dW[layers-1];//поправки к весам
  for(uint32_t n=0;n<layers-1;n++)
  {
   cMatrix_W[n]=CMatrix(neuron_in_layers[n+1],neuron_in_layers[n]);
   cMatrix_dW[n]=CMatrix(neuron_in_layers[n+1],neuron_in_layers[n]);
  }
   
  //инициализируем веса
  for(uint32_t n=0;n<layers-1;n++)
  {
   for(uint32_t y=0;y<cMatrix_W[n].GetSizeY();y++)
   {
    for(uint32_t x=0;x<cMatrix_W[n].GetSizeX();x++) cMatrix_W[n].SetElement(y,x,GetRandValue(2.0)-1.0);
   }
  }
  CVector cVector_B[layers-1];//вектора сдвигов
  CVector cVector_dB[layers-1];//поправки к сдвигам
  for(uint32_t n=0;n<layers-1;n++)
  {
   cVector_B[n]=CVector(neuron_in_layers[n+1]);
   cVector_dB[n]=CVector(neuron_in_layers[n+1]);
  }
   
  //инициализируем вектора сдвигов
  for(uint32_t n=0;n<layers-1;n++)
  {
   for(uint32_t y=0;y<cVector_B[n].GetSize();y++) cVector_B[n].SetElement(y,GetRandValue(2.0)-1.0);
  }
  std::vector<CVector> cVector_Image(2,CVector(neuron_in_layers[0]));
  std::vector<CVector> cVector_Result(2,CVector(neuron_in_layers[layers-1]));
  uint32_t i=1;
   
  cVector_Image[i].SetElement(0,0);cVector_Result[i].SetElement(0,0.1*9);
  cVector_Image[i].SetElement(1,1);cVector_Result[i].SetElement(1,0.1*8);
  cVector_Image[i].SetElement(2,2);cVector_Result[i].SetElement(2,0.1*7);
  cVector_Image[i].SetElement(3,3);cVector_Result[i].SetElement(3,0.1*6);
  cVector_Image[i].SetElement(4,4);cVector_Result[i].SetElement(4,0.1*5);
  cVector_Image[i].SetElement(5,5);cVector_Result[i].SetElement(5,0.1*4);
  cVector_Image[i].SetElement(6,6);cVector_Result[i].SetElement(6,0.1*3);
  cVector_Image[i].SetElement(7,7);cVector_Result[i].SetElement(7,0.1*2);
  cVector_Image[i].SetElement(8,8);cVector_Result[i].SetElement(8,0.1*1);
  cVector_Image[i].SetElement(9,9);cVector_Result[i].SetElement(9,0.1*0);
  i=0;
   
  cVector_Image[i].SetElement(0,1);cVector_Result[i].SetElement(0,0.1*1);
  cVector_Image[i].SetElement(1,2);cVector_Result[i].SetElement(1,0.1*2);
  cVector_Image[i].SetElement(2,4);cVector_Result[i].SetElement(2,0.1*4);
  cVector_Image[i].SetElement(3,6);cVector_Result[i].SetElement(3,0.1*6);
  cVector_Image[i].SetElement(4,8);cVector_Result[i].SetElement(4,0.1*8);
  cVector_Image[i].SetElement(5,3);cVector_Result[i].SetElement(5,0.1*3);
  cVector_Image[i].SetElement(6,5);cVector_Result[i].SetElement(6,0.1*5);
  cVector_Image[i].SetElement(7,7);cVector_Result[i].SetElement(7,0.1*7);
  cVector_Image[i].SetElement(8,9);cVector_Result[i].SetElement(8,0.1*9);
  cVector_Image[i].SetElement(9,0);cVector_Result[i].SetElement(9,0.1*0);
   
  CVector cVector_H[layers];//значение после активационной функции
  CVector cVector_Z[layers];//значение до активационной функции
   
  for(uint32_t n=0;n<layers;n++)
  {
   cVector_H[n]=CVector(neuron_in_layers[n]);
   cVector_Z[n]=CVector(neuron_in_layers[n]);
  }
  //обучаем сеть
  uint32_t max_index=2;
  while(1)
  {
   bool done=true;
   //инициализируем поправки к весам и смещениям
   for(uint32_t n=0;n<layers-1;n++)
   {
    cMatrix_dW[n].Zero();
    cVector_dB[n].Zero();
   }
   double cost=0;
   //обучаем сеть
   for(uint32_t index=0;index<max_index;index++)
   {
     
    cVector_H[0]=cVector_Image[index];//входной вектор
    //вычисляем сеть
    for(uint32_t n=0;n<layers-1;n++)
    {
     cVector_Z[n+1]=cMatrix_W[n]*cVector_H[n]+cVector_B[n];
     //применим функцию активации
     for(uint32_t y=0;y<cVector_Z[n+1].GetSize();y++)
     {
      double v=cVector_Z[n+1].GetElement(y);
      cVector_H[n+1].SetElement(y,NeuronFunction(v));
     }
    }
    //вычислим ошибку
    CVector cVector_Error(neuron_in_layers[layers-1]);
    cVector_Error=cVector_Result[index]-cVector_H[layers-1];//вектор ошибок
    //вычисляем функцию стоимости
    for(uint32_t y=0;y<cVector_Error.GetSize();y++) cost+=cVector_Error.GetElement(y)*cVector_Error.GetElement(y);
     
    //определяем дельту последнего слоя
    CVector cVector_Delta[layers];
    for(uint32_t n=0;n<layers;n++) cVector_Delta[n]=CVector(neuron_in_layers[n]);
     
    for(uint32_t y=0;y<cVector_Error.GetSize();y++)
    {
     double v=cVector_Error.GetElement(y);
     double z=cVector_Z[layers-1].GetElement(y);
     cVector_Delta[layers-1].SetElement(y,-v*NeuronFunctionDifferencial(z));
    }
     
    //вычисляем дельты всех слоёв (дельта нулевого слоя не нужна)
    for(int32_t n=layers-2;n>=0;n--)
    {
     cVector_Delta[n]=cMatrix_W[n].Transpose()*cVector_Delta[n+1];
     for(uint32_t y=0;y<cVector_Delta[n].GetSize();y++)
     {
      double v=cVector_Delta[n].GetElement(y);
      double z=cVector_Z[n].GetElement(y);
      cVector_Delta[n].SetElement(y,v*NeuronFunctionDifferencial(z));
     }
    }
    //вычисляем поправки к весам и смещениям
    for(int32_t n=layers-2;n>=0;n--)
    {
     CMatrix cMatrix(cVector_Delta[n+1].GetSize(),cVector_H[n].GetSize());
     cMatrix=(cVector_Delta[n+1])&(cVector_H[n]);
     cMatrix_dW[n]=cMatrix_dW[n]+cMatrix;
      
     cVector_dB[n]=cVector_dB[n]+cVector_Delta[n+1];
    }     
   }
    
   if (cost>0.001)
   {
    static int counter=0;
    if (counter==0)
    {
     printf("cost: %f\r\n",cost);
    }
    counter++;
    counter%=10000;
    done=false;
   }
   if (done==true) break;
    
   double alpha=0.25;
   //выполняем градиентный спуск
   double k=alpha*(1.0/static_cast<double>(max_index));
   for(int32_t n=layers-2;n>=0;n--)
   {
    cMatrix_W[n]=cMatrix_W[n]-k*cMatrix_dW[n];
    cVector_B[n]=cVector_B[n]-k*cVector_dB[n];
   }
    
  }
   
  //выведем результат
  for(uint32_t y=0;y<cVector_H[layers-1].GetSize();y++)
  {
   printf("V[%i]:%f\r\n",y,cVector_H[layers-1].GetElement(y));
  }
 }
 catch(const char *text)
 {
  printf("Throw: %s\r\n",text);
 }
  
  
 printf("End.\r\n");
 return(0);
}
 
