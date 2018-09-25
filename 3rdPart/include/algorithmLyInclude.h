/*************************************************
 * \brief ���ļ�����ù��̵������㷨�ӿ�
 * \note  TriMesh Cloud RanegImage ��Ϊ���벻�������status���ԡ�
          TriMesh Cloud RangeImage �е�������ѡ���Կ��Դ��ڡ�����������ڣ�����ȷ������ȷ��
 * \author �Ž�
 **************************************************/

#ifndef SN3D_ALGORITHM_ALGORITHMLY_INCLUDE_H
#define SN3D_ALGORITHM_ALGORITHMLY_INCLUDE_H

//�ӿڵ��뵼��
#ifdef _MSC_VER
    #ifdef ALGORITHMLY_API_EXPORTS
#define SN3D_ALGORITHMLY_API __declspec(dllexport)
    #else
#define SN3D_ALGORITHMLY_API __declspec(dllimport)
    #endif
#else
    #define SN3D_ALGORITHMLY_API
#endif

//�ർ��(�������ĵ��ԡ��ڴ���㷨��ʱ���ø�ǰ׺�����Ľӿڻ����࣬���ᵼ��)
#ifdef _MSC_VER
    #ifdef ALGORITHMLY_CLASS_EXPORTS
#define SN3D_ALGORITHMLY_CLASS __declspec(dllexport)
    #else
#define SN3D_ALGORITHMLY_CLASS
    #endif
#else
    #define  SN3D_ALGORITHMLY_CLASS
#endif

#include "./common/baseType/Cloud.h"
#include "./common/baseType/TriMesh.h"
#include "./common/baseType/RangeData.h"
#include "./common/baseType/Image.h"
#include "./common/baseType/BaseTriMesh.h"
#include "./common/baseType/BoundingBox.h"
#include "./common/baseType/PointImage.h"
#include <string>
#include<iostream>
#include<fstream>

#include "sn3DStereoDefine.h"

using namespace LY3DStereoBuild;

namespace LY3DAlgorithm
{
	//===========================================================================
	// ������	sn3DUnphase
	// 
	// ������	unphase		[out]չ����λͼ
	//			maskImage	[out]��־λͼ��ֵΪ1��ʾ�õ���λ��Ч��0Ϊ��Ч
	//			scanImages	[in]ɨ��ͼ��
	//			param		[in]չ����λ��������λ������
	//			phaseTh		[in]��λ��ֵ����������maskImage
	//			grayTh		[in]�Ҷ���ֵ����������maskImage
	//          Normalize_WB[in]�Ƿ�ʹ�úڰ�ͼ��һ��
	//		
	// ���ܣ���Ƶ����������λ���ݡ�(CPU����λ)
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DUnphase(DoubleImageParam& unphase, GrayImageParam& maskImage, ScanImages& scanImages,
		PhaseParam& param, double phaseTh, double grayTh, bool Normalize_WB);
	
	//===========================================================================
	// ������	sn3DGpuDeviceCheck
	// 
	// ������	
	//		
	// ���ܣ��ж��豸�Ƿ�֧��GPU��(ͨ��)
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DGpuDeviceCheck();
	
	//===========================================================================
	// ������	sn3DUnphaseGPUIni
	// 
	// ������	param		[in]չ����λ��������λ������ 
	//		
	// ���ܣ� ��Ƶ��Gpu��������λ�����ڴ��ʼ����
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DGPUMultFreqIni(PhaseParam& param);
	
	//===========================================================================
	// ������	sn3DGPURelease
	// 
	// ������	
	//		 
	// ���ܣ���Ƶ����������λ�����Դ��ͷš�
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DGPURelease();
	
	//===========================================================================
	// ������	sn3DUnphaseGPU
	// 
	// ������	
	//			unphase		[out]�������λֵ
	//		    maskImage   [out]�����Mask����
	//          scanImages  [in]�����ͼ������
	//          param       [in]����λ����
	//          phaseTh grayTh [in]��λ��ֵ
	// ���ܣ���Ƶ����������λ����Gpuʵ�֡�
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DUnphaseGPU(DoubleImageParam& unphase, GrayImageParam& maskImage, ScanImages& scanImages, PhaseParam& param, double phaseTh, double grayTh);
	
	// ��ͼ�������ܴ�ʱֱ�ӽ��н���λ��õ��ܶ�����Ʈ����λֵ,���������ȥ�µ��㷨��ɾ����Ⱥ����λ�� 5.18 DLPʹ��
	bool SN3D_ALGORITHMLY_CLASS sn3DUnphaseWithDelete(DoubleImageParam& unphase, GrayImageParam& maskImage, ScanImages& scanImages, PhaseParam& param, double phaseTh, double grayTh);

	//  DLPʹ��
	//===========================================================================
	// ������	sn3DCalibread
	// 
	// ������	DeviceParam	[in]���������ͶӰ���궨����·�����ֱ���
	// ���ܣ���������궨���ݡ�
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DCalibread(C2Cam1ProjPara DeviceParam);
	
	//===========================================================================
	// ������	sn3DLoadTable2Cam1Proj
	// 
	// ������	tablePath	[in]���ұ�����·��
	//			boxPath	    [in]��Χ������·��
	// ���ܣ���������궨���ݡ�
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DLoadTable2Cam1Proj(const char *tablePath, const char* boxPath);
	
	//===========================================================================
	// ������	sn3DReleaseUndistortTable2Cam1Proj
	// ������	��
	// ���ܣ�������ұ�ռ�õ��ڴ�	
	//============================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DReleaseUndistortTable2Cam1Proj();
	
	//===========================================================================
	// ������	sn3DParamread
	// 
	// ������	IniPara	    [in]���ɲ��ұ���Ҫ�������ļ�����
	//			RebuildPara	    [in]��ά�ؽ��Ĳ���
	// ���ܣ�������ұ���ؽ����ݡ�
	//		
	//=========================================================================== 
	//bool SN3D_ALGORITHMLY_CLASS sn3DParamread(CLOOKTABLE_PARA_INI &IniPara, C2Cam1ProjRebuild_Para &RebuildPara);
	bool SN3D_ALGORITHMLY_CLASS sn3DTableParamRead(CLOOKTABLE_PARA_INI &IniPara);
	bool SN3D_ALGORITHMLY_CLASS sn3DRebuildParamRead(C2Cam1ProjRebuild_Para &RebuildPara);

	bool SN3D_ALGORITHMLY_CLASS sn3DRebuildDebug(bool isDebug);

	//---------------------------------------------------------------------------
	// ������	sn3D2Cam1ProjStereoBuild
	// 
	// ������	ScanRange	[out]�ؽ����������ͼ
	//			phaseLeft	[in]�����λͼ
	//			maskLeft	[in]�������ͼ
	//			phaseRight	[in]�ұ���λͼ
	//			maskRight	[in]�ұ�����ͼ
	//			inBox		[in]�Ƿ�ʹ�ð�Χ��
	//          island      [in]�Ƿ�ɾ���µ�
	//		
	// ���ܣ���λ�ؽ����ؽ�֮ǰ�Ĳ������£�
	//		1 ���ؽ�֮ǰ�����������������sn3DSet2Cam1ProjRebuildPara
	//		2 ������ұ�����sn3DLoadUndistortTable2Cam1Proj
	//		3 �����Χ�У�����sn3DLoadCalibrationBox
	//      �ؽ����ԭʼ����ʹ�þ�������ӵ�
	//
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3D2Cam1ProjBuildGPU(LY3DStereoBuild::ScanRange& range, DoubleImageParam& phaseL, GrayImageParam &maskL, DoubleImageParam& phaseR, GrayImageParam& maskR, bool inBox, bool island, bool isbinning);

	bool SN3D_ALGORITHMLY_CLASS sn3D2Cam1ProjStereoBuild(double *ParallValue, ScanRange& range,
		DoubleImageParam& phaseL, GrayImageParam &maskL, DoubleImageParam& phaseR, GrayImageParam& maskR, bool inBox, bool island, bool isbinning);
	
	//===========================================================================
	// ������	sn3DCreateUndistortTable2Cam1Proj
	// ������	tablePath	[in]������ұ����ɵ�Ŀ¼λ��
	// ���ܣ����ɲ��ұ�	
	//============================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DCreateUndistortTable2Cam1Proj(const char* tablePath);
	
	//===========================================================================
	// ������	GrayPhase
	// 
	// ������	unphase [out]��λ�����
	//		    maskImage [out]mask�����
	//          scanImages [in]ɨ��ͼ������
	//          ImageWidth [in]ͼ����
	//          ImageHeight [in]ͼ��߶�
	//          useMask     [in]�Ƿ�ʹ��mask
	// ���ܣ����������λ
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhase(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>&contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		ScanImages& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth,bool useMask);
	
	//===========================================================================
	// ������	GrayPhasMultExpGpu
	// 
	// ������	scanImages             [in] ����Ķ��ع�����
	//		    ImageWidth ImageHeight [in] ͼ�����
	//          grayTh Surth           [in] ��λ����
	//          unphase                [out] �����λ
	//          contra                 [out] 
	//          maskImage              [out] ���mask
	// ���ܣ���������ع����λ(GPU)
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhasMultExpGpu(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>& contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		std::vector<ScanImages>& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth, bool useMask);

	//===========================================================================
	// ������	GrayPhasMultExpGpu
	// 
	// ������	scanImages             [in] ����Ķ��ع�����
	//		    ImageWidth ImageHeight [in] ͼ�����
	//          grayTh Surth           [in] ��λ����
	//          unphase                [out] �����λ
	//          contra                 [out] 
	//          maskImage              [out] ���mask
	// ���ܣ���������ع����λ(CPU)
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhasMultExp(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>& contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		std::vector<ScanImages>& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth, bool useMask);

	//===========================================================================
	// ������	GrayPhaseGpuIni
	// 
	// ������	
	//		    ImageWidth ImageHeight [in] ͼ�����
	//          Path                   [in] ���ұ�·��
	// ���ܣ�������GPU��ʼ��
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhaseGpuIni(int ImageWidth, int ImageHeight, const std::string &Path);

	//===========================================================================
	// ������	GrayPhaseGpu
	// 
	// ������	unphase [out]��λ�����
	//		    maskImage [out]mask�����
	//          scanImages [in]ɨ��ͼ������
	//          ImageWidth [in]ͼ����
	//          ImageHeight [in]ͼ��߶�
	//          useMask     [in]�Ƿ�ʹ��mask
	// ���ܣ����������λ(Gpu)
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhaseGpu(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>& contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		ScanImages& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth, bool useMask);

	void SN3D_ALGORITHMLY_CLASS GrayPhaseGpuRelease();
	
	//===========================================================================
	// ������	IslanDelete
	// 
	// ������	range          [in/out]�������Ҫɾ�������� ͬʱ��Ϊ���
	//          disThread      [in]    ������ֵ
	//          numThread      [in]    �жϹµ�����ֵ���µ��е����С�ڸ�ֵ����Ϊ�ǹµ�
	//                                 �����ֵΪ0���ʾֻ���������Ǹ�����
	// ���ܣ����ͼ�йµ�ɾ��
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS IslandDelete(ScanRange& range, const int findRange, const float disThread, const int numThread);
	
	//===========================================================================
	// ������	sn3DSetCamera
	// 
	// ������	camLeft		[in]�������������
	//			camRight	[in]�������������
	//		
	// ���ܣ��������������ʹ�����º���֮ǰ������ô˺���		
	//============================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DSetCamera(CStreroPara Param);
	
	// ����������������ݴ���
	bool SN3D_ALGORITHMLY_CLASS snSetCamera(CameraParam &ParamLeft, CameraParam &CameraRight);
	
	//===========================================================================
	// ������	sn3DCreateUndistortTable
	// 
	// ������	tablePath	[in]������ұ�·��
	//		
	// ���ܣ��������ұ�������Ӳ��
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DCreateUndistortTable(const char* tablePath);
	
	//---------------------------------------------------------------------------
	// ������	sn3DLoadUndistortTable
	// 
	// ������	tablePath	[in]������ұ�·��
	//		
	// ���ܣ���Ӳ�̵�����ұ�
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DLoadUndistortTable(const char* tablePath);

	//---------------------------------------------------------------------------
	// ������	sn3DReleaseUndistortTable
	// 
	// ������	��
	//		
	// ���ܣ�������ұ�ռ�õ��ڴ�
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DReleaseUndistortTable();

	//---------------------------------------------------------------------------
	// ������	sn3DPhaseStereoBuild
	// 
	// ������	ScanRange	[out]�ؽ����������ͼ
	//			phaseLeft	[in]�����λͼ
	//			maskLeft	[in]�������ͼ
	//			phaseRight	[in]�ұ���λͼ
	//			maskRight	[in]�ұ�����ͼ
	//			inBox		[in]�Ƿ�ʹ�ð�Χ��
	//		
	// ���ܣ���λ�ؽ����ؽ�֮ǰ�Ĳ������£�
	//		1 ���ؽ�֮ǰ�����������������sn3DSetCamera
	//		2 ������ұ�����sn3DLoadUndistortTable
	//
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DPhaseStereoBuild(ScanRange& range,
		sn3DCore::sn3DImageData<double>& phaseLeft, sn3DCore::sn3DImageData<unsigned char>& maskLeft,
		sn3DCore::sn3DImageData<double>& phaseRight, sn3DCore::sn3DImageData<unsigned char>& maskRight, bool inBox, ScanType type);
	
	// ����+1
	bool SN3D_ALGORITHMLY_CLASS sn3DPhaseStereoBuild(ScanRange& range,
		DoubleImageParam& phaseLeft, GrayImageParam& maskLeft, DoubleImageParam& phaseRight, GrayImageParam& maskRight, bool inBox, bool isbinning, int SampleNums);

	// Dlpʹ�� �����Ӧ��CXCY���ؽ�����Ч����nn 
	bool SN3D_ALGORITHMLY_CLASS sn3DPhaseStereoBuild(ScanRange& range,
		DoubleImageParam& phaseLeft, GrayImageParam& maskLeft, DoubleImageParam& phaseRight, GrayImageParam& maskRight, double*& CX, double*& CY, int &nn, bool inBox);

	//===========================================================================
	// ������	sn3DIdentifyPoints14X17
	// 
	// ������	xPts		[out]ʶ����x���꣬��14X17��Ԫ�ء���Ҫ���ȷ����ڴ档
	//			yPts		[out]ʶ����y���꣬��14X17��Ԫ�ء���Ҫ���ȷ����ڴ档
	//			image		[in]��ʶ��Ҷ�ͼ��
	//			canny		[in]canny���Ӳ���
	//			circle		[in]��Բ����
	//		
	// ���ܣ��궨���ϵĻҶ�ͼ����Բʶ��
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DIdentifyPoints14X17(double* xPts, double* yPts,
		GrayImageParam& image, CannyParam& canny, CircleParam& circle);

	bool SN3D_ALGORITHMLY_CLASS sn3DReadCCFTxt(CameraParam& camera, const char *pathName);
	//===========================================================================
	// ������	sn3DRefPointsAlign
	// 
	// ������	freePts			[in]��־�����nPoint���Լ���־�����꣨xPoints��yPoints��yPoints��	
	//							[out]IDList��־�����ţ�ģ�͵ľ���
	//			lengthEpsilon	[in]����߶γ��ȳ��������ֵ��Ϊ���µ㣬С�������ֵ��Ϊ��ͬһ����
	//			removeEpsilon	[in]���С�������ֵ��Ϊ�Ǿ�ȷ�㣬�����־��ƴ�ӡ�����������ֵ����Ϊ�����ϴ�ĵ㣬������ƴ��id��Ϊ-1
	//			minCount		[in]���ٹ��������,��������3����ȡֵ3��4��5��6...
	// ���ܣ���־��ƥ�䡣
	//		ʹ�÷�����ѭ������sn3DInsertRefPoints�ο���־�㣬�ٵ���sn3DRefPointsAlign
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DRefPointsAlign(RefPoints& freePts);
	bool SN3D_ALGORITHMLY_CLASS sn3DRefPointsAlign(RefPoints& freePts, double lengthEpsilon, double removeEpsilon, int minCount);
    //===========================================================================
	// ������	sn3DHornAlign
	// 
	// ������	n		    [in]��Ӧ������
	//			xa ya za	[in]��Ӧ��a ˳�����
	//			xb yb zb	[in]��Ӧ��b ˳�����
	//			rotate		[out]�������ת����
	//			trans		[out]�����ƽ�ƾ���
	//		
	// ���ܣ�����Ӧ��������תƽ�ƾ���
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DHornAlign(int n, double *xa, double *ya, double *za, double *xb, double *yb, double*zb,
		double *rotate, double *trans);
	
	//============================================================================
	// ������	sn3Dbuildstereo (for Darren SDK)
	// 
	// ������	X,Y,Z [Out]�ؽ�������ά�㣬�ڲ������ڴ�
	//	xl,yr	[in]��ͼ��ĵ�
	//	xr,yr [in]��ͼ��ĵ�
	//	n	[in]��Ҫ�ؽ���ĸ���
	//	
	// ���ܣ����������������϶�Ӧ�㣬ֱ���ؽ���ά��
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3Dbuildstereo(int pointnum, double* X, double* Y, double* Z, double* xl, double* yl, double*xr, double* yr);
	
	//============================================================================
	// ������	sn3DUndistortStereoBuild
	// 
	// ������	ptsWorld	[Out]�ؽ�������ά�㣬�ڲ������ڴ�
	//			cor			[Out]�����Ӧ�㣬	 �ⲿ�����ڴ棬��СΪ��ͼ���־��ĸ���
	//			ptsLeft		[in]��ͼ���־��
	//			ptsRight	[in]��ͼ���־��
	//			epipolarTh	[in]������ֵ
	//		
	// ���ܣ����������������ϵĵ㣬ͨ�����߲��Ҷ�Ӧ��
	//		������ɢ����ά�ؽ�
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DUndistortStereoBuild(DataArray3D& ptsWorld, CorrespondPoint& cor,
		DataArray2D& ptsLeft, DataArray2D& ptsRight, double epipolarTh);
	
	//===========================================================================
	// ������	sn3DRecognPoints
	// 
	// ������	nPt			[out]ʶ��������
	//			xPts		[out]ʶ����x���꣬�����ڷ����ڴ�
	//			yPts		[out]ʶ����y���꣬�����ڷ����ڴ�
	//			image		[in]��ʶ��Ҷ�ͼ��
	//			canny		[in]canny���Ӳ���
	//			circle		[in]��Բ����
	//		
	// ���ܣ��Ҷ�ͼ����Բʶ��
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DRecognPoints(int &nPt, double*& xPts, double*& yPts,
		GrayImageParam& image, CannyParam& canny, CircleParam& circle);
	
	//===========================================================================
	// ������	sn3DInsertRefPoints
	// 
	// ������	rangeId		[in]ģ�͵�id
	//			refPts		[in]ģ���ϵı�־��
	//		
	// ���ܣ�����ο���	
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DInsertRefPoints(int rangeId, RefPoints& refPts);
	
	//------------------------------------------------                                                     ---------------------------
	// ������	sn3DCorrespondStereoBuild
	// 
	// ������	ptsWorld	[out]����ؽ���ռ���ά������
	//			ptsLeft		[in]��ͼ���Ӧ�������
	//			ptsRight	[in]��ͼ���Ӧ�������
	//		
	// ���ܣ������Ӧ�����ռ���ά����
	//		������ɢ����ά�ؽ�
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DCorrespondStereoBuild(DataArray3D& ptsWorld,
		DataArray2D& ptsLeft, DataArray2D& ptsRight);
	
	//---------------------------------------------------------------------------
	// ������	sn3DCreateCalibrationBox
	// 
	// ������	boxPath	[in]������ұ�·��
	//			ptsLeft		[in]�������ͼ���־�������
	//			ptsRight	[in]�ұ�����ͼ���־������꣬���ұ�־��Ҫ���Ӧ
	//
	// ���ܣ����ұ�־��Ҫ���Ӧ�����ɰ�Χ�ϣ�������Ӳ�̡�
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DCreateCalibrationBox(const char* boxPath, DataArray2D& leftPts,
		DataArray2D& rightPts);
	
	//---------------------------------------------------------------------------
	// ������	sn3DLoadCalibrationBox
	// 
	// ������	boxPath	[in]������ұ�·��
	//		
	// ���ܣ���Ӳ�̵����Χ��
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DLoadCalibrationBox(const char* boxPath);

	bool SN3D_ALGORITHMLY_CLASS sn3DRecognize(double *unphase1, double *unphase2, int width, int heigh, int phaseCycle, std::vector<LY3DStereoBuild::DistorData> &projectpoints,
		unsigned char* maskLeft, unsigned char* maskCross, double *CX, double *CY, int &nn, LY3DStereoBuild::ScanRange& range);

}//namespace

#endif //SN3D_ALGORITHM_ALGORITHMLY_INCLUDE_H