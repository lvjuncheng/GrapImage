/***********************************************
 * 定义无序点云类
 * \author 张健 
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_CLOUD_H
#define SN3D_ALGORITHM_COMMON_CLOUD_H

#include "Traits.h"
#include "BaseData.h"
#include "RigidMatrix.h"

namespace Sn3DAlgorithm
{
	union SN3D_COMMON_API VertexFlag
	{
		struct
		{
			bool Deleted:	1;
			bool Selected:	1;
			bool Visisted:	1;
			bool Modified:	1;
			bool Boundary:	1;
		};
		char flags;
		VertexFlag() :flags(0) {}
	};

	///无序点云类
	class SN3D_COMMON_API Cloud : public BaseData
	{
	public:
		///////////////////////////////////////////////////////构造函数和析构函数 
		Cloud();
		explicit Cloud(const std::vector<Eigen::Vector3>& vCoords);
		Cloud(const Cloud& from) = default;
		Cloud(Cloud&& from) NOEXCEPT;//! @todo default move ctor is not currently supported
		virtual ~Cloud();

		//assignment operator
		Cloud& operator=(const Cloud& from) = default;
		Cloud& operator=(Cloud&& from); //! @todo use default when compiler upgrate to vs15
		/////////////////////////////////////////////////////////access and modify
		inline size_t get_vn() const;
		inline const std::vector<Eigen::Vector3>& get_vCoords() const;
		inline void get_vCoords(std::vector<Eigen::Vector3>& vCoords) const;
		inline const Eigen::Vector3& get_vCoord(size_t id) const;
		inline Eigen::Vector3& get_vCoord(size_t id);
		inline void set_vCoord(size_t id, const Eigen::Vector3& vCoord);

		/////////////////////////////////////////////////////////add and delete
		inline virtual void reserve_vn(size_t num);
		inline virtual void resize_vn(size_t num);
		
		///调用该函数，顶点的备选状态会全部关闭
		inline virtual void set_vCoords(const std::vector<Eigen::Vector3>& vCoords);
		inline virtual void set_vCoords(std::vector<Eigen::Vector3>&& vCoords);
		inline virtual size_t add_vCoord(const Eigen::Vector3& vCoord);

		//////////////////////////////////////////////////////////可选组件
		inline void enable_vColor();
		inline void disable_vColor();
		inline bool is_enable_vColor() const;
		inline const std::vector<Eigen::Vector3f>& get_vColors() const;
		inline void get_vColors(std::vector<Eigen::Vector3f>& vColors) const;
		inline const Eigen::Vector3f& get_vColor(size_t id) const;
		inline Eigen::Vector3f& get_vColor(size_t id);
		inline void set_vColor(size_t id, const Eigen::Vector3f& vColor);
		

		inline void enable_vNormal();
		inline void disable_vNormal();
		inline bool is_enable_vNormal() const;
		inline const std::vector<Eigen::Vector3>& get_vNrvs() const;
		inline void get_vNrvs(std::vector<Eigen::Vector3>& vNrvs) const;
		inline const Eigen::Vector3& get_vNrv(size_t id) const;
		inline Eigen::Vector3& get_vNrv(size_t id);
		inline void set_vNrv(size_t id, const Eigen::Vector3& vNrv);

		inline void enable_vFlag();
		inline void disable_vFlag();
		inline bool is_enable_vFlag() const;
		inline const std::vector<VertexFlag>& get_vFlags() const;
		inline void get_vFlags(std::vector<VertexFlag>& vFlags) const;
		inline const VertexFlag& get_vFlag(size_t id) const;
		inline VertexFlag& get_vFlag(size_t id);
		inline void set_vFlag(size_t id, const VertexFlag& vFlag);

		///enable_vFlag后vDeleted才有效. 如果disable_vFlag,则返回false
		inline bool get_vDeleted(size_t id) const;
		///enable_vFlag后vDeleted才有效
		inline void set_vDeleted(size_t id, bool flag);

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

		////////////////////////////////////////////////////////////////////operator
        
        ///获取模型包围盒
        virtual void get_boundBox(Eigen::Vector3& boxMin, Eigen::Vector3& boxMax) const;
		///模型经过一个刚体变换
		virtual void rigid_transformation(const RigidMatrix& mat);

		///验证数据是否合法
		virtual bool is_legal() const;
		///修补不合法数据
		virtual void repair_illegal();

	protected:
		std::vector<Eigen::Vector3> _vCoords; ///< 顶点的坐标

		//可选组件
		bool _hasVNrv;
		std::vector<Eigen::Vector3> _vNrvs;   ///< 顶点的法向量
		bool _hasVColor;
		std::vector<Eigen::Vector3f> _vColors;  ///< 顶点的颜色, 取值范围：0--1.0
		bool _hasVFlag;
		std::vector<VertexFlag> _vFlags;        ///< 顶点标志位
	};

	/////////////////////////////////////////////////////////acess and modify
	inline size_t Cloud::get_vn() const
	{
		return _vCoords.size();
	}
	inline const std::vector<Eigen::Vector3>& Cloud::get_vCoords() const
	{
		return _vCoords;
	}
	inline void Cloud::get_vCoords(std::vector<Eigen::Vector3>& vCoords) const
	{
		vCoords = _vCoords;
	}
	inline const Eigen::Vector3& Cloud::get_vCoord(size_t id) const
	{
		Sn_Ensure(get_vn() > id);
		return _vCoords[id];
	}
	inline Eigen::Vector3& Cloud::get_vCoord(size_t id)
	{
		Sn_Ensure(get_vn() > id);
		return _vCoords[id];
	}
	inline void Cloud::set_vCoord(size_t id, const Eigen::Vector3& vCoord)
	{
		Sn_Ensure(get_vn() > id);
		_vCoords[id] = vCoord;
	}

	/////////////////////////////////////////////////////////add and delete
	inline void Cloud::reserve_vn(size_t num)
	{
		_vCoords.reserve(num);
		if(is_enable_vNormal()) _vNrvs.reserve(num);
		if(is_enable_vColor()) _vColors.reserve(num);
		if(is_enable_vFlag()) _vFlags.reserve(num);
	}
	inline void Cloud::resize_vn(size_t num)
	{
		_vCoords.resize(num);
		if(is_enable_vNormal()) _vNrvs.resize(num);
		if(is_enable_vColor()) _vColors.resize(num);
		if(is_enable_vFlag()) _vFlags.resize(num);
	}

	inline void Cloud::set_vCoords(const std::vector<Eigen::Vector3>& vCoords)
	{
		_vCoords = vCoords;
		disable_vNormal();
		disable_vColor();
		disable_vFlag();
	}


	inline void Cloud::set_vCoords(std::vector<Eigen::Vector3>&& vCoords)
	{
		_vCoords = std::move(vCoords);
		disable_vNormal();
		disable_vColor();
		disable_vFlag();
	}


	inline size_t Cloud::add_vCoord(const Eigen::Vector3& vCoord)
	{
		_vCoords.push_back(vCoord);
		if(is_enable_vNormal()) _vNrvs.push_back(Eigen::Vector3(0, 0, -1));
		if(is_enable_vColor()) _vColors.push_back(Eigen::Vector3f(0, 0, 0));
		if(is_enable_vFlag()) _vFlags.push_back(VertexFlag());
		return _vCoords.size()-1;

	}

	//////////////////////////////////////////////////////////可选组件
	inline void Cloud::enable_vColor()
	{
		if(!is_enable_vColor())
		{
			_hasVColor = true;
			_vColors.resize(get_vn());
		}
	}
	inline void Cloud::disable_vColor()
	{
		if(is_enable_vColor())
		{
			_hasVColor = false;
			std::vector<Eigen::Vector3f>().swap(_vColors);
		}
	}
	inline bool Cloud::is_enable_vColor() const
	{
		return _hasVColor;
	}
	inline const std::vector<Eigen::Vector3f>& Cloud::get_vColors() const
	{
		return _vColors;
	}
	inline void Cloud::get_vColors(std::vector<Eigen::Vector3f>& vColors) const
	{
		vColors = _vColors;
	}
	inline const Eigen::Vector3f& Cloud::get_vColor(size_t id) const
	{
		Sn_Ensure(_vColors.size() > id);
		return _vColors[id];
	}
	inline Eigen::Vector3f& Cloud::get_vColor(size_t id)
	{
		Sn_Ensure(_vColors.size() > id);
		return _vColors[id];
	}
	inline void Cloud::set_vColor(size_t id, const Eigen::Vector3f& vColor)
	{
		Sn_Ensure(_vColors.size() > id);
		_vColors[id] = vColor;
	}


	inline void Cloud::enable_vNormal()
	{
		if(!is_enable_vNormal())
		{
			_hasVNrv = true;
			_vNrvs.resize(get_vn());
		}
	}
	inline void Cloud::disable_vNormal()
	{
		if(is_enable_vNormal())
		{
			_hasVNrv = false;
			std::vector<Eigen::Vector3>().swap(_vNrvs);
		}
	}
	inline bool Cloud::is_enable_vNormal() const
	{
		return _hasVNrv;
	}
	inline const std::vector<Eigen::Vector3>& Cloud::get_vNrvs() const
	{
		return _vNrvs;
	}
	inline void Cloud::get_vNrvs(std::vector<Eigen::Vector3>& vNrvs) const
	{
		vNrvs = _vNrvs;
	}
	inline const Eigen::Vector3& Cloud::get_vNrv(size_t id) const
	{
		Sn_Ensure(_vNrvs.size() > id);
		return _vNrvs[id];
	}
	inline Eigen::Vector3& Cloud::get_vNrv(size_t id)
	{
		Sn_Ensure(_vNrvs.size() > id);
		return _vNrvs[id];
	}
	inline void Cloud::set_vNrv(size_t id, const Eigen::Vector3& vNrv)
	{
		Sn_Ensure(_vNrvs.size() > id);
		_vNrvs[id] = vNrv;
	}


	inline void Cloud::enable_vFlag()
	{
		if(!is_enable_vFlag())
		{
			_hasVFlag = true;
			_vFlags.resize(get_vn());
		}
	}
	inline void Cloud::disable_vFlag()
	{
		if(is_enable_vFlag())
		{
			_hasVFlag = false;
			std::vector<VertexFlag>().swap(_vFlags);
		}
	}
	inline bool Cloud::is_enable_vFlag() const
	{
		return _hasVFlag;
	}
	inline const std::vector<VertexFlag>& Cloud::get_vFlags() const
	{
		return _vFlags;
	}
	inline void Cloud::get_vFlags(std::vector<VertexFlag>& vFlags) const
	{
		vFlags = _vFlags;
	}
	inline const VertexFlag& Cloud::get_vFlag(size_t id) const
	{
		Sn_Ensure(_vFlags.size() > id);
		return _vFlags[id];
	}
	inline VertexFlag& Cloud::get_vFlag(size_t id)
	{
		Sn_Ensure(_vFlags.size() > id);
		return _vFlags[id];
	}
	inline void Cloud::set_vFlag(size_t id, const VertexFlag& vFlag)
	{
		Sn_Ensure(_vFlags.size() > id);
		_vFlags[id] = vFlag;
	}


	inline bool Cloud::get_vDeleted(size_t id) const
	{
		if(!_hasVFlag) return false;
		Sn_Ensure(_vFlags.size() > id);
		return _vFlags[id].Deleted;
	}
	inline void Cloud::set_vDeleted(size_t id, bool flag)
	{
		Sn_Ensure(_vFlags.size() > id);
		_vFlags[id].Deleted = flag;
	}

}//namespace


#endif //SN3D_ALGORITHM_COMMON_CLOUD_H