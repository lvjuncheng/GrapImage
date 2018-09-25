#pragma once


// CFolderDlg 对话框

class CFolderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFolderDlg)

public:
	CFolderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFolderDlg();

// 对话框数据
	enum { IDD = IDD_EDITFOLDERNAME };
	void SetFolderNamePtr(CString* folderName){ m_editString = folderName; }

private:
	CString* m_editString;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
};
