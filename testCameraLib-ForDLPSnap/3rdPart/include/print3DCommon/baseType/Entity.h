/***********************************************
 * 定义整个模型的打印路径类
 * \author 张健
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_ENTITY_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_ENTITY_H

#include <vector>
#include "Layer.h"
#include <iostream>

namespace Sn3DAlgorithm
{

	///枚举工艺的几种类型
	enum SN3D_PRINT3D_COMMON_API CraftType
	{
		CraftType_DEFAULT,
		CraftType_FDM, ///>熔积成型
		CraftType_SLA, ///>激光固化成型
		CraftType_SLS, ///>激光烧结成型
	};

	///整个模型的打印路径类
	class SN3D_PRINT3D_COMMON_API Entity
	{
	public:
		typedef std::shared_ptr<Entity> Ptr;
		typedef std::shared_ptr<const Entity> ConstPtr;
		//合并多个实体
		static RetVal merge_entities(const std::vector<Entity::ConstPtr>& entities, Entity::Ptr merged);
		/** \brief 从文件导入entity
		*  \param[in]     filename		文件路径
		*  \param[out]     entity		entity
		*/
		SN3D_PRINT3D_COMMON_API friend std::istream& operator >> (std::istream& is, Entity& entity);
		/** \brief entity写文件
		*  \param[in]     filename		文件路径
		*  \param[in]     entity		entity
		*/
		SN3D_PRINT3D_COMMON_API friend std::ostream& operator << (std::ostream& os, const Entity& entity);
		//////////////////////////////////////////////////////////构造函数和析构函数
		Entity();
		virtual ~Entity();

		/////////////////////////////////////////////////////////操作_layers
		size_t get_layer_size() const { return _layers.size(); }
		std::vector<Layer::Ptr>& get_layers() { return _layers; }
		Layer::ConstPtr operator()(size_t id) const {
			assert(get_layer_size() > id);
			return _layers[id];
		}
		Layer::Ptr operator()(size_t id) {
			assert(get_layer_size() > id);
			return _layers[id];
		}
		Layer::ConstPtr get_layer(size_t id) const{
			assert(get_layer_size() > id);
			return _layers[id];
		}
		Layer::Ptr get_layer(size_t id) {
			assert(get_layer_size() > id);
			return _layers[id];
		}
		void reserve_layer_size(size_t num) { _layers.reserve(num); }
		void resize_layer_size(size_t num) { _layers.resize(num); }

		void set_layers(const std::vector<Layer::Ptr>& layers) { _layers = layers; }
		void add_layer(const Layer::Ptr& layer) { _layers.push_back(layer);}
		void add_layer_at_front(const Layer::Ptr& layer) {_layers.insert(_layers.begin(), layer);}
		void set_layer(const Layer::Ptr& layer, int idx) {_layers.at(idx) = layer;}

		Scalar get_platform_height() const {return _platformHeight;}
		void set_platform_height(Scalar v) {_platformHeight = v;}

		///////////////////////////////////////////////////////////////////////操作_craftType
		void set_craft_type(const CraftType& ct) {_craftType = ct;}
		CraftType get_craft_type() const {return _craftType;}

		/** \brief entity清除相关层数据
		*  \param[in]      startClearCount	清除从0开始的一共startClearCount个层
		*  \param[in]     intervalCount		间隔几层进行清除，0表示不清除
		*/
		void clear_layers_every(int startClearCount, int intervalCount);
	protected:
		std::vector<Layer::Ptr> _layers; ///< 每一层的打印路径
		CraftType _craftType; ///>工艺类型
		Scalar _platformHeight; ///>打印平台基准高度(排除基座后的高度)
	};

}//namespace Sn3DAlgorithm


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_ENTITY_H