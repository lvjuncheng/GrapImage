#ifndef SN3D_ALGORITHM_PRINT3D_AUXILARY_ALGORITHM_AUXILARY_INCLUDE_H
#define SN3D_ALGORITHM_PRINT3D_AUXILARY_ALGORITHM_AUXILARY_INCLUDE_H

#include "algorithmAuxilaryTraits.h"

#include "common/baseType/BaseTriMesh.h"
#include "print3DCommon/baseType/Entity.h"

namespace Sn3DAlgorithm
{
	//////////////////////////////////////////////////////////////////////////
	//Support
	//////////////////////////////////////////////////////////////////////////

	/** \brief	设置当前生成支撑的网格
	*	\param[in]				mesh								待生成支撑网格
	*	\param[out]				meshID								网格内部标识
	*	\note	所有使用meshID的接口需要事先调用该接口（用于缓存中间数据，防止重复计算）
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API add_mesh(
		const BaseTriMesh& mesh,
		int& meshID
		);

	/** \brief	清除当前生成支撑的网格
	*	\param[in]				meshID								网格内部标识
	*	\note	获得需要的支撑和基座后，手动清除缓存，释放内存（一定要释放内存，防止内存泄露）
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API remove_mesh(
		int meshID
		);

	/** \brief	生成模型待支撑点
	*	\param[in]				meshID								待生成支撑网格ID
	*	\param[in]				baseZ								支撑从此z值位置向上生成，小于该值的位置被忽略
	*	\param[in]				supportRadius						支撑结构中插入部分半径，用于优化支撑位置
	*	\param[in]				MillimeterPerPixel					生成支撑点时使用图像采样的图像精度
	*	\param[in]				condition							需要生成支撑的区域的条件
	*	\param[out]				supportPointInfos					模型待支撑点信息
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_support_points(
		int meshID,
		Scalar baseZ,
		Scalar supportRadius,
		Scalar MillimeterPerPixel,
		const SupportGenerateCondition& condition,
		std::vector<SupportPointInfo>& supportPointInfos
		);

	/**	\brief	DLP生成模型支撑
	*	\param[in]					meshID								待生成支撑网格ID
	*	\param[out]					support								生成的支撑
	*	\param[in]					supportPointInfo					待支撑点信息
	*	\param[in]					needInnerSupport					是否需要生成内部支撑
	*	\param[in]					baseZ								支撑从此z值位置向上生成，小于该值的位置被忽略
	*	\param[in,out]				structure							支撑具体的结构细节，会被修改为最终生成的结构
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_DLP_support(
		int meshID,
		BaseTriMesh& support,
		const SupportPointInfo& supportPointInfo,
		bool needInnerSupport,
		Scalar baseZ,
		SupportStructure& structure
		);

	/**	\brief	添加支撑加固
	*	\param[in]					meshID								待生成支撑网格ID
	*	\param[out]					reinforce							生成的支撑
	*	\param[in]					reinforceLength						当支撑长度大于该长度，则生成支撑水平连接，用以加固支撑
	*	\param[in]					reinforceRadius						用以加固的杆半径
	*	\param[in]					maxDist								被加固连接的支撑间距最大值
	*	\param[in]					structs								已生成的支撑的结构
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_support_reinforce(
		int meshID,
		BaseTriMesh& reinforce,
		Scalar reinforceLength,
		Scalar reinforceRadius,
		Scalar reinforceAngle,
		Scalar maxDist,
		const std::vector<SupportStructure>& structs
		);

	/// \brief 创建树形外部支撑
	/// \param[in]					meshID								输入的网格
	/// \param[in]					supportPointInfos					输入的网格的待支撑点
	/// \param[out]					support								输出的树型支撑网格，包含原有的网格
	/// \param[in]					safePairDistance					寻找配对点的最大距离
	/// \param[in]					maxSafeAngle						配对点的最大支撑角度
	/// \param[in]					ptMoveDown							被支撑点起始向下移动距离
	/// \param[in]					headThickness						树型支撑柱子起始头的厚度
	/// \param[in]					footThickness						地面脚部的厚度
	/// \param[in]					objectRadius						跟模型接触面的半径
	/// \param[in]					normalRadius						树型支撑一般柱子的半径
	/// \param[in]					footRadius							地面接触脚部的半径
	/// \param[in]					polygonSidesNumber					柱子横截面的正多边形，边数
	/// \param[in]					growDownDistance					每次向下生长的长度

	RetVal SN3D_PRINT3D_AUXILARY_API get_tree_support(
		int meshID,
		const std::vector<SupportPointInfo>& supportPointInfos,
		BaseTriMesh &support,
		const TreeConfigure& treeConfig,
		Scalar baseZ
		);

	/** \brief 根据切片得到的每层轮廓生成对应的fdm支撑
	*  \param[in,out]		 entity									实体，包含切片得到的轮廓数据，填充线结果也存在entity中
	*  \param[out]			 firstSupportBoundarys					第一层的支撑轮廓数据
	*  \param[in]			 everywhere        						支撑生成的类型：true:生成 外部支撑+内部支撑； false:仅生成外部支撑(外部支撑是指支撑生长在平台上，内部支撑是指支撑生长在模型上)
	*  \param[in]			 margin									支撑边界（超出实体的部分）宽度
	*  \param[in]			 gap									支撑与实体在水平方向的距离
	*  \param[in]			 minAngle								允许生成支撑的最小角度(与z轴夹角)，倾斜度大于此数值才会生成支撑
	*  \param[in]			 surroundingOffset						稳固支撑时外包围轮廓的缩放系数
	*  \param[in]			 topLineDis								Top类型支撑线间距
	*  \param[in]			 topDownRate							Top类型支撑高度下降比率（相对于层厚）
	*  \param[in]			 topAngle								Top类型支撑路径角度
	*  \param[in]			 midiumLineDis							Midium类型支撑线间距
	*  \param[in]			 midiumDownRate							Midium类型支撑高度下降比率（相对于层厚）
	*  \param[in]			 midiumAngle							Midium类型支撑路径角度
	*  \param[in]			 baseLineDis							Base类型支撑线间距
	*  \param[in]			 baseDownRate							Base类型支撑高度下降比率（相对于层厚）
	*  \param[in]			 baseAngle								Base类型支撑路径角度
	*  \param[in]			 solidSupport							是否采用稳固支撑，即对base支撑增加外包围轮廓
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_fdm_support(
		Entity& entity,
		std::vector<Path::Ptr>& firstSupportBoundarys,
		bool everywhere,
		Scalar margin,
		Scalar gap,
		Scalar minAngle,
		Scalar surroundingOffset,
		Scalar topLineDis,
		Scalar topDownRate,
		Scalar topAngle,
		Scalar midiumLineDis,
		Scalar midiumDownRate,
		Scalar midiumAngle,
		Scalar baseLineDis,
		Scalar baseDownRate,
		Scalar baseAngle,
		bool solidSupport = false
		);



	//////////////////////////////////////////////////////////////////////////
	//raft
	//////////////////////////////////////////////////////////////////////////

	/**	\brief DLP生成模型基座
	*	\param[in]				mesh								待生成基座网格
	*	\param[out]				raft								生成的基座轮廓
	*	\param[in]				offset								基座适当的放缩（正扩负缩）
	*	\param[in]				baseType                            使用哪种方式生成基座（0：包围盒，1：凸包，2：投影区（有洞），3.投影区（无洞））
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_raft_boundary(
		const BaseTriMesh& mesh,
		std::vector<Point2Vector::Ptr>& raft,
		Scalar offset,
		int baseType
		);

	/** \brief 根据切片得到的首层轮廓和首层支撑生成对应的fdm基座(如果有支撑，生成基座一定要在生成支撑之后调用)
	*  \param[in,out]		 entity														实体，包含切片得到的轮廓数据，填充线结果也存在entity中
	*  \param[in]			 margin														基座边界（超出实体的部分）宽度
	*  \param[in]			 surroundingOffset											基座外包围与base类型基座的间距
	*  \param[in]			 initialHeight												基座初始高度（层底面高度）
	*  \param[in]			 baseLayers													base类型基座的层数
	*  \param[in]			 baseLineDis												base类型基座的线间距
	*  \param[in]			 baseThickness												base类型基座的厚度
	*  \param[in]			 baseAngle													base类型基座线旋转角度
	*  \param[in]			 firstInterfaceLayers										firstInterface类型基座的层数
	*  \param[in]			 firstInterfaceLineDis										firstInterface类型基座的线间距
	*  \param[in]			 firstInterfaceThickness									firstInterface类型基座的厚度
	*  \param[in]			 firstInterfaceAngle										firstInterface类型基座线旋转角度
	*  \param[in]			 secondInterfaceLayers										secondInterface类型基座的层数
	*  \param[in]			 secondInterfaceLineDis										secondInterface类型基座的线间距
	*  \param[in]			 secondInterfaceThickness									secondInterface类型基座的厚度
	*  \param[in]			 secondInterfaceAngle										secondInterface类型基座线旋转角度
	*  \param[in]		     firstSupportBoundarys										第一层的支撑轮廓数据
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_fdm_raft(
		Entity& entity,
		Scalar margin,
		Scalar surroundingOffset,
		Scalar initialHeight,
		int baseLayers,
		Scalar baseLineDis,
		Scalar baseThickness,
		Scalar baseAngle,
		int firstInterfaceLayers,
		Scalar firstInterfaceLineDis,
		Scalar firstInterfaceThickness,
		Scalar firstInterfaceAngle,
		int secondInterfaceLayers,
		Scalar secondInterfaceLineDis,
		Scalar secondInterfaceThickness,
		Scalar secondeInterfaceAngle,
		const std::vector<Path::ConstPtr>& firstSupportBoundarys
		);

	//////////////////////////////////////////////////////////////////////////
	//custumize for different craft
	//////////////////////////////////////////////////////////////////////////

	/** \brief 计算投影板相对投影仪光心的中心点坐标
	*  \param[in]      K              投影仪内参
	*  \param[in]      imageSize      投影图片的x,y方向的尺寸
	*  \param[in]      planeSize      投影板的x,y方向上的尺寸
	*  \param[out]     center         投影板的中心点坐标
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_compute_projection_plane_cener(
		const std::vector<double>& K, 
		const Eigen::Vector2i& imageSize,
		const Point2& planeSize, 
		Point2& center
		);

	/** \brief 通过测量打印标准模型的尺寸，进行DLP的投影板的姿态标定
	*  \param[in]      K              投影仪内参
	*  \param[in]      distortion     投影仪畸变
	*  \param[in]      imageCoords    投影仪像素平面的标示点
	*  \param[in]   pId1, pId2, dis   第pId1[i]点和第pId2[i]点在投影板上的距离是dis[i]
	*  \param[in]      height         投影板到投影仪的距离(参考值)
	*  \param[in]      center         投影板相对投影仪光心的中心点坐标
	*  \param[out]     mat            投影板到投影仪坐标系的RT
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_calibrate_extrinsic(
		const std::vector<double>& K, 
		const std::vector<double>& distortion,
		const std::vector<Point2>& imageCoords, 
		const std::vector<int>& pId1,
		const std::vector<int>& pId2,
		const std::vector<double>& dis, 
		double height,
		const Point2& center, 
		RigidMatrix& mat
		);

	/** \brief 通过测量打印模型的三维坐标，标定DLP投影仪的内外参数
	*  \param[in]      heightSamples      打印模型在图片长度上的采样
	*  \param[in]      widthSamples       打印模型在图片宽度上的采样
	*  \param[in]      coords             图片上有heightSamples.size() * widthSamples.size() 个采样点。coords是这些采样点实际打印后，
										  测量得到的坐标。coords[i*heightSamples.size()+j]为采样点(widthSamples[j],heightSamples[i])得到的坐标。
	*  \param[in]      centerCoord        coords坐标系中的中心点坐标
	*  \param[in, out] K                  投影仪内参
	*  \param[out]     distortion         投影仪畸变
	*  \param[out]     mat                投影板到投影仪坐标系的RT
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_calibrate_intr_extrinsic(
		const std::vector<double>& heightSamples,
		const std::vector<double>& widthSamples,
		const std::vector<Eigen::Vector3>& coords,
		const Eigen::Vector3& centerCoord,
		std::vector<double>& K,
		std::vector<double>& distortion,
		RigidMatrix& mat
		);

	/** \brief 通过测量打印模型的三维坐标，标定DLP投影仪的内外参数
	*  \param[in]      height width       图片的长和宽
	*  \param[in]      heightSamples      打印模型在图片长度上的采样
	*  \param[in]      widthSamples       打印模型在图片宽度上的采样
	*  \param[in]      coords             图片上有heightSamples.size() * widthSamples.size() 个采样点。coords是这些采样点实际打印后，
	                                      测量得到的坐标。coords[i*heightSamples.size()+j]为采样点(widthSamples[j],heightSamples[i])得到的坐标。
	*  \param[in]      centerCoord        coords坐标系中的中心点坐标
	*  \param[in, out] K                  投影仪内参
	*  \param[out]     distortion         投影仪畸变
	*  \param[out]     mat                投影板到投影仪坐标系的RT
	*  \param[out]     lutH, lutW         畸变残渣的查找表.长度方向和宽度方向的实际位置
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_calibrate_intr_extrinsic(
		int height,
		int width,
		const std::vector<double>& heightSamples,
		const std::vector<double>& widthSamples,
		const std::vector<Eigen::Vector3>& coords,
		const Eigen::Vector3& centerCoord,
		std::vector<double>& K,
		std::vector<double>& distortion,
		RigidMatrix& mat,
		ImageGreyf& lutH,
		ImageGreyf& lutW
		);

	/** \brief 通过多次测量打印模型的三维坐标，标定DLP投影仪的内外参数
	*  \param[in]      height width       图片的长和宽 必须是1080 * 1920
	*  \param[in]      heightSamples      打印模型在图片长度上的采样
	*  \param[in]      data               标定所需要的测量数据
	*  \param[in, out] K                  投影仪内参
	*  \param[out]     distortion         投影仪畸变
	*  \param[out]     mat                投影板到投影仪坐标系的RT
	*  \param[out]     lutH, lutW         畸变残渣的查找表.长度方向和宽度方向的实际位置
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_calibrate_intr_extrinsic(
		int height,
		int width,
		const std::vector<DLPCalibrationData>& data,
		std::vector<double>& K,
		std::vector<double>& distortion,
		RigidMatrix& mat,
		ImageGreyf& lutH,
		ImageGreyf& lutW
		);

	/** \brief 计算打印平面和投影图像平面的一一映射关系
	 *  \param[in]    p3D            打印平面中的3维点
	 *  \param[in]    p2D            图像平面中的2维点 和p3D是一一对应关系
	 *  \param[in]    imageSize      图像平面的长宽.imageSize[0]为宽,imageSize[1]为长。
	 *  \param[in]    planeSize      打印平面的长宽.planeSize[0]为宽x,planeSize[1]为长y.
	 *  \param[out]   table          返回的查找表
	 *  \param[in]    smoothWeight   平滑项权重.范围[0.1,0.5]
	 */
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_compute_map_relation(
		const std::vector<Eigen::Vector3>& p3D,
		const std::vector<Eigen::Vector2>& p2D,
		const Eigen::Vector2i& imageSize,
		const Eigen::Vector2& planeSize,
		DLPMapLookupTable& table,
		double smoothWeight = 0.3f
		);

	/** \brief 计算打印平面到投影的图像平面的映射关系,该接口适用于多次投影标定板图案
	*  \param[in]        widthSamples      标定板数据在图片宽度上的采样
	*  \param[in]        heightSamples     标定板数据在图片高度上的采样
	*  \param[in]        offsets           每张标定板数据的偏移量.offset[i][0]是宽上的偏移量,offset[i][1]是高上的偏移量。必须有(0,0)偏移的图片，用来标定坐标系。
	*  \param[in]        p3Ds              采样点的三维点坐标。有offsets.size()*heightSamples.size()*widthSample.size()个采样点.数据排列优先级从高到底width height offset.
	(-1,-1,-1)为无效点。返回时噪声点重新设为(-1,-1,-1)
	*  \param[in]        imageSize         图像平面的长宽.imageSize[0]为宽,imageSize[1]为长。
	*  \param[in]        planeSize         打印平面的长宽.planeSize[0]为宽x,planeSize[1]为长y.
	*  \param[out]       table             返回的查找表
	*  \param[in]        smoothWeight      平滑项权重.范围[0.1,0.5]
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_compute_map_use_calibration_board(
		const std::vector<float>& widthSamples,
		const std::vector<float>& heightSamples,
		const std::vector<Eigen::Vector2>& offsets,
		const std::vector<Eigen::Vector3>& p3Ds,
		const Eigen::Vector2i& imageSize,
		const Eigen::Vector2& planeSize,
		DLPMapLookupTable& table,
		double smoothWeight = 0.3f
		);

	RetVal SN3D_PRINT3D_AUXILARY_API read_DLPMapLookupTable(const std::string& fileName, DLPMapLookupTable& table);
	RetVal SN3D_PRINT3D_AUXILARY_API write_DLPMapLookupTable(const std::string& fileName, const DLPMapLookupTable& table);

	///临时接口，fileNames的长度可以是1，2，3，4.1是初始位置，2是x偏移，3是y偏移，4是xy都偏移。顺序不能错
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_create_DLPCalibrationData(
		const std::vector<std::string>& fileNames,
		std::vector<DLPCalibrationData>& data
		);

	/** \brief 得到每层的切片图片
	*	\param[out]				model									待打印的模型轮廓信息
	*	\param[out]				resImages								返回的切片图片
	*	\param[in]				imageWidth								图片宽度
	*	\param[in]				imageHeight								图片长度
	*	\param[in]				K										投影仪内参[fu, u0, v0, ar, skew]
	*	\param[in]				dis										投影仪的畸变[k1,k2,p1,p2,k3]
	*	\param[in]				mat										投影板到投影仪的RT变换
	*   \param[in]              lutH, lutW                              畸变残渣的查找表.长度方向和宽度方向的实际位置
	*	\param[in]				subPixelNum								子像素数
	*	\param[in]				enableGreyLevels						启用灰度等级，关闭则只有0和255
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_get_slice_images(
		const std::vector<DLPLayer>& model, 
		std::vector<ImageGreyb>& resImages,
		int imageWidth, 
		int imageHeight,
		const std::vector<double>& K, 
		const std::vector<double>& dis, 
		const RigidMatrix& mat,
		ImageGreyf& lutH,
		ImageGreyf& lutW,
		int subPixelNum,
		bool enableGreyLevels,
		bool enableMirrorY,
		Scalar mirrorY
		);

	RetVal SN3D_PRINT3D_AUXILARY_API DLP_get_slice_images(
		const std::vector<DLPLayer>& model,
		std::vector<ImageGreyb>& resImages,
		int imageWidth,
		int imageHeight,
		const std::vector<double>& K,
		const std::vector<double>& dis,
		const RigidMatrix& mat,
		int subPixelNum,
		bool enableGreyLevels,
		bool enableMirrorY,
		Scalar mirrorY
		);

	RetVal SN3D_PRINT3D_AUXILARY_API DLP_get_slice_images(
		const std::vector<DLPLayer>& model,
		std::vector<ImageGreyb>& resImages,
		int imageWidth,
		int imageHeight,
		const DLPMapLookupTable& table,
		int subPixelNum,
		bool enableGreyLevels,
		bool enableMirrorY,
		Scalar mirrorY
		);


	/**	\brief	对图片灰度作校准
	*  \param[out]          images								  返回的切片图片
	*  \param[in]           maxDecrease                           最大校准值（0~255）
	*  \param[in]           projDist							  投影距离，单位mm
	*  \param[in]           projWidth                             投影区宽度，单位mm
	*  \param[in]           projHeight                            投影区高度，单位mm
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_func(
		ImageGreyb& image,
		Scalar maxDecrease,
		Scalar projDist = 160,
		Scalar projWidth = 144,
		Scalar projHeight = 81,
		Scalar milimeterPerPixel = 0.075
		);

	/**	\brief	对图片灰度作校准
	*  \param[out]          images								  返回的切片图片
	*  \param[in]           rowCount                              图片分成rowCount行
	*  \param[in]           colCount							  图片分成colCount列
	*  \param[in]           ratio								  从图片最小坐标位置开始，横向扫描，每个区域的灰度调整比例，取值0~1
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_grid_divide(
		ImageGreyb& image,
		int rowCount,
		int colCount,
		const std::vector<Scalar>& ratio
		);

	/**	\brief	对图片灰度作校准
	*  \param[out]          images								  返回的切片图片
	*  \param[in]           rowCount                              图片分成rowCount行
	*  \param[in]           colCount							  图片分成colCount列
	*  \param[in]           ratio								  从图片最小坐标位置开始，横向扫描，每个区域的灰度调整比例，取值0~1
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_grid_divide_centered(
		ImageGreyb& image,
		int rowCount,
		int colCount,
		const std::vector<Scalar>& ratio
		);

	/**	\brief	对图片灰度作校准
	*  \param[out]          images								  返回的切片图片
	*  \param[in]           radiusRatio							  每一对pair代表半径r和校准比率ratio(取值0~1)，表示小于r（且大于更小的r'）的区域使用ratio
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_radius_divide(
		ImageGreyb& image,
		const std::vector<std::pair<Scalar, Scalar>>& radiusRatio
		);


	/** \brief 将得到的image图片保存成DLP可识别的图片格式
	*  \param[in]           fileName                              图片保存的路径和文件名
	*  \param[in]           image                                 待保存的图片
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_save_image(
		const std::string& fileName, 
		const ImageGreyb& image
		);

	/** \brief 计算图片上的亮节点个数
	*  \param[in]          image                                  待计算的图片
	*  \param[out]         num                                    返回的结果
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_get_valid_pixel_num(
		const ImageGreyb& image, 
		double& num
		);
}

#endif