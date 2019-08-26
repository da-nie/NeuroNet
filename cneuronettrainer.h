#ifndef C_NEURO_NET_TRAINER_H
#define C_NEURO_NET_TRAINER_H

//****************************************************************************************************
//����� �������� ���������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <vector>
#include <string>
#include "cmatrix.h"
#include "cvector.h"
#include "idatastream.h"
#include "cneuronet.h"

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//��������������� ����������
//****************************************************************************************************

//****************************************************************************************************
//����� �������� ���������
//****************************************************************************************************
class CNeuroNetTrainer
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------
  std::shared_ptr<CNeuroNet> cNeuroNet_Ptr;//��������� �� ����� ��������� ���������

  std::vector<CMatrix> cMatrix_dW;//��������� �����
  std::vector<CVector> cVector_dB;//�������� � �������
  std::vector<CVector> cVector_Delta;//������ ����
  CVector cVector_Error;//������ ������  

  //��������������� ������� � �������
  std::vector<CMatrix> cMatrix_TmpW;
  std::vector<CMatrix> cMatrix_TmpDelta;
  std::vector<CMatrix> cMatrix_Tmp;
  std::vector<CVector> cVector_Tmp;  
 public:
  //-�����������----------------------------------------------------------------------------------------
  CNeuroNetTrainer(void);
  //-����������-----------------------------------------------------------------------------------------
  ~CNeuroNetTrainer();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  void Connect(std::shared_ptr<CNeuroNet> cNeuroNet_Ptr_Set);//������������ � ���������
  double Training(const std::vector<std::pair<CVector,CVector>> &image,double speed,double max_cost,size_t max_iteration);//������� ���������
 private:
  //-�������� �������-----------------------------------------------------------------------------------  
};

#endif
