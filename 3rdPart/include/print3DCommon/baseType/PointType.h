/***********************************************
 * ����Point Color�����ݽṹ����ز���
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_POINT_TYPE_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_POINT_TYPE_H

#include "Print3DTraits.h"

namespace Sn3DAlgorithm
{
	//��ά��������
	typedef Eigen::Matrix<Scalar, 3, 1> Point3;

	struct Point3Hasher{
		size_t operator()(const Point3& v) const
		{
#define SN_MM2INT(n) (size_t)((n) * 1000)
			return ((SN_MM2INT(v[0]) + 30 / 2) / 30) ^ (((SN_MM2INT(v[1]) + 30 / 2) / 30) << 10) ^ (((SN_MM2INT(v[2]) + 30 / 2) / 30) << 20);
#undef SN_MM2INT
		}
	};

	//��ɫ��Ϣ��Color[0]ΪR,Color[1]ΪG��Color[2]λB����ɫȡֵ��Χ[0,1)
	typedef Eigen::Vector3f  Color;

	///�����������͵Ķ�ά��ṹ
	struct Point2
	{
		typedef Sn3DAlgorithm::Scalar Scalar;
		Point2() {};
		Point2( Scalar x , Scalar y )
		{
			_data[0] = x;
			_data[1] = y;
		}
		/////����'[]' ��Ϊ��ֵ
		//Scalar operator [] ( int idx ) const  
		//{
		//	assert(idx <= 1 && idx >= 0);
		//	return _data[idx];
		//}
		///����'[]' ��Ϊ��ֵ
		Scalar& operator [] ( int idx )  
		{
			assert(idx <= 1 && idx >= 0);
			return _data[idx];
		}
		///����'[]' ��Ϊ��ֵ
		const Scalar& operator [] (const int idx) const
		{
			assert(idx <= 1 && idx >= 0);
			return _data[idx];
		}
		///����'<'
		bool operator < ( const Point2& q ) const  
		{
			if( x() == q.x() )
				return y() < q.y();
			else
				return x() < q.x();
		}
		///����'=='
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
		///���ء�-��
		Point2 operator - (const Point2& q) const {return Point2(x() - q.x(), y() - q.y());}
		///���ء�+��
		Point2 operator + (const Point2& q) const {return Point2(x() + q.x(), y() + q.y());}
		///���ء�*��
		Point2 operator * (Scalar s) const {return Point2(x() * s, y() * s);}
		///���ء�/��
		Point2 operator / (Scalar s) const {return Point2(x() / s, y() / s);}
		///���ء�-=��
		const Point2& operator -= (const Point2& q)  
		{ 
			x() = x() - q.x();
			y() = y() - q.y();
			return *this; 
		}
		///���ء�+=��
		const Point2& operator += (const Point2& q)  
		{ 
			x() = x() + q.x();
			y() = y() + q.y();
			return *this; 
		}
		///���ء�-��
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

	///<�ж������߶��Ƿ��ཻ�������㽻��pt
	bool SN3D_COMMON_API cross_point(const Point2& A, const Point2& B, const Point2& C, const Point2& D, Point2& pt);

	///Scalar������ж����������
	inline bool equal_point2(const Point2& p1, const Point2& p2)
	{
		return equal_scalar(p1.x(), p2.x())
			&& equal_scalar(p1.y(), p2.y());
	}

	///���ء�*�� 
	inline Point2 operator *(Scalar s, const Point2& b) {return Point2(b.x() * s, b.y() * s);}
	inline Point2 operator *(const Eigen::Matrix2& mat, const Point2& b) {
		Eigen::Vector2 ret = mat*Eigen::Vector2(b.x(), b.y());
		return Point2(ret.x(), ret.y());
	}

	///����������Point2(a,b)��Point2(c,d)�ĸ����˷����õ�Point2(ac-bd, ad+bc)
	inline Point2  get_complex_mul(const Point2& p0, const Point2& p1)
	{
		return Point2(p0[0]*p1[0] - p0[1]*p1[1],  p0[0]*p1[1]+p0[1]*p1[0]);
	}

	///��ά����Ԫ�صĽṹ��
	struct PointKey
	{
		PointKey() {};
		PointKey( int x , int y )
		{
			this->x = x;
			this->y = y;
		}
		bool operator < ( const PointKey& q ) const  //����'<'
		{
			if( x == q.x )
				return y < q.y;
			else
				return x < q.x;
		}
		bool operator == ( const PointKey& q ) const  //����'=='
		{
			if(( x == q.x ) && (y == q.y))
				return true;
			else
				return false;
		}
		int x;
		int y;
	};

	/** \ �Ƚϵ�A�Ƿ�С�ڵ�B
	 *  \ ���αȽ�����ķ������꣬���ݷ�������Ĵ�С��Ϊ�ȽϽ��
	 *  \ �����������ڣ���Ϊ����������ȣ������Ƚ���һ����
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

	/** \ �Ƚϵ�A�Ƿ���ڵ�B
	 *  \ ���αȽ�����ķ������꣬���ݷ�������Ĵ�С��Ϊ�ȽϽ��
	 *  \ �����������ڣ���Ϊ����������ȣ������Ƚ���һ����
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
