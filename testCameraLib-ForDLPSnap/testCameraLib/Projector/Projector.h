// ==========================================================================
// Copyright (C), 2009-2011, Shining 3D Tech. Co., Ltd.
//
// File name:	sn3DProjector.h
//
// Description:	 
//
// Functions:	
//				
// Histroy:		
//				2011/03/25 ����ǿ	����Ϊ������
//
// ============================================================================
#include "BaseWindow.h"
#include "ProjectWnd.h"
#include <Windows.h>
#include <vector>

class CRasterImagesHolder
{
public:
	CRasterImagesHolder():m_bSelected(false){}

public:
	CRasterImages m_images;
	bool m_bSelected;
};

class Projector
{
public:
	int  CreateProjectorWindow( HINSTANCE  hInst);
	int  ReCreateProjectorWindow();
	void DestroyProjectorWindow();

	void CreateRasterImages(int iCount);
	void SelectRasterImages(int i, bool bSel);
	void SwitchRasterImages(int i);

	void BuildImages();
	void  PorjectorDisplay(int index);
	//��������
	void SetSize(int hight, int width);
    void SetRasterType(int rastertype, bool bHorz);
	void SetDoubleExposure(int doubleexposure);
	void SetLightRange(int lightlow, int lighthigh);
	/*void SetWaitTime(int waittime);
	void SetAdjustTime(int adjusttime);*/
	void SetZeroPosition(int zeropositon);
	void SetStepNum(int stepnum);
	void SetTExposure(int *texposure);
	void SetGain(int * gain);
	void SetTestStripPixels(int widthPixel);
	//�������
	void GetProjectSize(int *w, int *h);

	void InitPictureNum(int num);
	void SetPictureData(void **ppDataArray, int w, int h);
	void SetPictureData(void *pData, int w, int h, int index);
private:
	CProjectWnd *m_proWnd;
	
	int m_stripWidth;
	int m_lightLow;
	int m_lightHigh;
	std::vector<CRasterImagesHolder> m_ImagesHolderArray;
	
public:
	static Projector* m_projecter;
	static Projector * GetInstance();
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
public:
	class CGarbo // ����Ψһ��������������������ɾ��CSingleton��ʵ��  
	{  
	public:  ~CGarbo()  
			 {  
				 if (Projector::m_projecter)  
					 delete Projector::m_projecter;
				 Projector::m_projecter = NULL;
			 }  
	};  
	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������  } 
};

