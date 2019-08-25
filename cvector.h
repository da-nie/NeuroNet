#ifndef C_VECTOR_H
#define C_VECTOR_H

//****************************************************************************************************
//Класс векторов произвольной размерности
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <vector>
#include "idatastream.h"

//****************************************************************************************************
//макроопределения
//****************************************************************************************************

//****************************************************************************************************
//константы
//****************************************************************************************************

//****************************************************************************************************
//предварительные объявления
//****************************************************************************************************
class CVector;

//****************************************************************************************************
//прототипы функций
//****************************************************************************************************
CVector operator+(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "+"
CVector operator-(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "-"
double operator*(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "*" (скалярное произведение)
CVector operator^(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "^" (векторное произведение)
CVector operator*(const CVector& cVector_Left,const double &value_right);//оператор "*"
CVector operator*(const double &value_left,const CVector& cVector_Right);//оператор "*"
CVector operator/(const CVector& cVector_Left,const double &value_right);//оператор "/"

//****************************************************************************************************
//Класс векторов произвольной размерности
//****************************************************************************************************
class CVector
{
 public:
  //-перечисления---------------------------------------------------------------------------------------
  //-структуры------------------------------------------------------------------------------------------
  //-константы------------------------------------------------------------------------------------------
 private:
  //-переменные-----------------------------------------------------------------------------------------
  size_t Size;//размерность вектора
  std::vector<double> Item;//массив компонентов вектора
 public:
  //-конструктор----------------------------------------------------------------------------------------
  CVector(size_t size=1);
  //-конструктор копирования----------------------------------------------------------------------------
  CVector(const CVector &cVector);
  //-деструктор-----------------------------------------------------------------------------------------
  ~CVector();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  double *GetItemPtr(void);//получить указатель на данные
  size_t GetSize(void) const;//получить размер вектора
  void Normalize(void);//нормировка вектора
  double GetNorma(void) const;//получить норму вектора
  double GetElement(size_t index) const;//получить элемент вектора
  void SetElement(size_t index,double value);//задать элемент вектора
  void Set(double x);//задать одномерный вектор
  void Set(double x,double y);//задать двухмерный вектор
  void Set(double x,double y,double z);//задать трёхмерный вектор
  void Set(double x,double y,double z,double a);//задать четырёхмерный вектор
  void Zero(void);//обнулить вектор
  void Move(CVector &cVector);//переместить вектор
  CVector& operator=(const CVector& cVector);//оператор "="
  friend CVector operator+(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "+"
  friend CVector operator-(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "-"
  friend double operator*(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "*" (скалярное произведение)
  friend CVector operator^(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "^" (векторное произведение)
  friend CVector operator*(const CVector& cVector_Left,const double &value_right);//оператор "*"
  friend CVector operator*(const double &value_left,const CVector& cVector_Right);//оператор "*"
  friend CVector operator/(const CVector& cVector_Left,const double &value_right);//оператор "/"

  static void Add(CVector &cVector_Output,const CVector& cVector_Left,const CVector& cVector_Right);//сложить вектора
  static void Sub(CVector &cVector_Output,const CVector& cVector_Left,const CVector& cVector_Right);//вычесть вектора
  static double Mul(const CVector& cVector_Left,const CVector& cVector_Right);//скалярное произведение векторов
  static void Mul(CVector &cVector_Output,const CVector& cVector_Left,const CVector& cVector_Right);//векторное произведение
  static void Mul(CVector &cVector_Output,const CVector& cVector_Left,const double &value_right);//умножение на число справа
  static void Mul(CVector &cVector_Output,const double &value_left,const CVector& cVector_Right);//умножение на число слева
  static void Div(CVector &cVector_Output,const CVector& cVector_Left,const double &value_right);//деление на число

  bool Save(IDataStream *iDataStream_Ptr);//сохранить вектор
  bool Load(IDataStream *iDataStream_Ptr);//загрузить вектор

  static bool Test(void);//протестировать класс векторов

  void Save(FILE *file)
  {
   fwrite(&Size,sizeof(size_t),1,file);
   for(size_t n=0;n<Size;n++) fwrite(&Item[n],sizeof(double),1,file);
  }

  void Load(FILE *file)
  {
   fread(&Size,sizeof(size_t),1,file);
   for(size_t n=0;n<Size;n++) fread(&Item[n],sizeof(double),1,file);
  }

 private:
  //-закрытые функции-----------------------------------------------------------------------------------  
};

#endif

 
