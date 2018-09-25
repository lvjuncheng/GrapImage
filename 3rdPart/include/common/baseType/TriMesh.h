/***********************************************
 * 使用半边结构实现三角网格类
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_TRIMESH_H
#define SN3D_ALGORITHM_COMMON_TRIMESH_H

#include "Traits.h"
#include "BaseData.h"


namespace Sn3DAlgorithm
{
	///使用半边结构实现三角网格类。完全继承自OpenMesh
	class SN3D_COMMON_API TriMesh : public BaseData, public OpenMesh::OpenTriMesh
	{
	public:
		typedef TriMesh::Vertex			VertexType;
		typedef TriMesh::Face			FaceType;
		typedef	TriMesh::Scalar			ScalarType;
		typedef TriMesh::VertexHandle	VertexIterator;
		typedef TriMesh::FaceHandle		FaceIterator;
		typedef Eigen::Vector3			Point3x;

		TriMesh();
		virtual ~TriMesh();

		/////////////////////////////////////////////////////////////////硬盘与内存数据交换
		///把数据保存在硬盘
		virtual int swap_out_data();
		///把数据从硬盘读进来
		virtual int swap_in_data();

		///////////////////////////////////////////////////////////////////内存管理
		///清空内存
		virtual void clear();
		///把删除的数据空间释放出来
		virtual void squeeze();

		///获取模型包围盒
		virtual void get_boundBox(Eigen::Vector3& boxMin, Eigen::Vector3& boxMax) const;

		/*!
		 * @brief   	反转法向	- 包括： 面片内顶点顺序、面片法向、顶点法向
		 * @author  	yangxl [yangxuelian@shining3d.com]
		 *
		 * @warning		此接口实际实现中需要重新生成一个 TriMesh 对象，转换数据，操作耗时，不建议调用
		 *
		 * @date    	2015-05
		 */
		void reverse_norm();

		///验证数据是否合法，UNDO：只检查点的坐标和面的信息
		virtual bool is_legal() const;

		///修补不合法数据. UNDO:
		virtual void repair_illegal();
	};

	////////////////////////////////////operator
	class BaseTriMesh;
	class RangeData;
	class SimpRangeData;
	/*!
	 * @brief		TriMesh转化为baseMesh
	 * @param [in]	triMesh			待转换的半边结构网格
	 * @param [out]	baseMesh		转换后的基本三角面片网格
	 * @note		TriMesh中的属性VertexStatus,FaceStatus, FaceNormal, VertexNormal, VertexColor可转化
	 * @date    	2015-05
	 */
	void SN3D_COMMON_API TriMesh_convert_to_BaseTriMesh(const TriMesh& triMesh, BaseTriMesh& baseMesh);

	/*!
	 * @brief		BaseMesh 转化为 TriMesh
	 * @param [in]	baseMesh		待转换的基本三角面片网格
	 * @param [out]	triMesh			转换后的半边结构网格
	 * @date    	2015-05
	 */
	void SN3D_COMMON_API BaseTriMesh_convert_to_TriMesh(const BaseTriMesh& baseMesh, TriMesh& triMesh);

	/*!
	 * @brief   	多个 TriMesh 合并成一个 TriMesh
	 * @param[in]	sub_mesh_arr	待合并模型
	 * @param[out]	joined_mesh		合并的模型
	 * @return		
	 * @author  	yangxl [yangxuelian@shining3d.com]
	 * @date    	2015-08
	 */
	RetVal SN3D_COMMON_API join_TriMesh(const std::vector<TriMesh>& sub_mesh_arr, TriMesh& joined_mesh);

	/*!
	 * @brief   	多个 BaseTriMesh 合并成一个 BaseTriMesh
	 * @param[in]	sub_mesh_arr	待合并模型
	 * @param[out]	joined_mesh		合并的模型
	 * @return		
	 * @author  	yangxl [yangxuelian@shining3d.com]
	 * @date    	2015-08
	 */
	RetVal SN3D_COMMON_API join_BaseTriMesh(const std::vector<BaseTriMesh>& sub_mesh_arr, BaseTriMesh& joined_mesh);

	/*!
	 * @brief	RangeData 转换为TriMesh.
	 * @param [in]	range  	输入待转换RangeData.
	 * @param [out]	triMesh	输出TriMesh结果.
	 * @note 顶点属性只增加了法向
	 */
	void SN3D_COMMON_API RangeData_convert_to_TriMesh(const RangeData& range, TriMesh& triMesh);
	void SN3D_COMMON_API SimpRangeData_convert_to_TriMesh(const SimpRangeData& range, TriMesh& triMesh);
	/*!
	 * @fn	void SN3D_COMMON_API RangeData_convert_to_BaseTriMesh(const RangeData& range, BaseTriMesh& baseMesh);
	 * @brief	Rangedata 转换为 BaseTriMesh.
	 * @param [in] 	range   	输入待转换RangeData.
	 * @param [out]	baseMesh	输出BaseTriMesh结果.
	 * @note 未处理法向
	 */
	void SN3D_COMMON_API RangeData_convert_to_BaseTriMesh(const RangeData& range, BaseTriMesh& baseMesh);
}//namespace


#endif //SN3D_ALGORITHM_COMMON_TRIMESH_H