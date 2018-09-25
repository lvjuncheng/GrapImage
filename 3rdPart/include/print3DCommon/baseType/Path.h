/***********************************************
 * 定义路径类
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_PATH_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_PATH_H

#include <vector>
#include "Print3DTraits.h"
#include "./PointType.h"
#include <memory>

namespace Sn3DAlgorithm
{
	///枚举路径的几种类型
	enum SN3D_PRINT3D_COMMON_API PathType
	{
		PathType_DEFAULT = 0,

		//////////////////////////////////////////////////////////////////////////轮廓类型

		PathType_BOUNDARY_OUTER, ///<外轮廓（逆时针）
		PathType_BOUNDARY_INNER, ///<内轮廓（顺时针）

		//////////////////////////////////////////////////////////////////////////填充类型

		PathType_FILL_DISCONTINUE_OUTER_UP,	///<不连续线段填充，上表面，生成方式为将点集中每两个邻接点生成独立的线段路径，即[0, 1] [2, 3] [4, 5]....
		PathType_FILL_DISCONTINUE_OUTER_DOWN,	///<不连续线段填充，下表面，生成方式为将点集中每两个邻接点生成独立的线段路径，即[0, 1] [2, 3] [4, 5]....
		PathType_FILL_DISCONTINUE_OUTER_UP_DOWN,	///<不连续线段填充，既是上表面又是下表面，生成方式为将点集中每两个邻接点生成独立的线段路径，即[0, 1] [2, 3] [4, 5]....
		PathType_FILL_DISCONTINUE_DEFAULT,	///<不连续线段填充，默认填充，生成方式为将点集中每两个邻接点生成独立的线段路径，即[0, 1] [2, 3] [4, 5]....

		PathType_FILL_CONTINUE_OUTER_UP,		///<连续线段填充--上表面，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1, 2, 3....]
		PathType_FILL_CONTINUE_OUTER_DOWN,	///<连续线段填充--下表面，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1, 2, 3....]
		PathType_FILL_CONTINUE_OUTER_UP_DOWN,	///<连续线段填充--既是上表面又是下表面，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1, 2, 3....]
		PathType_FILL_CONTINUE_DEFAULT,		///<连续线段填充--默认填充，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1, 2, 3....]

		PathType_FILL_BOUNDARY_OUTER_UP,		///<填充线轮廓，上表面
		PathType_FILL_BOUNDARY_OUTER_DOWN,	///<填充线轮廓--下表面
		PathType_FILL_BOUNDARY_OUTER_UP_DOWN,	///<填充线轮廓---既是上表面又是下表面
		PathType_FILL_BOUNDARY_DEFAULT,		///<填充线轮廓--默认填充

		PathType_SMALL_REGION_EXTRA_SHELL,	///<生成内壁时多出的小区域轮廓
		PathType_SMALL_REGION_FILLING,	///<生成填充区时多出的小区域轮廓

		PathType_SMALL_REGION_EXTRA_SHELL_FILL_CONTINUE,	///<连续线段填充--生成内壁多出的小区域，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1, 2, 3....]
		PathType_SMALL_REGION_EXTRA_SHELL_FILL_DISCONTINUE,	///<不连续线段填充--生成内壁多出的小区域，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1] [2, 3] [4, 5]....
		PathType_SMALL_REGION_FILLING_FILL_CONTINUE,	///<连续线段填充--生成填充区多出的小区域，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1, 2, 3....]
		PathType_SMALL_REGION_FILLING_FILL_DISCONTINUE,///<不连续线段填充--生成填充区多出的小区域，生成方式为将点集中的所有点生成顺次连接的路径，即[0, 1] [2, 3] [4, 5]....
		//////////////////////////////////////////////////////////////////////////支撑类型

		PathType_TOP_SUPPORT, ///<顶层支撑
		PathType_MID_SUPPORT, ///<中间支撑
		PathType_BASE_SUPPORT, ///<基本支撑
		PathType_BASE_SUPPORT_SURROUNDING, ///<基本支撑包围结构

		//////////////////////////////////////////////////////////////////////////基座类型

		PathType_RAFT_BASE_SURROUNDING, ///<基础基座的包围结构
		PathType_RAFT_BASE, ///<基础基座
		PathType_RAFT_FIRST_INTERFACE, ///<第一层接触基座
		PathType_RAFT_SECOND_INTERFACE, ///<第二层接触基座

		//////////////////////////////////////////////////////////////////////////尖角与薄壁
		PathType_THINWALL,
		PathType_SHARPANGLE
	};

	class Point2Vector : public std::vector<Point2>
	{
	public:
		typedef std::shared_ptr<Point2Vector> Ptr;
		typedef std::shared_ptr<const Point2Vector> ConstPtr;

	};

	SN3D_PRINT3D_COMMON_API std::ostream&  operator << (std::ostream& os, const Point2Vector& p2Vec);

	SN3D_PRINT3D_COMMON_API std::istream&  operator >> (std::istream& is, Point2Vector& p2Vec);

	///颜色数组
	class ColorVector: public std::vector<Color>
	{
	public:
		typedef std::shared_ptr<ColorVector> Ptr;
		typedef std::shared_ptr<const ColorVector> ConstPtr;
	};

	/** \brief 计算轮廓包围盒
	*	\param[out]	boundarys			轮廓
	*	\param[in]	minX				x最小值
	*	\param[in]	minY				y最小值
	*	\param[in]	maxX				x最大值
	*	\param[in]	maxY				y最大值
	*/
	void SN3D_PRINT3D_COMMON_API get_boundarys_bbox(const std::vector<Point2Vector::ConstPtr>& boundarys, Scalar& minX, Scalar& minY, Scalar& maxX, Scalar& maxY);
	void SN3D_PRINT3D_COMMON_API get_boundary_bbox(Point2Vector::ConstPtr boundarys, Scalar& minX, Scalar& minY, Scalar& maxX, Scalar& maxY);

	void SN3D_PRINT3D_COMMON_API translate_boundarys(std::vector<Point2Vector::Ptr>& boundarys, const Point2& trans);
	void SN3D_PRINT3D_COMMON_API translate_boundary(Point2Vector::Ptr boundarys, const Point2& trans);

	void SN3D_PRINT3D_COMMON_API rotate_boundarys(std::vector<Point2Vector::Ptr>& boundarys, const Point2& center, Scalar rotDeg);
	void SN3D_PRINT3D_COMMON_API rotate_boundary(Point2Vector::Ptr boundarys, const Point2& center, Scalar rotDeg);
	
	///将Ptr转为ConstPtr
	std::vector<Point2Vector::ConstPtr> SN3D_PRINT3D_COMMON_API ptr_vector_to_const_ptr_vector( 
		const std::vector<Point2Vector::Ptr>& src);
	void SN3D_PRINT3D_COMMON_API ptr_vector_to_const_ptr_vector(
		const std::vector<Point2Vector::Ptr>& src, std::vector<Point2Vector::ConstPtr>& output);

	std::vector<ColorVector::ConstPtr> SN3D_PRINT3D_COMMON_API ptr_vector_to_const_ptr_vector(
		const std::vector<ColorVector::Ptr>& src);
	void SN3D_PRINT3D_COMMON_API ptr_vector_to_const_ptr_vector(
		const std::vector<ColorVector::Ptr>& src, std::vector<ColorVector::ConstPtr>& output);


	class SN3D_PRINT3D_COMMON_API ExtraShellComponent
	{
	public:
		typedef std::shared_ptr<ExtraShellComponent> Ptr;
		typedef std::shared_ptr<const ExtraShellComponent> ConstPtr;

		ExtraShellComponent() : _extraShellIdx(-1), _isDegenerateSmallRegion(false)
		{}
		int get_extraShell_id() const { return _extraShellIdx; }
		void set_extraShell_id(int id) { _extraShellIdx = id; }
		bool is_small_region() const { return _isDegenerateSmallRegion; }
		void set_small_region(bool isSmallRegion) { _isDegenerateSmallRegion = isSmallRegion; }
	protected:
		int _extraShellIdx; ///<多层内壁的层次索引, 从0开始，按生成先后顺序
		bool _isDegenerateSmallRegion;	///<生成内壁副产物，表示退化的区域所在的原轮廓路径
	};

	///带拖尾的轮廓类型
	class SN3D_PRINT3D_COMMON_API ExtendPathComponent
	{
	public:
		typedef std::shared_ptr<ExtendPathComponent> Ptr;
		typedef std::shared_ptr<const ExtendPathComponent> ConstPtr;

		ExtendPathComponent() { }

		Point2Vector::ConstPtr get_tails() const { return _tails; }
		Point2Vector::Ptr get_tails() { return _tails; }
		const Point2& get_tail(size_t id) const {
			assert(_tails);
			return _tails->at(id);
		}
		Point2& get_tail(size_t id) {
			assert(_tails);
			return _tails->at(id);
		}

		void set_tails(Point2Vector::Ptr points) { _tails = points; }


		Point2Vector::ConstPtr get_heads() const { return _heads; }
		Point2Vector::Ptr get_heads() { return _heads; }
		const Point2& get_head(size_t id) const {
			assert(_heads);
			return _heads->at(id);
		}
		Point2& get_head(size_t id) {
			assert(_heads);
			return _heads->at(id);
		}

		void set_heads(Point2Vector::Ptr points) { _tails = points; }
	protected:
		Point2Vector::Ptr _tails; ///< 组成拖尾路径的点的集合
		Point2Vector::Ptr _heads; ///< 组成头部拖尾的点的集合（即起始点前的路径序列）
	};

	///连续点集合构成的路径
	class SN3D_PRINT3D_COMMON_API Path
	{
	public:
		typedef std::shared_ptr<Path> Ptr;
		typedef std::shared_ptr<const Path> ConstPtr;

		/** \brief 从输入流导入path
		*  \param[in]      is			输入流
		*  \param[out]     path		path
		*/
		friend std::istream& operator>>(std::istream& is, Path& path);
		/** \brief path导出到输出流
		*  \param[in]      os			输出流
		*  \param[in]     path		path
		*/
		friend std::ostream& operator<<(std::ostream& os, const Path& path);

		//////////////////////////////////////////////////////构造函数和析构函数
		Path(); 
		~Path();

		/////////////////////////////////////////////////////set and get
		Scalar get_z() const { return _z; }
		void set_z(Scalar z) { _z = z; }
		Scalar get_width() const { return _width; }
		void set_width(Scalar w) { _width = w; }
		int get_id() const { return _idx; }
		void set_id(int id) { _idx = id; }
		PathType get_type() const { return _type; }
		void set_type(PathType type) { _type = type; }
		bool is_fill() { return _type >= PathType_FILL_DISCONTINUE_OUTER_UP  && _type <= PathType_FILL_CONTINUE_DEFAULT; }

		/////////////////////////////////////////////////////access and modify points
		size_t get_vn() const { 
			assert(_points);
			return _points->size(); 
		}
		Point2Vector::ConstPtr get_points() const { return _points; }
		Point2Vector::Ptr get_points() { return _points; }
		const Point2& get_point(size_t id) const { 
			assert(_points && get_vn() > id);
			return _points->at(id); 
		}
		Point2& get_point(size_t id) { 
			assert(_points && get_vn() > id);
			return _points->at(id); 
		}
		const Point2& get_start_point() const{
			if (_extendPathComp != nullptr && _extendPathComp->get_heads() != nullptr){
				return _extendPathComp->get_heads()->front();
			}
			return _points->front();
		}
		const Point2& get_end_point() const{
			if (_extendPathComp != nullptr && _extendPathComp->get_tails() != nullptr){
				return _extendPathComp->get_tails()->back();
			}
			return _points->back();
		}
		///返回路径中的第id个顶点
		const Point2& operator()(size_t id) const { 
			assert(_points && get_vn() > id);
			return _points->at(id); 
		}
		///返回路径中的第id个顶点
		Point2& operator()(size_t id) { 
			assert(_points && get_vn() > id);
			return _points->at(id);
		}

		//////////////////////////////////////////////////////////access and modify colors
		ColorVector::ConstPtr get_colors() const { return _colors; }
		ColorVector::Ptr get_colors() { return _colors; }
		const Color& get_color(size_t id) const {
			assert(_colors);
			return _colors->at(id);
		} 
		Color& get_color(size_t id) {
			assert(_colors);
			return _colors->at(id);
		}


		//////////////////////////////////////////////////////////add and delete points colors
		void reserve_vn(size_t num) { 
			if(!_points) init_points(); _points->reserve(num);
			if(_colors) _colors->reserve(num);
		}
		void resize_vn(size_t num) { 
			if(!_points) init_points(); _points->resize(num); 
			if(_colors) _colors->resize(num);
		}

		///给path中的points分配空间
		void init_points() {  _points = Point2Vector::Ptr(new Point2Vector); }
		void init_colors() { _colors = ColorVector::Ptr(new ColorVector); }
        void set_points(Point2Vector::Ptr points) { _points = points; }
		void set_colors(ColorVector::Ptr colors) { _colors = colors; }

		//////////////////////////////////////////////////////////////////////////

		void set_flags(std::shared_ptr<std::vector<unsigned char>> flags) { _flags = flags; }
		std::shared_ptr<const std::vector<unsigned char>> get_flags() const { return _flags; }

		bool has_extra_shell_component() const { return _extraShellComp != nullptr; }
		void set_extra_shell_component(ExtraShellComponent::Ptr comp) { _extraShellComp = comp; }
		ExtraShellComponent::Ptr get_extra_shell_component() { return _extraShellComp; }
		ExtraShellComponent::ConstPtr get_extra_shell_component() const { return _extraShellComp; }

		bool has_extend_path_component() const { return _extendPathComp != nullptr; }
		void set_extend_path_component(ExtendPathComponent::Ptr comp) { _extendPathComp = comp; }
		ExtendPathComponent::Ptr get_extend_path_component() { return _extendPathComp; }
		ExtendPathComponent::ConstPtr get_extend_path_component() const { return _extendPathComp; }

	protected:
		Scalar _z;                ///<路径所在的高度值
		Point2Vector::Ptr _points; ///<组成路径的点的集合
		ColorVector::Ptr _colors;  ///<组成路径的点的颜色集合
		std::shared_ptr<std::vector<unsigned char>> _flags; ///<组成路径的点的标记

		Scalar _width;                ///<线的宽度
		int _idx;                      ///<索引值，由0开始按生成先后顺序(将boundary,extrashell,fill这三类，将他们按照各自所属的区域组织起来，_idx就是所属区域的索引)
		PathType _type;              ///<路径类型

		ExtraShellComponent::Ptr _extraShellComp;
		ExtendPathComponent::Ptr _extendPathComp;
	};

}//namespace Sn3DAlgorithm


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_PATH_H