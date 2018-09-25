// RasterImages.h: interface for the CRasterImages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RASTERIMAGES_H__21A79CD7_6934_4E51_B916_351DBEAC9227__INCLUDED_)
#define AFX_RASTERIMAGES_H__21A79CD7_6934_4E51_B916_351DBEAC9227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "ConfigFile.h"


class ImageData
{
public:
	ImageData();
	~ImageData();
	void Build(int w, int h, int center, int lightLow,int lightHigh, bool bHorz);
	void Cross(int w, int h, int black=0,int white=255);
	
	void White(int w, int h, int white=0);
	void Red(int w, int h, int white=0);
	void Green(int w, int h, int white=0);
	void Blue(int w, int h, int white=0);

	void Copy(unsigned char *pData, int w, int h);

protected:
	void alloc(int w, int h);

public:
	int width;
	int height;

	unsigned char *data;
	double pixelWidth;
	int moveId;
	int moveCount;

	bool R;
	bool G;
	bool B;
};

class CRasterImages  
{
public:
	CRasterImages();
	virtual ~CRasterImages();
	void SetSize(int w, int h)			{m_width=w, m_height=h;};
	//void SetIni(CCaptureSetting *set)	{ m_ini = set;};

	void Draw( int i, bool bPic = false);
	void DrawTest();

	void Build(int lightlow, int lighthigh);// 预生成一系列投影条纹图
	void BuildTest(int w, int lightlow, int lighthigh);// 测试用的投影条纹图
	void BuildWhite(int lighthigh  );
	void BuildRed(int lighthigh  );
	void BuildGreen(int lighthigh  );
	void BuildBlue(int lighthigh  );
	void BuildCross( int lightlow, int lighthigh );

	void InitPictureNum(int num);
	void SetPictureData(void **ppDataArray, int w, int h);
	void SetPictureData(void *pData, int w, int h, int index);

protected:
	void glDrawRGB(int w, int h, unsigned char *data);

public:
	ImageData *m_ImageArray;
	
	int m_width;
	int m_height;
	int m_rasterType;
	int m_zeroPosition;
	int m_stepNum;
	bool m_bHorz;

	ImageData *m_PicsArray;
	int m_PicsNum;

protected:
	static ImageData m_TestImage;
	//CCaptureSetting *m_ini;

	/*int m_w;
	int m_h;*/
	//int m_lightLow;
	//int m_lightHigh;

	//sn3DProjector::ProjectType m_prjType;
};

#endif // !defined(AFX_RASTERIMAGES_H__21A79CD7_6934_4E51_B916_351DBEAC9227__INCLUDED_)
