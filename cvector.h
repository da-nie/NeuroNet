#ifndef C_VECTOR_H
#define C_VECTOR_H

//****************************************************************************************************
//����� �������� ������������ �����������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <vector>
#include "idatastream.h"

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//��������������� ����������
//****************************************************************************************************
class CVector;

//****************************************************************************************************
//��������� �������
//****************************************************************************************************
CVector operator+(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "+"
CVector operator-(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "-"
double operator*(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "*" (��������� ������������)
CVector operator^(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "^" (��������� ������������)
CVector operator*(const CVector& cVector_Left,const double &value_right);//�������� "*"
CVector operator*(const double &value_left,const CVector& cVector_Right);//�������� "*"
CVector operator/(const CVector& cVector_Left,const double &value_right);//�������� "/"

//****************************************************************************************************
//����� �������� ������������ �����������
//****************************************************************************************************
class CVector
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------
  size_t Size;//����������� �������
  std::vector<double> Item;//������ ����������� �������
 public:
  //-�����������----------------------------------------------------------------------------------------
  CVector(size_t size=1);
  //-����������� �����������----------------------------------------------------------------------------
  CVector(const CVector &cVector);
  //-����������-----------------------------------------------------------------------------------------
  ~CVector();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  double *GetItemPtr(void);//�������� ��������� �� ������
  size_t GetSize(void) const;//�������� ������ �������
  void Normalize(void);//���������� �������
  double GetNorma(void) const;//�������� ����� �������
  double GetElement(size_t index) const;//�������� ������� �������
  void SetElement(size_t index,double value);//������ ������� �������
  void Set(double x);//������ ���������� ������
  void Set(double x,double y);//������ ���������� ������
  void Set(double x,double y,double z);//������ ��������� ������
  void Set(double x,double y,double z,double a);//������ ������������ ������
  void Zero(void);//�������� ������
  void Move(CVector &cVector);//����������� ������
  CVector& operator=(const CVector& cVector);//�������� "="
  friend CVector operator+(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "+"
  friend CVector operator-(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "-"
  friend double operator*(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "*" (��������� ������������)
  friend CVector operator^(const CVector& cVector_Left,const CVector& cVector_Right);//�������� "^" (��������� ������������)
  friend CVector operator*(const CVector& cVector_Left,const double &value_right);//�������� "*"
  friend CVector operator*(const double &value_left,const CVector& cVector_Right);//�������� "*"
  friend CVector operator/(const CVector& cVector_Left,const double &value_right);//�������� "/"

  static void Add(CVector &cVector_Output,const CVector& cVector_Left,const CVector& cVector_Right);//������� �������
  static void Sub(CVector &cVector_Output,const CVector& cVector_Left,const CVector& cVector_Right);//������� �������
  static double Mul(const CVector& cVector_Left,const CVector& cVector_Right);//��������� ������������ ��������
  static void Mul(CVector &cVector_Output,const CVector& cVector_Left,const CVector& cVector_Right);//��������� ������������
  static void Mul(CVector &cVector_Output,const CVector& cVector_Left,const double &value_right);//��������� �� ����� ������
  static void Mul(CVector &cVector_Output,const double &value_left,const CVector& cVector_Right);//��������� �� ����� �����
  static void Div(CVector &cVector_Output,const CVector& cVector_Left,const double &value_right);//������� �� �����

  bool Save(IDataStream *iDataStream_Ptr);//��������� ������
  bool Load(IDataStream *iDataStream_Ptr);//��������� ������

  static bool Test(void);//�������������� ����� ��������

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
  //-�������� �������-----------------------------------------------------------------------------------  
};

#endif

 
