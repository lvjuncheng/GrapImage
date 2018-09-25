
// testCameraLibDlg.h : header file
//

#pragma once
#include "DisplayWnd.h"
#include <string>
#include <vector>
#include "afxwin.h"
#include "sn3DCamera.h"
#include "sn3DProjector\Projector\Projector.h"
#include "sn3DImageData.h"
#include "algorithmLyInclude.h"
#include "algorithmAuxilaryInclude.h"
#include "FolderDlg.h"

#define DEFAULT_CAMERACOUNT 2
#define DEFAULT_SCANSTRIPE_COUNT 15

struct LOGBUF
{
	int projType;
	int camType;
	bool bEnableLoopScan;
	long exposure;
	int gain;
	int gainStart;
	int waitTime;
	int imgGroupIndex;
	bool bUseBigPlane;
	bool bEnableCheckAfterGrab;
	double smoothRatio;
};

typedef struct _IMGGROUP
{
	std::string groupName;
	int startIndex;
	std::vector<std::string> imgNames;
}IMGGROUP, *PIMGGROUP;

typedef struct _CAMERAINFO
{
	char serial[20];
	int width;
	int height;
	int trigMode; // [0:Continue, 1 : External - Trigger]
}CAMINFO, *PCAMINFO;

// CtestCameraLibDlg dialog
class CtestCameraLibDlg : public CDialogEx
{
// Construction
public:
	CtestCameraLibDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTCAMERALIB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	static unsigned int __stdcall WriteStripeThread(void* pVoid);
	static unsigned int __stdcall SnapThread(void* pVoid);
	bool ReadParam();
	bool WriteParam();
	bool StereobuildDLP(ScanImages& scanImagesLeft, ScanImages& scanImagesRight, ScanImages& scanImageCross, std::string cameraPath, vector<Eigen::Vector3>& p3D, vector<Eigen::Vector2>& p2D);
	bool stereobuild_with_Ref(ScanImages& scanImagesLeft, ScanImages& scanImagesRight, std::string cameraPath, vector<Eigen::Vector3>& p3D);

private:
	int m_cameraType;
	std::vector<PCAMINFO> m_cameraParams;
	CDisplayWnd* m_displayWnds[2];
	unsigned char* m_displayImgBufs[2];
	bool m_bEnableLoopScan;

	long m_defaultExposure;
	int  m_defaultGain;
	int  m_startGain;

	int  m_grabCount;
	bool m_grabbing;
	bool m_hasGrabbed[DEFAULT_CAMERACOUNT];
	HANDLE m_hasGrabEvent[DEFAULT_CAMERACOUNT];
	CString m_savePath;
	CString m_programPath;

	char m_formatTxtFullPath[260];
	bool m_bSuccessWriteStripe;
	HANDLE m_hFinishWrite;

	CRITICAL_SECTION m_snapLock;
	Projector* m_hProjector;
	std::vector<PIMGGROUP> m_imgGroups;
	int m_imgGroupIndex;
	std::vector<sn3DCamera::HCAMERA> m_cameraList;
	HANDLE m_hSnapThread;
	volatile bool m_bKeepLoop;
	volatile bool m_bHold;
	long m_waitTime;

	CString m_targetSaveFolder;
	bool m_bEnableCheckAfterGrab;
	bool m_bUsedInBigPlane;
	double m_smoothRatio;

	CFolderDlg m_childDlg;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnStnDblclickStaticShow0();
	afx_msg void OnStnClickedStaticShow1();
	afx_msg void OnBnClickedRadioWhite();
	afx_msg void OnBnClickedRadioTeststripe();
	afx_msg void OnBnClickedRadioCross();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnCbnSelchangeComboProjtype();
	afx_msg void OnCbnSelchangeComboCamtype();
	afx_msg void OnBnClickedButtonGrab();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheckLoopscan();
	afx_msg void OnCbnSelchangeComboImgGroup();
	afx_msg void OnBnClickedCheckEnable();
	afx_msg void OnBnClickedRadioBig();
	afx_msg void OnBnClickedRadioSmall();
	afx_msg void OnBnClickedButtonNewfolder();
};
