// File: sn3DStereoDefine.h

#ifndef _sn3DStereoDefine_H
#define _sn3DStereoDefine_H
#include <stdio.h>
#include <vector>
#include "sn3DImageData.h"
#ifndef PI
#define  PI 3.1415926535897932384626433832795
#endif
using namespace std;
namespace LY3DStereoBuild
{
	// dlp 测试使用
	struct DistorData
	{
		double x;
		double y;
		double z;
		double u;
		double v;
	};
	// add 518
	class RefPoints
	{
	public:
		RefPoints()
		{
			nPoint = 0;
			xPoints = yPoints = zPoints = NULL;
			IDList = NULL;
			InitTransform();
		}
		~RefPoints()			{ Free(); }
		void Alloc(int n)
		{
			Free();
			if (n <= 0)
				return;
			nPoint = n;
			xPoints = new double[nPoint];
			yPoints = new double[nPoint];
			zPoints = new double[nPoint];
			IDList = new int[nPoint];
			InitTransform();

		}
		void Free()
		{
			nPoint = 0;
			if (xPoints)
			{
				delete[] xPoints; xPoints = NULL;
				delete[] yPoints; yPoints = NULL;
				delete[] zPoints; zPoints = NULL;
				delete[] IDList;  IDList = NULL;
			}
		}
	protected:
		void InitTransform()
		{
			for (int i = 0; i < 3; i++)
			{
				trans[i] = 0.0;
				for (int j = 0; j < 3; j++)
					rotate[i * 3 + j] = (i == j);
			}

		}
	public:
		int nPoint;
		double *xPoints;
		double *yPoints;
		double *zPoints;

		int *IDList;

		double trans[3];
		double rotate[9];
	};

	// 椭圆识别：Canny算子参数 提取标定圆使用 add 518
	class CannyParam
	{
	public:
		double sigma;			// sigma		0.60-2.40
		double thresholdLow;	// 低阈值		0.20-0.50
		double thresholdHigh;	// 高阈值		0.60-0.90
	};

	// 椭圆识别：椭圆参数
	class CircleParam
	{
	public:
		double	pointQuality;	// 椭圆误差		0.05
		double	circularity;	// 圆度 长宽比	2.0
		int		minDiameter;	// 最短半径		4
		int		maxDiameter;	// 最长半径		200
	};

	//	载入图像参数
	//            
	class ScanImages
	{
	public:
		enum { MAX_SCAN_IMAGES_COUNT = 60 };
		sn3DCore::sn3DImageData<unsigned char> images[MAX_SCAN_IMAGES_COUNT];
		int imageCount;
	};

	//查找表的所有输出值
	class CLOOKTABLE_OUTPUT
	{
	public:
		vector<vector<double>> Phase2U_val;
		vector<vector<double>> Phase2V_val;

		vector<double> a_right_L;
		vector<double> b_right_L;
		vector<double> x_left_all_undis_U;
		vector<double> x_left_all_undis_V;

		vector<double> a_right_R;
		vector<double> b_right_R;
		vector<double> x_right_all_undis_U;
		vector<double> x_right_all_undis_V;
	};

	//生成查找表需要的配置文件参数
	class CLOOKTABLE_PARA_INI
	{
	public:
		double Pro_center;//投影机中心位置
		float Phase_T;//投影条纹周期

		int crow_cam;//拟合参数,行数
		int ccol_cam;//拟合参数,列数
		int pn;//拟合阶数

		double BorderLine[2];//标定的深度方向范围
		int VboardNum;//深度位置个数

		CLOOKTABLE_PARA_INI(void)
		{
			Pro_center = 399;//调试程序时注意，matlab和C的相差1
			Phase_T = 8;

			crow_cam = 1024;
			ccol_cam = 1280;
			pn = 3;

			BorderLine[0] = 250;
			BorderLine[1] = 550;

			VboardNum = 15;
		}

		~CLOOKTABLE_PARA_INI(void){}
	};

	//	解相位参数 条纹
	//             格雷码
	class PhaseParam
	{
	public:
		// 条纹相位求解
		enum { MAX_PHASE_STEP = 7 };
		int height;			// 图片高                          
		int width;			// 图片宽   
		int stepNum;		// 变频阶数
		int rasterType;		// 光栅类型,1为疏密光疏,0为全密光栅
		float pixelWidth[MAX_PHASE_STEP];// 每阶条纹宽度
		int moveNum[MAX_PHASE_STEP];	 // 每阶相移步数
		double frangeNum[MAX_PHASE_STEP];// 每阶条纹总数
		int index;                       // 相机索引 0 1标识左右相机
		int graySubTd;		//灰度差阈值
		// 格雷码相位求解
		std::vector<std::vector<double>> GrayTable;
	};

	// 图片参数
	class GrayImageParam 
	{
	public:
		GrayImageParam():width(0),height(0),nChannel(0),data(NULL){};
		int width;					// 图片宽度
		int height;					// 图片高度
		int nChannel;
		unsigned char *data;		// 图片数据，是单通道灰度图
	};

	class DoubleImageParam
	{
	public:
		DoubleImageParam():width(0),height(0),data(NULL){};
		int width;					// 图片宽度
		int height;					// 图片高度
		double *data;				// 图片数据，是单通道浮点数
	};

	// intrinsicMat内参矩阵
	//	||	focal0		skew		principal0	||
	//	||		0		focal1		principal1	||
	//	||		0		0				1		||

	// 三维相机的内外参数
	class CameraParam
	{
	public:
		int imageWidth;				// 图片宽度 
		int imageHeight;			// 图片高度

		// 相机内参
		double	focus[2];
		double	principal[2];
		double  alpha;

		double	distortion[5];		// 畸变系数 K1 K2 P1 P2 skew

		// R（rotate） T（trans） 是从世界坐标系转换到相机坐标系的变换,即：Xl = Xw * R + T;
		// 相机坐标系Xl=(x0,y0,z0)';世界坐标系 Xw = (x,y,z)'
		double trans[3];		    // 平移向量
		double rotate[9];			// 旋转向量
	};

	enum
	{
		MAX_CALIBR_IMAGE_COUNT = 25,
		CALIBR_POINT_COUNT = 238	//14*17
	};

	// 标定板上椭圆空间三维坐标
	class CalibrPanel3D
	{
	public:

		double pointx[CALIBR_POINT_COUNT];
		double pointy[CALIBR_POINT_COUNT];
		double pointz[CALIBR_POINT_COUNT];
	};

	// 标定板在图片二维坐标
	class CalibrPanel2D
	{
	public:

		double pointx[CALIBR_POINT_COUNT];
		double pointy[CALIBR_POINT_COUNT];
	};

	// 单相机标定
	class CalibrationParam
	{
	public:
		// 被检测的点
		class Inspect
		{
		public:
			int		code[2];
			double	dist;
		};
	public:

		// 图片上椭圆位置
		int           imageCount;
		bool		  active[MAX_CALIBR_IMAGE_COUNT];
		CalibrPanel2D imagePanel2D[MAX_CALIBR_IMAGE_COUNT];
		CalibrPanel3D lookupPanel3D;

		// 导出CCF文件名	
		char	ccfPath[260];

		// 相机参数
		CameraParam camera;
		Inspect		inspect[2];
	};

	class AxisParam
	{
	public:
		double AxisCircleX[3];//X轴原点
		double AxisCircleY[3];//Y轴原点
		double AxisVectorX[3];//X轴另一点，与X轴原点构成一个向量
		double AxisVectorY[3];//Y轴另一点，与Y轴原点构成一个向量
		double AxisCircleA[3];//A轴原点（印模翻转轴）
		double AxisVectorA[3];//A轴另一点，与A轴原点构成一个向量
        double AngleFromRT[3];//校正矩阵转换过来的欧拉角 
	};

	//三维重建的参数
	class C2Cam1ProjRebuild_Para
	{
	public:
		int crow;//图像纵坐标
		int ccol;//图像横坐标

		int num_find;//连续查找的点数需要放入ini文件中
		int sample;  //抽样比例

		int Near;//深度方向最近范围
		int Far;//深度方向最远范围

		int FiltWin;//滤波窗口

		double HightSearchThre;//最大搜索阈值
		double LowSearchThre;//最小搜索阈值
		double MedSearchThre;//中间搜索阈值，用来去除重建时的孔洞情况

		int findRange;       //去杂点流程
		float disThread;     
		int numThread;

		C2Cam1ProjRebuild_Para(void):crow(960),ccol(1280),num_find(5),sample(1),Near(100),Far(800),FiltWin(1),HightSearchThre(30*PI),
			LowSearchThre(PI/4),MedSearchThre(PI/4){};
	};

	//相机投影的标定路径及相机和投影机的分辨率参数(带投影机重建)
	class C2Cam1ProjPara
	{
	public:
		std::string CalibPathL;
		std::string CalibPathR;
		std::string CalibPathP;

		int LeftSize[2];
		int RightSize[2];
		int ProjSize[2];
	};

	//相机投影的标定路径及相机和投影机的分辨率参数(不带投影机重建)
	class CStreroPara
	{
	public:
		std::string CalibPathL;
		std::string CalibPathR;

		int LeftSize[2];
		int RightSize[2];
	};

	// ScanRange深度图是指：一幅图像，它的每个元素是一个三维坐标点的索引，如果该索引值
	//	为-1表示此处没有三维点。
	class ScanRange
	{
	public:
		int width;		// 深度图宽
		int height;		// 深度图高
		int* idList;	// 顶点索引列表，idList数组大小为width*height，其中元素为-1表示该点不存在。

		int numPt;		// 顶点个数
		double* xPt;		// 顶点x坐标数组
		double* yPt;		// 顶点y坐标数组
		double* zPt;		// 顶点z坐标数组
	};

	class CCamProjRebuild_Para
	{
	public:
		int crow;//图像纵坐标
		int ccol;//图像横坐标

		int sample;  //抽样比例

		CCamProjRebuild_Para(void):crow(1024),ccol(1280),sample(1){};
	};

	// 二维数据
	class DataArray2D
	{
	public:
		DataArray2D() :numPt(0), pointx(NULL), pointy(NULL){};
		~DataArray2D()				{ Free(); };
		void  Alloc(int n)
		{
			Free();		numPt = n;
			pointx = new double[n];
			pointy = new double[n];
		}
		void Free()
		{
			if (pointx)
			{
				delete[] pointx; pointx = NULL;
				delete[] pointy; pointy = NULL;
			}
			numPt = 0;
		}
	public:
		int numPt;
		double *pointx;
		double *pointy;
	};

	// 三维数据
	class DataArray3D
	{
	public:
		DataArray3D() :numPt(0), pointx(NULL), pointy(NULL), pointz(NULL){};
		~DataArray3D()				{ Free(); };
		void  Alloc(int n)
		{
			Free();		numPt = n;
			pointx = new double[n];
			pointy = new double[n];
			pointz = new double[n];

		}
		void Free()
		{
			if (pointx)
			{
				delete[] pointx; pointx = NULL;
				delete[] pointy; pointy = NULL;
				delete[] pointz; pointz = NULL;
			}
			numPt = 0;
		}
	public:
		int numPt;
		double *pointx;
		double *pointy;
		double *pointz;
	};

	// 三维重建对应点
	class CorrespondPoint
	{
	public:
		CorrespondPoint() :num(0), idsLeft(NULL), idsRight(NULL){};
		~CorrespondPoint()				{ Free(); };
		void  Alloc(int n)
		{
			Free();		num = n;
			idsLeft = new int[n];
			idsRight = new int[n];
		}
		void Free()
		{
			if (idsLeft)
			{
				delete[] idsLeft;  idsLeft = NULL;
				delete[] idsRight; idsRight = NULL;
			}
			num = 0;
		}
	public:
		int num;
		int* idsLeft;
		int* idsRight;
	};
}

#endif //_sn3DStereoDefine_H