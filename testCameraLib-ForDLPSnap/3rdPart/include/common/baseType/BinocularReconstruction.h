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
		* @brief  ����궨���ݣ�����궨���ݣ��궨�������ı���ʽ���,�������������
		* @date  	2018/4/4
		* @param [in]	    leftCamParPath	  	                              ������궨����·�� 
		* @param [in]	    rightCamParPath	  	                              ������궨����·��
		* @param [in]	    caliPlanXId	  	                                  �����˶�����ͼ������ЩX������Ҫ�����
		* @param [in]	    cameraResolution	  	                          ������ֱ���
		* @param [in]	    projectResolution	  	                          ��ͶӰ���ֱ���
		*/
		RetVal generate_look_table(const std::string&leftCamParPath , const std::string&rightCamParPath
		, const Eigen::Vector2i&cameraResolution , const Eigen::Vector2i&projectResolution);

		
		/*!
         * @brief ���߷��� 
		* @param[in]	    coor             ��ͼ�ϵ�ͼ������
		* @return	    	  	             ���߷���
		*/
		inline Eigen::Vector3d count_pole_lines(const Eigen::Vector2f& coor)
		{
			Eigen::Vector3d lineEqu(coor[0], coor[1], 1);
			lineEqu =  _fundMentalMatrix * lineEqu;
			return lineEqu;   
		}

		
		
		/*!
		* @brief  �ҵ���Ӧ��֮�����ǻ����õ���ά�ؽ����,ԭ��Ϊ���㹫���ߡ�
		* @date  	2018/4/4
		* @param [in]	    caliPlanXId	  	                                  ��Ҫ��ϵĹ�ƽ���X������
		* @param [in]	    lookTable	  	                                  ���ұ���Ͻ��
		*/
		Eigen::Vector3d triangulation(const Eigen::Vector2d&leftMatch , const Eigen::Vector2d&rightMatch);


		/*!
		* @brief  �ҵ���Ӧ��֮�����ǻ�,����ͶӰ��η��̽⡣ͬʱ��������Ӧ���ö����
		* @date  	2018/4/4
		* @param [in]	    leftMatchSet	  	                                  ��ͼ��ƥ��
		* @param [in]	    rightMatchSet	  	                                  ���ұ���Ͻ��
		*/
		bool BinocularReconstruction::triangulation_pointset(const std::vector<Eigen::Vector2d>&leftMatchSet, const std::vector<Eigen::Vector2d>&rightMatchSet, std::vector<Eigen::Vector3d>&resultPoint);

		//���ػ�������
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
		* @brief  ���ݶ�ȡ��������궨���������˫Ŀϵͳ��������
		* @date  	2018/4/4
		*/
		void generate_fundmental_matrix();

		CameraCalibParam _leftCamPar;///<���������
		CameraCalibParam _rightCamPar;///<���������
		Eigen::Matrix3d _fundMentalMatrix;///<˫Ŀϵͳ��������
		std::vector<std::vector<Eigen::Vector4d>>_kIdFitResult;///<���ߺͲ�ֵԴ֮�����Ϲ�ϵ�����ұ�
		RigidMatrix _rtLeftCorrIntoRightCorr;///<���������ϵ�任�����������ϵ�µı任
		Eigen::Matrix<double, 3, 4> _rightProject;///<Kr*[R|T]�������ǻ���⣬��������ϵ�任�Լ����������
		Eigen::Matrix<double, 3, 4> _leftProject;///<Kl[R|T]�������ǻ���⣬��������ϵ�任�Լ����������
	};
}//namespace Sn3DAlgorithm
#endif