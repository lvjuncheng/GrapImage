/*************************************************
* \brief ���ļ�����ù��̵������㷨�ӿ�
* \note  �ù�����Ҫ�����ڼ�¼����ؽ��õ��ĵ㣬��������ת��������
* \author ���ڽ�
**************************************************/


#ifndef SN3D_ALGORITHM_COMMON_BASE_POINTIMAGE_H
#define SN3D_ALGORITHM_COMMON_BASE_POINTIMAGE_H

#include "Cloud.h"
#include "RigidMatrix.h"
#include "Image.h"
//#include "printError.h"

namespace Sn3DAlgorithm
{
	/** \brief ���ͼ��
	 *  \note  �����ڵĵ㣬����������ɫ��(0,0,0)����ʾ
	 */
	class SN3D_COMMON_API PointImage : public Cloud
	{
	public:
		PointImage() :_imageHeight(0), _imageWidth(0)
		{
			_rtGlobal.set_identity();
			_hasWeight = false;
            std::vector<float>().swap(_weight);
            _sampleX = _sampleY = 0;
			_cameraInParam.set_param(0, 0, 0, 0);
		}
		
		/**  \brief ���ص�ͼ�Ƿ����Ȩ������
		*/
		inline bool is_enable_vWeight() const
		{ return _hasWeight;} 

		/**  \brief ʹ�õ�ͼ���ϸ���Ȩ�ص�����,���ҽ�������֮ǰ���е�ֵ��գ���֮ǰһ�������������֮ǰ�Ѿ���õ�ֵ�ᱻ���
		*/
		void enable_vWeight();

		/**  \brief �رոõ�ͼ����Ȩ�ص�����
		*/
		void disable_vWeight();

		/**  \brief �������ŵ�ͼ�е���Ч����
		*/
		int count_vaildNum() const;

		/** \brief �������ŵ�ͼ������
		*   \param[out] cloudFocuse                   ��ͼ������
		*/
		void count_focuse(Eigen::Vector3&cloudFocuse) const;


		/**  \brief ���㵥Ƭ����Ч������Լ���Ƭ�������
		*   \param[out] cloudFocuse                   ��ͼ������
		*   \param[out] vaildNum                      ��ͼ����Ч�����
		*/
		void count_focuse_vaildNum(int&vaildNum, Eigen::Vector3&cloudFocuse) const;

		/**  \brief �����ʼ������ֻ����ռ䡣������ֵ
		*   \param[in] x                             ��ͼ�Ŀ�
		*   \param[in] y                             ��ͼ�ĸ�
		*   \param[in] camParInput                   �ؽ����ŵ�ͼ������ڲ���
		*   \param[in] sample                        ��ͼ�Ĳ����ʣ�Ĭ��X��Y����Ĳ���������ͬ��
		*/
		void ini(const int x, const int y, const CameraInParam&camParInput, const int sample);


		/**  \brief �����ʼ������ֻ����ռ䡣������ֵ
		*   \param[in] x                             ��ͼ�Ŀ�
		*   \param[in] y                             ��ͼ�ĸ�
		*   \param[in] camParInput                   �ؽ����ŵ�ͼ������ڲ���
		*   \param[in] sampleX                       ��ͼ�Ĳ����ʣ���ͼ��X�����ϵĲ�����
		*   \param[in] sampleY                       ��ͼ�Ĳ����ʣ���ͼ��Y�����ϵĲ�����
		*/
		void ini(const int x, const int y, const CameraInParam&camParInput, const int sampleX , const int sampleY);

		/**  \brief ֻ��ʼ����ͼ��С�������ʺ�����������Ժ�����ֵ
		*   \param[in] x                             ��ͼ�Ŀ�
		*   \param[in] y                             ��ͼ�ĸ�
		*/
		void ini(const int x, const int y);

		/**  \brief ����ĵ�ͷ�����Ϊ0
		*/
		void setZero();

		/**  \brief ���޷����㷨�ߵĵ�ȫ����0
		*/
		void delete_point_without_normal();

		/**  \brief ����ͼ�任��ԭʼ�������ϵ
		*/
		void to_ori_coor();


		/**  \brief ���µ�ķ�����ʹ�ü���Eigen�ķ�ʽ��ʹ��tbb����,Ϊ��֤���������ȷ�����뱣֤��ǰ���������ϵ����ȷ��set_rt����
		*   \param[in] range                                ����ʱ���ƽ��������С
		*   \param[in] vaildThread                          ����ʱ�����ڲ���Ч��������ڸ���ֵ�ſɼ���
		*   \param[in] depthDiff                            �ò�����Ϊ�����ƽ����ȡ����ʱ�����ĵ�������Z����Ĳ��Ӧ��С��depthDiff�����ܲ������
		*/
		void update_VNrv(const int range, const int vaildThread , const float depthDiff);


		/**  \brief ���ڶԵ�ǰ����и���任��ֻ����Ч����С���ν��Ч�㣨abs(point��0))<1e-5 && abs(point(1)) < 1e-5 && abs(point(2) , 1e-5��
		*   \param[in] rt                            �Ե���������任ʱ��rt
		*/
		virtual void rigid_transformation(const RigidMatrix& rt);


		/**  \brief ��ȡ��ǰ�������ڵ�����ϵ
		*/
		inline const RigidMatrix&get_rt() const{ return _rtGlobal;}


		/**  \brief �ҵ���ͼ�ϵı�Ե�㣬���ҽ���Щ��Ե��flag���Ϊtrue����Щ��Ե�㲻���ϸ������ϵı�Ե�㣬����ͼ���ڲ���С����������
		*   \param[in] range                        ��ʾ�ҵ���Щ���range������
		*   \param[in] disThread                    �ж��Ƿ�����ľ�����ֵ��
		*   \param[in] pointNumThread               ��Ե������ֵ����Ե���ȴ������ֵ�ű��ж�Ϊ��Ե��
		*/
		void find_egde_point(const int range, const float disThread , const int pointNumThread);

		/**  \brief ��ȡ���ŵ�ͼ�Ŀ��
		*/
		inline const int get_image_width() const { return _imageWidth; }

		/**  \brief ��ȡ���ŵ�ͼ�ĸ߶�
		*/
		inline const int get_image_height() const{ return _imageHeight; }

		/**  \brief ������ĵ�ͼ������ԭʼ�������ϵ֮��ʱ����Ҫ���ⲿ���뵱ǰ�����ڵ�rt
		*   \param[in] rt                            ��ǰ�����ڵ�����ϵ�����ȫ������ϵ��rt
		*/
		void set_rt(const RigidMatrix& rt){ _rtGlobal = rt; }

		/**  \brief ��ȡid = Y*_imageWidth + X�ĵ�
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline const Eigen::Vector3&get_point(const int x, const int y) const
		{
			int id = y*_imageWidth + x;
			return(get_vCoord(id));
		}

		/**  \brief ��ȡid = Y*_imageWidth + X�ĵ�
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline Eigen::Vector3&get_point(const int x, const int y)
		{
			int id = y*_imageWidth + x;
			return(get_vCoord(id));
		}


		/**  \brief ��ȡid = Y*_imageWidth + X�ķ���
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline const Eigen::Vector3&get_normal(const int x, const int y) const
		{
			int id = y*_imageWidth + x;
			return(get_vNrv(id));
		}

		/**  \brief ��ȡid = Y*_imageWidth + X�ķ���
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline Eigen::Vector3&get_normal(const int x, const int y)
		{
			int id = y*_imageWidth + x;
			return(get_vNrv(id));
		}

		/**  \brief ��ȡid = Y*_imageWidth + X����ɫ
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline const Eigen::Vector3&get_color(const int x, const int y) const
		{
			int id = y*_imageWidth + x;
			return get_vColor(id);
		}

		/**  \brief ��ȡid = Y*_imageWidth + X�ĵ��Ȩ��
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline float get_vWeight(const int x , const int y) const
		{
			int id = y*_imageWidth + x;
			return _weight[id];
		}

		inline float get_vWeight(const int id) const
		{
			return _weight[id];
		}

		/**  \brief ����һ�ݵ�vWeight
		*/
		inline std::vector<float>&get_vWeights()
		{
			return _weight;
		}

		inline const std::vector<float>& get_vWeights() const
		{
			return _weight;
		}

		/**  \brief ����id = Y*_imageWidth + X�ĵ��Ȩ��
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline void set_vWeight(const int x , const int y , const float value)
		{
			int id = y*_imageWidth + x;
			_weight[id] = value;
		}

		inline void set_vWeight(const int id , const float value)
		{
			_weight[id] = value;
		}

		/**  \brief һ�����������õ�ͼȨ��
		*/
		inline void set_vWeights(const std::vector<float>&sourceData)
		{
			enable_vWeight();
			if (sourceData.size() != _imageWidth*_imageHeight)
			{
				std::cout << "wrong input because sourceData.size() != _imageWidth*_imageHeight" << std::endl;
				return;
			}
			_weight = sourceData;
		}

		/**  \brief ��ȡid = Y*_imageWidth + X����ɫ
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*/
		inline Eigen::Vector3&get_color(const int x, const int y)
		{
			int id = y*_imageWidth + x;
			return get_vColor(id);
		}


		/**  \brief ����id = Y*_imageWidth + X�ĵ�
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*   \param[in] point                         ����ĵ�
		*/
		inline void set_point(const int x, const int y, const Eigen::Vector3&point)
		{
			int id = y*_imageWidth + x;
			set_vCoord(id, point);
		}

		/**  \brief ����id = Y*_imageWidth + X�ķ���
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*   \param[in] normal                        ����ķ���
		*/
		inline void set_normal(const int x, const int y, const Eigen::Vector3&normal)
		{
			int id = y*_imageWidth + x;
			set_vNrv(id, normal);
		}

		/**  \brief ����id = Y*_imageWidth + X����ɫ
		*   \param[in] x                             �õ���ͼƬ��X���λ��
		*   \param[in] y                             �õ���ͼƬ��Y���λ��
		*   \param[in] color                         �������ɫ
		*/
		inline void set_color(const int x, const int y, const Eigen::Vector3&color)
		{
			int id = y*_imageWidth + x;
			set_vColor(id, color);
		}

		/**  \brief ���㷨���tbb�ӿڣ����ɵ���
		*   \param[in] range                                ��ȡ����ʱ�����ƽ�������С
		*   \param[in] pN                                   ��������λ���ϵ�id
		*   \param[in] vaildThread                          ����ʱ�����ڲ���Ч��������ڸ���ֵ�ſɼ���
		*   \param[in] depthDiff                            �ò�����Ϊ�����ƽ����ȡ����ʱ�����ĵ�������Z����Ĳ��Ӧ��С��depthDiff�����ܲ������
		*/
		void countNormal_eigen_tbb(const int range, const int pN, const float depthDiff, const int vaildThread , const std::vector<int>&vaildPointSet);

		/**  \brief ��ȡ��ǰ��ͼ�豸�ڲ�
		*/
		inline const CameraInParam&get_camPar() const{ return _cameraInParam;}

		/**  \brief ���õ�ǰ��ͼ�豸�ڲ�
		*   \param[in] camearInPar                          �������õ�ǰ��ͼ������ڲ�
		*/
		void set_camPar(const CameraInParam camearInPar)
		{
			_cameraInParam = camearInPar;
		}

		/**  \brief ��ȡ��x��y������غ͹��ߵ��ߵķ��򣬸�����δ��һ��������Z����Ϊ1��
		*   \param[in] x                                     ����x��������
		*   \param[in] y                                     ����y��������
		*/
		inline Eigen::Vector3 get_ray_direct(const int x, const int y) const
		{
			float _viewRadialX = get_viewRadialX(); float _viewRadialY = get_viewRadialY();
			return(Eigen::Vector3((x + _viewRadialX - _cameraInParam.get_cx()) / _cameraInParam.get_fx(), (y + _viewRadialY - _cameraInParam.get_cy()) / _cameraInParam.get_fy(), 1));
		}

		/**  \brief ��ȡ��x��y������غ͹��ߵ��ߵķ��򣬸�����δ��һ��������Z����Ϊ1��
		*   \param[in] pointId                               �õ�������������е�Id
		*/
		inline Eigen::Vector3 get_ray_direct(const int pointId) const
		{
			int y = pointId / _imageWidth; int x = pointId - y*_imageWidth;
			float _viewRadialX = get_viewRadialX(); float _viewRadialY = get_viewRadialY();
			return(Eigen::Vector3((x + _viewRadialX - _cameraInParam.get_cx()) / _cameraInParam.get_fx(), (y + _viewRadialY - _cameraInParam.get_cy()) / _cameraInParam.get_fy(), 1));
		}
		inline int get_sampleX() const 
		{
			return _sampleX;
		}

		inline int get_sampleY() const
		{
			return _sampleY;
		}
		
		/**  \brief �Ծ���Ϊԭ��ɾ�������������еĹµ�
		*   \param[in] findRange                               ��������Ĵ�С
		*   \param[in] disThread                               ������ֵ
		*   \param[in] numThread                               �жϹµ�����ֵ���µ��е����С�ڸ�ֵ����Ϊ�ǹµ�
		*   \                                                  �����ֵΪ0���ʾֻ���������Ǹ�����
		*/
		void delete_island(const int findRange , const float disThread , const int numThread);


		/**  \brief �����Ϊԭ��ɾ�������������еĹµ�
		*   \param[in] findRange                               ��������Ĵ�С
		*   \param[in] disThread                               ��Ⱦ�����ֵ
		*   \param[in] numThread                               �жϹµ�����ֵ���µ��е����С�ڸ�ֵ����Ϊ�ǹµ�
		*   \                                                  �����ֵΪ0���ʾֻ���������Ǹ�����
		*/
		void delete_island_through_depth(const int findRange, const float disThread, const int numThread , Eigen::Vector3 deleteResult = Eigen::Vector3(0 ,0 ,0));

		/**  \brief ������ǵĵ���Ϊ0
		*/
		void delete_point_flaged(const Eigen::Vector3 deleteResult = Eigen::Vector3(0, 0, 0));
		

		inline float get_viewRadialX() const{ return 0.5 / _sampleX; }
		inline float get_viewRadialY() const{ return 0.5 / _sampleY; }

		/**  \brief ����ĳ��ָ����ķ���
		*   \param[in] imageX                                   ĳ�����X����
		*   \param[in] imageY                                   ĳ�����Y����
		*   \param[in] vaildThread                              ��������Ч���������������С�ڸ�ֵ�򲻼����䷨��
		*   \param[in] depthDiff                                ��������Ч��֮�����Ȳ������ڸ�ֵ����Ϊ�õ������
		*   \param[in] range                                    ��Ϸ���Ĵ��ڰ뾶
		*/
		void count_point_normal(const int imageX, const int imageY, const int range, const int vaildThread, const float depthDiff);

		inline void set_sampleX(const int sampleX)
		{
			_sampleX = sampleX;
		}

		inline void set_sampleY(const int sampleY)
		{
			_sampleY = sampleY;
		}

		
		/**  \brief ��pointImage����õ��Ľ����һ��ͼƬ��չʾ����������ͼƬ���������ڼ������ŵ�ͼ��ͼƬ,ͼƬ������float����
		*   \param[in] Image                                    �����ͼ��ԭʼͼƬ
		*/
		void draw_pointImage_on_pic(const float*sourceImage , const std::string&outputpath);


		/**  \brief �ж���Ƭ�������ж��ٰٷֱȵĵ�����һ������ƽ��
		*   \param[in] planeRata                                   ĳ��ƽ���ϵ��Ƶİٷֱ�ռ��
		*   \param[in] planeThreshold                              �ж�һ�����Ƿ���һ��ƽ���ϵľ�����ֵ
		*/
		void plane_detect(float&planeRata , const float planeThreshold);

 	private:

		/**  \brief �����Ϊԭ��ɾ�������������еĹµ�
		*   \param[in] findRange                               ��������Ĵ�С
		*   \param[in] disThread                               ��Ⱦ�����ֵ
		*   \param[in] numThread                               �жϹµ�����ֵ���µ��е����С�ڸ�ֵ����Ϊ�ǹµ�
		*   \                                                  �����ֵΪ0���ʾֻ���������Ǹ�����
		*   \param[in] isDepth                                 �Ƿ�ͨ����ȵĲ�ֵ���ж�������Ƿ�Ϊһ���ŵ�
		*/
		void delete_core_function(const int findRange, const float disThread, const int numThread, const bool isDepth , const Eigen::Vector3&deleteresult);


		/**  \brief ɾ���µ����׺���
		*   \param[in] pointIdSet                              �ñ�����¼�˽���ɾ���ĵ��id
		*   \param[in] imageId                                 ��ǰ��id
		*   \param[in] windowRange                             ���������İ뾶 ��������N*N�Ĵ���
		*   \param[in] disThread                               �жϵ��Ƿ�Ϊ����ľ�����ֵ
		*   \param[in] isDepth                                 �Ƿ�ͨ����ȵĲ�ֵ���ж�������Ƿ�Ϊһ���ŵģ�������������ֱ���õ�֮��ľ�����Ϊ�ж���ֵ
		*/
		void get_point_around(std::vector<int>&pointIdSet, const int imageId, const int windowRange, const float disThread , const bool isDepth);

		/**  \brief �����ҳ������Ǹ��������ҽ������ĵ��ĵ㶼ɾ���ĺ���
		*   \param[in] islandRecord                            ���ڼ�¼�ı���
		*/
		void save_max_island(const std::vector<std::vector<int> >&islandRecord);

		/**  \brief ȷ���ñ�Ե�Ƿ�Ϊ����Ե
		*   \param[in] idX                                     ��ǰ���X����
		*   \param[in] idY                                     ��ǰ���Y����
		*   \param[in] disThread                               �ж��Ƿ�Ϊ����ľ�����ֵ
		*/
		bool is_edge(const Eigen::Vector3&pointCenter , const int idX, const int idY, const float disThread);


		/**  \brief ���ڲ��ұ�Ե�ĵ����ͳ�Ƶĺ���
		*   \param[in] idRecord                                ��¼����һ�ű�Ե��id��vector
		*   \param[in] pointId                                 ��ǰ�������ID
		*/
		void get_edge_around(std::vector<int>&idRecord , const int pointId);

 		int _imageWidth;///< ���ͼ�Ŀ��
 		int _imageHeight;///< ���ͼ�ĸ߶�
 		RigidMatrix _rtGlobal;///< ��ǰ���Ƶ��µ�����ϵ��RT���ֲ�����ϵ��ȫ������ϵ��RT
		CameraInParam _cameraInParam;///< �ؽ���ǰƬ���������ڲ���
		int _sampleX;///<���ŵ�ͼ��X�����ϵĲ�����
		int _sampleY;///<���ŵ�ͼ��Y�����ϵĲ�����
		bool _hasWeight;///<��ͼ�Ƿ����Ȩ�����Եı�����
		std::vector<float>_weight;///<�������Ӧ��Ȩ��
	};
}//namespace Sn3DAlgorithm

#endif//SN3D_ALGORITHM_COMMON_BASE_DEPTH
