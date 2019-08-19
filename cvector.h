#ifndef C_VECTOR_H
#define C_VECTOR_H

#include <math.h>

static const double CVECTOR_EPS=0.0000000001;//бесконечно-малое число

//класс вектора произвольной размерности
template<size_t size>
class CVector
{
 private:
  size_t Size;//размерность вектора
  double Item[size];//массив компонентов вектора
 public:
  CVector(void);//конструктор
  ~CVector(void);//деструктор
  CVector(const CVector<size> &cVector);//конструктор копирования
  void Normalize(void);//нормировка вектора
  double GetNorma(void) const;//получить норму вектора
  double GetElement(size_t index) const;//получить элемент вектора
  void SetElement(size_t index,double value);//задать элемент вектора
  void Set(double x);//задать одномерный вектор
  void Set(double x,double y);//задать двухмерный вектор
  void Set(double x,double y,double z);//задать трёхмерный вектор
  void Set(double x,double y,double z,double a);//задать четырёхмерный вектор
  void Zero(void);//обнулить вектор
  CVector<size>& operator=(const CVector<size>& cVector);//оператор "="

  template<size_t size>
  friend CVector<size> operator+(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right);//оператор "+"
  template<size_t size>
  friend CVector<size> operator-(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right);//оператор "-"
  template<size_t size>
  friend double operator*(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right);//оператор "*" (скалярное произведение)
  template<size_t size>
  friend CVector<size> operator^(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right);//оператор "^" (векторное произведение)
  template<size_t size>
  friend CVector<size> operator*(const CVector<size>& cVector_Left,const double &value_right);//оператор "*"
  template<size_t size>
  friend CVector<size> operator*(const double &value_left,const CVector<size>& cVector_Right);//оператор "*"
  template<size_t size>
  friend CVector<size> operator/(const CVector<size>& cVector_Left,const double &value_right);//оператор "/"
};


//====================================================================================================
//конструктор
//====================================================================================================
template<size_t size>
CVector<size>::CVector(void)
{
 Size=size;
}
//====================================================================================================
//деструктор
//====================================================================================================
template<size_t size>
CVector<size>::~CVector(void)
{
 Size=0;
}
//====================================================================================================
//конструктор копирования
//====================================================================================================
template<size_t size>
CVector<size>::CVector(const CVector<size> &cVector)
{
 if (&cVector==this) return;
 Size=cVector.Size;
 memcpy(Item,cVector.Item,Size*sizeof(double));
}
//----------------------------------------------------------------------------------------------------
//нормировка вектора
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::Normalize(void)
{
 double norma=GetNorma();
 if (norma<CVECTOR_EPS) return;
 for(size_t n=0;n<Size;n++) Item[n]/=norma;
}
//----------------------------------------------------------------------------------------------------
//получить норму вектора
//----------------------------------------------------------------------------------------------------
template<size_t size>
double CVector<size>::GetNorma(void) const
{
 double norma=0;
 for(size_t n=0;n<Size;n++) norma+=Item[n]*Item[n];
 norma=sqrt(norma);
 return(norma);
}
//----------------------------------------------------------------------------------------------------
//получить элемент вектора
//----------------------------------------------------------------------------------------------------
template<size_t size>
double CVector<size>::GetElement(size_t index) const
{
 if (index>=Size) return(0);
 return(Item[index]);
}
//----------------------------------------------------------------------------------------------------
//задать элемент вектора
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::SetElement(size_t index,double value)
{
 if (index>=Size) return;
 Item[index]=value;
}
//----------------------------------------------------------------------------------------------------
//задать одномерный вектор
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::Set(double x)
{
 if (size<1) return;
 Item[0]=x;
}
//----------------------------------------------------------------------------------------------------
//задать двухмерный вектор
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::Set(double x,double y)
{
 if (size<2) return;
 Item[0]=x;
 Item[1]=y;
}
//----------------------------------------------------------------------------------------------------
//задать трёхмерный вектор
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::Set(double x,double y,double z)
{
 if (size<3) return;
 Item[0]=x;
 Item[1]=y;
 Item[2]=z;
}
//----------------------------------------------------------------------------------------------------
//задать четырёхмерный вектор
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::Set(double x,double y,double z,double a)
{
 if (size<4) return;
 Item[0]=x;
 Item[1]=y;
 Item[2]=z;
 Item[3]=a;
}
//----------------------------------------------------------------------------------------------------
//обнулить вектор
//----------------------------------------------------------------------------------------------------
template<size_t size>
void CVector<size>::Zero(void)
{
 for(size_t n=0;n<size;n++) Item[n]=0;
}

//----------------------------------------------------------------------------------------------------
//оператор "="
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size>& CVector<size>::operator=(const CVector<size> &cVector)
{
 if (this!=&cVector)
 {
  memcpy(Item,cVector.Item,Size*sizeof(double));
 }
 return(*this);
}

//----------------------------------------------------------------------------------------------------
//оператор "+"
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size> operator+(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right)
{
 CVector<size> cVector;
 for(size_t n=0;n<size;n++)
 {
  cVector.Item[n]=cVector_Left.Item[n]+cVector_Right.Item[n];
 }
 return(cVector);
}
//----------------------------------------------------------------------------------------------------
//оператор "-"
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size> operator-(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right)
{
 CVector<size> cVector;
 for(long n=0;n<size;n++)
 {
  cVector.Item[n]=cVector_Left.Item[n]-cVector_Right.Item[n];
 }
 return(cVector);
}
//----------------------------------------------------------------------------------------------------
//оператор "*" (скалярное произведение)
//----------------------------------------------------------------------------------------------------
template<size_t size>
double operator*(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right)
{
 double ret=0;
 for(size_t n=0;n<size;n++)
 {
  ret+=cVector_Left.Item[n]*cVector_Right.Item[n];
 }
 return(ret);
}
//----------------------------------------------------------------------------------------------------
//оператор "^" (векторное произведение)
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size> operator^(const CVector<size>& cVector_Left,const CVector<size>& cVector_Right)
{
 if (size==3)//только для векторов размерности 3
 {
  CVector<size> cVector;
  cVector.Item[0]=cVector_Left.Item[1]*cVector_Right.Item[2]-cVector_Right.Item[1]*cVector_Left.Item[2];
  cVector.Item[1]=-(cVector_Left.Item[0]*cVector_Right.Item[2]-cVector_Right.Item[0]*cVector_Left.Item[2]);
  cVector.Item[2]=cVector_Left.Item[0]*cVector_Right.Item[1]-cVector_Right.Item[0]*cVector_Left.Item[1];
  return(cVector);
 }
 throw "Ошибка! Не определено векторное произведение векторов размерности отличной от 3.";
 return(cVector_Left);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size> operator*(const CVector<size>& cVector_Left,const double &value_right)
{
 CVector<size> cVector;
 for(long n=0;n<size;n++)
 {
  cVector.Item[n]=cVector_Left.Item[n]*value_right;
 }
 return(cVector);
}
//----------------------------------------------------------------------------------------------------
//оператор "*"
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size> operator*(const double &value_left,const CVector<size>& cVector_Right)
{
 CVector<size> cVector;
 for(long n=0;n<size;n++)
 {
  cVector.Item[n]=cVector_Right.Item[n]*value_left;
 }
 return(cVector);
}
//----------------------------------------------------------------------------------------------------
//оператор "/"
//----------------------------------------------------------------------------------------------------
template<size_t size>
CVector<size> operator/(const CVector<size>& cVector_Left,const double &value_right)
{
 CVector<size> cVector;
 for(long n=0;n<size;n++)
 {
  cVector.Item[n]=cVector_Left.Item[n]/value_right;
 }
 return(cVector);
}

#endif
