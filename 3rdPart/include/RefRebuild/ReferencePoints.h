// File: sn3DReferencePoints.h

#ifndef _sn3DReferencePoints_H
#define _sn3DReferencePoints_H

#include "../PhaseUnwrap/sn3DStereoDefine.h"

namespace LY3DStereoBuild
{
	//===========================================================================
	// 函数：	RefPointsAlign
	// 
	// 参数：	freePts			[in]标志点个数nPoint，以及标志点坐标（xPoints，yPoints，yPoints）	
	//							[out]IDList标志点的序号，模型的矩阵
	//			lengthEpsilon	[in]如果线段长度超出这个阈值认为是新点，小于这个阈值认为是同一个点
	//			removeEpsilon	[in]如果小于这个阈值认为是精确点，参与标志点拼接。如果大于这个值，认为是误差较大的点，不参与拼接id设为-1
	//			minCount		[in]最少公共点个数,不能少于3。可取值3，4，5，6...
	// 功能：标志点匹配。
	//		使用方法先循环加入sn3DInsertRefPoints参考标志点，再调用sn3DRefPointsAlign
	//===========================================================================

	bool RefPointsAlign( RefPoints& freePts );
	bool RefPointsAlign( RefPoints& freePts, double lengthEpsilon, double removeEpsilon,int minCount);
	
	//===========================================================================
	// 函数：	RecognPoints
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
	bool RecognPoints(int &nPt, double*& xPts, double*& yPts,GrayImageParam& image, CannyParam& canny, CircleParam& circle);
	//===========================================================================
	// 函数：	InsertRefPoints
	// 
	// 参数：	rangeId		[in]模型的id
	//			refPts		[in]模型上的标志点
	//		
	// 功能：加入参考点	
	//===========================================================================
	bool InsertRefPoints(int rangeId, RefPoints& refPts);



}

#endif //_sn3DReferencePoints_H