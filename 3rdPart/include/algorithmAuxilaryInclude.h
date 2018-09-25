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

	/** \brief	���õ�ǰ����֧�ŵ�����
	*	\param[in]				mesh								������֧������
	*	\param[out]				meshID								�����ڲ���ʶ
	*	\note	����ʹ��meshID�Ľӿ���Ҫ���ȵ��øýӿڣ����ڻ����м����ݣ���ֹ�ظ����㣩
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API add_mesh(
		const BaseTriMesh& mesh,
		int& meshID
		);

	/** \brief	�����ǰ����֧�ŵ�����
	*	\param[in]				meshID								�����ڲ���ʶ
	*	\note	�����Ҫ��֧�źͻ������ֶ�������棬�ͷ��ڴ棨һ��Ҫ�ͷ��ڴ棬��ֹ�ڴ�й¶��
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API remove_mesh(
		int meshID
		);

	/** \brief	����ģ�ʹ�֧�ŵ�
	*	\param[in]				meshID								������֧������ID
	*	\param[in]				baseZ								֧�ŴӴ�zֵλ���������ɣ�С�ڸ�ֵ��λ�ñ�����
	*	\param[in]				supportRadius						֧�Žṹ�в��벿�ְ뾶�������Ż�֧��λ��
	*	\param[in]				MillimeterPerPixel					����֧�ŵ�ʱʹ��ͼ�������ͼ�񾫶�
	*	\param[in]				condition							��Ҫ����֧�ŵ����������
	*	\param[out]				supportPointInfos					ģ�ʹ�֧�ŵ���Ϣ
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_support_points(
		int meshID,
		Scalar baseZ,
		Scalar supportRadius,
		Scalar MillimeterPerPixel,
		const SupportGenerateCondition& condition,
		std::vector<SupportPointInfo>& supportPointInfos
		);

	/**	\brief	DLP����ģ��֧��
	*	\param[in]					meshID								������֧������ID
	*	\param[out]					support								���ɵ�֧��
	*	\param[in]					supportPointInfo					��֧�ŵ���Ϣ
	*	\param[in]					needInnerSupport					�Ƿ���Ҫ�����ڲ�֧��
	*	\param[in]					baseZ								֧�ŴӴ�zֵλ���������ɣ�С�ڸ�ֵ��λ�ñ�����
	*	\param[in,out]				structure							֧�ž���Ľṹϸ�ڣ��ᱻ�޸�Ϊ�������ɵĽṹ
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_DLP_support(
		int meshID,
		BaseTriMesh& support,
		const SupportPointInfo& supportPointInfo,
		bool needInnerSupport,
		Scalar baseZ,
		SupportStructure& structure
		);

	/**	\brief	���֧�żӹ�
	*	\param[in]					meshID								������֧������ID
	*	\param[out]					reinforce							���ɵ�֧��
	*	\param[in]					reinforceLength						��֧�ų��ȴ��ڸó��ȣ�������֧��ˮƽ���ӣ����Լӹ�֧��
	*	\param[in]					reinforceRadius						���Լӹ̵ĸ˰뾶
	*	\param[in]					maxDist								���ӹ����ӵ�֧�ż�����ֵ
	*	\param[in]					structs								�����ɵ�֧�ŵĽṹ
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

	/// \brief ���������ⲿ֧��
	/// \param[in]					meshID								���������
	/// \param[in]					supportPointInfos					���������Ĵ�֧�ŵ�
	/// \param[out]					support								���������֧�����񣬰���ԭ�е�����
	/// \param[in]					safePairDistance					Ѱ����Ե��������
	/// \param[in]					maxSafeAngle						��Ե�����֧�ŽǶ�
	/// \param[in]					ptMoveDown							��֧�ŵ���ʼ�����ƶ�����
	/// \param[in]					headThickness						����֧��������ʼͷ�ĺ��
	/// \param[in]					footThickness						����Ų��ĺ��
	/// \param[in]					objectRadius						��ģ�ͽӴ���İ뾶
	/// \param[in]					normalRadius						����֧��һ�����ӵİ뾶
	/// \param[in]					footRadius							����Ӵ��Ų��İ뾶
	/// \param[in]					polygonSidesNumber					���Ӻ�����������Σ�����
	/// \param[in]					growDownDistance					ÿ�����������ĳ���

	RetVal SN3D_PRINT3D_AUXILARY_API get_tree_support(
		int meshID,
		const std::vector<SupportPointInfo>& supportPointInfos,
		BaseTriMesh &support,
		const TreeConfigure& treeConfig,
		Scalar baseZ
		);

	/** \brief ������Ƭ�õ���ÿ���������ɶ�Ӧ��fdm֧��
	*  \param[in,out]		 entity									ʵ�壬������Ƭ�õ����������ݣ�����߽��Ҳ����entity��
	*  \param[out]			 firstSupportBoundarys					��һ���֧����������
	*  \param[in]			 everywhere        						֧�����ɵ����ͣ�true:���� �ⲿ֧��+�ڲ�֧�ţ� false:�������ⲿ֧��(�ⲿ֧����ָ֧��������ƽ̨�ϣ��ڲ�֧����ָ֧��������ģ����)
	*  \param[in]			 margin									֧�ű߽磨����ʵ��Ĳ��֣����
	*  \param[in]			 gap									֧����ʵ����ˮƽ����ľ���
	*  \param[in]			 minAngle								��������֧�ŵ���С�Ƕ�(��z��н�)����б�ȴ��ڴ���ֵ�Ż�����֧��
	*  \param[in]			 surroundingOffset						�ȹ�֧��ʱ���Χ����������ϵ��
	*  \param[in]			 topLineDis								Top����֧���߼��
	*  \param[in]			 topDownRate							Top����֧�Ÿ߶��½����ʣ�����ڲ��
	*  \param[in]			 topAngle								Top����֧��·���Ƕ�
	*  \param[in]			 midiumLineDis							Midium����֧���߼��
	*  \param[in]			 midiumDownRate							Midium����֧�Ÿ߶��½����ʣ�����ڲ��
	*  \param[in]			 midiumAngle							Midium����֧��·���Ƕ�
	*  \param[in]			 baseLineDis							Base����֧���߼��
	*  \param[in]			 baseDownRate							Base����֧�Ÿ߶��½����ʣ�����ڲ��
	*  \param[in]			 baseAngle								Base����֧��·���Ƕ�
	*  \param[in]			 solidSupport							�Ƿ�����ȹ�֧�ţ�����base֧���������Χ����
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

	/**	\brief DLP����ģ�ͻ���
	*	\param[in]				mesh								�����ɻ�������
	*	\param[out]				raft								���ɵĻ�������
	*	\param[in]				offset								�����ʵ��ķ���������������
	*	\param[in]				baseType                            ʹ�����ַ�ʽ���ɻ�����0����Χ�У�1��͹����2��ͶӰ�����ж�����3.ͶӰ�����޶�����
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API get_raft_boundary(
		const BaseTriMesh& mesh,
		std::vector<Point2Vector::Ptr>& raft,
		Scalar offset,
		int baseType
		);

	/** \brief ������Ƭ�õ����ײ��������ײ�֧�����ɶ�Ӧ��fdm����(�����֧�ţ����ɻ���һ��Ҫ������֧��֮�����)
	*  \param[in,out]		 entity														ʵ�壬������Ƭ�õ����������ݣ�����߽��Ҳ����entity��
	*  \param[in]			 margin														�����߽磨����ʵ��Ĳ��֣����
	*  \param[in]			 surroundingOffset											�������Χ��base���ͻ����ļ��
	*  \param[in]			 initialHeight												������ʼ�߶ȣ������߶ȣ�
	*  \param[in]			 baseLayers													base���ͻ����Ĳ���
	*  \param[in]			 baseLineDis												base���ͻ������߼��
	*  \param[in]			 baseThickness												base���ͻ����ĺ��
	*  \param[in]			 baseAngle													base���ͻ�������ת�Ƕ�
	*  \param[in]			 firstInterfaceLayers										firstInterface���ͻ����Ĳ���
	*  \param[in]			 firstInterfaceLineDis										firstInterface���ͻ������߼��
	*  \param[in]			 firstInterfaceThickness									firstInterface���ͻ����ĺ��
	*  \param[in]			 firstInterfaceAngle										firstInterface���ͻ�������ת�Ƕ�
	*  \param[in]			 secondInterfaceLayers										secondInterface���ͻ����Ĳ���
	*  \param[in]			 secondInterfaceLineDis										secondInterface���ͻ������߼��
	*  \param[in]			 secondInterfaceThickness									secondInterface���ͻ����ĺ��
	*  \param[in]			 secondInterfaceAngle										secondInterface���ͻ�������ת�Ƕ�
	*  \param[in]		     firstSupportBoundarys										��һ���֧����������
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

	/** \brief ����ͶӰ�����ͶӰ�ǹ��ĵ����ĵ�����
	*  \param[in]      K              ͶӰ���ڲ�
	*  \param[in]      imageSize      ͶӰͼƬ��x,y����ĳߴ�
	*  \param[in]      planeSize      ͶӰ���x,y�����ϵĳߴ�
	*  \param[out]     center         ͶӰ������ĵ�����
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_compute_projection_plane_cener(
		const std::vector<double>& K, 
		const Eigen::Vector2i& imageSize,
		const Point2& planeSize, 
		Point2& center
		);

	/** \brief ͨ��������ӡ��׼ģ�͵ĳߴ磬����DLP��ͶӰ�����̬�궨
	*  \param[in]      K              ͶӰ���ڲ�
	*  \param[in]      distortion     ͶӰ�ǻ���
	*  \param[in]      imageCoords    ͶӰ������ƽ��ı�ʾ��
	*  \param[in]   pId1, pId2, dis   ��pId1[i]��͵�pId2[i]����ͶӰ���ϵľ�����dis[i]
	*  \param[in]      height         ͶӰ�嵽ͶӰ�ǵľ���(�ο�ֵ)
	*  \param[in]      center         ͶӰ�����ͶӰ�ǹ��ĵ����ĵ�����
	*  \param[out]     mat            ͶӰ�嵽ͶӰ������ϵ��RT
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

	/** \brief ͨ��������ӡģ�͵���ά���꣬�궨DLPͶӰ�ǵ��������
	*  \param[in]      heightSamples      ��ӡģ����ͼƬ�����ϵĲ���
	*  \param[in]      widthSamples       ��ӡģ����ͼƬ����ϵĲ���
	*  \param[in]      coords             ͼƬ����heightSamples.size() * widthSamples.size() �������㡣coords����Щ������ʵ�ʴ�ӡ��
										  �����õ������ꡣcoords[i*heightSamples.size()+j]Ϊ������(widthSamples[j],heightSamples[i])�õ������ꡣ
	*  \param[in]      centerCoord        coords����ϵ�е����ĵ�����
	*  \param[in, out] K                  ͶӰ���ڲ�
	*  \param[out]     distortion         ͶӰ�ǻ���
	*  \param[out]     mat                ͶӰ�嵽ͶӰ������ϵ��RT
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

	/** \brief ͨ��������ӡģ�͵���ά���꣬�궨DLPͶӰ�ǵ��������
	*  \param[in]      height width       ͼƬ�ĳ��Ϳ�
	*  \param[in]      heightSamples      ��ӡģ����ͼƬ�����ϵĲ���
	*  \param[in]      widthSamples       ��ӡģ����ͼƬ����ϵĲ���
	*  \param[in]      coords             ͼƬ����heightSamples.size() * widthSamples.size() �������㡣coords����Щ������ʵ�ʴ�ӡ��
	                                      �����õ������ꡣcoords[i*heightSamples.size()+j]Ϊ������(widthSamples[j],heightSamples[i])�õ������ꡣ
	*  \param[in]      centerCoord        coords����ϵ�е����ĵ�����
	*  \param[in, out] K                  ͶӰ���ڲ�
	*  \param[out]     distortion         ͶӰ�ǻ���
	*  \param[out]     mat                ͶӰ�嵽ͶӰ������ϵ��RT
	*  \param[out]     lutH, lutW         ��������Ĳ��ұ�.���ȷ���Ϳ�ȷ����ʵ��λ��
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

	/** \brief ͨ����β�����ӡģ�͵���ά���꣬�궨DLPͶӰ�ǵ��������
	*  \param[in]      height width       ͼƬ�ĳ��Ϳ� ������1080 * 1920
	*  \param[in]      heightSamples      ��ӡģ����ͼƬ�����ϵĲ���
	*  \param[in]      data               �궨����Ҫ�Ĳ�������
	*  \param[in, out] K                  ͶӰ���ڲ�
	*  \param[out]     distortion         ͶӰ�ǻ���
	*  \param[out]     mat                ͶӰ�嵽ͶӰ������ϵ��RT
	*  \param[out]     lutH, lutW         ��������Ĳ��ұ�.���ȷ���Ϳ�ȷ����ʵ��λ��
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

	/** \brief �����ӡƽ���ͶӰͼ��ƽ���һһӳ���ϵ
	 *  \param[in]    p3D            ��ӡƽ���е�3ά��
	 *  \param[in]    p2D            ͼ��ƽ���е�2ά�� ��p3D��һһ��Ӧ��ϵ
	 *  \param[in]    imageSize      ͼ��ƽ��ĳ���.imageSize[0]Ϊ��,imageSize[1]Ϊ����
	 *  \param[in]    planeSize      ��ӡƽ��ĳ���.planeSize[0]Ϊ��x,planeSize[1]Ϊ��y.
	 *  \param[out]   table          ���صĲ��ұ�
	 *  \param[in]    smoothWeight   ƽ����Ȩ��.��Χ[0.1,0.5]
	 */
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_compute_map_relation(
		const std::vector<Eigen::Vector3>& p3D,
		const std::vector<Eigen::Vector2>& p2D,
		const Eigen::Vector2i& imageSize,
		const Eigen::Vector2& planeSize,
		DLPMapLookupTable& table,
		double smoothWeight = 0.3f
		);

	/** \brief �����ӡƽ�浽ͶӰ��ͼ��ƽ���ӳ���ϵ,�ýӿ������ڶ��ͶӰ�궨��ͼ��
	*  \param[in]        widthSamples      �궨��������ͼƬ����ϵĲ���
	*  \param[in]        heightSamples     �궨��������ͼƬ�߶��ϵĲ���
	*  \param[in]        offsets           ÿ�ű궨�����ݵ�ƫ����.offset[i][0]�ǿ��ϵ�ƫ����,offset[i][1]�Ǹ��ϵ�ƫ������������(0,0)ƫ�Ƶ�ͼƬ�������궨����ϵ��
	*  \param[in]        p3Ds              ���������ά�����ꡣ��offsets.size()*heightSamples.size()*widthSample.size()��������.�����������ȼ��Ӹߵ���width height offset.
	(-1,-1,-1)Ϊ��Ч�㡣����ʱ������������Ϊ(-1,-1,-1)
	*  \param[in]        imageSize         ͼ��ƽ��ĳ���.imageSize[0]Ϊ��,imageSize[1]Ϊ����
	*  \param[in]        planeSize         ��ӡƽ��ĳ���.planeSize[0]Ϊ��x,planeSize[1]Ϊ��y.
	*  \param[out]       table             ���صĲ��ұ�
	*  \param[in]        smoothWeight      ƽ����Ȩ��.��Χ[0.1,0.5]
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

	///��ʱ�ӿڣ�fileNames�ĳ��ȿ�����1��2��3��4.1�ǳ�ʼλ�ã�2��xƫ�ƣ�3��yƫ�ƣ�4��xy��ƫ�ơ�˳���ܴ�
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_create_DLPCalibrationData(
		const std::vector<std::string>& fileNames,
		std::vector<DLPCalibrationData>& data
		);

	/** \brief �õ�ÿ�����ƬͼƬ
	*	\param[out]				model									����ӡ��ģ��������Ϣ
	*	\param[out]				resImages								���ص���ƬͼƬ
	*	\param[in]				imageWidth								ͼƬ���
	*	\param[in]				imageHeight								ͼƬ����
	*	\param[in]				K										ͶӰ���ڲ�[fu, u0, v0, ar, skew]
	*	\param[in]				dis										ͶӰ�ǵĻ���[k1,k2,p1,p2,k3]
	*	\param[in]				mat										ͶӰ�嵽ͶӰ�ǵ�RT�任
	*   \param[in]              lutH, lutW                              ��������Ĳ��ұ�.���ȷ���Ϳ�ȷ����ʵ��λ��
	*	\param[in]				subPixelNum								��������
	*	\param[in]				enableGreyLevels						���ûҶȵȼ����ر���ֻ��0��255
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


	/**	\brief	��ͼƬ�Ҷ���У׼
	*  \param[out]          images								  ���ص���ƬͼƬ
	*  \param[in]           maxDecrease                           ���У׼ֵ��0~255��
	*  \param[in]           projDist							  ͶӰ���룬��λmm
	*  \param[in]           projWidth                             ͶӰ����ȣ���λmm
	*  \param[in]           projHeight                            ͶӰ���߶ȣ���λmm
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_func(
		ImageGreyb& image,
		Scalar maxDecrease,
		Scalar projDist = 160,
		Scalar projWidth = 144,
		Scalar projHeight = 81,
		Scalar milimeterPerPixel = 0.075
		);

	/**	\brief	��ͼƬ�Ҷ���У׼
	*  \param[out]          images								  ���ص���ƬͼƬ
	*  \param[in]           rowCount                              ͼƬ�ֳ�rowCount��
	*  \param[in]           colCount							  ͼƬ�ֳ�colCount��
	*  \param[in]           ratio								  ��ͼƬ��С����λ�ÿ�ʼ������ɨ�裬ÿ������ĻҶȵ���������ȡֵ0~1
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_grid_divide(
		ImageGreyb& image,
		int rowCount,
		int colCount,
		const std::vector<Scalar>& ratio
		);

	/**	\brief	��ͼƬ�Ҷ���У׼
	*  \param[out]          images								  ���ص���ƬͼƬ
	*  \param[in]           rowCount                              ͼƬ�ֳ�rowCount��
	*  \param[in]           colCount							  ͼƬ�ֳ�colCount��
	*  \param[in]           ratio								  ��ͼƬ��С����λ�ÿ�ʼ������ɨ�裬ÿ������ĻҶȵ���������ȡֵ0~1
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_grid_divide_centered(
		ImageGreyb& image,
		int rowCount,
		int colCount,
		const std::vector<Scalar>& ratio
		);

	/**	\brief	��ͼƬ�Ҷ���У׼
	*  \param[out]          images								  ���ص���ƬͼƬ
	*  \param[in]           radiusRatio							  ÿһ��pair����뾶r��У׼����ratio(ȡֵ0~1)����ʾС��r���Ҵ��ڸ�С��r'��������ʹ��ratio
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_image_calibrate_with_radius_divide(
		ImageGreyb& image,
		const std::vector<std::pair<Scalar, Scalar>>& radiusRatio
		);


	/** \brief ���õ���imageͼƬ�����DLP��ʶ���ͼƬ��ʽ
	*  \param[in]           fileName                              ͼƬ�����·�����ļ���
	*  \param[in]           image                                 �������ͼƬ
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_save_image(
		const std::string& fileName, 
		const ImageGreyb& image
		);

	/** \brief ����ͼƬ�ϵ����ڵ����
	*  \param[in]          image                                  �������ͼƬ
	*  \param[out]         num                                    ���صĽ��
	*/
	RetVal SN3D_PRINT3D_AUXILARY_API DLP_get_valid_pixel_num(
		const ImageGreyb& image, 
		double& num
		);
}

#endif