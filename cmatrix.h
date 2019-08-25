#ifndef C_MATRIX_H
#define C_MATRIX_H

//****************************************************************************************************
//Класс матриц произвольной размерности
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <vector>
#include "cvector.h"
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
class CMatrix;

//****************************************************************************************************
//прототипы функций
//****************************************************************************************************
CMatrix operator+(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "+"
CMatrix operator-(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "-"
CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "*"
CMatrix operator*(const CMatrix& cMatrix_Left,const double& value_right);//оператор "*"
CMatrix operator*(const double& value_left,const CMatrix& cMatrix_Right);//оператор "*"
 
CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "*"
 
CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right);//оператор "*"
CVector operator*(const CVector &cVector_Left,const CMatrix &cMatrix_Right);//оператор "*"
 
CMatrix operator&(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "&" (умножение вектора столбца на вектор строку - результатом будет матрица)

//****************************************************************************************************
//Класс матриц произвольной размерности
//****************************************************************************************************
class CMatrix
{
 public:
  //-перечисления---------------------------------------------------------------------------------------
  //-структуры------------------------------------------------------------------------------------------
  //-константы------------------------------------------------------------------------------------------
 private:
  //-переменные-----------------------------------------------------------------------------------------
  std::vector<double> Item;//массив компонентов матрицы
  size_t Size_X;//размер по X
  size_t Size_Y;//размер по Y
 public:
  //-конструктор----------------------------------------------------------------------------------------
  CMatrix(size_t size_y=1,size_t size_x=1);
  //-конструктор копирования----------------------------------------------------------------------------
  CMatrix(const CMatrix &cMatrix);
  //-деструктор-----------------------------------------------------------------------------------------
  ~CMatrix();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  size_t GetSizeX(void) const;//получить размер по x
  size_t GetSizeY(void) const;//получить размер по y
  double GetElement(size_t y,size_t x) const;//получить элемент матрицы
  void SetElement(size_t y,size_t x,double value);//задать элемент матрицы
  void Unitary(void);//привести к единичному виду
  void Zero(void);//обнулить матрицу
  CMatrix Transpose(void);//получить транспонированную матрицу
  void Move(CMatrix &cMatrix);//переместить матрицу

  CMatrix& operator=(const CMatrix &cMatrix);//оператор "="
   
  friend CMatrix operator+(const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right);//оператор "+"
  friend CMatrix operator-(const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right);//оператор "-"
   
  friend CMatrix operator*(const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right);//оператор "*"
   
  friend CMatrix operator*(const CMatrix &cMatrix_Left,const double &value_right);//оператор "*"
  friend CMatrix operator*(const double &value_left,const CMatrix &cMatrix_Right);//оператор "*"
   
  friend CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right);//оператор "*"
  friend CVector operator*(const CVector &cVector_Left,const CMatrix &cMatrix_Right);//оператор "*"
   
  friend CMatrix operator&(const CVector &cVector_Left,const CVector &cVector_Right);//оператор "&" (умножение вектора столбца на вектор строку - результатом будет матрица)

  static void Add(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right);//сложить матрицы
  static void Sub(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right);//вычесть матрицы
  static void Mul(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right);//умножить матрицы
  static void Mul(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const double &value_right);//умножить матрицу на число
  static void Mul(CMatrix &cMatrix_Output,const double &value_left,const CMatrix &cMatrix_Right);//умножить матрицу на число
  static void Mul(CVector &cVector_Output,const CMatrix &cMatrix_Left,const CVector &cVector_Right);//умножить матрицу на вектор
  static void Mul(CVector &cVector_Output,const CVector &cVector_Left,const CMatrix &cMatrix_Right);//умножить вектор на матрицу
  static void Mul(CMatrix &cMatrix_Output,const CVector &cVector_Left,const CVector &cVector_Right);//умножить вектора и получить матрицу
  static void Transponse(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Input);//транспонировать матрицу

  bool Save(IDataStream *iDataStream_Ptr);//сохранить матрицу
  bool Load(IDataStream *iDataStream_Ptr);//загрузить матрицу

  void Save(FILE *file)
  {
   fwrite(&Size_X,sizeof(size_t),1,file);
   fwrite(&Size_Y,sizeof(size_t),1,file);
   for(size_t n=0;n<Size_X*Size_Y;n++) fwrite(&Item[n],sizeof(double),1,file);
  }
  void Load(FILE *file)
  {
   fread(&Size_X,sizeof(size_t),1,file);
   fread(&Size_Y,sizeof(size_t),1,file);
   for(size_t n=0;n<Size_X*Size_Y;n++) fread(&Item[n],sizeof(double),1,file);
  }


  static bool Test(void);//проестировать класс матриц

 private:
  //-закрытые функции-----------------------------------------------------------------------------------  
};

#endif
 
