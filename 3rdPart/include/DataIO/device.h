/*******************************************************************
* \brief 该文件定义设备检测功能类
* \note  功能为检测Gpu设备，看设备能否达到算法运行的最低要求
* \author 黄磊杰
*******************************************************************/

#ifndef SN3D_GRAPHICS_ABOUTDEVICE_H
#define  SN3D_GRAPHICS_ABOUTDEVICE_H

namespace Ly3DGPUAlgorithm
{
	namespace device
	{
		class AboutDevice
		{
		public:
			AboutDevice();
			int device_detect();
		};
	}//namespace Rebuild
}//namespace Sn3DGraphics
#endif // !SN3D_GRAPHICS_ABOUTDEVICE_H
