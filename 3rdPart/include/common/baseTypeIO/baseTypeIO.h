/***********************************************
 * ����һЩ�����������͵Ķ�д����
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_BASE_TYPE_IO_H
#define SN3D_ALGORITHM_COMMON_BASE_TYPE_IO_H

//STL
#include <string>
//3RD
#include <OpenMesh/Core/IO/Options.hh>
//SN3D
#include "../baseType/BaseTriMesh.h"
#include "../baseType/TriMesh.h"
#include "../baseType/RangeData.h"
#include "../baseType/Image.h"
#include "../baseType/PointImage.h"
#include "../baseType/RangeImage.h"

namespace Sn3DAlgorithm
{
	///��RangeImageд��mesh�ļ�
	int SN3D_COMMON_API write_mesh(const std::string& fileName, const RangeData& range, bool isBinary = true);

	///��asc����cloud��
	int SN3D_COMMON_API read_asc(const std::string& fileName, Cloud& cloud, bool hasColor, bool hasNormal);

	///����pointImage
	int SN3D_COMMON_API read_pointimage(const std::string& fileName , PointImage&oneFrame);


	///��cloudд��asc�ļ���
	int SN3D_COMMON_API write_asc(const std::string& fileName, const Cloud& cloud);

	/**  \brief ��pointImageд��asc�ļ���
	*   \param[out] isCompact                            �Ƿ�����ͼ�е���Ч�㣨0�㣩Ҳ���
	*/
	int SN3D_COMMON_API write_asc(const std::string& fileName, const PointImage& cloud , const bool isCompact);

	///UNDO:��ɫ��Ϣ���ܶ���
	int SN3D_COMMON_API read_mesh(const std::string& fileName, BaseTriMesh& mesh);
	
	///UNDO:��ɫ��Ϣ����д��
	int SN3D_COMMON_API write_mesh(const std::string& fileName, const BaseTriMesh& mesh, bool isBinary = true);

	///UNDO:��ʱֻ֧����ɫ������
	int SN3D_COMMON_API read_3mf_mesh(const std::string& meshFileName, BaseTriMesh& triMesh, bool withColor, bool withTexture);

	///UNDO:��ʱֻ֧����ɫ������ͬʱ����д������
	int SN3D_COMMON_API write_3mf_mesh(const std::string& meshFileName, BaseTriMesh& triMesh, bool withColor, bool withTexture);

	//! @{
	//! @name TriMesh ��д

	/*!
	 * @brief	��ģ���ļ���ȡ��TriMesh.
	 * @param [in] 	fileName	�ļ�ȫ��.
	 * @param [out]	mesh		����������.
	 * @return	�ɹ�����0.
	 */
	int SN3D_COMMON_API read_mesh(const std::string& fileName, TriMesh& mesh);

	/*!
	 * @brief	��TriMeshд�����ļ�.
	 * @param [in]	fileName 	�ļ�ȫ��.
	 * @param [in]	mesh	 	��д������.
	 * @param [in]	opt		 	(Optional) д��ѡ�ָ�������ơ�������ɫ�ȣ���OpenMesh.
	 * @param [in]	precision	(Optional) ASCIIд��ʱ������������.
	 * @return	�ɹ�����0.
	 * @note ���stl��Ҫд�������� opt����OpenMesh::IO::Options::Binary. objд����֧�ֶ�����
	 * 		 д��stl�ļ���֧��Options::VertexNormal
	 */
	int SN3D_COMMON_API write_mesh(const std::string& fileName, const TriMesh& mesh, OpenMesh::IO::Options opt = OpenMesh::IO::Options::Default, std::streamsize precision = 6);

	//! @}

	/*!
	 * @brief   	��ȡ rgb ͼ��
	 * @param[in]	filename	�ļ���
	 * @param[out]	image		�洢��ȡ��ͼ������
	 * @return		0-->����
	 *				1-->ʧ��
	 * @date    	2015-08
	 */
	int SN3D_COMMON_API read_rgb_image(const std::string& fileName, ImageRGBb& image);

	///��ȡ�Ҷ�ͼ��
	int SN3D_COMMON_API read_grey_image(const std::string& fileName, ImageGreyb& image);

	/*!
	 * @brief   	���� rgb ͼ��
	 * @param[in]	filename	�ļ���
	 * @param[in]	image		��д��ͼ������
	 * @return		0-->����
	 *				1-->ʧ��
	 * @date    	2015-08
	 */
	int SN3D_COMMON_API write_rgb_image(const std::string& fileName, const ImageRGBb& image);

	///����Ҷ�ͼ��
	int SN3D_COMMON_API write_grey_image(const std::string& fileName, const ImageGreyb& image);

	///��ȡ�������objģ�͡�obj�ļ�ֻ�������һ��mtllib��һ��usemtl
	/** \brief ��ȡ�������objģ�͡�obj�ļ�ֻ�������һ��mtllib��һ��usemtl
	 *  \return 0 ��ȷ��1 obj�ļ��д���2 ����������
	 */
	int SN3D_COMMON_API read_obj_with_texture(const std::string& fileName, BaseTriMesh& mesh);

	///���ɴ������objģ��
	int SN3D_COMMON_API write_obj_with_texture(const std::string& fileName, const BaseTriMesh& mesh);

	int SN3D_COMMON_API read_RigidMatrix(const std::string& fileName, RigidMatrix& mat);
	int SN3D_COMMON_API write_RigidMatrix(const std::string& fileName, const RigidMatrix& mat);

	int SN3D_COMMON_API read_point_image_to_rangeImage(const std::string& fileName, RangeImage& image, int height, int width, bool hasColor, bool hasNormal);

	int SN3D_COMMON_API write_asc(const std::string& fileName, const RangeImage& image);

	int SN3D_COMMON_API read_rangeImage(const std::string& fileName, RangeImage& image);
	///��׺��Ϊrig
	int SN3D_COMMON_API write_rangeImage(const std::string& fileName, const RangeImage& image);


	/////////////////////////////////////////////////��д����

	///stl�ļ��Ƿ��Ƕ����Ƶ�
	RetVal SN3D_COMMON_API stl_is_binary(const std::string& fileName, bool& isBinary);

} //namespace

#endif //SN3D_ALGORITHM_COMMON_BASE_TYPE_IO_H