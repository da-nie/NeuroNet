//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "cmatrix.h"
#include <math.h>

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
CMatrix::CMatrix(size_t size_y,size_t size_x)
{
 Size_X=size_x;
 Size_Y=size_y;
 Item.resize(Size_X*Size_Y);
}
//----------------------------------------------------------------------------------------------------
//конструктор копирования
//----------------------------------------------------------------------------------------------------
CMatrix::CMatrix(const CMatrix &cMatrix)
{
 if (&cMatrix==this) return;
 Item=cMatrix.Item;
 Size_X=cMatrix.Size_X;
 Size_Y=cMatrix.Size_Y;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CMatrix::~CMatrix()
{
 Item.clear();
 Size_X=0;
 Size_Y=0;
}
//****************************************************************************************************
//закрытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------

//****************************************************************************************************
//открытые функции
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//получить размер по x
//----------------------------------------------------------------------------------------------------
size_t CMatrix::GetSizeX(void) const
{
 return(Size_X);
}
//----------------------------------------------------------------------------------------------------
//получить размер по y
//----------------------------------------------------------------------------------------------------
size_t CMatrix::GetSizeY(void) const
{
 return(Size_Y);
}
//----------------------------------------------------------------------------------------------------
//получить элемент матрицы
//----------------------------------------------------------------------------------------------------
double CMatrix::GetElement(size_t y,size_t x) const
{
 if (x>=Size_X) return(0);
 if (y>=Size_Y) return(0);
 return(Item[Size_X*y+x]);
}
//----------------------------------------------------------------------------------------------------
//задать элемент матрицы
//----------------------------------------------------------------------------------------------------
void CMatrix::SetElement(size_t y,size_t x,double value)
{
 if (x>=Size_X) return;
 if (y>=Size_Y) return;
 Item[Size_X*y+x]=value;
}
//----------------------------------------------------------------------------------------------------
//привести к единичному виду
//----------------------------------------------------------------------------------------------------
void CMatrix::Unitary(void)
{
 size_t ptr=0;
 for(size_t y=0;y<Size_Y;y++)
 {
  for(size_t x=0;x<Size_X;x++,ptr++)
  {
   if (x==y) Item[ptr]=1;
   else Item[ptr]=0;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//обнулить матрицу
//----------------------------------------------------------------------------------------------------
void CMatrix::Zero(void)
{
 size_t ptr=0;
 for(size_t y=0;y<Size_Y;y++)
 {
  for(size_t x=0;x<Size_X;x++,ptr++)
  {
   Item[ptr]=0;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//получить транспонированную матрицу
//----------------------------------------------------------------------------------------------------
CMatrix CMatrix::Transpose(void)
{
 CMatrix cMatrix(Size_X,Size_Y);
 for(size_t y=0;y<Size_Y;y++)
 {
  for(size_t x=0;x<Size_X;x++)
  {
   cMatrix.Item[Size_Y*x+y]=Item[Size_X*y+x];
  }
 }
 return(cMatrix);
}

//----------------------------------------------------------------------------------------------------
//оператор "="
//----------------------------------------------------------------------------------------------------
CMatrix& CMatrix::operator=(const CMatrix &cMatrix)
{
 if (this!=&cMatrix)
 {
  Item=cMatrix.Item;
  Size_X=cMatrix.Size_X;
  Size_Y=cMatrix.Size_Y;
 }
 return(*this);
}
//----------------------------------------------------------------------------------------------------
//оператор "+"
//----------------------------------------------------------------------------------------------------
CMatrix operator+(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Left.Size_X);
  
 if (cMatrix_Left.Size_X!=cMatrix_Right.Size_X || cMatrix_Left.Size_Y!=cMatrix_Right.Size_Y)
 {
  throw "Ошибка оператора '+'! Размерности матриц не совпадают!";
  return(cMatrix);
 }
 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Left.Size_X;x++)
  {
   cMatrix.Item[cMatrix_Left.Size_X*y+x]=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x]+cMatrix_Right.Item[cMatrix_Right.Size_X*y+x];
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "-"
//----------------------------------------------------------------------------------------------------
CMatrix operator-(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Left.Size_X);
 if (cMatrix_Left.Size_X!=cMatrix_Right.Size_X || cMatrix_Left.Size_Y!=cMatrix_Right.Size_Y)
 {
  throw "Ошибка оператора '-'! Размерности матриц не совпадают!";
  return(cMatrix);
 }
  
 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Left.Size_X;x++)
  {
   cMatrix.Item[cMatrix_Left.Size_X*y+x]=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x]-cMatrix_Right.Item[cMatrix_Right.Size_X*y+x];
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Right.Size_X);
  
 if (cMatrix_Left.Size_X!=cMatrix_Right.Size_Y)
 {
  throw "Ошибка оператора '*'! Размерности матриц не совпадают!";
  return(cMatrix);
 }
  
 double *m=&cMatrix.Item[0];
 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  const double *m1_begin=&cMatrix_Left.Item[y*cMatrix_Left.Size_X];
  for(size_t x=0;x<cMatrix_Right.Size_X;x++,m++)
  {
   double s=0;
   const double *m2=&cMatrix_Right.Item[x];
   const double *m1=m1_begin;
   for(size_t n=0;n<cMatrix_Left.Size_X;n++,m1++,m2+=cMatrix_Right.Size_X) s+=(*m1)*(*m2);
   *m=s;
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
CMatrix operator*(const CMatrix& cMatrix_Left,const double& value_right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Left.Size_X);
 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Left.Size_X;x++)
  {
   cMatrix.Item[cMatrix_Left.Size_X*y+x]=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x]*value_right;
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
CMatrix operator*(const double& value_left,const CMatrix& cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Right.Size_Y,cMatrix_Right.Size_X);
 for(size_t y=0;y<cMatrix_Right.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Right.Size_X;x++)
  {
   cMatrix.Item[cMatrix_Right.Size_X*y+x]=cMatrix_Right.Item[cMatrix_Right.Size_X*y+x]*value_left;
  }
 }
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right)
{
 CVector cVector(cMatrix_Left.Size_Y);
 if (cMatrix_Left.Size_X!=cVector_Right.GetSize())
 {
  throw "Ошибка оператора '*'! Размерность матрицы и вектора не совпадают!";
  return(cVector);
 }
 //умножается строка на столбец
 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  double value=0;
  for(size_t x=0;x<cMatrix_Left.Size_X;x++)
  {
   double m_value=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x];
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
CVector operator*(const CVector&cVector_Left,const CMatrix &cMatrix_Right)
{
 CVector cVector(cMatrix_Right.Size_X);
 if (cMatrix_Right.Size_Y!=cVector_Left.GetSize())
 {
  throw "Ошибка оператора '*'! Размерность матрицы и вектора не совпадают!";
  return(cVector);
 }
 //умножается строка на столбец
 for(size_t x=0;x<cMatrix_Right.Size_X;x++)
 {
  double value=0;
  for(size_t y=0;y<cMatrix_Right.Size_Y;y++)
  {
   double m_value=cMatrix_Right.Item[cMatrix_Right.Size_X*y+x];
   double v_value=cVector_Left.GetElement(y);
   value+=m_value*v_value;
  }
  cVector.SetElement(x,value);
 }
 return(cVector);
}

//----------------------------------------------------------------------------------------------------
//оператор "&" (умножение вектора столбца на вектор строку - результатом будет матрица)
//----------------------------------------------------------------------------------------------------
CMatrix operator&(const CVector& cVector_Left,const CVector& cVector_Right)
{
 CMatrix cMatrix(cVector_Left.GetSize(),cVector_Right.GetSize());
 double *m=&cMatrix.Item[0];
 for(size_t y=0;y<cVector_Left.GetSize();y++)
 {
  for(size_t x=0;x<cVector_Right.GetSize();x++,m++)
  {
   *m=cVector_Left.GetElement(y)*cVector_Right.GetElement(x);
  }
 }
 return(cMatrix);
}

