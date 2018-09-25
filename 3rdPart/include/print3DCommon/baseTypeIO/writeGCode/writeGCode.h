/***********************************************
 * ��G�����ʽ���Entity
 * \author ��ͥ��
 ***********************************************/

#ifndef SN3D_ALGORITHM_PRINT3D_COMMON_WRITE_GCODE_H
#define SN3D_ALGORITHM_PRINT3D_COMMON_WRITE_GCODE_H


#include <vector>
#include "baseType/Print3DTraits.h"
#include "baseType/PointType.h"
#include "baseType/Entity.h"
#include "baseType/Layer.h"
#include "baseType/Path.h"
#include "baseTypeIO/print3DIO.h"
#include <fstream>
#include <iostream>



namespace Sn3DAlgorithm
{

	class SN3D_PRINT3D_COMMON_CLASS GCode
	{
	public:
		GCode() 
		{
			_decimalPlace = 4;
			_totalRowCnt = 0;
			_assignColour = false;
		};
		GCode(size_t decimal, const std::vector<StructureType>& structSequence)
		{
			_decimalPlace = decimal;
			_structSequence = structSequence;
			_totalRowCnt = 0;
		}
		
	public:
		//////////////////////////////////////////////////////////////////////////get and set
		Scalar get_decimal_place() const { return _decimalPlace; }
		void set_decimal_place(Scalar decimal) { _decimalPlace = decimal; }
		const std::vector<StructureType>& get_struct_sequence() const { return _structSequence; }
		void set_structSequence(const std::vector<StructureType>& stVector) { _structSequence = stVector; }
		const std::vector<std::string>& get_outputs() const {return _outputs;}
		///��һ��Entity������в��gcode
		RetVal get_gcode_from_entity(const Entity& entity);
		///��һ��Layer���һ���gcode
		RetVal get_gcode_from_layer(const Layer& layer);
		void set_assign_colour(bool v) {_assignColour = v;}
	private:
		RetVal get_layer_gcode(const Layer& layer);
		RetVal get_path_gcode(const Path& path, StructureType& st);

	private:
		size_t _decimalPlace;///<���ݾ��ȣ���С���������λ
		std::vector<StructureType> _structSequence;///<ָ���������սṹ�����˳��
		std::string _output;///<��Ϊÿһ��gcode����ʱ���
		std::vector<std::string> _outputs;///<����ÿһ���gcode
		size_t _totalRowCnt;///<��ǰ����gcode����ͳ��
		bool _assignColour; ///<ָ����ɫ��������(black), ������(red), �ڱ�(blue), ���(green), ֧��(orange), ֧�Ű�Χ(yellow), ʣ��(purple, brown, pink, dimgray)
	};




}
#endif //SN3D_ALGORITHM_PRINT3D_COMMON_WRITE_GCODE_H
