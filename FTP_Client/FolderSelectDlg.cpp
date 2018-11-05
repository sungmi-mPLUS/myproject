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
	, m_strBackupFolder(_T(""))
{

}

FolderSelectDlg::~FolderSelectDlg()
{
}

void FolderSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_BACKUP_F, m_strBackupFolder);
}


BEGIN_MESSAGE_MAP(FolderSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACKUPFOLDER, &FolderSelectDlg::OnBnClickedBtnBackupfolder)
END_MESSAGE_MAP()


// FolderSelectDlg 메시지 처리기


void FolderSelectDlg::OnBnClickedBtnBackupfolder()
{
	CString strDefPath;
	strDefPath = GetSelectFolderPath();
	if (strDefPath.IsEmpty())
	{
		AfxMessageBox(_T(" 폴더가 선택되지 않았습니다."));
		return;
	}

	m_strBackupFolder = strDefPath;
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

