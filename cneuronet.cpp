//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include "cneuronet.h"
#include <fstream>

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
CNeuroNet::CNeuroNet(void)
{ 
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CNeuroNet::~CNeuroNet()
{
}

//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
double CNeuroNet::GetRandValue(double max_value)
{
 return((static_cast<double>(rand())*max_value)/static_cast<double>(RAND_MAX));
}
//----------------------------------------------------------------------------------------------------
//������� ��������� �������
//----------------------------------------------------------------------------------------------------
double CNeuroNet::NeuronFunction(double value)
{
 return(1.0/(1.0+exp(-value)));
}
//----------------------------------------------------------------------------------------------------
//����������� ������� ��������� �������
//----------------------------------------------------------------------------------------------------
double CNeuroNet::NeuronFunctionDifferencial(double value)
{
 return((1.0-NeuronFunction(value))*NeuronFunction(value));
}
//----------------------------------------------------------------------------------------------------
//��������� ������ ������ �� ����
//----------------------------------------------------------------------------------------------------
void CNeuroNet::Forward(void)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return;
 //��������� ����
 for(size_t n=0;n<layers-1;n++)
{ 
  CMatrix::Mul(cVector_TmpZ[n],cMatrix_W[n],cVector_H[n]);
  CVector::Add(cVector_Z[n+1],cVector_TmpZ[n],cVector_B[n]);
  //cVector_Z[n+1].Move(cMatrix_W[n]*cVector_H[n]+cVector_B[n]);
  //�������� ������� ���������
  for(size_t y=0;y<cVector_Z[n+1].GetSize();y++)
  {
   double v=cVector_Z[n+1].GetElement(y);
   cVector_H[n+1].SetElement(y,NeuronFunction(v));
  }
 }
}

//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//������� ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNet::Create(const std::vector<size_t> &neuron_in_layers)
{
 size_t layers=neuron_in_layers.size();

 if (layers<2) return;
 NeuronInLayers=neuron_in_layers;

 cMatrix_W.resize(layers-1);

 cVector_B.resize(layers-1);

 for(size_t n=0;n<layers-1;n++)
 {
  cMatrix_W[n]=CMatrix(NeuronInLayers[n+1],NeuronInLayers[n]);
  cVector_B[n]=CVector(NeuronInLayers[n+1]);
 }

 cVector_H.resize(layers);
 cVector_Z.resize(layers);

 for(size_t n=0;n<layers;n++)
 {
  cVector_H[n]=CVector(NeuronInLayers[n]);
  cVector_Z[n]=CVector(NeuronInLayers[n]);
 }

 cVector_TmpZ.resize(layers-1);
 for(size_t n=0;n<layers-1;n++)
 {
  cVector_TmpZ[n]=CVector(cVector_Z[n+1].GetSize());
 }

 Reset();
}
//----------------------------------------------------------------------------------------------------
//�������� ��������� � �������� ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNet::Reset(void)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return;
 //�������������� ���� � ������
 for(size_t n=0;n<layers-1;n++)
 {
  //����
  for(size_t y=0;y<cMatrix_W[n].GetSizeY();y++)
  {
   for(size_t x=0;x<cMatrix_W[n].GetSizeX();x++) cMatrix_W[n].SetElement(y,x,GetRandValue(2.0)-1.0);
  }
  //������
  for(size_t y=0;y<cVector_B[n].GetSize();y++) cVector_B[n].SetElement(y,GetRandValue(2.0)-1.0);
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ��������� ������ ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNet::GetAnswer(const CVector &input,CVector &output)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return;
 cVector_H[0]=input;//������� ������
 Forward();
 output=cVector_H[layers-1];
}
//----------------------------------------------------------------------------------------------------
//�������������� ���������
//----------------------------------------------------------------------------------------------------
bool CNeuroNet::Export(const std::string &file_name)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return(false);

 std::ofstream ofile;
 ofile.open(file_name,std::ios_base::out|std::ios_base::binary);
 if (ofile.is_open()==false) return(false);
  
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;
 ofile<<"//������������ ����������"<<"\r"<<std::endl;
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;
 ofile<<"#include <stdio.h>"<<"\r"<<std::endl;
 ofile<<"#include <math.h>"<<"\r"<<std::endl;
 ofile<<""<<"\r"<<std::endl;

 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;
 ofile<<"//���������"<<"\r"<<std::endl;
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;

 for(size_t n=0;n<layers-1;n++)
 {
  size_t row=cMatrix_W[n].GetSizeY();
  size_t col=cMatrix_W[n].GetSizeX();
  ofile<<"//������� ����� ������ ���� "<<n<<" �� ����� "<<(n+1)<<"\r"<<std::endl;
  ofile<<"static const float MatrixW"<<n<<"["<<row<<"*"<<col<<"]="<<"\r"<<std::endl;
  ofile<<"{"<<"\r"<<std::endl;
  for(size_t r=0;r<row;r++)
  {
   ofile<<" ";
   for(size_t c=0;c<col;c++) 
   {
	ofile<<cMatrix_W[n].GetElement(r,c);
	if (r!=row-1 || c!=col-1) ofile<<",";
   }   
   ofile<<"\r"<<std::endl;
  }
  ofile<<"};"<<"\r"<<std::endl;

  ofile<<"//������ ������� ������ ���� "<<n<<" �� ����� "<<(n+1)<<"\r"<<std::endl;
  row=cVector_B[n].GetSize();
  ofile<<"static const float VectorB"<<n<<"["<<row<<"]="<<"\r"<<std::endl;
  ofile<<"{"<<"\r"<<std::endl;
  for(size_t r=0;r<row;r++)
  {
   ofile<<" "<<cVector_B[n].GetElement(r);
   if (r!=row-1) ofile<<",";
   ofile<<"\r"<<std::endl;
  }
  ofile<<"};"<<"\r"<<std::endl;
  ofile<<"//----------------------------------------------------------------------------------------------------"<<"\r"<<std::endl;
 }

 ofile<<"//���������� �������� � �����"<<"\r"<<std::endl;
 ofile<<"static const size_t NeuronInLayer["<<layers<<"]="<<"\r"<<std::endl;
 ofile<<"{"<<"\r"<<std::endl;
 for(size_t n=0;n<layers;n++)
 {
  ofile<<" "<<NeuronInLayers[n];
  if (n!=layers-1) ofile<<",";
  ofile<<"\r"<<std::endl;
 }
 ofile<<"};"<<"\r"<<std::endl;
 ofile<<"//������ ���������� �� ������� ������"<<"\r"<<std::endl;
 ofile<<"static const float *MatrixWPtr["<<(layers-1)<<"]="<<"\r"<<std::endl;
 ofile<<"{"<<"\r"<<std::endl;
 for(size_t n=0;n<layers-1;n++)
 {
  ofile<<" MatrixW"<<n;
  if (n!=layers-2) ofile<<",";
  ofile<<"\r"<<std::endl;
 }
 ofile<<"};"<<"\r"<<std::endl;

 ofile<<"//������ ���������� �� ������� �������"<<"\r"<<std::endl;
 ofile<<"static const float *VectorBPtr["<<(layers-1)<<"]="<<"\r"<<std::endl;
 ofile<<"{"<<"\r"<<std::endl;
 for(size_t n=0;n<layers-1;n++)
 {
  ofile<<" VectorB"<<n;
  if (n!=layers-2) ofile<<",";
  ofile<<"\r"<<std::endl;
 }
 ofile<<"};"<<"\r"<<std::endl;

 ofile<<""<<"\r"<<std::endl;
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;
 ofile<<"//���������� ����������"<<"\r"<<std::endl;
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;

 for(size_t n=0;n<layers;n++)
 {
  ofile<<"//������ ��������� �������� ��� ���� "<<n<<"\r"<<std::endl;
  size_t row=cVector_H[n].GetSize();
  ofile<<"static float VectorH"<<n<<"["<<row<<"];"<<"\r"<<std::endl;
 }
 ofile<<"//������ ���������� �� ������� ���������"<<"\r"<<std::endl;

 ofile<<"static float * const VectorHPtr["<<layers<<"]="<<"\r"<<std::endl;
 ofile<<"{"<<"\r"<<std::endl;
 for(size_t n=0;n<layers;n++)
 {
  ofile<<" VectorH"<<n;
  if (n!=layers-1) ofile<<",";
  ofile<<"\r"<<std::endl;
 }
 ofile<<"};"<<"\r"<<std::endl;
 
 //������� ���������
 ofile<<"\r"<<std::endl;
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;
 ofile<<"//�������"<<"\r"<<std::endl;
 ofile<<"//****************************************************************************************************"<<"\r"<<std::endl;

 ofile<<"//----------------------------------------------------------------------------------------------------"<<"\r"<<std::endl;
 ofile<<"//������� ��������� �������"<<"\r"<<std::endl;
 ofile<<"//----------------------------------------------------------------------------------------------------"<<"\r"<<std::endl;

 ofile<<"static float NeuronFunction(float value)"<<"\r"<<std::endl;
 ofile<<"{"<<"\r"<<std::endl;
 ofile<<" return(1.0f/(1.0f+(float)(exp(-value))));"<<"\r"<<std::endl;
 ofile<<"}"<<"\r"<<std::endl;
 ofile<<""<<"\r"<<std::endl;

 ofile<<"//----------------------------------------------------------------------------------------------------"<<"\r"<<std::endl;
 ofile<<"//������� ������� ���������"<<"\r"<<std::endl;
 ofile<<"//----------------------------------------------------------------------------------------------------"<<"\r"<<std::endl;

 ofile<<"void CreateAnswer(void)"<<"\r"<<std::endl;
 ofile<<"{"<<"\r"<<std::endl;
 ofile<<" for(size_t n=0;n<"<<(layers-1)<<";n++)"<<"\r"<<std::endl;
 ofile<<" {"<<"\r"<<std::endl;
 ofile<<"  float *vector_h_current=VectorHPtr[n];"<<"\r"<<std::endl;
 ofile<<"  float *vector_h_next=VectorHPtr[n+1];"<<"\r"<<std::endl;
 ofile<<"  const float *vector_b_current=VectorBPtr[n];"<<"\r"<<std::endl;
 ofile<<"  const float *matrix_w_current=MatrixWPtr[n];"<<"\r"<<std::endl;
 ofile<<""<<"\r"<<std::endl;
 ofile<<"  size_t mx=NeuronInLayer[n];//������ ������� �� x"<<"\r"<<std::endl;
 ofile<<"  size_t my=NeuronInLayer[n+1];//������ ������� �� y"<<"\r"<<std::endl;
 ofile<<""<<"\r"<<std::endl;
 ofile<<"  //���������� ������ ������� �� ������� �������, ����������� ������ �������� � ����������� ������� ��������� �������"<<"\r"<<std::endl;
 ofile<<"  for(size_t y=0;y<my;y++)"<<"\r"<<std::endl;
 ofile<<"  {"<<"\r"<<std::endl;
 ofile<<"   float value=0;"<<"\r"<<std::endl;
 ofile<<"   for(size_t x=0;x<mx;x++)"<<"\r"<<std::endl;
 ofile<<"   {"<<"\r"<<std::endl;
 ofile<<"    float m_value=matrix_w_current[mx*y+x];"<<"\r"<<std::endl;
 ofile<<"    float v_value=vector_h_current[x];"<<"\r"<<std::endl;
 ofile<<"    value+=m_value*v_value;"<<"\r"<<std::endl;
 ofile<<"   }"<<"\r"<<std::endl;
 ofile<<"   vector_h_next[y]=NeuronFunction(value+vector_b_current[y]);"<<"\r"<<std::endl;
 ofile<<"  }"<<"\r"<<std::endl;
 ofile<<" }"<<"\r"<<std::endl;
 ofile<<"}"<<"\r"<<std::endl;
 
 ofile.close();
 return(true);
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
bool CNeuroNet::Save(IDataStream *iDataStream_Ptr)
{
 //��������� ���������� �������� � �����
 uint32_t layers=NeuronInLayers.size();
 iDataStream_Ptr->SaveUInt32(layers);
 for(uint32_t n=0;n<layers;n++) iDataStream_Ptr->SaveUInt32(NeuronInLayers[n]);
 //��������� ���� � �������� ��������
 uint32_t size;
 size=cMatrix_W.size();
 iDataStream_Ptr->SaveUInt32(size);
 for(uint32_t n=0;n<size;n++) cMatrix_W[n].Save(iDataStream_Ptr);

 size=cVector_B.size();
 iDataStream_Ptr->SaveUInt32(size);
 for(uint32_t n=0;n<size;n++) cVector_B[n].Save(iDataStream_Ptr);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
bool CNeuroNet::Load(IDataStream *iDataStream_Ptr)
{
 //��������� ���������� �������� � �����
 uint32_t layers=iDataStream_Ptr->LoadUInt32();
 std::vector<size_t> neuron_in_layers(layers);
 for(uint32_t n=0;n<layers;n++)  neuron_in_layers[n]=iDataStream_Ptr->LoadUInt32();  
 //�������������� ���������
 Create(neuron_in_layers);
 
 //��������� ���� � �������� ��������
 uint32_t size;
 size=iDataStream_Ptr->LoadUInt32();
 for(uint32_t n=0;n<size;n++) cMatrix_W[n].Load(iDataStream_Ptr);

 size=iDataStream_Ptr->LoadUInt32();
 for(uint32_t n=0;n<size;n++) cVector_B[n].Load(iDataStream_Ptr);
 return(true);
}