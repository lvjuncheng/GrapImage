/***************************************************
 * 定义有向包围盒类以及相关函数
 * \author 张健
 ***************************************************/
#ifndef SN3D_ALGORITHM_COMMON_OBBOX_H
#define SN3D_ALGORITHM_COMMON_OBBOX_H

#include "Traits.h"
#include "RigidMatrix.h"

namespace Sn3DAlgorithm
{
	/** \brief 有向包围盒类  
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

		///获得box的顶点坐标
		Eigen::Vector3 corner(int i) const {
			Eigen::Vector3 cor;
			corner(i, cor);
			return cor;
		}

		///获得box的顶点坐标
		void corner(int i, Eigen::Vector3& cor) const{
			assert(i >= 0 && i <= 7);
			Eigen::Vector3 temp;
			temp[0] = (i&1) ? _max[0] : _min[0];
			temp[1] = (i&2) ? _max[1] : _min[1];
			temp[2] = (i&4) ? _max[2] : _min[2];
			cor = _mat*temp;

		}

		/** \brief 计算点云集合的OBBox，并保存到该类中
		 *  \param[in] points  点云集合
		 *  \param[in] weigth  点云中每个点的权重
		 */
		void compute_obbox(const std::vector<Eigen::Vector3>& points, const std::vector<Scalar>& weigth = std::vector<Scalar>());

		///根据OBBox的8个顶点坐标，计算OBBox，并保存到该类中。8个点必须按照默认顺序
		///正确运行，则返回0
		int set_obbox(const std::vector<Eigen::Vector3>& verts);

		///获得包围盒所占体积
		Scalar get_volume() const {
			Eigen::Vector3 tmp = _max - _min;
			return  std::fabs(tmp.data()[0] * tmp.data()[1] * tmp.data()[2]);
		}

		//#####################################################求交操作

		///点p是否在box中
		bool is_in_box(const Eigen::Vector3& p) const{
			Eigen::Vector3 temp = _mat.inverse()*p;
			for (int i = 0; i < 3; ++i)
			{
				if (_min[i]>temp[i] || _max[i]<temp[i])   return false;
			}
			return true;
		}

		///线段是否与包围盒相交
		bool is_intersection(const Eigen::Vector3& p1, const Eigen::Vector3& p2) const;

		///两个bbox是否相交
		bool is_intersection(const OBBox& box) const;

		///box是否为空
		bool is_empty() const{
			return (_min[0]>_max[0]) || (_min[1] > _max[1]) || (_min[2] > _max[2]);
		}

	private:
		Eigen::Vector3 _min;  ///< 包围盒局部坐标系下的最小值点
		Eigen::Vector3 _max;  ///< 包围盒局部坐标系下的最大值点
		RigidMatrix _mat;     ///< 局部坐标系到全局坐标系的变换。局部坐标系下包围盒是平行于三个轴的
	};


}//namespace

#endif //SN3D_ALGORITHM_COMMON_OBBOX_H