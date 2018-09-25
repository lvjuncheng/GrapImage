/***********************************************
 * 基本数据类型输入输出
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_BASETYPE_IO_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_BASETYPE_IO_H


#include <vector>
#include <string>
#include "../baseType/Print3DTraits.h"
#include "../baseType/Entity.h"
#include "../baseType/Layer.h"
#include "../../common/baseType/BaseTriMesh.h"

namespace Sn3DAlgorithm
{

	///定义工艺结构类型
	enum SN3D_PRINT3D_COMMON_API StructureType
	{
		StructureType_BOUNDARY, ///<轮廓
		StructureType_EXTRASHELL, ///<内壁
		StructureType_FILL, ///<填充
		StructureType_SUPPORT, ///<支撑
		StructureType_RAFT, ///<基座
	};


	/**	\brief 将切片数据以G代码格式输出
	*	\param[in]		gcodeFileName			保存路径
	*	\param[in]		entity					切片实体
	*	\param[in]		decimalPlaces			保留小数点后位数
	*	\param[in]		structSequence			指明各个工艺结构的输出顺序
	*	\param[in]		assignColour			是否指定颜色：外轮廓(black), 内轮廓(red), 内壁(blue), 填充(green), 支撑(orange), 支撑包围(yellow), 否则随机颜色显示
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_gcode(
		const std::string& gcodeFileName,
		const Entity& entity,
		const size_t decimalPlaces,
		const std::vector<StructureType>& structSequence, 
		const bool assignColour);


	/** \brief 将切片数据以G代码格式输出
	*  \param[in]     gcodeFileName		保存路径
	*  \param[in]     Layer						一层数据
	*  \param[in]     decimalPlaces			保留小数点后位数
	*  \param[in]     structSequence       指明各个工艺结构的输出顺序
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_gcode(
		const std::string& gcodeFileName,
		const Layer& layer,
		const size_t decimalPlaces,
		const std::vector<StructureType>& structSequence);


	/** \brief 通用文件保存
	*  \param[in]     fileName				文件路径
	*  \param[in]     text						文件内容
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_common_file(
		const std::string& fileName,
		const std::vector<std::string>& text);

	/** \brief 通用文件保存
	*  \param[in]     fileName				文件路径
	*  \param[in]     text						文件内容
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_common_file(
		const std::string& fileName,
		const std::string& text);

	/** \brief 将带颜色的entity转变成带颜色的BaseTriMesh。方便查看颜色的连贯性
	 *  \param[in] extraShellWidth  extraShell用于显示的线宽
	 *  \param[in] fillWidth        填充用于显示的线宽
	 */
	RetVal SN3D_PRINT3D_COMMON_API convert_entity_to_baseTriMesh_with_color(const Entity& entity, BaseTriMesh& mesh, 
		float extraShellWidth, float fillWidth);

	/** \brief 从文件名读入三角网格模型存入BaseTriMesh
	*	\param[in]		meshFileName		文件名（从扩展名获取类型）
	*	\param[out]		triMesh				以BaseTriMesh格式存储的三角网格
	*	\param[in]		deflection			stp模型三角化的误差，只有读取stp模型才需要指定
	*	\note: 目前只支持stl，obj（忽略曲面、曲线、纹理等）,3mf, stp
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh(
		const std::string& meshFileName,
		BaseTriMesh& triMesh,
		Scalar deflectionCoeff = 0.004
		);

	/** \brief 从文件名读入三角网格模型存入BaseTriMesh
	*	\param[in]		meshFileName		文件名（从扩展名获取类型）
	*	\param[out]		triMesh				以BaseTriMesh格式存储的三角网格
	*	\param[in]		processCallBack		进度条回调函数
	*	\note: 目前只支持stl，obj（忽略曲面、曲线、纹理等）,3mf, stp
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh(
		const std::string& meshFileName,
		BaseTriMesh& triMesh,
		std::function<void(const int&)> processCallBack
		);


	/** \brief 从文件名读入三角网格模型存入BaseTriMesh
	*	\param[in]		meshFileName		文件名
	*	\param[out]		triMesh				以BaseTriMesh格式存储的三角网格
	*	\param[in]		ext					指定文件扩展名，不从文件名扩展获取类型
	*	\param[in]		deflection			stp模型三角化的误差，只有读取stp模型才需要指定
	*	\note: 目前只支持stl，obj（忽略曲面、曲线、纹理等）,3mf, stp
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh(
		const std::string& meshFileName,
		BaseTriMesh& triMesh,
		const std::string& ext,
		Scalar deflectionCoeff = 0.004,
		std::function<void(const int&)> callback = nullptr
		);


	/** \brief 从内存数据读入三角网格模型存入BaseTriMesh
	*  \param[in]     stl_data		内存数据
	*	\praram[in]		ext			数据类型，支持stl,obj
	*  \param[out]   triMesh					以BaseTriMesh格式存储的三角网格
	*  \note: 目前只支持stl，obj（忽略曲面、曲线、纹理等）
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh_in_memory(
		const std::vector<unsigned char>& mesh_data,
		const std::string& ext,
		BaseTriMesh& triMesh
		);
	
	/** \brief 将三角网格写入文件（保留任何数据）
	*  \param[in]     meshFileName		文件名
	*  \param[in]	  triMesh					以BaseTriMesh格式存储的三角网格
	*  \UNDO:目前只支持binary-stl，后面会增加assic-stl，obj
	*/
	RetVal SN3D_PRINT3D_COMMON_API write_TriMesh(
		const std::string& meshFileName,
		const BaseTriMesh& triMesh
		);

	/** \brief 将三角网格写入内存缓冲区
	*  \param[in]     meshFileName		文件名
	*  \param[in]	  triMesh					以BaseTriMesh格式存储的三角网格
	*  \UNDO:目前只支持binary-stl，后面会增加assic-stl，obj
	*/
	RetVal SN3D_PRINT3D_COMMON_API write_TriMesh_to_memory(
		std::vector<unsigned char>& memBuf,
		const BaseTriMesh& triMesh
		);
}
#endif //SN3D_ALGORITHM_PRINT3D_COMMON_BASETYPE_IO_H
