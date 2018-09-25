// FolderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testCameraLib.h"
#include "FolderDlg.h"
#include "afxdialogex.h"


// CFolderDlg 对话框

IMPLEMENT_DYNAMIC(CFolderDlg, CDialogEx)

CFolderDlg::CFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFolderDlg::IDD, pParent)
{
	m_editString = NULL;
}

CFolderDlg::~CFolderDlg()
{
}

BOOL CFolderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CenterWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFolderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CFolderDlg::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CFolderDlg 消息处理程序

BOOL CFolderDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST){
		if (VK_RETURN == pMsg->wParam){
			CFolderDlg::OnBnClickedButtonOk();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CFolderDlg::OnClose()
{
	printf("Close Window\n");
	CDialogEx::OnClose();
}

void CFolderDlg::OnPaint()
{
	
	CDialogEx::OnPaint();
}

void CFolderDlg::OnBnClickedButtonOk()
{
	// TODO:  在此添加控件通知处理程序代码
	if (NULL == m_editString)
		return;

	GetDlgItemText(IDC_EDIT_FOLDERNAME, *m_editString);
	if (_T("") == *m_editString){
		SYSTEMTIME tmpTime;
		GetLocalTime(&tmpTime);
		m_editString->Format(_T("%d-%d-%d_%d-%d-%d"), tmpTime.wYear, tmpTime.wMonth, tmpTime.wDay, tmpTime.wHour, tmpTime.wMinute, tmpTime.wSecond);
	}
	this->PostMessage(WM_CLOSE);
}
