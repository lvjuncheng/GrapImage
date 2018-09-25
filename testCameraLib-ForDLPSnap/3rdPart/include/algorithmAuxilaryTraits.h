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
		Scalar faceMaxAngle;  ///< ��ˮƽ����ĽǶ�����[0, faceMaxAngle],����Ҫ���֧��
		Scalar facesMinArea;   ///< ֧�ŵ��������С���, С�������
		Scalar faceHorizontalThreshold; ///< ��ˮƽ����ĽǶ�����[0, faceHorizontalThreshold],����Ϊˮƽ�棬���ڱ߽���ص�֧��
		Scalar supportDistance; ///<֧�Ų������
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
		SupportPointLevel_Normal = 0,///<һ��֧�ŵ㣬������б�ǲ������������ĽǶȷ�Χ������֧��
		SupportPointLevel_Important,///<��Ҫ֧�ŵ㣬������б�������Ƕȷ�Χ�ͻ�����֧��
		SupportPointLevel_Significant///<�ش�֧�ŵ㣬�κ�����¶�������֧��
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
		Scalar maxInclined;///< >0 ������֧����ģ�ͽӴ�������б��ֵΪ�����б�Ƕȣ�ͬʱsupportParts����Ϊ���Σ�������Ĳ���͵����Ӵ����֣�������б����
		Scalar minFaceAngle;///< ֧�ű��泬���ýǶȵĲ��ֲ�����������б֧��
		Scalar conjunctionFactor;///< б֧����б����ת�۵���ֱ���ֵİ뾶��ֵ��ȡֵ[0,1]
		std::vector<SupportPart> supportParts;///<֧�ŵ�ÿһ���ֵĿ�ʼ�뾶�볤�ȣ�������Ҫ�������֣�����ģ�Ͳ��֣�������Ӵ��Ĳ��֣����������µĲ��ֽ��ᱻ��ȥ
		Scalar bottomRadius;///<֧��������Ӵ��İ뾶
		int nArris;///<���ɵ�֧��Ϊ��n����

		Scalar length;///<֧����ģ�������ֱ���ֳ��ȣ�����������ɽӿڸ�ֵ��
		Eigen::Vector3f endPt;///<֧��ĩ�˵����꣨����������ɽӿڸ�ֵ��
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
		std::vector<Point2Vector::ConstPtr> boundary; ///< ģ�͵�����
		std::vector<Point2Vector::ConstPtr> support;  ///< ֧�ŵ�����
		std::vector<Point2Vector::ConstPtr> raft; ///< ����������
	};

	///DLP�궨����Ҫ������
	struct DLPCalibrationData
	{
		std::vector<double> heightSamples; ///< ͼƬ���ڸ߶ȷ���Ĳ�����.size=H
		std::vector<double> widthSamples;  ///< ͼƬ���ڿ�ȷ���Ĳ�����.size=W
		std::vector<Eigen::Vector3> coords; ///< ��ӡ��ɨ���õĲ����������.size=H*W.coord[i*W+j]��ʾ��i�У���j�еĲ���������
		Eigen::Vector3 centerCoord;         ///< coords����ϵ�е����ĵ�����
	};

	///��ӡƽ�浽ͶӰƽ��Ĳ��ұ�
	struct DLPMapLookupTable
	{
		Eigen::Vector2   minValue; ///< ���ұ����Сֵ��
		Eigen::Vector2   maxValue; ///< ���ұ����Сֵ��
		Eigen::Vector2   gridSize; ///< ���ұ�ĸ��ӿ�
		Eigen::Vector2i  gridNum;  ///< ���ұ�ĸ�������
		Eigen::MatrixXf  table[2];   ///< ���ұ�
	};
}

#endif//SN3D_ALGORITHM_PRINT3D_AUXILARY_ALGORITHM_AUXILARY_TRAITS_H