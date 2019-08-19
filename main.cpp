#include <windows.h>
#include <stdio.h>

#include "cmatrix.h"
#include <stdint.h>
#include <math.h>

//тест на равенство значений
#define CUNITTEST_CHECK_EQU(a,b) {if (a!=b) printf("%s==%s is ERROR!\r\n",#a,#b);}

void UnitTest(void);

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
 UnitTest();

 static const uint32_t layers=2;//число слоёв
 static const uint32_t neuron=10;//количество нейронов в слое

 //слой 0 - входной

 CMatrix<neuron,neuron> cMatrix_W[layers];
 //инициализируем веса
 for(uint32_t n=0;n<layers;n++)
 {
  for(uint32_t y=0;y<neuron;y++)
  {
   for(uint32_t x=0;x<neuron;x++) cMatrix_W[n].SetElement(y,x,GetRandValue(2.0)-1.0);
  }
 }

 CVector<neuron> cVector_B[layers];//вектора сдвигов
 //инициализируем вектора сдвигов
 for(uint32_t n=0;n<layers;n++)
 {
  for(uint32_t y=0;y<neuron;y++) cVector_B[n].SetElement(y,GetRandValue(2.0)-1.0);
 }

 CVector<neuron> cVector_Image[2];
 CVector<neuron> cVector_Result[2];

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

 CVector<neuron> cVector_H[layers+1];//значение после активационной функции
 CVector<neuron> cVector_Z[layers+1];//значение до активационной функции

 //обучаем сеть
 uint32_t max_index=2;
 while(1)
 {
  bool done=true;

 //инициализируем поправки к весам и смещениям
 CMatrix<neuron,neuron> cMatrix_dW[layers];
 CVector<neuron> cVector_dB[layers];
 for(uint32_t n=0;n<layers;n++)
 {
  cMatrix_dW[n].Zero();
  cVector_dB[n].Zero();
 }

 double cost=0;
 //обучаем сеть
 for(uint32_t index=0;index<max_index;index++)
 {

 cVector_H[0]=cVector_Image[index];//выходной вектор
 //вычисляем сеть
 for(uint32_t n=0;n<layers;n++)
 {
  cVector_Z[n+1]=cMatrix_W[n]*cVector_H[n]+cVector_B[n];
  //применим функцию активации
  for(uint32_t y=0;y<neuron;y++)
  {
   double v=cVector_Z[n+1].GetElement(y);
   cVector_H[n+1].SetElement(y,NeuronFunction(v));
  }
 }

 //вычислим ошибку
 CVector<neuron> cVector_Error=cVector_Result[index]-cVector_H[layers];//вектор ошибок
 //вычисляем функцию стоимости
 for(uint32_t y=0;y<neuron;y++) cost+=cVector_Error.GetElement(y)*cVector_Error.GetElement(y);


 //определяем дельту последнего слоя
 CVector<neuron> cVector_Delta[layers+1];//вектор дельты
 for(uint32_t y=0;y<neuron;y++)
 {
  double v=cVector_Error.GetElement(y);
  double z=cVector_Z[layers].GetElement(y);
  cVector_Delta[layers].SetElement(y,-v*NeuronFunctionDifferencial(z));
 }

 //вычисляем дельты всех слоёв (дельта нулевого слоя не нужна)
 for(int32_t n=layers-1;n>=0;n--)
 {
  cVector_Delta[n]=cMatrix_W[n].Transpose()*cVector_Delta[n+1];
  for(uint32_t y=0;y<neuron;y++)
  {
   double v=cVector_Delta[n].GetElement(y);
   double z=cVector_Z[n].GetElement(y);
   cVector_Delta[n].SetElement(y,v*NeuronFunctionDifferencial(z));
  }
 }
 //вычисляем поправки к весам и смещениям
 for(int32_t n=layers-1;n>=0;n--)
 {
  CMatrix<neuron,neuron> cMatrix=(cVector_Delta[n+1])&(cVector_H[n]);
  cMatrix_dW[n]=cMatrix_dW[n]+cMatrix;

  cVector_dB[n]=cVector_dB[n]+cVector_Delta[n+1];
 }

 }

 if (cost>0.001)
 {
  static int counter=0;
  if (counter==0)
  {
   printf("%f\r\n",cost);
  }
  counter++;
  counter%=10000;
  done=false;
 }
 if (done==true) break;

 double alpha=0.25;
 //выполняем градиентный спуск
 double k=alpha*(1.0/static_cast<double>(max_index));
 for(int32_t n=layers-1;n>=0;n--)
 {
  cMatrix_W[n]=cMatrix_W[n]-k*cMatrix_dW[n];
  cVector_B[n]=cVector_B[n]-k*cVector_dB[n];
 }

 }

 //выведем результат
 FILE *file=fopen("out.txt","wb");
 for(uint32_t y=0;y<neuron;y++)
 {
  fprintf(file,"%f\r\n",cVector_H[layers].GetElement(y));
 }
 fclose(file);
 

 MessageBox(NULL,"Обработка завершена","Завершено",MB_OK);
 return(0);
}

void UnitTest(void)
{
 CMatrix<2,2> cMatrixA;
 CMatrix<2,2> cMatrixB;
 CMatrix<2,2> cMatrixC;

 cMatrixA.SetElement(0,0,1);
 cMatrixA.SetElement(0,1,2);
 cMatrixA.SetElement(1,0,3);
 cMatrixA.SetElement(1,1,4);

 cMatrixB.SetElement(0,0,1);
 cMatrixB.SetElement(0,1,2);
 cMatrixB.SetElement(1,0,3);
 cMatrixB.SetElement(1,1,4);

 //тест 1
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(0,0),1);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(0,1),2);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(1,0),3);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(1,1),4);

 //тест 2
 cMatrixA=cMatrixA*2;
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(0,0),2);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(0,1),4);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(1,0),6);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(1,1),8);
 //тест 3
 cMatrixA=2*cMatrixA;
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(0,0),4);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(0,1),8);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(1,0),12);
 CUNITTEST_CHECK_EQU(cMatrixA.GetElement(1,1),16);

 //тест 4

 cMatrixC=cMatrixA*cMatrixB;
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(0,0),28);
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(0,1),40);
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(1,0),60);
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(1,1),88);


 //тест 5
 cMatrixC=cMatrixA-cMatrixB;
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(0,0),3);
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(0,1),6);
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(1,0),9);
 CUNITTEST_CHECK_EQU(cMatrixC.GetElement(1,1),12);
}