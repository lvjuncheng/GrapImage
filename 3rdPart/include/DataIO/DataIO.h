// ==========================================================================
// File name:	DataIO.h
//
// Copyright (C), Shining 3D Tech. Co., Ltd.
//
// Description:	�����ؽ�����IO������
//
// Histroy:		
//	- Created : 2018/04/24  
//
// ============================================================================
#ifndef _DataIO_H
#define _DataIO_H

#include <sstream>
#include <fstream>
#include <iostream>

// DLP��Ӧ����������
struct CorresPoints
{
	double xl;
	double yl;
	double xr;
	double yr;
};

// �������ؽ�����
enum ScanType{
	ScanType_Proj,                                      // ��ͶӰ���ؽ�
	ScanType_Proj_Bin,                                  // ��ͶӰ���ؽ�+���binning״̬
	ScanType_Bino,                                      // ˫Ŀ�ؽ������ڷֿ�
	ScanType_Bino_Bin,                                  // ˫Ŀ�ؽ������ڷֿ飬���binning״̬
	ScanType_BinoFast,                                  // ˫Ŀ�ؽ������ڶ��ַ�ȫ��������Ӧ��
	ScanType_BinoFast_Bin,                              // ˫Ŀ�ؽ������ڶ��ַ�ȫ��������Ӧ�㣬���binning״̬

	///////////////////////////////////////
	ScanType_Bino_DLP,                                  // DLP�ã���Ҫ�����Ӧ������
	///////////////////////////////////////

	ScanType_Gray,                                      // �������ؽ������ڷֿ�
	ScanType_Gray_bin,                                  // �������ؽ������ڷֿ飬���binning
	ScanType_Gray_Fast,                                 // �������ؽ������ַ�ȫ��������Ӧ��
	ScanType_Gray_Fast_bin                              // �������ؽ������ַ�ȫ��������Ӧ��,���binning
};

class DataIO
{
public:
	DataIO(){}
	~DataIO(){}

public:
	// ����궨����
	bool ReadCameraCalibrateTxt(const char* pNamePath, double* fc, double* cc, double* kc, double& skew, double* rotate, double* trans);

	void landdelete(int*& idImage, int &count, double*& X, double*& Y, double*& Z, int width, int heigh, const int findRange, const float disThread, const int numThread);
};
#endif