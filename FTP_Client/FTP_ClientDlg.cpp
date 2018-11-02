
// FTP_ClientDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FTP_Client.h"
#include "FTP_ClientDlg.h"
#include "afxdialogex.h"
#include "CConnectDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTPClientDlg 대화 상자



CFTPClientDlg::CFTPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FTP_CLIENT_DIALOG, pParent)
{
	m_strLocalDir = _T("");
	m_strLocalDirText = _T("");
	m_strRemoteDirText = _T("");
	m_strRemoteDir = _T("");
	m_strLocalFile = _T("");
	m_strRemoteFile = _T("");

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pFileFind = NULL;
	m_pConnection = NULL;
}

void CFTPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RFILE, m_ctrlRemoteFile);
	DDX_Control(pDX, IDC_LIST_LFILE, m_ctrlLocalFile);
	DDX_Control(pDX, IDC_LIST_RDIR, m_ctrlRemoteDir);
	DDX_Control(pDX, IDC_LIST_LDIR, m_ctrlLocalDir);

	DDX_LBString(pDX, IDC_LIST_LDIR, m_strLocalDir);
	DDX_Text(pDX, IDC_LOCALDIRTEXT, m_strLocalDirText);
	DDX_Text(pDX, IDC_REMOTEDIRTEXT, m_strRemoteDirText);
	DDX_LBString(pDX, IDC_LIST_RDIR, m_strRemoteDir);
	DDX_LBString(pDX, IDC_LIST_LFILE, m_strLocalFile);
	DDX_LBString(pDX, IDC_LIST_RFILE, m_strRemoteFile);

}

BEGIN_MESSAGE_MAP(CFTPClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CFTPClientDlg::OnBnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_UPLOAD_BTN, &CFTPClientDlg::OnBnClickedUploadBtn)
	ON_BN_CLICKED(IDC_DOWN_BTN, &CFTPClientDlg::OnBnClickedDownBtn)

	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST_LDIR, &CFTPClientDlg::OnLbnDblclkListLdir)
	ON_LBN_DBLCLK(IDC_LIST_RDIR, &CFTPClientDlg::OnLbnDblclkListRdir)

END_MESSAGE_MAP()


// CFTPClientDlg 메시지 처리기

BOOL CFTPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	UpdateLocalListBox();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFTPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFTPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFTPClientDlg::OnBnClickedMfcbutton1()
{
	//CConnectDlg dlg;
	//dlg.DoModal();


	BeginWaitCursor();

	// 이미 연결된 것이 있으면 해제
	if (m_pConnection != NULL)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	// 연결하고자하는 FTP 서버의 IP 주소와 사용자명, 암호 입력
	CConnectDlg dlg;
	if (dlg.DoModal() != IDOK) return;

	
	// Anonymous FTP로 로그인 하는 경우
	if ((dlg.m_strPassword).GetLength() == 0)
		m_pConnection = m_Session.GetFtpConnection(dlg.m_strAddress);

	// 사용자명과 암호를 입력하는 경우
	else
		m_pConnection = m_Session.GetFtpConnection(dlg.m_strAddress,
			dlg.m_strName, dlg.m_strPassword);

	// 연결이 안됐을 경우
	if (!m_pConnection)
	{
		AfxMessageBox("연결이 않됩니다.", MB_OK);
		m_pConnection = NULL;
		return;
	}

	// 현재 디렉터리를 얻어 화면에 표시
	m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
	UpdateData(FALSE);

	if (m_pFileFind)
		delete m_pFileFind;

	// FTP 서버의 파일명을 얻어옴
	m_pFileFind = new CFtpFileFind(m_pConnection);

	// 디렉터리 구조 표시
	UpdateRemoteListBox();
	
	EndWaitCursor();
}


void CFTPClientDlg::OnBnClickedUploadBtn()
{
	if (m_pConnection)
	{
		BeginWaitCursor();

		UpdateData();
		CString strFilename, strPathname;

		// 디렉터리명과 파일명을 붙이기	
		strPathname = m_strRemoteDirText + "/" + m_strLocalFile;

		// 파일 업로드
		m_pConnection->PutFile(m_strLocalFile, strPathname);

		UpdateRemoteListBox();
		EndWaitCursor();
	}
}


void CFTPClientDlg::OnBnClickedDownBtn()
{
	if (m_pConnection)
	{
		BeginWaitCursor();

		UpdateData();
		CString strFilename, strPathname;

		// 디렉터리명과 파일명을 붙이기	
		strPathname = m_strRemoteDirText + "/" + m_strRemoteFile;

		// 파일 다운로드
		m_pConnection->GetFile(strPathname, m_strRemoteFile);

		// 파일 List Box 컨트롤 갱신
		UpdateLocalListBox();

		EndWaitCursor();
	}

}


void CFTPClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// 이미 연결된 것이 있으면 해제
	if (m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	if (m_pFileFind)
		delete m_pFileFind;
}

void CFTPClientDlg::UpdateLocalListBox()
{
	// 기존에 있는 것을 모두 지우고...
	m_ctrlLocalFile.ResetContent();
	m_ctrlLocalDir.ResetContent();

	// 현재 디렉터리 표시
	char buff[256];
	_getcwd(buff, 255);
	m_strLocalDirText = buff;
	UpdateData(FALSE);

	// 파일명 표시
	m_ctrlLocalFile.Dir(0x00, "*.*");

	// 디렉터리명 표시
	m_ctrlLocalDir.Dir(0x8010, "*.*");
}

void CFTPClientDlg::UpdateRemoteListBox()
{
	CString str;
	BOOL bContinue = TRUE;

	// 기존에 있던 것을 지우고...
	m_ctrlRemoteDir.ResetContent();
	m_ctrlRemoteFile.ResetContent();

	m_pFileFind->FindFile(m_strRemoteDirText);

	// 상위 디렉터리로 이동할 수 있도록...
	m_ctrlRemoteDir.AddString("[..]");
	while (bContinue)
	{
		// FTP 서버의 파일명을 계속 얻어옴
		bContinue = m_pFileFind->FindNextFile();
		str = m_pFileFind->GetFileName();

		// 디렉터리 인 경우
		if (m_pFileFind->IsDirectory())
		{
			// 디렉터리이면 "[ ]"를 추가
			str = "[" + str + "]";
			m_ctrlRemoteDir.AddString(str);
		}
		// 파일인 경우
		else
			m_ctrlRemoteFile.AddString(str);
	}
}


void CFTPClientDlg::OnLbnDblclkListLdir()
{
	UpdateData();

	// 양쪽의 "["와 "]" 제거
	m_strLocalDir = m_strLocalDir.Left(m_strLocalDir.GetLength() - 1);
	m_strLocalDir = m_strLocalDir.Right(m_strLocalDir.GetLength() - 1);

	// 디렉터리 변경
	_chdir(m_strLocalDir);

	// 파일 List Box 컨트롤 갱신
	UpdateLocalListBox();
}

void CFTPClientDlg::OnLbnDblclkListRdir()
{
	BeginWaitCursor();

	UpdateData();

	// 양쪽의 "["와 "]" 제거
	m_strRemoteDir = m_strRemoteDir.Left(m_strRemoteDir.GetLength() - 1);
	m_strRemoteDir = m_strRemoteDir.Right(m_strRemoteDir.GetLength() - 1);

	//".."가 눌려 한 수준 위로 올라가는 경우
	if (m_strRemoteDir == "..") {
		m_strRemoteDirText = m_strRemoteDirText.Left(
			m_strRemoteDirText.ReverseFind('/'));
		if (m_strRemoteDirText == "") m_strRemoteDirText += "/";
	}
	else
		m_strRemoteDirText += "/" + m_strRemoteDir;

	// 디렉터리 이동
	if (!m_pConnection->SetCurrentDirectory(m_strRemoteDirText))
	{
		m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
		AfxMessageBox("디렉터리 이동이 불가능합니다.");
		return;
	}

	// 현재 디렉터리를 화면에 표시
	m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
	UpdateData(FALSE);

	UpdateRemoteListBox();
}


