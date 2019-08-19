#ifndef C_MATRIX_H
#define C_MATRIX_H

//****************************************************************************************************
// ласс матриц произвольной размерности
//****************************************************************************************************

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include <vector>
#include "cvector.h"

//****************************************************************************************************
//макроопределени€
//****************************************************************************************************

//****************************************************************************************************
//константы
//****************************************************************************************************

//****************************************************************************************************
//предварительные объ€влени€
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
// ласс матриц произвольной размерности
//****************************************************************************************************
class CMatrix
{
 public:
  //-перечислени€---------------------------------------------------------------------------------------
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
  //-конструктор копировани€----------------------------------------------------------------------------
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

  CMatrix& operator=(const CMatrix& cMatrix);//оператор "="
   
  friend CMatrix operator+(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "+"
  friend CMatrix operator-(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "-"
   
  friend CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//оператор "*"
   
  friend CMatrix operator*(const CMatrix& cMatrix_Left,const double& value_right);//оператор "*"
  friend CMatrix operator*(const double& value_left,const CMatrix& cMatrix_Right);//оператор "*"
   
  friend CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right);//оператор "*"
  friend CVector operator*(const CVector &cVector_Left,const CMatrix &cMatrix_Right);//оператор "*"
   
  friend CMatrix operator&(const CVector& cVector_Left,const CVector& cVector_Right);//оператор "&" (умножение вектора столбца на вектор строку - результатом будет матрица)
 private:
  //-закрытые функции-----------------------------------------------------------------------------------  
};

#endif
 
