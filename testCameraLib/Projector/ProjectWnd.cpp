// ProjectWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "Monitors.h"
//#include "MultiMonitor.h"
//#include "MonitorDC.h"
#include "ProjectWnd.h"

#include "windows.h"  //吴军--

//#include "gl/glew.h"
#include "gl/glew.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectWnd
CProjectWnd::CProjectWnd(HINSTANCE hInst, CONST WNDCLASSEX* wcx) 
:CBaseWindow(hInst, wcx)
{ 
	SetWindowTitle(_T("Derived Window"));
	m_imges = NULL;
	m_hCursor = NULL;
	m_backColor = 0;
	m_hGLContext = NULL;
	m_hDC = NULL;
	m_GLPixelIndex = 0;
	m_idProject = 0;
	m_width = 800;
	m_height = 600;
};

CProjectWnd::~CProjectWnd()
{
	Destroy();
}

BOOL CProjectWnd::RegisterWindow()
{
	WNDCLASSEX wcx; 

	// Fill in the window class structure with default parameters 
	wcx.cbSize = sizeof(WNDCLASSEX);							// size of structure 
	wcx.style = CS_HREDRAW | CS_VREDRAW;						// redraw if size changes 
	wcx.lpfnWndProc = CBaseWindow::stWinMsgHandler;				// points to window procedure 
	wcx.cbClsExtra = 0;											// no extra class memory 
	wcx.cbWndExtra = 0;											// no extra window memory 
	wcx.hInstance = hInstance;									// handle to instance 
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
	wcx.lpszMenuName = NULL;									// name of menu resource 
	wcx.lpszClassName = _T("BaseWindow");							// name of window class 
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				// small class icon 

	// Register the window class. 
	return CBaseWindow::RegisterWindow(&wcx); 

}
BOOL CProjectWnd::Create()
{ 	// Create the window
	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = 0;
	rect.bottom = 0;

	return CBaseWindow::Create(WS_VISIBLE|WS_POPUP, &rect);
}


////////////////////////////////////////////////////////////////////////////
void CProjectWnd::Destroy()
{
	if( !bWindowClosed )
		::DestroyWindow( m_hwnd );
	bWindowClosed = TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CProjectWnd message handlers

int CProjectWnd::OnCreate() 
{
	//if (CWnd::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	
    // load nifty cursor
    m_hCursor = LoadCursor(NULL,IDC_ARROW);
 
    // we'll want to do this in OnActivate too
    SetCursor(m_hCursor);
    ShowCursor(true);

	POINT point_m;
	point_m.x = 0;
	point_m.y = 0;

	HMONITOR Primary_hMonitor = ::MonitorFromPoint( point_m, MONITOR_DEFAULTTOPRIMARY );
	MONITORINFO  Primary_mi;     
	RECT        Primary_rect;
	Primary_mi.cbSize = sizeof( Primary_mi );
	::GetMonitorInfo( Primary_hMonitor, & Primary_mi );	
	Primary_rect = Primary_mi.rcMonitor;
	int x = Primary_rect.right - Primary_rect.left;
	POINT point_s;
	point_s.x = x;
	point_s.y = 0;
	HMONITOR second_hMonitor = ::MonitorFromPoint( point_s, MONITOR_DEFAULTTONEAREST );
	MONITORINFO  second_mi;   
	RECT        second_rect;	
	second_mi.cbSize = sizeof( second_mi );	
	::GetMonitorInfo( second_hMonitor, &second_mi );	
	second_rect = second_mi.rcMonitor;
	int pixelsX = second_rect.right - second_rect.left;	
	int pixelsY = second_rect.bottom - second_rect.top;
	::MoveWindow(m_hwnd, x, 0, pixelsX,  pixelsY, TRUE);
 
	//RECT rect;
	//CMonitors monits;
	//CMonitor monitor=monits.GetPrimaryMonitor();
	//CMonitor second = monits.GetMonitor(monits.GetCount()-1);
	//monitor.GetMonitorRect( &rect );
	//int x=rect.right-rect.left;
	//second.GetMonitorRect( &rect );
	//int pixelsX = rect.right-rect.left;
	//int pixelsY = rect.bottom-rect.top;
	//::MoveWindow(m_hwnd, x, 0, pixelsX,  pixelsY, TRUE);

	//m_imges->SetSize(pixelsX, pixelsY); //投影图片大小设置
	m_width = pixelsX;
	m_height = pixelsY;
  
	// since this window has its own DC we can stash it away... 
	m_hDC = ::GetDC(m_hwnd);


	// opengl 初始化
	if(SetWindowPixelFormat(m_hDC)==FALSE)
		return 0;
	if(CreateViewGLContext(m_hDC)==FALSE)
		return 0;

	return 0;
}

//********************************************
// OnPaint
//********************************************
void CProjectWnd::OnPaint() 
{
 	RenderTest();
}
//********************************************
// OnDestroy
// Cleanup every OpenGL rendering context
//********************************************
void CProjectWnd::OnDestroy() 
{
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	ReleaseDC(m_hwnd,m_hDC);
	//CWnd::OnDestroy();
}

//********************************************
// OnSize
//********************************************
void CProjectWnd::OnSize(UINT nType, int cx, int cy) 
{
	if(wglMakeCurrent(m_hDC,m_hGLContext)==FALSE)
		return ;
}

//********************************************
// SetWindowPixelFormat
//********************************************
BOOL CProjectWnd::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
			return FALSE;
	}
	
	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))
		return FALSE;
	
	return TRUE;
}

//********************************************
// CreateViewGLContext
// Create an OpenGL rendering context
//********************************************
BOOL CProjectWnd::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	
	if(m_hGLContext==NULL)
		return FALSE;
	
	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
	{
	//	int b = GetLastError();
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////
//////////////////////////////////////////////
// PAINTING 
//////////////////////////////////////////////
//////////////////////////////////////////////

void CProjectWnd::RenderImages(int i, bool bPic)
{
	//HDC hDC = ::GetDC(m_hwnd);
	//m_hGLContext = wglCreateContext(hDC);
	if(wglMakeCurrent(m_hDC,m_hGLContext)==FALSE)
	{
	//	DWORD err = GetLastError();
		return ;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_imges->Draw(i, bPic);
	glFinish();
	
	SwapBuffers(m_hDC);
	//::ReleaseDC( m_hwnd,hDC);
	wglMakeCurrent(m_hDC, NULL);
	return;
}
/////////////////////////////////////////////////
//void CProjectWnd::BuildTest(sn3DProjector::ProjectType type,int lighthigh, int lightlow, int stripWidth )
//{
//	if( type == PT_NONE )
//		type = m_proType;
//	else
//		m_proType = type;
//
//	if (m_rasIndex >= 0)
//	{
//		if( type == PT_WHITE )
//			m_ImagesArray[m_rasIndex].BuildWhite( lighthigh );
//		else if (type == PT_RED)
//			m_ImagesArray[m_rasIndex].BuildRed(lighthigh);
//		else if (type == PT_GREEN)
//			m_ImagesArray[m_rasIndex].BuildGreen(lighthigh);
//		else if (type == PT_BLUE)
//			m_ImagesArray[m_rasIndex].BuildBlue(lighthigh);
//		else if( type == PT_STRIP )
//			m_ImagesArray[m_rasIndex].BuildTest( stripWidth,lightlow,lighthigh );
//		else if( type == PT_CROSS)
//			m_ImagesArray[m_rasIndex].BuildCross( lightlow, lighthigh );
//	}
//	else if (m_rasIndex == -1)
//	{
//		for (size_t i = 0; i < m_ImagesArray.size(); i++)
//		{
//			if( type == PT_WHITE )
//				m_ImagesArray[i].BuildWhite( lighthigh );
//			else if (type == PT_RED)
//				m_ImagesArray[i].BuildRed(lighthigh);
//			else if (type == PT_GREEN)
//				m_ImagesArray[i].BuildGreen(lighthigh);
//			else if (type == PT_BLUE)
//				m_ImagesArray[i].BuildBlue(lighthigh);
//			else if( type == PT_STRIP )
//				m_ImagesArray[i].BuildTest( stripWidth,lightlow,lighthigh );
//			else if( type == PT_CROSS)
//				m_ImagesArray[i].BuildCross( lightlow, lighthigh );
//		}
//	}
//
//	RenderTest();
//	return ;
//}
void CProjectWnd::RenderTest()
{
	if(wglMakeCurrent(m_hDC,m_hGLContext)==FALSE)
		return ;
	m_imges->DrawTest();
	SwapBuffers(m_hDC);
	wglMakeCurrent(m_hDC, NULL);
	
}

void CProjectWnd::DrawRect(float color, float xScale, float yScale)
{
	float r[][2]={
		{-1.0f, 1.0f},
		{ 1.0f, 1.0f},
		{ 1.0f,-1.0f},
		{-1.0f,-1.0f}};

	glViewport(0,0, 800, 600);						// 重置当前的视口	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glBegin(GL_QUADS);//  绘制正方形
	glColor3f(color, color, color);
	for (int i=0;i<4;i++)
	{
		glVertex3f(r[i][0]*xScale, r[i][1]*yScale, 0.0f);	
	}
	glEnd();
}
