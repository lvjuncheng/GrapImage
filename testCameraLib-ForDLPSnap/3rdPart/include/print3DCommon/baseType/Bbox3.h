/***********************************************
 * 定义bounding box类
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_BBOX_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_BBOX_H

#include "../baseType/Print3DTraits.h"
#include "commonTool.h"
#include "../baseType/PointType.h"

namespace Sn3DAlgorithm
{
	///3维的bounding box类
	class SN3D_PRINT3D_COMMON_CLASS Bbox3
	{
	public:
		Bbox3() 
		{
			_rep[0] = _rep[1] = _rep[2] = 0;
			_rep[3] = _rep[4] = _rep[5] = 0;
		}
		Bbox3(Scalar xMin, Scalar yMin, Scalar zMin, Scalar xMax, Scalar yMax, Scalar zMax)
		{
			_rep[0] = xMin; _rep[1] = yMin; _rep[2] = zMin;
			_rep[3] = xMax; _rep[4] = yMax; _rep[5] = zMax;
		}

		Scalar xMin() const { return _rep[0]; }
		Scalar yMin() const { return _rep[1]; }
		Scalar zMin() const { return _rep[2]; }
		Scalar xMax() const { return _rep[3]; }
		Scalar yMax() const { return _rep[4]; }
		Scalar zMax() const { return _rep[5]; }

		Scalar& xMin()  { return _rep[0]; }
		Scalar& yMin()  { return _rep[1]; }
		Scalar& zMin()  { return _rep[2]; }
		Scalar& xMax()  { return _rep[3]; }
		Scalar& yMax()  { return _rep[4]; }
		Scalar& zMax()  { return _rep[5]; }

		Scalar Min(int i) const
		{
			if(i == 0) return xMin();
			if(i == 1) return yMin();
			return zMin();
		}
		Scalar Max(int i) const
		{
			if(i == 0) return xMax();
			if(i == 1) return yMax();
			return zMax();
		}

		Scalar& Min(int i)
		{
			if(i == 0) return xMin();
			if(i == 1) return yMin();
			return zMin();
		}
		Scalar& Max(int i)
		{
			if(i == 0) return xMax();
			if(i == 1) return yMax();
			return zMax();
		}

		inline Bbox3 operator + (const Bbox3& b) const;
		inline Bbox3& operator += (const Bbox3& b);


		///获得某个维度的长度
		inline Scalar get_side(int i) const { return Max(i) - Min(i); }
		//获得对角线长度
		inline Scalar get_diag_length() const { return sqrt(get_side(0)*get_side(0) + get_side(1)*get_side(1) + get_side(2)*get_side(2)); }
		//获得box中心
		Eigen::Vector3 get_center() const { return Eigen::Vector3((Max(0) + Min(0)) / 2, (Max(1) + Min(1)) / 2, (Max(2) + Min(2)) / 2); }

		///获得包围盒所占体积
		Scalar get_volume() const { return get_side(0)*get_side(1)*get_side(2); }
		/**  \brief 从点坐标序列计算包围盒
		*  \param[in]  coords    所有点坐标
		*/
		void calc_from_coords(const std::vector<Eigen::Vector3>& coords);
	private:
		Scalar _rep[6]; //{xMin, yMin, zMin, xMax, yMax, zMax}
	};

	Bbox3 Bbox3::operator + (const Bbox3& b) const
	{
		return Bbox3(Sn_min(xMin(), b.xMin()),
			Sn_min(yMin(), b.yMin()),
			Sn_min(zMin(), b.zMin()),
			Sn_max(xMax(), b.xMax()),
			Sn_max(yMax(), b.yMax()),
			Sn_max(zMax(), b.zMax()));
	}
	Bbox3& Bbox3::operator += (const Bbox3& b)
	{
		_rep[0] = Sn_min(xMin(), b.xMin());
		_rep[1] = Sn_min(yMin(), b.yMin());
		_rep[2] = Sn_min(zMin(), b.zMin());
		_rep[3] = Sn_max(xMax(), b.xMax());
		_rep[4] = Sn_max(yMax(), b.yMax());
		_rep[5] = Sn_max(zMax(), b.zMax());
		return *this;
	}

	///返回两个bbox是否相交
	inline bool is_overlap(const Bbox3& b1, const Bbox3& b2)
	{
		if (b1.xMax() < b2.xMin() || b2.xMax() < b1.xMin()) return false;
		if (b1.yMax() < b2.yMin() || b2.yMax() < b1.yMin()) return false;
		if (b1.zMax() < b2.zMin() || b2.zMax() < b1.zMin()) return false;
		return true;
	}
	
	///返回两个box是否嵌套关系
	inline bool is_inside(const Bbox3& b1, const Bbox3& b2)
	{
		return b1.xMin() >= b2.xMin() && b1.yMin() >= b2.yMin() &&
			b1.zMin() >= b2.zMin() && b1.xMax() <= b2.xMax() &&
			b1.yMax() <= b2.yMax() && b1.zMax() <= b2.zMax();
	}

	////////////////////////////////////Bbox排序函数
	bool SN3D_PRINT3D_COMMON_API comp_x(const Bbox3& b1, const Bbox3& b2);
	bool SN3D_PRINT3D_COMMON_API comp_y(const Bbox3& b1, const Bbox3& b2);
	bool SN3D_PRINT3D_COMMON_API comp_z(const Bbox3& b1, const Bbox3& b2);

	/////////////////////////////////////Bbox相关的操作
	///返回point是否在bbox内部
	bool SN3D_PRINT3D_COMMON_API is_in_box(const Point3& point, const Bbox3& bbox);

	///计算point到bbox中8个顶点中最近点的距离，返回距离的平方
	Scalar SN3D_PRINT3D_COMMON_API closest_distance2(const Point3& point, const Bbox3& bbox);

	///计算point到bbox距离的平方
	Scalar SN3D_PRINT3D_COMMON_API distance2(const Point3& point, const Bbox3& bbox);
}


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_BBOX_H
