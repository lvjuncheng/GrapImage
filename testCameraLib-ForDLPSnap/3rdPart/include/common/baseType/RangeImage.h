/***********************************************
* 定义深度图类
* \author 张健
***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_RANGE_IMAGE_H
#define SN3D_ALGORITHM_COMMON_RANGE_IMAGE_H

#include "RangeData.h"
#include "PointImage.h"
#include "Image.h"

namespace Sn3DAlgorithm
{
	/** \brief 定义深度图类。该深度图完全按照相机摄影变换。点的坐标存在一定的噪声。
	 *  \note  保存的所有信息都是基于局部坐标系。保存的RigidMatrix是局部到全局的刚体变换
	 *          在局部坐标系中相机的位置(0, 0, 0), 朝向(0, 0, 1)
	 *
	 *         ----------> x,v,width
	 *         |
	 *         |
	 *        \|/ y,u,height     Z轴朝向里边
	 */
	class SN3D_COMMON_API RangeImage : public Cloud
	{
	public:
		///图像的UV坐标
		struct UVCoord
		{
			UVCoord() : u(0), v(0) {}
			UVCoord(int uu, int vv) :u(uu), v(vv) {}
			int u;   ///< 行坐标  y 范围 [0,height]或 [0, row]
			int v;   ///< 列坐标  x 范围 [0,width] 或 [0, col]
		};

		/////////////////////////////////////////构造函数和析构函数
		RangeImage();

		//////////////////////////////////////////////按照图的方式初始化
		/** \brief 初始化点云坐标
		 *  \param[in]  height, width   深度图的长宽
		 *  \param[in]  pointImage      点云图。pointImage[u*height+v]表示序号为(u,v)的点的坐标。（0,0,0）表示该点不存在
		 *  \param[in]  normalImage     法相图。如果normalImage.size()==0.则不会初始化法相.（0,0,0）表示该点法相不存在
		 *  \param[in]  colorImage      颜色图。如果colorImage.size()==0.则不会初始化颜色
		 *  \param[in]  delBadPoint     是否删除法向量不存在的点
		 *  \note  调用该函数开始，备选状态全部关闭
		 */
		void init_point_image(int height, int width, const std::vector<Eigen::Vector3>& pointImage,
			const std::vector<Eigen::Vector3>& normalImage, const std::vector<Eigen::Vector3f>& colorImage, bool delBadPoint = true);
		void init_point_image(int height, int width, const std::vector<Eigen::Vector3>& pointImage);

		void get_point_image(std::vector<Eigen::Vector3>& pointImage) const;
		void get_global_point_image(std::vector<Eigen::Vector3>& pointImage) const;
		void get_normal_image(std::vector<Eigen::Vector3>& normalImage) const;
		void get_global_normal_image(std::vector<Eigen::Vector3>& normalImage) const;
		void get_color_image(std::vector<Eigen::Vector3f>& colorImage) const;

		/////////////////////////////////////////////////acess and modify
		inline const std::vector<UVCoord>& get_vUVs() const;
		inline void get_vUVs(std::vector<UVCoord>& vUVs) const;
		inline const UVCoord& get_vUV(size_t id) const;
		inline UVCoord& get_vUV(size_t id);
		inline void set_vUV(size_t id, const UVCoord& vUV);

		///获得顶点的全局坐标
		inline void get_global_vCoords(std::vector<Eigen::Vector3>& vCoords) const;
		inline Eigen::Vector3 get_global_vCoord(size_t id) const;
		inline void get_global_vCoord(size_t id, Eigen::Vector3& vCoord) const;

		///点的全局法向量
		inline void get_global_vNrv(std::vector<Eigen::Vector3>& vNrvs) const;
		inline Eigen::Vector3 get_global_vNrv(size_t id) const;
		inline void get_global_vNrv(size_t id, Eigen::Vector3& vNrv) const;

		//////////////////////////////////////////////////////////////////////////
		

		/////////////////////////////////////////////////////////////add and delete
		inline virtual void reserve_vn(size_t num);
		inline virtual void resize_vn(size_t num);

		///调用该函数，备选状态全部关闭，重置UV
		inline virtual void set_vCoords(const std::vector<Eigen::Vector3>& vCoords);
		inline virtual size_t add_vCoord(const Eigen::Vector3& vCoord);

		///调用该函数，备选状态全部关闭
		inline void set_vCoords_and_vUVs(const std::vector<Eigen::Vector3>& vCoords,
			const std::vector<UVCoord>& vUVs);
		inline size_t add_vCoord_and_vUV(const Eigen::Vector3& vCoord, const UVCoord& vUV);

		////////////////////////////////////////////图片属性
		inline void set_size(int h, int w);
		inline int get_height() const;
		inline int get_width() const;

		inline void set_offset(float offsetY, float offsetX);
		inline float get_offsetY() const;
		inline float get_offsetX() const;

		//////////////////////////////////////////////_cameraInParam
		inline void set_cameraInParam(const CameraInParam& param);
		inline const CameraInParam& get_cameraInParam() const;
		inline CameraInParam& get_cameraInParam();

		///////////////////////////////////////////////rigidMatrix
		inline void set_rigidMatrix(const RigidMatrix& mat);
		inline const RigidMatrix& get_rigidMatrix() const;
		inline RigidMatrix& get_rigidMatrix();

		/////////////////////////////////////////////refPts
		inline bool is_has_refPts() const;
		inline void set_refPts(const std::vector<RefPtsData>& refPts);
		inline const std::vector<RefPtsData>& get_refPts() const;
		inline std::vector<RefPtsData>& get_refPts();

		////////////////////////////////////////////_s2hMat
		inline void set_s2hMat(const RigidMatrix& mat);
		inline const RigidMatrix& get_s2hMat() const;
		inline RigidMatrix& get_s2hMat();

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

		/////////////////////////////////////////////////////////////////operator

		/** \brief 创建像素点到顶点的索引关系，
		*  \note map(i, j) = k, 代表像素(i,j)对应第k个点。
		*        map(i, j) = -1, 代表像素(i,j)没有对应点
		*        map.size() = height, width;
		*/
		void create_vertices_index_map(Eigen::MatrixXi& map) const;

		///获取模型包围盒
		void get_global_boundBox(Eigen::Vector3& boxMin, Eigen::Vector3& boxMax) const;

		///模型经过一个刚体变换
		virtual void rigid_transformation(const RigidMatrix& mat);

		///验证数据是否合法. UNDO:_refPts中的nrv和weight没有检查
		virtual bool is_legal() const;

		///修补不合法数据. UNDO: _refPts没有检查
		virtual void repair_illegal();

		/** \brief 更新点的法向量
		 *  \param[in]  range       求法向时拟合平面的邻域大小
		 *  \param[in]  validThres  求法向时临域内部有效点个数大于该阈值才可计算
		 *  \param[in]  depthThres  该参数意为在拟合平面求取法向时，中心点和领域点Z方向的差距应该小于depthDiff，才能参与拟合
		 */
		void update_VNrv(const Eigen::MatrixXi& map, int range, int validThres, float depthThres);

		///得到点的法向量
		void compute_VNrv(const Eigen::MatrixXi& map, int range, int validThres, float depthThres, 
			std::vector<Eigen::Vector3>& vNrvs) const;

	protected:
		int _width;   ///<当前深度图的宽度
		int _height;  ///<当前深度图的高度
		float _offsetX; ///<点坐标相对UV值的偏移量
		float _offsetY; ///<点坐标相对UV值的偏移量
		CameraInParam _cameraInParam;///< 重建当前片点的相机的内参数
		RigidMatrix _mat;                       ///< 该点云的刚体变换
		std::vector<UVCoord> _vUVs;            ///< 点在图片坐标系下的坐标(矫正前)
		std::vector<RefPtsData> _refPts;        ///< 标志点的信息。标志点的位置和是在局部坐标系下
		RigidMatrix _s2hMat;                ///<从相机坐标系到主相机坐标系的变换矩阵
	};

	int SN3D_COMMON_API RangeImage_convert_to_PointImage(const RangeImage& range, PointImage& point);
	int SN3D_COMMON_API PointImage_convert_to_RangeImage(const PointImage& point, RangeImage& range);

	/////////////////////////////////////////////////acess and modify
	inline const std::vector<RangeImage::UVCoord>& RangeImage::get_vUVs() const
	{
		return _vUVs;
	}
	inline void RangeImage::get_vUVs(std::vector<RangeImage::UVCoord>& vUVs) const
	{
		vUVs = _vUVs;
	}
	inline const RangeImage::UVCoord& RangeImage::get_vUV(size_t id) const
	{
		Sn_Ensure(_vUVs.size() > id);
		return _vUVs[id];
	}
	inline RangeImage::UVCoord& RangeImage::get_vUV(size_t id)
	{
		Sn_Ensure(_vUVs.size() > id);
		return _vUVs[id];
	}
	inline void RangeImage::set_vUV(size_t id, const RangeImage::UVCoord& vUV)
	{
		Sn_Ensure(_vUVs.size() > id);
		_vUVs[id] = vUV;
	}

	inline void RangeImage::get_global_vCoords(std::vector<Eigen::Vector3>& vCoords) const
	{
		vCoords.resize(get_vn());
		for (size_t i = 0; i < get_vn(); ++i)
		{
			vCoords[i] = _mat * _vCoords[i];
		}
	}
	inline Eigen::Vector3 RangeImage::get_global_vCoord(size_t id) const
	{
		Sn_Ensure(get_vn() > id);
		return _mat * _vCoords[id];
	}
	inline void RangeImage::get_global_vCoord(size_t id, Eigen::Vector3& vCoord) const
	{
		Sn_Ensure(get_vn() > id);
		vCoord = _mat * _vCoords[id];
	}

	inline void RangeImage::get_global_vNrv(std::vector<Eigen::Vector3>& vNrvs) const
	{
		vNrvs.resize(_vNrvs.size());
		for (int i = 0; i < vNrvs.size(); ++i) vNrvs[i] = _mat.transform_normal(_vNrvs[i]);
	}

	inline Eigen::Vector3 RangeImage::get_global_vNrv(size_t id) const
	{
		Sn_Ensure(_vNrvs.size() > id);
		return _mat.transform_normal(_vNrvs[id]);
	}

	inline void RangeImage::get_global_vNrv(size_t id, Eigen::Vector3& vNrv) const
	{
		Sn_Ensure(_vNrvs.size() > id);
		vNrv = _mat.transform_normal(_vNrvs[id]);
	}

	/////////////////////////////////////////////////////////////add and delete
	inline void RangeImage::reserve_vn(size_t num)
	{
		_vUVs.reserve(num);
		Cloud::reserve_vn(num);
	}
	inline void RangeImage::resize_vn(size_t num)
	{
		_vUVs.resize(num);
		Cloud::resize_vn(num);
	}

	inline void RangeImage::set_vCoords(const std::vector<Eigen::Vector3>& vCoords)
	{
		Cloud::set_vCoords(vCoords);
		_vUVs.resize(get_vn());
	}
	inline size_t RangeImage::add_vCoord(const Eigen::Vector3& vCoord)
	{
		size_t id = Cloud::add_vCoord(vCoord);
		_vUVs.push_back(UVCoord(0, 0));
		return id;
	}

	inline void RangeImage::set_vCoords_and_vUVs(const std::vector<Eigen::Vector3>& vCoords,
		const std::vector<UVCoord>& vUVs)
	{
		Sn_Ensure(vUVs.size() == vCoords.size());
		Cloud::set_vCoords(vCoords);
		_vUVs = vUVs;
	}
	inline size_t RangeImage::add_vCoord_and_vUV(const Eigen::Vector3& vCoord, const UVCoord& vUV)
	{
		size_t id = Cloud::add_vCoord(vCoord);
		_vUVs.push_back(vUV);
		return id;
	}


	////////////////////////////////////////////图片属性
	inline void RangeImage::set_size(int h, int w)
	{
		Sn_Ensure(h > 0 && w > 0);
		_height = h;
		_width = w;
	}
	inline int RangeImage::get_height() const
	{
		return _height;
	}
	inline int RangeImage::get_width() const
	{
		return _width;
	}

	inline void RangeImage::set_offset(float offsetY, float offsetX)
	{
		Sn_Ensure(offsetY > 0 && offsetX > 0);
		_offsetY = offsetY;
		_offsetX = offsetX;
	}
	inline float RangeImage::get_offsetY() const
	{
		return _offsetY;
	}
	inline float RangeImage::get_offsetX() const
	{
		return _offsetX;
	}

	//////////////////////////////////////////////_cameraInParam
	inline void RangeImage::set_cameraInParam(const CameraInParam& param)
	{
		_cameraInParam = param;
	}
	inline const CameraInParam& RangeImage::get_cameraInParam() const
	{
		return _cameraInParam;
	}
	inline CameraInParam& RangeImage::get_cameraInParam()
	{
		return _cameraInParam;
	}

	///////////////////////////////////////////////rigidMatrix
	inline void RangeImage::set_rigidMatrix(const RigidMatrix& mat)
	{
		_mat = mat;
	}
	inline const RigidMatrix& RangeImage::get_rigidMatrix() const
	{
		return _mat;
	}
	inline RigidMatrix& RangeImage::get_rigidMatrix()
	{
		return _mat;
	}

	/////////////////////////////////////////////refPts
	bool RangeImage::is_has_refPts() const
	{
		return _refPts.size() != 0;
	}
	void RangeImage::set_refPts(const std::vector<RefPtsData>& refPts)
	{
		_refPts = refPts;
	}
	const std::vector<RefPtsData>& RangeImage::get_refPts() const
	{
		return _refPts;
	}
	std::vector<RefPtsData>& RangeImage::get_refPts()
	{
		return _refPts;
	}

	////////////////////////////////////////////_s2hMat
	inline void RangeImage::set_s2hMat(const RigidMatrix& mat)
	{
		_s2hMat = mat;
	}
	inline const RigidMatrix& RangeImage::get_s2hMat() const
	{
		return _s2hMat;
	}
	inline RigidMatrix& RangeImage::get_s2hMat()
	{
		return _s2hMat;
	}

}//namespace

#endif //SN3D_ALGORITHM_COMMON_RANGE_IMAGE_H