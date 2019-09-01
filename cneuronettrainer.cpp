//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include "cneuronettrainer.h"

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
CNeuroNetTrainer::CNeuroNetTrainer(void)
{ 
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CNeuroNetTrainer::~CNeuroNetTrainer()
{
}

//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//������������� �������� � ����� � ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::InitDeltaWeighAndBias(void)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;
 //�������������� �������� � ����� � ���������
 for(size_t n=0;n<layers-1;n++)
 {
  cMatrix_dW[n].Zero();
  cVector_dB[n].Zero();
 }
}

//----------------------------------------------------------------------------------------------------
//��������� ������ ���������� ���� � ������� ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::CreateOutputLayerDeltaAndCost(const CVector &cVector_Output,double &cost)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;

 //�������� ������   
 //cVector_Error.Move(cVector_Output-cVector_H[layers-1]);//������ ������
 CVector::Sub(cVector_Error,cVector_Output,cNeuroNet_Ptr->cVector_H[layers-1]);//������ ������

 //���������� ������ ���������� ����
 for(size_t y=0;y<cVector_Error.GetSize();y++)
 {
  cost+=cVector_Error.GetElement(y)*cVector_Error.GetElement(y);//��������� ������� ���������
  double v=cVector_Error.GetElement(y);
  double z=cNeuroNet_Ptr->cVector_Z[layers-1].GetElement(y);
  cVector_Delta[layers-1].SetElement(y,-v*cNeuroNet_Ptr->NeuronFunctionDifferencial(z));
 }
}
//----------------------------------------------------------------------------------------------------
//������ ������ ���������� ����
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::SetOutputDelta(const CVector &cVector_Delta_Set)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;
 if (cVector_Delta[layers-1].GetSize()!=cVector_Delta_Set.GetSize())
 {
  throw("������ ����������� �������� � CNeuroNetTrainer::SetOutputDelta");
 }
 cVector_Delta[layers-1]=cVector_Delta_Set;
}
//----------------------------------------------------------------------------------------------------
//��������� ���� ������ ��������
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::OneStep(void)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;

 //��������� ������ ���� ���� (������ �������� ���� ����� �� �����)
 for(size_t n=layers-2;n>0;n--)
 {
  //cVector_Delta[n]=cMatrix_W[n].Transpose()*cVector_Delta[n+1];
  CMatrix::Transponse(cMatrix_TmpW[n],cNeuroNet_Ptr->cMatrix_W[n]);
  CMatrix::Mul(cVector_Delta[n],cMatrix_TmpW[n],cVector_Delta[n+1]);

  for(size_t y=0;y<cVector_Delta[n].GetSize();y++)
  {
   double v=cVector_Delta[n].GetElement(y);
   double z=cNeuroNet_Ptr->cVector_Z[n].GetElement(y);
   cVector_Delta[n].SetElement(y,v*cNeuroNet_Ptr->NeuronFunctionDifferencial(z));
  }
 }

 //��������� �������� � ����� � ���������
 for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
 {
  CMatrix::Mul(cMatrix_TmpDelta[n],cVector_Delta[n+1],cNeuroNet_Ptr->cVector_H[n]);
  CMatrix::Add(cMatrix_dW[n],cMatrix_dW[n],cMatrix_TmpDelta[n]);
   
  CVector::Add(cVector_dB[n],cVector_dB[n],cVector_Delta[n+1]);

/*  CMatrix cMatrix(cVector_Delta[n+1].GetSize(),cVector_H[n].GetSize());
  cMatrix.Move((cVector_Delta[n+1])&(cVector_H[n]));
  cMatrix_dW[n].Move(cMatrix_dW[n]+cMatrix);
     
  cVector_dB[n].Move(cVector_dB[n]+cVector_Delta[n+1]);
 */
 }     
}

//----------------------------------------------------------------------------------------------------
//��������� ������ �������� ����
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::CreateDeltaInputLayer(void)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;
 CMatrix::Transponse(cMatrix_TmpW[0],cNeuroNet_Ptr->cMatrix_W[0]);
 CMatrix::Mul(cVector_Delta[0],cMatrix_TmpW[0],cVector_Delta[1]);

 for(size_t y=0;y<cVector_Delta[0].GetSize();y++)
 {
  double v=cVector_Delta[0].GetElement(y);
  double z=cNeuroNet_Ptr->cVector_Z[0].GetElement(y);
  //cVector_Delta[0].SetElement(y,v*cNeuroNet_Ptr->NeuronFunctionDifferencial(z));
 }
}   

//----------------------------------------------------------------------------------------------------
//�������� ���� � ��������
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::UpdateWeightAndBias(double k)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;

 //��������� ����������� �����
 for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
 {
  CMatrix::Mul(cMatrix_Tmp[n],k,cMatrix_dW[n]);
//  cMatrix_W[n].Move(cMatrix_W[n]-k*cMatrix_dW[n]);
//  cVector_B[n].Move(cVector_B[n]-k*cVector_dB[n]);

  CMatrix::Sub(cNeuroNet_Ptr->cMatrix_W[n],cNeuroNet_Ptr->cMatrix_W[n],cMatrix_Tmp[n]);

  CVector::Mul(cVector_Tmp[n],k,cVector_dB[n]);
  CVector::Sub(cNeuroNet_Ptr->cVector_B[n],cNeuroNet_Ptr->cVector_B[n],cVector_Tmp[n]);
 }
}

//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//������������ � ���������
//----------------------------------------------------------------------------------------------------
void CNeuroNetTrainer::Connect(std::shared_ptr<CNeuroNet> cNeuroNet_Ptr_Set)
{
 cNeuroNet_Ptr=cNeuroNet_Ptr_Set;

 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return;
  
 cMatrix_dW.resize(layers-1);
 cVector_dB.resize(layers-1);

 for(size_t n=0;n<layers-1;n++)
 {
  cMatrix_dW[n]=CMatrix(cNeuroNet_Ptr->NeuronInLayers[n+1],cNeuroNet_Ptr->NeuronInLayers[n]);
  cVector_dB[n]=CVector(cNeuroNet_Ptr->NeuronInLayers[n+1]);
 }
 cVector_Delta.resize(layers);

 for(size_t n=0;n<layers;n++)
 {
  cVector_Delta[n]=CVector(cNeuroNet_Ptr->NeuronInLayers[n]);
 }

 cVector_Error=CVector(cNeuroNet_Ptr->NeuronInLayers[layers-1]);

 cMatrix_TmpW.resize(layers-1);
 for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
 {
  cMatrix_TmpW[n]=CMatrix(cNeuroNet_Ptr->cMatrix_W[n].GetSizeX(),cNeuroNet_Ptr->cMatrix_W[n].GetSizeY());
 }
 cMatrix_TmpDelta.resize(layers-1);
 for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
 {
  cMatrix_TmpDelta[n]=CMatrix(cVector_Delta[n+1].GetSize(),cNeuroNet_Ptr->cVector_H[n].GetSize());
 }

 cMatrix_Tmp.resize(layers-1);
 cVector_Tmp.resize(layers-1);
 for(size_t n=layers-2,m=layers-1;m>0;n--,m--)
 {
  cMatrix_Tmp[n]=CMatrix(cMatrix_dW[n].GetSizeY(),cMatrix_dW[n].GetSizeX());
  cVector_Tmp[n]=CVector(cVector_dB[n].GetSize());
 }   
}

//----------------------------------------------------------------------------------------------------
//������� ���������
//----------------------------------------------------------------------------------------------------
double CNeuroNetTrainer::Training(const std::vector<std::pair<CVector,CVector>> &image,double speed,double max_cost,size_t max_iteration)
{
 size_t layers=cNeuroNet_Ptr->NeuronInLayers.size();
 if (layers<2) return(0);

 double cost=0;
 size_t training_amount=image.size();

 double k=speed*(1.0/static_cast<double>(training_amount));

 while(max_iteration>0)
 {
  max_iteration--;
  //�������������� �������� � ����� � ���������
  InitDeltaWeighAndBias();
  cost=0;
  //������� ����
  for(size_t index=0;index<training_amount;index++)
  {
   const std::pair<CVector,CVector> &pair=image[index];
   const CVector &cVector_Input=pair.first;
   const CVector &cVector_Output=pair.second;

   cNeuroNet_Ptr->SetInput(cVector_Input);//������� ������
   //��������� ����
   cNeuroNet_Ptr->Forward();
   //��������� ������ ���������� ����
   CreateOutputLayerDeltaAndCost(cVector_Output,cost);
   //��������� ���������� �����
   OneStep();
  }
  if (cost<=max_cost) break;//�������� ��������
  UpdateWeightAndBias(k);
 }
 return(cost);
}