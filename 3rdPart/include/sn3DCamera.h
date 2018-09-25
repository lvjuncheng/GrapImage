// ==========================================================================
// Copyright (C), 2009-2011, Shining 3D Tech. Co., Ltd.
//
// File name:	sn3DCamera.h
//
// Description:	 该模块是为三维扫描仪工业相机使用统一接口，目前使用了“大恒相机1303um/1351um”
//				 和“方成相机”
//
// Functions:	
//				
// Histroy:		2009/09/10 吴正强   三维扫描仪使用大恒相机
//				2010/03/10 魏琪瑜   增加方成相机和外触发
//				2011/03/25 吴正强	整理为独立库
//				2017/07/18 高成岗	增加海康网络相机、IP设置
//
// ============================================================================
#ifndef _sn3DCamera_H
#define _sn3DCamera_H

#include <string>
#include <vector>

#ifdef SN3DCAMERA_EXPORTS
#define SN3D_CAMERA_API __declspec(dllexport)
#else
#define SN3D_CAMERA_API __declspec(dllimport)
#endif

#pragma warning(disable: 4251)

enum SN_IMG_FORMAT
{
	FORMAT_MONO8 = 0x01,
	FORMAT_RAW = FORMAT_MONO8,
	FORMAT_MONO16 = 0x02,
	FORMAT_RGBRAW = 0x10,
	FORMAT_RGB888 = 0x11,
	FORMAT_RGB48 = 0x12,
};

enum SN_TRIG_MODE
{
	SOFTWARE_TRIGGER, // be triggered continue by device's internal signal
	HARDWARE_TRIGGER  // be triggered by external signal
};

enum CAMPARAM
{
	// Double
	// Bool
	CAMERA_PARAM_STROBEENABLE,
	// Int
	CAMERA_PARAM_STROBETIME,
	CAMERA_PARAM_STROBEDELAY,
	CAMERA_PARAM_TRIGGERPERIOD,
	CAMERA_PARAM_TRIGGERHIGHTIME,
	CAMERA_PARAM_SHARP,
	CAMERA_PARAM_GAMMA,
	CAMERA_PARAM_BTNSTATUS
	// Do
};
enum TIMESTAMP_SRC
{
	FROM_SOFTWARE,
	FROM_HARDWARE
};

namespace sn3DCamera
{
	// 相机对象句柄
	typedef void* HCAMERA;

	//========================================================================
	// 函数：	GetCurDeviceTotal
	// 参数：	devNum		[out]输出连接本机的相机数目
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   获取连接本机的相机数目
	//=========================================================================
	int SN3D_CAMERA_API GetDeviceTotal(HCAMERA hCamera, int& devNum);

	//=========================================================================
	// 函数：	CreateCamera
	// 参数：	hCamera		[out]输出创建的相机对象句柄
	//			cameraType	[in]相机类型。“FC”开头表示方成相机
	// 返回值： =0:			创建成功
	//			<0:         创建失败
	// 功能：   创建相机对象
	//=========================================================================
	int SN3D_CAMERA_API CreateCamera(HCAMERA& hCamera, const char* const& cameraType);

	//=========================================================================
	// 函数：	ReleaseCamera
	// 参数：	hCamera		[in]释放相机对象句柄
	// 功能：   释放相机对象
	//==========================================================================
	void SN3D_CAMERA_API ReleaseCamera(HCAMERA& hCamera);


	//=========================================================================
	// 函数：	OpenCamera
	// 参数：	hCamera		[in]相机对象句柄
	//			deviceNum   [in]将要打开的相机的序号
	//          serialNo	[in]将要打开的相机的序列号
	//			w,h         [in]相机图片的大小
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   打开相机。两种方式一是通过序号，而是通过序列号
	//==========================================================================
	int SN3D_CAMERA_API OpenCamera(const HCAMERA& hCamera, const int& deviceNum, const int& w, const int& h);
	int SN3D_CAMERA_API OpenCamera(const HCAMERA& hCamera, const char* const& serialNo, const int& w, const int& h);

	//=========================================================================
	// 函数：	SetPacketSize
	// 参数：	hCamera		[in]相机对象句柄
	//			nSize		[in]采集传输包大小
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   设置相机采集传输包大小。1394相机、网络相机会用到此参数。
	//==========================================================================
	int SN3D_CAMERA_API SetPacketSize(const HCAMERA& hCamera, const int& nSize);

	//=========================================================================
	// 函数：	StartCamera
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   开始拍摄，内部线程
	//==========================================================================
	int SN3D_CAMERA_API StartCamera(const HCAMERA& hCamera, const double& transSpeed);

	//=========================================================================
	// 函数：	CloseCamera
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   关闭相机
	//==========================================================================
	int SN3D_CAMERA_API CloseCamera(const HCAMERA& hCamera);

	
	//=========================================================================
	// 函数：	SnapShot
	// 参数：	hCamera		[in]相机对象句柄
	//			data		[out]返回数据，内部释放。大小为：图片长*图片宽(字节)
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   采集图片
	//==========================================================================
	int SN3D_CAMERA_API SnapShot(const HCAMERA& hCamera, unsigned char*& data);

	//=========================================================================
	// 函数：	GetSerialNo
	// 参数：	hCamera		[in]相机对象句柄
	//			serialNo	[out]返回相机序列号
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   采集图片
	//==========================================================================
	int SN3D_CAMERA_API GetSerialNo(const HCAMERA& hCamera, std::string& serialNo);
	
	//=========================================================================
	// 函数：	设置获取相机参数
	//
	//			SetExposureTime		设置曝光时间
	//			SetGain				设置增益
	//			GetGainRange		获取增益范围
	//			SetSnapSpeed		设置采集速度，大恒相机high/normal
	//			SetBlanking
	//			GetResolution
	//			SetBinningSize      设置binning的大小
	//          GetBinningSize      获取binning的大小
	//==========================================================================
	int SN3D_CAMERA_API SetExposureTime(const HCAMERA& hCamera, const long& exposureTime);
	int SN3D_CAMERA_API GetExposureTime(const HCAMERA& hCamera, long& exposureTime);
	int SN3D_CAMERA_API SetGain(const HCAMERA& hCamera, const int& gain);
	int SN3D_CAMERA_API GetGain(const HCAMERA& hCamera, int& gain);
	int SN3D_CAMERA_API GetGainRange(const HCAMERA& hCamera, int& min, int& max);
	int SN3D_CAMERA_API SetSnapSpeed(const HCAMERA& hCamera, const bool& bHigh);
	int SN3D_CAMERA_API SetBlanking(const HCAMERA& hCamera, const int& hor, const int& ver);
	int SN3D_CAMERA_API GetResolution(const HCAMERA& hCamera, int& w, int& h);
	int SN3D_CAMERA_API SetBinningSize(const HCAMERA& hCamera, int binningSize);
	int SN3D_CAMERA_API GetBinningSize(const HCAMERA& hCamera, int& binningSize);

	//=========================================================================
	// 函数：	设置采集触发模式
	//
	//			SetTriggerMode		    设置触发模式
	//			UnlockOthers			
	//			SetStart				
	//			SetEnd					
	//==========================================================================
	int SN3D_CAMERA_API SetTriggerMode(const HCAMERA& hCamera, const SN_TRIG_MODE& trigMode,  const int& parellelAddress,
		const int& highLevel, const int& lowLevel);
	int SN3D_CAMERA_API GetTriggerMode(const HCAMERA& hCamera, SN_TRIG_MODE& triggerMode);
	int SN3D_CAMERA_API SetStrobeMode(const HCAMERA& hCamera, const bool& bStrobe);
	int SN3D_CAMERA_API SetStrobeDelay(const HCAMERA& hCamera, const double& delay);
	int SN3D_CAMERA_API SetStrobeTime(const HCAMERA& hCamera, const double& time);
	int SN3D_CAMERA_API SetStrobeDelay(const HCAMERA& hCamera, const float& delay);
	int SN3D_CAMERA_API SetStrobeTime(const HCAMERA& hCamera, const float& time);
	void SN3D_CAMERA_API UnlockOthers(const HCAMERA& hCamera);
	void SN3D_CAMERA_API DiscardFrames(const HCAMERA& hCamera);
	void SN3D_CAMERA_API SetStart();
	void SN3D_CAMERA_API SetEnd();

	//=========================================================================
	// 函数：	SetColorMode, GetColorMode
	// 参数：	hCamera		[in]相机对象句柄
	//			colorMode	[in]相机色彩模式，如:FORMAT_MONO8, FORMAT_RGB888
	// 返回值： =0:			成功
	//			<0:         失败
	// 功能：   设置和获得相机的色彩模式
	//==========================================================================
	int SN3D_CAMERA_API SetColorMode(const HCAMERA& hCamera, const SN_IMG_FORMAT& imgFormat);
	int SN3D_CAMERA_API GetColorMode(const HCAMERA& hCamera, SN_IMG_FORMAT& imgFormat);

	//=========================================================================
	// 函数：	SetCameraToInitializedState
	// 参数：	hCamera		[in]相机对象句柄
	//			
	// 返回值： void
	// 功能：   恢复相机的状态至初始状态,包括Buffer的清空，状态标志的重置
	//==========================================================================
	void SN3D_CAMERA_API SetCameraToInitializedState(const HCAMERA& hCamera);

	//=========================================================================
	// 函数：	GetCurrentFrameRate
	// 参数：	hCamera		[in]相机对象句柄
	//			
	// 返回值： double, 返回负数表示失败
	// 功能：   获取当前相机的帧率(目前仅PointGrey支持)
	//==========================================================================
	double SN3D_CAMERA_API GetCurrentFrameRate(const HCAMERA& hCamera);

	//=========================================================================
	// 函数：	AutoWhiteBalance
	// 参数：	hCamera		[in]相机对象句柄
	//			bOperateOrGet [in]指示进行白平衡计算(true)还是设置参数(false)
	//			rRatio [in]红色通道比例
	//			gRatio [in]绿色通道比例
	//			bRatio [in]蓝色通道比例
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   相机执行一次白平衡(仅支持彩色相机操作)
	//==========================================================================
	bool SN3D_CAMERA_API AutoWhiteBalance(const HCAMERA& hCamera, const bool& bOperateOrGet, float& rRatio, float& gRatio, float& bRatio);

	//=========================================================================
	// 函数：	AutoExposure
	// 参数：	hCamera		[in]相机对象句柄
	//			bAutoExp    [in]开启(true)/关闭(false)自动曝光
	//			bAutoGain   [in]开启(true)/关闭(false)自动增益
	//			destLight	[in]目标亮度
	//          minExp      [in]最小曝光时间
	//          maxExp      [in]最大曝光时间
	//          minGain     [in]最小增益
	//          maxGain     [in]最大增益
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置相机自动曝光开启或者关闭
	//==========================================================================
	bool SN3D_CAMERA_API AutoExposureAndGain(const HCAMERA& hCamera, bool bAutoExp, bool bAutoGain,
		int destLight = 128, double minExp = 16.6, double maxExp = 33.3,int minGain = 1, int maxGain = 50);

	//=========================================================================
	// 函数：	AutoGain
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置相机自动增益开启或者关闭
	//==========================================================================
	bool SN3D_CAMERA_API AutoGain(const HCAMERA& hCamera, bool bAuto, int destLight = 128, int minGain = 1, int maxGain = 50);

	//=========================================================================
	// 函数：	DoFlip
	// 参数：	hCamera		[in]相机对象句柄
	//			bHFlip      [in]横向翻转(true)
	//			bVFlip		[in]纵向翻转(true)
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   相机执行一次白平衡(仅支持彩色相机操作)
	//==========================================================================
	bool SN3D_CAMERA_API DoFlip(const HCAMERA& hCamera, const bool& bHFlip, const bool& bVFlip);

	//=========================================================================
	// 函数：	SplitImage
	// 参数：	hCamera		[in]相机对象句柄
	//			destImages  [in, out] 存放分离后图片数据的buffer
	//          destWidths  [in, out] 存放分离后图片的宽度值(顺序与图片顺序一一对应)
	//          destHeights [in, out] 存放分离后图片的高度值(顺序与图片顺序一一对应)
	//          destImgFormats [in, out] 存放分离后图片的像素格式(顺序与图片顺序一一对应)
	//          srcImageData   [in] 待分离的原始图片数据
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   使用指定相机分图功能来分离图片(仅支持多合一相机)
	//==========================================================================
	bool SN3D_CAMERA_API SplitImage(const HCAMERA& hCamera, std::vector<unsigned char*>& destImages, std::vector<int>& destWidths,
		std::vector<int>& destHeights, std::vector<SN_IMG_FORMAT>& destImgFormats, unsigned char* srcImageData);

	//=========================================================================
	// 函数：	LockSnapThread
	// 参数：	hCamera		[in]相机对象句柄
	//			bLock       [in]锁定线程(true)				
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   锁定或解锁相机采图线程
	//==========================================================================
	bool SN3D_CAMERA_API LockSnapThread(const HCAMERA& hCamera, bool bLock);

	//=========================================================================
	// 函数：	GetDeviceType
	// 参数：	hCamera		[in]相机对象句柄
	//			DeviceType  [out]返回相机类型，sensor型号			
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   获取相机类型
	//==========================================================================
	bool SN3D_CAMERA_API GetDeviceType(const HCAMERA& hCamera, int& DeviceType);

	//=========================================================================
	// 函数：	SetGIGEIP
	// 参数：	hCamera			[in]相机对象句柄
	//			nIP				[in]IP地址(true)
	//			nSubNetMask		[in]子网掩码(true)
	//			nDefaultGateWay	[in]默认网关(true)
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置IP地址；子网掩码；默认网关
	//==========================================================================
	int SN3D_CAMERA_API SetGIGEIP(const HCAMERA& hCamera, unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay);

	//=========================================================================
	// 函数：	IsGIGEDevice
	// 参数：	hCamera		[in]相机对象句柄
	//			nIndex      [in]设备索引号
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   判断设备是否是GIGE相机 
	//==========================================================================
	bool SN3D_CAMERA_API IsGIGEDevice(const HCAMERA& hCamera, unsigned int nIndex);

	//=========================================================================
	// 函数：	SetSnapWaitTime
	// 参数：	hCamera		[in]相机对象句柄
	//			nTime       [in]等待时间
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置Grab模式采图线程等待时间  
	//==========================================================================
	bool SN3D_CAMERA_API SetSnapWaitTime(const HCAMERA& hCamera, unsigned int nTime);

	//=========================================================================
	// 函数：	ResetCameraState
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   重置相机可调整的内部状态到初始状态 
	//==========================================================================
	bool SN3D_CAMERA_API ResetCameraState(const HCAMERA& hCamera);

	//=========================================================================
	// 函数：	SpecifiedCameraTimeStampSource
	// 参数：	hCamera		    [in]相机对象句柄
	//          timeStampSource	[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   指定相机的时间戳来源 
	//==========================================================================
	bool SN3D_CAMERA_API SpecifiedCameraTimeStampSource(const HCAMERA& hCamera, TIMESTAMP_SRC timeStampSource);

	//=========================================================================
	// 函数：	SetCameraDoubleParam/GetCameraDoubleParam
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置/获取指定参数(双浮点数据类型) 
	//==========================================================================
	bool SN3D_CAMERA_API SetCameraDoubleParam(const HCAMERA& hCamera, CAMPARAM camParam, double paramVal);
	bool SN3D_CAMERA_API GetCameraDoubleParam(const HCAMERA& hCamera, CAMPARAM camParam, double& paramVal);

	//=========================================================================
	// 函数：	SetCameraIntParam/SetCameraIntParam
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置/获取指定参数(整数数据类型)
	//==========================================================================
	bool SN3D_CAMERA_API SetCameraIntParam(const HCAMERA& hCamera, CAMPARAM camParam, int paramVal);
	bool SN3D_CAMERA_API GetCameraIntParam(const HCAMERA& hCamera, CAMPARAM camParam, int& paramVal);

	//=========================================================================
	// 函数：	SetCameraBoolParam/GetCameraBoolParam
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   设置/获取指定参数值(布尔数据类型)
	//==========================================================================
	bool SN3D_CAMERA_API SetCameraBoolParam(const HCAMERA& hCamera, CAMPARAM camParam, bool paramVal);
	bool SN3D_CAMERA_API GetCameraBoolParam(const HCAMERA& hCamera, CAMPARAM camParam, bool& paramVal);

	//=========================================================================
	// 函数：	CameraDo
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   执行指定操作
	//==========================================================================
	bool SN3D_CAMERA_API CameraDo(const HCAMERA& hCamera, CAMPARAM camParam);

	//=========================================================================
	// 函数：	Raw2RGB
	// 参数：	hCamera		[in]相机对象句柄
	// 返回值： bool, 返回false表示失败,否则成功
	// 功能：   执行指定操作
	//==========================================================================
	bool SN3D_CAMERA_API Raw2RGB(const HCAMERA& hCamera, unsigned char*& rgbImage, const unsigned char* rawImage, int width, int height);

	/*************************************************************************************/
}


#endif //_sn3DCamera_H