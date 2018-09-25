/***********************************************
 * 定义打印层类
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_LAYER_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_LAYER_H

#include <vector>
#include "Path.h"

namespace Sn3DAlgorithm
{
	///填充子区域的类型
	enum SN3D_PRINT3D_COMMON_API FillRegionType
	{
		FillRegionType_DEFAULT,  ///< 默认填充
		FillRegionType_OUTER_UP,  ///<上表面填充
		FillRegionType_OUTER_DOWN, ///<下表面填充
		FillRegionType_OUTER_UP_DOWN ///<既是上表面又是下表面填充
		
	};

	///属于特定区域，特征属性的填充子区域
	class SN3D_PRINT3D_COMMON_API TypeFillRegion
	{
	public:
		typedef std::shared_ptr<TypeFillRegion> Ptr;
		typedef std::shared_ptr<const TypeFillRegion> ConstPtr;

		TypeFillRegion();

		~TypeFillRegion();

		TypeFillRegion::Ptr copy();

		////////////////////////////////////////////////////////////////set and get
		int get_id() const { return _idx; }
		void set_id(int id) { _idx = id; }
		FillRegionType get_type() const { return _type; }
		void set_type(FillRegionType type) { _type = type; }

		////////////////////////////////////////////////////////////////操作_regions
		Point2Vector::ConstPtr get_region(size_t i) const {
			assert(get_regions_size() > i);
			return _regions[i];
		}
		Point2Vector::Ptr get_region(size_t i) {
			assert(get_regions_size() > i);
			return _regions[i];
		}
		size_t get_regions_size() const { return _regions.size(); }
		std::vector<Point2Vector::Ptr>& get_regions() { return _regions; }

		void set_regions(const std::vector<Point2Vector::Ptr>& regions) { _regions = regions; }
		void add_region(const Point2Vector::Ptr region) { _regions.push_back(region); }

		/** \brief TypeFillRegion导出到输出流
		*  \param[in]      os			输出流
		*  \param[in]     region		TypeFillRegion
		*/
		friend std::ostream& operator<<(std::ostream& os, const TypeFillRegion& region);

		/** \brief 从输入流导入TypeFillRegion
		*  \param[in]      is			输入流
		*  \param[out]     region		TypeFillRegion
		*/
		friend std::istream& operator>>(std::istream& is, TypeFillRegion& region);
	protected:
		std::vector<Point2Vector::Ptr> _regions;    ///< 点连线组成的轮廓集合。每一个轮廓(_regions[i])首尾点不同并默认相连
		int _idx;                                   ///<所属区域的索引. 由0开始按生成先后顺序(将boundary,extrashell,fill这三类，将他们按照各自所属的区域组织起来，_idx就是所属区域的索引)
		FillRegionType _type;

	};

	///打印中的一层结构，有多条路径组成
	class SN3D_PRINT3D_COMMON_API Layer
	{
	public:
		typedef std::shared_ptr<Layer> Ptr;
		typedef std::shared_ptr<const Layer> ConstPtr;

		/** \brief 合并多个Z值相同的层
		*  \param[in]      layers		Z值相同的层
		*  \param[out]     merged		合并后的层数据
		*  \param[in]      idx			合并后的层的索引
		*/
		static RetVal merge_layers(const std::vector<Layer::ConstPtr>& layers, Layer::Ptr merged, int idx);

		/** \brief 从输入流导入layer
		*  \param[in]      is			输入流
		*  \param[out]     layer		Layer
		*/
		SN3D_PRINT3D_COMMON_API friend std::istream& operator >> (std::istream& is, Layer& layer);

		/** \brief layer导出到输出流
		*  \param[in]      os			输出流
		*  \param[in]		layer		Layer
		*/
		SN3D_PRINT3D_COMMON_API friend std::ostream& operator << (std::ostream& os, const Layer& layer);
		/////////////////////////////////////////////////////构造函数和析构函数
		Layer();
		virtual ~Layer();

		/////////////////////////////////////////////////////set and get
		Scalar get_z() const { return _z; }
		void set_z(Scalar z) { _z = z; }
		Scalar get_thickness() const { return _thickness; }
		void set_thickness(Scalar thick) { _thickness = thick; }
		int get_id() const { return _idx; }
		void set_id(int id) { _idx = id; }

		//////////////////////////////////////////////////////操作_boundaryPaths
		Path::ConstPtr get_boundary_path(size_t i) const {
			assert(get_boundary_path_size() > i);
			return _boundaryPaths[i];
		}
		Path::Ptr get_boundary_path(size_t i) {
			assert(get_boundary_path_size() > i);
			return _boundaryPaths[i];
		}
		size_t get_boundary_path_size() const { return _boundaryPaths.size(); }
		std::vector<Path::Ptr>& get_boundary_paths() { return _boundaryPaths; }

		void set_boundary_paths(const std::vector<Path::Ptr>& paths) { _boundaryPaths = paths; }
		void add_boundary_path(const Path::Ptr path) { _boundaryPaths.push_back(path); }

		//////////////////////////////////////////////////////操作_extraShellPaths
		Path::ConstPtr get_extraShell_path(size_t i) const {
			assert(get_extraShell_path_size() > i);
			return _extraShellPaths[i];
		}
		Path::Ptr get_extraShell_path(size_t i) {
			assert(get_extraShell_path_size() > i);
			return _extraShellPaths[i];
		}
		size_t get_extraShell_path_size() const { return _extraShellPaths.size(); }
		std::vector<Path::Ptr>& get_extraShell_paths() { return _extraShellPaths; }

		void set_extraShell_paths(const std::vector<Path::Ptr>& paths) { _extraShellPaths = paths; }
		void add_extraShell_path(const Path::Ptr path) { _extraShellPaths.push_back(path); }

		Path::ConstPtr get_small_region_path(size_t i) const{
			assert(get_small_region_path_size() > i);
			return _smallRegionPaths[i];
		}

		Path::Ptr get_small_region_path(size_t i) {
			assert(get_small_region_path_size() > i);
			return _smallRegionPaths[i];
		}

		size_t get_small_region_path_size() const { return _smallRegionPaths.size(); }
		std::vector<Path::Ptr>& get_small_region_paths() { return _smallRegionPaths; }

		void set_small_region_paths(const std::vector<Path::Ptr>& paths) { _smallRegionPaths = paths; }
		void add_small_region_path(const Path::Ptr path) { _smallRegionPaths.push_back(path); }

		void set_extraShell_offsets(const std::vector<Scalar>& offsets) { _extraShellOffsets = offsets; }
		const std::vector<Scalar>& get_extraShell_offsets() const { return _extraShellOffsets; }

		//////////////////////////////////////////////////////操作_fillPaths
		Path::ConstPtr get_fill_path(size_t i) const {
			assert(get_fill_path_size() > i);
			return _fillPaths[i];
		}
		Path::Ptr get_fill_path(size_t i) {
			assert(get_fill_path_size() > i);
			return _fillPaths[i];
		}
		size_t get_fill_path_size() const { return _fillPaths.size(); }
		std::vector<Path::Ptr>& get_fill_paths() { return _fillPaths; }

		void set_fill_paths(const std::vector<Path::Ptr>& paths) { _fillPaths = paths; }
		void add_fill_path(const Path::Ptr path) { _fillPaths.push_back(path); }

		//////////////////////////////////////////////////////////////////////////操作_thinWalledFillPaths
		Path::ConstPtr get_thinWalledFill_path(size_t i) const{
			assert(get_thinWalledFill_path_size() > i);
			return _thinWalledPaths[i];
		}
		Path::Ptr get_thinWalledFill_path(size_t i) {
			assert(get_thinWalledFill_path_size() > i);
			return _thinWalledPaths[i];
		}
		size_t get_thinWalledFill_path_size() const { return _thinWalledPaths.size(); }
		std::vector<Path::Ptr>& get_thinWalledFill_paths() { return _thinWalledPaths; }

		void set_thinWalledFill_paths(const std::vector<Path::Ptr>& paths){ _thinWalledPaths = paths; }
		void add_thinWalledFill_path(const Path::Ptr path) { _thinWalledPaths.push_back(path); }

		//////////////////////////////////////////////////////////////////////////operate _sharpAnglePaths
		Path::ConstPtr get_sharpAngle_path(size_t i) const{
			assert(get_sharpAngle_path_size() > i);
			return _sharpAnglePaths[i];
		}
		Path::Ptr get_sharpAngle_path(size_t i){
			assert(get_sharpAngle_path_size() > i);
			return _sharpAnglePaths[i];
		}
		size_t get_sharpAngle_path_size() const { return _sharpAnglePaths.size(); }
		std::vector<Path::Ptr>& get_sharpAngle_paths(){ return _sharpAnglePaths; }

		void set_sharpAngle_paths(const std::vector<Path::Ptr>& paths){ _sharpAnglePaths = paths; }
		void add_sharpAngle_paths(const Path::Ptr path) { _sharpAnglePaths.push_back(path); }

		//////////////////////////////////////////////////////操作_supportPaths
		Path::ConstPtr get_support_path(size_t i) const {
			assert(get_support_path_size() > i);
			return _supportPaths[i];
		}
		Path::Ptr get_support_path(size_t i) {
			assert(get_support_path_size() > i);
			return _supportPaths[i];
		}
		size_t get_support_path_size() const { return _supportPaths.size(); }
		std::vector<Path::Ptr>& get_support_paths() { return _supportPaths; }
		void add_support_path(const Path::Ptr path) {_supportPaths.push_back(path);}
		void set_support_paths(const std::vector<Path::Ptr>& paths) { _supportPaths = paths; }
	

		//////////////////////////////////////////////////////操作_raftPaths
		Path::ConstPtr get_raft_path(size_t i) const {
			assert(get_raft_path_size() > i);
			return _raftPaths[i];
		}
		Path::Ptr get_raft_path(size_t i) {
			assert(get_raft_path_size() > i);
			return _raftPaths[i];
		}
		size_t get_raft_path_size() const { return _raftPaths.size(); }
		std::vector<Path::Ptr>& get_raft_paths() { return _raftPaths; }
		void add_raft_path(const Path::Ptr path) {_raftPaths.push_back(path);}
		void set_raft_paths(const std::vector<Path::Ptr>& paths) { _raftPaths = paths; }

		/** \brief layer清除所有路径（仅保留厚度，Z和索引）
		*/
		void clear_all_paths();

		///从layer的boundaryPath和extrashellPath中筛选出用于生成填充的“最后一圈”
		bool get_last_points(std::vector< std::vector<Point2Vector::Ptr> >& last);

		/**	\brief	对layer的所有路径划分所属区域（薄壁归属于起始点连接的轮廓所在区域）
		*	\note	轮廓的属性将被重置（z重置为layer._z）,
		*	\note	调用接口时，轮廓默认已加简化为简单轮廓（无自相交）
		*	\return true: success 
		*/
		bool number_region();

	protected:
		Scalar _z; ///<高度值
		Scalar _thickness; ///<层厚
		int _idx;          ///<索引值, 索引值从0依次增大。
		
		unsigned int _flags;
		std::vector<Path::Ptr> _boundaryPaths;  ///<轮廓路径  
		std::vector<Path::Ptr> _extraShellPaths; ///<内外轮廓的内壁 
		std::vector<Path::Ptr> _smallRegionPaths; ///<生成内壁时，退化掉的小区域的轮廓路径
		std::vector<Scalar> _extraShellOffsets; ///<内外轮廓的内壁距离上层的偏置
		std::vector<Path::Ptr> _fillPaths;       ///<填充 
		std::vector<Path::Ptr> _thinWalledPaths; ///<薄壁线
		std::vector<Path::Ptr> _sharpAnglePaths; ///<尖角线
		std::vector<Path::Ptr> _supportPaths;    ///<支撑 
		std::vector<Path::Ptr> _raftPaths;	///<基座 
	public:
		enum FLAGS{ IsRegionNumbered = 0x01 };

		void set_flags(unsigned int flags){ _flags = flags; }

		/**	\brief 是否已经对区域编号（调用了number_region）
		*/
		bool is_region_numbered(){ return _flags&IsRegionNumbered; }
	};


}//namespace Sn3DAlgorithm


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_LAYER_H
