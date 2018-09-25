#ifndef _ProjectWnd_H
#define _ProjectWnd_H

// ProjectWnd.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CProjectWnd window
//#include "stdafx.h"
#include "BaseWindow.h"
#include "RasterImages.h"

class CProjectWnd : public CBaseWindow
{
// Construction
public:
	CProjectWnd(HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL);
	virtual ~CProjectWnd();

	BOOL RegisterWindow();
	BOOL Create();
	void Destroy();

	void SetRasterImages(CRasterImages *pImages) {m_imges = pImages;}
	void RenderImages(int i, bool bPic = false);

	//void BuildTest(sn3DProjector::ProjectType type, int lighthigh, int lightlow, int stripWith=-1);
	void RenderTest();

// Attributes
public:
    // screen stuff
    // ...and our very own cursor even!
    HCURSOR m_hCursor; 
    // hows about a nice background brush color...
    COLORREF    m_backColor;

	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	
	HGLRC	m_hGLContext;
	HDC		m_hDC;
	int		m_GLPixelIndex;
	int		m_idProject;
	
	CRasterImages *m_imges;
	
	int m_width;
	int m_height;
	// Generated message map functions
protected:
	void DrawRect(float color, float xScale, float yScale);

	void OnDestroy();
	int  OnCreate();
	void OnSize(UINT nType, int cx, int cy);
	void OnPaint();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ProjectWnd_H__6E1D4ED1_D9A6_11D4_9B9F_525400DAE6A0__INCLUDED_)
