/***********************************************
 * ��������ģ�͵Ĵ�ӡ·����
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_ENTITY_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_ENTITY_H

#include <vector>
#include "Layer.h"
#include <iostream>

namespace Sn3DAlgorithm
{

	///ö�ٹ��յļ�������
	enum SN3D_PRINT3D_COMMON_API CraftType
	{
		CraftType_DEFAULT,
		CraftType_FDM, ///>�ۻ�����
		CraftType_SLA, ///>����̻�����
		CraftType_SLS, ///>�����ս����
	};

	///����ģ�͵Ĵ�ӡ·����
	class SN3D_PRINT3D_COMMON_API Entity
	{
	public:
		typedef std::shared_ptr<Entity> Ptr;
		typedef std::shared_ptr<const Entity> ConstPtr;
		//�ϲ����ʵ��
		static RetVal merge_entities(const std::vector<Entity::ConstPtr>& entities, Entity::Ptr merged);
		/** \brief ���ļ�����entity
		*  \param[in]     filename		�ļ�·��
		*  \param[out]     entity		entity
		*/
		SN3D_PRINT3D_COMMON_API friend std::istream& operator >> (std::istream& is, Entity& entity);
		/** \brief entityд�ļ�
		*  \param[in]     filename		�ļ�·��
		*  \param[in]     entity		entity
		*/
		SN3D_PRINT3D_COMMON_API friend std::ostream& operator << (std::ostream& os, const Entity& entity);
		//////////////////////////////////////////////////////////���캯������������
		Entity();
		virtual ~Entity();

		/////////////////////////////////////////////////////////����_layers
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

		///////////////////////////////////////////////////////////////////////����_craftType
		void set_craft_type(const CraftType& ct) {_craftType = ct;}
		CraftType get_craft_type() const {return _craftType;}

		/** \brief entity�����ز�����
		*  \param[in]      startClearCount	�����0��ʼ��һ��startClearCount����
		*  \param[in]     intervalCount		���������������0��ʾ�����
		*/
		void clear_layers_every(int startClearCount, int intervalCount);
	protected:
		std::vector<Layer::Ptr> _layers; ///< ÿһ��Ĵ�ӡ·��
		CraftType _craftType; ///>��������
		Scalar _platformHeight; ///>��ӡƽ̨��׼�߶�(�ų�������ĸ߶�)
	};

}//namespace Sn3DAlgorithm


#endif //SN3D_ALGORITHM_PRINT3D_COMMON_ENTITY_H