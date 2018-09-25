/**************************************
 * 定义刚体变换类
 * \author 张健
 **************************************/


#ifndef SN3D_ALGORITHM_COMMON_RIGID_MATRIX_H
#define SN3D_ALGORITHM_COMMON_RIGID_MATRIX_H

#include "Traits.h"

namespace Sn3DAlgorithm
{
	/** 
	 * 使用选择矩阵和平移矩阵表示刚体变换
	 */
	class SN3D_COMMON_API RigidMatrix
	{
	public:
		/////////////////////////////////////构造函数
		RigidMatrix();
		RigidMatrix(const Eigen::Matrix3d& rot, const Eigen::Vector3d& vec);
		RigidMatrix(const RigidMatrix& from) = default;
		RigidMatrix(RigidMatrix&& from) NOEXCEPT;

		//////////////////////////////////////////access and modify
		const Eigen::Matrix3d& get_rotation() const { return _rot; }
		const Eigen::Vector3d& get_translation() const { return _trans; }
		Eigen::Matrix3d& get_rotation() { return _rot; }
		Eigen::Vector3d& get_translation() { return _trans; }
		void set_rotation(const Eigen::Matrix3d& rot) { _rot = rot; }
		void set_translation(const Eigen::Vector3d& trans) { _trans = trans; }



		///////////////////////////////////////////////////operator
		RigidMatrix& operator=(const RigidMatrix& other) = default;
		RigidMatrix& operator=(RigidMatrix&& other)
		{
			_rot = std::move(other._rot);
			_trans = std::move(other._trans);
			return *this;
		}
		
		///设置初始的刚体变换
		inline void set_identity() {
			_rot.setIdentity();
			_trans.setZero();
		}

		///计算自身的逆刚体变换.
		inline void inverseSelf();

		///返回拟刚体变换
		inline RigidMatrix inverse() const;

		/** 刚体变换.
		 *  @return 返回ptCoord通过刚体变换后的新坐标
		 */
		inline Eigen::Vector3d operator* (const Eigen::Vector3d& ptCoord) const;

		/** 法向量的刚体变换.
		 *  @return 返回nrv通过刚体变换后的新坐标 
		 */
		inline Eigen::Vector3d transform_normal(const Eigen::Vector3d& nrv) const;

		/** 刚体变换.
		 *  @return 返回ptCoord通过刚体变换后的新坐标
		 */
		inline Eigen::Vector3f operator* (const Eigen::Vector3f& ptCoord) const;

		/** 法向量的刚体变换.
		 *  @return 返回nrv通过刚体变换后的新坐标
		 */
		inline Eigen::Vector3f transform_normal(const Eigen::Vector3f& nrv) const;
		inline RigidMatrix operator* (const RigidMatrix& mat) const;

		///*this = *this * mat
		inline void right_multiply(const RigidMatrix& mat);
		///*this = mat * *this
		inline void left_multiply(const RigidMatrix& mat);

		////////////////////////////////////////////////////////norm
		/// 计算刚体变换的norm值.
		double norm();
        double squaredNorm();
		 
		////计算两个刚体变换的差异
		double normLen(const RigidMatrix& mat);

		///验证数据是否合法
		bool is_legal() const;

	protected:
		Eigen::Matrix3d _rot;  ///< 旋转矩阵
		Eigen::Vector3d _trans; ///< 平移向量
	};

	////////////////////////////////////////////////////inline
	inline void RigidMatrix::inverseSelf()
	{
		_rot.transposeInPlace();
		_trans = - _rot * _trans;
	}
	inline RigidMatrix RigidMatrix::inverse() const
	{
		RigidMatrix mat;
		mat._rot = _rot.transpose();
		mat._trans = - mat._rot * _trans;
		return mat;
	}
	inline Eigen::Vector3d RigidMatrix::operator* (const Eigen::Vector3d& ptCoord) const 
	{
		return _rot * ptCoord + _trans;
	}
	inline Eigen::Vector3d RigidMatrix::transform_normal(const Eigen::Vector3d& nrv) const 
	{
		return _rot* nrv;
	}
	inline Eigen::Vector3f RigidMatrix::operator* (const Eigen::Vector3f& ptCoord) const 
	{
		return (_rot * ptCoord.cast<double>() + _trans).cast<float>();
	}
	inline Eigen::Vector3f RigidMatrix::transform_normal(const Eigen::Vector3f& nrv) const 
	{
		return (_rot * nrv.cast<double>()).cast<float>();
	}
	inline RigidMatrix RigidMatrix::operator* (const RigidMatrix& mat) const 
	{
		RigidMatrix temp;
		temp._rot = _rot * mat._rot;
		temp._trans = _rot * mat._trans + _trans;
		return temp;
	}
	inline void RigidMatrix::right_multiply(const RigidMatrix& mat)
	{
		_trans += _rot * mat._trans;
		_rot = _rot * mat._rot;
	}
	inline void RigidMatrix::left_multiply(const RigidMatrix& mat)
	{
		_trans = mat._rot * _trans + mat._trans;
		_rot = mat._rot *  _rot;
	}


	///罗德里格斯变换
	void SN3D_COMMON_API rodrigures(const Eigen::Matrix3d& rot, Eigen::Vector3d& rVec);
	void SN3D_COMMON_API rodrigures(const Eigen::Vector3d& rVec, Eigen::Matrix3d& rot);

}//namespace


#endif //SN3D_ALGORITHM_COMMON_RIGID_MATRIX_H