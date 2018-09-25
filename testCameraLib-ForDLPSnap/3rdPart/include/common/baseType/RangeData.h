/***********************************************
 * 定义有序点云类
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_RANGE_DATA_H
#define SN3D_ALGORITHM_COMMON_RANGE_DATA_H

#include "Traits.h"
#include "Cloud.h"

namespace Sn3DAlgorithm
{
    ///标志点的数据类型
    struct SN3D_COMMON_API RefPtsData
    {
        Eigen::Vector3d coord; ///< 标志点的坐标
        Eigen::Vector3d nrv;   ///< 标志点的法向量
        int corrId;           ///< 标志点的id，相同id的标志点为对应点
        double weight;        ///< 标志点的可信权重，范围(0, 1]
    };
    

	/** \brief 定义有序点云类
	 *  \note  保存的所有信息都是基于局部坐标系。保存的RigidMatrix是局部到全局的刚体变换
	 *          在局部坐标系中主相机的位置(0, 0, 0), 朝向(0, 0, 1)
	 *
	 *         ----------> x,v
	 *         |
	 *         |
     *        \|/ y,u     Z轴朝向里边
	 */
	class SN3D_COMMON_API RangeData : public Cloud
	{
	public:
		///图像的UV坐标
		struct UVCoord
		{
			UVCoord(): u(0), v(0) {}
			UVCoord(int uu, int vv) :u(uu), v(vv) {}
			int u;   ///< 行坐标  y 范围 [0,height]或 [0, row]
			int v;   ///< 列坐标  x 范围 [0,width] 或 [0, col]
		};

		/////////////////////////////////////////构造函数和析构函数
		RangeData();
		RangeData(const RangeData& from) = default;
		RangeData(RangeData&& from) NOEXCEPT;//! @todo use =default after vs2015
		virtual ~RangeData() = default;

		//assignment operator
		RangeData& operator=(const RangeData& from) = default;
		RangeData& operator=(RangeData&& from);//! @todo use =default after vs2015

		/////////////////////////////////////////////////acess and modify
		inline const std::vector<UVCoord>& get_vUVs() const;
		inline void get_vUVs(std::vector<UVCoord>& vUVs) const;
		inline const UVCoord& get_vUV(size_t id) const;
		inline UVCoord& get_vUV(size_t id);
		inline void set_vUV(size_t id, const UVCoord& vUV);

		///0表示该cell无效，否则表示该cell有效
		inline const std::vector<char>& get_cellFlags() const;
		inline void get_cellFlags(std::vector<char>& cellFlags) const;
		inline char get_cellFlag(size_t id) const;
		inline void set_cellFlag(size_t id, char cellFlag);

		///获得顶点的全局坐标
		inline void get_global_vCoords(std::vector<Eigen::Vector3>& vCoords) const;
		inline Eigen::Vector3 get_global_vCoord(size_t id) const;
		inline void get_global_vCoord(size_t id, Eigen::Vector3& vCoord) const;

		/////////////////////////////////////////////////////////////add and delete
		inline virtual void reserve_vn(size_t num);
		inline virtual void resize_vn(size_t num);

		///调用该函数，备选状态全部关闭，重置cellFlag和UV
		inline virtual void set_vCoords(const std::vector<Eigen::Vector3>& vCoords);
		inline virtual size_t add_vCoord(const Eigen::Vector3& vCoord);

		///调用该函数，备选状态全部关闭，重置cellFlag
		inline void set_vCoords_and_vUVs(const std::vector<Eigen::Vector3>& vCoords, 
			const std::vector<UVCoord>& vUVs);
		inline size_t add_vCoord_and_vUV(const Eigen::Vector3& vCoord, const UVCoord& vUV);


		////////////////////////////////////////////height width
		inline void set_size(int h, int w);
		inline int get_height() const;
		inline int get_width() const;

		///////////////////////////////////////////////rigidMatrix
		inline void set_rigidMatrix(const RigidMatrix& mat);
		inline const RigidMatrix& get_rigidMatrix() const;
		inline RigidMatrix& get_rigidMatrix();

		/////////////////////////////////////////////refPts
		inline bool is_has_refPts() const;
		inline void set_refPts(const std::vector<RefPtsData>& refPts);
		inline const std::vector<RefPtsData>& get_refPts() const;
		inline std::vector<RefPtsData>& get_refPts();

		///////////////////////////////////////////////主从相机矩阵
		RigidMatrix& get_host_camera_matrix() { return _hostCameraRT;}
		RigidMatrix& get_slave_camera_matrix() { return _slaveCameraRT;}
		const RigidMatrix& get_host_camera_matrix()	const { return _hostCameraRT; }
		const RigidMatrix& get_slave_camera_matrix() const { return _slaveCameraRT; }

		/////////////////////////////////////////////////////////////////可选组件
		inline void enable_FNormal();
		inline void disable_FNormal();
		inline bool is_enable_FNormal() const;
		inline const std::vector<Eigen::Vector3>& get_FNrvs() const;
		inline void get_FNrvs(std::vector<Eigen::Vector3>& FNrvs) const;
		inline const Eigen::Vector3& get_FNrv(size_t id) const;
		inline Eigen::Vector3& get_FNrv(size_t id);
		inline void set_FNrv(size_t id, const Eigen::Vector3& FNrv);

		///获得cell的全局法向量
		inline void get_global_FNrv(std::vector<Eigen::Vector3>& FNrvs) const;
		inline Eigen::Vector3 get_global_FNrv(size_t id) const;
		inline void get_global_FNrv(size_t id, Eigen::Vector3& FNrv) const;

		///点的全局法向量
		inline void get_global_vNrv(std::vector<Eigen::Vector3>& vNrvs) const;
		inline Eigen::Vector3 get_global_vNrv(size_t id) const;
		inline void get_global_vNrv(size_t id, Eigen::Vector3& vNrv) const;

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

		///////////////////////////////////////////////////////////////////operator
		///删除孤立点，删除点的数据空间需要手动释放
		void delete_isolate_vertices(const Eigen::MatrixXi& map);

        ///获取模型包围盒
        inline void get_global_boundBox(Eigen::Vector3& boxMin, Eigen::Vector3& boxMax) const;

        /** \brief 创建像素点到顶点的索引关系， 
        *  \note map(i, j) = k, 代表像素(i,j)对应第k个点。
        *        map(i, j) = -1, 代表像素(i,j)没有对应点
        *        map.size() = height, width;
        */
        void create_vertices_index_map(Eigen::MatrixXi& map) const;

		/// 删除顶点，和相邻cell 
		void delete_vertex_and_cell(int id, const Eigen::MatrixXi& map);

        /** \brief 有序网格转化成四边形网格 
        *  \param[in]   map        像素到顶点的索引关系
        *  \param[out]  facePtIdx  facePtIdx[i][j] = k 代表第i个四边形的第j个顶点的id是k
        */ 
		void convert_to_quad(const Eigen::MatrixXi& map, std::vector<Eigen::Vector4i>& facePtIdx) const;

		///更新点的法向量
		void update_VNrv(const Eigen::MatrixXi& map);

		///得到点的法向量
		void compute_VNrv(const Eigen::MatrixXi& map, std::vector<Eigen::Vector3>& vNrvs) const;

		///更新面的法向量
		void update_FNrv(const Eigen::MatrixXi& map);

		///得到面的法向量
		void compute_FNrv(const Eigen::MatrixXi& map, std::vector<Eigen::Vector3>& FNrvs) const;

		///模型经过一个刚体变换
		virtual void rigid_transformation(const RigidMatrix& mat);

		///验证数据是否合法. UNDO:_refPts中的nrv和weight没有检查
		virtual bool is_legal() const;
		
		///修补不合法数据. UNDO: _refPts没有检查
		virtual void repair_illegal();     
       
        ///根据主从相机参数，计算从相机相对于主相机的位置
        Eigen::Vector3      get_slave_carama_position();

        ///为保持读写数据一致
        double      get_meadistance() const {return _meadistance;}
        void        set_meadistance(double meadistance)  {_meadistance =meadistance; } 
		Eigen::Vector3d& get_cameraDir() { return _cameraDir;}
		const Eigen::Vector3d& get_cameraDir() const { return _cameraDir; }

        ///计算重心
        virtual Eigen::Vector3 get_barycenter() const;

    protected:
        int _height;     ///<  相机的长分辨率
        int _width;      ///<  相机的宽分辨率
        std::vector<UVCoord> _vUVs;            ///< 点在图片坐标系下的坐标(矫正前)
        std::vector<char> _cellFlags;       ///< 即以{(u,v)(u+1,v)(u+1,v+1)(u,v+1)}组成的面的标志位。1为有效，0为无效   
        RigidMatrix _mat;                       ///< 该点云的刚体变换
        std::vector<RefPtsData> _refPts;        ///< 标志点的信息。标志点的位置和是在局部坐标系下

		RigidMatrix  _hostCameraRT;      ///<主相机矩阵。标定板坐标系转化到主相机坐标系的矩阵
		RigidMatrix  _slaveCameraRT;     ///<从相机矩阵。标定板坐标系转化到从相机坐标系的矩阵

		//可选属性
		bool _hasFNrv;
		std::vector<Eigen::Vector3> _FNrvs;     ///< 面片的法向量


		//########################################为保持读写数据一致
        Eigen::Vector3d  _cameraDir;         ///<主相机朝向
        double           _meadistance;           ///<模型平均点距
     
    };

	/////////////////////////////////////////////////acess and modify
	inline const std::vector<RangeData::UVCoord>& RangeData::get_vUVs() const
	{
		return _vUVs;
	}
	inline void RangeData::get_vUVs(std::vector<RangeData::UVCoord>& vUVs) const
	{
		vUVs = _vUVs;
	}
	inline const RangeData::UVCoord& RangeData::get_vUV(size_t id) const
	{
		Sn_Ensure(_vUVs.size() > id);
		return _vUVs[id];
	}
	inline RangeData::UVCoord& RangeData::get_vUV(size_t id)
	{
		Sn_Ensure(_vUVs.size() > id);
		return _vUVs[id];
	}
	inline void RangeData::set_vUV(size_t id, const RangeData::UVCoord& vUV)
	{
		Sn_Ensure(_vUVs.size() > id);
		_vUVs[id] = vUV;
	}

	inline const std::vector<char>& RangeData::get_cellFlags() const
	{
		return _cellFlags;
	}
	inline void RangeData::get_cellFlags(std::vector<char>& cellFlags) const
	{
		cellFlags = _cellFlags;
	}
	inline char RangeData::get_cellFlag(size_t id) const
	{
		Sn_Ensure(_cellFlags.size() > id);
		return _cellFlags[id];
	}
	inline void RangeData::set_cellFlag(size_t id, char cellFlag)
	{
		Sn_Ensure(_cellFlags.size() > id);
		_cellFlags[id] = cellFlag;
	}

	inline void RangeData::get_global_vCoords(std::vector<Eigen::Vector3>& vCoords) const
	{
		vCoords.resize(get_vn());
		for(size_t i = 0; i < get_vn(); ++i)
		{
			vCoords[i] = _mat * _vCoords[i];
		}
	}
	inline Eigen::Vector3 RangeData::get_global_vCoord(size_t id) const
	{
		Sn_Ensure(get_vn() > id);
		return _mat * _vCoords[id];
	}
	inline void RangeData::get_global_vCoord(size_t id, Eigen::Vector3& vCoord) const
	{
		Sn_Ensure(get_vn() > id);
		vCoord = _mat * _vCoords[id];
	}

	/////////////////////////////////////////////////////////////add and delete
	inline void RangeData::reserve_vn(size_t num)
	{
		_cellFlags.reserve(num);
		_vUVs.reserve(num);
		Cloud::reserve_vn(num);
		if (is_enable_FNormal()) _FNrvs.reserve(num);
	}
	inline void RangeData::resize_vn(size_t num)
	{
		_cellFlags.resize(num);
		_vUVs.resize(num);
        if (is_enable_FNormal()) _FNrvs.resize(num);
		Cloud::resize_vn(num);
	}

	inline void RangeData::set_vCoords(const std::vector<Eigen::Vector3>& vCoords)
	{
		Cloud::set_vCoords(vCoords);
		_cellFlags.resize(get_vn());
		_vUVs.resize(get_vn());
		disable_FNormal();
	}
	inline size_t RangeData::add_vCoord(const Eigen::Vector3& vCoord)
	{
		size_t id = Cloud::add_vCoord(vCoord);
		_cellFlags.push_back(0);
		_vUVs.push_back(UVCoord(0, 0));
		if(is_enable_FNormal())_FNrvs.push_back(Eigen::Vector3(0, 0, -1));
		return id;
	}

	inline void RangeData::set_vCoords_and_vUVs(const std::vector<Eigen::Vector3>& vCoords, 
		const std::vector<UVCoord>& vUVs)
	{
		Sn_Ensure(vUVs.size() == vCoords.size());
		Cloud::set_vCoords(vCoords);
		_vUVs = vUVs;
		_cellFlags.resize(get_vn());
		disable_FNormal();
	}
	inline size_t RangeData::add_vCoord_and_vUV(const Eigen::Vector3& vCoord, const UVCoord& vUV)
	{
		size_t id = Cloud::add_vCoord(vCoord);
		_vUVs.push_back(vUV);
		_cellFlags.push_back(0);
		if(is_enable_FNormal())_FNrvs.push_back(Eigen::Vector3(0, 0, -1));
		return id;
	}

	////////////////////////////////////////////height width
	inline void RangeData::set_size(int h, int w)
	{	
		Sn_Ensure(h > 0 && w > 0);
		_height = h;
		_width = w;
	}
	inline int RangeData::get_height() const
	{
		return _height;
	}
	inline int RangeData::get_width() const
	{
		return _width;
	}

	///////////////////////////////////////////////rigidMatrix
	inline void RangeData::set_rigidMatrix(const RigidMatrix& mat)
	{
		_mat = mat;
	}
	inline const RigidMatrix& RangeData::get_rigidMatrix() const
	{
		return _mat;
	}
	inline RigidMatrix& RangeData::get_rigidMatrix()
	{
		return _mat;
	}

	/////////////////////////////////////////////refPts
	bool RangeData::is_has_refPts() const
	{
		return _refPts.size() != 0;
	}
	void RangeData::set_refPts(const std::vector<RefPtsData>& refPts)
	{
		_refPts = refPts;
	}
	const std::vector<RefPtsData>& RangeData::get_refPts() const
	{
		return _refPts;
	}
	std::vector<RefPtsData>& RangeData::get_refPts()
	{
		return _refPts;
	}
//////////////////////////////////////////////////////////////////////可选组件
	inline void RangeData::enable_FNormal()
	{
		if(!is_enable_FNormal())
		{
			_hasFNrv = true;
			_FNrvs.resize(get_vn());
		}
	}
	inline void RangeData::disable_FNormal()
	{
		if(is_enable_FNormal())
		{
			_hasFNrv = false;
			std::vector<Eigen::Vector3>().swap(_FNrvs);
		}
	}
	inline bool RangeData::is_enable_FNormal() const
	{
		return _hasFNrv;
	}
	inline const std::vector<Eigen::Vector3>& RangeData::get_FNrvs() const
	{
		return _FNrvs;
	}
	inline void RangeData::get_FNrvs(std::vector<Eigen::Vector3>& FNrvs) const
	{
		FNrvs = _FNrvs;
	}
	inline const Eigen::Vector3& RangeData::get_FNrv(size_t id) const
	{
		Sn_Ensure(_FNrvs.size() > id);
		return _FNrvs[id];
	}
	inline Eigen::Vector3& RangeData::get_FNrv(size_t id)
	{
		Sn_Ensure(_FNrvs.size() > id);
		return _FNrvs[id];
	}
	inline void RangeData::set_FNrv(size_t id, const Eigen::Vector3& FNrv)
	{
		Sn_Ensure(_FNrvs.size() > id);
		_FNrvs[id] = FNrv;
	}

	inline void RangeData::get_global_FNrv(std::vector<Eigen::Vector3>& FNrvs) const
	{
		FNrvs.resize(_FNrvs.size());
		for(int i = 0; i < FNrvs.size(); ++i) FNrvs[i] = _mat.transform_normal(_FNrvs[i]);
	}

	inline Eigen::Vector3 RangeData::get_global_FNrv(size_t id) const
	{
		Sn_Ensure(_FNrvs.size() > id);
		return _mat.transform_normal(_FNrvs[id]);
	}

	inline void RangeData::get_global_FNrv(size_t id, Eigen::Vector3& FNrv) const
	{
		Sn_Ensure(_FNrvs.size() > id);
		FNrv = _mat.transform_normal(_FNrvs[id]);
	}

	inline void RangeData::get_global_vNrv(std::vector<Eigen::Vector3>& vNrvs) const
	{
		vNrvs.resize(_vNrvs.size());
		for(int i = 0; i < vNrvs.size(); ++i) vNrvs[i] = _mat.transform_normal(_vNrvs[i]);
	}

	inline Eigen::Vector3 RangeData::get_global_vNrv(size_t id) const
	{
		Sn_Ensure(_vNrvs.size() > id);
		return _mat.transform_normal(_vNrvs[id]);
	}

	inline void RangeData::get_global_vNrv(size_t id, Eigen::Vector3& vNrv) const
	{
		Sn_Ensure(_vNrvs.size() > id);
		vNrv = _mat.transform_normal(_vNrvs[id]);
	}

	inline void RangeData::get_global_boundBox(Eigen::Vector3& gBoxMin, Eigen::Vector3& gBoxMax) const 
    {
        gBoxMin.setZero();
        gBoxMax.setZero();

        uint ii=0;
        uint vn = (uint)_vCoords.size();
        std::vector<Eigen::Vector3> gCoords(vn);
        get_global_vCoords(gCoords);

        
        // 寻找第一个点
        for ( ii=0; ii<vn; ii++)
        {
            if(!_vFlags.empty() && _vFlags[ii].Deleted)
            {
                continue;
            }

            gBoxMin = gBoxMax = gCoords[ii];
            break;
        }

        for (; ii<vn; ii++)
        {
            if(!_vFlags.empty() && _vFlags[ii].Deleted)
            {
                continue;
            }

            for (int k=0; k<3; k++)
            {
                if(gCoords[ii][k]>gBoxMax[k])
                {
                   gBoxMax[k] = gCoords[ii][k];
                }                    

                if(gCoords[ii][k]<gBoxMin[k])
                {
                    gBoxMin[k] = gCoords[ii][k];
                }

            }//end k
        }//end ii
    }

}//namespace


#endif //SN3D_ALGORITHM_COMMON_RANGE_DATA_H