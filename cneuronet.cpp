//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include "cneuronet.h"

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
 cMatrix_dW.resize(layers-1);

 cVector_B.resize(layers-1);
 cVector_dB.resize(layers-1);

 for(size_t n=0;n<layers-1;n++)
 {
  cMatrix_W[n]=CMatrix(NeuronInLayers[n+1],NeuronInLayers[n]);
  cMatrix_dW[n]=CMatrix(NeuronInLayers[n+1],NeuronInLayers[n]);

  cVector_B[n]=CVector(NeuronInLayers[n+1]);
  cVector_dB[n]=CVector(NeuronInLayers[n+1]);
 }

 cVector_H.resize(layers);
 cVector_Z.resize(layers);
 cVector_Delta.resize(layers);

 for(size_t n=0;n<layers;n++)
 {
  cVector_H[n]=CVector(NeuronInLayers[n]);
  cVector_Z[n]=CVector(NeuronInLayers[n]);

  cVector_Delta[n]=CVector(NeuronInLayers[n]);
 }

 cVector_Error=CVector(NeuronInLayers[layers-1]);

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
//������� ���������
//----------------------------------------------------------------------------------------------------
double CNeuroNet::Training(const std::vector<std::pair<CVector,CVector>> &image,double speed,double max_cost,size_t max_iteration)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return(0);

 double cost=0;
 size_t training_amount=image.size();

 double k=speed*(1.0/static_cast<double>(training_amount));

 while(max_iteration>0)
 {
  max_iteration--;
  //�������������� �������� � ����� � ���������
  for(size_t n=0;n<layers-1;n++)
  {
   cMatrix_dW[n].Zero();
   cVector_dB[n].Zero();
  }
  cost=0;
  //������� ����
  for(size_t index=0;index<training_amount;index++)
  {
   const std::pair<CVector,CVector> &pair=image[index];
   const CVector &cVector_Input=pair.first;
   const CVector &cVector_Output=pair.second;

   cVector_H[0]=cVector_Input;//������� ������
   //��������� ����
   for(size_t n=0;n<layers-1;n++)
   {
    cVector_Z[n+1]=cMatrix_W[n]*cVector_H[n]+cVector_B[n];
    //�������� ������� ���������
    for(size_t y=0;y<cVector_Z[n+1].GetSize();y++)
    {
     double v=cVector_Z[n+1].GetElement(y);
     cVector_H[n+1].SetElement(y,NeuronFunction(v));
    }
   }
   //�������� ������   
   cVector_Error=cVector_Output-cVector_H[layers-1];//������ ������
   //��������� ������� ���������
   for(size_t y=0;y<cVector_Error.GetSize();y++) cost+=cVector_Error.GetElement(y)*cVector_Error.GetElement(y);
   
   //���������� ������ ���������� ����
   for(size_t y=0;y<cVector_Error.GetSize();y++)
   {
    double v=cVector_Error.GetElement(y);
    double z=cVector_Z[layers-1].GetElement(y);
    cVector_Delta[layers-1].SetElement(y,-v*NeuronFunctionDifferencial(z));
   }
   //��������� ������ ���� ���� (������ �������� ���� �� �����)
   for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
   {
    cVector_Delta[n]=cMatrix_W[n].Transpose()*cVector_Delta[n+1];
    for(size_t y=0;y<cVector_Delta[n].GetSize();y++)
    {
     double v=cVector_Delta[n].GetElement(y);
     double z=cVector_Z[n].GetElement(y);
     cVector_Delta[n].SetElement(y,v*NeuronFunctionDifferencial(z));
    }
   }
   //��������� �������� � ����� � ���������
   for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
   {
    CMatrix cMatrix(cVector_Delta[n+1].GetSize(),cVector_H[n].GetSize());
    cMatrix=(cVector_Delta[n+1])&(cVector_H[n]);
    cMatrix_dW[n]=cMatrix_dW[n]+cMatrix;
     
    cVector_dB[n]=cVector_dB[n]+cVector_Delta[n+1];
   }     
  }
  if (cost<=max_cost) break;//�������� ��������

  //��������� ����������� �����
  for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
  {
   cMatrix_W[n]=cMatrix_W[n]-k*cMatrix_dW[n];
   cVector_B[n]=cVector_B[n]-k*cVector_dB[n];
  }
 }
 return(cost);
}
//----------------------------------------------------------------------------------------------------
//��������� ��������� ������ ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNet::GetAnswer(const CVector &input,CVector &output)
{
 size_t layers=NeuronInLayers.size();
 if (layers<2) return;
 cVector_H[0]=input;//������� ������
 //��������� ����
 for(size_t n=0;n<layers-1;n++)
 {
  cVector_Z[n+1]=cMatrix_W[n]*cVector_H[n]+cVector_B[n];
  //�������� ������� ���������
  for(size_t y=0;y<cVector_Z[n+1].GetSize();y++)
  {
   double v=cVector_Z[n+1].GetElement(y);
   cVector_H[n+1].SetElement(y,NeuronFunction(v));
  }
 }
 output=cVector_H[layers-1];
}