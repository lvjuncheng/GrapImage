#ifndef _DisplayWnd_H
#define _DisplayWnd_H

#define OLDVERSION_FOR_KOREA_USE 0

#if !OLDVERSION_FOR_KOREA_USE
class GLDrawFont
{
public:
    GLDrawFont()
    {
        bBuilded=false;
    }
    ~GLDrawFont()
    {
        KillFont();
    }
    // ����λͼ����
    void BuildFont(HDC hDC);						
    // ɾ����ʾ�б�
    void KillFont(void);							
    // �Զ���GL������庯��
    void GLPrint(const char *str, float x, float y);	

    bool IsBuild()
    {
        return bBuilded;
    }

protected:
    unsigned int m_FontList;
    bool	     bBuilded;
};
#endif

class CDisplayWnd
{
public:
	CDisplayWnd();
	~CDisplayWnd();
	void SetCamID(int id){ m_camID = id; };
	int  GetCamID(){ return m_camID; }
	void SetViewport(HWND view);		// alloc memory
#if !OLDVERSION_FOR_KOREA_USE
    HWND GetViewport(){ return m_viewport; }
#endif

	// set image function
	void SetImageSize(int w, int h);		// resize the image data
	void SetImageData(BYTE* image);	
	void GetImageSize(int *w, int *h){ *w=m_imageWidth; *h=m_imageHeight; }
#if !OLDVERSION_FOR_KOREA_USE
    BYTE* GetImagePointer() { m_bDrawCircle = FALSE; return m_image; };
#endif
	
	// set ref point
    void SetRefPoints(int count,double *x, double *y, int* idList);
#if !OLDVERSION_FOR_KOREA_USE
    // ������ʾ����
    void SetCirclePoints(int count,double *x, double *y, int* ids);
#endif
	
	// draw
	void GLDraw();
	void Invalidate();
#if !OLDVERSION_FOR_KOREA_USE
    // ����
    void GLDrawCircle();
    void Resize();
#endif

	// set attribute
	BOOL SetRed(BOOL bRed)		{ return m_bRed=bRed; }
	BOOL SetCross(BOOL bCross)	{ return m_bCross=bCross; }
	void SetDrawRefPoints(BOOL bDraw){ m_bDrawCircle=bDraw; };
#if !OLDVERSION_FOR_KOREA_USE
    // ������������
    BOOL SetZoom();

    // ������
    void MouseMove( int x, int y);
    void LeftClick( int x, int y);
    void LeftUp();

    //opengl������ʼ��
    BOOL SetWindowPixelFormat(HDC hDC);
    BOOL CreateViewGLContext(HDC hDC);
#endif
	void GetWindowSize();
protected:

#if OLDVERSION_FOR_KOREA_USE
	void Resize();
#endif
    void GLDrawRefPoints();

	void GLDrawRedCross();
	void ImageWnd();	// zoom the image
	
#if !OLDVERSION_FOR_KOREA_USE
    void GLDrawWndPos();
#endif

#if OLDVERSION_FOR_KOREA_USE
	//opengl dc
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
#endif
	
protected:
#if !OLDVERSION_FOR_KOREA_USE
    BOOL m_bMousePress;
    int m_mouseX;
    int m_mouseY;

    BOOL m_bZoom;
#endif
	BOOL m_bRed;
	BOOL m_bCross;
	BOOL m_bDrawCircle;
protected:
	HWND m_viewport;

	BYTE* m_image;		// original image data
	int m_imageWidth;
	int m_imageHeight;

	BYTE* m_wndImage;	// display image data
	int m_wndWidth;
	int m_wndHeight;

#if !OLDVERSION_FOR_KOREA_USE
    int m_imgPosx;		// ������ԭʼͼƬ�ϵ�λ��
    int m_imgPosy;
#endif

	float m_wndPosx;	// original position [-1.0  1.0]
	float m_wndPosy;
    
	double *m_refPointx;
	double *m_refPointy;
	int m_refCount;
    int *m_idList;
#if OLDVERSION_FOR_KOREA_USE
    int m_refCount3D;
#else
    double *m_circlePointx;
    double *m_circlePointy;
    int *m_circleIds;
    int m_cirlceCount;
    GLDrawFont m_font;
#endif

private:
	HGLRC	m_hGLContext;
	HDC		m_hDC;
	int		m_GLPixelIndex;
	int     m_camID;//���id
};


#endif
