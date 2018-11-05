
// FTP_ClientDlg.h: 헤더 파일
//

#pragma once


// CFTPClientDlg 대화 상자
class CFTPClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CFTPClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTP_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


public:

	void UpdateRemoteListBox();
	void UpdateLocalListBox();
	CInternetSession m_Session;
	CFtpConnection *m_pConnection;
	CFtpFileFind *m_pFileFind;

	CString	m_strLocalDir;
	CString	m_strLocalDirText;
	CString	m_strRemoteDirText;
	CString	m_strRemoteDir;
	CString	m_strLocalFile;
	CString	m_strRemoteFile;

	CListBox	m_ctrlRemoteFile;
	CListBox	m_ctrlLocalFile;
	CListBox	m_ctrlRemoteDir;
	CListBox	m_ctrlLocalDir;

	// 저장항목
	CString m_arrFileData[10];
	void ReadFIleData();
	void WriteFIleData();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnBnClickedUploadBtn();
	afx_msg void OnBnClickedDownBtn();
	afx_msg void OnDestroy();
	afx_msg void OnLbnDblclkListLdir();
	afx_msg void OnLbnDblclkListRdir();

	CString m_strConInfo;
	afx_msg void OnBnClickedMfcbuttonThread();
};
