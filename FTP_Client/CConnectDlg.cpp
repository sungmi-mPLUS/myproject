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
{

}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialogEx)
END_MESSAGE_MAP()


// CConnectDlg 메시지 처리기
