/***************************************************
 * 定义包围盒类以及相关函数
 * \author 张健
 ***************************************************/
#ifndef SN3D_ALGORITHM_COMMON_BOUNDING_BOX_H
#define SN3D_ALGORITHM_COMMON_BOUNDING_BOX_H

#include "Traits.h"
#include "RigidMatrix.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace Sn3DAlgorithm
{
	/** \brief 包围盒类  
	 *         2____________3
	 *         /|          /|
	 *        / |         / |         y
	 *   	6/__|________/7 |        |
	 *       |  |        |  |        |
	 *       | 0---------|--/ 1      /------>x
	 *       | /         | /        /
	 *       |/          |/         z
	 *       -------------
	 *       4            5
	 */
	class SN3D_COMMON_CLASS BoundingBox
	{
	public:
		BoundingBox() { empty(); }
		BoundingBox(const Eigen::Vector3& min, const Eigen::Vector3& max){
			set_bounding_box(min, max);
		}

		void set_bounding_box(const Eigen::Vector3& min, const Eigen::Vector3& max) {
			_min = min;
			_max = max;
			if(is_empty()) empty();
		}
		void get_bounding_box(Eigen::Vector3& min, Eigen::Vector3& max) const {
			min = _min;
			max = _max;
		}


		Scalar xMin() const { return _min[0]; }
		Scalar yMin() const { return _min[1]; }
		Scalar zMin() const { return _min[2]; }
		Scalar xMax() const { return _max[0]; }
		Scalar yMax() const { return _max[1]; }
		Scalar zMax() const { return _max[2]; }

		Scalar& xMin()  { return _min[0]; }
		Scalar& yMin()  { return _min[1]; }
		Scalar& zMin()  { return _min[2]; }
		Scalar& xMax()  { return _max[0]; }
		Scalar& yMax()  { return _max[1]; }
		Scalar& zMax()  { return _max[2]; }

		//获得box中心
		Eigen::Vector3 get_center() const { 
			return (_max + _min) / 2; 
		}

		///获得box的顶点坐标
		Eigen::Vector3 corner(int i) const {
			Eigen::Vector3 cor;
			corner(i, cor);
			return cor;
		}

		///获得box的顶点坐标
		void corner(int i, Eigen::Vector3& cor) const{
			assert(i >= 0 && i <= 7);
			cor[0] = (i&1) ? _max[0] : _min[0];
			cor[1] = (i&2) ? _max[1] : _min[1];
			cor[2] = (i&4) ? _max[2] : _min[2];
		}
		///加入一个点，更新box。使box包含改点
		void add(const Eigen::Vector3& p) {
			for (int i = 0; i < 3; ++i)
			{
				if( p[i] < _min[i]) _min[i] = p[i];
				if( p[i] > _max[i]) _max[i] = p[i];
			}
		}
		///加入一个点，更新box。使box包含改点
		void add(Scalar x, Scalar y, Scalar z) {
			add(Eigen::Vector3(x, y, z));
		}
		///使box为空
		void empty(){
			_min[0] = _min[1] = _min[2] = std::numeric_limits<Scalar>::max();
			_max[0] = _max[1] = _max[2] = -std::numeric_limits<Scalar>::max();
		}
		///点p是否在box中
		bool is_in_box(const Eigen::Vector3& p) const{
			for (int i = 0; i < 3; ++i)
			{
				if( _min[i]>p[i] || _max[i]<p[i])   return false;
			}
			return true;
		}
		///box是否为空
		bool is_empty() const{
			return (_min[0]>_max[0]) || (_min[1]>_max[1]) || (_min[2]>_max[2]);
		}
		inline Scalar get_diag_length() const{
			return (_max - _min).norm();
		}
		///result = this同box的交集。如果交集为空，返回false. result不能为*this的引用
		inline bool intersection(const BoundingBox& box, BoundingBox& result) const;
		///this = this同box的交集。如果交集为空，返回false
		inline bool intersection_in_self(const BoundingBox& box);
		///result = this同box的并集。 result不能为*this的引用
		inline void join(const BoundingBox& box, BoundingBox& result) const;
		///this = this同box的并集
		inline void join_in_self(const BoundingBox& box);
		///result = mat*this。 result不能为*this的引用
		inline void transform(const RigidMatrix& mat, BoundingBox& result) const;
		///this = mat*this
		inline void transform_self(const RigidMatrix& mat);

		inline BoundingBox operator + (const BoundingBox& b) const;
		inline BoundingBox& operator += (const BoundingBox& b);

	private:
		Eigen::Vector3 _min;
		Eigen::Vector3 _max;
	};

	////////////////////////////////////////////////////inline
	inline bool BoundingBox::intersection(const BoundingBox& box, BoundingBox& result) const 
	{
		for (int i = 0; i < 3; ++i)
		{
			result._min[i] =  _min[i]>box._min[i]?_min[i]:box._min[i];
			result._max[i] =  _max[i]<box._max[i]?_max[i]:box._max[i];         
		}
		if(result.is_empty()) return false;
		return true;
	}
	inline bool BoundingBox::intersection_in_self(const BoundingBox& box) 
	{
		for (int i = 0; i < 3; ++i)
		{
			_min[i] =  _min[i]>box._min[i]?_min[i]:box._min[i];
			_max[i] =  _max[i]<box._max[i]?_max[i]:box._max[i];         
		}
		if(is_empty())
		{
			empty();
			return false;
		}
		return true;
	}
	inline void BoundingBox::join(const BoundingBox& box, BoundingBox& result) const
	{
		for (int i = 0; i < 3; ++i)
		{
			result._min[i] =  _min[i]<box._min[i]?_min[i]:box._min[i];
			result._max[i] =  _max[i]>box._max[i]?_max[i]:box._max[i];         
		}
	}
	inline void BoundingBox::join_in_self(const BoundingBox& box) {
		for (int i = 0; i < 3; ++i)
		{
			_min[i] =  _min[i]<box._min[i]?_min[i]:box._min[i];
			_max[i] =  _max[i]>box._max[i]?_max[i]:box._max[i];         
		}
	}
	inline void BoundingBox::transform(const RigidMatrix& mat, BoundingBox& result) const{
		result.empty();
		for(int i = 0; i < 8; ++i) result.add(mat*corner(i));
	}
	inline void BoundingBox::transform_self(const RigidMatrix& mat){
		BoundingBox result;
		transform(mat, result);
		_min = result._min;
		_max = result._max;
	}

	BoundingBox BoundingBox::operator + (const BoundingBox& b) const
	{
		return BoundingBox(Eigen::Vector3(Sn_min(xMin(), b.xMin()),
			Sn_min(yMin(), b.yMin()),
			Sn_min(zMin(), b.zMin())),
			Eigen::Vector3(Sn_max(xMax(), b.xMax()),
			Sn_max(yMax(), b.yMax()),
			Sn_max(zMax(), b.zMax())));
	}
	BoundingBox& BoundingBox::operator += (const BoundingBox& b)
	{
		_min[0] = Sn_min(xMin(), b.xMin());
		_min[1] = Sn_min(yMin(), b.yMin());
		_min[2] = Sn_min(zMin(), b.zMin());
		_max[0] = Sn_max(xMax(), b.xMax());
		_max[1] = Sn_max(yMax(), b.yMax());
		_max[2] = Sn_max(zMax(), b.zMax());
		return *this;
	}

	///返回两个bbox是否相交
	inline bool is_overlap(const BoundingBox& b1, const BoundingBox& b2)
	{
		if (b1.xMax() < b2.xMin() || b2.xMax() < b1.xMin()) return false;
		if (b1.yMax() < b2.yMin() || b2.yMax() < b1.yMin()) return false;
		if (b1.zMax() < b2.zMin() || b2.zMax() < b1.zMin()) return false;
		return true;
	}

	///返回两个box是否嵌套关系
	inline bool is_inside(const BoundingBox& b1, const BoundingBox& b2)
	{
		return b1.xMin() >= b2.xMin() && b1.yMin() >= b2.yMin() &&
			b1.zMin() >= b2.zMin() && b1.xMax() <= b2.xMax() &&
			b1.yMax() <= b2.yMax() && b1.zMax() <= b2.zMax();
	}

}//Sn3DAlgorithm

#endif //SN3D_ALGORITHM_COMMON_BOUNDING_BOX_H