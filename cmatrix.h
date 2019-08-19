#ifndef C_MATRIX_H
#define C_MATRIX_H

#include "cvector.h"
#include <string.h>

//класс матрицы произвольной размерности
template<size_t size_y,size_t size_x>
class CMatrix
{
 private:
  double Item[size_x*size_y];//массив компонентов матрицы
 public:
  CMatrix(void);//конструктор
  ~CMatrix(void);//деструктор
  CMatrix(const CMatrix<size_y,size_x> &cMatrix);//конструктор копирования
  double GetElement(size_t y,size_t x) const;//получить элемент матрицы
  void SetElement(size_t y,size_t x,double value);//задать элемент матрицы

  CMatrix<size_y,size_x>& operator=(const CMatrix<size_y,size_x>& cMatrix);//оператор "="

  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator+(const CMatrix<size_y,size_x>& cMatrix_Left,const CMatrix<size_y,size_x>& cMatrix_Right);//оператор "+"
  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator-(const CMatrix<size_y,size_x>& cMatrix_Left,const CMatrix<size_y,size_x>& cMatrix_Right);//оператор "-"

  template<size_t size_left_y,size_t size_left_x,size_t size_right_x>
  friend CMatrix<size_left_y,size_right_x> operator*(const CMatrix<size_left_y,size_left_x>& cMatrix_Left,const CMatrix<size_left_x,size_right_x>& cMatrix_Right);//оператор "*"

  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator*(const CMatrix<size_y,size_x>& cMatrix_Left,const double& value_right);//оператор "*"
  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator*(const double& value_left,const CMatrix<size_y,size_x>& cMatrix_Right);//оператор "*"

  template<size_t size_y,size_t size_x>
  friend CVector<size_y> operator*(const CMatrix<size_y,size_x> &cMatrix_Left,const CVector<size_x> &cVector_Right);//оператор "*"
  template<size_t size_y,size_t size_x>
  friend CVector<size_x> operator*(const CVector<size_y> &cVector_Left,const CMatrix<size_y,size_x> &cMatrix_Right);//оператор "*"

  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator&(const CVector<size_y>& cVector_Left,const CVector<size_x>& cVector_Right);//оператор "&" (умножение вектора столбца на вектор строку - результатом будет матрица)

  void Unitary(void);//привести к единичному виду
  void Zero(void);//обнулить матрицу
  CMatrix<size_x,size_y> Transpose(void);//получить транспонированную матрицу
};


//====================================================================================================
//конструктор
//====================================================================================================
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>::CMatrix(void)
{
}
//====================================================================================================
//деструктор
//====================================================================================================
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>::~CMatrix(void)
{
}

//====================================================================================================
//конструктор копирования
//====================================================================================================
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>::CMatrix(const CMatrix<size_y,size_x> &cMatrix)
{
 if (&cMatrix==this) return;
 memcpy(Item,cMatrix.Item,size_x*size_y*sizeof(double));
}
//----------------------------------------------------------------------------------------------------
//получить элемент матрицы
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
double CMatrix<size_y,size_x>::GetElement(size_t y,size_t x) const
{
 if (x>=size_x) return(0);
 if (y>=size_y) return(0);
 return(Item[size_x*y+x]);
}
//----------------------------------------------------------------------------------------------------
//задать элемент матрицы
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
void CMatrix<size_y,size_x>::SetElement(size_t y,size_t x,double value)
{
 if (x>=size_x) return;
 if (y>=size_y) return;
 Item[size_x*y+x]=value;
}
//----------------------------------------------------------------------------------------------------
//оператор "="
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>& CMatrix<size_y,size_x>::operator=(const CMatrix<size_y,size_x> &cMatrix)
{
 if (this!=&cMatrix)
 {
  memcpy(Item,cMatrix.Item,size_x*size_y*sizeof(double));
 }
 return(*this);
}

//----------------------------------------------------------------------------------------------------
//оператор "+"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x> operator+(const CMatrix<size_y,size_x>& cMatrix_Left,const CMatrix<size_y,size_x>& cMatrix_Right)
{
 CMatrix<size_y,size_x> cMatrix;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++)
  {
   cMatrix.Item[size_x*y+x]=cMatrix_Left.Item[size_x*y+x]+cMatrix_Right.Item[size_x*y+x];
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "-"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x> operator-(const CMatrix<size_y,size_x>& cMatrix_Left,const CMatrix<size_y,size_x>& cMatrix_Right)
{
 CMatrix<size_y,size_x> cMatrix;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++)
  {
   cMatrix.Item[size_x*y+x]=cMatrix_Left.Item[size_x*y+x]-cMatrix_Right.Item[size_x*y+x];
  }
 }
 return(cMatrix);
}

//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_left_y,size_t size_left_x,size_t size_right_x>
CMatrix<size_left_y,size_right_x> operator*(const CMatrix<size_left_y,size_left_x>& cMatrix_Left,const CMatrix<size_left_x,size_right_x>& cMatrix_Right)
{
 CMatrix<size_left_y,size_right_x> cMatrix;
 double *m=cMatrix.Item;
 for(size_t y=0;y<size_left_y;y++)
 {
  const double *m1_begin=&cMatrix_Left.Item[y*size_left_x];
  for(size_t x=0;x<size_right_x;x++,m++)
  {
   double s=0;
   const double *m2=&cMatrix_Right.Item[x];
   const double *m1=m1_begin;
   for(size_t n=0;n<size_left_x;n++,m1++,m2+=size_right_x) s+=(*m1)*(*m2);
   *m=s;
  }
 }
 return(cMatrix);
}

//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x> operator*(const CMatrix<size_y,size_x>& cMatrix_Left,const double& value_right)
{
 CMatrix<size_y,size_x> cMatrix;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++)
  {
   cMatrix.Item[size_x*y+x]=cMatrix_Left.Item[size_x*y+x]*value_right;
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x> operator*(const double& value_left,const CMatrix<size_y,size_x>& cMatrix_Right)
{
 CMatrix<size_y,size_x> cMatrix;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++)
  {
   cMatrix.Item[size_x*y+x]=cMatrix_Right.Item[size_x*y+x]*value_left;
  }
 }
 return(cMatrix);
}

//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CVector<size_y> operator*(const CMatrix<size_y,size_x> &cMatrix_Left,const CVector<size_x> &cVector_Right)
{
 CVector<size_y> cVector;
 //умножается строка на столбец
 for(size_t y=0;y<size_y;y++)
 {
  double value=0;
  for(size_t x=0;x<size_x;x++)
  {
   double m_value=cMatrix_Left.Item[size_x*y+x];
   double v_value=cVector_Right.GetElement(x);
   value+=m_value*v_value;
  }
  cVector.SetElement(y,value);
 }
 return(cVector);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CVector<size_x> operator*(const CVector<size_y> &cVector_Left,const CMatrix<size_y,size_x> &cMatrix_Right)
{
 CVector<size_x> cVector;
 //умножается строка на столбец
 for(size_t x=0;x<size_x;x++)
 {
  double value=0;
  for(size_t y=0;y<size_y;y++)
  {
   double m_value=cMatrix_Right.Item[size_x*y+x];
   double v_value=cVector_Left.GetElement(y);
   value+=m_value*v_value;
  }
  cVector.SetElement(x,value);
 }
 return(cVector);
}

//----------------------------------------------------------------------------------------------------
//привести к единичному виду
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
void CMatrix<size_y,size_x>::Unitary(void)
{
 size_t ptr=0;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++,ptr++)
  {
   if (x==y) Item[ptr]=1;
        else Item[ptr]=0;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//обнулить матрицу
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
void CMatrix<size_y,size_x>::Zero(void)
{
 size_t ptr=0;
 for(size_t y=0;y<size_y;y++)
  for(size_t x=0;x<size_x;x++,ptr++)
  {
   Item[ptr]=0;
  }
}
//----------------------------------------------------------------------------------------------------
//получить транспонированную матрицу
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_x,size_y> CMatrix<size_y,size_x>::Transpose(void)
{
 CMatrix<size_x,size_y> cMatrix;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++)
  {
   cMatrix.Item[size_y*x+y]=Item[size_x*y+x];
  }
 }
 return(cMatrix);
}

//----------------------------------------------------------------------------------------------------
//оператор "&" (умножение вектора столбца на вектор строку - результатом будет матрица)
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x> operator&(const CVector<size_y>& cVector_Left,const CVector<size_x>& cVector_Right)
{
 CMatrix<size_y,size_x> cMatrix;
 double *m=cMatrix.Item;
 for(size_t y=0;y<size_y;y++)
 {
  for(size_t x=0;x<size_x;x++,m++)
  {
   *m=cVector_Left.GetElement(y)*cVector_Right.GetElement(x);
  }
 }
 return(cMatrix);
}

#endif
