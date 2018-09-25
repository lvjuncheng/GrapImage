/***********************************************
* �������ͼ��
* \author �Ž�
***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_RANGE_IMAGE_H
#define SN3D_ALGORITHM_COMMON_RANGE_IMAGE_H

#include "RangeData.h"
#include "PointImage.h"
#include "Image.h"

namespace Sn3DAlgorithm
{
	/** \brief �������ͼ�ࡣ�����ͼ��ȫ���������Ӱ�任������������һ����������
	 *  \note  �����������Ϣ���ǻ��ھֲ�����ϵ�������RigidMatrix�Ǿֲ���ȫ�ֵĸ���任
	 *          �ھֲ�����ϵ�������λ��(0, 0, 0), ����(0, 0, 1)
	 *
	 *         ----------> x,v,width
	 *         |
	 *         |
	 *        \|/ y,u,height     Z�ᳯ�����
	 */
	class SN3D_COMMON_API RangeImage : public Cloud
	{
	public:
		///ͼ���UV����
		struct UVCoord
		{
			UVCoord() : u(0), v(0) {}
			UVCoord(int uu, int vv) :u(uu), v(vv) {}
			int u;   ///< ������  y ��Χ [0,height]�� [0, row]
			int v;   ///< ������  x ��Χ [0,width] �� [0, col]
		};

		/////////////////////////////////////////���캯������������
		RangeImage();

		//////////////////////////////////////////////����ͼ�ķ�ʽ��ʼ��
		/** \brief ��ʼ����������
		 *  \param[in]  height, width   ���ͼ�ĳ���
		 *  \param[in]  pointImage      ����ͼ��pointImage[u*height+v]��ʾ���Ϊ(u,v)�ĵ�����ꡣ��0,0,0����ʾ�õ㲻����
		 *  \param[in]  normalImage     ����ͼ�����normalImage.size()==0.�򲻻��ʼ������.��0,0,0����ʾ�õ㷨�಻����
		 *  \param[in]  colorImage      ��ɫͼ�����colorImage.size()==0.�򲻻��ʼ����ɫ
		 *  \param[in]  delBadPoint     �Ƿ�ɾ�������������ڵĵ�
		 *  \note  ���øú�����ʼ����ѡ״̬ȫ���ر�
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

		///��ö����ȫ������
		inline void get_global_vCoords(std::vector<Eigen::Vector3>& vCoords) const;
		inline Eigen::Vector3 get_global_vCoord(size_t id) const;
		inline void get_global_vCoord(size_t id, Eigen::Vector3& vCoord) const;

		///���ȫ�ַ�����
		inline void get_global_vNrv(std::vector<Eigen::Vector3>& vNrvs) const;
		inline Eigen::Vector3 get_global_vNrv(size_t id) const;
		inline void get_global_vNrv(size_t id, Eigen::Vector3& vNrv) const;

		//////////////////////////////////////////////////////////////////////////
		

		/////////////////////////////////////////////////////////////add and delete
		inline virtual void reserve_vn(size_t num);
		inline virtual void resize_vn(size_t num);

		///���øú�������ѡ״̬ȫ���رգ�����UV
		inline virtual void set_vCoords(const std::vector<Eigen::Vector3>& vCoords);
		inline virtual size_t add_vCoord(const Eigen::Vector3& vCoord);

		///���øú�������ѡ״̬ȫ���ر�
		inline void set_vCoords_and_vUVs(const std::vector<Eigen::Vector3>& vCoords,
			const std::vector<UVCoord>& vUVs);
		inline size_t add_vCoord_and_vUV(const Eigen::Vector3& vCoord, const UVCoord& vUV);

		////////////////////////////////////////////ͼƬ����
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

		/////////////////////////////////////////////////////////////////Ӳ�����ڴ����ݽ���
		///�����ݱ�����Ӳ��
		virtual int swap_out_data();
		///�����ݴ�Ӳ�̶�����
		virtual int swap_in_data();

		///////////////////////////////////////////////////////////////////�ڴ����
		///����ڴ�
		virtual void clear();
		///��ɾ�������ݿռ��ͷų���
		virtual void squeeze();

		/////////////////////////////////////////////////////////////////operator

		/** \brief �������ص㵽�����������ϵ��
		*  \note map(i, j) = k, ��������(i,j)��Ӧ��k���㡣
		*        map(i, j) = -1, ��������(i,j)û�ж�Ӧ��
		*        map.size() = height, width;
		*/
		void create_vertices_index_map(Eigen::MatrixXi& map) const;

		///��ȡģ�Ͱ�Χ��
		void get_global_boundBox(Eigen::Vector3& boxMin, Eigen::Vector3& boxMax) const;

		///ģ�;���һ������任
		virtual void rigid_transformation(const RigidMatrix& mat);

		///��֤�����Ƿ�Ϸ�. UNDO:_refPts�е�nrv��weightû�м��
		virtual bool is_legal() const;

		///�޲����Ϸ�����. UNDO: _refPtsû�м��
		virtual void repair_illegal();

		/** \brief ���µ�ķ�����
		 *  \param[in]  range       ����ʱ���ƽ��������С
		 *  \param[in]  validThres  ����ʱ�����ڲ���Ч��������ڸ���ֵ�ſɼ���
		 *  \param[in]  depthThres  �ò�����Ϊ�����ƽ����ȡ����ʱ�����ĵ�������Z����Ĳ��Ӧ��С��depthDiff�����ܲ������
		 */
		void update_VNrv(const Eigen::MatrixXi& map, int range, int validThres, float depthThres);

		///�õ���ķ�����
		void compute_VNrv(const Eigen::MatrixXi& map, int range, int validThres, float depthThres, 
			std::vector<Eigen::Vector3>& vNrvs) const;

	protected:
		int _width;   ///<��ǰ���ͼ�Ŀ��
		int _height;  ///<��ǰ���ͼ�ĸ߶�
		float _offsetX; ///<���������UVֵ��ƫ����
		float _offsetY; ///<���������UVֵ��ƫ����
		CameraInParam _cameraInParam;///< �ؽ���ǰƬ���������ڲ���
		RigidMatrix _mat;                       ///< �õ��Ƶĸ���任
		std::vector<UVCoord> _vUVs;            ///< ����ͼƬ����ϵ�µ�����(����ǰ)
		std::vector<RefPtsData> _refPts;        ///< ��־�����Ϣ����־���λ�ú����ھֲ�����ϵ��
		RigidMatrix _s2hMat;                ///<���������ϵ�����������ϵ�ı任����
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


	////////////////////////////////////////////ͼƬ����
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