/*************************************************
 * \brief 该文件定义该工程的所有算法接口
 * \note  TriMesh Cloud RanegImage 作为输入不允许存在status属性。
          TriMesh Cloud RangeImage 中的其他可选属性可以存在。但是如果存在，必须确保是正确的
 * \author 张健
 **************************************************/

#ifndef SN3D_ALGORITHM_ALGORITHMLY_INCLUDE_H
#define SN3D_ALGORITHM_ALGORITHMLY_INCLUDE_H

//接口导入导出
#ifdef _MSC_VER
    #ifdef ALGORITHMLY_API_EXPORTS
#define SN3D_ALGORITHMLY_API __declspec(dllexport)
    #else
#define SN3D_ALGORITHMLY_API __declspec(dllimport)
    #endif
#else
    #define SN3D_ALGORITHMLY_API
#endif

//类导出(方便代码的调试。在打包算法库时，用该前缀声明的接口或者类，不会导出)
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
	// 函数：	sn3DUnphase
	// 
	// 参数：	unphase		[out]展开相位图
	//			maskImage	[out]标志位图像，值为1表示该点相位有效，0为无效
	//			scanImages	[in]扫描图像
	//			param		[in]展开相位参数，相位阶数等
	//			phaseTh		[in]相位阈值，用来产生maskImage
	//			grayTh		[in]灰度阈值，用来产生maskImage
	//          Normalize_WB[in]是否使用黑白图归一化
	//		
	// 功能：多频外差法解条纹相位数据。(CPU解相位)
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DUnphase(DoubleImageParam& unphase, GrayImageParam& maskImage, ScanImages& scanImages,
		PhaseParam& param, double phaseTh, double grayTh, bool Normalize_WB);
	
	//===========================================================================
	// 函数：	sn3DGpuDeviceCheck
	// 
	// 参数：	
	//		
	// 功能：判断设备是否支持GPU。(通用)
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DGpuDeviceCheck();
	
	//===========================================================================
	// 函数：	sn3DUnphaseGPUIni
	// 
	// 参数：	param		[in]展开相位参数，相位阶数等 
	//		
	// 功能： 多频外差法Gpu解条纹相位数据内存初始化。
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DGPUMultFreqIni(PhaseParam& param);
	
	//===========================================================================
	// 函数：	sn3DGPURelease
	// 
	// 参数：	
	//		 
	// 功能：多频外差法解条纹相位数据显存释放。
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DGPURelease();
	
	//===========================================================================
	// 函数：	sn3DUnphaseGPU
	// 
	// 参数：	
	//			unphase		[out]解出的相位值
	//		    maskImage   [out]解出的Mask数据
	//          scanImages  [in]读入的图像数据
	//          param       [in]解相位参数
	//          phaseTh grayTh [in]相位阈值
	// 功能：多频外差法解条纹相位数据Gpu实现。
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DUnphaseGPU(DoubleImageParam& unphase, GrayImageParam& maskImage, ScanImages& scanImages, PhaseParam& param, double phaseTh, double grayTh);
	
	// 当图像噪声很大时直接进行解相位会得到很多错误的飘离相位值,这里加入了去孤岛算法，删除离群的相位点 5.18 DLP使用
	bool SN3D_ALGORITHMLY_CLASS sn3DUnphaseWithDelete(DoubleImageParam& unphase, GrayImageParam& maskImage, ScanImages& scanImages, PhaseParam& param, double phaseTh, double grayTh);

	//  DLP使用
	//===========================================================================
	// 函数：	sn3DCalibread
	// 
	// 参数：	DeviceParam	[in]左右相机及投影机标定数据路径及分辨率
	// 功能：载入相机标定数据。
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DCalibread(C2Cam1ProjPara DeviceParam);
	
	//===========================================================================
	// 函数：	sn3DLoadTable2Cam1Proj
	// 
	// 参数：	tablePath	[in]查找表数据路径
	//			boxPath	    [in]包围盒数据路径
	// 功能：载入相机标定数据。
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DLoadTable2Cam1Proj(const char *tablePath, const char* boxPath);
	
	//===========================================================================
	// 函数：	sn3DReleaseUndistortTable2Cam1Proj
	// 参数：	空
	// 功能：清除查找表占用的内存	
	//============================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DReleaseUndistortTable2Cam1Proj();
	
	//===========================================================================
	// 函数：	sn3DParamread
	// 
	// 参数：	IniPara	    [in]生成查找表需要的配置文件参数
	//			RebuildPara	    [in]三维重建的参数
	// 功能：载入查找表和重建数据。
	//		
	//=========================================================================== 
	//bool SN3D_ALGORITHMLY_CLASS sn3DParamread(CLOOKTABLE_PARA_INI &IniPara, C2Cam1ProjRebuild_Para &RebuildPara);
	bool SN3D_ALGORITHMLY_CLASS sn3DTableParamRead(CLOOKTABLE_PARA_INI &IniPara);
	bool SN3D_ALGORITHMLY_CLASS sn3DRebuildParamRead(C2Cam1ProjRebuild_Para &RebuildPara);

	bool SN3D_ALGORITHMLY_CLASS sn3DRebuildDebug(bool isDebug);

	//---------------------------------------------------------------------------
	// 函数：	sn3D2Cam1ProjStereoBuild
	// 
	// 参数：	ScanRange	[out]重建出来的深度图
	//			phaseLeft	[in]左边相位图
	//			maskLeft	[in]左边掩码图
	//			phaseRight	[in]右边相位图
	//			maskRight	[in]右边掩码图
	//			inBox		[in]是否使用包围盒
	//          island      [in]是否删除孤岛
	//		
	// 功能：相位重建，重建之前的步骤如下：
	//		1 在重建之前必须设置相机，函数sn3DSet2Cam1ProjRebuildPara
	//		2 导入查找表，函数sn3DLoadUndistortTable2Cam1Proj
	//		3 导入包围盒，函数sn3DLoadCalibrationBox
	//      重建后对原始数据使用聚类过滤杂点
	//
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3D2Cam1ProjBuildGPU(LY3DStereoBuild::ScanRange& range, DoubleImageParam& phaseL, GrayImageParam &maskL, DoubleImageParam& phaseR, GrayImageParam& maskR, bool inBox, bool island, bool isbinning);

	bool SN3D_ALGORITHMLY_CLASS sn3D2Cam1ProjStereoBuild(double *ParallValue, ScanRange& range,
		DoubleImageParam& phaseL, GrayImageParam &maskL, DoubleImageParam& phaseR, GrayImageParam& maskR, bool inBox, bool island, bool isbinning);
	
	//===========================================================================
	// 函数：	sn3DCreateUndistortTable2Cam1Proj
	// 参数：	tablePath	[in]输入查找表生成的目录位置
	// 功能：生成查找表	
	//============================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DCreateUndistortTable2Cam1Proj(const char* tablePath);
	
	//===========================================================================
	// 函数：	GrayPhase
	// 
	// 参数：	unphase [out]相位求解结果
	//		    maskImage [out]mask求解结果
	//          scanImages [in]扫描图像数据
	//          ImageWidth [in]图像宽度
	//          ImageHeight [in]图像高度
	//          useMask     [in]是否使用mask
	// 功能：格雷码解相位
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhase(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>&contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		ScanImages& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth,bool useMask);
	
	//===========================================================================
	// 函数：	GrayPhasMultExpGpu
	// 
	// 参数：	scanImages             [in] 传入的多曝光数据
	//		    ImageWidth ImageHeight [in] 图像参数
	//          grayTh Surth           [in] 相位参数
	//          unphase                [out] 输出相位
	//          contra                 [out] 
	//          maskImage              [out] 输出mask
	// 功能：格雷码多曝光解相位(GPU)
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhasMultExpGpu(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>& contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		std::vector<ScanImages>& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth, bool useMask);

	//===========================================================================
	// 函数：	GrayPhasMultExpGpu
	// 
	// 参数：	scanImages             [in] 传入的多曝光数据
	//		    ImageWidth ImageHeight [in] 图像参数
	//          grayTh Surth           [in] 相位参数
	//          unphase                [out] 输出相位
	//          contra                 [out] 
	//          maskImage              [out] 输出mask
	// 功能：格雷码多曝光解相位(CPU)
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhasMultExp(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>& contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		std::vector<ScanImages>& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth, bool useMask);

	//===========================================================================
	// 函数：	GrayPhaseGpuIni
	// 
	// 参数：	
	//		    ImageWidth ImageHeight [in] 图像参数
	//          Path                   [in] 查找表路径
	// 功能：格雷码GPU初始化
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhaseGpuIni(int ImageWidth, int ImageHeight, const std::string &Path);

	//===========================================================================
	// 函数：	GrayPhaseGpu
	// 
	// 参数：	unphase [out]相位求解结果
	//		    maskImage [out]mask求解结果
	//          scanImages [in]扫描图像数据
	//          ImageWidth [in]图像宽度
	//          ImageHeight [in]图像高度
	//          useMask     [in]是否使用mask
	// 功能：格雷码解相位(Gpu)
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS GrayPhaseGpu(sn3DCore::sn3DImageData<double>& unphase, sn3DCore::sn3DImageData<double>& contra, sn3DCore::sn3DImageData<unsigned char>& maskImage,
		ScanImages& scanImages, int ImageWidth, int ImageHeight, int grayTh, int Surth, bool useMask);

	void SN3D_ALGORITHMLY_CLASS GrayPhaseGpuRelease();
	
	//===========================================================================
	// 函数：	IslanDelete
	// 
	// 参数：	range          [in/out]输入的需要删除的数据 同时作为输出
	//          disThread      [in]    距离阈值
	//          numThread      [in]    判断孤岛的阈值，孤岛中点个数小于该值则认为是孤岛
	//                                 如果该值为0则表示只保留最大的那个岛。
	// 功能：深度图中孤岛删除
	//============================================================================
	void SN3D_ALGORITHMLY_CLASS IslandDelete(ScanRange& range, const int findRange, const float disThread, const int numThread);
	
	//===========================================================================
	// 函数：	sn3DSetCamera
	// 
	// 参数：	camLeft		[in]输入左相机参数
	//			camRight	[in]输入右相机参数
	//		
	// 功能：输入相机参数。使用以下函数之前必须调用此函数		
	//============================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DSetCamera(CStreroPara Param);
	
	// 参数加密情况下数据传入
	bool SN3D_ALGORITHMLY_CLASS snSetCamera(CameraParam &ParamLeft, CameraParam &CameraRight);
	
	//===========================================================================
	// 函数：	sn3DCreateUndistortTable
	// 
	// 参数：	tablePath	[in]保存查找表路径
	//		
	// 功能：创建查找表，并保存硬盘
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DCreateUndistortTable(const char* tablePath);
	
	//---------------------------------------------------------------------------
	// 函数：	sn3DLoadUndistortTable
	// 
	// 参数：	tablePath	[in]导入查找表路径
	//		
	// 功能：从硬盘导入查找表
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DLoadUndistortTable(const char* tablePath);

	//---------------------------------------------------------------------------
	// 函数：	sn3DReleaseUndistortTable
	// 
	// 参数：	空
	//		
	// 功能：清除查找表占用的内存
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DReleaseUndistortTable();

	//---------------------------------------------------------------------------
	// 函数：	sn3DPhaseStereoBuild
	// 
	// 参数：	ScanRange	[out]重建出来的深度图
	//			phaseLeft	[in]左边相位图
	//			maskLeft	[in]左边掩码图
	//			phaseRight	[in]右边相位图
	//			maskRight	[in]右边掩码图
	//			inBox		[in]是否使用包围盒
	//		
	// 功能：相位重建，重建之前的步骤如下：
	//		1 在重建之前必须设置相机，函数sn3DSetCamera
	//		2 导入查找表，函数sn3DLoadUndistortTable
	//
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DPhaseStereoBuild(ScanRange& range,
		sn3DCore::sn3DImageData<double>& phaseLeft, sn3DCore::sn3DImageData<unsigned char>& maskLeft,
		sn3DCore::sn3DImageData<double>& phaseRight, sn3DCore::sn3DImageData<unsigned char>& maskRight, bool inBox, ScanType type);
	
	// 重载+1
	bool SN3D_ALGORITHMLY_CLASS sn3DPhaseStereoBuild(ScanRange& range,
		DoubleImageParam& phaseLeft, GrayImageParam& maskLeft, DoubleImageParam& phaseRight, GrayImageParam& maskRight, bool inBox, bool isbinning, int SampleNums);

	// Dlp使用 输出对应点CXCY和重建点有效数量nn 
	bool SN3D_ALGORITHMLY_CLASS sn3DPhaseStereoBuild(ScanRange& range,
		DoubleImageParam& phaseLeft, GrayImageParam& maskLeft, DoubleImageParam& phaseRight, GrayImageParam& maskRight, double*& CX, double*& CY, int &nn, bool inBox);

	//===========================================================================
	// 函数：	sn3DIdentifyPoints14X17
	// 
	// 参数：	xPts		[out]识别点的x坐标，有14X17个元素。需要事先分配内存。
	//			yPts		[out]识别点的y坐标，有14X17个元素。需要事先分配内存。
	//			image		[in]待识别灰度图像
	//			canny		[in]canny算子参数
	//			circle		[in]椭圆参数
	//		
	// 功能：标定板上的灰度图像椭圆识别
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DIdentifyPoints14X17(double* xPts, double* yPts,
		GrayImageParam& image, CannyParam& canny, CircleParam& circle);

	bool SN3D_ALGORITHMLY_CLASS sn3DReadCCFTxt(CameraParam& camera, const char *pathName);
	//===========================================================================
	// 函数：	sn3DRefPointsAlign
	// 
	// 参数：	freePts			[in]标志点个数nPoint，以及标志点坐标（xPoints，yPoints，yPoints）	
	//							[out]IDList标志点的序号，模型的矩阵
	//			lengthEpsilon	[in]如果线段长度超出这个阈值认为是新点，小于这个阈值认为是同一个点
	//			removeEpsilon	[in]如果小于这个阈值认为是精确点，参与标志点拼接。如果大于这个值，认为是误差较大的点，不参与拼接id设为-1
	//			minCount		[in]最少公共点个数,不能少于3。可取值3，4，5，6...
	// 功能：标志点匹配。
	//		使用方法先循环加入sn3DInsertRefPoints参考标志点，再调用sn3DRefPointsAlign
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DRefPointsAlign(RefPoints& freePts);
	bool SN3D_ALGORITHMLY_CLASS sn3DRefPointsAlign(RefPoints& freePts, double lengthEpsilon, double removeEpsilon, int minCount);
    //===========================================================================
	// 函数：	sn3DHornAlign
	// 
	// 参数：	n		    [in]对应点数量
	//			xa ya za	[in]对应点a 顺序键入
	//			xb yb zb	[in]对应点b 顺序键入
	//			rotate		[out]输出的旋转矩阵
	//			trans		[out]输出的平移矩阵
	//		
	// 功能：求解对应点的相对旋转平移矩阵。
	//		
	//=========================================================================== 
	bool SN3D_ALGORITHMLY_CLASS sn3DHornAlign(int n, double *xa, double *ya, double *za, double *xb, double *yb, double*zb,
		double *rotate, double *trans);
	
	//============================================================================
	// 函数：	sn3Dbuildstereo (for Darren SDK)
	// 
	// 参数：	X,Y,Z [Out]重建出的三维点，内部分配内存
	//	xl,yr	[in]左图像的点
	//	xr,yr [in]右图像的点
	//	n	[in]需要重建点的个数
	//	
	// 功能：输入左相机右相机上对应点，直接重建三维点
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3Dbuildstereo(int pointnum, double* X, double* Y, double* Z, double* xl, double* yl, double*xr, double* yr);
	
	//============================================================================
	// 函数：	sn3DUndistortStereoBuild
	// 
	// 参数：	ptsWorld	[Out]重建出的三维点，内部分配内存
	//			cor			[Out]输出对应点，	 外部分配内存，大小为左图像标志点的个数
	//			ptsLeft		[in]左图像标志点
	//			ptsRight	[in]右图像标志点
	//			epipolarTh	[in]极线阈值
	//		
	// 功能：输入左相机右相机上的点，通过极线查找对应点
	//		用于离散点三维重建
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DUndistortStereoBuild(DataArray3D& ptsWorld, CorrespondPoint& cor,
		DataArray2D& ptsLeft, DataArray2D& ptsRight, double epipolarTh);
	
	//===========================================================================
	// 函数：	sn3DRecognPoints
	// 
	// 参数：	nPt			[out]识别出点个数
	//			xPts		[out]识别点的x坐标，函数内分配内存
	//			yPts		[out]识别点的y坐标，函数内分配内存
	//			image		[in]待识别灰度图像
	//			canny		[in]canny算子参数
	//			circle		[in]椭圆参数
	//		
	// 功能：灰度图像椭圆识别
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DRecognPoints(int &nPt, double*& xPts, double*& yPts,
		GrayImageParam& image, CannyParam& canny, CircleParam& circle);
	
	//===========================================================================
	// 函数：	sn3DInsertRefPoints
	// 
	// 参数：	rangeId		[in]模型的id
	//			refPts		[in]模型上的标志点
	//		
	// 功能：加入参考点	
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DInsertRefPoints(int rangeId, RefPoints& refPts);
	
	//------------------------------------------------                                                     ---------------------------
	// 函数：	sn3DCorrespondStereoBuild
	// 
	// 参数：	ptsWorld	[out]输出重建后空间三维点坐标
	//			ptsLeft		[in]左图像对应点的坐标
	//			ptsRight	[in]右图像对应点的坐标
	//		
	// 功能：输入对应点求解空间三维坐标
	//		用于离散点三维重建
	//===========================================================================
	bool SN3D_ALGORITHMLY_CLASS sn3DCorrespondStereoBuild(DataArray3D& ptsWorld,
		DataArray2D& ptsLeft, DataArray2D& ptsRight);
	
	//---------------------------------------------------------------------------
	// 函数：	sn3DCreateCalibrationBox
	// 
	// 参数：	boxPath	[in]导入查找表路径
	//			ptsLeft		[in]左边所有图像标志点的坐标
	//			ptsRight	[in]右边所有图像标志点的坐标，左右标志点要相对应
	//
	// 功能：左右标志点要相对应，生成包围合，并保存硬盘。
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DCreateCalibrationBox(const char* boxPath, DataArray2D& leftPts,
		DataArray2D& rightPts);
	
	//---------------------------------------------------------------------------
	// 函数：	sn3DLoadCalibrationBox
	// 
	// 参数：	boxPath	[in]导入查找表路径
	//		
	// 功能：从硬盘导入包围盒
	//---------------------------------------------------------------------------
	bool SN3D_ALGORITHMLY_CLASS sn3DLoadCalibrationBox(const char* boxPath);

	bool SN3D_ALGORITHMLY_CLASS sn3DRecognize(double *unphase1, double *unphase2, int width, int heigh, int phaseCycle, std::vector<LY3DStereoBuild::DistorData> &projectpoints,
		unsigned char* maskLeft, unsigned char* maskCross, double *CX, double *CY, int &nn, LY3DStereoBuild::ScanRange& range);

}//namespace

#endif //SN3D_ALGORITHM_ALGORITHMLY_INCLUDE_H