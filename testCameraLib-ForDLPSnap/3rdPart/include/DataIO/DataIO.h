// ==========================================================================
// File name:	DataIO.h
//
// Copyright (C), Shining 3D Tech. Co., Ltd.
//
// Description:	条纹重建基础IO操作。
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

// DLP对应点数据类型
struct CorresPoints
{
	double xl;
	double yl;
	double xr;
	double yr;
};

// 基础的重建类型
enum ScanType{
	ScanType_Proj,                                      // 带投影的重建
	ScanType_Proj_Bin,                                  // 带投影的重建+相机binning状态
	ScanType_Bino,                                      // 双目重建，基于分块
	ScanType_Bino_Bin,                                  // 双目重建，基于分块，相机binning状态
	ScanType_BinoFast,                                  // 双目重建，基于二分法全局搜索对应点
	ScanType_BinoFast_Bin,                              // 双目重建，基于二分法全局搜索对应点，相机binning状态

	///////////////////////////////////////
	ScanType_Bino_DLP,                                  // DLP用，需要输出对应点数据
	///////////////////////////////////////

	ScanType_Gray,                                      // 格雷码重建，基于分块
	ScanType_Gray_bin,                                  // 格雷码重建，基于分块，相机binning
	ScanType_Gray_Fast,                                 // 格雷码重建，二分法全局搜索对应点
	ScanType_Gray_Fast_bin                              // 格雷码重建，二分法全局搜索对应点,相机binning
};

class DataIO
{
public:
	DataIO(){}
	~DataIO(){}

public:
	// 读入标定数据
	bool ReadCameraCalibrateTxt(const char* pNamePath, double* fc, double* cc, double* kc, double& skew, double* rotate, double* trans);

	void landdelete(int*& idImage, int &count, double*& X, double*& Y, double*& Z, int width, int heigh, const int findRange, const float disThread, const int numThread);
};
#endif