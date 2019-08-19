#ifndef C_MATRIX_H
#define C_MATRIX_H

#include "cvector.h"
#include <string.h>

//����� ������� ������������ �����������
template<size_t size_y,size_t size_x>
class CMatrix
{
 private:
  double Item[size_x*size_y];//������ ����������� �������
 public:
  CMatrix(void);//�����������
  ~CMatrix(void);//����������
  CMatrix(const CMatrix<size_y,size_x> &cMatrix);//����������� �����������
  double GetElement(size_t y,size_t x) const;//�������� ������� �������
  void SetElement(size_t y,size_t x,double value);//������ ������� �������

  CMatrix<size_y,size_x>& operator=(const CMatrix<size_y,size_x>& cMatrix);//�������� "="

  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator+(const CMatrix<size_y,size_x>& cMatrix_Left,const CMatrix<size_y,size_x>& cMatrix_Right);//�������� "+"
  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator-(const CMatrix<size_y,size_x>& cMatrix_Left,const CMatrix<size_y,size_x>& cMatrix_Right);//�������� "-"

  template<size_t size_left_y,size_t size_left_x,size_t size_right_x>
  friend CMatrix<size_left_y,size_right_x> operator*(const CMatrix<size_left_y,size_left_x>& cMatrix_Left,const CMatrix<size_left_x,size_right_x>& cMatrix_Right);//�������� "*"

  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator*(const CMatrix<size_y,size_x>& cMatrix_Left,const double& value_right);//�������� "*"
  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator*(const double& value_left,const CMatrix<size_y,size_x>& cMatrix_Right);//�������� "*"

  template<size_t size_y,size_t size_x>
  friend CVector<size_y> operator*(const CMatrix<size_y,size_x> &cMatrix_Left,const CVector<size_x> &cVector_Right);//�������� "*"
  template<size_t size_y,size_t size_x>
  friend CVector<size_x> operator*(const CVector<size_y> &cVector_Left,const CMatrix<size_y,size_x> &cMatrix_Right);//�������� "*"

  template<size_t size_y,size_t size_x>
  friend CMatrix<size_y,size_x> operator&(const CVector<size_y>& cVector_Left,const CVector<size_x>& cVector_Right);//�������� "&" (��������� ������� ������� �� ������ ������ - ����������� ����� �������)

  void Unitary(void);//�������� � ���������� ����
  void Zero(void);//�������� �������
  CMatrix<size_x,size_y> Transpose(void);//�������� ����������������� �������
};


//====================================================================================================
//�����������
//====================================================================================================
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>::CMatrix(void)
{
}
//====================================================================================================
//����������
//====================================================================================================
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>::~CMatrix(void)
{
}

//====================================================================================================
//����������� �����������
//====================================================================================================
template<size_t size_y,size_t size_x>
CMatrix<size_y,size_x>::CMatrix(const CMatrix<size_y,size_x> &cMatrix)
{
 if (&cMatrix==this) return;
 memcpy(Item,cMatrix.Item,size_x*size_y*sizeof(double));
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �������
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
double CMatrix<size_y,size_x>::GetElement(size_t y,size_t x) const
{
 if (x>=size_x) return(0);
 if (y>=size_y) return(0);
 return(Item[size_x*y+x]);
}
//----------------------------------------------------------------------------------------------------
//������ ������� �������
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
void CMatrix<size_y,size_x>::SetElement(size_t y,size_t x,double value)
{
 if (x>=size_x) return;
 if (y>=size_y) return;
 Item[size_x*y+x]=value;
}
//----------------------------------------------------------------------------------------------------
//�������� "="
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
//�������� "+"
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
//�������� "-"
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
//�������� "*"
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
//�������� "*"
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
//�������� "*"
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
//�������� "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CVector<size_y> operator*(const CMatrix<size_y,size_x> &cMatrix_Left,const CVector<size_x> &cVector_Right)
{
 CVector<size_y> cVector;
 //���������� ������ �� �������
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
//�������� "*"
//----------------------------------------------------------------------------------------------------
template<size_t size_y,size_t size_x>
CVector<size_x> operator*(const CVector<size_y> &cVector_Left,const CMatrix<size_y,size_x> &cMatrix_Right)
{
 CVector<size_x> cVector;
 //���������� ������ �� �������
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
//�������� � ���������� ����
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
//�������� �������
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
//�������� ����������������� �������
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
//�������� "&" (��������� ������� ������� �� ������ ������ - ����������� ����� �������)
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
