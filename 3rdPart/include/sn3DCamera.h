// ==========================================================================
// Copyright (C), 2009-2011, Shining 3D Tech. Co., Ltd.
//
// File name:	sn3DCamera.h
//
// Description:	 ��ģ����Ϊ��άɨ���ǹ�ҵ���ʹ��ͳһ�ӿڣ�Ŀǰʹ���ˡ�������1303um/1351um��
//				 �͡����������
//
// Functions:	
//				
// Histroy:		2009/09/10 ����ǿ   ��άɨ����ʹ�ô�����
//				2010/03/10 κ���   ���ӷ���������ⴥ��
//				2011/03/25 ����ǿ	����Ϊ������
//				2017/07/18 �߳ɸ�	���Ӻ������������IP����
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
	// ���������
	typedef void* HCAMERA;

	//========================================================================
	// ������	GetCurDeviceTotal
	// ������	devNum		[out]������ӱ����������Ŀ
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   ��ȡ���ӱ����������Ŀ
	//=========================================================================
	int SN3D_CAMERA_API GetDeviceTotal(HCAMERA hCamera, int& devNum);

	//=========================================================================
	// ������	CreateCamera
	// ������	hCamera		[out]������������������
	//			cameraType	[in]������͡���FC����ͷ��ʾ�������
	// ����ֵ�� =0:			�����ɹ�
	//			<0:         ����ʧ��
	// ���ܣ�   �����������
	//=========================================================================
	int SN3D_CAMERA_API CreateCamera(HCAMERA& hCamera, const char* const& cameraType);

	//=========================================================================
	// ������	ReleaseCamera
	// ������	hCamera		[in]�ͷ����������
	// ���ܣ�   �ͷ��������
	//==========================================================================
	void SN3D_CAMERA_API ReleaseCamera(HCAMERA& hCamera);


	//=========================================================================
	// ������	OpenCamera
	// ������	hCamera		[in]���������
	//			deviceNum   [in]��Ҫ�򿪵���������
	//          serialNo	[in]��Ҫ�򿪵���������к�
	//			w,h         [in]���ͼƬ�Ĵ�С
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   ����������ַ�ʽһ��ͨ����ţ�����ͨ�����к�
	//==========================================================================
	int SN3D_CAMERA_API OpenCamera(const HCAMERA& hCamera, const int& deviceNum, const int& w, const int& h);
	int SN3D_CAMERA_API OpenCamera(const HCAMERA& hCamera, const char* const& serialNo, const int& w, const int& h);

	//=========================================================================
	// ������	SetPacketSize
	// ������	hCamera		[in]���������
	//			nSize		[in]�ɼ��������С
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   ��������ɼ��������С��1394���������������õ��˲�����
	//==========================================================================
	int SN3D_CAMERA_API SetPacketSize(const HCAMERA& hCamera, const int& nSize);

	//=========================================================================
	// ������	StartCamera
	// ������	hCamera		[in]���������
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   ��ʼ���㣬�ڲ��߳�
	//==========================================================================
	int SN3D_CAMERA_API StartCamera(const HCAMERA& hCamera, const double& transSpeed);

	//=========================================================================
	// ������	CloseCamera
	// ������	hCamera		[in]���������
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   �ر����
	//==========================================================================
	int SN3D_CAMERA_API CloseCamera(const HCAMERA& hCamera);

	
	//=========================================================================
	// ������	SnapShot
	// ������	hCamera		[in]���������
	//			data		[out]�������ݣ��ڲ��ͷš���СΪ��ͼƬ��*ͼƬ��(�ֽ�)
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   �ɼ�ͼƬ
	//==========================================================================
	int SN3D_CAMERA_API SnapShot(const HCAMERA& hCamera, unsigned char*& data);

	//=========================================================================
	// ������	GetSerialNo
	// ������	hCamera		[in]���������
	//			serialNo	[out]����������к�
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   �ɼ�ͼƬ
	//==========================================================================
	int SN3D_CAMERA_API GetSerialNo(const HCAMERA& hCamera, std::string& serialNo);
	
	//=========================================================================
	// ������	���û�ȡ�������
	//
	//			SetExposureTime		�����ع�ʱ��
	//			SetGain				��������
	//			GetGainRange		��ȡ���淶Χ
	//			SetSnapSpeed		���òɼ��ٶȣ�������high/normal
	//			SetBlanking
	//			GetResolution
	//			SetBinningSize      ����binning�Ĵ�С
	//          GetBinningSize      ��ȡbinning�Ĵ�С
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
	// ������	���òɼ�����ģʽ
	//
	//			SetTriggerMode		    ���ô���ģʽ
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
	// ������	SetColorMode, GetColorMode
	// ������	hCamera		[in]���������
	//			colorMode	[in]���ɫ��ģʽ����:FORMAT_MONO8, FORMAT_RGB888
	// ����ֵ�� =0:			�ɹ�
	//			<0:         ʧ��
	// ���ܣ�   ���úͻ�������ɫ��ģʽ
	//==========================================================================
	int SN3D_CAMERA_API SetColorMode(const HCAMERA& hCamera, const SN_IMG_FORMAT& imgFormat);
	int SN3D_CAMERA_API GetColorMode(const HCAMERA& hCamera, SN_IMG_FORMAT& imgFormat);

	//=========================================================================
	// ������	SetCameraToInitializedState
	// ������	hCamera		[in]���������
	//			
	// ����ֵ�� void
	// ���ܣ�   �ָ������״̬����ʼ״̬,����Buffer����գ�״̬��־������
	//==========================================================================
	void SN3D_CAMERA_API SetCameraToInitializedState(const HCAMERA& hCamera);

	//=========================================================================
	// ������	GetCurrentFrameRate
	// ������	hCamera		[in]���������
	//			
	// ����ֵ�� double, ���ظ�����ʾʧ��
	// ���ܣ�   ��ȡ��ǰ�����֡��(Ŀǰ��PointGrey֧��)
	//==========================================================================
	double SN3D_CAMERA_API GetCurrentFrameRate(const HCAMERA& hCamera);

	//=========================================================================
	// ������	AutoWhiteBalance
	// ������	hCamera		[in]���������
	//			bOperateOrGet [in]ָʾ���а�ƽ�����(true)�������ò���(false)
	//			rRatio [in]��ɫͨ������
	//			gRatio [in]��ɫͨ������
	//			bRatio [in]��ɫͨ������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ���ִ��һ�ΰ�ƽ��(��֧�ֲ�ɫ�������)
	//==========================================================================
	bool SN3D_CAMERA_API AutoWhiteBalance(const HCAMERA& hCamera, const bool& bOperateOrGet, float& rRatio, float& gRatio, float& bRatio);

	//=========================================================================
	// ������	AutoExposure
	// ������	hCamera		[in]���������
	//			bAutoExp    [in]����(true)/�ر�(false)�Զ��ع�
	//			bAutoGain   [in]����(true)/�ر�(false)�Զ�����
	//			destLight	[in]Ŀ������
	//          minExp      [in]��С�ع�ʱ��
	//          maxExp      [in]����ع�ʱ��
	//          minGain     [in]��С����
	//          maxGain     [in]�������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ��������Զ��ع⿪�����߹ر�
	//==========================================================================
	bool SN3D_CAMERA_API AutoExposureAndGain(const HCAMERA& hCamera, bool bAutoExp, bool bAutoGain,
		int destLight = 128, double minExp = 16.6, double maxExp = 33.3,int minGain = 1, int maxGain = 50);

	//=========================================================================
	// ������	AutoGain
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ��������Զ����濪�����߹ر�
	//==========================================================================
	bool SN3D_CAMERA_API AutoGain(const HCAMERA& hCamera, bool bAuto, int destLight = 128, int minGain = 1, int maxGain = 50);

	//=========================================================================
	// ������	DoFlip
	// ������	hCamera		[in]���������
	//			bHFlip      [in]����ת(true)
	//			bVFlip		[in]����ת(true)
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ���ִ��һ�ΰ�ƽ��(��֧�ֲ�ɫ�������)
	//==========================================================================
	bool SN3D_CAMERA_API DoFlip(const HCAMERA& hCamera, const bool& bHFlip, const bool& bVFlip);

	//=========================================================================
	// ������	SplitImage
	// ������	hCamera		[in]���������
	//			destImages  [in, out] ��ŷ����ͼƬ���ݵ�buffer
	//          destWidths  [in, out] ��ŷ����ͼƬ�Ŀ��ֵ(˳����ͼƬ˳��һһ��Ӧ)
	//          destHeights [in, out] ��ŷ����ͼƬ�ĸ߶�ֵ(˳����ͼƬ˳��һһ��Ӧ)
	//          destImgFormats [in, out] ��ŷ����ͼƬ�����ظ�ʽ(˳����ͼƬ˳��һһ��Ӧ)
	//          srcImageData   [in] �������ԭʼͼƬ����
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ʹ��ָ�������ͼ����������ͼƬ(��֧�ֶ��һ���)
	//==========================================================================
	bool SN3D_CAMERA_API SplitImage(const HCAMERA& hCamera, std::vector<unsigned char*>& destImages, std::vector<int>& destWidths,
		std::vector<int>& destHeights, std::vector<SN_IMG_FORMAT>& destImgFormats, unsigned char* srcImageData);

	//=========================================================================
	// ������	LockSnapThread
	// ������	hCamera		[in]���������
	//			bLock       [in]�����߳�(true)				
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ��������������ͼ�߳�
	//==========================================================================
	bool SN3D_CAMERA_API LockSnapThread(const HCAMERA& hCamera, bool bLock);

	//=========================================================================
	// ������	GetDeviceType
	// ������	hCamera		[in]���������
	//			DeviceType  [out]����������ͣ�sensor�ͺ�			
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ��ȡ�������
	//==========================================================================
	bool SN3D_CAMERA_API GetDeviceType(const HCAMERA& hCamera, int& DeviceType);

	//=========================================================================
	// ������	SetGIGEIP
	// ������	hCamera			[in]���������
	//			nIP				[in]IP��ַ(true)
	//			nSubNetMask		[in]��������(true)
	//			nDefaultGateWay	[in]Ĭ������(true)
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ����IP��ַ���������룻Ĭ������
	//==========================================================================
	int SN3D_CAMERA_API SetGIGEIP(const HCAMERA& hCamera, unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay);

	//=========================================================================
	// ������	IsGIGEDevice
	// ������	hCamera		[in]���������
	//			nIndex      [in]�豸������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   �ж��豸�Ƿ���GIGE��� 
	//==========================================================================
	bool SN3D_CAMERA_API IsGIGEDevice(const HCAMERA& hCamera, unsigned int nIndex);

	//=========================================================================
	// ������	SetSnapWaitTime
	// ������	hCamera		[in]���������
	//			nTime       [in]�ȴ�ʱ��
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ����Grabģʽ��ͼ�̵߳ȴ�ʱ��  
	//==========================================================================
	bool SN3D_CAMERA_API SetSnapWaitTime(const HCAMERA& hCamera, unsigned int nTime);

	//=========================================================================
	// ������	ResetCameraState
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ��������ɵ������ڲ�״̬����ʼ״̬ 
	//==========================================================================
	bool SN3D_CAMERA_API ResetCameraState(const HCAMERA& hCamera);

	//=========================================================================
	// ������	SpecifiedCameraTimeStampSource
	// ������	hCamera		    [in]���������
	//          timeStampSource	[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ָ�������ʱ�����Դ 
	//==========================================================================
	bool SN3D_CAMERA_API SpecifiedCameraTimeStampSource(const HCAMERA& hCamera, TIMESTAMP_SRC timeStampSource);

	//=========================================================================
	// ������	SetCameraDoubleParam/GetCameraDoubleParam
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ����/��ȡָ������(˫������������) 
	//==========================================================================
	bool SN3D_CAMERA_API SetCameraDoubleParam(const HCAMERA& hCamera, CAMPARAM camParam, double paramVal);
	bool SN3D_CAMERA_API GetCameraDoubleParam(const HCAMERA& hCamera, CAMPARAM camParam, double& paramVal);

	//=========================================================================
	// ������	SetCameraIntParam/SetCameraIntParam
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ����/��ȡָ������(������������)
	//==========================================================================
	bool SN3D_CAMERA_API SetCameraIntParam(const HCAMERA& hCamera, CAMPARAM camParam, int paramVal);
	bool SN3D_CAMERA_API GetCameraIntParam(const HCAMERA& hCamera, CAMPARAM camParam, int& paramVal);

	//=========================================================================
	// ������	SetCameraBoolParam/GetCameraBoolParam
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ����/��ȡָ������ֵ(������������)
	//==========================================================================
	bool SN3D_CAMERA_API SetCameraBoolParam(const HCAMERA& hCamera, CAMPARAM camParam, bool paramVal);
	bool SN3D_CAMERA_API GetCameraBoolParam(const HCAMERA& hCamera, CAMPARAM camParam, bool& paramVal);

	//=========================================================================
	// ������	CameraDo
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ִ��ָ������
	//==========================================================================
	bool SN3D_CAMERA_API CameraDo(const HCAMERA& hCamera, CAMPARAM camParam);

	//=========================================================================
	// ������	Raw2RGB
	// ������	hCamera		[in]���������
	// ����ֵ�� bool, ����false��ʾʧ��,����ɹ�
	// ���ܣ�   ִ��ָ������
	//==========================================================================
	bool SN3D_CAMERA_API Raw2RGB(const HCAMERA& hCamera, unsigned char*& rgbImage, const unsigned char* rawImage, int width, int height);

	/*************************************************************************************/
}


#endif //_sn3DCamera_H