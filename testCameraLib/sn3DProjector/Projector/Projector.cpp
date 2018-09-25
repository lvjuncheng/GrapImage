// File: Projector.cpp

#include "stdafx.h"
#include "Projector.h"

Projector* Projector::m_projecter = NULL;

Projector * Projector::GetInstance()
{
	if ( m_projecter == NULL )
	{
		m_projecter = new Projector();
		return m_projecter;
	}
	return m_projecter;
}
int  Projector::CreateProjectorWindow( HINSTANCE  hInst)
{
	m_proWnd = new CProjectWnd( hInst );
	m_proWnd->RegisterWindow();
	m_proWnd->Create();
	return 0;
}

int Projector::ReCreateProjectorWindow()
{
	if (m_proWnd == NULL)
		return -1;

	m_proWnd->Destroy();
	m_proWnd->Create();

	return 0;
}

void  Projector::DestroyProjectorWindow()
{
	m_proWnd->Destroy();
	delete m_proWnd;
	m_proWnd = NULL;
}
void Projector::SetTestStripPixels(int widthPixel)
{
	m_stripWidth = widthPixel;
}
void Projector::GetProjectSize(int *w, int *h)
{
	*w = m_proWnd->m_width;
	*h = m_proWnd->m_height;
}
void Projector::InitPictureNum(int num)
{
	m_customImages.InitPictureNum(num);
	m_proWnd->SetRasterImages(&m_customImages);
}
void Projector::SetPictureData(void **ppDataArray, int w, int h)
{
	m_customImages.SetPictureData(ppDataArray, w, h);
}
void Projector::SetPictureData(void *pData, int w, int h, int index)
{
	m_customImages.SetPictureData(pData, w, h, index);
}
void Projector::SetSize(int hight, int width)
{
	/*m_height = hight;
	m_width  = width;*/

}
void Projector::SetRasterType(int rastertype, bool bHorz)
{
	for (size_t i = 0; i < m_ImagesHolderArray.size(); i++)
	{
		if (m_ImagesHolderArray[i].m_bSelected )
		{
			m_ImagesHolderArray[i].m_images.m_rasterType = rastertype;
			m_ImagesHolderArray[i].m_images.m_bHorz = bHorz;
		}
	}
}
void Projector::SetDoubleExposure(int doubleexposure)
{
	//m_doubleExposure = doubleexposure;

}
void Projector::SetLightRange(int lightlow, int lighthigh)
{
	m_lightLow = lightlow;
	m_lightHigh= lighthigh;
	//BuildImages();
}
//void Projector::SetWaitTime(int waittime)
//{
//	m_waitTime = waittime;
//
//}
//void Projector::SetAdjustTime(int adjusttime)
//{
//	m_adjustTime = adjusttime;
//}
void Projector::SetZeroPosition(int zeropositon)
{
	for (size_t i = 0; i < m_ImagesHolderArray.size(); i++)
	{
		if (m_ImagesHolderArray[i].m_bSelected)
		{
			m_ImagesHolderArray[i].m_images.m_zeroPosition = zeropositon;
		}
	}
}
void Projector::SetStepNum(int stepnum)
{
	for (size_t i = 0; i < m_ImagesHolderArray.size(); i++)
	{
		if (m_ImagesHolderArray[i].m_bSelected)
		{
			m_ImagesHolderArray[i].m_images.m_stepNum = stepnum;
		}
	}
}
void Projector::SetTExposure(int *texposure)
{
	/*for (int i=0; i<4; i++)
	{
		m_tExposure[i] = texposure[i];
	}*/

}
void Projector::SetGain(int * gain)
{
	/*for (int i=0; i<4; i++)
	{
		m_gain[i] = gain[i];
	}*/

}
void Projector::CreateRasterImages(int iCount)
{
	m_ImagesHolderArray.resize(iCount);
	if (m_proWnd != NULL)
	{
		for (size_t i = 0; i < m_ImagesHolderArray.size(); i++)
		{
			m_ImagesHolderArray[i].m_images.SetSize(m_proWnd->m_width, m_proWnd->m_height);
		}
	}
}
void Projector::SelectRasterImages(int i, bool bSel)
{
	if (i < (int)m_ImagesHolderArray.size())
	{
		m_ImagesHolderArray[i].m_bSelected = bSel;
	}
}
void Projector::SwitchRasterImages(int i)
{
	m_proWnd->SetRasterImages(&m_customImages);
	return;
	if (i < (int)m_ImagesHolderArray.size())
	{
		m_proWnd->SetRasterImages(&m_ImagesHolderArray[i].m_images);
	}
}
void Projector::BuildImages()
{
	for (size_t i = 0; i < m_ImagesHolderArray.size(); i++)
	{
		if (m_ImagesHolderArray[i].m_bSelected)
		{
			m_ImagesHolderArray[i].m_images.Build(m_lightLow, m_lightHigh);
		}
	}
}
void Projector::PorjectorDisplay(int index)
{
	m_proWnd->RenderImages(index, true);
	return;
	if (index < 100)
	{
		m_proWnd->RenderImages(index, true);
	}
	else
	{
		switch (index)
		{
		case 0:break;
		case 1:
			m_ImagesHolderArray[0].m_images.BuildWhite(m_lightHigh);break;
		case 2:
			m_ImagesHolderArray[0].m_images.BuildWhite(0);break;
		case 3:
			m_ImagesHolderArray[0].m_images.BuildRed(m_lightHigh);break;
		case 4:
			m_ImagesHolderArray[0].m_images.BuildGreen(m_lightHigh);break;
		case 5:
			m_ImagesHolderArray[0].m_images.BuildBlue(m_lightHigh);break;
		case 6:
			m_ImagesHolderArray[0].m_images.BuildTest(m_stripWidth, m_lightLow, m_lightHigh);break;
		case 7:
			m_ImagesHolderArray[0].m_images.BuildCross(m_lightLow, m_lightHigh);break;
		default:break;
		}
		m_proWnd->RenderTest();
	}
}





