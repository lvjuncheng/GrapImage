/***********************************************
 * 定义Point Color等数据结构和相关操作
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_POINT_TYPE_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_POINT_TYPE_H

#include "Print3DTraits.h"

namespace Sn3DAlgorithm
{
	//三维顶点坐标
	typedef Eigen::Matrix<Scalar, 3, 1> Point3;

	struct Point3Hasher{
		size_t operator()(const Point3& v) const
		{
#define SN_MM2INT(n) (size_t)((n) * 1000)
			return ((SN_MM2INT(v[0]) + 30 / 2) / 30) ^ (((SN_MM2INT(v[1]) + 30 / 2) / 30) << 10) ^ (((SN_MM2INT(v[2]) + 30 / 2) / 30) << 20);
#undef SN_MM2INT
		}
	};

	//颜色信息。Color[0]为R,Color[1]为G，Color[2]位B。颜色取值范围[0,1)
	typedef Eigen::Vector3f  Color;

	///浮点数据类型的二维点结构
	struct Point2
	{
		typedef Sn3DAlgorithm::Scalar Scalar;
		Point2() {};
		Point2( Scalar x , Scalar y )
		{
			_data[0] = x;
			_data[1] = y;
		}
		/////重载'[]' 作为右值
		//Scalar operator [] ( int idx ) const  
		//{
		//	assert(idx <= 1 && idx >= 0);
		//	return _data[idx];
		//}
		///重载'[]' 作为左值
		Scalar& operator [] ( int idx )  
		{
			assert(idx <= 1 && idx >= 0);
			return _data[idx];
		}
		///重载'[]' 作为右值
		const Scalar& operator [] (const int idx) const
		{
			assert(idx <= 1 && idx >= 0);
			return _data[idx];
		}
		///重载'<'
		bool operator < ( const Point2& q ) const  
		{
			if( x() == q.x() )
				return y() < q.y();
			else
				return x() < q.x();
		}
		///重载'=='
		bool operator == ( const Point2& q ) const  
		{
			if(( std::abs(x() - q.x()) < 1e-14 ) && (std::abs(y() - q.y()) < 1e-14))
				return true;
			else
				return false;
		}
		bool operator != (const Point2& q) const
		{
			return !operator==(q);
		}
		///重载‘-’
		Point2 operator - (const Point2& q) const {return Point2(x() - q.x(), y() - q.y());}
		///重载‘+’
		Point2 operator + (const Point2& q) const {return Point2(x() + q.x(), y() + q.y());}
		///重载‘*’
		Point2 operator * (Scalar s) const {return Point2(x() * s, y() * s);}
		///重载‘/’
		Point2 operator / (Scalar s) const {return Point2(x() / s, y() / s);}
		///重载‘-=’
		const Point2& operator -= (const Point2& q)  
		{ 
			x() = x() - q.x();
			y() = y() - q.y();
			return *this; 
		}
		///重载‘+=’
		const Point2& operator += (const Point2& q)  
		{ 
			x() = x() + q.x();
			y() = y() + q.y();
			return *this; 
		}
		///重载‘-’
		const Point2 operator-() const {return Point2(-x(), -y());}

		///rotate as a vector 
		const Point2& rotate(Scalar degCCW)
		{
			static const Scalar pi = std::acos(-1.0);
			Scalar s = std::sin(degCCW*pi / 180.0);
			Scalar c = std::cos(degCCW*pi / 180.0);
			Scalar newX = x()*c - y()*s;
			Scalar newY = x()*s + y()*c;
			x() = newX;
			y() = newY;
			return *this;
		}

		Scalar norm2() const {return x()*x()+y()*y();}
		Scalar norm() const {return std::sqrt(norm2());}
		Scalar dot(const Point2& q) const{return x()*q.x() + y()*q.y();}
		Scalar cross(const Point2& q) const{ return x()*q.y() - y()*q.x(); }
		void normalize(){
			Scalar l = norm();
			assert(l > 0);
			x() /= l;
			y() /= l;
		}
		bool is_on_line(const Point2& A, const Point2& B) const{
			return equal_scalar((A - *this).cross(B - *this), 0);
		}
		bool is_in_bBox(const Point2& A, const Point2& B) const{
			return x() >= Sn_min(A.x(), B.x()) && x() <= Sn_max(A.x(), B.x())
				&& y() >= Sn_min(A.y(), B.y()) && y() <= Sn_max(A.y(), B.y());
		}
		bool is_on_segment(const Point2& A, const Point2& B) const{
			return is_in_bBox(A, B) && is_on_line(A, B);
		}

		int size() const {return 2;}

		Scalar x() const { return _data[0]; }
		Scalar& x() {return _data[0];}
		Scalar y() const { return _data[1]; }
		Scalar& y() {return _data[1];}
	private:
		Scalar _data[2];
	};

	struct Point2Hasher{
		size_t operator()(const Point2& v) const
		{
#define SN_MM2INT(n) (size_t)((n) * 1000)
			return ((SN_MM2INT(v[0]) + 30 / 2) / 30) ^ (((SN_MM2INT(v[1]) + 30 / 2) / 30) << 15);
#undef SN_MM2INT
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Point2& p)
	{
		os << "(" << p.x() << "," << p.y() << ")";
		return os;
	}

	///<判断两个线段是否相交，并计算交点pt
	bool SN3D_COMMON_API cross_point(const Point2& A, const Point2& B, const Point2& C, const Point2& D, Point2& pt);

	///Scalar误差内判断两个点相等
	inline bool equal_point2(const Point2& p1, const Point2& p2)
	{
		return equal_scalar(p1.x(), p2.x())
			&& equal_scalar(p1.y(), p2.y());
	}

	///重载‘*’ 
	inline Point2 operator *(Scalar s, const Point2& b) {return Point2(b.x() * s, b.y() * s);}
	inline Point2 operator *(const Eigen::Matrix2& mat, const Point2& b) {
		Eigen::Vector2 ret = mat*Eigen::Vector2(b.x(), b.y());
		return Point2(ret.x(), ret.y());
	}

	///计算两个点Point2(a,b)和Point2(c,d)的复数乘法，得到Point2(ac-bd, ad+bc)
	inline Point2  get_complex_mul(const Point2& p0, const Point2& p1)
	{
		return Point2(p0[0]*p1[0] - p0[1]*p1[1],  p0[0]*p1[1]+p0[1]*p1[0]);
	}

	///二维整型元素的结构体
	struct PointKey
	{
		PointKey() {};
		PointKey( int x , int y )
		{
			this->x = x;
			this->y = y;
		}
		bool operator < ( const PointKey& q ) const  //重载'<'
		{
			if( x == q.x )
				return y < q.y;
			else
				return x < q.x;
		}
		bool operator == ( const PointKey& q ) const  //重载'=='
		{
			if(( x == q.x ) && (y == q.y))
				return true;
			else
				return false;
		}
		int x;
		int y;
	};

	/** \ 比较点A是否小于点B
	 *  \ 依次比较两点的分量坐标，根据分量坐标的大小作为比较结果
	 *  \ 如果在误差限内，认为分量坐标相等，继续比较下一分量
	 */
	template <typename Point_Type>
	class cmpPointLess
	{
	public:
		bool operator()(const Point_Type & A, const Point_Type & B) const
		{
			assert(A.size() == B.size());
			for(size_t i = 0; i < A.size(); ++i){
				if(std::abs(A[i]-B[i]) < Sn_F_EPS) continue;
				return A[i] < B[i];
			}

			return false;
		}
	};

	/** \ 比较点A是否大于点B
	 *  \ 依次比较两点的分量坐标，根据分量坐标的大小作为比较结果
	 *  \ 如果在误差限内，认为分量坐标相等，继续比较下一分量
	 */
	template <typename Point_Type>
	class cmpPointGreater
	{
	public:
		bool operator()(const Point_Type & A, const Point_Type & B) const
		{
			assert(A.size() == B.size());
			for(size_t i = 0; i < A.size(); ++i){
				if(std::abs(A[i]-B[i]) < Sn_F_EPS) continue;
				return A[i] > B[i];
			}

			return false;
		}
	};

}//namespace


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_POINT_TYPE_H
