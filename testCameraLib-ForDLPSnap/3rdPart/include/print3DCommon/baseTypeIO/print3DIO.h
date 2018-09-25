/***********************************************
 * �������������������
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_BASETYPE_IO_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_BASETYPE_IO_H


#include <vector>
#include <string>
#include "../baseType/Print3DTraits.h"
#include "../baseType/Entity.h"
#include "../baseType/Layer.h"
#include "../../common/baseType/BaseTriMesh.h"

namespace Sn3DAlgorithm
{

	///���幤�սṹ����
	enum SN3D_PRINT3D_COMMON_API StructureType
	{
		StructureType_BOUNDARY, ///<����
		StructureType_EXTRASHELL, ///<�ڱ�
		StructureType_FILL, ///<���
		StructureType_SUPPORT, ///<֧��
		StructureType_RAFT, ///<����
	};


	/**	\brief ����Ƭ������G�����ʽ���
	*	\param[in]		gcodeFileName			����·��
	*	\param[in]		entity					��Ƭʵ��
	*	\param[in]		decimalPlaces			����С�����λ��
	*	\param[in]		structSequence			ָ���������սṹ�����˳��
	*	\param[in]		assignColour			�Ƿ�ָ����ɫ��������(black), ������(red), �ڱ�(blue), ���(green), ֧��(orange), ֧�Ű�Χ(yellow), ���������ɫ��ʾ
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_gcode(
		const std::string& gcodeFileName,
		const Entity& entity,
		const size_t decimalPlaces,
		const std::vector<StructureType>& structSequence, 
		const bool assignColour);


	/** \brief ����Ƭ������G�����ʽ���
	*  \param[in]     gcodeFileName		����·��
	*  \param[in]     Layer						һ������
	*  \param[in]     decimalPlaces			����С�����λ��
	*  \param[in]     structSequence       ָ���������սṹ�����˳��
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_gcode(
		const std::string& gcodeFileName,
		const Layer& layer,
		const size_t decimalPlaces,
		const std::vector<StructureType>& structSequence);


	/** \brief ͨ���ļ�����
	*  \param[in]     fileName				�ļ�·��
	*  \param[in]     text						�ļ�����
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_common_file(
		const std::string& fileName,
		const std::vector<std::string>& text);

	/** \brief ͨ���ļ�����
	*  \param[in]     fileName				�ļ�·��
	*  \param[in]     text						�ļ�����
	 */
	RetVal SN3D_PRINT3D_COMMON_API write_common_file(
		const std::string& fileName,
		const std::string& text);

	/** \brief ������ɫ��entityת��ɴ���ɫ��BaseTriMesh������鿴��ɫ��������
	 *  \param[in] extraShellWidth  extraShell������ʾ���߿�
	 *  \param[in] fillWidth        ���������ʾ���߿�
	 */
	RetVal SN3D_PRINT3D_COMMON_API convert_entity_to_baseTriMesh_with_color(const Entity& entity, BaseTriMesh& mesh, 
		float extraShellWidth, float fillWidth);

	/** \brief ���ļ���������������ģ�ʹ���BaseTriMesh
	*	\param[in]		meshFileName		�ļ���������չ����ȡ���ͣ�
	*	\param[out]		triMesh				��BaseTriMesh��ʽ�洢����������
	*	\param[in]		deflection			stpģ�����ǻ�����ֻ�ж�ȡstpģ�Ͳ���Ҫָ��
	*	\note: Ŀǰֻ֧��stl��obj���������桢���ߡ�����ȣ�,3mf, stp
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh(
		const std::string& meshFileName,
		BaseTriMesh& triMesh,
		Scalar deflectionCoeff = 0.004
		);

	/** \brief ���ļ���������������ģ�ʹ���BaseTriMesh
	*	\param[in]		meshFileName		�ļ���������չ����ȡ���ͣ�
	*	\param[out]		triMesh				��BaseTriMesh��ʽ�洢����������
	*	\param[in]		processCallBack		�������ص�����
	*	\note: Ŀǰֻ֧��stl��obj���������桢���ߡ�����ȣ�,3mf, stp
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh(
		const std::string& meshFileName,
		BaseTriMesh& triMesh,
		std::function<void(const int&)> processCallBack
		);


	/** \brief ���ļ���������������ģ�ʹ���BaseTriMesh
	*	\param[in]		meshFileName		�ļ���
	*	\param[out]		triMesh				��BaseTriMesh��ʽ�洢����������
	*	\param[in]		ext					ָ���ļ���չ���������ļ�����չ��ȡ����
	*	\param[in]		deflection			stpģ�����ǻ�����ֻ�ж�ȡstpģ�Ͳ���Ҫָ��
	*	\note: Ŀǰֻ֧��stl��obj���������桢���ߡ�����ȣ�,3mf, stp
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh(
		const std::string& meshFileName,
		BaseTriMesh& triMesh,
		const std::string& ext,
		Scalar deflectionCoeff = 0.004,
		std::function<void(const int&)> callback = nullptr
		);


	/** \brief ���ڴ����ݶ�����������ģ�ʹ���BaseTriMesh
	*  \param[in]     stl_data		�ڴ�����
	*	\praram[in]		ext			�������ͣ�֧��stl,obj
	*  \param[out]   triMesh					��BaseTriMesh��ʽ�洢����������
	*  \note: Ŀǰֻ֧��stl��obj���������桢���ߡ�����ȣ�
	*/
	RetVal SN3D_PRINT3D_COMMON_API read_TriMesh_in_memory(
		const std::vector<unsigned char>& mesh_data,
		const std::string& ext,
		BaseTriMesh& triMesh
		);
	
	/** \brief ����������д���ļ��������κ����ݣ�
	*  \param[in]     meshFileName		�ļ���
	*  \param[in]	  triMesh					��BaseTriMesh��ʽ�洢����������
	*  \UNDO:Ŀǰֻ֧��binary-stl�����������assic-stl��obj
	*/
	RetVal SN3D_PRINT3D_COMMON_API write_TriMesh(
		const std::string& meshFileName,
		const BaseTriMesh& triMesh
		);

	/** \brief ����������д���ڴ滺����
	*  \param[in]     meshFileName		�ļ���
	*  \param[in]	  triMesh					��BaseTriMesh��ʽ�洢����������
	*  \UNDO:Ŀǰֻ֧��binary-stl�����������assic-stl��obj
	*/
	RetVal SN3D_PRINT3D_COMMON_API write_TriMesh_to_memory(
		std::vector<unsigned char>& memBuf,
		const BaseTriMesh& triMesh
		);
}
#endif //SN3D_ALGORITHM_PRINT3D_COMMON_BASETYPE_IO_H
