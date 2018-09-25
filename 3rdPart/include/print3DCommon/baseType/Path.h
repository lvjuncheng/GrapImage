/***********************************************
 * ����·����
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_PATH_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_PATH_H

#include <vector>
#include "Print3DTraits.h"
#include "./PointType.h"
#include <memory>

namespace Sn3DAlgorithm
{
	///ö��·���ļ�������
	enum SN3D_PRINT3D_COMMON_API PathType
	{
		PathType_DEFAULT = 0,

		//////////////////////////////////////////////////////////////////////////��������

		PathType_BOUNDARY_OUTER, ///<����������ʱ�룩
		PathType_BOUNDARY_INNER, ///<��������˳ʱ�룩

		//////////////////////////////////////////////////////////////////////////�������

		PathType_FILL_DISCONTINUE_OUTER_UP,	///<�������߶���䣬�ϱ��棬���ɷ�ʽΪ���㼯��ÿ�����ڽӵ����ɶ������߶�·������[0, 1] [2, 3] [4, 5]....
		PathType_FILL_DISCONTINUE_OUTER_DOWN,	///<�������߶���䣬�±��棬���ɷ�ʽΪ���㼯��ÿ�����ڽӵ����ɶ������߶�·������[0, 1] [2, 3] [4, 5]....
		PathType_FILL_DISCONTINUE_OUTER_UP_DOWN,	///<�������߶���䣬�����ϱ��������±��棬���ɷ�ʽΪ���㼯��ÿ�����ڽӵ����ɶ������߶�·������[0, 1] [2, 3] [4, 5]....
		PathType_FILL_DISCONTINUE_DEFAULT,	///<�������߶���䣬Ĭ����䣬���ɷ�ʽΪ���㼯��ÿ�����ڽӵ����ɶ������߶�·������[0, 1] [2, 3] [4, 5]....

		PathType_FILL_CONTINUE_OUTER_UP,		///<�����߶����--�ϱ��棬���ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1, 2, 3....]
		PathType_FILL_CONTINUE_OUTER_DOWN,	///<�����߶����--�±��棬���ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1, 2, 3....]
		PathType_FILL_CONTINUE_OUTER_UP_DOWN,	///<�����߶����--�����ϱ��������±��棬���ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1, 2, 3....]
		PathType_FILL_CONTINUE_DEFAULT,		///<�����߶����--Ĭ����䣬���ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1, 2, 3....]

		PathType_FILL_BOUNDARY_OUTER_UP,		///<������������ϱ���
		PathType_FILL_BOUNDARY_OUTER_DOWN,	///<���������--�±���
		PathType_FILL_BOUNDARY_OUTER_UP_DOWN,	///<���������---�����ϱ��������±���
		PathType_FILL_BOUNDARY_DEFAULT,		///<���������--Ĭ�����

		PathType_SMALL_REGION_EXTRA_SHELL,	///<�����ڱ�ʱ�����С��������
		PathType_SMALL_REGION_FILLING,	///<���������ʱ�����С��������

		PathType_SMALL_REGION_EXTRA_SHELL_FILL_CONTINUE,	///<�����߶����--�����ڱڶ����С�������ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1, 2, 3....]
		PathType_SMALL_REGION_EXTRA_SHELL_FILL_DISCONTINUE,	///<�������߶����--�����ڱڶ����С�������ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1] [2, 3] [4, 5]....
		PathType_SMALL_REGION_FILLING_FILL_CONTINUE,	///<�����߶����--��������������С�������ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1, 2, 3....]
		PathType_SMALL_REGION_FILLING_FILL_DISCONTINUE,///<�������߶����--��������������С�������ɷ�ʽΪ���㼯�е����е�����˳�����ӵ�·������[0, 1] [2, 3] [4, 5]....
		//////////////////////////////////////////////////////////////////////////֧������

		PathType_TOP_SUPPORT, ///<����֧��
		PathType_MID_SUPPORT, ///<�м�֧��
		PathType_BASE_SUPPORT, ///<����֧��
		PathType_BASE_SUPPORT_SURROUNDING, ///<����֧�Ű�Χ�ṹ

		//////////////////////////////////////////////////////////////////////////��������

		PathType_RAFT_BASE_SURROUNDING, ///<���������İ�Χ�ṹ
		PathType_RAFT_BASE, ///<��������
		PathType_RAFT_FIRST_INTERFACE, ///<��һ��Ӵ�����
		PathType_RAFT_SECOND_INTERFACE, ///<�ڶ���Ӵ�����

		//////////////////////////////////////////////////////////////////////////����뱡��
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

	///��ɫ����
	class ColorVector: public std::vector<Color>
	{
	public:
		typedef std::shared_ptr<ColorVector> Ptr;
		typedef std::shared_ptr<const ColorVector> ConstPtr;
	};

	/** \brief ����������Χ��
	*	\param[out]	boundarys			����
	*	\param[in]	minX				x��Сֵ
	*	\param[in]	minY				y��Сֵ
	*	\param[in]	maxX				x���ֵ
	*	\param[in]	maxY				y���ֵ
	*/
	void SN3D_PRINT3D_COMMON_API get_boundarys_bbox(const std::vector<Point2Vector::ConstPtr>& boundarys, Scalar& minX, Scalar& minY, Scalar& maxX, Scalar& maxY);
	void SN3D_PRINT3D_COMMON_API get_boundary_bbox(Point2Vector::ConstPtr boundarys, Scalar& minX, Scalar& minY, Scalar& maxX, Scalar& maxY);

	void SN3D_PRINT3D_COMMON_API translate_boundarys(std::vector<Point2Vector::Ptr>& boundarys, const Point2& trans);
	void SN3D_PRINT3D_COMMON_API translate_boundary(Point2Vector::Ptr boundarys, const Point2& trans);

	void SN3D_PRINT3D_COMMON_API rotate_boundarys(std::vector<Point2Vector::Ptr>& boundarys, const Point2& center, Scalar rotDeg);
	void SN3D_PRINT3D_COMMON_API rotate_boundary(Point2Vector::Ptr boundarys, const Point2& center, Scalar rotDeg);
	
	///��PtrתΪConstPtr
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
		int _extraShellIdx; ///<����ڱڵĲ������, ��0��ʼ���������Ⱥ�˳��
		bool _isDegenerateSmallRegion;	///<�����ڱڸ������ʾ�˻����������ڵ�ԭ����·��
	};

	///����β����������
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
		Point2Vector::Ptr _tails; ///< �����β·���ĵ�ļ���
		Point2Vector::Ptr _heads; ///< ���ͷ����β�ĵ�ļ��ϣ�����ʼ��ǰ��·�����У�
	};

	///�����㼯�Ϲ��ɵ�·��
	class SN3D_PRINT3D_COMMON_API Path
	{
	public:
		typedef std::shared_ptr<Path> Ptr;
		typedef std::shared_ptr<const Path> ConstPtr;

		/** \brief ������������path
		*  \param[in]      is			������
		*  \param[out]     path		path
		*/
		friend std::istream& operator>>(std::istream& is, Path& path);
		/** \brief path�����������
		*  \param[in]      os			�����
		*  \param[in]     path		path
		*/
		friend std::ostream& operator<<(std::ostream& os, const Path& path);

		//////////////////////////////////////////////////////���캯������������
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
		///����·���еĵ�id������
		const Point2& operator()(size_t id) const { 
			assert(_points && get_vn() > id);
			return _points->at(id); 
		}
		///����·���еĵ�id������
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

		///��path�е�points����ռ�
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
		Scalar _z;                ///<·�����ڵĸ߶�ֵ
		Point2Vector::Ptr _points; ///<���·���ĵ�ļ���
		ColorVector::Ptr _colors;  ///<���·���ĵ����ɫ����
		std::shared_ptr<std::vector<unsigned char>> _flags; ///<���·���ĵ�ı��

		Scalar _width;                ///<�ߵĿ��
		int _idx;                      ///<����ֵ����0��ʼ�������Ⱥ�˳��(��boundary,extrashell,fill�����࣬�����ǰ��ո���������������֯������_idx�����������������)
		PathType _type;              ///<·������

		ExtraShellComponent::Ptr _extraShellComp;
		ExtendPathComponent::Ptr _extendPathComp;
	};

}//namespace Sn3DAlgorithm


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_PATH_H