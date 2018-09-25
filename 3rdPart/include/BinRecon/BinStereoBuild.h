/********************************************************************
* Copyright(c) 2018,  Shining 3D Technology
* All rights reversed
*
* �ļ�����: BinStereoBuild
* �ļ���־: Ly
* ժ    Ҫ: ��դ��λ�ؽ�
* ��Ŀ����: ������άɨ��ϵͳ
* ����ϵͳ: MS WIndows XP SP3/MS Windows 7 Windows 10
* ����ƽ̨: MS VS2013
*********************************************************************/
#ifndef _StereoBuild_H
#define _StereoBuild_H

#include <iostream>
#include "sn3DImageData.h"

#include "tnt/tnt.h"
#include "tnt/tnt_linalg.h"
#include "./PhaseUnwrap/sn3DStereoDefine.h"
#include "common/baseType/Cloud.h"
#include "common/baseType/PointImage.h"

#include"./DataIO/DataIO.h"
// matrix inverse
template <class Real>
inline TNT::Matrix<Real> matrix_inv(TNT::Matrix<Real> &M)
{
	TNT::Linear_Algebra::LU<Real> lu(M);
	TNT::Matrix<Real> I(M.num_rows(), M.num_cols(), 0.0);
	for (int i = 0; i < M.num_cols(); i++)
	{
		I[i][i] = 1;
	}

	return lu.solve(I);
};

class CPhaseInterpolation;
class StreroRebuild
{
public:
	StreroRebuild();
	~StreroRebuild();

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////  ��ʼ���׶α���
	bool m_bParamLoad;
	int m_w;					                                                                              //ͼ����
	int m_h;					                                                                              //ͼ��߶�
	int m_size;					                                                                              							                                                    
	                                                                                                          //����ڲ�					                                                    
	double m_focalL[2];			                                                                              //���������
	double m_focalR[2];			                                                                              //���������
								                                                                              
	double m_principalL[2];		                                                                              //������λ��
	double m_principalR[2];		                                                                              //������λ��
								                                                                              
	double m_skewL;				                                                                              //����б����
	double m_skewR;				                                                                              //����б����
								                                                                              
	double m_distortCoeffL[5];                                                                                //���������
	double m_distortCoeffR[5];                                                                                //�һ�������
									                                                                          
	TNT::Matrix<double> m_inMatL;	                                                                          //���ڲξ���
	TNT::Matrix<double> m_inMatR;	                                                                          //���ڲξ���
	                                                                                                          //������	
	TNT::Matrix<double> m_rotate;	                                                                          //��ת����
	double m_trans[3];																                          
																					                          
	double m_invRotLeft[9];		                                                                              //��ת����
	double m_transLeft[3];

	double m_boxx[2];                                                                                         //��Ч����
	double m_boxy[2];
	double m_boxz[2];
	
	bool m_bBox;
	bool is_useGpu;

	////////////////////////////////////////////////////////////////////////////////////////////////////////// DLP������
	double *CX, *CY;                                                                                         //DLP �����Ӧ��

	void Fit(Sn3DAlgorithm::Cloud Data, std::vector<LY3DStereoBuild::DistorData> &vecPointTemp, std::vector<LY3DStereoBuild::DistorData> &projectpoints, int randomTime,
		int SampleRata, float planeThresholdCopy, bool isDebug);
	// ������λ��ͶӰ��ƽ����� �����ƽ�� ������DLP�������
	// unphase1:����� ��������λ
	// unphase2:����� ��������λ
	// width:ͶӰ�����
	// heigh:ͶӰ���߶�
	// phaseCycle:��λ���� �ٶ�ͶӰ�����Ϊ1920 ��ϸ������100������ ��ô�������Ƶ�phaseCycle����19.2
	// projectpoints:�����ά���ƽ���Ӧ��
	// maskLeft:����� mask
	// maskCross:����� mask
	// CX CY���������Ӧ��
	// nn ��Ӧ������
	// range �ؽ������������
	void Recognize(double *unphase1, double *unphase2, int width, int heigh, int phaseCycle, std::vector<LY3DStereoBuild::DistorData> &projectpoints, unsigned char* maskLeft, unsigned char* maskCross, double *CX, double *CY, int &nn, LY3DStereoBuild::ScanRange& range);

	////////////////////////////////////////////////////////////////////////////////////////////////////////// DLP������
public:
	/**  ��ʼ��,��������ʱ���ã������ظ���������һ�Ρ�                                                         ��ʽ����
	*  @para[in] w,h �����߲���
	*  @para[in] fcl,ccl,alphal,kcl,rcl,tcl,fcr,ccr,alphar,kcr,rcr,tcr �궨�����
	*/
	bool InitParameter(int w, int h,
		double *fcl, double *ccl, double alphal, double *kcl, double *rcl, double *tcl,
		double *fcr, double *ccr, double alphar, double *kcr, double *rcr, double *tcr);
	
	void fundamentMat(TNT::Matrix<double> &fundM);                                                            //�����������
	
	bool epipolarPoint(TNT::Matrix<double> &f);                                                               //���㼫��
	
	void FundamentMat();                                                                                      // �������󼰼�����㣬�ⲿ����
	
	bool computeDistortion(double *xlist, double *ylist, int n,                                               // �������
		double k1, double k2, double k3, double p1, double p2);
	
	bool normalize(double *xlist, double *ylist, int n,                                                       // ����ת��������£�ȥ����
		double *focal, double *principal, double *distortCoef, double skew);
	
	bool undistorted(double *xlist, double *ylist, int w, int h,                                              // ��ʼ����㣬����������ת����ȥ����
		double *focal, double *principal, double *distortCoef, double skew);
	
	bool computeEpipolar(TNT::Matrix<double> &f,                                                              // ���㼫�����ݼ��ֿ�����ĵ�����
		double *xUnLeft, double *yUnLeft, double *xUnRight, double *yUnRight,
		double *aRightL, double *bRightL, double *bRightE,
		double *aLeftR, double *bLeftR, double *bLeftE);
	
	/**  �������ұ�,һ���ڱ궨��ɺ����,���������ļ���������Ҫ�������ɡ�
	*  @para[in] name ���ұ��ļ�·��
	*/
	bool CreateUndistortTable(const char *name);                                                              // �������ұ�
	
	bool LoadUndistortTable(const char *pathName);                                                            // �ؽ��׶κ���

	bool ReleaseUndistortTable();

	bool SetBoxMask(bool bset);

	bool UnwarpPhase(int w, int h,
		double* phiLeft, unsigned  char *pLeft, double *phiRight, unsigned char *pRight);

	bool CheckBoxMask(int w, int h, int *idImage, int count, double *x, double *y, double *z);

	bool buildStereo(int n, double *X, double *Y, double *Z, double*xl, double*yl, double*xr, double *yr,ScanType type);

	bool isInBox(double x, double y, double z);

	// �Ƿ�ʼ���
	void SetBinRebuildDebug(bool isDebug);
	// �ؽ�����������
	//void PointsImageGPU(double*& X, double*& Y, double*& Z, int*& idImage, int &count, const std::vector<float3> &PointData);

	/**  ִ���ؽ����� �ؽ���������
	*  @para[in] w,h ���������ע��binning�����������ֵӦ��һ��
	*  @para[in] phiLeft,pLeft,phiRight,pRight ���������λmask����
	*  @para[out] idImage ���������ݶ�Ӧ��id
	*
	*/
	bool StartBuild(int w, int h,
		double* phiLeft, unsigned char *pLeft, double *phiRight, unsigned char *pRight,
		int*& idImage, int &count, double*& X, double*& Y, double*& Z, ScanType scan_type, int SampleNums);

	/**  ��־��ƥ��
	*  @para[in] nl xl yl �ֱ�Ϊ����ͼ��־���������־�����ͼ��x���ꡢy����
	*  @para[in] nr xr yr �ֱ�Ϊ����ͼ��־���������־�����ͼ��x���ꡢy����
	*  @para[out] n X Y Z �ؽ������ĵ�������Լ�x y z����, ��Ҫ�����ͷ�X Y Z�ռ�
	*  @para[out] idl �ؽ�����������е�id
	*/
	bool UndistortStereo(int nl, double *xxl, double *yyl,
		int nr, double *xxr, double *yyr,
		int &n, double *X, double *Y, double *Z,
		int *idl, int *idr, double distanceThre);

	bool UndistortStereo(int n, double *xxl, double *yyl, double *xxr, double *yyr,
		double *X, double *Y, double *Z);

	bool SaveActiveBox(const char *dirname);

	bool LoadActiveBox(const char *dirname);

	bool CreateActiveBox(int n, double *xl, double *yl, double *xr, double *yr);

protected:
	// ���� ��������
	double m_e1[2];				// �󼫵�
	double m_e2[2];				// �Ҽ���

	TNT::Matrix<double> m_fundament; // ��������

	void DataRelease(double *data);

	void DataRelease(unsigned char *data);

	void leftToWorldPoint(double x0, double y0, double z0, double *x, double *y, double *z);

	void leftToWorldNorm(double x0, double y0, double z0, double *x, double *y, double *z);
};

inline void StreroRebuild::DataRelease(double *data)
{
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}
}

inline void StreroRebuild::DataRelease(unsigned char *data)
{
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}
}

inline bool StreroRebuild::isInBox(double x, double y, double z)
{
	if (x<m_boxx[0] || x>m_boxx[1]
		|| y<m_boxy[0] || y>m_boxy[1]
		|| z<m_boxz[0] || z>m_boxz[1])
		return false;
	else
		return true;
}

// ���� leftToWorld()
//		�����������ϵ�ϵĵ�ת��Ϊ��������ĵ㡣Xw = (Xl-T) * R'
//			Xl=(x0,y0,z0)'; Xw = (x,y,z)'
// R T �Ǵ���������ϵ��ת��Ϊ���������ϵ,����Xl = Xw * R + T;
inline void StreroRebuild::leftToWorldPoint(double x0, double y0, double z0, double *x, double *y, double *z)
{
	*x = m_invRotLeft[0] * (x0 - m_transLeft[0]) + m_invRotLeft[1] * (y0 - m_transLeft[1]) + m_invRotLeft[2] * (z0 - m_transLeft[2]);
	*y = m_invRotLeft[3] * (x0 - m_transLeft[0]) + m_invRotLeft[4] * (y0 - m_transLeft[1]) + m_invRotLeft[5] * (z0 - m_transLeft[2]);
	*z = m_invRotLeft[6] * (x0 - m_transLeft[0]) + m_invRotLeft[7] * (y0 - m_transLeft[1]) + m_invRotLeft[8] * (z0 - m_transLeft[2]);
}

inline void StreroRebuild::leftToWorldNorm(double x0, double y0, double z0, double *x, double *y, double *z)
{
	*x = m_invRotLeft[0] * x0 + m_invRotLeft[1] * y0 + m_invRotLeft[2] * z0;
	*y = m_invRotLeft[3] * x0 + m_invRotLeft[4] * y0 + m_invRotLeft[5] * z0;
	*z = m_invRotLeft[6] * x0 + m_invRotLeft[7] * y0 + m_invRotLeft[8] * z0;
}

#endif