
// FTP_ClientDlg.h: 헤더 파일
//

#pragma once

#define TIMER_CNT 5

enum ARRTIMER {
	TIMER1 = 0,//200,
	TIMER2,
	TIMER3,
	TIMER4,
	TIMER5,
	TIMER6,
	TIMER7,
	TIMER8,
	TIMER9
};
struct SendingItem {
	CString strLocalFolder;
	CString strTargetFolder;
	CString strBackupFolder;
	int nDelayTime;
	BOOL IsUse;
	UINT_PTR nIDEvent;
	void Initialize()
	{
		strLocalFolder = _T("D://");
		strTargetFolder = _T("/");
		strBackupFolder = strLocalFolder +_T("/Backup");
		nDelayTime = 1;
		IsUse = FALSE;
		nIDEvent = TIMER1;
	};

};

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
	SendingItem mCondition[TIMER_CNT];
	int m_ConditionIndex;
	int m_nUseTime[TIMER_CNT];

	////////   FTP test
	CInternetSession m_Session;
	CFtpConnection *m_pConnection;
	CFtpFileFind *m_pFileFind;

	CString	m_strLocalDir;
	CString	m_strLocalDirText;
	CString	m_strRemoteDirText;
	CString	m_strRemoteDir;
	CString	m_strLocalFile;
	CString	m_strRemoteFile;

	// 저장항목
	CString m_arrFileData[3];
	CString m_arrFileList[TIMER_CNT];
	bool ReadFIleData();
	void WriteFIleData();
//	bool ReadFIleData();
//	void WriteFIleData();

	//file control
	CString FindNewFile(CString strPath);// 오래된 file부터 찾기
	BOOL doMoveFile(CString strPath, CString strFileName);

	BOOL OnUpload(CString m_strLocalFile, CString m_strLocalDirText, CString m_strRemoteDirText);


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
//	afx_msg void OnBnClickedUploadBtn();
//	afx_msg void OnBnClickedDownBtn();
	afx_msg void OnDestroy();
//	afx_msg void OnLbnDblclkListLdir();
//	afx_msg void OnLbnDblclkListRdir();

	CString m_strConInfo;
	afx_msg void OnBnClickedMfcbuttonThread();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_list_ctrl;
	afx_msg void OnBnClickedMfcbuttonThread2();

	CStatic m_ctlStr[TIMER_CNT];
	CListBox m_ctlList[TIMER_CNT];

	CString TimerFile[TIMER_CNT];
	CString TimerBackup[TIMER_CNT];
	afx_msg void OnBnClickedMfcbutton2();
	CString GetSelectFolderPath();

	afx_msg void OnBnClickedButton1();
	BOOL isLogView;
	void SettingTimer(int nIndex, CString LoFolder, CString TaFolder, int nDelay);
	void InputListData();

	afx_msg void OnBnClickedOk();
};
