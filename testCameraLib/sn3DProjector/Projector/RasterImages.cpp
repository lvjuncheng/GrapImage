// RasterImages.cpp: implementation of the CRasterImages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "RasterImages.h"
#include "gl/glew.h"
#include "gl/glut.h"


//#include "gl/glaux.h"
//#include "Utility/ImageConvert.h"
//#include <windows.h>
//#include <gl/GL.h>


//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
ImageData::ImageData()
	:width(0)
	,height(0)
	,moveId(0)
	,moveCount(0)
{ 
	
	data = NULL;
	R = G = B = true;
	pixelWidth = 80;
}
ImageData::~ImageData()
{ 
	if( data ) 
		delete[] data;
}
void ImageData::alloc(int w, int h)
{
	if( data )
		delete[] data;
	data = new unsigned char[w*h*3];
}

void ImageData::Build(int w, int h,int center, int lightLow,int lightHigh, bool bHorz)
{	
	double centerPhase = 2.0*M_PI*moveId/moveCount;
	double deltaPixel = 2.0*M_PI/pixelWidth;
	double startPhase = centerPhase-deltaPixel*(center);
	double range = (lightHigh-lightLow)/2.0;

	unsigned char *d;
	int inStep, outStep, inNum, outNum;
	if (bHorz)	{inStep = 3;outStep = 3*w;inNum = w;outNum = h;}
	else	{inStep = 3*w;outStep = 3;inNum = h;outNum = w;}

	alloc( w, h );
	for( int i=0; i<outNum; i++)
	{
		double cosN = cos( startPhase + deltaPixel*i )+1; //范围为【0 2】
		int color = (int)(range*cosN+lightLow+0.5);
		if( color>255)
			color = 255;
		if( color<0 )
			color = 0;
		d = data+i*outStep;
		for( int j=0; j<inNum; j++)
		{
			//unsigned char *d = data+j*w*3+i*3;
			if( R )
				*(d + 0 ) = color;
			else
				*(d + 0 ) = 0;

			if( G )
				*(d + 1 ) = color;
			else
				*(d + 1 ) = 0;

			if( B )
				*(d + 2 ) = color;
			else
				*(d + 2 ) = 0;

			d += inStep;
		}
	}

	//double centerPhase = 2.0*M_PI*moveId/moveCount;
	//double deltaPixel = 2.0*M_PI/pixelWidth;
	//double startPhase = centerPhase-deltaPixel*(center);
	//double range = (lightHigh-lightLow)/2.0;
	//
	//alloc( w, h );
	//for( int i=0; i<w; i++)
	//{
	//	double cosN = cos( startPhase + deltaPixel*i )+1; //范围为【0 2】
	//	int color = (int)(range*cosN+lightLow+0.5);
	//	if( color>255)
	//		color = 255;
	//	if( color<0 )
	//		color = 0;
	//	for( int j=0; j<h; j++)
	//	{
	//		unsigned char *d = data+j*w*3+i*3;
	//		if( R )
	//			*(d + 0 ) = color;
	//		else
	//			*(d + 0 ) = 0;
	//	
	//		if( G )
	//			*(d + 1 ) = color;
	//		else
	//			*(d + 1 ) = 0;
	//		
	//		if( B )
	//			*(d + 2 ) = color;
	//		else
	//			*(d + 2 ) = 0;
	//	}
	//}
}

void ImageData::Cross(int w, int h, int black,int white)
{
	White( w, h, white );

	int lineWide = 5;
	int hh = (200<(h-lineWide)) ? 200 : (h-lineWide);
	int ww = (200<(w-lineWide)) ? 200 : (w-lineWide);

	int i,j;
	int startY = (h-hh)/2;
	int startX = (w-lineWide)/2;

	for ( j=0; j<hh; j++)
	{
		for( i=0; i<lineWide; i++)
		{
			unsigned char *d = data+(j+startY)*w*3+(i+startX)*3;
			*(d + 0 ) = black;
			*(d + 1 ) = black;
			*(d + 2 ) = black;
		}
	}
	startY = (h-lineWide)/2;
	startX = (w-ww)/2;
	for ( j=0; j<lineWide; j++)
	{
		for( i=0; i<ww; i++)
		{
			unsigned char *d = data+(j+startY)*w*3+(i+startX)*3;
			*(d + 0 ) = black;
			*(d + 1 ) = black;
			*(d + 2 ) = black;
		}
	}
}
void ImageData::White(int w, int h, int white)
{
	alloc( w, h );
	for( int i=0; i<w; i++)
	{
		for( int j=0; j<h; j++)
		{
			unsigned char *d = data+j*w*3+i*3;
			*(d + 0 ) = (unsigned char)white;
			*(d + 1 ) = (unsigned char)white;
			*(d + 2 ) = (unsigned char)white;
		}
	}
}

void ImageData::Red(int w, int h, int white)
{
	alloc( w, h );
	for( int i=0; i<w; i++)
	{
		for( int j=0; j<h; j++)
		{
			unsigned char *d = data+j*w*3+i*3;
			*(d + 0 ) = (unsigned char)255;
			*(d + 1 ) = (unsigned char)0;
			*(d + 2 ) = (unsigned char)0;
		}
	}
}
void ImageData::Green(int w, int h, int white)
{
	alloc( w, h );
	for( int i=0; i<w; i++)
	{
		for( int j=0; j<h; j++)
		{
			unsigned char *d = data+j*w*3+i*3;
			*(d + 0 ) = (unsigned char)0;
			*(d + 1 ) = (unsigned char)255;
			*(d + 2 ) = (unsigned char)0;
		}
	}
}
void ImageData::Blue(int w, int h, int white)
{
	alloc( w, h );
	for( int i=0; i<w; i++)
	{
		for( int j=0; j<h; j++)
		{
			unsigned char *d = data+j*w*3+i*3;
			*(d + 0 ) = (unsigned char)0;
			*(d + 1 ) = (unsigned char)0;
			*(d + 2 ) = (unsigned char)255;
		}
	}
}

void ImageData::Copy(unsigned char *pData, int w, int h)
{
	if (data != NULL)
		delete[] data;

	data = new unsigned char[w*h*3];
	memcpy(data, pData, w*h*3);
	width = w;
	height = h;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageData CRasterImages::m_TestImage;

CRasterImages::CRasterImages()
	:m_width(0)
	,m_height(0)
	,m_rasterType(0)
	,m_zeroPosition(0)
	,m_stepNum(0)
{
	m_ImageArray = NULL;
	//m_lightLow  = 0;
	//m_lightHigh = 255;
	m_bHorz = false;

	m_PicsArray = NULL;
	m_PicsNum = 0;
}	

CRasterImages::~CRasterImages()
{
	if( m_ImageArray )
		delete[] m_ImageArray;

	if (m_PicsArray != NULL)
		delete[] m_PicsArray;

	m_PicsNum = 0;
}
//----------------------------------------------------------------------------
// Draw
void CRasterImages::glDrawRGB(int w, int h, unsigned char *data)
{
	glViewport(0,0, w, h);						// 重置当前的视口	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(  w, h,GL_RGB,GL_UNSIGNED_BYTE,  data);

}
void CRasterImages::Draw( int i, bool bPic)
{
	if (true/*bPic*/)
	{
		if (i >= 0 && i < m_PicsNum)
		{
			glDrawRGB(m_PicsArray[i].width, m_PicsArray[i].height, m_PicsArray[i].data);
		}
		return ;
	}
	return;
	int count=0;
	for( int t=0; t<m_stepNum && t<7; t++)
	{
	//	count += m_step[t].moveNum;
	}

	if( i< count)
	{
		glDrawRGB( m_width, m_height,m_ImageArray[i].data);
	}
}
void CRasterImages::DrawTest()
{
	if (m_TestImage.data != NULL)
		glDrawRGB(m_width, m_height,m_TestImage.data);
}

//----------------------------------------------------------------------------
// Build
void CRasterImages::Build(int lightlow, int lighthigh)
{
	return;
	//int count  = framecount;
	int center = m_zeroPosition;
	int lightLow  = lightlow;
	int lightHigh = lighthigh;
	
	int count=0;
	for( int i=0; i<m_stepNum && i<7; i++)
	{
	//	count += m_step[i].moveNum;
	}

	if( m_ImageArray )
		delete[] m_ImageArray;
	m_ImageArray = new ImageData[count];

	// 设置参数
	int id=0;
/*	for( int m=0; m<m_stepNum; m++)
	{
		for( int n=0; n<m_step[m].moveNum; n++)
		{
			m_ImageArray[id].moveCount  = m_step[m].moveNum;
			if (m_rasterType == 1)
			{
				m_ImageArray[id].pixelWidth = (double)m_step[m].pixelWidth;
			}
			else
			{
				m_ImageArray[id].pixelWidth = (double)(m_bHorz ? m_height : m_width)/m_step[m].frangeNum;
			}
			m_ImageArray[id].moveId = n; 
			id++;
		}
	}*/

	// char szName[128];
	// 生成图片
	for (int t=0; t<count; t++)
	{
		m_ImageArray[t].Build( m_width, m_height, center, lightLow, lightHigh, m_bHorz);
	//	sprintf(szName,"project%d.bmp",t);
	//	CImageConvert::SaveBMP( szName, m_ImageArray[t].data, m_w, m_h);
	}
}

void CRasterImages::BuildTest(int w, int lightlow, int lighthigh)
{
	int lightLow  = lightlow;
	int lightHigh = lighthigh;

	if( w > 0)
		m_TestImage.pixelWidth = w;

	m_TestImage.moveCount = 1;
	m_TestImage.moveId = 0; 
	m_TestImage.Build( m_width, m_height, m_zeroPosition, lightLow, lightHigh, false);
}

void CRasterImages::BuildWhite(int lighthigh )
{
	m_TestImage.White(m_width, m_height, lighthigh);
}
void CRasterImages::BuildRed(int lighthigh  )
{
	m_TestImage.Red(m_width, m_height, lighthigh);
}
void CRasterImages::BuildGreen(int lighthigh  )
{
	m_TestImage.Green(m_width, m_height, lighthigh);
}
void CRasterImages::BuildBlue(int lighthigh  )
{
	m_TestImage.Blue(m_width, m_height, lighthigh);
}
void CRasterImages::BuildCross(int lightlow, int lighthigh )
{
	int lightLow  = lightlow;
	int lightHigh = lighthigh;
	m_TestImage.Cross(m_width, m_height, lightLow,  lightHigh);
}
void CRasterImages::InitPictureNum(int num)
{
	if (num <= 0 || num == m_PicsNum)
		return ;

	if (m_PicsArray != NULL ) 
	{
		delete[] m_PicsArray;
		m_PicsNum = 0;
	}

	m_PicsArray = new ImageData[num];
	m_PicsNum = num;
}

void CRasterImages::SetPictureData(void **ppDataArray, int w, int h)
{
	for (int i = 0; i < m_PicsNum; i++)
	{
		m_PicsArray[i].Copy((unsigned char*)ppDataArray[i], w, h);
	}
}

void CRasterImages::SetPictureData(void *pData, int w, int h, int index)
{
	if (index >= 0 && index < m_PicsNum)
		m_PicsArray[index].Copy((unsigned char*)pData, w, h);
}