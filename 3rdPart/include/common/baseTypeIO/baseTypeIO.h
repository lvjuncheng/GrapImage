/***********************************************
 * 定义一些基本数据类型的读写函数
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_BASE_TYPE_IO_H
#define SN3D_ALGORITHM_COMMON_BASE_TYPE_IO_H

//STL
#include <string>
//3RD
#include <OpenMesh/Core/IO/Options.hh>
//SN3D
#include "../baseType/BaseTriMesh.h"
#include "../baseType/TriMesh.h"
#include "../baseType/RangeData.h"
#include "../baseType/Image.h"
#include "../baseType/PointImage.h"
#include "../baseType/RangeImage.h"

namespace Sn3DAlgorithm
{
	///将RangeImage写成mesh文件
	int SN3D_COMMON_API write_mesh(const std::string& fileName, const RangeData& range, bool isBinary = true);

	///将asc读入cloud中
	int SN3D_COMMON_API read_asc(const std::string& fileName, Cloud& cloud, bool hasColor, bool hasNormal);

	///读入pointImage
	int SN3D_COMMON_API read_pointimage(const std::string& fileName , PointImage&oneFrame);


	///将cloud写入asc文件中
	int SN3D_COMMON_API write_asc(const std::string& fileName, const Cloud& cloud);

	/**  \brief 将pointImage写入asc文件中
	*   \param[out] isCompact                            是否将这张图中的无效点（0点）也输出
	*/
	int SN3D_COMMON_API write_asc(const std::string& fileName, const PointImage& cloud , const bool isCompact);

	///UNDO:颜色信息不能读出
	int SN3D_COMMON_API read_mesh(const std::string& fileName, BaseTriMesh& mesh);
	
	///UNDO:颜色信息不能写入
	int SN3D_COMMON_API write_mesh(const std::string& fileName, const BaseTriMesh& mesh, bool isBinary = true);

	///UNDO:暂时只支持颜色或纹理
	int SN3D_COMMON_API read_3mf_mesh(const std::string& meshFileName, BaseTriMesh& triMesh, bool withColor, bool withTexture);

	///UNDO:暂时只支持颜色或纹理，同时存在写入纹理
	int SN3D_COMMON_API write_3mf_mesh(const std::string& meshFileName, BaseTriMesh& triMesh, bool withColor, bool withTexture);

	//! @{
	//! @name TriMesh 读写

	/*!
	 * @brief	将模型文件读取到TriMesh.
	 * @param [in] 	fileName	文件全名.
	 * @param [out]	mesh		待读入网格.
	 * @return	成功返回0.
	 */
	int SN3D_COMMON_API read_mesh(const std::string& fileName, TriMesh& mesh);

	/*!
	 * @brief	将TriMesh写出到文件.
	 * @param [in]	fileName 	文件全名.
	 * @param [in]	mesh	 	待写出网格.
	 * @param [in]	opt		 	(Optional) 写出选项，指定二进制、顶点颜色等，见OpenMesh.
	 * @param [in]	precision	(Optional) ASCII写出时，浮点数精度.
	 * @return	成功返回0.
	 * @note 如果stl需要写出二进制 opt传入OpenMesh::IO::Options::Binary. obj写出不支持二进制
	 * 		 写出stl文件不支持Options::VertexNormal
	 */
	int SN3D_COMMON_API write_mesh(const std::string& fileName, const TriMesh& mesh, OpenMesh::IO::Options opt = OpenMesh::IO::Options::Default, std::streamsize precision = 6);

	//! @}

	/*!
	 * @brief   	读取 rgb 图像
	 * @param[in]	filename	文件名
	 * @param[out]	image		存储读取的图像数据
	 * @return		0-->正常
	 *				1-->失败
	 * @date    	2015-08
	 */
	int SN3D_COMMON_API read_rgb_image(const std::string& fileName, ImageRGBb& image);

	///读取灰度图像
	int SN3D_COMMON_API read_grey_image(const std::string& fileName, ImageGreyb& image);

	/*!
	 * @brief   	保存 rgb 图像
	 * @param[in]	filename	文件名
	 * @param[in]	image		待写的图像数据
	 * @return		0-->正常
	 *				1-->失败
	 * @date    	2015-08
	 */
	int SN3D_COMMON_API write_rgb_image(const std::string& fileName, const ImageRGBb& image);

	///保存灰度图像
	int SN3D_COMMON_API write_grey_image(const std::string& fileName, const ImageGreyb& image);

	///读取带纹理的obj模型。obj文件只允许存在一个mtllib和一个usemtl
	/** \brief 读取带纹理的obj模型。obj文件只允许存在一个mtllib和一个usemtl
	 *  \return 0 正确。1 obj文件有错误。2 纹理有问题
	 */
	int SN3D_COMMON_API read_obj_with_texture(const std::string& fileName, BaseTriMesh& mesh);

	///生成带纹理的obj模型
	int SN3D_COMMON_API write_obj_with_texture(const std::string& fileName, const BaseTriMesh& mesh);

	int SN3D_COMMON_API read_RigidMatrix(const std::string& fileName, RigidMatrix& mat);
	int SN3D_COMMON_API write_RigidMatrix(const std::string& fileName, const RigidMatrix& mat);

	int SN3D_COMMON_API read_point_image_to_rangeImage(const std::string& fileName, RangeImage& image, int height, int width, bool hasColor, bool hasNormal);

	int SN3D_COMMON_API write_asc(const std::string& fileName, const RangeImage& image);

	int SN3D_COMMON_API read_rangeImage(const std::string& fileName, RangeImage& image);
	///后缀名为rig
	int SN3D_COMMON_API write_rangeImage(const std::string& fileName, const RangeImage& image);


	/////////////////////////////////////////////////读写工具

	///stl文件是否是二进制的
	RetVal SN3D_COMMON_API stl_is_binary(const std::string& fileName, bool& isBinary);

} //namespace

#endif //SN3D_ALGORITHM_COMMON_BASE_TYPE_IO_H