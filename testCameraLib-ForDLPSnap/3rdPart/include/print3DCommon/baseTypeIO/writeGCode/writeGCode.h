/***********************************************
 * 以G代码格式输出Entity
 * \author 唐庭阁
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
		///从一个Entity获得所有层的gcode
		RetVal get_gcode_from_entity(const Entity& entity);
		///从一个Layer获得一层的gcode
		RetVal get_gcode_from_layer(const Layer& layer);
		void set_assign_colour(bool v) {_assignColour = v;}
	private:
		RetVal get_layer_gcode(const Layer& layer);
		RetVal get_path_gcode(const Path& path, StructureType& st);

	private:
		size_t _decimalPlace;///<数据精度，即小数点后保留几位
		std::vector<StructureType> _structSequence;///<指明各个工艺结构的输出顺序
		std::string _output;///<作为每一层gcode的临时存放
		std::vector<std::string> _outputs;///<包含每一层的gcode
		size_t _totalRowCnt;///<当前所有gcode行数统计
		bool _assignColour; ///<指定颜色：外轮廓(black), 内轮廓(red), 内壁(blue), 填充(green), 支撑(orange), 支撑包围(yellow), 剩余(purple, brown, pink, dimgray)
	};




}
#endif //SN3D_ALGORITHM_PRINT3D_COMMON_WRITE_GCODE_H
