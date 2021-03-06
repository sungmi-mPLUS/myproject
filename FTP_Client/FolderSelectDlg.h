#pragma once


// FolderSelectDlg 대화 상자

class FolderSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FolderSelectDlg)

public:
	FolderSelectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FolderSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UPLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedBtnBackupfolder();
//	CString GetSelectFolderPath();

	CString m_strLocalFolder;
	CString m_strTargetFolder;
	int m_radioIndex;
	afx_msg void OnBnClickedOk();
	CEdit m_ctlTargetFolder;
	int nDelayNum;
	CEdit m_ctlLocalFolder;
};
