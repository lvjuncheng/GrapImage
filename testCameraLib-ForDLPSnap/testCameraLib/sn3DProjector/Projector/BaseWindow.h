// here is the base class for my windows

#ifndef _BASEWINDOW_H_
#define _BASEWINDOW_H_
#include <tchar.h>
#include<windows.h>
#pragma warning(disable:4996)

class CBaseWindow
{
public:
	//contructor 
	CBaseWindow(HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL);

	// many different ways to register
	virtual BOOL RegisterWindow();
	virtual BOOL RegisterWindow(UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
		LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm);
	virtual BOOL RegisterWindow(CONST WNDCLASSEX* wcx);

	// static message handler to put in WNDCLASSEX structure
	static LRESULT CALLBACK stWinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// just so you can change the window caption...
	void SetWindowTitle(LPCTSTR lpszTitle) 
	{
		ZeroMemory(szWindowTitle, sizeof(TCHAR)*256);
		_tcscpy(szWindowTitle, lpszTitle);
	};

	// 2 ways to create
	virtual BOOL Create();
	virtual BOOL Create(DWORD dwStyles, RECT* rect);

	//void MsgLoop();
	BOOL IsWindowClosed() { return bWindowClosed; };


protected:
	virtual void OnPaint(){}
	virtual void OnDraw(HDC hdc){ OnPaint();};

	virtual int  OnCreate(){ return 0;};
	virtual void OnDestroy(){ bWindowClosed = TRUE;};
	virtual void OnSize(UINT nType, int cx, int cy){};

protected:
	HINSTANCE hInstance;
	HWND m_hwnd;
	BOOL bWindowClosed;
	TCHAR szClassName[256];
	TCHAR szWindowTitle[256];

	// the real message handler
	//virtual LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// returns a pointer the window (stored as the WindowLong)
	inline static CBaseWindow *GetObjectFromWindow(HWND hWnd)
	{
		return (CBaseWindow *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
};

#endif