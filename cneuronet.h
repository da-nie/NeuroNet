#ifndef C_NEURO_NET_H
#define C_NEURO_NET_H

//****************************************************************************************************
//����� ���������
//****************************************************************************************************

//���� 0 - �������, ��������� ���� - ��������

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <vector>
#include <string>
#include "cmatrix.h"
#include "cvector.h"
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
class CNeuroNetTrainer;

//****************************************************************************************************
//����� ���������
//****************************************************************************************************
class CNeuroNet
{
 friend class CNeuroNetTrainer;

 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
 private:
  //-����������-----------------------------------------------------------------------------------------	 
  std::vector<size_t> NeuronInLayers;//���������� �������� � �����
  std::vector<CMatrix> cMatrix_W;//����
  std::vector<CVector> cVector_B;//������
  std::vector<CVector> cVector_H;//�������� ����� ������������� �������
  std::vector<CVector> cVector_Z;//�������� �� ������������� �������

  //��������������� ������� � �������
  std::vector<CVector> cVector_TmpZ;
 public:
  //-�����������----------------------------------------------------------------------------------------
  CNeuroNet(void);
  //-����������-----------------------------------------------------------------------------------------
  ~CNeuroNet();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  void Create(const std::vector<size_t> &neuron_in_layers);//������� ���������
  void Reset(void);//�������� ��������� � �������� ���������
  void GetAnswer(const CVector &input,CVector &output);//��������� ��������� ������ ���������
  bool Export(const std::string &file_name);//�������������� ���������

  bool Save(IDataStream *iDataStream_Ptr);//��������� ���������
  bool Load(IDataStream *iDataStream_Ptr);//��������� ���������

 private:
  //-�������� �������-----------------------------------------------------------------------------------  
  double GetRandValue(double max_value);//��������� �����
  double NeuronFunction(double value);//������� ��������� �������
  double NeuronFunctionDifferencial(double value);//����������� ������� ��������� �������
  void Forward(void);//��������� ������ ������ �� ����

};

#endif
