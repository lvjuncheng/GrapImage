/********************************************************************
* Copyright(c) 2018,  Shining 3D Technology
* All rights reversed
*
* 文件名称: BinStereoBuild
* 文件标志: Ly
* 摘    要: 解相位相关函数
* 项目名称: 先临三维扫描系统
* 操作系统: MS WIndows XP SP3/MS Windows 7 Windows 10
* 编译平台: MS VS2013
*********************************************************************/
#ifndef _PhaseUnwrap_H
#define _PhaseUnwrap_H

#include "sn3DStereoDefine.h"
#include "sn3DImageData.h"
#include <vector>

#include "opencv2/opencv.hpp"
#include "cv.h"

using namespace LY3DStereoBuild;
using namespace sn3DCore;

class PhaseUnwrap
{
public:
	PhaseUnwrap();
	~PhaseUnwrap();

public:
	int ImageHeigh;                                                                           // 图像参数 高度和宽度
	int ImageWidth;

	bool is_Debug;                                                                            // 是否开始debug模式 输出到窗口和文件
	bool use_BW;

public:
	void getWrappedPhase(int stepNum, int moveCount, ScanImages& scanImages,                  // 计算相对相位
		std::vector<cv::Mat> &WrappedPhase, std::vector<cv::Mat> &ContrPattern,               // 注意 这里的相对相位需要不同频率相移步数相同
		float *NormalImages);

	void heterodyneSubtract(std::vector<cv::Mat> &wrappedSubArray, cv::Mat &unwrapped,        // 用于绝对相位求解-step1
		int stepNum, std::vector<cv::Mat> &WrappedPhase);

	void UnwrapPhase(cv::Mat &wrapped, cv::Mat &unwrapped, double &ratio);                    // 用于绝对相位求解-step2

	void UnwrapPhaseFast(const std::vector<cv::Mat> &wrap, cv::Mat &unwrapped,                // 用于绝对相位求解-step3
		cv::Mat &unwrapSum, double *ratios, int stepNum, double *unphase);

	void heterodyneUnwrapPhase(int stepNum, double *frangeNum, double *unphase,               // 用于绝对相位求解-总的步骤
		std::vector<cv::Mat> &WrappedPhase);

	void autoGetMaskDense(double grayTh, int stepNum, unsigned char *maskImage,               // 稠密条纹情况下的mask获取
		std::vector<cv::Mat> &ContrPattern);
	
	void generalUnwrapPhase(int stepNum, int phaseTh, double *unphase,
		unsigned char *maskImage, float *pixelWidth, std::vector<cv::Mat> &WrappedPhase);     // 粗条纹解相位

	int NeighborSearch(const int &i, const int &j, unsigned char *maskImage);                 // 领域搜索，确定mask边缘 返回0标识mask不在边缘 1234标识mask在边缘

	void NeighborDelete(unsigned char *maskImage);                                            // mask 邻域删除

	void autoGetMaskSparse(double grayTh, unsigned char *maskImage,                           // 粗条纹mask获取
		std::vector<cv::Mat> &ContrPattern);

	void SetMaskZero(int ParamWidth, int ParamHeigh,                                          // binning情况下 mask全部置零 后续有效点位再置1
		unsigned char *maskImage, double *unphase);
	
	void getScaleChangeL(int ParamWidth, int ParamHeigh, double *BinningPhase,                // binning情况下 相位扩充 
		double *Phase, unsigned char *BinningMask, unsigned char *Mask);

	void getScaleChangeR(int ParamWidth, int ParamHeigh, double *BinningPhase,                // binning情况下 相位扩充 
		double *Phase, unsigned char *BinningMask, unsigned char *Mask);

	void getPaseId(int ParamWidth, int ParamHeigh, int &rangeWidth, int &rangeHeigh,          // Gpu用 非binning和binning状态下的IDlist输出 
		int *IDList, float *xpt, float *ypt, float *zpt, int *&rangIDlist,                    // 注意 图像的高度和宽度使用正常的相机参数
		double *&rangeX, double *&rangeY, double *&rangeZ, int &rangeNums, int RangeSample);  
																							  
	void Unphase(double *unphase, unsigned char *maskImage, ScanImages& scanImages,           // 功能：解条纹数据相位
		PhaseParam& param, const double &phaseTh, const double &grayTh,                       // out unphase maskImage 输出的相位数据,mask数据
		const bool &Normalize_WB);                                                            // phaseTh 相位阈值，用来产生maskImage
	                                                                                          // grayTh 灰度阈值，用来产生maskImage
	void BWImageProcess(ScanImages &scanImages, float *NormalImages);                         // 黑白图归一化，带黑白图模式下使用
																							  
	void SetPhaseDebug(bool isDebug);                                                         // 设定是否为debug模式进行调试

	void getWrappedPhase(int stepNum, int moveCount, int *moveNums, ScanImages& scanImages,   // DLP测试使用
		std::vector<cv::Mat> &WrappedPhase, std::vector<cv::Mat> &ContrPattern);              // 解不同频率相移步数不同的相对相位图  
																							  
	void Unphasedlp(double *unphase, unsigned char *maskImage, ScanImages& scanImages,        // 解不同频率相移步数不同的相位图
		PhaseParam& param, double phaseTh, double grayTh);									  
																							  
	void GetPhasePic(float* PhaseData);                                                       // 测试用 导出数据(float)
																							  
	void GetPhasePic(unsigned char* PhaseData);                                               // 测试用 导出数据(unsigned char)

	void SavePhase(int index, cv::Mat data);                                            

	void SavePhaseTxt(int index, cv::Mat data);              
};
#endif 