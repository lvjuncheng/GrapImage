/***********************************************
* ¶¨ÒåBBox2
* \author Çñ¿­¼Ñ
***********************************************/
#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_BBOX2_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_BBOX2_H

#include "PointType.h"
namespace Sn3DAlgorithm
{
	struct AABB2D
	{
		AABB2D(const Eigen::Vector2& lower, const Eigen::Vector2& upper)
			: lowerBound(lower), upperBound(upper)
		{}

		AABB2D(){}

		bool overlap(const AABB2D& aabb) const {
			Eigen::Vector2 d1, d2;
			d1 = aabb.lowerBound - upperBound;
			d2 = lowerBound - aabb.upperBound;

			if (d1.x() > 0.0f || d1.y() > 0.0f)
				return false;

			if (d2.x() > 0.0f || d2.y() > 0.0f)
				return false;

			return true;
		}

		bool contain(const Eigen::Vector2& pt) const{
			return pt.x() >= lowerBound.x() && pt.y() >= lowerBound.y()
				&& pt.x() <= upperBound.x() && pt.y() <= upperBound.y();
		}

		bool contain(const AABB2D& aabb) const {
			return aabb.lowerBound.x() >= lowerBound.x() && aabb.lowerBound.y() >= lowerBound.y()
				&& aabb.upperBound.x() <= upperBound.x() && aabb.upperBound.y() <= upperBound.y();
		}

		AABB2D get_translated(const Eigen::Vector2& trans) const{
			return AABB2D(lowerBound + trans, upperBound + trans);
		}

		Scalar get_perimeter() const {
			Scalar wx = upperBound.x() - lowerBound.x();
			Scalar wy = upperBound.y() - lowerBound.y();
			return 2.0f * (wx + wy);
		}

		void combine(const AABB2D& aabb1, const AABB2D& aabb2){
			lowerBound = Eigen::Vector2(Sn_min(aabb1.lowerBound.x(), aabb2.lowerBound.x()), Sn_min(aabb1.lowerBound.y(), aabb2.lowerBound.y()));
			upperBound = Eigen::Vector2(Sn_max(aabb1.upperBound.x(), aabb2.upperBound.x()), Sn_max(aabb1.upperBound.y(), aabb2.upperBound.y()));
		}

		Eigen::Vector2 lowerBound;
		Eigen::Vector2 upperBound;
	};
}
#endif