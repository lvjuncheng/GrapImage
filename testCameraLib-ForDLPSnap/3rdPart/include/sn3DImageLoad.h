// File: sn3DImageLoad.h

#ifndef _sn3DImageLoad_H
#define _sn3DImageLoad_H


#ifdef SN3DIMAGELOAD_EXPORTS
#define SN3D_IMAGE_API __declspec(dllexport)
#else
#define SN3D_IMAGE_API __declspec(dllimport)
#endif

#include "sn3DImageData.h"
using namespace sn3DCore;

namespace sn3DImageLoad
{
	//===========================================================================
	// 函数：	sn3DReadImage
	// 
	// 参数：	fileName	[in]输出图像
	//			image		[out]输入图像
	//		
	// 功能：读取图片
	//===========================================================================
	int SN3D_IMAGE_API sn3DReadImage( const char* fileName, sn3DImageData<unsigned char>& image );

	//===========================================================================
	// 函数：	sn3DWriteImage
	// 
	// 参数：	fileName	[in]输出图像
	//			image		[in]输入图像
	//		
	// 功能：保存图片
	//===========================================================================
	int SN3D_IMAGE_API sn3DWriteImage( const char* fileName, sn3DImageData<unsigned char>& image );
	
}





#endif //_sn3DRecognition_H