/***********************************************
 * 定义所有的底层数据结构
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_TRAIST_H
#define SN3D_ALGORITHM_COMMON_TRAIST_H

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <Eigen/StdVector>

// 接口导入导出
#ifdef _WIN32
	#ifdef COMMON_API_EXPORTS
		#define SN3D_COMMON_API __declspec(dllexport)
	#else
		#define SN3D_COMMON_API __declspec(dllimport)
	#endif
#else
	#define SN3D_COMMON_API
#endif

// 类导出(方便代码的调试。在打包算法库时，用该前缀声明的接口或者类，不会导出)
#ifdef _WIN32
	#ifdef COMMON_CLASS_EXPORTS
		#define SN3D_COMMON_CLASS __declspec(dllexport)
	#else
		#define SN3D_COMMON_CLASS 
	#endif
#else
	#define SN3D_COMMON_CLASS
#endif

//!< 关键字
#ifndef _MSC_VER
#define NOEXCEPT noexcept
#elif _MSC_VER >= 1900 //vs15及以上
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

namespace Sn3DAlgorithm
{
	typedef unsigned char SnByte;

	///算法接口的返回值
	enum SN3D_COMMON_API RetVal {
		RetVal_OK					///< 执行正确
		, RetVal_FINISHED			///< 执行完成，但并未做操作
		, RetVal_ILLEGAL_INPUT		///< 输入非法，需检查输入参数的合法性
		, RetVal_RUNNING_ERROR		///< 运行时报错, 可能是由于输入的待处理数据无法应用算法等
		, RetVal_FATAL_ERROR		///< 执行发生严重错误，虽然调用其它独立的接口不受本次影响，但不建议继续调用
		, RetVal_ENCRYPTION_ERROR   ///< 加密出现问题
		, RetVal_CANCELLED          ///< 被外部强制结束
		//-----------------------------特殊返回类型-------------------------------------------
		, RetVal_NO_DONGLE			///< 没有加密狗
		, RetVal_SPECIAL_TIME		///< 特定时间
		, RetVal_MULTIPLY_REGIONS	///< 模型 存在多个连通区域；用于全局优化、组组拼接、激光扫描仪标志点优化    
	};

	//UNDO: 用类来定义，可以定义很多功能
	/** \brief 声明算法运行进度的返回函数
	 *  \param[in] 算法运行的进度，范围[0,100]
	 *  \return    返回0表示程序继续运行，返回非0表示终止程序
	 */
	typedef int (*CallbackProc) (int);
	typedef std::function<int(int)> CallbackProcFun;


#ifndef PI
	const double PI = 3.141592653589793;
#endif

	///获得最小值
	template <typename T>
	inline T Sn_min(T a, T b)
	{
		if (a <= b) return a;
		return b;
	}
	///获得最大值 
	template <typename T>
	inline T Sn_max(T a, T b)
	{
		if (a >= b) return a;
		return b;
	}

	/**
	* 该库使用的基本的浮点类型.
	* 这样定义方便float和double之间切换
	*/
	typedef float Scalar;
	// 浮点数为零阈值,
#define Sn_S_EPS  (1e-5)
#define Sn_F_EPS  (1e-5)
#define Sn_D_EPS  (1e-8)
}







inline bool equal_scalar(float a, float b){
	float tmp = a - b;
	return tmp >= -Sn_F_EPS && tmp <= Sn_F_EPS;
}
inline bool equal_scalar(double a, double b){
	double tmp = a - b;
	return tmp >= -Sn_D_EPS && tmp <= Sn_D_EPS;
}

inline bool less_scalar(float a, float b){
	return a - b < -Sn_F_EPS;
}

inline bool less_scalar(double a, double b){
	return a - b < -Sn_D_EPS;
}

namespace Eigen
{
	typedef Matrix<Sn3DAlgorithm::Scalar, 2, 2> Matrix2;
	typedef Matrix<Sn3DAlgorithm::Scalar, 3, 3> Matrix3;
	typedef Matrix<Sn3DAlgorithm::Scalar, 4, 4> Matrix4;
	typedef Matrix<Sn3DAlgorithm::Scalar, 5, 5> Matrix5;
	typedef Matrix<Sn3DAlgorithm::Scalar, 6, 6> Matrix6;
	typedef Matrix<Sn3DAlgorithm::Scalar, Dynamic, Dynamic> MatrixX;

	typedef Matrix<Sn3DAlgorithm::Scalar, 2, 1> Vector2;
	typedef Matrix<Sn3DAlgorithm::Scalar, 3, 1> Vector3;
	typedef Matrix<Sn3DAlgorithm::Scalar, 4, 1> Vector4;
	typedef Matrix<Sn3DAlgorithm::Scalar, 5, 1> Vector5;
	typedef Matrix<Sn3DAlgorithm::Scalar, 6, 1> Vector6;
	typedef Matrix<Sn3DAlgorithm::Scalar, Dynamic, 1> VectorX;

	typedef Matrix<Sn3DAlgorithm::Scalar, 1, 2> RowVector2;
	typedef Matrix<Sn3DAlgorithm::Scalar, 1, 3> RowVector3;
	typedef Matrix<Sn3DAlgorithm::Scalar, 1, 4> RowVector4;
	typedef Matrix<Sn3DAlgorithm::Scalar, 1, 5> RowVector5;
	typedef Matrix<Sn3DAlgorithm::Scalar, 1, 6> RowVector6;
	typedef Matrix<Sn3DAlgorithm::Scalar, 1, Dynamic> RowVectorX;

	typedef Matrix<size_t, 2, 2> Matrix2s;
	typedef Matrix<size_t, 3, 3> Matrix3s;
	typedef Matrix<size_t, 4, 4> Matrix4s;
	typedef Matrix<size_t, 5, 5> Matrix5s;
	typedef Matrix<size_t, 6, 6> Matrix6s;
	typedef Matrix<size_t, Dynamic, Dynamic> MatrixXs;

	typedef Matrix<size_t, 2, 1> Vector2s;
	typedef Matrix<size_t, 3, 1> Vector3s;
	typedef Matrix<size_t, 4, 1> Vector4s;
	typedef Matrix<size_t, 5, 1> Vector5s;
	typedef Matrix<size_t, 6, 1> Vector6s;
	typedef Matrix<size_t, Dynamic, 1> VectorXs;

	typedef Matrix<size_t, 1, 2> RowVector2s;
	typedef Matrix<size_t, 1, 3> RowVector3s;
	typedef Matrix<size_t, 1, 4> RowVector4s;
	typedef Matrix<size_t, 1, 5> RowVector5s;
	typedef Matrix<size_t, 1, 6> RowVector6s;
	typedef Matrix<size_t, 1, Dynamic> RowVectorXs;

	typedef Matrix<float, 3, 4> Matrix34f;
	typedef Matrix<unsigned char, 3, 1> char3;
}

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::RowVector2)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::RowVector2d)

namespace OpenMesh
{
	///表示三角网格的数据类型
	typedef TriMesh_ArrayKernelT<EigenFloatTraits> OpenTriMesh;
	///表示多边形网格的数据类型
	typedef PolyMesh_ArrayKernelT<EigenFloatTraits> OpenPolyMesh;

	//可能有bug存在
	typedef TriMesh_ArrayKernelT<EigenDoubleTraits> OpenTriMeshD;
	typedef PolyMesh_ArrayKernelT<EigenDoubleTraits> OpenPolyMeshD;
}


#if 0
#define Sn_Ensure(STATEMENT) { \
	if(!(STATEMENT)) { \
	std::cerr << "ENSURE FAILED at " \
	<< __FILE__ << ", line #" << __LINE__ << ":\n" \
	<< "    " << #STATEMENT << std::endl; \
	exit(1); \
	} \
}
#else
#define Sn_Ensure
#endif

#endif //SN3D_ALGORITHM_COMMON_TRAIST_H
