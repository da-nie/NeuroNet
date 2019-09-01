//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include "cmatrix.h"
#include <math.h>

//****************************************************************************************************
//���������� ����������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//����������� � ����������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CMatrix::CMatrix(size_t size_y,size_t size_x)
{
 Size_X=size_x;
 Size_Y=size_y;
 Item.resize(Size_X*Size_Y);
}
//----------------------------------------------------------------------------------------------------
//����������� �����������
//----------------------------------------------------------------------------------------------------
CMatrix::CMatrix(const CMatrix &cMatrix)
{
 if (&cMatrix==this) return;
 Item=cMatrix.Item;
 Size_X=cMatrix.Size_X;
 Size_Y=cMatrix.Size_Y;
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CMatrix::~CMatrix()
{
 Item.clear();
 Size_X=0;
 Size_Y=0;
}
//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------

//****************************************************************************************************
//����������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Add(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right)
{
 if (cMatrix_Left.Size_X!=cMatrix_Right.Size_X || cMatrix_Left.Size_Y!=cMatrix_Right.Size_Y)
 {
  throw "������ ��������� '+'! ����������� ������ �� ���������!";
  return;
 }
 const double *left_ptr=&cMatrix_Left.Item[0];
 const double *right_ptr=&cMatrix_Right.Item[0];
 double *o_ptr=&cMatrix_Output.Item[0];

 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Left.Size_X;x++,o_ptr++,left_ptr++,right_ptr++)
  {
   *o_ptr=(*left_ptr)+(*right_ptr);
   //cMatrix_Output.Item[cMatrix_Left.Size_X*y+x]=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x]+cMatrix_Right.Item[cMatrix_Right.Size_X*y+x];
  }
 }
}
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Sub(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right)
{
 if (cMatrix_Left.Size_X!=cMatrix_Right.Size_X || cMatrix_Left.Size_Y!=cMatrix_Right.Size_Y)
 {
  throw "������ ��������� '-'! ����������� ������ �� ���������!";
  return;
 }  
 const double *left_ptr=&cMatrix_Left.Item[0];
 const double *right_ptr=&cMatrix_Right.Item[0];
 double *o_ptr=&cMatrix_Output.Item[0];

 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Left.Size_X;x++,o_ptr++,left_ptr++,right_ptr++)
  {
   //cMatrix.Item[cMatrix_Left.Size_X*y+x]=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x]-cMatrix_Right.Item[cMatrix_Right.Size_X*y+x];
   *o_ptr=(*left_ptr)-(*right_ptr);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Mul(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right)
{
 if (cMatrix_Left.Size_X!=cMatrix_Right.Size_Y || cMatrix_Output.Size_Y!=cMatrix_Left.Size_Y || cMatrix_Output.Size_X!=cMatrix_Right.Size_X)
 {
  throw "������ ��������� '*'! ����������� ������ �� ���������!";
  return;
 }
  
 double *m=&cMatrix_Output.Item[0];
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
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �� �����
//----------------------------------------------------------------------------------------------------
void CMatrix::Mul(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Left,const double &value_right)
{
 if (cMatrix_Output.Size_X!=cMatrix_Left.Size_X || cMatrix_Output.Size_Y!=cMatrix_Left.Size_Y)
 {
  throw "������ ��������� '*'! ����������� ������ �� ���������!";
  return;
 }

 const double *left_ptr=&cMatrix_Left.Item[0];
 double *o_ptr=&cMatrix_Output.Item[0];

 for(size_t y=0;y<cMatrix_Left.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Left.Size_X;x++,o_ptr++,left_ptr++)
  {
   *o_ptr=(*left_ptr)*value_right;
   //cMatrix.Item[cMatrix_Left.Size_X*y+x]=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x]*value_right;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �� �����
//----------------------------------------------------------------------------------------------------
void CMatrix::Mul(CMatrix &cMatrix_Output,const double &value_left,const CMatrix &cMatrix_Right)
{
 if (cMatrix_Output.Size_X!=cMatrix_Right.Size_X || cMatrix_Output.Size_Y!=cMatrix_Right.Size_Y)
 {
  throw "������ ��������� '*'! ����������� ������ �� ���������!";
  return;
 }

 const double *right_ptr=&cMatrix_Right.Item[0];
 double *o_ptr=&cMatrix_Output.Item[0];

 for(size_t y=0;y<cMatrix_Right.Size_Y;y++)
 {
  for(size_t x=0;x<cMatrix_Right.Size_X;x++,o_ptr++,right_ptr++)
  {
   *o_ptr=(*right_ptr)*value_left;
   //cMatrix.Item[cMatrix_Right.Size_X*y+x]=cMatrix_Right.Item[cMatrix_Right.Size_X*y+x]*value_left;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �� ������
//----------------------------------------------------------------------------------------------------
void CMatrix::Mul(CVector &cVector_Output,const CMatrix &cMatrix_Left,const CVector &cVector_Right)
{
 //CVector cVector(cMatrix_Left.Size_Y);

 if (cMatrix_Left.Size_X!=cVector_Right.GetSize() || cVector_Output.GetSize()!=cMatrix_Left.Size_Y)
 {
  throw "������ ��������� '*'! ����������� ������� � ������� �� ���������!";
  return;
 }

 const double *left_ptr=&cMatrix_Left.Item[0];
 const double *right_ptr=const_cast<CVector&>(cVector_Right).GetItemPtr();
 double *o_ptr=cVector_Output.GetItemPtr();

 //���������� ������ �� �������
 for(size_t y=0;y<cMatrix_Left.Size_Y;y++,left_ptr+=cMatrix_Left.Size_X,o_ptr++)
 {
  double value=0;
  const double *left_ptr_local=left_ptr;
  const double *right_ptr_local=right_ptr;
  for(size_t x=0;x<cMatrix_Left.Size_X;x++,left_ptr_local++,right_ptr_local++)
  {
//   double m_value=cMatrix_Left.Item[cMatrix_Left.Size_X*y+x];
//   double v_value=cVector_Right.GetElement(x);

   double m_value=*left_ptr_local;
   double v_value=*right_ptr_local;

   value+=m_value*v_value;
  }
  *o_ptr=value;
  //cVector.SetElement(y,value);
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ������ �� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Mul(CVector &cVector_Output,const CVector &cVector_Left,const CMatrix &cMatrix_Right)
{
 //CVector cVector(cMatrix_Right.Size_X);

 if (cMatrix_Right.Size_Y!=cVector_Left.GetSize() || cVector_Output.GetSize()!=cMatrix_Right.Size_X)
 {
  throw "������ ��������� '*'! ����������� ������� � ������� �� ���������!";
  return;
 }

 const double *left_ptr=const_cast<CVector&>(cVector_Left).GetItemPtr();
 const double *right_ptr=&cMatrix_Right.Item[0];
 double *o_ptr=cVector_Output.GetItemPtr();

 //���������� ������ �� �������
 for(size_t x=0;x<cMatrix_Right.Size_X;x++,o_ptr++,right_ptr++)
 {
  double value=0;
  const double *left_ptr_local=left_ptr;
  const double *right_ptr_local=right_ptr;
  for(size_t y=0;y<cMatrix_Right.Size_Y;y++,right_ptr_local+=cMatrix_Right.Size_X,left_ptr_local++)
  {
   //double m_value=cMatrix_Right.Item[cMatrix_Right.Size_X*y+x];
   //double v_value=cVector_Left.GetElement(y);

   double m_value=*right_ptr_local;
   double v_value=*left_ptr_local;

   value+=m_value*v_value;
  }
  *o_ptr=value;
  //cVector.SetElement(x,value);
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ������� � �������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Mul(CMatrix &cMatrix_Output,const CVector &cVector_Left,const CVector &cVector_Right)
{
 //CMatrix cMatrix(cVector_Left.GetSize(),cVector_Right.GetSize());

 if (cMatrix_Output.Size_Y!=cVector_Left.GetSize() || cMatrix_Output.Size_X!=cVector_Right.GetSize())
 {
  throw "������ ��������� '&'! ����������� ������� � �������� �� ���������!";
  return;
 }
 double *m=&cMatrix_Output.Item[0];

 const double *left_ptr=const_cast<CVector&>(cVector_Left).GetItemPtr();
 const double *right_ptr=const_cast<CVector&>(cVector_Right).GetItemPtr();

 for(size_t y=0;y<cVector_Left.GetSize();y++,left_ptr++)
 {
  const double *right_ptr_local=right_ptr;
  for(size_t x=0;x<cVector_Right.GetSize();x++,m++,right_ptr_local++)
  {
//   *m=cVector_Left.GetElement(y)*cVector_Right.GetElement(x);
   *m=(*left_ptr)*(*right_ptr_local);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//��������������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Transponse(CMatrix &cMatrix_Output,const CMatrix &cMatrix_Input)
{
 if (cMatrix_Output.Size_Y!=cMatrix_Input.Size_X || cMatrix_Output.Size_X!=cMatrix_Input.Size_Y)
 {
  throw "������ ����������������! ����������� ������ �� ���������!";
  return;
 }
 const double *i_ptr=&cMatrix_Input.Item[0];
 double *o_ptr=&cMatrix_Output.Item[0];
 for(size_t y=0;y<cMatrix_Input.Size_Y;y++,o_ptr++)
 {
  double *o_ptr_local=o_ptr;
  for(size_t x=0;x<cMatrix_Input.Size_X;x++,o_ptr_local+=cMatrix_Input.Size_Y,i_ptr++)
  {
   //cMatrix_Output.Item[cMatrix_Input.Size_Y*x+y]=cMatrix_Input.Item[cMatrix_Input.Size_X*y+x];
   *o_ptr_local=*i_ptr;
  }
 }
}

//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//�������� ������ �� x
//----------------------------------------------------------------------------------------------------
size_t CMatrix::GetSizeX(void) const
{
 return(Size_X);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ �� y
//----------------------------------------------------------------------------------------------------
size_t CMatrix::GetSizeY(void) const
{
 return(Size_Y);
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �������
//----------------------------------------------------------------------------------------------------
double CMatrix::GetElement(size_t y,size_t x) const
{
 if (x>=Size_X) 
 {
  throw("������ ������� � �������� ������� ��� ������!");
  return(0);
 }
 if (y>=Size_Y)
 {
  throw("������ ������� � �������� ������� ��� ������!");
  return(0);
 }
 return(Item[Size_X*y+x]);
}
//----------------------------------------------------------------------------------------------------
//������ ������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::SetElement(size_t y,size_t x,double value)
{
 if (x>=Size_X) 
 {
  throw("������ ������� � �������� ������� ��� ������!");
  return;
 }
 if (y>=Size_Y)
 {
  throw("������ ������� � �������� ������� ��� ������!");
  return;
 }
 Item[Size_X*y+x]=value;
}
//----------------------------------------------------------------------------------------------------
//�������� � ���������� ����
//----------------------------------------------------------------------------------------------------
void CMatrix::Unitary(void)
{
 double *o_ptr=&Item[0];
 for(size_t y=0;y<Size_Y;y++)
 {
  for(size_t x=0;x<Size_X;x++,o_ptr++)
  {
   if (x==y) *o_ptr=1;
        else *o_ptr=0;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Zero(void)
{
 double *o_ptr=&Item[0];
 for(size_t y=0;y<Size_Y;y++)
 {
  for(size_t x=0;x<Size_X;x++,o_ptr++)
  {
   *o_ptr=0;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ����������������� �������
//----------------------------------------------------------------------------------------------------
CMatrix CMatrix::Transpose(void)
{
 CMatrix cMatrix(Size_X,Size_Y);
 CMatrix::Transponse(cMatrix,*this);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//����������� �������
//----------------------------------------------------------------------------------------------------
void CMatrix::Move(CMatrix &cMatrix)
{
 if (this==&cMatrix) return;
 Item=std::move(cMatrix.Item);
 Size_X=cMatrix.Size_X;
 Size_Y=cMatrix.Size_Y;
 cMatrix.Size_X=0;
 cMatrix.Size_Y=0;
}

//----------------------------------------------------------------------------------------------------
//�������� "="
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
//�������� "+"
//----------------------------------------------------------------------------------------------------
CMatrix operator+(const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Left.Size_X);
 CMatrix::Add(cMatrix,cMatrix_Left,cMatrix_Right);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//�������� "-"
//----------------------------------------------------------------------------------------------------
CMatrix operator-(const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Left.Size_X);
 CMatrix::Sub(cMatrix,cMatrix_Left,cMatrix_Right);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//�������� "*"
//----------------------------------------------------------------------------------------------------
CMatrix operator*(const CMatrix &cMatrix_Left,const CMatrix &cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Right.Size_X);
 CMatrix::Mul(cMatrix,cMatrix_Left,cMatrix_Right);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//�������� "*"
//----------------------------------------------------------------------------------------------------
CMatrix operator*(const CMatrix &cMatrix_Left,const double &value_right)
{
 CMatrix cMatrix(cMatrix_Left.Size_Y,cMatrix_Left.Size_X);
 CMatrix::Mul(cMatrix,cMatrix_Left,value_right);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//�������� "*"
//----------------------------------------------------------------------------------------------------
CMatrix operator*(const double &value_left,const CMatrix &cMatrix_Right)
{
 CMatrix cMatrix(cMatrix_Right.Size_Y,cMatrix_Right.Size_X);
 CMatrix::Mul(cMatrix,value_left,cMatrix_Right);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//�������� "*"
//----------------------------------------------------------------------------------------------------
CVector operator*(const CMatrix &cMatrix_Left,const CVector &cVector_Right)
{
 CVector cVector(cMatrix_Left.Size_Y);
 CMatrix::Mul(cVector,cMatrix_Left,cVector_Right); 
 return(cVector);
}
//----------------------------------------------------------------------------------------------------
//�������� "*"
//----------------------------------------------------------------------------------------------------
CVector operator*(const CVector &cVector_Left,const CMatrix &cMatrix_Right)
{
 CVector cVector(cMatrix_Right.Size_X);
 CMatrix::Mul(cVector,cVector_Left,cMatrix_Right);
 return(cVector);
}

//----------------------------------------------------------------------------------------------------
//�������� "&" (��������� ������� ������� �� ������ ������ - ����������� ����� �������)
//----------------------------------------------------------------------------------------------------
CMatrix operator&(const CVector &cVector_Left,const CVector &cVector_Right)
{
 CMatrix cMatrix(cVector_Left.GetSize(),cVector_Right.GetSize());
 CMatrix::Mul(cMatrix,cVector_Left,cVector_Right);
 return(cMatrix);
}
//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
bool CMatrix::Save(IDataStream *iDataStream_Ptr)
{
 //��������� ����������� �������
 iDataStream_Ptr->SaveUInt32(Size_Y);
 iDataStream_Ptr->SaveUInt32(Size_X);
 //��������� ������ �������
 for(size_t n=0;n<Size_X*Size_Y;n++) iDataStream_Ptr->SaveDouble(Item[n]);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
bool CMatrix::Load(IDataStream *iDataStream_Ptr)
{
 //��������� ����������� �������
 Size_Y=iDataStream_Ptr->LoadUInt32();
 Size_X=iDataStream_Ptr->LoadUInt32();

 std::vector<double> item(Size_X*Size_Y);
 Item.clear();
 std::swap(Item,item);

 //��������� ������ �������
 for(size_t n=0;n<Size_X*Size_Y;n++) Item[n]=iDataStream_Ptr->LoadDouble();
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������������� ����� ������
//----------------------------------------------------------------------------------------------------
bool CMatrix::Test(void)
{
 CMatrix cMatrixA(2,2);
 CMatrix cMatrixB(2,2);
 CMatrix cMatrixC(2,2);

 cMatrixA.SetElement(0,0,1);
 cMatrixA.SetElement(0,1,2);
 cMatrixA.SetElement(1,0,3);
 cMatrixA.SetElement(1,1,4);

 cMatrixB.SetElement(0,0,1);
 cMatrixB.SetElement(0,1,2);
 cMatrixB.SetElement(1,0,3);
 cMatrixB.SetElement(1,1,4);

 //�������� �� ���������� �������
 if (cMatrixA.GetElement(0,0)!=1) return(false);
 if (cMatrixA.GetElement(0,1)!=2) return(false);
 if (cMatrixA.GetElement(1,0)!=3) return(false);
 if (cMatrixA.GetElement(1,1)!=4) return(false);

 //��������� �� ����� ������
 cMatrixC=cMatrixA*2;
 if (cMatrixC.GetElement(0,0)!=2) return(false);
 if (cMatrixC.GetElement(0,1)!=4) return(false);
 if (cMatrixC.GetElement(1,0)!=6) return(false);
 if (cMatrixC.GetElement(1,1)!=8) return(false);
 //��������� �� ����� �����
 cMatrixC=2*cMatrixA;
 if (cMatrixC.GetElement(0,0)!=2) return(false);
 if (cMatrixC.GetElement(0,1)!=4) return(false);
 if (cMatrixC.GetElement(1,0)!=6) return(false);
 if (cMatrixC.GetElement(1,1)!=8) return(false);
 //��������� ������
 cMatrixC=cMatrixA*cMatrixB;
 if (cMatrixC.GetElement(0,0)!=7) return(false);
 if (cMatrixC.GetElement(0,1)!=10) return(false);
 if (cMatrixC.GetElement(1,0)!=15) return(false); 
 if (cMatrixC.GetElement(1,1)!=22) return(false);
 //��������� ������
 cMatrixC=cMatrixA-cMatrixB;
 if (cMatrixC.GetElement(0,0)!=0) return(false);
 if (cMatrixC.GetElement(0,1)!=0) return(false);
 if (cMatrixC.GetElement(1,0)!=0) return(false);
 if (cMatrixC.GetElement(1,1)!=0) return(false);
  //�������� ������
 cMatrixC=cMatrixA+cMatrixB;
 if (cMatrixC.GetElement(0,0)!=2) return(false);
 if (cMatrixC.GetElement(0,1)!=4) return(false);
 if (cMatrixC.GetElement(1,0)!=6) return(false);
 if (cMatrixC.GetElement(1,1)!=8) return(false);
  
 //��������� ������� �� ������ ������
 CVector cVectorA(2);
 CVector cVectorB(2);
 cVectorA.Set(10,20);

 cVectorB=cMatrixA*cVectorA;
 if (cVectorB.GetElement(0)!=50) return(false);
 if (cVectorB.GetElement(1)!=110) return(false);

 //��������� ������� �� ������ �����
 cVectorB=cVectorA*cMatrixA;
 if (cVectorB.GetElement(0)!=70) return(false);
 if (cVectorB.GetElement(1)!=100) return(false);

 //��������� ������� �� ��������
 cVectorA.Set(1,2);
 cVectorB.Set(3,4);
 cMatrixC=cVectorA&cVectorB;

 if (cMatrixC.GetElement(0,0)!=3) return(false);
 if (cMatrixC.GetElement(0,1)!=4) return(false);
 if (cMatrixC.GetElement(1,0)!=6) return(false);
 if (cMatrixC.GetElement(1,1)!=8) return(false);

 //���������������� �������
 cMatrixC=cMatrixA.Transpose();
 if (cMatrixC.GetElement(0,0)!=1) return(false);
 if (cMatrixC.GetElement(0,1)!=3) return(false);
 if (cMatrixC.GetElement(1,0)!=2) return(false);
 if (cMatrixC.GetElement(1,1)!=4) return(false);

 return(true);
}