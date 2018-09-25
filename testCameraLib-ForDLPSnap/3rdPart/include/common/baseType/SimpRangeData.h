//! @file SimpRangeData.h
//! @brief 深度图简化数据结构定义
//! @author 贾颜铭[jiayanming01@shining3d.com]
//! @date 2018/5/10
#ifndef SN3D_ALGORITHM_COMMON_SIMP_RANGDATA_H
#define SN3D_ALGORITHM_COMMON_SIMP_RANGDATA_H

#include "../globalDef.h"
#include "RangeData.h"


namespace Sn3DAlgorithm
{
	/*!
	 * @class	SimpRangeData
	 * @brief	非均匀简化RangeData类
	 * @author	jiayanming[jiayanming01@shining3d.com]
	 * @date	2018/5/28
	 */
	class SN3D_COMMON_API SimpRangeData : public RangeData
	{
	public:
		SimpRangeData();
		SimpRangeData(const RangeData& rge);

		/*!
		 * @fn	SimpRangeData::SimpRangeData(RangeData&& rge);
		 * @brief	由RangeData移动构造 使用右值传入： SimpRangeData(std::move(rge))
		 * @param [in]	RangeData数据
		 * @note	对象rge中的数据已被移动到当前对象中，后续不要再使用rge
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/8/2.
		 */
		SimpRangeData(RangeData&& rge);

		/*!
		 * @fn	int SimpRangeData::triangulation(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<Eigen::Vector3i>& tris, int rid = 0) const;
		 * @brief	导出三角网格.
		 * @see	update_cell_width.
		 * @param [in] 	uv_map 	顶点uv到id的映射.
		 * @param [out]	vid_map	按序存储输出顶点信息，存储信息为二元组(rid, 在range中的位置).
		 * @param [out]	tris   	三角网格面片集，其中存储的顶点id是vid_map中元素的位置.
		 * @param [in] 	rid	   	(Optional) 指定rge的id, 用于多片共同三角化，单片时无需指定.
		 * @return	正常退出返回0.
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/5/17.
		 * @note	# 如果调用之前有修改，需要先调用update_cell_width.
		 * 			# 顶点坐标、法向等信息由vid_map的映射到原rge中查找.
		 */
		int triangulation(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<Eigen::Vector3i>& tris, const int rid = 0) const;

		/*!
		 * @fn	int SimpRangeData::polygonization(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<std::vector<int> >& polys, const int rid = 0) const;
		 * @brief	将srge转换为多边形网格
		 * @param [in]	  	uv_map 	uv到顶点id的映射表.
		 * @param [in,out]	vid_map	顶点到原rge的映射，二元组&lt;srge序号，srge中的顶点序号&gt;
		 * @param [out]	  	polys  	网格化结果，其中每个vector中存储新生成顶点id序列.
		 * @param [in]	  	rid	   	(Optional) 当前rge是第几片， 用于多片共同三角化，单片时无需指定.
		 * @return	An int.
		 * @note	vid_map可不为空，即连续对多片srge调用此函数可将所有点加入到同一个vid_map中，新片会加到数组末尾，polys中的id是vid_map中的全局位置
		 * @author	jiayanming[jiayanming01@shining3d.com]
		 * @date	2018/8/28
		 */
		int polygonization(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<std::vector<int> >& polys, const int rid = 0) const;

		int triangulate_polygon(const std::vector<Eigen::Vector2i>& vid_map, const std::vector<int>& vid, const std::vector<int>& corner, std::vector<Eigen::Vector3i>& tris) const;
		/*!
		 * @fn	int SimpRangeData::update_cell_width(const Eigen::MatrixXi& uv_map);
		 * @brief	更新顶点和cell的信息.
		 * @param	uv_map	uv坐标到顶点id的映射.
		 * @return	当前保留总顶点数
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/5/10.
		 * @note	将没有对应cell的顶点width设置为-1，cell点width设置为0.
		 */
		int update_cell_width(const Eigen::MatrixXi& uv_map);


		/*!
		 * @fn	bool SimpRangeData::is_merge_cell_ok(const Eigen::MatrixXi& uv_map, const int idx) const;
		 * @brief	指定cell及其周围是否可以merge
		 * @param	[in] uv_map	uv_map
		 * @param	[in] idx   	cell id
		 * @return	可以merge返回true
		 * @author	jiayanming[jiayanming01@shining3d.com]
		 * @date	2018/4/28
		 */
		bool is_merge_cell_ok(const Eigen::MatrixXi& uv_map, const int idx) const;

		/*!
		 * @fn	int SimpRangeData::merge_cell(const Eigen::MatrixXi& uv_map, int id);
		 * @brief	将cell和其邻近的同宽度cell合并为一个
		 * @param	[in] uv_map	uv map.
		 * @param	[in] id	  	cell id
		 * @return	正常返回0
		 * @author	jiayanming[jiayanming01@shining3d.com]
		 * @date	2018/5/28
		 */
		int merge_cell(const Eigen::MatrixXi& uv_map, int id);

		int get_cell_points(const Eigen::MatrixXi& uv_map, const int idx, std::vector<int>& adj);

		int set_cell_width(int idx, int width);
		char get_cell_width(int idx) const { return _cellWidth[idx]; }
		int get_cell_root(int idx) const;

		int write_to_obj(const std::string& filename, bool write_normal = true) const;

	public:
		inline const std::vector<char>& get_cell_width() const { return _cellWidth; }
		inline const std::vector<int>& get_cell_root() const { return _cellRoot; }
		//数据设置
		inline void set_cell_width(const std::vector<char>& cellWidth) { _cellWidth = cellWidth; }
		inline void set_cell_width(std::vector<char>&& cellWidth) { _cellWidth = std::move(cellWidth); }

		inline void set_cell_root(const std::vector<int>& cellRoot) { _cellRoot = cellRoot; }
		inline void set_cell_root(std::vector<int>&& cellRoot) { _cellRoot = std::move(cellRoot); }

	private:
		//!< 记录单片简化后顶点对应cell的宽度，孤立点为0。
		//!< 此外，对应cell被删除/不存在，且不是其它cell角点则宽度为-1, 是角点则为0. 即不为-1时，该点为有效点
		std::vector<char> _cellWidth;
		std::vector<int> _cellRoot;//!< 记录顶点到cell根点的映射，简化后，被删除的cell角点保存其根cell的角点id
	};
}//namespace Sn3DAlgorithm
#endif //SN3D_ALGORITHM_COMMON_SIMP_RANGDATA_H
