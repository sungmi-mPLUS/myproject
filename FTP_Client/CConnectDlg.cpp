// CConnectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FTP_Client.h"
#include "CConnectDlg.h"
#include "afxdialogex.h"


// CConnectDlg 대화 상자

IMPLEMENT_DYNAMIC(CConnectDlg, CDialogEx)

CConnectDlg::CConnectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONNECT_DIALOG, pParent)
	, m_strAddress(_T("192.168.1.57"))
	, m_DwAddress(3232235833)
{
	m_strPassword = _T("1111");
//	m_strAddress = _T("x");
	m_strName = _T("bbbb");
//	m_ctlIP.SetWindowTextW(m_strAddress);
}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASSWORD, m_ctrlPassword);
	//DDX_Control(pDX, IDC_IP_ADDRESS, m_ctrlFtpAddress);
	//DDX_Control(pDX, IDC_NAME, m_ctrlName);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//	DDX_CBString(pDX, IDC_IP_ADDRESS, m_strAddress);
	//	DDX_CBString(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_IPAddress(pDX, IDC_IP_ADDRESS, m_DwAddress);
	DDX_Control(pDX, IDC_IP_ADDRESS, m_ctlIP);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConnectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConnectDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CConnectDlg 메시지 처리기


void CConnectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_ctlIP.GetWindowTextW(m_strAddress);

	CDialogEx::OnOK();
}


void CConnectDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CConnectDlg::ChangeValue()
{
	unsigned bytes[4];
	int fields = _stscanf(m_strAddress.GetString(), _T("%u.%u.%u.%u"),
	bytes + 3, bytes + 2, bytes + 1, bytes);

	m_DwAddress = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}

