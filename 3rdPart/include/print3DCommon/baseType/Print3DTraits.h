/***********************************************
 * 定义声明所有的底层数据结构
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_PRINT3D_TRAIST_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_PRINT3D_TRAIST_H

#ifdef _WIN32
#ifdef PRINT3D_COMMON_API_EXPORTS
#define SN3D_PRINT3D_COMMON_API __declspec(dllexport)
#else
#define SN3D_PRINT3D_COMMON_API __declspec(dllimport)
#endif
#else
#define SN3D_PRINT3D_COMMON_API
#endif

#ifdef _WIN32
#ifdef PRINT3D_COMMON_CLASS_EXPORTS
#define SN3D_PRINT3D_COMMON_CLASS __declspec(dllexport)
#else
#define SN3D_PRINT3D_COMMON_CLASS 
#endif
#else
#define SN3D_PRINT3D_COMMON_CLASS 
#endif

#include "../../common/baseType/Traits.h"

namespace Sn3DAlgorithm
{
	void SN3D_PRINT3D_COMMON_API sn_init_logger(int logLevel, const char* existDir);
}

#endif //SN3D_ALGORITHM_PRINT3D_COMMON_PRINT3D_TRAIST_H
