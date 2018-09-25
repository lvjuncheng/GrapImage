/********************************************************************
* Copyright(c) 2018,  Shining 3D Technology
* All rights reversed
*
* �ļ�����: BinStereoBuild
* �ļ���־: Ly
* ժ    Ҫ: ����λ��غ���
* ��Ŀ����: ������άɨ��ϵͳ
* ����ϵͳ: MS WIndows XP SP3/MS Windows 7 Windows 10
* ����ƽ̨: MS VS2013
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
	int ImageHeigh;                                                                           // ͼ����� �߶ȺͿ��
	int ImageWidth;

	bool is_Debug;                                                                            // �Ƿ�ʼdebugģʽ ��������ں��ļ�
	bool use_BW;

public:
	void getWrappedPhase(int stepNum, int moveCount, ScanImages& scanImages,                  // ���������λ
		std::vector<cv::Mat> &WrappedPhase, std::vector<cv::Mat> &ContrPattern,               // ע�� ����������λ��Ҫ��ͬƵ�����Ʋ�����ͬ
		float *NormalImages);

	void heterodyneSubtract(std::vector<cv::Mat> &wrappedSubArray, cv::Mat &unwrapped,        // ���ھ�����λ���-step1
		int stepNum, std::vector<cv::Mat> &WrappedPhase);

	void UnwrapPhase(cv::Mat &wrapped, cv::Mat &unwrapped, double &ratio);                    // ���ھ�����λ���-step2

	void UnwrapPhaseFast(const std::vector<cv::Mat> &wrap, cv::Mat &unwrapped,                // ���ھ�����λ���-step3
		cv::Mat &unwrapSum, double *ratios, int stepNum, double *unphase);

	void heterodyneUnwrapPhase(int stepNum, double *frangeNum, double *unphase,               // ���ھ�����λ���-�ܵĲ���
		std::vector<cv::Mat> &WrappedPhase);

	void autoGetMaskDense(double grayTh, int stepNum, unsigned char *maskImage,               // ������������µ�mask��ȡ
		std::vector<cv::Mat> &ContrPattern);
	
	void generalUnwrapPhase(int stepNum, int phaseTh, double *unphase,
		unsigned char *maskImage, float *pixelWidth, std::vector<cv::Mat> &WrappedPhase);     // �����ƽ���λ

	int NeighborSearch(const int &i, const int &j, unsigned char *maskImage);                 // ����������ȷ��mask��Ե ����0��ʶmask���ڱ�Ե 1234��ʶmask�ڱ�Ե

	void NeighborDelete(unsigned char *maskImage);                                            // mask ����ɾ��

	void autoGetMaskSparse(double grayTh, unsigned char *maskImage,                           // ������mask��ȡ
		std::vector<cv::Mat> &ContrPattern);

	void SetMaskZero(int ParamWidth, int ParamHeigh,                                          // binning����� maskȫ������ ������Ч��λ����1
		unsigned char *maskImage, double *unphase);
	
	void getScaleChangeL(int ParamWidth, int ParamHeigh, double *BinningPhase,                // binning����� ��λ���� 
		double *Phase, unsigned char *BinningMask, unsigned char *Mask);

	void getScaleChangeR(int ParamWidth, int ParamHeigh, double *BinningPhase,                // binning����� ��λ���� 
		double *Phase, unsigned char *BinningMask, unsigned char *Mask);

	void getPaseId(int ParamWidth, int ParamHeigh, int &rangeWidth, int &rangeHeigh,          // Gpu�� ��binning��binning״̬�µ�IDlist��� 
		int *IDList, float *xpt, float *ypt, float *zpt, int *&rangIDlist,                    // ע�� ͼ��ĸ߶ȺͿ��ʹ���������������
		double *&rangeX, double *&rangeY, double *&rangeZ, int &rangeNums, int RangeSample);  
																							  
	void Unphase(double *unphase, unsigned char *maskImage, ScanImages& scanImages,           // ���ܣ�������������λ
		PhaseParam& param, const double &phaseTh, const double &grayTh,                       // out unphase maskImage �������λ����,mask����
		const bool &Normalize_WB);                                                            // phaseTh ��λ��ֵ����������maskImage
	                                                                                          // grayTh �Ҷ���ֵ����������maskImage
	void BWImageProcess(ScanImages &scanImages, float *NormalImages);                         // �ڰ�ͼ��һ�������ڰ�ͼģʽ��ʹ��
																							  
	void SetPhaseDebug(bool isDebug);                                                         // �趨�Ƿ�Ϊdebugģʽ���е���

	void getWrappedPhase(int stepNum, int moveCount, int *moveNums, ScanImages& scanImages,   // DLP����ʹ��
		std::vector<cv::Mat> &WrappedPhase, std::vector<cv::Mat> &ContrPattern);              // �ⲻͬƵ�����Ʋ�����ͬ�������λͼ  
																							  
	void Unphasedlp(double *unphase, unsigned char *maskImage, ScanImages& scanImages,        // �ⲻͬƵ�����Ʋ�����ͬ����λͼ
		PhaseParam& param, double phaseTh, double grayTh);									  
																							  
	void GetPhasePic(float* PhaseData);                                                       // ������ ��������(float)
																							  
	void GetPhasePic(unsigned char* PhaseData);                                               // ������ ��������(unsigned char)

	void SavePhase(int index, cv::Mat data);                                            

	void SavePhaseTxt(int index, cv::Mat data);              
};
#endif 