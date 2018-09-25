/***************************************************
 * ���������Χ�����Լ���غ���
 * \author �Ž�
 ***************************************************/
#ifndef SN3D_ALGORITHM_COMMON_OBBOX_H
#define SN3D_ALGORITHM_COMMON_OBBOX_H

#include "Traits.h"
#include "RigidMatrix.h"

namespace Sn3DAlgorithm
{
	/** \brief �����Χ����  
	 *         2____________3
	 *         /|          /|
	 *        / |         / |         
	 *   	6/__|________/7 |        
	 *       |  |        |  |        
	 *       | 0---------|--/ 1      
	 *       | /         | /        
	 *       |/          |/         
	 *       -------------
	 *       4            5
	 * 
	 */
	class SN3D_COMMON_API OBBox
	{
	public:
		OBBox() { _min.setZero(), _max.setZero(), _mat.set_identity(); }

		void set_bounding_box(const Eigen::Vector3& min, const Eigen::Vector3& max) {
			_min = min;
			_max = max;
		}
		void get_bounding_box(Eigen::Vector3& min, Eigen::Vector3& max) const {
			min = _min;
			max = _max;
		}

		RigidMatrix& get_mat() { return _mat;  }
		const RigidMatrix& get_mat() const { return _mat;  }

		///���box�Ķ�������
		Eigen::Vector3 corner(int i) const {
			Eigen::Vector3 cor;
			corner(i, cor);
			return cor;
		}

		///���box�Ķ�������
		void corner(int i, Eigen::Vector3& cor) const{
			assert(i >= 0 && i <= 7);
			Eigen::Vector3 temp;
			temp[0] = (i&1) ? _max[0] : _min[0];
			temp[1] = (i&2) ? _max[1] : _min[1];
			temp[2] = (i&4) ? _max[2] : _min[2];
			cor = _mat*temp;

		}

		/** \brief ������Ƽ��ϵ�OBBox�������浽������
		 *  \param[in] points  ���Ƽ���
		 *  \param[in] weigth  ������ÿ�����Ȩ��
		 */
		void compute_obbox(const std::vector<Eigen::Vector3>& points, const std::vector<Scalar>& weigth = std::vector<Scalar>());

		///����OBBox��8���������꣬����OBBox�������浽�����С�8������밴��Ĭ��˳��
		///��ȷ���У��򷵻�0
		int set_obbox(const std::vector<Eigen::Vector3>& verts);

		///��ð�Χ����ռ���
		Scalar get_volume() const {
			Eigen::Vector3 tmp = _max - _min;
			return  std::fabs(tmp.data()[0] * tmp.data()[1] * tmp.data()[2]);
		}

		//#####################################################�󽻲���

		///��p�Ƿ���box��
		bool is_in_box(const Eigen::Vector3& p) const{
			Eigen::Vector3 temp = _mat.inverse()*p;
			for (int i = 0; i < 3; ++i)
			{
				if (_min[i]>temp[i] || _max[i]<temp[i])   return false;
			}
			return true;
		}

		///�߶��Ƿ����Χ���ཻ
		bool is_intersection(const Eigen::Vector3& p1, const Eigen::Vector3& p2) const;

		///����bbox�Ƿ��ཻ
		bool is_intersection(const OBBox& box) const;

		///box�Ƿ�Ϊ��
		bool is_empty() const{
			return (_min[0]>_max[0]) || (_min[1] > _max[1]) || (_min[2] > _max[2]);
		}

	private:
		Eigen::Vector3 _min;  ///< ��Χ�оֲ�����ϵ�µ���Сֵ��
		Eigen::Vector3 _max;  ///< ��Χ�оֲ�����ϵ�µ����ֵ��
		RigidMatrix _mat;     ///< �ֲ�����ϵ��ȫ������ϵ�ı任���ֲ�����ϵ�°�Χ����ƽ�����������
	};


}//namespace

#endif //SN3D_ALGORITHM_COMMON_OBBOX_H