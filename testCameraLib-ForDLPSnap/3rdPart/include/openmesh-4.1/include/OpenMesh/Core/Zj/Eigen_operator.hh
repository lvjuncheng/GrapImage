#ifndef ZJ_EIGEN_OPERATOR_H
#define ZJ_EIGEN_OPERATOR_H 

#include <Eigen/Dense>

namespace OpenMesh{

template<typename Scalar, int N>
inline Scalar dot(const Eigen::Matrix<Scalar, N, 1>& v1, const Eigen::Matrix<Scalar, N, 1>& v2)
{
	return v1.dot(v2);
}

template<typename Scalar>
inline Eigen::Matrix<Scalar, 3, 1> cross(const Eigen::Matrix<Scalar, 3, 1>& v1, 
										 const Eigen::Matrix<Scalar, 3, 1>& v2)
{
	return v1.cross(v2);
}
template<typename Scalar>
inline Eigen::Matrix<Scalar, 3, 1> operator%(const Eigen::Matrix<Scalar, 3, 1>& v1, 
											 const Eigen::Matrix<Scalar, 3, 1>& v2)
{
	return v1.cross(v2);
}

template<typename Scalar, int N>
inline Scalar norm2(const Eigen::Matrix<Scalar, N, 1>& v)
{
	return v.squaredNorm();
}

template<typename Scalar, int N>
inline void constant(Eigen::Matrix<Scalar, N, 1>& mat,Scalar value)
{
	mat.setConstant(value);
}

} //namepspace

#endif