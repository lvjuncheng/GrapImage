/*********************************************
 * 全局定义，包括宏，数学常量等
 * \author 张健 
 *********************************************/


#ifndef SN3D_ALGORITHM_COMMON_GLOBAL_DEF_H
#define SN3D_ALGORITHM_COMMON_GLOBAL_DEF_H

#include <iostream>
#include "./baseType/Traits.h"

////////////////////////异常类
///人为中断异常
class SN3D_COMMON_CLASS CCancelException
{
};
///未知错误异常
class SN3D_COMMON_CLASS CUnknownException
{
};

//屏蔽warning
#pragma warning(disable:4267) //conversion from 'size_t' to 'int'
#pragma warning(disable:4996) //OpenMeshError
#pragma warning(disable:4996)

//用于debug时，输出信息
#if 1
#define Sn_Cout std::cout << __FILE__ << " " << __LINE__  << std::endl
#define Sn_Cout1(i) std::cout << __FILE__ << " " << __LINE__ << ": " << i << std::endl
#define Sn_Cout2(i, j) std::cout << __FILE__ << " " << __LINE__ << ": " << i << " " << j << std::endl
#define Sn_wCout std::wcout << __FILE__ << " " << __LINE__  << std::endl
#define Sn_wCout1(i) std::wcout << __FILE__ << " " << __LINE__ << ": " << i << std::endl
#define Sn_wCout2(i, j) std::wcout << __FILE__ << " " << __LINE__ << ": " << i << " " << j << std::endl
#else
#define Sn_Cout
#define Sn_Cout1(i)
#define Sn_Cout2(i, j)
#define Sn_wCout
#define Sn_wCout1(i)
#define Sn_wCout2(i, j)
#endif

//基本数据值
#define Sn_PI 3.141592653589793
#define Sn_PI_Celling 3.1416 ///< pi的上限，用来判断某数是否大于pi 
#define Sn_E 2.718281828459

namespace Sn3DAlgorithm
{

	const char File_Separator = '/';
}//namespace Sn3DAlgorithm

#if (defined __DARWIN__) || (defined __linux__)
	#define printf_s printf
#endif 

#ifdef _WIN32
	#define snprintf _snprintf
#endif

#ifdef _WIN32
	#define Sn_isnan(x) _isnan(x)
#else
	#define Sn_isnan(x) std::isnan(x)
#endif

#ifdef _WIN32
#define Sn_isInf(x) (!_finite(x))
#else
#define Sn_isInf(x) std::isinf(x)
#endif

///当编译器不支持OpenMP时，不使用
#ifndef _OPENMP
#define omp_get_num_procs() 1
#define omp_get_max_threads() 1
#define omp_get_thread_num() 0
inline void omp_set_num_threads(int numThreads) {}
#define omp_get_num_threads() 1
#endif

#endif //SN3D_ALGORITHM_COMMON_GLOBAL_DEF_H
