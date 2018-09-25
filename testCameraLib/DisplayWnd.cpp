#include "stdafx.h"

#include "ImageConvert.h"
#include "DisplayWnd.h"

#if OLDVERSION_FOR_KOREA_USE
#include <glew.h>
#include <glut.h>
#else
#include "glew.h"
#include "glut.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if !OLDVERSION_FOR_KOREA_USE
//////////////////////////////////////////////////////////////////////////////
// 
// class GLDrawFont
//----------------------------------------------------------------------------
// class GLDrawFont
void GLDrawFont::BuildFont(HDC hDC)						// 创建位图字体
{
    if( bBuilded )
        return;
    HFONT hFont = CreateFont( 14,0,0,0,FW_THIN/*FW_BOLD*/,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,_T("Arial")); //吴军

    // Now that we have created a new font, we need to select that font into our global HDC.
    HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);

    m_FontList = glGenLists(256);
    BOOL ret = wglUseFontBitmaps(hDC, 0, 255, m_FontList); // 初始化字体
    SelectObject(hDC, hFontOld );					// 选择原来的字体
    DeleteObject(hFont);		
    bBuilded = true;

}
void GLDrawFont::KillFont(GLvoid)							// 删除显示列表
{
    if( bBuilded )
        glDeleteLists(m_FontList, 1);					//删除96个显示列表
    bBuilded = false;
}
void GLDrawFont::GLPrint(const char *str, float x, float y)			// 自定义GL输出字体函数
{
    if( !bBuilded )
        return ;
    glRasterPos2f( x, y);
    //	glPushAttrib(GL_LIST_BIT);
    glListBase(m_FontList);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
    //	glPopAttrib();
}

//GLDrawFont g_font;
//****************************************************************************
#endif

// class CDisplayWnd

CDisplayWnd::CDisplayWnd()
{
	m_camID = 0;
	m_viewport  = NULL;
	m_image		= NULL;	//原始图片数据
	m_wndImage	= NULL;	// 显示图片数据
	
#if OLDVERSION_FOR_KOREA_USE
	m_imageWidth	= 0;
	m_imageHeight	= 0;
#else
    m_imageWidth	= 0;
    m_imageHeight	= 0;

    m_imgPosx = 0;
    m_imgPosy = 0;
#endif

	m_wndPosx = -1.0f;
	m_wndPosy = -1.0f;

	m_wndWidth = 0;
	m_wndHeight= 0;
#if !OLDVERSION_FOR_KOREA_USE
    m_bZoom = TRUE;
#endif
	m_bRed = TRUE;
	m_bCross = TRUE;

	m_bDrawCircle = FALSE;
#if OLDVERSION_FOR_KOREA_USE
    m_refPointx = NULL;
	m_refPointy = NULL;
	m_refCount = 0;
	m_idList = NULL;
	m_refCount3D = 0;
#else
    m_bMousePress = FALSE;
    m_circlePointx = NULL;
    m_circlePointy = NULL;
    m_cirlceCount = 0;
#endif

	m_hDC = NULL;
	m_hGLContext = NULL;
}

CDisplayWnd::~CDisplayWnd()
{
	if( m_image )
		delete[] m_image;
	if( m_wndImage)
		delete[] m_wndImage;
#if OLDVERSION_FOR_KOREA_USE
	if(m_idList )
		delete []m_idList;

	if(m_refPointx )
	{
		delete[] m_refPointx;
		delete[] m_refPointy;
	}
#else
    if(m_circlePointx )
    {
        delete[] m_circlePointx;
        delete[] m_circlePointy;
        delete[] m_circleIds;
    }
#endif
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);

	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}
void CDisplayWnd::SetImageSize(int w, int h)
{
	m_imageWidth = w;
	m_imageHeight = h;
	if( m_image )
		delete[] m_image;
	m_image = new BYTE[3*w*h];
}
void CDisplayWnd::SetImageData(BYTE* image)
{
	if(m_image)
	{
#if !OLDVERSION_FOR_KOREA_USE
        m_bDrawCircle = FALSE;
#endif
		memcpy(m_image,image,3*m_imageWidth*m_imageHeight);
	}
}

void CDisplayWnd::SetRefPoints(int count,double *x, double *y, int* idList)
{
	if( m_refCount != 0 )
	{
		delete[] m_refPointx;
		delete[] m_refPointy;
	}
	m_refCount = count;
	m_refPointx = new double[m_refCount];
	m_refPointy = new double[m_refCount];
	m_idList = new int[m_refCount];

	for (int i=0; i<m_refCount; i++)
	{
		m_refPointx[i] = x[i];
		m_refPointy[i] = y[i];
		m_idList[i] = idList[i];
	}

}
#if !OLDVERSION_FOR_KOREA_USE
void CDisplayWnd::SetCirclePoints(int count,double *x, double *y, int* ids)
{
    if( m_cirlceCount != 0 )
    {
        delete[] m_circlePointx;
        delete[] m_circlePointy;
        delete[] m_circleIds;
    }
    m_cirlceCount = count;
    m_circlePointx = new double[m_cirlceCount];
    m_circlePointy = new double[m_cirlceCount];
    m_circleIds    = new int[m_cirlceCount];

    printf("++++++++++++++++++++++++++++++++\n");
    for (int i=0; i<m_cirlceCount; i++)
    {
        m_circlePointx[i] = x[i];
        m_circlePointy[i] = y[i];
        if( ids )
            m_circleIds[i] = ids[i];
        else
            m_circleIds[i] = -1;
        printf("%i:[%d][%lf, %lf]\n",i, m_circleIds[i], m_circlePointx[i],m_circlePointy[i]);
    }
    printf("+++++++++++++++++++++++++++++++++\n");
    m_bDrawCircle = TRUE;

}
#endif

void CDisplayWnd::SetViewport(HWND view)
{
	m_viewport = view;
	m_hDC = ::GetDC(m_viewport);
	SetWindowPixelFormat(m_hDC);
	CreateViewGLContext(m_hDC);

#if !OLDVERSION_FOR_KOREA_USE
    m_font.BuildFont(m_hDC);
#endif
	GetWindowSize();

	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glShadeModel(GL_SMOOTH);

	Resize();
}
void CDisplayWnd::GetWindowSize()
{
	RECT r;

	if( !m_viewport)
		return;

	GetClientRect(m_viewport, &r);
	int w = r.right-r.left;
	w=(w+3)>>2<<2;
	int h = r.bottom-r.top;
	if( !m_wndImage || m_wndWidth!=w || m_wndHeight!= h )
	{
		if( m_wndImage )
			delete[] m_wndImage;
		m_wndWidth = w;
		m_wndHeight = h;
		m_wndImage = new BYTE[3*m_wndWidth*m_wndHeight];
	}
}

#if !OLDVERSION_FOR_KOREA_USE
BOOL CDisplayWnd::SetZoom( )
{ 
    m_bZoom = !m_bZoom;
    if( !m_bZoom )
    {
        m_imgPosx=(m_imageWidth-m_wndWidth)/2;
        m_imgPosy=(m_imageHeight-m_wndHeight)/2;
    }
    return m_bZoom;
}
#endif

void CDisplayWnd::Invalidate()
{
	if( !m_viewport)
		return;
	if (wglMakeCurrent(m_hDC, m_hGLContext) == FALSE)
	{
		m_hGLContext = wglCreateContext(m_hDC);
		wglMakeCurrent(m_hDC, m_hGLContext);
#if !OLDVERSION_FOR_KOREA_USE
        m_font.KillFont();
        m_font.BuildFont(m_hDC);
#endif
	}
	GLDraw();
}

#if !OLDVERSION_FOR_KOREA_USE
void CDisplayWnd::MouseMove( int x, int y)
{
    if( m_bMousePress && !m_bZoom )
    {
        int tx = m_imgPosx-(x-m_mouseX);
        int ty = m_imgPosy-(y-m_mouseY);
        if( tx>=0 && tx< m_imageWidth-m_wndWidth && 
            ty>=0 && ty<m_imageHeight-m_wndHeight)
        {
            m_imgPosx = tx;
            m_imgPosy = ty;
            m_mouseX = x;
            m_mouseY = y;
            GLDraw();
        }
    }
}
void CDisplayWnd::LeftClick( int x, int y)
{ 
    m_bMousePress = TRUE; 
    m_mouseX = x; 
    m_mouseY=y;
};
void CDisplayWnd::LeftUp()
{ 
    m_bMousePress = FALSE;
    m_mouseX = 0;
    m_mouseY = 0;
};
#endif

void CDisplayWnd::GLDrawRefPoints()
{
	if( m_refCount<=0 )
		return;
	float x=0;
	float y=0;
	float lenx=0.015f;
	float leny=0.02f;
	float fontx=0.025f;
	float fonty=0.025f;
	for ( int i=0; i<m_refCount; i++)
	{
		if( m_refPointx[i]<0 || m_refPointy[i]<0 )
			continue;

		x = (float)( (m_refPointx[i]+1.0)*2.0/(m_imageWidth)) - 1.0f;
		y = (float)( (m_refPointy[i]+1.0)*2.0/(m_imageHeight))- 1.0f;
		y = -1.0f * y;

		if(m_idList[i] == -1)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(0.0f,1.0f,0.0f);
		glBegin(GL_LINES);
		glVertex2f( x-lenx, y);
		glVertex2f( x+lenx, y);
		glVertex2f( x, y-leny);
		glVertex2f( x, y+leny);
		glEnd();
	}
}

void CDisplayWnd::GLDrawCircle()
{
    if( m_cirlceCount<=0 )
        return;
    float x=0;
    float y=0;
    float lenx=0.015f;
    float leny=0.02f;
    float fontx=0.025f;
    float fonty=0.025f;
    char str[128];
    for ( int i=0; i<m_cirlceCount; i++)
    {
        if( m_circlePointx[i]<0 || m_circlePointy[i]<0 )
            continue;

        x = (float)( (m_circlePointx[i]+1.0)*2.0/(m_imageWidth)) - 1.0f;
        y = (float)( (m_circlePointy[i]+1.0)*2.0/(m_imageHeight))- 1.0f;
        y = -1.0f * y;

        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        glVertex2f( x-lenx, y);
        glVertex2f( x+lenx, y);
        glVertex2f( x, y-leny);
        glVertex2f( x, y+leny);
        glEnd();

        if( m_circleIds[i] < 0)
            continue;
        glColor3f(0.0f,1.0f,0.0f);
        sprintf(str,"%d",m_circleIds[i]);
        m_font.GLPrint(str,x+fontx, y-fonty);
    }
}

void CDisplayWnd::GLDraw( )
{
	if(!m_viewport || !m_image)
		return;
	ImageWnd();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(m_wndPosx,m_wndPosy);
	glDrawPixels( m_wndWidth, m_wndHeight,GL_BGR, GL_UNSIGNED_BYTE, m_wndImage);

#if !OLDVERSION_FOR_KOREA_USE
    if( !m_bZoom )
        GLDrawWndPos();
#endif

	if( m_bCross )
		GLDrawRedCross();
    
    // 此处为标定和扫描过程中的绘制所共用 存在bug Note by lishuang
    // 应分开处理 标定为DrawCircle 扫描中的识别为DrawRefPoints
	if( m_bDrawCircle ) 
    {
		GLDrawRefPoints();
        GLDrawCircle();
    }
	
	SwapBuffers(m_hDC);
}

#if !OLDVERSION_FOR_KOREA_USE
void CDisplayWnd::GLDrawWndPos()
{
    float ratioZoom = 1.0;
    float ratioWndXY = m_imageWidth*1.0f/m_imageHeight;
    if( ratioWndXY >1 )
        ratioZoom = 0.4f/m_imageWidth;
    else
        ratioZoom = 0.4f/m_imageHeight;

    float bigRectStartX= 1.0f - m_imageWidth * ratioZoom; //窗口在opengl中宽度为2.0f，从右上角开始显示
    float bigRectStartY= 1.0f - m_imageHeight * ratioZoom;

    float smallWidth = m_wndWidth * ratioZoom;  // 小窗口宽度
    float smallHeight= m_wndHeight* ratioZoom;

    float smallRectStartX = bigRectStartX + m_imgPosx * ratioZoom;  
    float smallRectStartY = bigRectStartY-smallHeight + (m_imageHeight-m_imgPosy) * ratioZoom;

    // 画大矩形
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.6f);    
    glVertex3f( bigRectStartX,	bigRectStartY,	0.0f); // 左下	
    glVertex3f( bigRectStartX,	1.0f,			0.0f);	
    glVertex3f( 1.0f,			1.0f,			0.0f); //右上	
    glVertex3f( 1.0f,			bigRectStartY,	0.0f);	
    glEnd();

    // 画小矩形
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);  
    glVertex3f( smallRectStartX,			smallRectStartY,				0.0f);
    glVertex3f( smallRectStartX,			smallRectStartY+smallHeight,	0.0f);
    glVertex3f( smallRectStartX+smallWidth,	smallRectStartY+smallHeight,	0.0f);
    glVertex3f( smallRectStartX+smallWidth,	smallRectStartY,				0.0f);
    glEnd();
}
#endif

void CDisplayWnd::GLDrawRedCross()
{
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex2f(0.0f,1.0f);
	glVertex2f(0.0f,-1.0f);
	glVertex2f(1.0f,0.0f);
	glVertex2f(-1.0f,0.0f);
	glEnd();


	float box_width = 0.25;

	glBegin(GL_LINE_LOOP);
	glVertex2f(-box_width,   box_width);
	glVertex2f(box_width, box_width);
	glVertex2f(box_width, -box_width);
	glVertex2f(-box_width, -box_width);
	/*glVertex2f(1.0f, 0.0f);
	glVertex2f(-1.0f, 0.0f);*/
	glEnd();

}
void CDisplayWnd::ImageWnd()
{
	GetWindowSize();
	// 抽样拷贝，或者部分拷贝，并且过亮地方设置红色
	//CopyToWndImage();

#if !OLDVERSION_FOR_KOREA_USE
    if( m_bZoom )
    {
#endif

	CImageConvert::ImageNearestScale( m_wndImage, m_wndWidth, m_wndHeight, 
			m_image, m_imageWidth, m_imageHeight);
    
#if !OLDVERSION_FOR_KOREA_USE
    }
    else
    {
		m_wndPosx =  -1.0f;
		m_wndPosy =  -1.0f;

        CImageConvert::ImageCopyClip(  m_wndImage, m_wndWidth, m_wndHeight, 
            m_image, m_imageWidth, m_imageHeight,
            m_imgPosx,m_imgPosy);

    }
#endif

	if( m_bRed )
	{
		CImageConvert::RedFullImage(m_wndImage, m_wndWidth, m_wndHeight);
	}
	CImageConvert::ImageFlip(m_wndImage, m_wndWidth, m_wndHeight);
}

//********************************************
// SetWindowPixelFormat
//********************************************
BOOL CDisplayWnd::SetWindowPixelFormat(HDC hDC)
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
	if(m_GLPixelIndex == 0) 
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
BOOL CDisplayWnd::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	
	if(m_hGLContext==NULL)
		return FALSE;
	
	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
	{
		int b = GetLastError();
		return FALSE;
	}
	return TRUE;
}

void CDisplayWnd::Resize()
{
	glViewport(0, 0, m_wndWidth, m_wndHeight);
}
