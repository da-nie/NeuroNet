//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "cneuronet.h"
#include <fstream>

//****************************************************************************************************
//глобальные переменные
//****************************************************************************************************

//****************************************************************************************************
//константы
//****************************************************************************************************

//****************************************************************************************************
//макроопределения
//****************************************************************************************************

//****************************************************************************************************
//конструктор и деструктор
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CNeuroNet::CNeuroNet(void)
{ 
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CNeuroNet::~CNeuroNet()
{
}

//****************************************************************************************************
//закрытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//случайное число
//----------------------------------------------------------------------------------------------------
double CNeuroNet::GetRandValue(double max_value)
{
 return((static_cast<double>(rand())*max_value)/static_cast<double>(RAND_MAX));
}
//----------------------------------------------------------------------------------------------------
//функция активации нейрона
//----------------------------------------------------------------------------------------------------
double CNeuroNet::NeuronFunction(double value)
{
 return(1.0/(1.0+exp(-value)));
}
//----------------------------------------------------------------------------------------------------
//производная функции активации нейрона
//----------------------------------------------------------------------------------------------------
double CNeuroNet::NeuronFunctionDifferencial(double value)
{
 return((1.0-NeuronFunction(value))*NeuronFunction(value));
}

//****************************************************************************************************
//открытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//создать нейросеть
//----------------------------------------------------------------------------------------------------
void CNeuroNet::Create(const std::vector<size_t> &neuron_in_layers)
{
 size_t layers=neuron_in_layers.size();

 if (layers<2) return;
 NeuronInLayers=neuron_in_layers;

 cMatrix_W.resize(layers-1);
 cMatrix_dW.resize(layers-1);

 cVector_B.resize(layers-1);
 cVector_dB.resize(layers-1);

 for(size_t n=0;n<layers-1;n++)
 {
  cMatrix_W[n]=CMatrix(NeuronInLayers[n+1],NeuronInLayers[n]);
  cMatrix_dW[n]=CMatrix(NeuronInLayers[n+1],NeuronInLayers[n]);

  cVector_B[n]=CVector(NeuronInLayers[n+1]);
  cVector_dB[n]=CVector(NeuronInLayers[n+1]);
 }

 cVector_H.resize(layers);
 cVector_Z.resize(layers);
 cVector_Delta.resize(layers);

 for(size_t n=0;n<layers;n++)
 {
  cVector_H[n]=CVector(NeuronInLayers[n]);
  cVector_Z[n]=CVector(NeuronInLayers[n]);

  cVector_Delta[n]=CVector(NeuronInLayers[n]);
 }

 cVector_Error=CVector(NeuronInLayers[layers-1]);

 Reset();
}
//----------------------------------------------------------------------------------------------------
//привести нейросеть в исходное состояние
//----------------------------------------------------------------------------------------------------
void CNeuroNet::Reset(void)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return;
 //инициализируем веса и сдвиги
 for(size_t n=0;n<layers-1;n++)
 {
  //веса
  for(size_t y=0;y<cMatrix_W[n].GetSizeY();y++)
  {
   for(size_t x=0;x<cMatrix_W[n].GetSizeX();x++) cMatrix_W[n].SetElement(y,x,GetRandValue(2.0)-1.0);
  }
  //сдвиги
  for(size_t y=0;y<cVector_B[n].GetSize();y++) cVector_B[n].SetElement(y,GetRandValue(2.0)-1.0);
 }
}
//----------------------------------------------------------------------------------------------------
//обучить нейросеть
//----------------------------------------------------------------------------------------------------
double CNeuroNet::Training(const std::vector<std::pair<CVector,CVector>> &image,double speed,double max_cost,size_t max_iteration)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return(0);

 double cost=0;
 size_t training_amount=image.size();

 double k=speed*(1.0/static_cast<double>(training_amount));

 while(max_iteration>0)
 {
  max_iteration--;
  //инициализируем поправки к весам и смещениям
  for(size_t n=0;n<layers-1;n++)
  {
   cMatrix_dW[n].Zero();
   cVector_dB[n].Zero();
  }
  cost=0;
  //обучаем сеть
  for(size_t index=0;index<training_amount;index++)
  {
   const std::pair<CVector,CVector> &pair=image[index];
   const CVector &cVector_Input=pair.first;
   const CVector &cVector_Output=pair.second;

   cVector_H[0]=cVector_Input;//входной вектор
   //вычисляем сеть
   for(size_t n=0;n<layers-1;n++)
   {
    cVector_Z[n+1]=cMatrix_W[n]*cVector_H[n]+cVector_B[n];
    //применим функцию активации
    for(size_t y=0;y<cVector_Z[n+1].GetSize();y++)
    {
     double v=cVector_Z[n+1].GetElement(y);
     cVector_H[n+1].SetElement(y,NeuronFunction(v));
    }
   }
   //вычислим ошибку   
   cVector_Error=cVector_Output-cVector_H[layers-1];//вектор ошибок
   //вычисляем функцию стоимости
   for(size_t y=0;y<cVector_Error.GetSize();y++) cost+=cVector_Error.GetElement(y)*cVector_Error.GetElement(y);
   
   //определяем дельту последнего слоя
   for(size_t y=0;y<cVector_Error.GetSize();y++)
   {
    double v=cVector_Error.GetElement(y);
    double z=cVector_Z[layers-1].GetElement(y);
    cVector_Delta[layers-1].SetElement(y,-v*NeuronFunctionDifferencial(z));
   }
   //вычисляем дельты всех слоёв (дельта нулевого слоя не нужна)
   for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
   {
    cVector_Delta[n]=cMatrix_W[n].Transpose()*cVector_Delta[n+1];
    for(size_t y=0;y<cVector_Delta[n].GetSize();y++)
    {
     double v=cVector_Delta[n].GetElement(y);
     double z=cVector_Z[n].GetElement(y);
     cVector_Delta[n].SetElement(y,v*NeuronFunctionDifferencial(z));
    }
   }
   //вычисляем поправки к весам и смещениям
   for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
   {
    CMatrix cMatrix(cVector_Delta[n+1].GetSize(),cVector_H[n].GetSize());
    cMatrix=(cVector_Delta[n+1])&(cVector_H[n]);
    cMatrix_dW[n]=cMatrix_dW[n]+cMatrix;
     
    cVector_dB[n]=cVector_dB[n]+cVector_Delta[n+1];
   }     
  }
  if (cost<=max_cost) break;//обучение окончено

  //выполняем градиентный спуск
  for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
  {
   cMatrix_W[n]=cMatrix_W[n]-k*cMatrix_dW[n];
   cVector_B[n]=cVector_B[n]-k*cVector_dB[n];
  }
 }
 return(cost);
}
//----------------------------------------------------------------------------------------------------
//вычислить результат работы нейросети
//----------------------------------------------------------------------------------------------------
void CNeuroNet::GetAnswer(const CVector &input,CVector &output)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return;
 cVector_H[0]=input;//входной вектор
 //вычисляем сеть
 for(size_t n=0;n<layers-1;n++)
 {
  cVector_Z[n+1]=cMatrix_W[n]*cVector_H[n]+cVector_B[n];
  //применим функцию активации
  for(size_t y=0;y<cVector_Z[n+1].GetSize();y++)
  {
   double v=cVector_Z[n+1].GetElement(y);
   cVector_H[n+1].SetElement(y,NeuronFunction(v));
  }
 }
 output=cVector_H[layers-1];
}
//----------------------------------------------------------------------------------------------------
//экспортировать нейросеть
//----------------------------------------------------------------------------------------------------
bool CNeuroNet::Export(const std::string &file_name)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return(false);

 /*
 FILE *file=fopen(file_name.c_str(),"wb");
 
 fprintf(file,"//****************************************************************************************************"<<std::endl);
 fprintf(file,"//подключаемые библиотеки"<<std::endl);
 fprintf(file,"//****************************************************************************************************"<<std::endl);
 fprintf(file,"#include <stdio.h>"<<std::endl);
 fprintf(file,"#include <math.h>"<<std::endl);
 fprintf(file,""<<std::endl);

 fprintf(file,"//****************************************************************************************************"<<std::endl);
 fprintf(file,"//константы"<<std::endl);
 fprintf(file,"//****************************************************************************************************"<<std::endl);

 for(size_t n=0;n<layers-1;n++)
 {
  size_t row=cMatrix_W[n].GetSizeY();
  size_t col=cMatrix_W[n].GetSizeX();
  fprintf(file,"//Матрица весов связей слоя %ld со слоем %ld"<<std::endl,n,n+1);
  fprintf(file,"static const float MatrixW%ld[%ld*%ld]="<<std::endl,n,row,col);
  fprintf(file,"{"<<std::endl);
  for(size_t r=0;r<row;r++)
  {
   fprintf(file," ");
   for(size_t c=0;c<col;c++) 
   {
	fprintf(file,"%f",cMatrix_W[n].GetElement(r,c));
	if (r!=row-1 || c!=col-1) fprintf(file,",");
   }   
   fprintf(file,""<<std::endl);
  }
  fprintf(file,"};"<<std::endl);

  fprintf(file,"//Вектор сдвигов связей слоя %ld со слоем %ld"<<std::endl,n,n+1);
  row=cVector_B[n].GetSize();
  fprintf(file,"static const float VectorB%ld[%ld]="<<std::endl,n,row);
  fprintf(file,"{"<<std::endl);
  for(size_t r=0;r<row;r++)
  {
   fprintf(file," %f",cVector_B[n].GetElement(r));
   if (r!=row-1) fprintf(file,",");
   fprintf(file,""<<std::endl);
  }
  fprintf(file,"};"<<std::endl);
  fprintf(file,"//----------------------------------------------------------------------------------------------------"<<std::endl);
 }

 fprintf(file,"//Количество нейронов в слоях"<<std::endl);
 fprintf(file,"static const size_t NeuronInLayer[%ld]="<<std::endl,layers);
 fprintf(file,"{"<<std::endl);
 for(size_t n=0;n<layers;n++)
 {
  fprintf(file," %ld",NeuronInLayers[n]);
  if (n!=layers-1) fprintf(file,",");
  fprintf(file,""<<std::endl);
 }
 fprintf(file,"};"<<std::endl);
 fprintf(file,"//Массив указателей на матрицы связей"<<std::endl);
 fprintf(file,"static const float *MatrixWPtr[%ld]="<<std::endl,layers-1);
 fprintf(file,"{"<<std::endl);
 for(size_t n=0;n<layers-1;n++)
 {
  fprintf(file," MatrixW%ld",n);
  if (n!=layers-2) fprintf(file,",");
  fprintf(file,""<<std::endl);
 }
 fprintf(file,"};"<<std::endl);

 fprintf(file,"//Массив указателей на вектора сдвигов"<<std::endl);
 fprintf(file,"static const float *VectorBPtr[%ld]="<<std::endl,layers-1);
 fprintf(file,"{"<<std::endl);
 for(size_t n=0;n<layers-1;n++)
 {
  fprintf(file," VectorB%ld",n);
  if (n!=layers-2) fprintf(file,",");
  fprintf(file,""<<std::endl);
 }
 fprintf(file,"};"<<std::endl);

 fprintf(file,""<<std::endl);
 fprintf(file,"//****************************************************************************************************"<<std::endl);
 fprintf(file,"//глобальные переменные"<<std::endl);
 fprintf(file,"//****************************************************************************************************"<<std::endl);

 for(size_t n=0;n<layers;n++)
 {
  fprintf(file,"//Вектор состояния нейронов для слоя %ld"<<std::endl,n);
  size_t row=cVector_H[n].GetSize();
  fprintf(file,"static float VectorH%ld[%ld];"<<std::endl,n,row);
 }
 fprintf(file,"//Массив указателей на вектора состояний"<<std::endl);

 fprintf(file,"static float * const VectorHPtr[%ld]="<<std::endl,layers);
 fprintf(file,"{"<<std::endl);
 for(size_t n=0;n<layers;n++)
 {
  fprintf(file," VectorH%ld",n);
  if (n!=layers-1) fprintf(file,",");
  fprintf(file,""<<std::endl);
 }
 fprintf(file,"};"<<std::endl);
 
 //функция активации
 fprintf(file,""<<std::endl);
 fprintf(file,"//****************************************************************************************************"<<std::endl);
 fprintf(file,"//функции"<<std::endl);
 fprintf(file,"//****************************************************************************************************"<<std::endl);

 fprintf(file,"//----------------------------------------------------------------------------------------------------"<<std::endl);
 fprintf(file,"//функция активации нейрона"<<std::endl);
 fprintf(file,"//----------------------------------------------------------------------------------------------------"<<std::endl);

 fprintf(file,"static float NeuronFunction(float value)"<<std::endl);
 fprintf(file,"{"<<std::endl);
 fprintf(file," return(1.0f/(1.0f+(float)(exp(-value))));"<<std::endl);
 fprintf(file,"}"<<std::endl);
 fprintf(file,""<<std::endl);

 fprintf(file,"//----------------------------------------------------------------------------------------------------"<<std::endl);
 fprintf(file,"//функция расчёта нейросети"<<std::endl);
 fprintf(file,"//----------------------------------------------------------------------------------------------------"<<std::endl);

 fprintf(file,"void CreateAnswer(void)"<<std::endl);
 fprintf(file,"{"<<std::endl);
 fprintf(file," for(size_t n=0;n<%ld;n++)"<<std::endl,layers-1);
 fprintf(file," {"<<std::endl);
 fprintf(file,"  float *vector_h_current=VectorHPtr[n];"<<std::endl);
 fprintf(file,"  float *vector_h_next=VectorHPtr[n+1];"<<std::endl);
 fprintf(file,"  const float *vector_b_current=VectorBPtr[n];"<<std::endl);
 fprintf(file,"  const float *matrix_w_current=MatrixWPtr[n];"<<std::endl);
 fprintf(file,""<<std::endl);
 fprintf(file,"  size_t mx=NeuronInLayer[n];//размер матрицы по x"<<std::endl);
 fprintf(file,"  size_t my=NeuronInLayer[n+1];//размер матрицы по y"<<std::endl);
 fprintf(file,""<<std::endl);
 fprintf(file,"  //умножается строка матрицы на столбец вектора, добавляется вектор смещения и применяется функция активации нейрона"<<std::endl);
 fprintf(file,"  for(size_t y=0;y<my;y++)"<<std::endl);
 fprintf(file,"  {"<<std::endl);
 fprintf(file,"   float value=0;"<<std::endl);
 fprintf(file,"   for(size_t x=0;x<mx;x++)"<<std::endl);
 fprintf(file,"   {"<<std::endl);
 fprintf(file,"    float m_value=matrix_w_current[mx*y+x];"<<std::endl);
 fprintf(file,"    float v_value=vector_h_current[x];"<<std::endl);
 fprintf(file,"    value+=m_value*v_value;"<<std::endl);
 fprintf(file,"   }"<<std::endl);
 fprintf(file,"   vector_h_next[y]=NeuronFunction(value+vector_b_current[y]);"<<std::endl);
 fprintf(file,"  }"<<std::endl);
 fprintf(file," }"<<std::endl);
 fprintf(file,"}"<<std::endl);

 fclose(file);
 return(true);
 */

 std::ofstream ofile;
 ofile.open(file_name,std::ios_base::out|std::ios_base::binary);
 if (ofile.is_open()==false) return(false);
  
 ofile<<"//****************************************************************************************************"<<std::endl;
 ofile<<"//подключаемые библиотеки"<<std::endl;
 ofile<<"//****************************************************************************************************"<<std::endl;
 ofile<<"#include <stdio.h>"<<std::endl;
 ofile<<"#include <math.h>"<<std::endl;
 ofile<<""<<std::endl;

 ofile<<"//****************************************************************************************************"<<std::endl;
 ofile<<"//константы"<<std::endl;
 ofile<<"//****************************************************************************************************"<<std::endl;

 for(size_t n=0;n<layers-1;n++)
 {
  size_t row=cMatrix_W[n].GetSizeY();
  size_t col=cMatrix_W[n].GetSizeX();
  ofile<<"//Матрица весов связей слоя "<<n<<" со слоем "<<(n+1)<<std::endl;
  ofile<<"static const float MatrixW"<<n<<"["<<row<<"*"<<col<<"]="<<std::endl;
  ofile<<"{"<<std::endl;
  for(size_t r=0;r<row;r++)
  {
   ofile<<" ";
   for(size_t c=0;c<col;c++) 
   {
	ofile<<cMatrix_W[n].GetElement(r,c);
	if (r!=row-1 || c!=col-1) ofile<<",";
   }   
   ofile<<std::endl;
  }
  ofile<<"};"<<std::endl;

  ofile<<"//Вектор сдвигов связей слоя "<<n<<" со слоем "<<(n+1)<<std::endl;
  row=cVector_B[n].GetSize();
  ofile<<"static const float VectorB"<<n<<"["<<row<<"]="<<std::endl;
  ofile<<"{"<<std::endl;
  for(size_t r=0;r<row;r++)
  {
   ofile<<" "<<cVector_B[n].GetElement(r);
   if (r!=row-1) ofile<<",";
   ofile<<std::endl;
  }
  ofile<<"};"<<std::endl;
  ofile<<"//----------------------------------------------------------------------------------------------------"<<std::endl;
 }

 ofile<<"//Количество нейронов в слоях"<<std::endl;
 ofile<<"static const size_t NeuronInLayer["<<layers<<"]="<<std::endl;
 ofile<<"{"<<std::endl;
 for(size_t n=0;n<layers;n++)
 {
  ofile<<" "<<NeuronInLayers[n];
  if (n!=layers-1) ofile<<",";
  ofile<<std::endl;
 }
 ofile<<"};"<<std::endl;
 ofile<<"//Массив указателей на матрицы связей"<<std::endl;
 ofile<<"static const float *MatrixWPtr["<<(layers-1)<<"]="<<std::endl;
 ofile<<"{"<<std::endl;
 for(size_t n=0;n<layers-1;n++)
 {
  ofile<<" MatrixW"<<n;
  if (n!=layers-2) ofile<<",";
  ofile<<std::endl;
 }
 ofile<<"};"<<std::endl;

 ofile<<"//Массив указателей на вектора сдвигов"<<std::endl;
 ofile<<"static const float *VectorBPtr["<<(layers-1)<<"]="<<std::endl;
 ofile<<"{"<<std::endl;
 for(size_t n=0;n<layers-1;n++)
 {
  ofile<<" VectorB"<<n;
  if (n!=layers-2) ofile<<",";
  ofile<<std::endl;
 }
 ofile<<"};"<<std::endl;

 ofile<<""<<std::endl;
 ofile<<"//****************************************************************************************************"<<std::endl;
 ofile<<"//глобальные переменные"<<std::endl;
 ofile<<"//****************************************************************************************************"<<std::endl;

 for(size_t n=0;n<layers;n++)
 {
  ofile<<"//Вектор состояния нейронов для слоя "<<n<<std::endl;
  size_t row=cVector_H[n].GetSize();
  ofile<<"static float VectorH"<<n<<"["<<row<<"];"<<std::endl;
 }
 ofile<<"//Массив указателей на вектора состояний"<<std::endl;

 ofile<<"static float * const VectorHPtr["<<layers<<"]="<<std::endl;
 ofile<<"{"<<std::endl;
 for(size_t n=0;n<layers;n++)
 {
  ofile<<" VectorH"<<n;
  if (n!=layers-1) ofile<<",";
  ofile<<std::endl;
 }
 ofile<<"};"<<std::endl;
 
 //функция активации
 ofile<<std::endl;
 ofile<<"//****************************************************************************************************"<<std::endl;
 ofile<<"//функции"<<std::endl;
 ofile<<"//****************************************************************************************************"<<std::endl;

 ofile<<"//----------------------------------------------------------------------------------------------------"<<std::endl;
 ofile<<"//функция активации нейрона"<<std::endl;
 ofile<<"//----------------------------------------------------------------------------------------------------"<<std::endl;

 ofile<<"static float NeuronFunction(float value)"<<std::endl;
 ofile<<"{"<<std::endl;
 ofile<<" return(1.0f/(1.0f+(float)(exp(-value))));"<<std::endl;
 ofile<<"}"<<std::endl;
 ofile<<""<<std::endl;

 ofile<<"//----------------------------------------------------------------------------------------------------"<<std::endl;
 ofile<<"//функция расчёта нейросети"<<std::endl;
 ofile<<"//----------------------------------------------------------------------------------------------------"<<std::endl;

 ofile<<"void CreateAnswer(void)"<<std::endl;
 ofile<<"{"<<std::endl;
 ofile<<" for(size_t n=0;n<"<<(layers-1)<<";n++)"<<std::endl;
 ofile<<" {"<<std::endl;
 ofile<<"  float *vector_h_current=VectorHPtr[n];"<<std::endl;
 ofile<<"  float *vector_h_next=VectorHPtr[n+1];"<<std::endl;
 ofile<<"  const float *vector_b_current=VectorBPtr[n];"<<std::endl;
 ofile<<"  const float *matrix_w_current=MatrixWPtr[n];"<<std::endl;
 ofile<<""<<std::endl;
 ofile<<"  size_t mx=NeuronInLayer[n];//размер матрицы по x"<<std::endl;
 ofile<<"  size_t my=NeuronInLayer[n+1];//размер матрицы по y"<<std::endl;
 ofile<<""<<std::endl;
 ofile<<"  //умножается строка матрицы на столбец вектора, добавляется вектор смещения и применяется функция активации нейрона"<<std::endl;
 ofile<<"  for(size_t y=0;y<my;y++)"<<std::endl;
 ofile<<"  {"<<std::endl;
 ofile<<"   float value=0;"<<std::endl;
 ofile<<"   for(size_t x=0;x<mx;x++)"<<std::endl;
 ofile<<"   {"<<std::endl;
 ofile<<"    float m_value=matrix_w_current[mx*y+x];"<<std::endl;
 ofile<<"    float v_value=vector_h_current[x];"<<std::endl;
 ofile<<"    value+=m_value*v_value;"<<std::endl;
 ofile<<"   }"<<std::endl;
 ofile<<"   vector_h_next[y]=NeuronFunction(value+vector_b_current[y]);"<<std::endl;
 ofile<<"  }"<<std::endl;
 ofile<<" }"<<std::endl;
 ofile<<"}"<<std::endl;


 ofile.close();
 return(true);
}
