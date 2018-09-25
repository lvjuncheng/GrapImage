/***********************************************
 * �����ӡ����
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_LAYER_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_LAYER_H

#include <vector>
#include "Path.h"

namespace Sn3DAlgorithm
{
	///��������������
	enum SN3D_PRINT3D_COMMON_API FillRegionType
	{
		FillRegionType_DEFAULT,  ///< Ĭ�����
		FillRegionType_OUTER_UP,  ///<�ϱ������
		FillRegionType_OUTER_DOWN, ///<�±������
		FillRegionType_OUTER_UP_DOWN ///<�����ϱ��������±������
		
	};

	///�����ض������������Ե����������
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

		////////////////////////////////////////////////////////////////����_regions
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

		/** \brief TypeFillRegion�����������
		*  \param[in]      os			�����
		*  \param[in]     region		TypeFillRegion
		*/
		friend std::ostream& operator<<(std::ostream& os, const TypeFillRegion& region);

		/** \brief ������������TypeFillRegion
		*  \param[in]      is			������
		*  \param[out]     region		TypeFillRegion
		*/
		friend std::istream& operator>>(std::istream& is, TypeFillRegion& region);
	protected:
		std::vector<Point2Vector::Ptr> _regions;    ///< ��������ɵ��������ϡ�ÿһ������(_regions[i])��β�㲻ͬ��Ĭ������
		int _idx;                                   ///<�������������. ��0��ʼ�������Ⱥ�˳��(��boundary,extrashell,fill�����࣬�����ǰ��ո���������������֯������_idx�����������������)
		FillRegionType _type;

	};

	///��ӡ�е�һ��ṹ���ж���·�����
	class SN3D_PRINT3D_COMMON_API Layer
	{
	public:
		typedef std::shared_ptr<Layer> Ptr;
		typedef std::shared_ptr<const Layer> ConstPtr;

		/** \brief �ϲ����Zֵ��ͬ�Ĳ�
		*  \param[in]      layers		Zֵ��ͬ�Ĳ�
		*  \param[out]     merged		�ϲ���Ĳ�����
		*  \param[in]      idx			�ϲ���Ĳ������
		*/
		static RetVal merge_layers(const std::vector<Layer::ConstPtr>& layers, Layer::Ptr merged, int idx);

		/** \brief ������������layer
		*  \param[in]      is			������
		*  \param[out]     layer		Layer
		*/
		SN3D_PRINT3D_COMMON_API friend std::istream& operator >> (std::istream& is, Layer& layer);

		/** \brief layer�����������
		*  \param[in]      os			�����
		*  \param[in]		layer		Layer
		*/
		SN3D_PRINT3D_COMMON_API friend std::ostream& operator << (std::ostream& os, const Layer& layer);
		/////////////////////////////////////////////////////���캯������������
		Layer();
		virtual ~Layer();

		/////////////////////////////////////////////////////set and get
		Scalar get_z() const { return _z; }
		void set_z(Scalar z) { _z = z; }
		Scalar get_thickness() const { return _thickness; }
		void set_thickness(Scalar thick) { _thickness = thick; }
		int get_id() const { return _idx; }
		void set_id(int id) { _idx = id; }

		//////////////////////////////////////////////////////����_boundaryPaths
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

		//////////////////////////////////////////////////////����_extraShellPaths
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

		//////////////////////////////////////////////////////����_fillPaths
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

		//////////////////////////////////////////////////////////////////////////����_thinWalledFillPaths
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

		//////////////////////////////////////////////////////����_supportPaths
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
	

		//////////////////////////////////////////////////////����_raftPaths
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

		/** \brief layer�������·������������ȣ�Z��������
		*/
		void clear_all_paths();

		///��layer��boundaryPath��extrashellPath��ɸѡ�������������ġ����һȦ��
		bool get_last_points(std::vector< std::vector<Point2Vector::Ptr> >& last);

		/**	\brief	��layer������·�������������򣨱��ڹ�������ʼ�����ӵ�������������
		*	\note	���������Խ������ã�z����Ϊlayer._z��,
		*	\note	���ýӿ�ʱ������Ĭ���ѼӼ�Ϊ�������������ཻ��
		*	\return true: success 
		*/
		bool number_region();

	protected:
		Scalar _z; ///<�߶�ֵ
		Scalar _thickness; ///<���
		int _idx;          ///<����ֵ, ����ֵ��0��������
		
		unsigned int _flags;
		std::vector<Path::Ptr> _boundaryPaths;  ///<����·��  
		std::vector<Path::Ptr> _extraShellPaths; ///<�����������ڱ� 
		std::vector<Path::Ptr> _smallRegionPaths; ///<�����ڱ�ʱ���˻�����С���������·��
		std::vector<Scalar> _extraShellOffsets; ///<�����������ڱھ����ϲ��ƫ��
		std::vector<Path::Ptr> _fillPaths;       ///<��� 
		std::vector<Path::Ptr> _thinWalledPaths; ///<������
		std::vector<Path::Ptr> _sharpAnglePaths; ///<�����
		std::vector<Path::Ptr> _supportPaths;    ///<֧�� 
		std::vector<Path::Ptr> _raftPaths;	///<���� 
	public:
		enum FLAGS{ IsRegionNumbered = 0x01 };

		void set_flags(unsigned int flags){ _flags = flags; }

		/**	\brief �Ƿ��Ѿ��������ţ�������number_region��
		*/
		bool is_region_numbered(){ return _flags&IsRegionNumbered; }
	};


}//namespace Sn3DAlgorithm


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_LAYER_H
