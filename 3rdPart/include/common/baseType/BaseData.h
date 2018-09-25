/***********************************************
 * 定义网格，无序点云，有序点云的基类
 * \author 张健 
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_BASE_DATA_H
#define SN3D_ALGORITHM_COMMON_BASE_DATA_H

#include "Traits.h"
#include <string>
#include<iostream> 


namespace Sn3DAlgorithm
{
	///定义几种基本的模型类型
	enum  SN3D_COMMON_API ObjectType
	{
		ObjectType_BASE,   ///< 基本类型
		ObjectType_TRIMESH, ///< 半边结构表示的三角网格
		ObjectType_CLOUD,   ///< 无序点云
		ObjectType_RANGEDATA, ///< 有序点云
		ObjectType_RANGEIMAGE, ///< 深度图
		ObjectType_BASETRIMESH ///< 点面表示的三角网格 
	};

	/** \brief 网格，无序点云，有序点云的基类
	*/
	class SN3D_COMMON_API BaseData
	{
	public:
		BaseData();
		BaseData(const BaseData& from) = default;
		virtual ~BaseData();

		//////////////////////////////////////////////////////模型ID
		inline int get_id() const;
		inline void set_id(int id);

		///////////////////////////////////////////////////////模型类型
		inline ObjectType get_type() const;
		inline bool is_type(ObjectType type) const;

		//////////////////////////////////////////////////////硬盘与内存数据交换
		inline bool is_in_memory() const;
		///把数据保存在硬盘
		virtual int swap_out_data() = 0;
		///把数据从硬盘读进来
		virtual int swap_in_data() = 0;
		///设置保存内存数据的临时文件夹
		inline static void set_temp_fold(const std::wstring& tempFold);
		//得到保存内存数据的临时文件夹
		inline static const std::wstring& get_temp_flod();
        ///得到临时文件完整名称
        virtual const std::wstring get_temp_full_name();

		////////////////////////////////////////////////////////内存管理
		///清空内存
		virtual void clear() = 0;
		///把删除的数据空间释放出来
		virtual void squeeze() = 0;

        ///获取模型包围盒
        virtual void get_boundBox(Eigen::Vector3& boxMin, Eigen::Vector3& boxMax) const = 0;

		///////////////////////////////////////////////////////operator

		///验证数据是否合法
		virtual bool is_legal() const = 0;

		///修补不合法数据
		virtual void repair_illegal() = 0;

		//////////////////////////////////////////////////////数据加密
		void set_is_encryption(bool is) { _isEncryption = is; }
		bool get_is_encryption() const { return _isEncryption; }

	protected:
		ObjectType _type;
		int _id;               ///< 模型序号，整个程序唯一ID

		bool _isInMemory;     ///< 数据是否在内存
		static std::wstring _tempFold;   ///< 保存内存数据的临时文件夹

		bool _isEncryption;   ///< 数据是否加密
	};

	/////////////////////////////////////////////////////////////////函数实现
	inline int BaseData::get_id() const 
	{ 
		return _id; 
	}
	inline void BaseData::set_id(int id) 
	{ 
		_id = id; 
	}
	inline ObjectType BaseData::get_type() const 
	{ 
		return _type; 
	}
	inline bool BaseData::is_type(ObjectType type) const 
	{ 
		return (_type == type); 
	} 
	inline bool BaseData::is_in_memory() const 
	{ 
		return _isInMemory; 
	}
	inline void BaseData::set_temp_fold(const std::wstring& tempFold) 
	{    
#ifdef _WIN32 
	//如果路径不存在， 则创建;如果路径非法，暂不处理
        std::wstring temp(tempFold);
        std::vector<std::wstring> paths;
        do
        {
            int ret = ::_wmkdir(temp.c_str());
            if (0 == ret || EEXIST == ret)
            {
                break;
            }

            paths.push_back(temp);
            size_t pos = temp.find_last_of(L"\\");
            if (pos == std::wstring::npos)
            {
                break;
            }

            temp = temp.substr(0, pos);
        }while(!temp.empty());

        for (int i = (int)paths.size()-1; i>=0; --i)
        {
            ::_wmkdir(paths[i].c_str());
        }

		_tempFold = tempFold; 
#else
	std::cout << "function: BaseData::set_temp_fold only implemented in windows! Please replace '::_wmkdir' with linux api if you need it in linux" << std::endl;

#endif
	}
	inline const std::wstring& BaseData::get_temp_flod() 
	{ 
		return _tempFold; 
	}
    inline const std::wstring BaseData::get_temp_full_name() 
    { 
#ifdef _WIN32
	std::wstring path = _tempFold;
        std::wstring name =  std::to_wstring((long long)_id);
        std::wstring extension = L".tmp";
        std::wstring fullName = path +L"\\" + name + extension;
        return fullName; 
#else
       	//std::cout << "function to_(w)string is defined when __cplusplus >= 201103L && defined(_GLIBCXX_USE_C99) in linux(see basic_string.h)" <<std::endl;
	return std::wstring();
#endif
    }

}//namepsace


#endif //SN3D_ALGORITHM_COMMON_BASE_DATA_H
