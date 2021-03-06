#pragma once


// CConnectDlg 대화 상자

class CConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConnectDlg();
//	CEdit	m_ctrlPassword;
	CString	m_strPassword;
//	CString	m_strAddress;
	CString	m_strName;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlPassword;
	CString m_strAddress;
	DWORD m_DwAddress;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl m_ctlIP;
	void ChangeValue();
};
