#ifndef C_MATRIX_H
#define C_MATRIX_H

//****************************************************************************************************
//����� ������ ������������ �����������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <vector>
#include "cvector.h"

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//��������������� ����������
//****************************************************************************************************
class CMatrix;

//****************************************************************************************************
//��������� �������
//****************************************************************************************************
CMatrix operator+(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "+"
CMatrix operator-(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "-"
CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "*"
CMatrix operator*(const CMatrix& cMatrix_Left,const double& value_right);//�������� "*"
CMatrix operator*(const double& value_left,const CMatrix& cMatrix_Right);//�������� "*"
 
CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "*"
 
CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right);//�������� "*"
CVector operator*(const CVector &cVector_Left,const CMatrix &cMatrix_Right);//�������� "*"
 
CMatrix operator&(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "&" (��������� ������� ������� �� ������ ������ - ����������� ����� �������)

//****************************************************************************************************
//����� ������ ������������ �����������
//****************************************************************************************************
class CMatrix
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------
  std::vector<double> Item;//������ ����������� �������
  size_t Size_X;//������ �� X
  size_t Size_Y;//������ �� Y
 public:
  //-�����������----------------------------------------------------------------------------------------
  CMatrix(size_t size_y=1,size_t size_x=1);
  //-����������� �����������----------------------------------------------------------------------------
  CMatrix(const CMatrix &cMatrix);
  //-����������-----------------------------------------------------------------------------------------
  ~CMatrix();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  size_t GetSizeX(void) const;//�������� ������ �� x
  size_t GetSizeY(void) const;//�������� ������ �� y
  double GetElement(size_t y,size_t x) const;//�������� ������� �������
  void SetElement(size_t y,size_t x,double value);//������ ������� �������
  void Unitary(void);//�������� � ���������� ����
  void Zero(void);//�������� �������
  CMatrix Transpose(void);//�������� ����������������� �������

  CMatrix& operator=(const CMatrix& cMatrix);//�������� "="
   
  friend CMatrix operator+(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "+"
  friend CMatrix operator-(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "-"
   
  friend CMatrix operator*(const CMatrix& cMatrix_Left,const CMatrix& cMatrix_Right);//�������� "*"
   
  friend CMatrix operator*(const CMatrix& cMatrix_Left,const double& value_right);//�������� "*"
  friend CMatrix operator*(const double& value_left,const CMatrix& cMatrix_Right);//�������� "*"
   
  friend CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right);//�������� "*"
  friend CVector operator*(const CVector &cVector_Left,const CMatrix &cMatrix_Right);//�������� "*"
   
  friend CMatrix operator&(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "&" (��������� ������� ������� �� ������ ������ - ����������� ����� �������)
 private:
  //-�������� �������-----------------------------------------------------------------------------------  
};

#endif
 
