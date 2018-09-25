/***************************************************
 * 为了方便调用，定义vtk输出的高级函数
 * \author 张健
 ***************************************************/



#ifndef SN3D_ALGORITHM_COMMON_VTK_EXT_H
#define SN3D_ALGORITHM_COMMON_VTK_EXT_H

#include "globalDef.h"
#include "vtk.h" 
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include "printError.h"

namespace Sn3DAlgorithm
{
	/** \brief 用点的类型，生成vtk文件
	 *  \param[in] fileName vtk文件的名字
	 *  \param[in] points   vtk文件中点的坐标
	 */
	template<typename T>
	void vtk_show_point_cloud(const std::string& fileName, const std::vector<Eigen::Matrix<T, 3, 1> >& points)
	{
		std::vector<T> node;
		std::vector<size_t> id;
		for(size_t i = 0; i < points.size(); ++i)
		{
			node.push_back(points[i][0]);
			node.push_back(points[i][1]);
			node.push_back(points[i][2]);
			id.push_back(i);
		}

		std::ofstream fout(fileName.c_str());
		if(!fout.is_open())
		{
			Sn_Error(fileName+" can not open");
			return;
		}
		point2vtk(fout, &(node[0]), id.size(), &(id[0]), id.size());
	}

	/** \brief 用线的类型，生成vtk文件
	 *  \param[in] fileName vtk文件的名字
	 *  \param[in] points   vtk文件中线的端点坐标，排列顺序(第一线首，第一线尾， 第二线首， 第二线尾)
	 */
	template<typename T>
	void vtk_show_lines(const std::string& fileName, const std::vector<Eigen::Matrix<T, 3, 1> >& points)
	{
		std::vector<T> node;
		std::vector<size_t> id;
		for(size_t i = 0; i < points.size(); ++i)
		{
			node.push_back(points[i][0]);
			node.push_back(points[i][1]);
			node.push_back(points[i][2]);
			id.push_back(i);
		}
		std::ofstream fout(fileName.c_str());
		if(!fout.is_open())
		{
			Sn_Error(fileName + " can not open");
			return;
		}
		line2vtk(fout, &(node[0]), id.size(), &(id[0]), id.size()/2);
	}

}//namespace





#endif //SN3D_ALGORITHM_COMMON_VTK_EXT_H