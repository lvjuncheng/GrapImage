#ifndef _GrayDreconstruction_H
#define _GrayDreconstruction_H

#include <cv.h>
#include <vector>
#include "./BinRecon/BinStereoBuild.h"
#include "./PhaseUnwrap/sn3DStereoDefine.h"

//using namespace cv;

class GrayDreconstruction
{
public:
	GrayDreconstruction(){}
	~GrayDreconstruction(){}

public:                                                          
	// 图像宽度 高度参数
	int ImageWidth,ImageHeight;
	

public:
	// 逐像素获取格雷码绝对相位
	void GrayCode(const std::vector<cv::Mat> &CvImages, const int &i, const int &j, const vector<int> &m_grayTable0, const vector<int> &m_grayTable1, const int &TableNums, double &graycode);
	
	// 功能 解相对相位
	void getWrappedPhase(const std::vector<cv::Mat> &CvImages, int i, int j, LY3DStereoBuild::ScanImages& scanImages, double &Phase, double &MaskImage, uchar &Mask, double* sinValue, double* cosValue, int grayTh, int Surth, bool useMask);
	
	// 中值滤波-1
	double Median(const double &n0, const double &n1, const double &n2, const double &n3, const double &n4, const double &n5,
		const double &n6, const double &n7, const double &n8, const double &n9, const double &n10);
	
	// 功能 解相位后进行中值滤波来平滑数据
	void MedianFlitering(cv::Mat &AbsData, double* PhaseData);
	
	// 功能 执行格雷码解相位的主函数
	void GrayPahse(LY3DStereoBuild::ScanImages& scanImages, double* PhaseData, double* Contra, unsigned char* MaskData, int Width, int Height, int grayTh, int Surth, bool useMask);
	
	// 功能 执行图像归一化
	void ImageNormal(LY3DStereoBuild::ScanImages& scanImages, std::vector<cv::Mat> &CvImages);
	
	// 功能 测试用 输出数据到图像
	void GetPhasePic(double* PhaseData);
	void GetPhasePic(unsigned char* MaskData);
	void GetContraPic(double* ContraData);

	// 多曝光测试
	// 多曝光下的多次相位求解
	void getPhaseMultExp(std::vector<cv::Mat> &CvImages, LY3DStereoBuild::ScanImages& scanImages, cv::Mat &PhaseData, cv::Mat &MaskData, cv::Mat &ContraData, cv::Mat &GrayCodeData,
		double* sinValue, double* cosValue, int grayTh, int Surth, bool useMask);

	// 功能 多曝光下的相位求解-格雷码
	void GrayPahseMultiExp(std::vector<LY3DStereoBuild::ScanImages> &scanImages, double* PhaseData, double* Contra, unsigned char* MaskData, int Width, int Height, int grayTh, int Surth);

	// 功能 多曝光相位融合
	void GetFusePhaseDataMult(const std::vector<cv::Mat> &PhaseDataMultiExp, const std::vector<cv::Mat> &ContraDataMultiExp,
		const std::vector<cv::Mat> &MaskDataMultiExp, const std::vector<cv::Mat> &GrayDataMultiExp, cv::Mat &AbsPhase, uchar *MaskData);

	void FindMaxContra(double *ContraData, uchar *MaskData, int &Index, const int &ExpNums);
};

inline void GrayDreconstruction::FindMaxContra(double *ContraData, uchar *MaskData, int &Index, const int &ExpNums)
{
	double ContraTemp = -1;
	for (int i = 0; i < ExpNums;i++)
	{
		if (ContraTemp<ContraData[i] && MaskData[i])
		{
			Index = i;
			ContraTemp = ContraData[i];
		}
	}
}
#endif