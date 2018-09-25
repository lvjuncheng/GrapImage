
// testCameraLibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testCameraLib.h"
#include "testCameraLibDlg.h"
#include "afxdialogex.h"
#include "sn3DImageLoad.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <common/baseType/DataArchiveConfig.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Sn3DAlgorithm;
using namespace LY3DStereoBuild;
using namespace LY3DAlgorithm;
using namespace Eigen;
using namespace std;

char g_folderPath[MAX_PATH] = { 0 };
// CAboutDlg dialog used for App About

#define ISCHAR(a) (((a)>='a' && (a)<='z')||((a)>='A'&&(a)<='Z')?true:false)

std::string MultiStringFromUnicodeString(CString& tString)
{
	char tmpBuf[2048] = { 0 };
	std::string tmpTarget = "";
#ifdef _UNICODE
	size_t tmpConvertedSize = 0;
	wcstombs_s(&tmpConvertedSize, tmpBuf, tString.GetBuffer(), 2000);
	tmpTarget = tmpBuf;
#else
	tmpTarget = tString;
#endif
	return tmpTarget;
}
CString UnicodeStringFromMultiString(std::string& str)
{
	wchar_t tmpBuf[2048] = { 0 };
	CString tmpTarget = _T("");
#ifdef _UNICODE
	size_t tmpConvertedSize = 0;
	mbstowcs_s(&tmpConvertedSize, tmpBuf, str.c_str(), 2000);
	tmpTarget = tmpBuf;
#else
	tmpTarget = tString;
#endif
	return tmpTarget;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestCameraLibDlg dialog


HANDLE g_hBoard = NULL;

CtestCameraLibDlg::CtestCameraLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestCameraLibDlg::IDD, pParent)
	, m_cameraType(-1)
	, m_childDlg(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_displayWnds[0] = NULL;
	m_displayWnds[1] = NULL;
	m_grabCount = 0;
	m_grabbing = false;
	for (int i = 0; i < DEFAULT_CAMERACOUNT; ++i)
	{
		m_hasGrabbed[i] = false;
		m_hasGrabEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	m_displayImgBufs[0] = NULL;
	m_displayImgBufs[1] = NULL;
	m_hSnapThread = NULL;;
	m_bKeepLoop = false;;
	m_bHold = false;
}

void CtestCameraLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestCameraLibDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestCameraLibDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CtestCameraLibDlg::OnBnClickedButtonDisconnect)
	ON_STN_DBLCLK(IDC_STATIC_SHOW0, &CtestCameraLibDlg::OnStnDblclickStaticShow0)
	ON_STN_DBLCLK(IDC_STATIC_SHOW1, &CtestCameraLibDlg::OnStnClickedStaticShow1)
	ON_BN_CLICKED(IDC_RADIO_WHITE, &CtestCameraLibDlg::OnBnClickedRadioWhite)
	ON_BN_CLICKED(IDC_RADIO_TESTSTRIPE, &CtestCameraLibDlg::OnBnClickedRadioTeststripe)
	ON_BN_CLICKED(IDC_RADIO_CROSS, &CtestCameraLibDlg::OnBnClickedRadioCross)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CtestCameraLibDlg::OnBnClickedButtonScan)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJTYPE, &CtestCameraLibDlg::OnCbnSelchangeComboProjtype)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMTYPE, &CtestCameraLibDlg::OnCbnSelchangeComboCamtype)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &CtestCameraLibDlg::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_BUTTON5, &CtestCameraLibDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK_LOOPSCAN, &CtestCameraLibDlg::OnBnClickedCheckLoopscan)
	ON_CBN_SELCHANGE(IDC_COMBO_IMG_GROUP, &CtestCameraLibDlg::OnCbnSelchangeComboImgGroup)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, &CtestCameraLibDlg::OnBnClickedCheckEnable)
	ON_BN_CLICKED(IDC_RADIO_BIG, &CtestCameraLibDlg::OnBnClickedRadioBig)
	ON_BN_CLICKED(IDC_RADIO_SMALL, &CtestCameraLibDlg::OnBnClickedRadioSmall)
END_MESSAGE_MAP()


// CtestCameraLibDlg message handlers

bool CtestCameraLibDlg::ReadParam()
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "./parameter.bin","rb");
	if (NULL == pFile){
		printf("Open File Failed\n");
		return false;
	}
	int tmpBufSize = sizeof(LOGBUF);
	LOGBUF* buf = new LOGBUF();
	int tmpSize = fread_s(buf, tmpBufSize, 1, tmpBufSize, pFile);
	if (tmpSize != tmpBufSize){
		fclose(pFile);
		delete[]buf;
		printf("Read File Failed\n");
		return false;
	}
	m_cameraType = buf->camType;
	m_defaultExposure = buf->exposure;
	m_bEnableLoopScan = buf->bEnableLoopScan;
	m_defaultGain = buf->gain;
	m_startGain = buf->gainStart;
	m_imgGroupIndex = buf->imgGroupIndex;
	m_waitTime = buf->waitTime;
	m_bEnableCheckAfterGrab = buf->bEnableCheckAfterGrab;
	m_bUsedInBigPlane = buf->bUseBigPlane;
	m_smoothRatio = buf->smoothRatio;
	fclose(pFile);
	pFile = NULL;
	delete[]buf;
	return true;
}

bool CtestCameraLibDlg::WriteParam()
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "./parameter.bin", "wb");
	if (NULL == pFile){
		printf("Open File Failed\n");
		return false;
	}
	int tmpBufSize = sizeof(LOGBUF);
	LOGBUF* buf = new LOGBUF;
	buf->camType = m_cameraType;
	buf->exposure = m_defaultExposure;
	buf->bEnableLoopScan = m_bEnableLoopScan;
	buf->gain = m_defaultGain;
	buf->gainStart = m_startGain;
	buf->imgGroupIndex = m_imgGroupIndex;
	buf->waitTime = m_waitTime;
	buf->bEnableCheckAfterGrab = m_bEnableCheckAfterGrab;
	buf->bUseBigPlane = m_bUsedInBigPlane;
	buf->smoothRatio = m_smoothRatio;
	size_t tmpSize = fwrite(buf, 1, tmpBufSize, pFile);
	fclose(pFile);
	pFile = NULL;
	delete buf;
	return tmpSize == tmpBufSize;
}

BOOL CtestCameraLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_targetSaveFolder = _T("");
	// TODO: Add extra initialization here
	InitializeCriticalSection(&m_snapLock);
	// save path
	CString tmpFolderPath;
	::GetModuleFileName(::GetModuleHandle(NULL), tmpFolderPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	int tmpIndex = tmpFolderPath.ReverseFind(_T('\\'));
	tmpFolderPath = tmpFolderPath.Left(tmpIndex + 1);
	m_programPath = tmpFolderPath;
	tmpFolderPath += _T("saveImages\\");
	m_savePath = tmpFolderPath;
	tmpFolderPath += _T("ScanImages");
	if (FALSE == PathFileExists(tmpFolderPath))
	{
		CreateDirectory(tmpFolderPath, NULL);
	}
	// display window
	if (NULL == m_displayWnds[0])
	{
		m_displayWnds[0] = new CDisplayWnd();
		m_displayWnds[0]->SetCamID(0);
		m_displayWnds[0]->SetViewport(GetDlgItem(IDC_STATIC_SHOW0)->GetSafeHwnd());
		m_displayWnds[0]->SetCross(TRUE);
	}
	if (NULL == m_displayWnds[1])
	{
		m_displayWnds[1] = new CDisplayWnd();
		m_displayWnds[1]->SetCamID(1);
		m_displayWnds[1]->SetViewport(GetDlgItem(IDC_STATIC_SHOW1)->GetSafeHwnd());
		m_displayWnds[0]->SetCross(TRUE);
	}
	
	m_hFinishWrite = CreateEvent(NULL, TRUE, FALSE, NULL);

	// camera param
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	// 添加投影类型和相机类型
//	((CComboBox*)GetDlgItem(IDC_COMBO_PROJTYPE))->InsertString(0,_T("SOCK_PROJ"));
//	((CComboBox*)GetDlgItem(IDC_COMBO_PROJTYPE))->InsertString(1,_T("HID_PROJ"));

	//((CComboBox*)GetDlgItem(IDC_COMBO_CAMTYPE))->AddString(_T("PGR_500M_GIGE"));
	((CComboBox*)GetDlgItem(IDC_COMBO_CAMTYPE))->AddString(_T("PGR_120M_U2"));

	if (false == ReadParam()){
		m_defaultExposure = 10000;
		m_defaultGain = 10;
		m_startGain = 0;
		m_cameraType = -1;
		m_waitTime = 100;
		m_bEnableCheckAfterGrab = true;
		m_bUsedInBigPlane = true;
		m_smoothRatio = 0.3;
		m_imgGroupIndex = 0;
		m_bEnableLoopScan = false;
		WriteParam();
	}
//	((CComboBox*)GetDlgItem(IDC_COMBO_PROJTYPE))->SetCurSel(-1);
	((CComboBox*)GetDlgItem(IDC_COMBO_CAMTYPE))->SetCurSel(m_cameraType);
	if (m_cameraType >= 0)
		OnCbnSelchangeComboCamtype();
	((CComboBox*)GetDlgItem(IDC_COMBO_IMG_GROUP))->SetCurSel(m_imgGroupIndex);
	CString tmpCtrlText;
	tmpCtrlText.Format(_T("%lf"), m_defaultExposure/1000.0);
	SetDlgItemText(IDC_EDIT_EXP, tmpCtrlText);
	tmpCtrlText.Format(_T("%d"), m_defaultGain);
	SetDlgItemText(IDC_EDIT_GAIN, tmpCtrlText);
	tmpCtrlText.Format(_T("%d"), m_startGain);
	SetDlgItemText(IDC_EDIT_GAIN_START, tmpCtrlText);
	tmpCtrlText.Format(_T("%d"), m_waitTime);
	SetDlgItemText(IDC_EDIT_SCANTRIG_PERIOD, tmpCtrlText);
	((CButton*)GetDlgItem(IDC_CHECK_LOOPSCAN))->SetCheck(m_bEnableLoopScan ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_ENABLE))->SetCheck(m_bEnableCheckAfterGrab ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_BIG))->SetCheck(m_bUsedInBigPlane ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SMALL))->SetCheck(m_bUsedInBigPlane ? FALSE : TRUE);
	tmpCtrlText.Format(_T("%lf"), m_smoothRatio);
	SetDlgItemText(IDC_EDIT_SMOOTH, tmpCtrlText);

	m_hProjector = Projector::GetInstance();
	m_hProjector->CreateProjectorWindow(::GetModuleHandle(NULL));
	OnBnClickedButtonScan();
	m_bKeepLoop = true;
	m_hSnapThread = (HANDLE)_beginthreadex(NULL, 0, SnapThread, this, 0, NULL);

	SYSTEMTIME tmpTime;
	GetLocalTime(&tmpTime);
	m_targetSaveFolder.Format(_T("%d-%d-%d_%d-%d-%d"), tmpTime.wYear, tmpTime.wMonth, tmpTime.wDay, tmpTime.wHour, tmpTime.wMinute, tmpTime.wSecond);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CtestCameraLibDlg::OnClose()
{
	if (m_hSnapThread)
	{
		m_bHold = false;
		m_bKeepLoop = false;
		WaitForSingleObject(m_hSnapThread, INFINITE);
		CloseHandle(m_hSnapThread);
		m_hSnapThread = NULL;
	}

	for (int i = 0; i < DEFAULT_CAMERACOUNT; ++i)
	{
		if (m_hasGrabEvent[i])
		{
			CloseHandle(m_hasGrabEvent[i]);
			m_hasGrabEvent[i] = NULL;
		}
	}

	int tmpCamCount = m_cameraList.size();
	for (int i = 0; i < tmpCamCount; ++i)
	{
		if (m_cameraList[i])
		{
			sn3DCamera::ReleaseCamera(m_cameraList[i]);
			m_cameraList[i] = NULL;
		}
	}

	int tmpParamCount = m_cameraParams.size();
	for (int i = 0; i < tmpParamCount; ++i)
	{
		if (m_cameraParams[i])
		{
			delete(m_cameraParams[i]);
			m_cameraParams[i] = NULL;
		}
	}
	m_cameraParams.erase(m_cameraParams.begin(), m_cameraParams.end());
	if (m_hFinishWrite)
	{
		CloseHandle(m_hFinishWrite);
		m_hFinishWrite = NULL;
	}
	if (m_hProjector)
	{
		m_hProjector->DestroyProjectorWindow();
		m_hProjector = NULL;
	}

	DeleteCriticalSection(&m_snapLock);
	CDialog::OnClose();
}

void CtestCameraLibDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent)
	{
		if (m_bEnableLoopScan)
		{
			OnBnClickedButtonScan();
		}
		else
		{
			GetDlgItem(IDC_CHECK_LOOPSCAN)->EnableWindow(TRUE);
		}
	}
}

void CtestCameraLibDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


BOOL CtestCameraLibDlg::PreTranslateMessage(MSG *pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CString tmpCtrlStr;
			TCHAR *tmpPtr = NULL;
			if (GetDlgItem(IDC_EDIT_EXP)->GetSafeHwnd() == pMsg->hwnd)
			{
				GetDlgItemText(IDC_EDIT_EXP, tmpCtrlStr);
				double tmpExposure = _tcstod(tmpCtrlStr.GetBuffer(), &tmpPtr);
				if (NULL != tmpPtr && tmpExposure >= 0)
				{
					if (m_defaultExposure == (long)(tmpExposure*1000.0))
					{
						return TRUE;
					}
					for (int i = 0; i < m_cameraList.size(); ++i)
					{
						sn3DCamera::SetExposureTime(m_cameraList[i], tmpExposure*1000);
					}
					m_defaultExposure = tmpExposure*1000;
					SetDlgItemText(IDC_EDIT_EXP, tmpCtrlStr);
					WriteParam();
					MessageBox(_T("设置曝光时间成功"));
					return TRUE;
				}
				else
				{
					tmpCtrlStr.Format(_T("%lf"), m_defaultExposure);
					SetDlgItemText(IDC_EDIT_EXP, tmpCtrlStr);
					MessageBox(_T("曝光时间需为非负浮点数"));
					return TRUE;
				}
			}
			else if (GetDlgItem(IDC_EDIT_GAIN)->GetSafeHwnd() == pMsg->hwnd)
			{
				GetDlgItemText(IDC_EDIT_GAIN, tmpCtrlStr);
				long tmpGain = _tcstol(tmpCtrlStr.GetBuffer(), &tmpPtr, 10);
				if (NULL != tmpPtr && tmpGain >= 0)
				{
					if (m_defaultGain == tmpGain)
					{
						return TRUE;
					}
					for (int i = 0; i < m_cameraList.size(); ++i)
					{
						sn3DCamera::SetGain(m_cameraList[i], tmpGain+m_startGain);
					}
					m_defaultGain = (int)tmpGain;
					WriteParam();
					MessageBox(_T("设置增益成功"));
					return TRUE;
				}
				else
				{
					tmpCtrlStr.Format(_T("%d"), m_defaultGain);
					SetDlgItemText(IDC_EDIT_GAIN, tmpCtrlStr);
					MessageBox(_T("增益需为非负整数"));
					return TRUE;
				}
			}
			else if (GetDlgItem(IDC_EDIT_GAIN_START)->GetSafeHwnd() == pMsg->hwnd)
			{
				GetDlgItemText(IDC_EDIT_GAIN_START, tmpCtrlStr);
				long tmpGain = _tcstol(tmpCtrlStr.GetBuffer(), &tmpPtr, 10);
				if (NULL != tmpPtr)
				{
					if (m_startGain == tmpGain)
					{
						return TRUE;
					}
					m_startGain = tmpGain;
					WriteParam();
					MessageBox(_T("设置增益起点成功"));
					return TRUE;
				}
				else
				{
					tmpCtrlStr.Format(_T("%d"), m_startGain);
					SetDlgItemText(IDC_EDIT_GAIN_START, tmpCtrlStr);
					return TRUE;
				}
			}
			else if (GetDlgItem(IDC_EDIT_SCANTRIG_PERIOD)->GetSafeHwnd() == pMsg->hwnd)
			{
				GetDlgItemText(IDC_EDIT_SCANTRIG_PERIOD, tmpCtrlStr);
				long tmpPeriod = _tcstol(tmpCtrlStr.GetBuffer(), &tmpPtr, 10);
				if (NULL != tmpPtr && tmpPeriod >= 0)
				{
					if (m_waitTime == tmpPeriod)
					{
						return TRUE;
					}
					m_waitTime = tmpPeriod;
					WriteParam();
					MessageBox(_T("设置间隔时间成功"));
					return TRUE;
				}
				else
				{
					tmpCtrlStr.Format(_T("%d"), m_waitTime);
					SetDlgItemText(IDC_EDIT_SCANTRIG_PERIOD, tmpCtrlStr);
					MessageBox(_T("间隔时间需为非负整数"));
					return TRUE;
				}
			}
			else if (GetDlgItem(IDC_EDIT_SMOOTH)->GetSafeHwnd() == pMsg->hwnd)
			{
				GetDlgItemText(IDC_EDIT_SMOOTH, tmpCtrlStr);
				double tmpSmooth = _tcstod(tmpCtrlStr.GetBuffer(), &tmpPtr);
				if (NULL != tmpPtr && tmpSmooth >= 0)
				{
					if (m_smoothRatio == tmpSmooth)
					{
						return TRUE;
					}
					m_smoothRatio = tmpSmooth;
					WriteParam();
					MessageBox(_T("设置平滑系数成功"));
					return TRUE;
				}
				else
				{
					tmpCtrlStr.Format(_T("%lf"), m_smoothRatio);
					SetDlgItemText(IDC_EDIT_SMOOTH, tmpCtrlStr);
					MessageBox(_T("平滑系数为非负整数"));
					return TRUE;
				}
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CtestCameraLibDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CtestCameraLibDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// Connect
void CtestCameraLibDlg::OnBnClickedButton1()
{
	int tmpCamCount = m_cameraList.size();
	for (int i = 0; i < tmpCamCount; ++i)
	{
		if (m_cameraList[i])
		{
			sn3DCamera::ReleaseCamera(m_cameraList[i]);
			m_cameraList[i] = NULL;
		}
	}
	m_cameraList.erase(m_cameraList.begin(), m_cameraList.end());
	m_cameraList.resize(0);
	tmpCamCount = m_cameraParams.size();
	printf("Want Open Camera Count: %d\n", tmpCamCount);
	for (int i = 0; i < tmpCamCount; ++i)
	{
		sn3DCamera::HCAMERA tmphCam = NULL;
		printf("Camera %d: %s\n", i, m_cameraParams[i]->serial);
		int tmpRet = sn3DCamera::CreateCamera(tmphCam, m_cameraParams[i]->serial);
		if (0 != tmpRet)
		{
			printf("Create Camera Failed\n");
			continue;
		}
		tmpRet = sn3DCamera::OpenCamera(tmphCam, m_cameraParams[i]->serial, m_cameraParams[i]->width, m_cameraParams[i]->height);
		if (0 != tmpRet)
		{
			sn3DCamera::ReleaseCamera(tmphCam);
			printf("Open Camera Failed\n");
			continue;
		}
		sn3DCamera::SetExposureTime(tmphCam, m_defaultExposure);
		sn3DCamera::SetGain(tmphCam, m_defaultGain);
		sn3DCamera::SetTriggerMode(tmphCam, m_cameraParams[i]->trigMode?SN_TRIG_MODE::HARDWARE_TRIGGER:SN_TRIG_MODE::SOFTWARE_TRIGGER, 0, 0, 0);
		sn3DCamera::StartCamera(tmphCam, 0.0);
		m_cameraList.push_back(tmphCam);
	}
	if (m_cameraList.size() < tmpCamCount)
	{
		for (int i = 0; i < m_cameraList.size(); ++i)
		{
			if (m_cameraList[i])
			{
				sn3DCamera::ReleaseCamera(m_cameraList[i]);
				m_cameraList[i] = NULL;
			}
		}
		m_cameraList.erase(m_cameraList.begin(), m_cameraList.end());
		MessageBox(_T("连接相机失败"));
	}
}

void CtestCameraLibDlg::OnBnClickedButtonDisconnect()
{
	int tmpCamCount = m_cameraList.size();
	for (int i = 0; i < tmpCamCount; ++i)
	{
		if (m_cameraList[i])
		{
			sn3DCamera::ReleaseCamera(m_cameraList[i]);
			m_cameraList[i] = NULL;
		}
	}
	m_cameraList.erase(m_cameraList.begin(), m_cameraList.end());
	m_cameraList.resize(0);
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);

}

void CtestCameraLibDlg::OnStnDblclickStaticShow0()
{
	return;
	char buff[1024] = { 0 };
	// TODO:  在此添加控件通知处理程序代码
	static RECT window_rt;
	if (GetDlgItem(IDC_STATIC_SHOW1)->IsWindowVisible())
	{
			 
		GetDlgItem(IDC_STATIC_SHOW1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_SHOW0)->GetWindowRect(&window_rt);
		this->ScreenToClient(&window_rt);
		 
		int height = window_rt.bottom - window_rt.top;
		int width = height * 4 / 3.0;

		RECT window_rt1;
		this->GetClientRect(&window_rt1);

		RECT rt = window_rt;
		rt.left = (window_rt1.right - window_rt1.left - width) / 2;
		rt.right = rt.left + width;
		GetDlgItem(IDC_STATIC2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_SHOW0)->MoveWindow(rt.left, rt.top, rt.right - rt.left, rt.bottom-rt.top);

		 
	}
	else
	{
		GetDlgItem(IDC_STATIC2)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_SHOW0)->MoveWindow(window_rt.left, window_rt.top, window_rt.right - window_rt.left, window_rt.bottom- window_rt.top );
		GetDlgItem(IDC_STATIC_SHOW1)->ShowWindow(true);
	}
	m_displayWnds[0]->SetViewport(GetDlgItem(IDC_STATIC_SHOW0)->GetSafeHwnd());
	m_displayWnds[0]->SetZoom();
}

void CtestCameraLibDlg::OnStnClickedStaticShow1()
{
	return;
	// TODO:  在此添加控件通知处理程序代码
	static RECT window_rt;
	if (GetDlgItem(IDC_STATIC_SHOW0)->IsWindowVisible())
	{
	 

		GetDlgItem(IDC_STATIC_SHOW0)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_SHOW1)->GetWindowRect(&window_rt);
		this->ScreenToClient(&window_rt);

		int height = window_rt.bottom - window_rt.top;
		int width = height * 4 / 3.0;

		RECT window_rt1;
		this->GetClientRect(&window_rt1);

		RECT rt = window_rt;
		rt.left=(window_rt1.right - window_rt1.left - width) / 2;
		rt.right = rt.left + width;
		 
		GetDlgItem(IDC_STATIC1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_SHOW1)->MoveWindow(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top);
	}
	else
	{
		GetDlgItem(IDC_STATIC1)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_SHOW1)->MoveWindow(window_rt.left, window_rt.top, window_rt.right - window_rt.left, window_rt.bottom - window_rt.top);
		GetDlgItem(IDC_STATIC_SHOW0)->ShowWindow(true);
	}

	m_displayWnds[1]->SetViewport(GetDlgItem(IDC_STATIC_SHOW1)->GetSafeHwnd());
	 
	m_displayWnds[1]->SetZoom();
}

void CtestCameraLibDlg::OnBnClickedRadioWhite()
{
	// TODO:  在此添加控件通知处理程序代码
	
}

void CtestCameraLibDlg::OnBnClickedRadioTeststripe()
{
	// TODO:  在此添加控件通知处理程序代码
	
}

void CtestCameraLibDlg::OnBnClickedRadioCross()
{
	// TODO:  在此添加控件通知处理程序代码

}

void CtestCameraLibDlg::OnBnClickedButtonScan()
{
	CString tmpFilePath = m_programPath + _T("ProjectImages\\imageGroups.txt");
	std::string tmpMFilePath = MultiStringFromUnicodeString(tmpFilePath);
	FILE* pFile = NULL;
	fopen_s(&pFile, tmpMFilePath.c_str(), "r");
	if (NULL == pFile)
		return;

	int tmpCount = m_imgGroups.size();
	for (int i = 0; i < tmpCount; ++i)
	{
		m_imgGroups[i]->imgNames.erase(m_imgGroups[i]->imgNames.begin(), m_imgGroups[i]->imgNames.end());
	}
	m_imgGroups.erase(m_imgGroups.begin(), m_imgGroups.end());
	((CComboBox*)GetDlgItem(IDC_COMBO_IMG_GROUP))->Clear();
	char buffer[1024] = { 0 };
	int tmpIndex = 0;
	int tmpImgTotalCount = 0;
	while (fgets(buffer, 1024, pFile))
	{
		int lineSize = strlen(buffer);
		while (!ISCHAR(buffer[lineSize - 1]) && lineSize > 0){ --lineSize;} // 去掉行尾的无效数据
		if (lineSize > 0)
			buffer[lineSize] = '\0';
		std::string tmpStr = buffer;
		int tmpSearchIndex = tmpStr.find(std::string(".bmp"));
		if (tmpSearchIndex >= 0) // 组内图片名
		{
			m_imgGroups[tmpIndex]->imgNames.push_back(tmpStr);
			++tmpImgTotalCount;
		}
		else // 组名
		{
			PIMGGROUP tmpGroup = new IMGGROUP();
			tmpGroup->groupName = tmpStr;
			tmpGroup->startIndex = tmpImgTotalCount;
			m_imgGroups.push_back(tmpGroup);
			tmpIndex = m_imgGroups.size() - 1;
			((CComboBox*)GetDlgItem(IDC_COMBO_IMG_GROUP))->AddString(UnicodeStringFromMultiString(tmpStr));
			printf("Group %d: %s\n", tmpIndex, tmpStr.c_str());
		}
		memset(buffer, 0, 1024);
	}
	m_hProjector->InitPictureNum(tmpImgTotalCount);
	// 读图 保存
	int tmpGroupCount = m_imgGroups.size();
	std::string tmpFolderPath = MultiStringFromUnicodeString(m_programPath);
	tmpFolderPath += "ProjectImages\\";
	for (int i = 0; i < tmpGroupCount; ++i)
	{
		std::string tmpGroupPath = tmpFolderPath + m_imgGroups[i]->groupName + "\\";
		int tmpImgCount = m_imgGroups[i]->imgNames.size();
		for (int j = 0; j < tmpImgCount; ++j)
		{
			sn3DImageData<unsigned char> tmpData;
			tmpData.m_data = NULL;
			std::string tmpFilePath = tmpGroupPath + m_imgGroups[i]->imgNames[j];
			int tmpRet = sn3DImageLoad::sn3DReadImage(tmpFilePath.c_str(), tmpData);
			if (0 == tmpRet)
			{
				m_hProjector->SetPictureData(tmpData.m_data, tmpData.m_w, tmpData.m_h, j + m_imgGroups[i]->startIndex);
			}
			delete[](tmpData.m_data);
			tmpData.m_data = NULL;
		}
	}
	m_imgGroupIndex = -1;
	((CComboBox*)GetDlgItem(IDC_COMBO_IMG_GROUP))->SetCurSel(m_imgGroupIndex);
	return;
}


void CtestCameraLibDlg::OnCbnSelchangeComboProjtype()
{
	// TODO:  在此添加控件通知处理程序代码

}


void CtestCameraLibDlg::OnCbnSelchangeComboCamtype()
{
	// TODO:  在此添加控件通知处理程序代码
	int tmpCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMTYPE))->GetCurSel();
	m_cameraType = -1;
	std::string tmpCamType = "";
	std::string tmpPixCam = "";
	int    tmpDefaultWidth = 1280;
	int    tmpDefaultHeight = 1024;
	bool   tmpbDefaultTrig = false;
	int tmpParamCount = m_cameraParams.size();
	for (int i = 0; i < tmpParamCount; ++i)
	{
		if (m_cameraParams[i])
		{
			delete(m_cameraParams[i]);
			m_cameraParams[i] = NULL;
		}
	}
	m_cameraParams.erase(m_cameraParams.begin(), m_cameraParams.end());
	switch (tmpCurSel)
	{
	case 0:
	/*	m_cameraType = 0;
		tmpCamType = "PGR00";
		tmpPixCam = "PGR";
		m_defaultExposure = 10000;
		m_defaultGain = 10;
		tmpDefaultWidth = 2448;
		tmpDefaultHeight = 2048;
		tmpbDefaultTrig = true;
		printf("Select 500W Camera\n");
		break;
	case 1:*/
		m_cameraType = 0;
		tmpCamType = "PGR00";
		tmpPixCam = "PGR";
		tmpDefaultWidth = 1280;
		tmpDefaultHeight = 960;
		tmpbDefaultTrig = false;
		printf("Select 130W Camera\n");
		break;
	default:break;
	}
	if (m_cameraType < 0)
		return;


	FILE* pFile = NULL;
	fopen_s(&pFile, "cameraSerial.txt", "r");
	char tmpBuf[1024];
	std::string tmpStr;
	for (int i = 0; i < DEFAULT_CAMERACOUNT; ++i)
	{
		PCAMINFO tmpCamParam = new CAMINFO();
		if (pFile)
		{
			memset(tmpBuf, 0, 1024);
			fgets(tmpBuf, 1024, pFile);
			tmpStr = tmpPixCam;
			tmpStr += tmpBuf;
			strcpy(tmpCamParam->serial, tmpStr.c_str());
			OutputDebugStringA(tmpStr.c_str());
			tmpStr = "";
		}
		else
		{
			strcpy(tmpCamParam->serial, tmpCamType.c_str());
			strcat(tmpCamParam->serial, "00");
		}

		tmpCamParam->width = tmpDefaultWidth;
		tmpCamParam->height = tmpDefaultHeight;
		tmpCamParam->trigMode = tmpbDefaultTrig ? 1 : 0;
		m_cameraParams.push_back(tmpCamParam);
		tmpCamParam = NULL;
	}

	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	WriteParam();
}


bool CtestCameraLibDlg::StereobuildDLP(ScanImages& scanImagesLeft, ScanImages& scanImagesRight, ScanImages& scanImageCross, std::string cameraPath, vector<Vector3>& p3D, vector<Vector2>& p2D)
{
	// 高斯滤波参数
	double kernel[] = { 0.0751136079541115, 0.123841403152974, 0.0751136079541115, 0.123841403152974, 0.204179955571658, 0.123841403152974, 0.0751136079541115, 0.123841403152974, 0.0751136079541115 };

	//投影机分辨率 1920*1080 相机分辨率 1280 960
	int ImageHeigh = 1080;
	int ImageWidth = 1920;

	std::string CalibPathL = cameraPath + "\\LeftCCF.txt";
	std::string CalibPathR = cameraPath + "\\RightCCF.txt";

	CStreroPara StreroPara;
	StreroPara.CalibPathL = CalibPathL;
	StreroPara.LeftSize[0] = scanImagesLeft.images[0].m_w;
	StreroPara.LeftSize[1] = scanImagesLeft.images[0].m_h;

	StreroPara.CalibPathR = CalibPathR;
	StreroPara.RightSize[0] = scanImagesRight.images[0].m_w;
	StreroPara.RightSize[1] = scanImagesRight.images[0].m_h;

	LY3DAlgorithm::sn3DSetCamera(StreroPara);

	std::ifstream ifs(cameraPath + "\\undistortion.table");
	if (!ifs.is_open())
	{
		LY3DAlgorithm::sn3DCreateUndistortTable((cameraPath + "\\undistortion.table").c_str());
	}

	LY3DAlgorithm::sn3DLoadUndistortTable((cameraPath + "\\undistortion.table").c_str());

	LY3DStereoBuild::PhaseParam param;
	param.width = scanImagesLeft.images[0].m_w;
	param.height = scanImagesLeft.images[0].m_h;

	param.stepNum = 5;
	param.rasterType = 1;

	param.pixelWidth[0] = 15;
	param.frangeNum[0] = 127.9995;
	param.moveNum[0] = 8;

	param.pixelWidth[1] = 50.45395;
	param.frangeNum[1] = 38.0545;
	param.moveNum[1] = 5;

	param.pixelWidth[2] = 169.705754;
	param.frangeNum[2] = 11.3137;
	param.moveNum[2] = 5;

	param.pixelWidth[3] = 570.82037;
	param.frangeNum[3] = 3.36358;
	param.moveNum[3] = 5;

	param.pixelWidth[4] = 1920;
	param.frangeNum[4] = 1;
	param.moveNum[4] = 5;

	double phaseTh = 1.57; double grayTh = 5;

	LY3DStereoBuild::DoubleImageParam m_phaseLeft;
	LY3DStereoBuild::DoubleImageParam m_phaseRight;
	LY3DStereoBuild::DoubleImageParam m_phaseCross;

	LY3DStereoBuild::GrayImageParam m_maskLeft;
	LY3DStereoBuild::GrayImageParam m_maskRight;
	LY3DStereoBuild::GrayImageParam m_maskCross;

	m_phaseLeft.width = param.width;
	m_phaseLeft.height = param.height;
	m_phaseLeft.data = new double[param.width*param.height];

	m_maskLeft.width = param.width;
	m_maskLeft.height = param.height;
	m_maskLeft.nChannel = 1;
	m_maskLeft.data = new unsigned char[param.width*param.height];

	m_phaseRight.width = param.width;
	m_phaseRight.height = param.height;
	m_phaseRight.data = new double[param.width*param.height];

	m_maskRight.width = param.width;
	m_maskRight.height = param.height;
	m_maskRight.nChannel = 1;
	m_maskRight.data = new unsigned char[param.width*param.height];

	m_phaseCross.width = param.width;
	m_phaseCross.height = param.height;
	m_phaseCross.data = new double[param.width*param.height];

	m_maskCross.width = param.width;
	m_maskCross.height = param.height;
	m_maskCross.nChannel = 1;
	m_maskCross.data = new unsigned char[param.width*param.height];

	LY3DAlgorithm::sn3DUnphaseWithDelete(m_phaseLeft, m_maskLeft, scanImagesLeft, param, phaseTh, grayTh);
	LY3DAlgorithm::sn3DUnphaseWithDelete(m_phaseRight, m_maskRight, scanImagesRight, param, phaseTh, grayTh);
	LY3DAlgorithm::sn3DUnphaseWithDelete(m_phaseCross, m_maskCross, scanImageCross, param, phaseTh, grayTh);

	ScanRange* range = new ScanRange;
	double *CX = new double;
	double *CY = new double;
	int nn = 0;
	bool ret = LY3DAlgorithm::sn3DPhaseStereoBuild(*range, m_phaseLeft, m_maskLeft, m_phaseRight, m_maskRight, CX, CY, nn, false);
	if (false == ret)
	{
		printf("sn3DPhaseStereoBuild Failed\n");
		return false;
	}

	std::cout << "range->numPt:" << range->numPt << std::endl;
	std::ofstream ofs("scan-result.asc");

	for (int i = 0; i < range->numPt; ++i)
	{
		ofs << range->xPt[i] << " " << range->yPt[i] << " " << range->zPt[i] << "\n";
	}
	ofs.close();

	std::vector<DistorData> data;
	ret = LY3DAlgorithm::sn3DRecognize(m_phaseLeft.data, m_phaseCross.data, ImageWidth, ImageHeigh, 15, data, m_maskLeft.data, m_maskCross.data, CX, CY, nn, *range);
	
	if (false == ret)
	{
		printf("sn3DRecognize Failed\n");
		return false;
	}

	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].u > 0 && data[i].u < ImageWidth && data[i].v > 0 && data[i].v < ImageHeigh)
		{
			Vector2 tmp2D;
			tmp2D[0] = data[i].u;
			tmp2D[1] = data[i].v;
			Vector3 tmp3D;
			tmp3D[0] = data[i].x;
			tmp3D[1] = data[i].y;
			tmp3D[2] = data[i].z;
			p2D.push_back(tmp2D);
			p3D.push_back(tmp3D);
		}
	}

	if (range->xPt != NULL)
	{
		delete[] range->xPt;
	}
	if (range->yPt != NULL)
	{
		delete[] range->yPt;
	}
	if (range->zPt != NULL)
	{
		delete[] range->zPt;
	}
	return true;
}

bool CtestCameraLibDlg::stereobuild_with_Ref(ScanImages& scanImagesLeft, ScanImages& scanImagesRight, std::string cameraPath, vector<Vector3>& p3D)
{
	int ImageWidth = scanImagesLeft.images[0].m_w;
	int ImageHeigh = scanImagesLeft.images[0].m_h;                                                         // 设定相机图像参数

	// 载入标定数据
	std::string CalibPathL = cameraPath + "\\LeftCCF.txt";                                               // 左相机标定文件路径
	std::string CalibPathR = cameraPath + "\\RightCCF.txt";                                              // 右相机标定文件路径
	printf("L:%s\n", CalibPathL.c_str());
	printf("R:%s\n", CalibPathR.c_str());

	CStreroPara StreroPara;

	StreroPara.CalibPathL = CalibPathL;
	StreroPara.LeftSize[0] = scanImagesLeft.images[0].m_w;
	StreroPara.LeftSize[1] = scanImagesLeft.images[0].m_h;

	StreroPara.CalibPathR = CalibPathR;
	StreroPara.RightSize[0] = scanImagesRight.images[0].m_w;
	StreroPara.RightSize[1] = scanImagesRight.images[0].m_h;

	LY3DAlgorithm::sn3DSetCamera(StreroPara);                                                            // 设定标定参数

	// 算子参数
	CannyParam canny;                                                                                // 图像边缘处理参数设定
	canny.sigma = 2.1;
	canny.thresholdHigh = 150;
	canny.thresholdLow = 75;
	
	CircleParam circle;
	circle.circularity = 2;
	circle.pointQuality = 0.4;
	circle.minDiameter = 6;
	circle.maxDiameter = 200;
	
	int m_circleCount = 17 * 14;																		 // m_circleCount 标定板大小
	long tmpImgSize = ImageWidth*ImageHeigh; 
	for (int k = 0; k < scanImagesLeft.imageCount; k++)                                                  // CaptureNums 一共采集了多少组
	{
		// 解左图
		GrayImageParam imageParamL;                                                                      // 提左右相机图像二维标志点坐标    
		imageParamL.width = scanImagesLeft.images[k].m_w;
		imageParamL.height = scanImagesLeft.images[k].m_h;
		imageParamL.data = scanImagesLeft.images[k].m_data;

		double *xl = new double[m_circleCount];
		double *yl = new double[m_circleCount];

		bool ret = LY3DAlgorithm::sn3DIdentifyPoints14X17(xl, yl, imageParamL, canny, circle);           // 二维标志点提取(标定板形式)
		if (!ret)                                                                                        // 如果二维点识别失败整组值设定为-1000
		{
			for (int L = 0; L < m_circleCount; L++)
			{
				Vector3 tmp3D;
				tmp3D[0] = -1000;
				tmp3D[1] = -1000;
				tmp3D[2] = -1000;
				p3D.push_back(tmp3D);
			}
			continue;
		}

		GrayImageParam imageParamR;
		imageParamR.width = scanImagesRight.images[k].m_w;
		imageParamR.height = scanImagesRight.images[k].m_h;
		imageParamR.data = scanImagesRight.images[k].m_data;

		double *xr = new double[m_circleCount];
		double *yr = new double[m_circleCount];

		ret = LY3DAlgorithm::sn3DIdentifyPoints14X17(xr, yr, imageParamR, canny, circle);

		if (!ret)
		{
			for (int L = 0; L < m_circleCount; L++)
			{
				Vector3 tmp3D;
				tmp3D[0] = -1000;
				tmp3D[1] = -1000;
				tmp3D[2] = -1000;
				p3D.push_back(tmp3D);
			}
			continue;
		}

		DataArray2D ptsLeft, ptsRight;
		DataArray3D	ptsWorld;

		ptsLeft.Alloc(m_circleCount); ptsLeft.numPt = m_circleCount;
		ptsRight.Alloc(m_circleCount); ptsRight.numPt = m_circleCount;
		ptsWorld.Alloc(m_circleCount); ptsWorld.numPt = m_circleCount;

		int *MaskData = new int[m_circleCount];                                                          // MaskData 用于标识一个标定板上的点位是否有效
		// 如果二维点的xy有-1存在,该点位记为无效
		int i;
		for (i = 0; i < m_circleCount; i++)
		{
			ptsLeft.pointx[i] = xl[i];
			ptsLeft.pointy[i] = yl[i];
			ptsRight.pointx[i] = xr[i];
			ptsRight.pointy[i] = yr[i];

			if (xl[i]<0 || yl[i]<0 || xr[i]<0 || yr[i]<0)
			{
				MaskData[i] = -1;
			}
			else
			{
				MaskData[i] = 1;
			}
		}

		LY3DAlgorithm::sn3DCorrespondStereoBuild(ptsWorld, ptsLeft, ptsRight);                      // 重建三维点

		for (i = 0; i < m_circleCount; i++)                                                           
		{              
			Vector3 tmp3D;
			if (MaskData[i] > 0)                                                               
			{
				tmp3D[0] = ptsWorld.pointx[i];
				tmp3D[1] = ptsWorld.pointy[i];
				tmp3D[2] = ptsWorld.pointz[i];
			}
			else
			{
				tmp3D[0] = -1000;
				tmp3D[1] = -1000;
				tmp3D[2] = -1000;
			}
			p3D.push_back(tmp3D);
		}
		delete[] xl; delete[] xr;
		delete[] yl; delete[] yr;

		ptsLeft.Free(); ptsRight.Free(); ptsWorld.Free();
	}
	return true;
}

/*
void CtestCameraLibDlg::OnBnClickedButtonGrab()
{
	// TODO:  在此添加控件通知处理程序代码
	
	int tmpImgCount = 28;
	ScanImages scanLImages;
	ScanImages scanRImages;
	ScanImages scanLHImages;
	scanLImages.imageCount = tmpImgCount;
	scanRImages.imageCount = tmpImgCount;
	scanLHImages.imageCount = tmpImgCount;
	int indexOffset = 0;
	for (int i = 0; i < tmpImgCount; ++i)
	{
		char lPath[200] = { 0 };
		char rPath[200] = { 0 };
		char lhPath[200] = { 0 };
		sprintf_s(lPath, 200, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\2GS\\Sc_0_%d.bmp", i);
		sprintf_s(rPath, 200, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\2GS\\Sc_1_%d.bmp", i);
		sprintf_s(lhPath, 200, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\2GS\\Sc_2_%d.bmp", i);
		int ret = sn3DImageLoad::sn3DReadImage(lPath, scanLImages.images[i]);
		ret = sn3DImageLoad::sn3DReadImage(rPath, scanRImages.images[i]);
		ret = sn3DImageLoad::sn3DReadImage(lhPath, scanLHImages.images[i]);
	}
	// 计算
	char tmpChildFolderPath[300] = { 0 };
	sprintf_s(tmpChildFolderPath, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\kk");
	char tmpCalibInfoPath[300] = { 0 };
	sprintf_s(tmpCalibInfoPath, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\200x150");
	char tmpDestFileName[300] = { 0 };
	sprintf_s(tmpDestFileName, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\kk\\kk.dml");


	printf("Stripe Rebuild\n");
	std::vector<Vector3> p3D;
	std::vector<Vector2> p2D;
	bool bRet = StereobuildDLP(scanLImages, scanRImages, scanLHImages, tmpCalibInfoPath, p3D, p2D);

	if (bRet){
		DLPMapLookupTable table;
		Sn3DAlgorithm::RetVal val = DLP_compute_map_relation(p3D, p2D, Vector2i(1920, 1080), m_bUsedInBigPlane ? Vector2(144, 81) : Vector2(96, 54), table, m_smoothRatio);
		cout << val << endl;
		cout << table.gridNum.transpose() << endl;
		cout << table.gridSize.transpose() << endl;
		cout << table.table[0].cols() << " " << table.table[0].rows() << endl;
		cout << table.table[1].cols() << " " << table.table[1].rows() << endl;
		if (0 == val)
			write_DLPMapLookupTable(tmpDestFileName, table);
		else
			bRet = false;
	}
	else
		printf("Rebuild Failed\n");

	for (int i = 0; i < scanLImages.imageCount; ++i){
		if (scanLImages.images[i].m_data){
			delete[](scanLImages.images[i].m_data);
			scanLImages.images[i].m_data = NULL;
		}
	}
	for (int i = 0; i < scanRImages.imageCount; ++i){
		if (scanRImages.images[i].m_data){
			delete[](scanRImages.images[i].m_data);
			scanRImages.images[i].m_data = NULL;
		}
	}
	if (bRet)
		MessageBox(_T("完成采集图片"));
	else
		MessageBox(_T("失败"));
	return;
}
*/

void CtestCameraLibDlg::OnBnClickedButtonGrab()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_imgGroupIndex < 0 || m_cameraList.size() <= 0)
	{
		MessageBox(_T("请先连接设备"));
		return;
	}

	if (NULL == m_hProjector)
	{
		MessageBox(_T("投影连接错误"));
		return;
	}

	// 循环采图
	HANDLE thrHandle;
	thrHandle = (HANDLE)_beginthreadex(NULL, 0, GrapPhotoThread, this, 0, NULL);
	CloseHandle(thrHandle);
}

unsigned int __stdcall CtestCameraLibDlg::WriteStripeThread(void* pVoid)
{
	if (NULL == pVoid)
	{
		TRACE("Pointor is NULL\n");
		Sleep(200);
		return 0;
	}

	return 0;
}

void CtestCameraLibDlg::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码

	if (m_imgGroupIndex < 0 || m_imgGroupIndex >= m_imgGroups.size())
	{
		MessageBox(_T("不存在图片"));
		return;
	}
	int tmpStartIndex = m_imgGroups[m_imgGroupIndex]->startIndex;
	m_hProjector->PorjectorDisplay(tmpStartIndex);
	return;



	TCHAR tmpFilePath[MAX_PATH] = { 0 };
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_PATHMUSTEXIST;
	ofn.lpstrFilter = _T("srp(*.srp)\0");
	ofn.lpstrInitialDir = _T("");
	ofn.lpstrTitle = _T("查找格式化条纹数据");
	ofn.lpstrFile = tmpFilePath;
	ofn.nMaxFile = sizeof(TCHAR)*MAX_PATH;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nFilterIndex = 0;
	ofn.hwndOwner = GetSafeHwnd();
	BOOL bSel = GetOpenFileName(&ofn);
	if (FALSE == bSel)
		return;

#ifdef _UNICODE
	size_t tmpConvertedSize = 0;
	wcstombs_s(&tmpConvertedSize, m_formatTxtFullPath, tmpFilePath, MAX_PATH);
	TRACE("%s\n",m_formatTxtFullPath); 
#else
	strcpy_s(m_formatTxtFullPath, MAX_PATH, tmpFilePath);
#endif
	m_bSuccessWriteStripe = false;
	ResetEvent(m_hFinishWrite);
	HANDLE tmpWriteThread = (HANDLE)_beginthreadex(NULL, 0, WriteStripeThread, this, 0, NULL);
	Sleep(1000);
	DWORD wRet = WaitForSingleObject(m_hFinishWrite, 600 * 1000);
	if (WAIT_OBJECT_0 != wRet || false == m_bSuccessWriteStripe)
	{
		MessageBox(_T("烧写图片失败"));
	}
	else
	{
		MessageBox(_T("烧写图片完成"));
	}
	return;
}


void CtestCameraLibDlg::OnBnClickedCheckLoopscan()
{
	// TODO:  在此添加控件通知处理程序代码
	int tmpVal = ((CButton*)GetDlgItem(IDC_CHECK_LOOPSCAN))->GetCheck();
	m_bEnableLoopScan = tmpVal ? true : false;
	WriteParam();
}


void CtestCameraLibDlg::OnCbnSelchangeComboImgGroup()
{
	// TODO:  在此添加控件通知处理程序代码
	int tmpSel = ((CComboBox*)GetDlgItem(IDC_COMBO_IMG_GROUP))->GetCurSel();
	m_imgGroupIndex = tmpSel;
	WriteParam();
}


unsigned int __stdcall CtestCameraLibDlg::SnapThread(void* pVoid)
{
	if (NULL == pVoid)
		return 0;

	CtestCameraLibDlg* pThis = (CtestCameraLibDlg*)pVoid;
	unsigned char* tmpShowBuf = NULL;
	while (pThis->m_bKeepLoop)
	{
		/*if (pThis->m_bHold)
		{
			Sleep(10);
		//	printf("BHold\n");
			continue;
		}*/
		Sleep(10);
		int tmpCamCount = pThis->m_cameraList.size();
		for (int i = 0; i < tmpCamCount; ++i)
		{
			if (pThis->m_cameraList[i])
			{
				unsigned char* tmpPtr = NULL;
				EnterCriticalSection(&pThis->m_snapLock);
				sn3DCamera::SnapShot(pThis->m_cameraList[i], tmpPtr);
				LeaveCriticalSection(&pThis->m_snapLock);
				if (tmpPtr)
				{
					long tmpSize = pThis->m_cameraParams[i]->width*pThis->m_cameraParams[i]->height;
					if (NULL == tmpShowBuf)
						tmpShowBuf = new unsigned char[tmpSize * 3];
					for (int i = 0; i < tmpSize; ++i)
					{
						tmpShowBuf[3 * i] = tmpPtr[i];
						tmpShowBuf[3 * i + 1] = tmpPtr[i];
						tmpShowBuf[3 * i + 2] = tmpPtr[i];
					}
					pThis->m_displayWnds[i]->SetImageSize(pThis->m_cameraParams[i]->width, pThis->m_cameraParams[i]->height);
					pThis->m_displayWnds[i]->SetImageData(tmpShowBuf);
					pThis->m_displayWnds[i]->Invalidate();
				}
				else
				{
					printf("Snap Failed\n");
				}
			}
		}
	}
	delete[]tmpShowBuf;
	tmpShowBuf = NULL;
	return 0;
}

void CtestCameraLibDlg::OnBnClickedCheckEnable()
{
	// TODO:  在此添加控件通知处理程序代码
	int nRet = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE))->GetCheck();
	if (nRet){
		m_bEnableCheckAfterGrab = true;
		GetDlgItem(IDC_RADIO_BIG)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SMALL)->EnableWindow(TRUE);
	}
	else{
		m_bEnableCheckAfterGrab = false;
		GetDlgItem(IDC_RADIO_BIG)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SMALL)->EnableWindow(FALSE);
	}
	WriteParam();
}


void CtestCameraLibDlg::OnBnClickedRadioBig()
{
	// TODO:  在此添加控件通知处理程序代码
	int nRet = ((CButton*)GetDlgItem(IDC_RADIO_BIG))->GetCheck();
	if (nRet){
		m_bUsedInBigPlane = true;
		((CButton*)GetDlgItem(IDC_RADIO_SMALL))->SetCheck(FALSE);
	}
	else{
		m_bUsedInBigPlane = false;
	}
	WriteParam();
}


void CtestCameraLibDlg::OnBnClickedRadioSmall()
{
	// TODO:  在此添加控件通知处理程序代码
	int nRet = ((CButton*)GetDlgItem(IDC_RADIO_SMALL))->GetCheck();
	if (nRet){
		m_bUsedInBigPlane = false;
		((CButton*)GetDlgItem(IDC_RADIO_BIG))->SetCheck(FALSE);
	}
	else{
		m_bUsedInBigPlane = true;
	}
	WriteParam();
}

unsigned int __stdcall CtestCameraLibDlg::GrapPhotoThread(void* pVoid)
{
	if (NULL == pVoid)
		return 0;
	CtestCameraLibDlg* pThis = (CtestCameraLibDlg*)pVoid;
	pThis->m_bHold = true;
	pThis->m_childDlg.SetFolderNamePtr(&pThis->m_targetSaveFolder);
	pThis->m_childDlg.DoModal();
	std::string mFolderName = MultiStringFromUnicodeString(pThis->m_targetSaveFolder);
	CString tmpwFolderPath = pThis->m_programPath + _T("saveImages\\") + pThis->m_targetSaveFolder;
	tmpwFolderPath += _T("\\");
	if (FALSE == PathFileExists(tmpwFolderPath))
		CreateDirectory(tmpwFolderPath, NULL);
	else
	{
		pThis->MessageBox(_T("目标文件夹重名，请检查。"));
		return 0;
	}

	std::string tmpFolderPath = MultiStringFromUnicodeString(pThis->m_programPath);
	int tmpWidth = 0;
	int tmpHeight = 0;
	SN_IMG_FORMAT tmpFormat;
	printf("lvc pThis->m_imgGroupIndex:%d", pThis->m_imgGroupIndex);
	int tmpImgCount = pThis->m_imgGroups[pThis->m_imgGroupIndex]->imgNames.size();
	int tmpStartIndex = pThis->m_imgGroups[pThis->m_imgGroupIndex]->startIndex;
	int tmpCamCount = pThis->m_cameraList.size();
	ScanImages scanLImages;
	ScanImages scanRImages;
	ScanImages scanLHImages;
	scanLImages.imageCount = 0;
	scanRImages.imageCount = 0;
	scanLHImages.imageCount = 0;
	int indexOffset = 0;
	for (int i = 0; i < tmpImgCount; ++i)
	{
		pThis->m_hProjector->PorjectorDisplay(tmpStartIndex + i);
		Sleep(pThis->m_waitTime);
		for (int j = 0; j < tmpCamCount; ++j)
		{
			if (56 == tmpImgCount && i >= 28)
				indexOffset = 2;
			unsigned char* tmpPtr = NULL;
			EnterCriticalSection(&pThis->m_snapLock);
			sn3DCamera::SnapShot(pThis->m_cameraList[j], tmpPtr);
			LeaveCriticalSection(&pThis->m_snapLock);
			if (tmpPtr)
			{
				sn3DImageData<unsigned char>* tmpImg = NULL;
				if (0 == j){
					if (0 == indexOffset){
						tmpImg = &(scanLImages.images[i]);
						++scanLImages.imageCount;
					}
					else{
						tmpImg = &(scanLHImages.images[i - 28]);
						++scanLHImages.imageCount;
					}
				}
				else{
					++scanRImages.imageCount;
					tmpImg = &(scanRImages.images[i]);
				}

				if (NULL == tmpImg)
					continue;

				sn3DCamera::GetResolution(pThis->m_cameraList[j], tmpWidth, tmpHeight);
				sn3DCamera::GetColorMode(pThis->m_cameraList[j], tmpFormat);
				tmpImg->create(tmpWidth, tmpHeight, SN_IMG_FORMAT::FORMAT_RGB888 == tmpFormat ? 3 : 1);

				memcpy(tmpImg->m_data, tmpPtr, tmpWidth*tmpHeight*(SN_IMG_FORMAT::FORMAT_RGB888 == tmpFormat ? 3 : 1));
				sprintf_s(tmpImg->m_name, 259, "%ssaveImages\\%s\\Sc_%d_%d.bmp", tmpFolderPath.c_str(), mFolderName.c_str(), j + indexOffset, (56 == tmpImgCount && i >= 28) ? i - 28 : i);
				int tmpRet = sn3DImageLoad::sn3DWriteImage(tmpImg->m_name, *tmpImg);
			}
		}
	}
	bool bSucessFinish = true;
	// 计算
	printf("Grab End\n");
	if (pThis->m_bEnableCheckAfterGrab){
		char tmpChildFolderPath[300] = { 0 };
		sprintf_s(tmpChildFolderPath, "%ssaveImages\\%s\\", tmpFolderPath.c_str(), mFolderName.c_str());
		char tmpCalibInfoPath[300] = { 0 };
		sprintf_s(tmpCalibInfoPath, "%s200x150", tmpFolderPath.c_str());
		char tmpDestFileName[300] = { 0 };
		sprintf_s(tmpDestFileName, "%s%s.dml", tmpChildFolderPath, mFolderName.c_str());
		if (85 == pThis->m_imgGroups[pThis->m_imgGroupIndex]->imgNames.size()){ // 标定板重建
			printf("Calib-board Rebuild\n");
			// 计算 2D (固定值)
			std::vector<float> widthSamples;
			for (int i = 0; i < 17; ++i)
				widthSamples.push_back(1600 - i * 80);
			std::vector<float> heightSamples;
			for (int i = 0; i < 14; ++i)
				heightSamples.push_back(1060 - i * 80);
			std::vector<Vector2> offsets;
			offsets.emplace_back(0, 0);
			vector<float> tw = { -320.f, -306.667f, -293.333f, -280.f, -266.667f, -253.333f, 253.333f, 266.667f, 280.f, 293.333f, 306.667f, 320.f };
			vector<float> th = { 40.f, 26.667f, 13.333f, 0.f, -13.333f, -26.667f, -40.f };
			for (int i = 0; i < th.size(); ++i)
			{
				for (int j = 0; j < tw.size(); ++j)
					offsets.emplace_back(tw[j], th[i]);
			}
			vector<Vector3> p3Ds;
			bool bRet = pThis->stereobuild_with_Ref(scanLImages, scanRImages, tmpCalibInfoPath, p3Ds);
			if (bRet){
				DLPMapLookupTable table;
				Sn3DAlgorithm::RetVal val = DLP_compute_map_use_calibration_board(widthSamples, heightSamples, offsets, p3Ds, Vector2i(1920, 1080), pThis->m_bUsedInBigPlane ? Vector2(144, 81) : Vector2(96, 54), table, pThis->m_smoothRatio);
				cout << val << endl;
				cout << table.gridNum.transpose() << endl;
				cout << table.gridSize.transpose() << endl;
				cout << table.table[0].cols() << " " << table.table[0].rows() << endl;
				cout << table.table[1].cols() << " " << table.table[1].rows() << endl;
				if (0 != val){
					printf("DLP_compute_map_use_calibration_board ReturnError: %d\n", val);
					bSucessFinish = false;
				}
				else
					write_DLPMapLookupTable(tmpDestFileName, table);
			}
			else
				printf("Rebuild Failed\n");
		}
		else{ // 条纹重建

			printf("Stripe Rebuild\n");
			std::vector<Vector3> p3D;
			std::vector<Vector2> p2D;
			bool bRet = pThis->StereobuildDLP(scanLImages, scanRImages, scanLHImages, tmpCalibInfoPath, p3D, p2D);
			if (bRet){
				DLPMapLookupTable table;
				Sn3DAlgorithm::RetVal val = DLP_compute_map_relation(p3D, p2D, Vector2i(1920, 1080), pThis->m_bUsedInBigPlane ? Vector2(144, 81) : Vector2(96, 54), table, pThis->m_smoothRatio);
				if (0 != val){
					bSucessFinish = false;
				}
				else
					write_DLPMapLookupTable(tmpDestFileName, table);
			}
			else
				printf("Rebuild Failed\n");
		}
	}

	for (int i = 0; i < scanLImages.imageCount; ++i){
		if (scanLImages.images[i].m_data){
			delete[](scanLImages.images[i].m_data);
			scanLImages.images[i].m_data = NULL;
		}
	}
	for (int i = 0; i < scanRImages.imageCount; ++i){
		if (scanRImages.images[i].m_data){
			delete[](scanRImages.images[i].m_data);
			scanRImages.images[i].m_data = NULL;
		}
	}
	for (int i = 0; i < scanLHImages.imageCount; ++i){
		if (scanLHImages.images[i].m_data){
			delete[](scanLHImages.images[i].m_data);
			scanLHImages.images[i].m_data = NULL;
		}
	}
	pThis->m_bHold = false;

	return 0;
}

unsigned int __stdcall CtestCameraLibDlg::TestGrapPhotoThread(void* pVoid)
{
	if (NULL == pVoid)
		return 0;

	CtestCameraLibDlg* pThis = (CtestCameraLibDlg*)pVoid;
	int tmpImgCount = 28;
	ScanImages scanLImages;
	ScanImages scanRImages;
	ScanImages scanLHImages;
	scanLImages.imageCount = tmpImgCount;
	scanRImages.imageCount = tmpImgCount;
	scanLHImages.imageCount = tmpImgCount;
	int indexOffset = 0;
	for (int i = 0; i < tmpImgCount; ++i)
	{
		char lPath[200] = { 0 };
		char rPath[200] = { 0 };
		char lhPath[200] = { 0 };
		sprintf_s(lPath, 200, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\2GS\\Sc_0_%d.bmp", i);
		sprintf_s(rPath, 200, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\2GS\\Sc_1_%d.bmp", i);
		sprintf_s(lhPath, 200, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\2GS\\Sc_2_%d.bmp", i);
		int ret = sn3DImageLoad::sn3DReadImage(lPath, scanLImages.images[i]);
		ret = sn3DImageLoad::sn3DReadImage(rPath, scanRImages.images[i]);
		ret = sn3DImageLoad::sn3DReadImage(lhPath, scanLHImages.images[i]);
	}
	// 计算
	char tmpChildFolderPath[300] = { 0 };
	sprintf_s(tmpChildFolderPath, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\kk");
	char tmpCalibInfoPath[300] = { 0 };
	sprintf_s(tmpCalibInfoPath, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\200x150");
	char tmpDestFileName[300] = { 0 };
	sprintf_s(tmpDestFileName, "H:\\GrapImage\\testCameraLib-ForDLPSnap\\x64\\Release\\saveImages\\ScanImages\\kk\\kk.dml");


	printf("Stripe Rebuild\n");
	std::vector<Vector3> p3D;
	std::vector<Vector2> p2D;
	bool bRet = pThis->StereobuildDLP(scanLImages, scanRImages, scanLHImages, tmpCalibInfoPath, p3D, p2D);

	if (bRet){
		DLPMapLookupTable table;
		Sn3DAlgorithm::RetVal val = DLP_compute_map_relation(p3D, p2D, Vector2i(1920, 1080), pThis->m_bUsedInBigPlane ? Vector2(144, 81) : Vector2(96, 54), table, pThis->m_smoothRatio);
		cout << val << endl;
		cout << table.gridNum.transpose() << endl;
		cout << table.gridSize.transpose() << endl;
		cout << table.table[0].cols() << " " << table.table[0].rows() << endl;
		cout << table.table[1].cols() << " " << table.table[1].rows() << endl;
		if (0 == val)
			write_DLPMapLookupTable(tmpDestFileName, table);
		else
			bRet = false;
	}
	else
		printf("Rebuild Failed\n");

	for (int i = 0; i < scanLImages.imageCount; ++i){
		if (scanLImages.images[i].m_data){
			delete[](scanLImages.images[i].m_data);
			scanLImages.images[i].m_data = NULL;
		}
	}
	for (int i = 0; i < scanRImages.imageCount; ++i){
		if (scanRImages.images[i].m_data){
			delete[](scanRImages.images[i].m_data);
			scanRImages.images[i].m_data = NULL;
		}
	}
	if (bRet)
		pThis->MessageBox(_T("完成采集图片"));
	else
		pThis->MessageBox(_T("失败"));
	return 0;
}
