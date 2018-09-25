
#ifndef _CPhaseInterpolation_H
#define _CPhaseInterpolation_H

#include "opencv2/opencv.hpp"
#include "cv.h"
#include"./DataIO/DataIO.h"
#include "cuda/commonGpuTool.h"

class CPhaseInterpolation
{
public:
	CPhaseInterpolation(void);
	virtual ~CPhaseInterpolation(void);

	ScanType scan_type_bino;                                                                  // 双目带分块扫描类型 
	                                                                                          // 输出的对应点数据
	double *gl_xa,*gl_ya,*gl_xb,*gl_yb;                                                       // 不带id的对应点数据

	double *gl_bTable, *gl_phTable;                                                           // 分块中间结果
	
	int *gl_iTable, *gl_jTable;

	int *gl_IdlistImage,*gl_Mask;

	double* gl_pta;                                                                           // DLP对应点数据输出 add by ly
	
	double* gl_ptb;

	int gl_Sample_Nums;                                                                       // 全局的采样数量
	
	int gl_PointsNums;

	int gl_build_inter;
    
	bool is_Debug;                                                                            // 是否进入debug模式(多数据输出)

public:
	bool LoadUndistortTable(const char *name);                                                // 查找表载入 非投影模式

	bool ReleaseUndistortTable();                                                             // 查找表释放
	
	bool UnwarpPhase(int w, int h, double* phLeft,unsigned char *pLeft,                       // 载入相位数据
		double *phRight,unsigned char *pRight );
    
	void UvnInit(int *uvN, int *uvBegin, int *uvEnd);                                         // 分块内容初始化
   
	void GetUVn(int *uvN);                                                                    // 确定每个分块的点数

	void GetUVBegin(int *uvN, int *uvBegin);                                                  // 搜索过程起始点

	void GetUVEnd(int *uvBegin, int *uvEnd, double *bTable,                                   // 搜索过程终止点
		double *phTable, int *iTable, int *jTable);
	
	void OuttestGpu(double *uvN, double *uvBegin);

	void OuttestCPU(double *uvN, double *uvBegin);                                            // 中间结果输出测试

	void PhaseRebuildFast_CPU();                                                              // 测试

	bool StartFind();                                                                         // 搜索对应点流程开始

	void SetFrqStrip(double frqStrip);                                                        // 设定模拟分块参数
	
	bool uvCenter();                                                                          // 确定根据b，phi分块的最大尺寸和中心点

	void PointsImageGPU(double*& X, double*& Y, double*& Z, int *&idListImage, int &count,
		const std::vector<float3> &PointData);

protected:
	int uv(int u, int v)                                                                      // 二维坐标变成一维
	{ 
		return u*m_nv+v;
	};

	bool uvInBound(int u,int v)
	{
		return (u>=0 && u<m_nu && v>=0 && v<m_nv);
	};

	int ij(int i, int j)
	{ 
		return i*m_w +j;
	};

	int round( double x);                                                                     // 四舍五入

	int getUL(int i, int j);                                                                  // 通过像素坐标，寻找小块 核心
	int getVL(int i, int j);
	int getUR(int i, int j);
	int getVR(int i, int j);

	void CheckCorresPointsForword(int id, int minij, double *phTable,                         // 在正方向继续搜索，是否存在更加匹配的点
		int *iTable, int *jTable, int *iRigh, int *jRigh);

	void CheckCorresPointsBackword(int id, int minij, double *phTable,                        // 在负方向继续搜索，是否存在更加匹配的点
		int *iTable, int *jTable, int *iRigh, int *jRigh);

	bool CheckPhaseLeft(const int &id, const int &k);                                         // 左相机极线上确定该点是否有效，避免左相机无效，右相机有效，生成杂点的情况

	void CorresFindRough(int *uvN, int *iLeft, int *jLeft, int *iRigh, int *jRigh,            // 粗略搜索，相机级的对应点数据
		int *uvBegin, int *uvEnd, double *bTable,
		double *phTable, int *iTable, int *jTable);

	bool CorresFindRefine(int *iLeft, int *jLeft, int *iRigh, int *jRigh,                     // 精细搜索，亚像素级别的搜索
		double *xl, double *yl, double *xr, double *yr);

	void GenerateIdListImage(int *PMask, double *xl, double *yl, double *xr, double *yr);     // 创建三维点对应的id

	bool build(	int *uvN, int *uvBegin, int *uvEnd, double *bTable,                           // 核心 对应点搜索函数
		double *phTable, int *iTable, int *jTable);

	void GetMaxMinb(double &MinValue, double &MaxValue);                                      // 确定根据b，phi分块的最大尺寸和中心点

	void GetMaxMinPhase(double &MinValue, double &MaxValue);

	int maskLegalCount( unsigned  char * mask, int count);                                    // 有效点数量查询
	
	bool biInterpolationRight(int idLeft,int i1,int j1 , int i2, int j2,                      // 双线性插值，查找左图像素idLeft，在右图矩形[左上角（i1，j1），右下角（i2，j2）]中的亚像素坐标
		double &rx, double &ry);
	
	bool biInterpolationLeft(int idRigh,int i1,int j1 , int i2, int j2,                       // 双线性插值，查找右图像素idRigh，在左图矩形[左上角（i1，j1），右下角（i2，j2）]中的亚像素坐标
		double &lx, double &ly);

	bool allocUndistortTable(int count);                                                      // 创建查找表空间
	
	bool freeUndistortTable();                                                                // 释放查找表数据

public:
	//---------------------------------------------------------------------------
	unsigned char* m_imgMaskR;                                                                // 重建有效点，该参数由解相位得到。有效为1，无效为0
	
	unsigned char* m_imgMaskL;
	
	double* m_imgPhiR;
	
	double* m_imgPhiL;

	int m_w;
	int m_h;
	int m_wl;
	int m_hl;
	int m_wr;
	int m_hr;
	int m_sizeLeft;
	int m_sizeRight;

	int m_nu;                                                                                  // 分小块的坐标，m_nu纵向块数，m_nv横向块数
	int m_nv;
	int m_cu;                                                                                  // 中心
	int m_cv;
                                                                                               // 图像信息
	double m_proWidth;		                                                                   // 投影图像宽度
	double m_frqStrip;		                                                                   // 条纹周期

	double m_reSlope;                                              
	double m_reTh;                                                                             // 条纹分块阈值的倒数
	double m_Th;

	//---------------------------------------------------------------------------
	// 查找表数据

	double *m_xUnLeft;
	double *m_yUnLeft;

	double *m_xUnRight;
	double *m_yUnRight;

	double *m_aRightL;                                                                         // 左图像对应的极线
	double *m_bRightL;

	double *m_aLeftR;                                                                          // 右图像对应的极线
	double *m_bLeftR;

	double *m_bRightE;	                                                                       // 通过极线计算b值
	double *m_bLeftE;

	int m_size;
	bool m_hasTable;
};

inline int CPhaseInterpolation::round( double x)//四舍五入
{ 
	if(x-int(x) > 0.5 )
		return (int)(x+0.5);
	return (int)x;
}

//左相机的u v
inline int CPhaseInterpolation::getUL(int i, int j)
{
	return round( m_bRightL[ij(i,j)] * m_reSlope ) + m_cu;
}
inline int CPhaseInterpolation::getVL(int i, int j)
{
	return round( m_imgPhiL[ij(i,j)] * m_reTh ) + m_cv;
}
//右相机的u v
inline int CPhaseInterpolation::getUR(int i, int j)
{
	return round( m_bRightE[ij(i,j)] * m_reSlope ) + m_cu;
}
inline int CPhaseInterpolation::getVR(int i, int j)
{
	return round( m_imgPhiR[ij(i,j)] * m_reTh ) + m_cv;
}

#endif // _CPhaseInterpolation_H
