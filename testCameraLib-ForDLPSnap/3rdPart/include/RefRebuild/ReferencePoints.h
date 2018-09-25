// File: sn3DReferencePoints.h

#ifndef _sn3DReferencePoints_H
#define _sn3DReferencePoints_H

#include "../PhaseUnwrap/sn3DStereoDefine.h"

namespace LY3DStereoBuild
{
	//===========================================================================
	// ������	RefPointsAlign
	// 
	// ������	freePts			[in]��־�����nPoint���Լ���־�����꣨xPoints��yPoints��yPoints��	
	//							[out]IDList��־�����ţ�ģ�͵ľ���
	//			lengthEpsilon	[in]����߶γ��ȳ��������ֵ��Ϊ���µ㣬С�������ֵ��Ϊ��ͬһ����
	//			removeEpsilon	[in]���С�������ֵ��Ϊ�Ǿ�ȷ�㣬�����־��ƴ�ӡ�����������ֵ����Ϊ�����ϴ�ĵ㣬������ƴ��id��Ϊ-1
	//			minCount		[in]���ٹ��������,��������3����ȡֵ3��4��5��6...
	// ���ܣ���־��ƥ�䡣
	//		ʹ�÷�����ѭ������sn3DInsertRefPoints�ο���־�㣬�ٵ���sn3DRefPointsAlign
	//===========================================================================

	bool RefPointsAlign( RefPoints& freePts );
	bool RefPointsAlign( RefPoints& freePts, double lengthEpsilon, double removeEpsilon,int minCount);
	
	//===========================================================================
	// ������	RecognPoints
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
	bool RecognPoints(int &nPt, double*& xPts, double*& yPts,GrayImageParam& image, CannyParam& canny, CircleParam& circle);
	//===========================================================================
	// ������	InsertRefPoints
	// 
	// ������	rangeId		[in]ģ�͵�id
	//			refPts		[in]ģ���ϵı�־��
	//		
	// ���ܣ�����ο���	
	//===========================================================================
	bool InsertRefPoints(int rangeId, RefPoints& refPts);



}

#endif //_sn3DReferencePoints_H