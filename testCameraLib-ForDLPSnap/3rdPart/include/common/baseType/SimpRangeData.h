//! @file SimpRangeData.h
//! @brief ���ͼ�����ݽṹ����
//! @author ������[jiayanming01@shining3d.com]
//! @date 2018/5/10
#ifndef SN3D_ALGORITHM_COMMON_SIMP_RANGDATA_H
#define SN3D_ALGORITHM_COMMON_SIMP_RANGDATA_H

#include "../globalDef.h"
#include "RangeData.h"


namespace Sn3DAlgorithm
{
	/*!
	 * @class	SimpRangeData
	 * @brief	�Ǿ��ȼ�RangeData��
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
		 * @brief	��RangeData�ƶ����� ʹ����ֵ���룺 SimpRangeData(std::move(rge))
		 * @param [in]	RangeData����
		 * @note	����rge�е������ѱ��ƶ�����ǰ�����У�������Ҫ��ʹ��rge
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/8/2.
		 */
		SimpRangeData(RangeData&& rge);

		/*!
		 * @fn	int SimpRangeData::triangulation(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<Eigen::Vector3i>& tris, int rid = 0) const;
		 * @brief	������������.
		 * @see	update_cell_width.
		 * @param [in] 	uv_map 	����uv��id��ӳ��.
		 * @param [out]	vid_map	����洢���������Ϣ���洢��ϢΪ��Ԫ��(rid, ��range�е�λ��).
		 * @param [out]	tris   	����������Ƭ�������д洢�Ķ���id��vid_map��Ԫ�ص�λ��.
		 * @param [in] 	rid	   	(Optional) ָ��rge��id, ���ڶ�Ƭ��ͬ���ǻ�����Ƭʱ����ָ��.
		 * @return	�����˳�����0.
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/5/17.
		 * @note	# �������֮ǰ���޸ģ���Ҫ�ȵ���update_cell_width.
		 * 			# �������ꡢ�������Ϣ��vid_map��ӳ�䵽ԭrge�в���.
		 */
		int triangulation(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<Eigen::Vector3i>& tris, const int rid = 0) const;

		/*!
		 * @fn	int SimpRangeData::polygonization(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<std::vector<int> >& polys, const int rid = 0) const;
		 * @brief	��srgeת��Ϊ���������
		 * @param [in]	  	uv_map 	uv������id��ӳ���.
		 * @param [in,out]	vid_map	���㵽ԭrge��ӳ�䣬��Ԫ��&lt;srge��ţ�srge�еĶ������&gt;
		 * @param [out]	  	polys  	���񻯽��������ÿ��vector�д洢�����ɶ���id����.
		 * @param [in]	  	rid	   	(Optional) ��ǰrge�ǵڼ�Ƭ�� ���ڶ�Ƭ��ͬ���ǻ�����Ƭʱ����ָ��.
		 * @return	An int.
		 * @note	vid_map�ɲ�Ϊ�գ��������Զ�Ƭsrge���ô˺����ɽ����е���뵽ͬһ��vid_map�У���Ƭ��ӵ�����ĩβ��polys�е�id��vid_map�е�ȫ��λ��
		 * @author	jiayanming[jiayanming01@shining3d.com]
		 * @date	2018/8/28
		 */
		int polygonization(const Eigen::MatrixXi& uv_map, std::vector<Eigen::Vector2i>& vid_map, std::vector<std::vector<int> >& polys, const int rid = 0) const;

		int triangulate_polygon(const std::vector<Eigen::Vector2i>& vid_map, const std::vector<int>& vid, const std::vector<int>& corner, std::vector<Eigen::Vector3i>& tris) const;
		/*!
		 * @fn	int SimpRangeData::update_cell_width(const Eigen::MatrixXi& uv_map);
		 * @brief	���¶����cell����Ϣ.
		 * @param	uv_map	uv���굽����id��ӳ��.
		 * @return	��ǰ�����ܶ�����
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/5/10.
		 * @note	��û�ж�Ӧcell�Ķ���width����Ϊ-1��cell��width����Ϊ0.
		 */
		int update_cell_width(const Eigen::MatrixXi& uv_map);


		/*!
		 * @fn	bool SimpRangeData::is_merge_cell_ok(const Eigen::MatrixXi& uv_map, const int idx) const;
		 * @brief	ָ��cell������Χ�Ƿ����merge
		 * @param	[in] uv_map	uv_map
		 * @param	[in] idx   	cell id
		 * @return	����merge����true
		 * @author	jiayanming[jiayanming01@shining3d.com]
		 * @date	2018/4/28
		 */
		bool is_merge_cell_ok(const Eigen::MatrixXi& uv_map, const int idx) const;

		/*!
		 * @fn	int SimpRangeData::merge_cell(const Eigen::MatrixXi& uv_map, int id);
		 * @brief	��cell�����ڽ���ͬ���cell�ϲ�Ϊһ��
		 * @param	[in] uv_map	uv map.
		 * @param	[in] id	  	cell id
		 * @return	��������0
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
		//��������
		inline void set_cell_width(const std::vector<char>& cellWidth) { _cellWidth = cellWidth; }
		inline void set_cell_width(std::vector<char>&& cellWidth) { _cellWidth = std::move(cellWidth); }

		inline void set_cell_root(const std::vector<int>& cellRoot) { _cellRoot = cellRoot; }
		inline void set_cell_root(std::vector<int>&& cellRoot) { _cellRoot = std::move(cellRoot); }

	private:
		//!< ��¼��Ƭ�򻯺󶥵��Ӧcell�Ŀ�ȣ�������Ϊ0��
		//!< ���⣬��Ӧcell��ɾ��/�����ڣ��Ҳ�������cell�ǵ�����Ϊ-1, �ǽǵ���Ϊ0. ����Ϊ-1ʱ���õ�Ϊ��Ч��
		std::vector<char> _cellWidth;
		std::vector<int> _cellRoot;//!< ��¼���㵽cell�����ӳ�䣬�򻯺󣬱�ɾ����cell�ǵ㱣�����cell�Ľǵ�id
	};
}//namespace Sn3DAlgorithm
#endif //SN3D_ALGORITHM_COMMON_SIMP_RANGDATA_H
