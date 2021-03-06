// FolderSelectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FTP_Client.h"
#include "FolderSelectDlg.h"
#include "afxdialogex.h"


// FolderSelectDlg 대화 상자

IMPLEMENT_DYNAMIC(FolderSelectDlg, CDialogEx)

FolderSelectDlg::FolderSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UPLOAD, pParent)
	, m_strLocalFolder(_T(""))
	, m_radioIndex(0)
	, nDelayNum(200)
{

}

FolderSelectDlg::~FolderSelectDlg()
{
}

void FolderSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STATIC_BACKUP_F, m_strLocalFolder);
	DDX_Radio(pDX, IDC_RADIO1, m_radioIndex);
	DDX_Control(pDX, IDC_EDIT1, m_ctlTargetFolder);
	DDX_Text(pDX, IDC_EDIT2, nDelayNum);
	DDV_MinMaxInt(pDX, nDelayNum, 200, 5000);
	DDX_Control(pDX, IDC_EDIT_LOCAL, m_ctlLocalFolder);
}


BEGIN_MESSAGE_MAP(FolderSelectDlg, CDialogEx)
//	ON_BN_CLICKED(IDC_BTN_BACKUPFOLDER, &FolderSelectDlg::OnBnClickedBtnBackupfolder)
	ON_BN_CLICKED(IDOK, &FolderSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// FolderSelectDlg 메시지 처리기

/*
void FolderSelectDlg::OnBnClickedBtnBackupfolder()
{
	CString strDefPath;
	strDefPath = GetSelectFolderPath();
	if (strDefPath.IsEmpty())
	{
		AfxMessageBox(_T(" 폴더가 선택되지 않았습니다."));
		return;
	}

	m_strLocalFolder = strDefPath;
	UpdateData(FALSE);
}

CString FolderSelectDlg::GetSelectFolderPath()
{
	CString strFolderPath;

	ITEMIDLIST *pidlBrowse;
	TCHAR       pszPathname[MAX_PATH];
	BROWSEINFO  BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL; //# Null이면 최초 위치는 "바탕화면"입니다.

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if (pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}

	strFolderPath.Format(_T("%s"), pszPathname);

	return strFolderPath;
}

*/

void FolderSelectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_ctlTargetFolder.GetWindowTextW(m_strTargetFolder);
	m_ctlLocalFolder.GetWindowTextW(m_strLocalFolder);

	CDialogEx::OnOK();
}
