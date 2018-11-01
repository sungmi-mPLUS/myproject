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
	, m_strAddress(_T(""))
	, m_DwAddress(0)
{
	m_strPassword = _T("1111");
	m_strAddress = _T("192.168.250.120");
	m_strName = _T("anodepre");
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
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialogEx)
END_MESSAGE_MAP()


// CConnectDlg 메시지 처리기
