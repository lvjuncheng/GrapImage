/*************************************************
* \brief 该文件定义该工程的所有算法接口
* \note  该工程主要是用于记录相机重建得到的点，用于坐标转换和求法向
* \author 黄磊杰
**************************************************/


#ifndef SN3D_ALGORITHM_COMMON_BASE_POINTIMAGE_H
#define SN3D_ALGORITHM_COMMON_BASE_POINTIMAGE_H

#include "Cloud.h"
#include "RigidMatrix.h"
#include "Image.h"
//#include "printError.h"

namespace Sn3DAlgorithm
{
	/** \brief 深度图类
	 *  \note  不存在的点，法向量，颜色用(0,0,0)来表示
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
		
		/**  \brief 返回点图是否带有权重属性
		*/
		inline bool is_enable_vWeight() const
		{ return _hasWeight;} 

		/**  \brief 使该点图带上各点权重的属性,并且将该属性之前所有的值清空，用之前一定看清楚，否则之前已经算好的值会被清空
		*/
		void enable_vWeight();

		/**  \brief 关闭该点图各点权重的属性
		*/
		void disable_vWeight();

		/**  \brief 计算这张点图中的有效点数
		*/
		int count_vaildNum() const;

		/** \brief 计算这张点图的重心
		*   \param[out] cloudFocuse                   点图的重心
		*/
		void count_focuse(Eigen::Vector3&cloudFocuse) const;


		/**  \brief 计算单片点有效点个数以及该片点的重心
		*   \param[out] cloudFocuse                   点图的重心
		*   \param[out] vaildNum                      点图的有效点个数
		*/
		void count_focuse_vaildNum(int&vaildNum, Eigen::Vector3&cloudFocuse) const;

		/**  \brief 必须初始化对象，只分配空间。但不赋值
		*   \param[in] x                             点图的宽
		*   \param[in] y                             点图的高
		*   \param[in] camParInput                   重建这张点图相机的内参数
		*   \param[in] sample                        点图的采样率，默认X和Y方向的采样率是相同的
		*/
		void ini(const int x, const int y, const CameraInParam&camParInput, const int sample);


		/**  \brief 必须初始化对象，只分配空间。但不赋值
		*   \param[in] x                             点图的宽
		*   \param[in] y                             点图的高
		*   \param[in] camParInput                   重建这张点图相机的内参数
		*   \param[in] sampleX                       点图的采样率，点图在X方向上的采样率
		*   \param[in] sampleY                       点图的采样率，点图在Y方向上的采样率
		*/
		void ini(const int x, const int y, const CameraInParam&camParInput, const int sampleX , const int sampleY);

		/**  \brief 只初始化点图大小，采样率和相机参数可以后续赋值
		*   \param[in] x                             点图的宽
		*   \param[in] y                             点图的高
		*/
		void ini(const int x, const int y);

		/**  \brief 将类的点和法向都置为0
		*/
		void setZero();

		/**  \brief 将无法计算法线的点全部置0
		*/
		void delete_point_without_normal();

		/**  \brief 将点图变换回原始相机坐标系
		*/
		void to_ori_coor();


		/**  \brief 更新点的法向量使用计算Eigen的方式，使用tbb并行,为保证法向朝向的正确，必须保证当前相机的坐标系是正确的set_rt（）
		*   \param[in] range                                求法向时拟合平面的邻域大小
		*   \param[in] vaildThread                          求法向时临域内部有效点个数大于该阈值才可计算
		*   \param[in] depthDiff                            该参数意为在拟合平面求取法向时，中心点和领域点Z方向的差距应该小于depthDiff，才能参与拟合
		*/
		void update_VNrv(const int range, const int vaildThread , const float depthDiff);


		/**  \brief 用于对当前点进行刚体变换，只对有效点进行。所谓无效点（abs(point（0))<1e-5 && abs(point(1)) < 1e-5 && abs(point(2) , 1e-5）
		*   \param[in] rt                            对点云做刚体变换时的rt
		*/
		virtual void rigid_transformation(const RigidMatrix& rt);


		/**  \brief 获取当前点云所在的坐标系
		*/
		inline const RigidMatrix&get_rt() const{ return _rtGlobal;}


		/**  \brief 找到点图上的边缘点，并且将这些边缘点flag标记为true，这些边缘点不是严格意义上的边缘点，对于图形内部的小孔有免疫性
		*   \param[in] range                        表示找到这些点的range环临域
		*   \param[in] disThread                    判断是否临域的距离阈值。
		*   \param[in] pointNumThread               边缘长度阈值，边缘长度大于这个值才被判定为边缘点
		*/
		void find_egde_point(const int range, const float disThread , const int pointNumThread);

		/**  \brief 获取这张点图的宽度
		*/
		inline const int get_image_width() const { return _imageWidth; }

		/**  \brief 获取这张点图的高度
		*/
		inline const int get_image_height() const{ return _imageHeight; }

		/**  \brief 当输入的点图不是在原始相机坐标系之下时，需要从外部输入当前点所在的rt
		*   \param[in] rt                            当前点所在的坐标系相对于全局坐标系的rt
		*/
		void set_rt(const RigidMatrix& rt){ _rtGlobal = rt; }

		/**  \brief 获取id = Y*_imageWidth + X的点
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*/
		inline const Eigen::Vector3&get_point(const int x, const int y) const
		{
			int id = y*_imageWidth + x;
			return(get_vCoord(id));
		}

		/**  \brief 获取id = Y*_imageWidth + X的点
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*/
		inline Eigen::Vector3&get_point(const int x, const int y)
		{
			int id = y*_imageWidth + x;
			return(get_vCoord(id));
		}


		/**  \brief 获取id = Y*_imageWidth + X的法向
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*/
		inline const Eigen::Vector3&get_normal(const int x, const int y) const
		{
			int id = y*_imageWidth + x;
			return(get_vNrv(id));
		}

		/**  \brief 获取id = Y*_imageWidth + X的法向
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*/
		inline Eigen::Vector3&get_normal(const int x, const int y)
		{
			int id = y*_imageWidth + x;
			return(get_vNrv(id));
		}

		/**  \brief 获取id = Y*_imageWidth + X的颜色
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*/
		inline const Eigen::Vector3&get_color(const int x, const int y) const
		{
			int id = y*_imageWidth + x;
			return get_vColor(id);
		}

		/**  \brief 获取id = Y*_imageWidth + X的点的权重
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
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

		/**  \brief 拷贝一份的vWeight
		*/
		inline std::vector<float>&get_vWeights()
		{
			return _weight;
		}

		inline const std::vector<float>& get_vWeights() const
		{
			return _weight;
		}

		/**  \brief 设置id = Y*_imageWidth + X的点的权重
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
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

		/**  \brief 一次性批量设置点图权重
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

		/**  \brief 获取id = Y*_imageWidth + X的颜色
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*/
		inline Eigen::Vector3&get_color(const int x, const int y)
		{
			int id = y*_imageWidth + x;
			return get_vColor(id);
		}


		/**  \brief 设置id = Y*_imageWidth + X的点
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*   \param[in] point                         输入的点
		*/
		inline void set_point(const int x, const int y, const Eigen::Vector3&point)
		{
			int id = y*_imageWidth + x;
			set_vCoord(id, point);
		}

		/**  \brief 设置id = Y*_imageWidth + X的法向
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*   \param[in] normal                        输入的法向
		*/
		inline void set_normal(const int x, const int y, const Eigen::Vector3&normal)
		{
			int id = y*_imageWidth + x;
			set_vNrv(id, normal);
		}

		/**  \brief 设置id = Y*_imageWidth + X的颜色
		*   \param[in] x                             该点在图片上X轴的位置
		*   \param[in] y                             该点在图片上Y轴的位置
		*   \param[in] color                         输入的颜色
		*/
		inline void set_color(const int x, const int y, const Eigen::Vector3&color)
		{
			int id = y*_imageWidth + x;
			set_vColor(id, color);
		}

		/**  \brief 计算法相的tbb接口，不可调用
		*   \param[in] range                                求取法向时，拟合平面领域大小
		*   \param[in] pN                                   点在线性位置上的id
		*   \param[in] vaildThread                          求法向时临域内部有效点个数大于该阈值才可计算
		*   \param[in] depthDiff                            该参数意为在拟合平面求取法向时，中心点和领域点Z方向的差距应该小于depthDiff，才能参与拟合
		*/
		void countNormal_eigen_tbb(const int range, const int pN, const float depthDiff, const int vaildThread , const std::vector<int>&vaildPointSet);

		/**  \brief 获取当前点图设备内参
		*/
		inline const CameraInParam&get_camPar() const{ return _cameraInParam;}

		/**  \brief 设置当前点图设备内参
		*   \param[in] camearInPar                          重新设置当前点图的相机内参
		*/
		void set_camPar(const CameraInParam camearInPar)
		{
			_cameraInParam = camearInPar;
		}

		/**  \brief 获取过x，y这个像素和光线的线的方向，该向量未归一化，其中Z方向为1。
		*   \param[in] x                                     像素x方向坐标
		*   \param[in] y                                     像素y方向坐标
		*/
		inline Eigen::Vector3 get_ray_direct(const int x, const int y) const
		{
			float _viewRadialX = get_viewRadialX(); float _viewRadialY = get_viewRadialY();
			return(Eigen::Vector3((x + _viewRadialX - _cameraInParam.get_cx()) / _cameraInParam.get_fx(), (y + _viewRadialY - _cameraInParam.get_cy()) / _cameraInParam.get_fy(), 1));
		}

		/**  \brief 获取过x，y这个像素和光线的线的方向，该向量未归一化，其中Z方向为1。
		*   \param[in] pointId                               该点的在整个数组中的Id
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
		
		/**  \brief 以距离为原则，删除单幅面数据中的孤岛
		*   \param[in] findRange                               查找临域的大小
		*   \param[in] disThread                               距离阈值
		*   \param[in] numThread                               判断孤岛的阈值，孤岛中点个数小于该值则认为是孤岛
		*   \                                                  如果该值为0则表示只保留最大的那个岛。
		*/
		void delete_island(const int findRange , const float disThread , const int numThread);


		/**  \brief 以深度为原则，删除单幅面数据中的孤岛
		*   \param[in] findRange                               查找临域的大小
		*   \param[in] disThread                               深度距离阈值
		*   \param[in] numThread                               判断孤岛的阈值，孤岛中点个数小于该值则认为是孤岛
		*   \                                                  如果该值为0则表示只保留最大的那个岛。
		*/
		void delete_island_through_depth(const int findRange, const float disThread, const int numThread , Eigen::Vector3 deleteResult = Eigen::Vector3(0 ,0 ,0));

		/**  \brief 将被标记的点置为0
		*/
		void delete_point_flaged(const Eigen::Vector3 deleteResult = Eigen::Vector3(0, 0, 0));
		

		inline float get_viewRadialX() const{ return 0.5 / _sampleX; }
		inline float get_viewRadialY() const{ return 0.5 / _sampleY; }

		/**  \brief 计算某个指定点的法向
		*   \param[in] imageX                                   某个点的X坐标
		*   \param[in] imageY                                   某个点的Y坐标
		*   \param[in] vaildThread                              邻域内有效点个数，如果点个数小于该值则不计算其法向
		*   \param[in] depthDiff                                邻域内有效点之间的深度差，如果大于该值则认为该点非邻域
		*   \param[in] range                                    拟合法向的窗口半径
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

		
		/**  \brief 把pointImage计算得到的结果在一张图片上展示出来，这张图片必须是用于计算这张点图的图片,图片必须是float类型
		*   \param[in] Image                                    计算点图的原始图片
		*/
		void draw_pointImage_on_pic(const float*sourceImage , const std::string&outputpath);


		/**  \brief 判断这片点云上有多少百分比的点属于一个最大的平面
		*   \param[in] planeRata                                   某个平面上点云的百分比占比
		*   \param[in] planeThreshold                              判定一个点是否在一个平面上的距离阈值
		*/
		void plane_detect(float&planeRata , const float planeThreshold);

 	private:

		/**  \brief 以深度为原则，删除单幅面数据中的孤岛
		*   \param[in] findRange                               查找临域的大小
		*   \param[in] disThread                               深度距离阈值
		*   \param[in] numThread                               判断孤岛的阈值，孤岛中点个数小于该值则认为是孤岛
		*   \                                                  如果该值为0则表示只保留最大的那个岛。
		*   \param[in] isDepth                                 是否通过深度的差值来判断这个点是否为一个团的
		*/
		void delete_core_function(const int findRange, const float disThread, const int numThread, const bool isDepth , const Eigen::Vector3&deleteresult);


		/**  \brief 删除孤岛配套函数
		*   \param[in] pointIdSet                              该变量记录了将被删除的点的id
		*   \param[in] imageId                                 当前点id
		*   \param[in] windowRange                             查找临域点的半径 在像素上N*N的窗口
		*   \param[in] disThread                               判断点是否为临域的距离阈值
		*   \param[in] isDepth                                 是否通过深度的差值来判断这个点是否为一个团的，如果不用深度则直接用点之间的距离作为判断阈值
		*/
		void get_point_around(std::vector<int>&pointIdSet, const int imageId, const int windowRange, const float disThread , const bool isDepth);

		/**  \brief 用于找出最大的那个岛，并且将其他的岛的点都删除的函数
		*   \param[in] islandRecord                            用于记录的变量
		*/
		void save_max_island(const std::vector<std::vector<int> >&islandRecord);

		/**  \brief 确定该边缘是否为最大边缘
		*   \param[in] idX                                     当前点的X坐标
		*   \param[in] idY                                     当前点的Y坐标
		*   \param[in] disThread                               判断是否为邻域的距离阈值
		*/
		bool is_edge(const Eigen::Vector3&pointCenter , const int idX, const int idY, const float disThread);


		/**  \brief 用于查找边缘的点个数统计的函数
		*   \param[in] idRecord                                记录了着一团边缘点id的vector
		*   \param[in] pointId                                 当前收索点的ID
		*/
		void get_edge_around(std::vector<int>&idRecord , const int pointId);

 		int _imageWidth;///< 深度图的宽度
 		int _imageHeight;///< 深度图的高度
 		RigidMatrix _rtGlobal;///< 当前点云到新的坐标系的RT。局部坐标系到全局坐标系的RT
		CameraInParam _cameraInParam;///< 重建当前片点的相机的内参数
		int _sampleX;///<这张点图在X方向上的采样率
		int _sampleY;///<这张点图在Y方向上的采样率
		bool _hasWeight;///<点图是否带有权重属性的标致量
		std::vector<float>_weight;///<单个点对应的权重
	};
}//namespace Sn3DAlgorithm

#endif//SN3D_ALGORITHM_COMMON_BASE_DEPTH
