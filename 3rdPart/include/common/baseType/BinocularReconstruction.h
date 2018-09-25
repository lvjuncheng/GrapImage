#ifndef SN3D_ALGORITHM_COMMON_BASE_BINRECON_H
#define SN3D_ALGORITHM_COMMON_BASE_BINRECON_H

#include <vector>
#include "Traits.h"
#include "CameraCalibParam.h"

namespace Sn3DAlgorithm
{
	class SN3D_COMMON_API BinocularReconstruction
	{
	public:
		BinocularReconstruction();
		/*!
		* @brief  载入标定数据，输入标定数据，标定数据以文本格式存放,并计算基础矩阵
		* @date  	2018/4/4
		* @param [in]	    leftCamParPath	  	                              左相机标定数据路径 
		* @param [in]	    rightCamParPath	  	                              右相机标定数据路径
		* @param [in]	    caliPlanXId	  	                                  表明了多幅深度图像上哪些X轴是需要计算的
		* @param [in]	    cameraResolution	  	                          左相机分辨率
		* @param [in]	    projectResolution	  	                          右投影机分辨率
		*/
		RetVal generate_look_table(const std::string&leftCamParPath , const std::string&rightCamParPath
		, const Eigen::Vector2i&cameraResolution , const Eigen::Vector2i&projectResolution);

		
		/*!
         * @brief 求极线方程 
		* @param[in]	    coor             左图上的图像坐标
		* @return	    	  	             极线方程
		*/
		inline Eigen::Vector3d count_pole_lines(const Eigen::Vector2f& coor)
		{
			Eigen::Vector3d lineEqu(coor[0], coor[1], 1);
			lineEqu =  _fundMentalMatrix * lineEqu;
			return lineEqu;   
		}

		
		
		/*!
		* @brief  找到对应点之后三角化，得到三维重建结果,原理为计算公垂线。
		* @date  	2018/4/4
		* @param [in]	    caliPlanXId	  	                                  需要拟合的光平面的X轴坐标
		* @param [in]	    lookTable	  	                                  查找表拟合结果
		*/
		Eigen::Vector3d triangulation(const Eigen::Vector2d&leftMatch , const Eigen::Vector2d&rightMatch);


		/*!
		* @brief  找到对应点之后三角化,利用投影齐次方程解。同时输入多个对应点解得多个点
		* @date  	2018/4/4
		* @param [in]	    leftMatchSet	  	                                  左图像匹配
		* @param [in]	    rightMatchSet	  	                                  查找表拟合结果
		*/
		bool BinocularReconstruction::triangulation_pointset(const std::vector<Eigen::Vector2d>&leftMatchSet, const std::vector<Eigen::Vector2d>&rightMatchSet, std::vector<Eigen::Vector3d>&resultPoint);

		//返回基础矩阵
		const Eigen::Matrix3d& get_foundation_matrix() const
		{
			return _fundMentalMatrix;
		}

		CameraCalibParam&get_leftCamPar()
		{
			return _leftCamPar;
		}

		const CameraCalibParam&get_leftCamPar() const
		{
			return _leftCamPar;
		}
		 
		CameraCalibParam&get_rightCamPar()
		{
			return _rightCamPar;
		}

		const CameraCalibParam&get_rightCamPar() const
		{
			return _rightCamPar;
		}

		Eigen::Matrix3d&get_fundmentalMatrix()
		{
			return _fundMentalMatrix;
		}
		
		const Eigen::Matrix3d&get_fundmentalMatrix() const 
		{
			return _fundMentalMatrix;
		}
		
		RigidMatrix& get_rtLeftCorrIntoRightCorr()
		{
			return _rtLeftCorrIntoRightCorr;
		}

		const RigidMatrix& get_rtLeftCorrIntoRightCorr() const
		{
			return _rtLeftCorrIntoRightCorr;
		}


	private:

		/*!
		* @brief  根据读取左右相机标定结果，生成双目系统基础矩阵。
		* @date  	2018/4/4
		*/
		void generate_fundmental_matrix();

		CameraCalibParam _leftCamPar;///<左相机参数
		CameraCalibParam _rightCamPar;///<右相机参数
		Eigen::Matrix3d _fundMentalMatrix;///<双目系统基础矩阵
		std::vector<std::vector<Eigen::Vector4d>>_kIdFitResult;///<极线和插值源之间的拟合关系，查找表。
		RigidMatrix _rtLeftCorrIntoRightCorr;///<左相机坐标系变换到右相机坐标系下的变换
		Eigen::Matrix<double, 3, 4> _rightProject;///<Kr*[R|T]用于三角化求解，包括坐标系变换以及右相机矩阵。
		Eigen::Matrix<double, 3, 4> _leftProject;///<Kl[R|T]用于三角化求解，包括坐标系变换以及左相机矩阵。
	};
}//namespace Sn3DAlgorithm
#endif