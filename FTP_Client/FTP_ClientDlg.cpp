
// FTP_ClientDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FTP_Client.h"
#include "FTP_ClientDlg.h"
#include "afxdialogex.h"
#include "CConnectDlg.h"
#include "FolderSelectDlg.h"

#include <fstream>
#include <iostream>

using namespace std;
#define MAX_SIZE 1000

char inputString[MAX_SIZE];

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTPClientDlg 대화 상자



CFTPClientDlg::CFTPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FTP_CLIENT_DIALOG, pParent)
	, m_strConInfo(_T("Not Connect"))
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
	//	DDX_Control(pDX, IDC_LIST_RFILE, m_ctrlRemoteFile);
	//	DDX_Control(pDX, IDC_LIST_LFILE, m_ctrlLocalFile);
	//	DDX_Control(pDX, IDC_LIST_RDIR, m_ctrlRemoteDir);
	//	DDX_Control(pDX, IDC_LIST_LDIR, m_ctrlLocalDir);

	//DDX_LBString(pDX, IDC_LIST_LDIR, m_strLocalDir);
	DDX_Text(pDX, IDC_LOCALDIRTEXT, m_strLocalDirText);
	//DDX_Text(pDX, IDC_REMOTEDIRTEXT, m_strRemoteDirText);
	//DDX_LBString(pDX, IDC_LIST_RDIR, m_strRemoteDir);
	//DDX_LBString(pDX, IDC_LIST_LFILE, m_strLocalFile);
	//DDX_LBString(pDX, IDC_LIST_RFILE, m_strRemoteFile);

	DDX_Text(pDX, IDC_CONNECT_INFO, m_strConInfo);
	DDX_Control(pDX, IDC_LIST_THREAD, m_list_ctrl);
	DDX_Control(pDX, IDC_LIST1, m_ctlList[0]);
	DDX_Control(pDX, IDC_LIST2, m_ctlList[1]);
	DDX_Control(pDX, IDC_LIST3, m_ctlList[2]);
	DDX_Control(pDX, IDC_LIST4, m_ctlList[3]);
	DDX_Control(pDX, IDC_LIST5, m_ctlList[4]);
	DDX_Control(pDX, IDC_FOLDER1, m_ctlStr[0]);
	DDX_Control(pDX, IDC_FOLDER2, m_ctlStr[1]);
	DDX_Control(pDX, IDC_FOLDER3, m_ctlStr[2]);
	DDX_Control(pDX, IDC_FOLDER4, m_ctlStr[3]);
	DDX_Control(pDX, IDC_FOLDER5, m_ctlStr[4]);
}

BEGIN_MESSAGE_MAP(CFTPClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CFTPClientDlg::OnBnClickedMfcbutton1)
//	ON_BN_CLICKED(IDC_UPLOAD_BTN, &CFTPClientDlg::OnBnClickedUploadBtn)
//	ON_BN_CLICKED(IDC_DOWN_BTN, &CFTPClientDlg::OnBnClickedDownBtn)

	ON_WM_DESTROY()
//	ON_LBN_DBLCLK(IDC_LIST_LDIR, &CFTPClientDlg::OnLbnDblclkListLdir)
//	ON_LBN_DBLCLK(IDC_LIST_RDIR, &CFTPClientDlg::OnLbnDblclkListRdir)

	ON_BN_CLICKED(IDC_MFCBUTTON_THREAD, &CFTPClientDlg::OnBnClickedMfcbuttonThread)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MFCBUTTON_THREAD2, &CFTPClientDlg::OnBnClickedMfcbuttonThread2)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CFTPClientDlg::OnBnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CFTPClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CFTPClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFTPClientDlg 메시지 처리기

BOOL CFTPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//UpdateLocalListBox();
	////////////////////////////////////기존에 있는 것을 모두 지우고...
	m_strLocalDirText = _T("C://FTP_Client");
	UpdateData(FALSE);
	SetCurrentDirectory(m_strLocalDirText);
	isLogView = FALSE;

	/////////////////////////////////////////////////////////////////////////////////////////
	SetBackgroundColor(RGB(176, 196, 222));

	// listview init
	m_list_ctrl.InsertColumn(0, _T("Num"), LVCFMT_CENTER, 50);
	m_list_ctrl.InsertColumn(1, _T("LOCAL FOLDER"), LVCFMT_LEFT, 200);
	m_list_ctrl.InsertColumn(2, _T("TARGET FOLDER"), LVCFMT_LEFT, 200);
//	m_list_ctrl.InsertColumn(3, _T("BACKUP FOLDER"), LVCFMT_CENTER, 115);
	m_list_ctrl.InsertColumn(3, _T("DELAY TIME(msec)"), LVCFMT_CENTER, 115);
//	m_list_ctrl.InsertColumn(4, _T("USE"), LVCFMT_CENTER, 115);

	m_list_ctrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 

	m_ConditionIndex = 0;

	for(int i = 0; i<TIMER_CNT ; i++)
	{
		m_nUseTime[i] = 0;
	}


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
//	ReadFIleData();

	// 이미 연결된 것이 있으면 해제
	if (m_pConnection != NULL)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	// 연결하고자하는 FTP 서버의 IP 주소와 사용자명, 암호 입력
	CConnectDlg dlg;
	if (ReadFIleData())
	{
		dlg.m_strAddress = m_arrFileData[0];
		dlg.m_strName = m_arrFileData[1];
		dlg.m_strPassword = m_arrFileData[2];
		dlg.ChangeValue();
	}

	if (dlg.DoModal() != IDOK) return;

	
	BeginWaitCursor();

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
		EndWaitCursor();
		AfxMessageBox(_T("연결이 않됩니다."), MB_OK);
		m_pConnection = NULL;
		return;
	}

	// 연결정보 저장
	m_arrFileData[0] = dlg.m_strAddress;
	m_arrFileData[1] = dlg.m_strName;
	m_arrFileData[2] = dlg.m_strPassword;

	//WriteFIleData();
	// 연결정보 보이기
	m_strConInfo.Format(_T("Server IP : %s   / User Name : %s"), dlg.m_strAddress, dlg.m_strName);

	// 현재 디렉터리를 얻어 화면에 표시
	//m_pConnection->GetCurrentDirectory(m_strRemoteDirText);
	UpdateData(FALSE);

	if (m_pFileFind)
		delete m_pFileFind;

	// FTP 서버의 파일명을 얻어옴
	m_pFileFind = new CFtpFileFind(m_pConnection);

	// list data 있을때 settimer
	InputListData();

	EndWaitCursor();
}

void CFTPClientDlg::InputListData()
{
	CString strToken[3];
	CString strT;
	int curpos = 0;
	int i = 0;
	
	m_list_ctrl.DeleteAllItems();
	
	for (int z = 0; z < 5; z++)
	{
		if (m_nUseTime[z] == 1)
		{
			m_nUseTime[z] == 0;
			KillTimer(z);
			m_ctlList[z].ResetContent();
			m_ctlStr[z].SetWindowTextW(_T("------"));
		}
		curpos = 0;
		i = 0;
		if (m_arrFileList[z].GetLength() > 6)
		{
			strT = m_arrFileList[z].Tokenize(_T("-"), curpos);
			while (strT != _T(""))
			{
				strToken[i] = strT; i++;
				strT = m_arrFileList[z].Tokenize(_T("-"), curpos);
			}

			m_nUseTime[z] = 1;// 사용중임

			SettingTimer(z, strToken[0], strToken[1], _ttoi(strToken[2]));
			SetTimer(z, _ttoi(strToken[2]), NULL);
		}
		else
			break;
	}
	for (int j = 0; j < TIMER_CNT; j++)
	{
		m_arrFileList[j] = _T("");
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
void CFTPClientDlg::OnBnClickedMfcbuttonThread()
{
	// timer 5개면 return
	if (m_list_ctrl.GetItemCount() > 4)
	{
		AfxMessageBox(_T("갯수를 초과햇습니다."), MB_OK);
		return;
	}

	FolderSelectDlg dlg;
	if (dlg.DoModal() == IDOK)
	{

	//	m_list_ctrl.SetItemText(0, 4, _T("Use"));

		int i = 0;
		for ( ; i<TIMER_CNT; i++)
		{
			if (m_nUseTime[i] == 0)
			{
				m_nUseTime[i] = 1;// 사용중임

				SetTimer(i, dlg.nDelayNum, NULL); break;

				break;
			}
		}

		SettingTimer(i, dlg.m_strLocalFolder, dlg.m_strTargetFolder, dlg.nDelayNum);

	}
}

void CFTPClientDlg::SettingTimer(int nIndex, CString LoFolder, CString TaFolder,int nDelay )
{
	mCondition[nIndex].Initialize();
	mCondition[nIndex].IsUse = TRUE;
	mCondition[nIndex].strLocalFolder = LoFolder;

	mCondition[nIndex].strTargetFolder = TaFolder;

	mCondition[nIndex].nDelayTime = nDelay;

	CString temp, temp1;
	temp.Format(_T("%d"), nIndex + 1);
	temp1.Format(_T("%d"), mCondition[nIndex].nDelayTime);

	m_list_ctrl.InsertItem(0, temp);
	m_list_ctrl.SetItemText(0, 1, LoFolder);
	m_list_ctrl.SetItemText(0, 2, mCondition[nIndex].strTargetFolder);
	m_list_ctrl.SetItemText(0, 3, temp1);

	if (TaFolder == _T(""))
		m_ctlStr[nIndex].SetWindowTextW(LoFolder + _T("  =>  /"));
	else
		m_ctlStr[nIndex].SetWindowTextW(LoFolder + _T("  =>  ") + mCondition[nIndex].strTargetFolder);

	m_ctlStr[nIndex].RedrawWindow();

}

bool CFTPClientDlg::ReadFIleData()
{
	//data 초기화
	for (int j = 0 ; j < 3; j++)
	{
		m_arrFileData[j] = _T("");
	}
	for (int j = 0; j < TIMER_CNT; j++)
	{
		m_arrFileList[j] = _T("");
	}

	char path_name[MAX_PATH];
	char file_name[MAX_PATH];

	GetModuleFileNameA(NULL, path_name, MAX_PATH);

	CString StrPath;
	StrPath = path_name;
	StrPath = StrPath.Mid(0, StrPath.ReverseFind('\\'));

	sprintf_s(file_name, "%S\\Config.txt", StrPath);

	char szData[64] = { 0 };
	char* pszData = NULL;

	CString strKey;

	ifstream inFile;
	inFile.open(file_name, ifstream::in);

	//no file
	if (!inFile.is_open())
	{
		//AfxMessageBox(_T(""));
		return false;
	}

	while (!inFile.eof())
	{
		inFile.getline(szData, 100);
		pszData = strtok(szData, ":");

		if (pszData != NULL)
		{
			strKey = pszData;

			if (strKey.Compare(_T("IPADDRESS")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileData[0] = pszData;
			}
			else if (strKey.Compare(_T("USERNAME")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileData[1] = pszData;
			}
			else if (strKey.Compare(_T("PASSWORD")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileData[2] = pszData;
			}
			else if (strKey.Compare(_T("LIST1")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileList[0] = pszData;
			}
			else if (strKey.Compare(_T("LIST2")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileList[1] = pszData;
			}
			else if (strKey.Compare(_T("LIST3")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileList[2] = pszData;
			}
			else if (strKey.Compare(_T("LIST4")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileList[3] = pszData;
			}
			else if (strKey.Compare(_T("LIST5")) == 0)
			{
				pszData = strtok(NULL, ":");
				m_arrFileList[4] = pszData;
			}

		}

	}

	inFile.close();
	return true;
}

void CFTPClientDlg::WriteFIleData()
{
	char path_name[MAX_PATH];
	char file_name[MAX_PATH];

	GetModuleFileNameA(NULL, path_name, MAX_PATH);

	CString StrPath;
	StrPath = path_name;
	StrPath = StrPath.Mid(0, StrPath.ReverseFind('\\'));

	sprintf_s(file_name, "%S\\Config.txt", StrPath);

	ofstream outFile(file_name);

	if (m_arrFileData[0].GetLength() > 8)
	{
		outFile << "IPADDRESS:";
		outFile << (CStringA)m_arrFileData[0];
		outFile << endl;
	}
	if (m_arrFileData[1].GetLength() > 1)
	{
		outFile << "USERNAME:";
		outFile << (CStringA)m_arrFileData[1];
		outFile << endl;
	}
	if (m_arrFileData[2].GetLength() > 1)
	{
		outFile << "PASSWORD:";
		outFile << (CStringA)m_arrFileData[2];
		outFile << endl;
	}

	if (m_arrFileList[0].GetLength() > 6)
	{
		outFile << "LIST1:";
		outFile << (CStringA)m_arrFileList[0];
		outFile << endl;
	}
	if (m_arrFileList[1].GetLength() > 6)
	{
		outFile << "LIST2:";
		outFile << (CStringA)m_arrFileList[1];
		outFile << endl;
	}
	if (m_arrFileList[2].GetLength() > 6)
	{
		outFile << "LIST3:";
		outFile << (CStringA)m_arrFileList[2];
		outFile << endl;
	}
	if (m_arrFileList[3].GetLength() > 6)
	{
		outFile << "LIST4:";
		outFile << (CStringA)m_arrFileList[3];
		outFile << endl;
	}
	if (m_arrFileList[4].GetLength() > 6)
	{
		outFile << "LIST5:";
		outFile << (CStringA)m_arrFileList[4];
		outFile << endl;
	}

	outFile.close();
}

void CFTPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	BOOL isOK = FALSE;
	int i = nIDEvent;
	switch (nIDEvent)
	{
	case TIMER1:
	case TIMER2:
	case TIMER3:
	case TIMER4:
	case TIMER5:

		TimerFile[i] = FindNewFile(m_strLocalDirText + _T("/") + mCondition[i].strLocalFolder);
		if (TimerFile[i] == _T("1"))
		{
			if (isLogView) m_ctlList[i].AddString(_T("no path"));// no path
		}
		else if (TimerFile[i] == _T("2"))
		{
			if (isLogView) m_ctlList[i].AddString(_T("no csv file"));// no csv file
		}
		else // send file
		{
			TimerBackup[i] = TimerFile[i];
			TimerFile[i] = TimerFile[i].Mid(TimerFile[i].ReverseFind('\\') + 1);
			isOK = OnUpload(TimerFile[i], mCondition[i].strLocalFolder, mCondition[i].strTargetFolder);
			if (isOK)
			{
				m_ctlList[i].AddString(TimerFile[i] + _T("__OK"));
				doMoveFile(TimerBackup[i].Mid(0, TimerBackup[i].ReverseFind('\\')), TimerBackup[i]);
			}
			else
			{
				m_ctlList[i].AddString(_T("---fail"));
			}
		}

		int nItem = m_ctlList[i].GetCount();
		if (nItem > 10)
		{
			m_ctlList[i].ResetContent();
		}
		else
		{
			m_ctlList[i].SetCurSel(nItem - 1);
		}
		m_ctlList[i].RedrawWindow();

		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CFTPClientDlg::OnUpload(CString m_strLocalFile, CString m_strLocalDirText, CString m_strRemoteDirText)
{
	BOOL bisOK = FALSE;
	CString strFilename, strPathname;
	if (m_pConnection)
	{

		strFilename = m_strLocalFile.Mid(m_strLocalFile.ReverseFind('\\')+1);

		// 디렉터리명과 파일명을 붙이기	
		strPathname =_T("//") + m_strRemoteDirText + "//" + strFilename;
		strFilename = m_strLocalDirText + "//" + strFilename;

		// 파일 업로드
		bisOK = m_pConnection->PutFile(strFilename, strPathname);
	}
	return bisOK;
}


void CFTPClientDlg::OnBnClickedMfcbuttonThread2()
{
	POSITION pos = m_list_ctrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_list_ctrl.GetNextSelectedItem(pos);
		//int a = 2;
		CString strTemp = m_list_ctrl.GetItemText(nItem, 0) + _T(" Del?");
		
		if (AfxMessageBox(strTemp, MB_OKCANCEL) == IDOK)
		{
			int nGetTimer = _ttoi(strTemp)-1;
			KillTimer(nGetTimer);
			m_list_ctrl.DeleteItem(nItem);
			m_nUseTime[nGetTimer] = 0;

			m_ctlList[nGetTimer].ResetContent();
			m_ctlStr[nGetTimer].SetWindowTextW(_T("------"));
		}
	}

}

//  오래된파일찾기  20180126
CString CFTPClientDlg::FindNewFile(CString strPath)
{
	int n_fileCount = 0;
	CString strNewFile = _T("");
	CString strFileName = _T("");
	CString strFileCnt = _T("_____________");

	if (GetFileAttributes(strPath) == -1)
	{
		return _T("1");
	}

	ATL::CTime tCreateTimefirst = NULL;
	ATL::CTime tCreateTimesecond = NULL;

	CFileFind ff;
	BOOL bRval = FALSE;

	bRval = ff.FindFile(strPath + _T("\\*.csv"));
	if (bRval == false)
	{
		return _T("2");// strNewFile;
	}

	BOOL is_notmovefile = FALSE;

	while (bRval)
	{
		bRval = ff.FindNextFile();

		if (ff.IsDots()) continue;

		is_notmovefile = FALSE;
		// file
		if (ff.IsDirectory() == FALSE)
		{
			strFileName = ff.GetFilePath();
			/*
			// 이동 안된 파일이면 이동시도하기
			INT_PTR n_cnt = m_NotMovefile[n_weld].GetCount();
			if (n_cnt > 0)
			{
				for (int k = 0; k < n_cnt; k++)
				{
					if (m_NotMovefile[n_weld].GetAt(k) == strFileName)
					{
						is_notmovefile = TRUE;
						if (doMoveFile(strPath, strFileName))
						{
							m_NotMovefile[n_weld].RemoveAt(k);
							//							CVisionDegassingManager::OutputMessage(mv::CAM_ALIGN, "%d_again_%d_%S",n_weld,n_cnt-1,strFileName.Right(30));
							//							n_cnt = m_NotMovefile[n_weld].GetCount();
						}
						break;
					}
				}
				if (is_notmovefile)
				{
					continue;
				}
			}
			*/

			ff.GetCreationTime(tCreateTimefirst);
			n_fileCount++;

			// 파일이름 기록해서 2개이상일때 로그로 남기기
			//strFileCnt = strFileCnt + ff.GetFileName() + _T("/");

			if (tCreateTimesecond != NULL)
			{
				//			if (tCreateTimefirst > tCreateTimesecond )
				if (tCreateTimefirst < tCreateTimesecond)
				{
					strNewFile = strFileName;
					tCreateTimesecond = tCreateTimefirst;
				}
			}
			else
			{
				strNewFile = strFileName;
				tCreateTimesecond = tCreateTimefirst;
			}
		}
	}

	return  strNewFile;
}

// 20180126_ 읽은 파일만 이동
BOOL CFTPClientDlg::doMoveFile(CString strPath, CString strFileName)
{

	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);

	char path_name[MAX_PATH];

	sprintf_s(path_name, "%S\\Backup\\", strPath);
	_mkdir(path_name);
	sprintf_s(path_name, "%S\\Backup\\%04d%02d\\", strPath, sys_time.wYear, sys_time.wMonth);
	_mkdir(path_name);

	CString strFolder;// = path_name;

	strFolder = strFileName.Mid(strFileName.ReverseFind(_T('\\')) + 1);

	strFolder.Format(_T("%S%s"), path_name, strFolder);
	
	return MoveFileEx(strFileName, strFolder, MOVEFILE_REPLACE_EXISTING);
}

void CFTPClientDlg::OnBnClickedMfcbutton2()
{
		CString strDefPath;
		strDefPath = GetSelectFolderPath();
		if (strDefPath.IsEmpty())
		{
			AfxMessageBox(_T(" 폴더가 선택되지 않았습니다."));
			return;
		}

		m_strLocalDirText = strDefPath;// _T("C://FTP_Client");
		UpdateData(FALSE);
		SetCurrentDirectory(m_strLocalDirText);
}

CString CFTPClientDlg::GetSelectFolderPath()
{
	CString strFolderPath;

	ITEMIDLIST *pidlBrowse;
	TCHAR       pszPathname[MAX_PATH] = _T("");
	//pszPathname = _T("");
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


// file 없음을 나타내기
void CFTPClientDlg::OnBnClickedButton1()
{
	//InputListData();
	isLogView = !isLogView;
}


void CFTPClientDlg::OnBnClickedOk()
{
	int nCnt = m_list_ctrl.GetItemCount();

	if (nCnt > 0) 
	{
		int ntemp = nCnt - 1;
		for (int i = 0; i < nCnt; i++)
		{
			m_arrFileList[i].Format(_T("%s-%s-%d"), m_list_ctrl.GetItemText(ntemp, 1), m_list_ctrl.GetItemText(ntemp, 2), _ttoi(m_list_ctrl.GetItemText(ntemp, 3)));
			ntemp--;
		}
	}

	WriteFIleData();
	CDialogEx::OnOK();
}
