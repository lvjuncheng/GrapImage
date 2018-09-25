/***********************************************
* 定义3MF的读写函数
* \author 邱凯佳
***********************************************/
#ifndef SN3D_ALGORITHM_COMMON_3MF_IO_H
#define SN3D_ALGORITHM_COMMON_3MF_IO_H

#include "baseType/BaseTriMesh.h"

namespace Sn3DAlgorithm
{
	int read_3mf_mesh_impl(const std::string& meshFileName, BaseTriMesh& triMesh, bool withColor, bool withTexture);

	int write_3mf_mesh_impl(const std::string& meshFileName, BaseTriMesh& triMesh, bool withColor, bool withTexture);
}

#endif