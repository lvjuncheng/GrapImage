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
	// dlp ����ʹ��
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

	// ��Բʶ��Canny���Ӳ��� ��ȡ�궨Բʹ�� add 518
	class CannyParam
	{
	public:
		double sigma;			// sigma		0.60-2.40
		double thresholdLow;	// ����ֵ		0.20-0.50
		double thresholdHigh;	// ����ֵ		0.60-0.90
	};

	// ��Բʶ����Բ����
	class CircleParam
	{
	public:
		double	pointQuality;	// ��Բ���		0.05
		double	circularity;	// Բ�� �����	2.0
		int		minDiameter;	// ��̰뾶		4
		int		maxDiameter;	// ��뾶		200
	};

	//	����ͼ�����
	//            
	class ScanImages
	{
	public:
		enum { MAX_SCAN_IMAGES_COUNT = 60 };
		sn3DCore::sn3DImageData<unsigned char> images[MAX_SCAN_IMAGES_COUNT];
		int imageCount;
	};

	//���ұ���������ֵ
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

	//���ɲ��ұ���Ҫ�������ļ�����
	class CLOOKTABLE_PARA_INI
	{
	public:
		double Pro_center;//ͶӰ������λ��
		float Phase_T;//ͶӰ��������

		int crow_cam;//��ϲ���,����
		int ccol_cam;//��ϲ���,����
		int pn;//��Ͻ���

		double BorderLine[2];//�궨����ȷ���Χ
		int VboardNum;//���λ�ø���

		CLOOKTABLE_PARA_INI(void)
		{
			Pro_center = 399;//���Գ���ʱע�⣬matlab��C�����1
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

	//	����λ���� ����
	//             ������
	class PhaseParam
	{
	public:
		// ������λ���
		enum { MAX_PHASE_STEP = 7 };
		int height;			// ͼƬ��                          
		int width;			// ͼƬ��   
		int stepNum;		// ��Ƶ����
		int rasterType;		// ��դ����,1Ϊ���ܹ���,0Ϊȫ�ܹ�դ
		float pixelWidth[MAX_PHASE_STEP];// ÿ�����ƿ��
		int moveNum[MAX_PHASE_STEP];	 // ÿ�����Ʋ���
		double frangeNum[MAX_PHASE_STEP];// ÿ����������
		int index;                       // ������� 0 1��ʶ�������
		int graySubTd;		//�ҶȲ���ֵ
		// ��������λ���
		std::vector<std::vector<double>> GrayTable;
	};

	// ͼƬ����
	class GrayImageParam 
	{
	public:
		GrayImageParam():width(0),height(0),nChannel(0),data(NULL){};
		int width;					// ͼƬ���
		int height;					// ͼƬ�߶�
		int nChannel;
		unsigned char *data;		// ͼƬ���ݣ��ǵ�ͨ���Ҷ�ͼ
	};

	class DoubleImageParam
	{
	public:
		DoubleImageParam():width(0),height(0),data(NULL){};
		int width;					// ͼƬ���
		int height;					// ͼƬ�߶�
		double *data;				// ͼƬ���ݣ��ǵ�ͨ��������
	};

	// intrinsicMat�ڲξ���
	//	||	focal0		skew		principal0	||
	//	||		0		focal1		principal1	||
	//	||		0		0				1		||

	// ��ά������������
	class CameraParam
	{
	public:
		int imageWidth;				// ͼƬ��� 
		int imageHeight;			// ͼƬ�߶�

		// ����ڲ�
		double	focus[2];
		double	principal[2];
		double  alpha;

		double	distortion[5];		// ����ϵ�� K1 K2 P1 P2 skew

		// R��rotate�� T��trans�� �Ǵ���������ϵת�����������ϵ�ı任,����Xl = Xw * R + T;
		// �������ϵXl=(x0,y0,z0)';��������ϵ Xw = (x,y,z)'
		double trans[3];		    // ƽ������
		double rotate[9];			// ��ת����
	};

	enum
	{
		MAX_CALIBR_IMAGE_COUNT = 25,
		CALIBR_POINT_COUNT = 238	//14*17
	};

	// �궨������Բ�ռ���ά����
	class CalibrPanel3D
	{
	public:

		double pointx[CALIBR_POINT_COUNT];
		double pointy[CALIBR_POINT_COUNT];
		double pointz[CALIBR_POINT_COUNT];
	};

	// �궨����ͼƬ��ά����
	class CalibrPanel2D
	{
	public:

		double pointx[CALIBR_POINT_COUNT];
		double pointy[CALIBR_POINT_COUNT];
	};

	// ������궨
	class CalibrationParam
	{
	public:
		// �����ĵ�
		class Inspect
		{
		public:
			int		code[2];
			double	dist;
		};
	public:

		// ͼƬ����Բλ��
		int           imageCount;
		bool		  active[MAX_CALIBR_IMAGE_COUNT];
		CalibrPanel2D imagePanel2D[MAX_CALIBR_IMAGE_COUNT];
		CalibrPanel3D lookupPanel3D;

		// ����CCF�ļ���	
		char	ccfPath[260];

		// �������
		CameraParam camera;
		Inspect		inspect[2];
	};

	class AxisParam
	{
	public:
		double AxisCircleX[3];//X��ԭ��
		double AxisCircleY[3];//Y��ԭ��
		double AxisVectorX[3];//X����һ�㣬��X��ԭ�㹹��һ������
		double AxisVectorY[3];//Y����һ�㣬��Y��ԭ�㹹��һ������
		double AxisCircleA[3];//A��ԭ�㣨ӡģ��ת�ᣩ
		double AxisVectorA[3];//A����һ�㣬��A��ԭ�㹹��һ������
        double AngleFromRT[3];//У������ת��������ŷ���� 
	};

	//��ά�ؽ��Ĳ���
	class C2Cam1ProjRebuild_Para
	{
	public:
		int crow;//ͼ��������
		int ccol;//ͼ�������

		int num_find;//�������ҵĵ�����Ҫ����ini�ļ���
		int sample;  //��������

		int Near;//��ȷ��������Χ
		int Far;//��ȷ�����Զ��Χ

		int FiltWin;//�˲�����

		double HightSearchThre;//���������ֵ
		double LowSearchThre;//��С������ֵ
		double MedSearchThre;//�м�������ֵ������ȥ���ؽ�ʱ�Ŀ׶����

		int findRange;       //ȥ�ӵ�����
		float disThread;     
		int numThread;

		C2Cam1ProjRebuild_Para(void):crow(960),ccol(1280),num_find(5),sample(1),Near(100),Far(800),FiltWin(1),HightSearchThre(30*PI),
			LowSearchThre(PI/4),MedSearchThre(PI/4){};
	};

	//���ͶӰ�ı궨·���������ͶӰ���ķֱ��ʲ���(��ͶӰ���ؽ�)
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

	//���ͶӰ�ı궨·���������ͶӰ���ķֱ��ʲ���(����ͶӰ���ؽ�)
	class CStreroPara
	{
	public:
		std::string CalibPathL;
		std::string CalibPathR;

		int LeftSize[2];
		int RightSize[2];
	};

	// ScanRange���ͼ��ָ��һ��ͼ������ÿ��Ԫ����һ����ά���������������������ֵ
	//	Ϊ-1��ʾ�˴�û����ά�㡣
	class ScanRange
	{
	public:
		int width;		// ���ͼ��
		int height;		// ���ͼ��
		int* idList;	// ���������б�idList�����СΪwidth*height������Ԫ��Ϊ-1��ʾ�õ㲻���ڡ�

		int numPt;		// �������
		double* xPt;		// ����x��������
		double* yPt;		// ����y��������
		double* zPt;		// ����z��������
	};

	class CCamProjRebuild_Para
	{
	public:
		int crow;//ͼ��������
		int ccol;//ͼ�������

		int sample;  //��������

		CCamProjRebuild_Para(void):crow(1024),ccol(1280),sample(1){};
	};

	// ��ά����
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

	// ��ά����
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

	// ��ά�ؽ���Ӧ��
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