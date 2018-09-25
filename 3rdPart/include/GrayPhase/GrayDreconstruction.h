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
	// ͼ���� �߶Ȳ���
	int ImageWidth,ImageHeight;
	

public:
	// �����ػ�ȡ�����������λ
	void GrayCode(const std::vector<cv::Mat> &CvImages, const int &i, const int &j, const vector<int> &m_grayTable0, const vector<int> &m_grayTable1, const int &TableNums, double &graycode);
	
	// ���� �������λ
	void getWrappedPhase(const std::vector<cv::Mat> &CvImages, int i, int j, LY3DStereoBuild::ScanImages& scanImages, double &Phase, double &MaskImage, uchar &Mask, double* sinValue, double* cosValue, int grayTh, int Surth, bool useMask);
	
	// ��ֵ�˲�-1
	double Median(const double &n0, const double &n1, const double &n2, const double &n3, const double &n4, const double &n5,
		const double &n6, const double &n7, const double &n8, const double &n9, const double &n10);
	
	// ���� ����λ�������ֵ�˲���ƽ������
	void MedianFlitering(cv::Mat &AbsData, double* PhaseData);
	
	// ���� ִ�и��������λ��������
	void GrayPahse(LY3DStereoBuild::ScanImages& scanImages, double* PhaseData, double* Contra, unsigned char* MaskData, int Width, int Height, int grayTh, int Surth, bool useMask);
	
	// ���� ִ��ͼ���һ��
	void ImageNormal(LY3DStereoBuild::ScanImages& scanImages, std::vector<cv::Mat> &CvImages);
	
	// ���� ������ ������ݵ�ͼ��
	void GetPhasePic(double* PhaseData);
	void GetPhasePic(unsigned char* MaskData);
	void GetContraPic(double* ContraData);

	// ���ع����
	// ���ع��µĶ����λ���
	void getPhaseMultExp(std::vector<cv::Mat> &CvImages, LY3DStereoBuild::ScanImages& scanImages, cv::Mat &PhaseData, cv::Mat &MaskData, cv::Mat &ContraData, cv::Mat &GrayCodeData,
		double* sinValue, double* cosValue, int grayTh, int Surth, bool useMask);

	// ���� ���ع��µ���λ���-������
	void GrayPahseMultiExp(std::vector<LY3DStereoBuild::ScanImages> &scanImages, double* PhaseData, double* Contra, unsigned char* MaskData, int Width, int Height, int grayTh, int Surth);

	// ���� ���ع���λ�ں�
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