/********************************************************************
* Copyright(c) 2018,  Shining 3D Technology
* All rights reversed
*
* 文件名称: BinStereoBuild
* 文件标志: Ly
* 摘    要: 光栅相位重建
* 项目名称: 先临三维扫描系统
* 操作系统: MS WIndows XP SP3/MS Windows 7 Windows 10
* 编译平台: MS VS2013
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
	//////////////////////////////////////////////////////////////////////////////////////////////////////////  初始化阶段变量
	bool m_bParamLoad;
	int m_w;					                                                                              //图像宽度
	int m_h;					                                                                              //图像高度
	int m_size;					                                                                              							                                                    
	                                                                                                          //相机内参					                                                    
	double m_focalL[2];			                                                                              //左相机焦距
	double m_focalR[2];			                                                                              //右相机焦距
								                                                                              
	double m_principalL[2];		                                                                              //左主点位置
	double m_principalR[2];		                                                                              //右主点位置
								                                                                              
	double m_skewL;				                                                                              //左倾斜因子
	double m_skewR;				                                                                              //右倾斜因子
								                                                                              
	double m_distortCoeffL[5];                                                                                //左畸变因子
	double m_distortCoeffR[5];                                                                                //右畸变因子
									                                                                          
	TNT::Matrix<double> m_inMatL;	                                                                          //左内参矩阵
	TNT::Matrix<double> m_inMatR;	                                                                          //右内参矩阵
	                                                                                                          //相机外参	
	TNT::Matrix<double> m_rotate;	                                                                          //旋转矩阵
	double m_trans[3];																                          
																					                          
	double m_invRotLeft[9];		                                                                              //旋转矩阵
	double m_transLeft[3];

	double m_boxx[2];                                                                                         //有效区域
	double m_boxy[2];
	double m_boxz[2];
	
	bool m_bBox;
	bool is_useGpu;

	////////////////////////////////////////////////////////////////////////////////////////////////////////// DLP测试用
	double *CX, *CY;                                                                                         //DLP 输出对应点

	void Fit(Sn3DAlgorithm::Cloud Data, std::vector<LY3DStereoBuild::DistorData> &vecPointTemp, std::vector<LY3DStereoBuild::DistorData> &projectpoints, int randomTime,
		int SampleRata, float planeThresholdCopy, bool isDebug);
	// 根据相位从投影机平面提点 并拟合平面 适用于DLP光机矫正
	// unphase1:左相机 竖条纹相位
	// unphase2:左相机 横条纹相位
	// width:投影机宽度
	// heigh:投影机高度
	// phaseCycle:相位参数 假定投影机宽度为1920 最细条纹有100个周期 那么横竖条纹的phaseCycle都是19.2
	// projectpoints:输出三维点和平面对应点
	// maskLeft:左相机 mask
	// maskCross:右相机 mask
	// CX CY左右相机对应点
	// nn 对应点总数
	// range 重建出的深度数据
	void Recognize(double *unphase1, double *unphase2, int width, int heigh, int phaseCycle, std::vector<LY3DStereoBuild::DistorData> &projectpoints, unsigned char* maskLeft, unsigned char* maskCross, double *CX, double *CY, int &nn, LY3DStereoBuild::ScanRange& range);

	////////////////////////////////////////////////////////////////////////////////////////////////////////// DLP测试用
public:
	/**  初始化,程序启动时调用，不能重复，仅调用一次。                                                         正式代码
	*  @para[in] w,h 相机宽高参数
	*  @para[in] fcl,ccl,alphal,kcl,rcl,tcl,fcr,ccr,alphar,kcr,rcr,tcr 标定内外参
	*/
	bool InitParameter(int w, int h,
		double *fcl, double *ccl, double alphal, double *kcl, double *rcl, double *tcl,
		double *fcr, double *ccr, double alphar, double *kcr, double *rcr, double *tcr);
	
	void fundamentMat(TNT::Matrix<double> &fundM);                                                            //计算基础矩阵
	
	bool epipolarPoint(TNT::Matrix<double> &f);                                                               //计算极点
	
	void FundamentMat();                                                                                      // 基础矩阵及极点计算，外部调用
	
	bool computeDistortion(double *xlist, double *ylist, int n,                                               // 畸变计算
		double k1, double k2, double k3, double p1, double p2);
	
	bool normalize(double *xlist, double *ylist, int n,                                                       // 坐标转换到相机下，去畸变
		double *focal, double *principal, double *distortCoef, double skew);
	
	bool undistorted(double *xlist, double *ylist, int w, int h,                                              // 初始虚拟点，并进行坐标转换和去畸变
		double *focal, double *principal, double *distortCoef, double skew);
	
	bool computeEpipolar(TNT::Matrix<double> &f,                                                              // 计算极线数据及分块所需的点数据
		double *xUnLeft, double *yUnLeft, double *xUnRight, double *yUnRight,
		double *aRightL, double *bRightL, double *bRightE,
		double *aLeftR, double *bLeftR, double *bLeftE);
	
	/**  创建查找表,一般在标定完成后调用,后续若此文件不存在需要重新生成。
	*  @para[in] name 查找表文件路径
	*/
	bool CreateUndistortTable(const char *name);                                                              // 创建查找表
	
	bool LoadUndistortTable(const char *pathName);                                                            // 重建阶段函数

	bool ReleaseUndistortTable();

	bool SetBoxMask(bool bset);

	bool UnwarpPhase(int w, int h,
		double* phiLeft, unsigned  char *pLeft, double *phiRight, unsigned char *pRight);

	bool CheckBoxMask(int w, int h, int *idImage, int count, double *x, double *y, double *z);

	bool buildStereo(int n, double *X, double *Y, double *Z, double*xl, double*yl, double*xr, double *yr,ScanType type);

	bool isInBox(double x, double y, double z);

	// 是否开始输出
	void SetBinRebuildDebug(bool isDebug);
	// 重建输出有序点云
	//void PointsImageGPU(double*& X, double*& Y, double*& Z, int*& idImage, int &count, const std::vector<float3> &PointData);

	/**  执行重建操作 重建的主函数
	*  @para[in] w,h 相机参数，注意binning和正常情况该值应该一致
	*  @para[in] phiLeft,pLeft,phiRight,pRight 左右相机相位mask数据
	*  @para[out] idImage 输出深度数据对应的id
	*
	*/
	bool StartBuild(int w, int h,
		double* phiLeft, unsigned char *pLeft, double *phiRight, unsigned char *pRight,
		int*& idImage, int &count, double*& X, double*& Y, double*& Z, ScanType scan_type, int SampleNums);

	/**  标志点匹配
	*  @para[in] nl xl yl 分别为：左图标志点个数、标志点的做图像x坐标、y坐标
	*  @para[in] nr xr yr 分别为：右图标志点个数、标志点的做图像x坐标、y坐标
	*  @para[out] n X Y Z 重建出来的点个数，以及x y z坐标, 需要外面释放X Y Z空间
	*  @para[out] idl 重建点在输入点中的id
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
	// 极点 基础矩阵
	double m_e1[2];				// 左极点
	double m_e2[2];				// 右极点

	TNT::Matrix<double> m_fundament; // 基础矩阵

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

// 函数 leftToWorld()
//		把左相机坐标系上的点转化为世界坐标的点。Xw = (Xl-T) * R'
//			Xl=(x0,y0,z0)'; Xw = (x,y,z)'
// R T 是从世界坐标系，转换为左相机坐标系,即：Xl = Xw * R + T;
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