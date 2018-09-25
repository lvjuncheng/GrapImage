#ifndef SN3D_ALGORITHM_PRINT3D_AUXILARY_ALGORITHM_AUXILARY_TRAITS_H
#define SN3D_ALGORITHM_PRINT3D_AUXILARY_ALGORITHM_AUXILARY_TRAITS_H

#ifdef _WIN32
#ifdef PRINT3D_AUXILARY_API_EXPORTS
#define SN3D_PRINT3D_AUXILARY_API __declspec(dllexport)
#else
#define SN3D_PRINT3D_AUXILARY_API __declspec(dllimport)
#endif
#else
#define SN3D_PRINT3D_AUXILARY_API
#endif

#ifdef _WIN32
#ifdef PRINT3D_AUXILARY_CLASS_EXPORTS
#define SN3D_PRINT3D_AUXILARY_CLASS __declspec(dllexport)
#else
#define SN3D_PRINT3D_AUXILARY_CLASS
#endif
#else
#define SN3D_PRINT3D_AUXILARY_CLASS
#endif

#include "print3DCommon/baseType/Path.h"
#include "common/baseType/Traits.h"

namespace Sn3DAlgorithm
{
	struct SupportGenerateCondition{
		Scalar faceMaxAngle;  ///< 与水平方向的角度属于[0, faceMaxAngle],则需要添加支撑
		Scalar facesMinArea;   ///< 支撑的区域的最小面积, 小于则忽略
		Scalar faceHorizontalThreshold; ///< 与水平方向的角度属于[0, faceHorizontalThreshold],则视为水平面，会在边界加重点支撑
		Scalar supportDistance; ///<支撑采样间距
		SupportGenerateCondition(
			Scalar fMaxAngle = 40.0f, Scalar fsMinArea = 0.5f , Scalar fHrztThres = 5.0f, Scalar dist = 0.5f
			) :
			faceMaxAngle(fMaxAngle),
			facesMinArea(fsMinArea),
			faceHorizontalThreshold(fHrztThres),
			supportDistance(dist)
		{}
	};

	enum SupportPointLevel{
		SupportPointLevel_Normal = 0,///<一般支撑点，法线倾斜角不符参数给定的角度范围则不生成支撑
		SupportPointLevel_Important,///<重要支撑点，法线倾斜角在最大角度范围就会生成支撑
		SupportPointLevel_Significant///<重大支撑点，任何情况下都会生成支撑
	};

	struct SupportPointInfo
	{
		Eigen::Vector3 pt;
		Eigen::Vector3 nrv;
		SupportPointLevel level;
	};

	struct SupportPart{
		Scalar radius;
		Scalar length;
	};

	struct SupportStructure{
		Scalar maxInclined;///< >0 则启用支撑与模型接触部分倾斜，值为最大倾斜角度；同时supportParts至少为三段，除必须的插入和底座接触部分，还有倾斜部分
		Scalar minFaceAngle;///< 支撑表面超过该角度的部分才予以生成倾斜支撑
		Scalar conjunctionFactor;///< 斜支撑倾斜部分转折到竖直部分的半径插值，取值[0,1]
		std::vector<SupportPart> supportParts;///<支撑的每一部分的开始半径与长度，至少需要两个部分（插入模型部分，与底座接触的部分），底座以下的部分将会被裁去
		Scalar bottomRadius;///<支撑与底座接触的半径
		int nArris;///<生成的支撑为正n棱柱

		Scalar length;///<支撑在模型外的竖直部分长度（输出参数，由接口赋值）
		Eigen::Vector3f endPt;///<支撑末端的坐标（输出参数，由接口赋值）
		SupportStructure() :nArris(4) {}
	};

	struct TreeConfigure
	{
		Scalar sampleDistance;				// support points sample distance
		// build pillar paramerters
		Scalar ptMoveDown;					// gap of start point to object
		Scalar growDownDistance;			// every time one node go down distance
		Scalar safePairDistance;			// pair two node distance
		Scalar maxSafeAngle;				// for the bridge structure
		Scalar headThickness;				// the thickness of the pillar from start point
		Scalar footThickness;               // to make foot
		Scalar normalRadius;				// pillar radius
		Scalar objectRadius;				// object stick radius or head radius
		Scalar footRadius;					// foot radius;
		unsigned int polygonSidesNumber;	// edges of regular polygon for pillar
	};

	struct DLPLayer
	{
		std::vector<Point2Vector::ConstPtr> boundary; ///< 模型的轮廓
		std::vector<Point2Vector::ConstPtr> support;  ///< 支撑的轮廓
		std::vector<Point2Vector::ConstPtr> raft; ///< 基座的轮廓
	};

	///DLP标定所需要的数据
	struct DLPCalibrationData
	{
		std::vector<double> heightSamples; ///< 图片上在高度方向的采样点.size=H
		std::vector<double> widthSamples;  ///< 图片上在宽度方向的采样点.size=W
		std::vector<Eigen::Vector3> coords; ///< 打印后，扫描获得的采样点的坐标.size=H*W.coord[i*W+j]表示第i行，第j列的采样点坐标
		Eigen::Vector3 centerCoord;         ///< coords坐标系中的中心点坐标
	};

	///打印平面到投影平面的查找表
	struct DLPMapLookupTable
	{
		Eigen::Vector2   minValue; ///< 查找表的最小值点
		Eigen::Vector2   maxValue; ///< 查找表的最小值点
		Eigen::Vector2   gridSize; ///< 查找表的格子宽长
		Eigen::Vector2i  gridNum;  ///< 查找表的格子数量
		Eigen::MatrixXf  table[2];   ///< 查找表
	};
}

#endif//SN3D_ALGORITHM_PRINT3D_AUXILARY_ALGORITHM_AUXILARY_TRAITS_H