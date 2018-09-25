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
	// ������	sn3DReadImage
	// 
	// ������	fileName	[in]���ͼ��
	//			image		[out]����ͼ��
	//		
	// ���ܣ���ȡͼƬ
	//===========================================================================
	int SN3D_IMAGE_API sn3DReadImage( const char* fileName, sn3DImageData<unsigned char>& image );

	//===========================================================================
	// ������	sn3DWriteImage
	// 
	// ������	fileName	[in]���ͼ��
	//			image		[in]����ͼ��
	//		
	// ���ܣ�����ͼƬ
	//===========================================================================
	int SN3D_IMAGE_API sn3DWriteImage( const char* fileName, sn3DImageData<unsigned char>& image );
	
}





#endif //_sn3DRecognition_H