// ReSimDataDownload.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP3SIMPORT.h"
#include "MFCP3SIMPORTDlg.h"
#include "ReSimDataDownload.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "ThreadPool.h"
#include "Task.h"
#include "ADOManage.h"
#include "Log.h"

// CReSimDataDownload �Ի���
#define WM_MainRePortThreadControl WM_USER+1310

CString strReFolderpath, strReOKFolderpath, strReFolderFile, m_resimdatafolderPath;//���ļ���·��
HWND MainReFormHWND;//�����߳̾��
CString reComNo;
HANDLE reporthandler;
int simrestratflag = 0;
CString ReLogName;
BOOL RePortAbnomal = FALSE;
//CString LastReRID=L"";
//CString LastReIMEI=L"";

volatile BOOL m_RePortDownloadWrite;
volatile BOOL m_RePortDownloadRead;
volatile BOOL m_RePortDownloadMain;

volatile BOOL m_RePortDownloadWrite1;
volatile BOOL m_RePortDownloadWrite2;
volatile BOOL m_RePortDownloadRead1;
volatile BOOL m_RePortDownloadRead2;
volatile BOOL m_RePortDownloadReadEnd2;


IMPLEMENT_DYNAMIC(CReSimDataDownload, CDialogEx)

CReSimDataDownload::CReSimDataDownload(CWnd* pParent /*=NULL*/)
   : CDialogEx(CReSimDataDownload::IDD, pParent)
   , simreconnectflag(0)
{

}

CReSimDataDownload::~CReSimDataDownload()
{
}

void CReSimDataDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORTLIST1_COMBO, m_report1list1Combo);
	DDX_Control(pDX, IDC_RECURRENTINFORMATION_RICHEDIT, m_recurrentinformationRichedit);
}


BEGIN_MESSAGE_MAP(CReSimDataDownload, CDialogEx)
	ON_BN_CLICKED(IDC_REOPENSIMDATAFOLDERPATH_BUTTON, &CReSimDataDownload::OnBnClickedReopensimdatafolderpathButton)
	ON_BN_CLICKED(IDC_REPORT1CONNECT_BUTTON, &CReSimDataDownload::OnBnClickedReport1connectButton)
	ON_BN_CLICKED(IDC_RESTART_BUTTON, &CReSimDataDownload::OnBnClickedRestart1Button)
	ON_BN_CLICKED(IDCANCEL, &CReSimDataDownload::OnBnClickedCancel)
	//ON_MESSAGE(WM_MainRePortThreadControl, &CReSimDataDownload::MainRePortThreadControl)
	ON_CBN_DROPDOWN(IDC_REPORTLIST1_COMBO, &CReSimDataDownload::OnCbnDropdownReportlist1Combo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CReSimDataDownload ��Ϣ�������
BOOL CReSimDataDownload::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�������б�
	parentdlg->FindCommPort((CComboBox*)GetDlgItem(IDC_REPORTLIST1_COMBO), reComNo, 1);

	//��ini�����ļ���Ϣ�Ķ���������
	CString strpath;
	GetPrivateProfileString(_T("SimPathName"), _T("resimfolderpathname"), _T(""), strpath.GetBuffer(100), 100, _T(".\\SystemInfo.ini"));
	SetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, strpath);
	strpath.ReleaseBuffer();
	
	//��ʼ������
	fontinit();

	GetDlgItem(IDC_RESTART_BUTTON)->EnableWindow(FALSE);

	//��ȡ��־����
	ReLogName = GetLogTime()+L"ReLog";

	//LastReRID = L"";
	//LastReIMEI = L"";

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//����Ϊ��ʼ��ģ�麯��

//�������ļ��а�ť
void CReSimDataDownload::OnBnClickedReopensimdatafolderpathButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���ļ���
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = LPWSTR(name);
	bi.lpszTitle = L"ѡ���ļ���Ŀ¼";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return;
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if (strDirectoryPath.IsEmpty())
		return;
	if (strDirectoryPath.Right(1) != "\\")
		strDirectoryPath += "\\";

	//��ѡ���·����ʾ��SIM�����ݿ�·������
	SetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, strDirectoryPath);
	//��·����Ϣ���浽��Ϣ�ļ���
	WritePrivateProfileString(_T("SimPathName"), _T("resimfolderpathname"), strDirectoryPath, _T(".\\SystemInfo.ini"));
}

//������Ӱ�ť
void CReSimDataDownload::OnBnClickedReport1connectButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (simreconnectflag == 0)
	{
	//�Ȼ�ȡ��ǰѡ��Ĵ��ں�
	parentdlg->GetCommPort((CComboBox*)GetDlgItem(IDC_REPORTLIST1_COMBO), reComNo);

	//Ȼ���ٳ�ʼ�����ں�
	GetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, m_resimdatafolderPath);
	if (m_resimdatafolderPath == L""||m_resimdatafolderPath.Find(L"OK")==-1)
	{
		MessageBox(L"��ѡ������ɵ�SIM������·�����������������ع���sim�������ļ��У����ļ��У�", L"��ʾ��Ϣ", NULL);
		return;
	}

	reporthandler = parentdlg->InitCom(reComNo);

	if (reporthandler == NULL)
	{
		PrintReLog(L"���ڳ�ʼ��ʧ��");
		MessageBox(L"���ڳ�ʼ��ʧ��",L"��ʾ��Ϣ",NULL);
		return;
	}

	GetDlgItem(IDC_RESTART_BUTTON)->EnableWindow(TRUE);
	simreconnectflag = 1;
	SetDlgItemText(IDC_REPORT1CONNECT_BUTTON, L"�Ͽ�");
	GetDlgItem(IDC_REOPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(FALSE);
	OnBnClickedRestart1Button();
	GetDlgItem(IDC_REPORTLIST1_COMBO)->EnableWindow(FALSE);
	}
	else if (simreconnectflag == 1)
	{
		if (simrestratflag == 1)
		{
			OnBnClickedRestart1Button();
		}
		if (!parentdlg->CloseCom(reporthandler))
		{
			PrintReLog(L"�رմ���ʧ��");
			MessageBox(L"�رմ���ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}

		reporthandler = NULL;
		PrintReLog(L"�رմ��ڳɹ�");
		//SetRicheditText(L"�رմ��ڳɹ�", 0);
		GetDlgItem(IDC_RESTART_BUTTON)->EnableWindow(FALSE);
		simreconnectflag = 0;
		SetDlgItemText(IDC_REPORT1CONNECT_BUTTON, L"����");
		GetDlgItem(IDC_REOPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_REPORTLIST1_COMBO)->EnableWindow(TRUE);
	}

	return;
}

//�����ʼ������ť
void CReSimDataDownload::OnBnClickedRestart1Button()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (simrestratflag == 0)
	{
		GetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, strReOKFolderpath);

		if (!PathIsDirectory(strReOKFolderpath))
		{
			MessageBox(L"����ѡ����ļ��в����ڣ�������ѡ��", L"��ʾ��Ϣ", NULL);
			return;
		}

		simrestratflag = 1;
		SetDlgItemText(IDC_RESTART_BUTTON, L"ֹͣ����");
		SetDlgItemText(IDC_REPORT1HINT_STATIC, L"����");
		//m_Port2DownloadControl = TRUE;
		s_bReExit = FALSE;
		MainThread = AfxBeginThread(ReDownloadMainThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		//OpenThreadPoolTask(PORT_REAUTO_THREAD);
	}
	else if (simrestratflag == 1)
	{
		s_bReExit = TRUE;
		//m_Port2DownloadControl = FALSE;
		simrestratflag = 0;
		SetDlgItemText(IDC_RESTART_BUTTON, L"��ʼ����");
		SetDlgItemText(IDC_REPORT1HINT_STATIC, L"ֹͣ");
		SetRePortEditEmpty();
		//StrFolder[1] = L"";
	}
}

//�������1������
void CReSimDataDownload::OnCbnDropdownReportlist1Combo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	parentdlg->FindCommPort((CComboBox*)GetDlgItem(IDC_REPORTLIST1_COMBO), reComNo, 1);
}

//���IMEI��RID�༭��
void CReSimDataDownload::SetRePortEditEmpty()
{
	SetDlgItemText(IDC_REPORT1RID_EDIT, L"");
	SetDlgItemText(IDC_REPORT1IMEI_EDIT, L"");
}


//����Ϊ����λģ�麯��

//�ļ�����д�����,0��TESTָ��,1��RIDָ��,2��IMEIָ��,3�ǿ�ʼ����ָ��,4�����ؽ���ָ��
CString CReSimDataDownload::CommandWriteUnit(int strcommandNo)
{
	CString strCommandWrite[7] = {
		L"AT^GT_CM=TEST\r\n", //���������������ֱ���þ���
		L"AT^GT_CM=ID,1\r\n", //��RID��һ������
		L"AT^GT_CM=IMEI\r\n",
		L"AT^GT_CM=SOFTSIM,CLEAN\r\n",
		L"AT+AGENT=4,FFFFFFFFFFFFFFFFFFFF\r\n",
		L"AT^GT_CM=aging,off\r\n",
		L"AT^GT_CM=reset,1\r\n",
	};//��IMEI��һ������
	return strCommandWrite[strcommandNo];
}

//�ļ�����д�����Ӧ���ַ�������,0��TESTָ��,1��RIDָ��,2��IMEIָ��,3�ǿ�ʼ����ָ��,4�����ؽ���ָ��
CString CReSimDataDownload::CommandReadUnit(int strcommandNo)
{
	switch (strcommandNo)
	{
	case 0:
		return L"TEST_OK";
	case 1:
		return L"Chip RID:";
	case 2:
		return L"IMEI:";
	case 3:
		return L"\r\nOK!";
	case 4:
		return L"\r\nOK";
	case 5:
		return L"\r\nOK";
	default:
		break;
	}
	return L"";
}

//����λ�����߳�
UINT ReDownloadMainThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;

	strReFolderpath = strOKFolderpath.Left(strOKFolderpath.GetLength() - 3) + L"\\";
	if (!PathIsDirectory(strReFolderpath))
	{
		::CreateDirectory(strReFolderpath, NULL);//����Ŀ¼,���еĻ���Ӱ��
	}

	//dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"����");

	dlg->DWThread = AfxBeginThread(ReDownloadWirtePortThread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);//����дtest�߳�
	m_RePortDownloadMain = TRUE;
	//���û�б�ֹͣ���Ǿ�һֱȥ����test�߳�
	while (m_RePortDownloadMain)
	{
		if (dlg->DWThread == NULL)
			dlg->DWThread = AfxBeginThread(ReDownloadWirtePortThread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		Sleep(4000);
	}

	//while (!s_bReExit)
	//{
	//	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Test, NULL);
	//	Sleep(8000);
	//}
	dlg->MainThread = NULL;
	return 0;
}

//����λ����д�߳�
UINT ReDownloadWirtePortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;

	//dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"����");

	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//���ڱ���
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//��ָ���ñ���
	CString strcommand;


	//һֱtest������������û��������
PortTest:
	dlg->SetRePortEditEmpty();
	m_RePortDownloadWrite = TRUE;
	dlg->CommandNo = 0;
	strcommand = dlg->CommandWriteUnit(dlg->CommandNo);
	ClearCommError(reporthandler, &dwErrorFlags, &ComStat);

	//ͬʱ�������߳�
	dlg->DWThread = AfxBeginThread(ReDownloadReadPortThread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	if (RePortAbnomal == TRUE)//��������������쳣��Σ�����ͣ�ٸ�2.5��ż���������
	{
		Sleep(2000);
	}

	dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"�ȴ�����");
	do
	{
		dlg->PrintReLog(L"��:" + strcommand);
		//dlg->SetRicheditText(L"��:" + strcommand, 0);
		bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			//dlg->ReDownloadClosePortThread();
			return 0;
		}
		Sleep(2000);
	} while (m_RePortDownloadWrite);

	if (m_RePortDownloadMain == FALSE)
	{
		dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"ֹͣ");
		return 0;
	}

	dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"������");

	RePortAbnomal = FALSE;

	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//�������˾Ϳ�ʼ������RID��IMEI��������������������û��Ӧ���Ǿͷ���Test�������¼���豸
	for (int i = 1; i < 6; i++)
	{
		int count = 0;
		dlg->CommandNo = i;
		strcommand = dlg->CommandWriteUnit(dlg->CommandNo);
		ClearCommError(reporthandler, &dwErrorFlags, &ComStat);

		//Ȼ��ͬʱ�������߳�
		m_RePortDownloadWrite = TRUE;
		dlg->DWThread = AfxBeginThread(ReDownloadReadPortThread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		do
		{

			dlg->PrintReLog(L"��:" + strcommand);
			//dlg->SetRicheditText(L"��:" + strcommand, 0);
			bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 3)
			{
				//dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"ʧ�ܴ�����");

				//һֱ���뷢��testָ������ⲻ�����Ǵ������Ѿ��Ͽ���
				BOOL reporttestflag = TRUE, reportfailflag = TRUE;
				int reporttestcount = 0;
				char reportteststr[100];
				memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
				DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
				BOOL reporttestbReadStat, reporttestbWriteStat;
				CString reportteststrread, reportteststrcommand = L"AT^GT_CM=TEST\r\n";;
				COMSTAT reporttestComStat;

				ClearCommError(reporthandler, &reporttestdwErrorFlags, &reporttestComStat);

				do
				{
					reporttestbWriteStat = WriteFile(reporthandler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

					Sleep(90);
					reporttestbReadStat = ReadFile(reporthandler, reportteststr, 100, &reporttestreadreal, 0);
					if (reporttestbReadStat)
					{
						reportteststrread = reportteststr;
						if (reportteststrread.Find(L"TEST_OK") >= 0)
						{
							if (reportfailflag == TRUE)
							{
								dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"ʧ�ܴ�����");
								reportfailflag = FALSE;
							}
							reporttestcount = 0;
						}
						else
						{
							if (reportfailflag == TRUE)
							{
								dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"�쳣���");
								reportfailflag = FALSE;
								RePortAbnomal = TRUE;
							}
						}
					}
					reporttestcount++;
					if (reporttestcount == 5)
					{
						reporttestflag = FALSE;
					}
					Sleep(10);
					memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
					dlg->PrintReLog(L"��:" + reportteststrcommand);
					PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (reporttestflag);

				Sleep(100);
				m_RePortDownloadWrite = FALSE;
				m_RePortDownloadRead = FALSE;
				goto PortTest;
			}
			count++;
			Sleep(1500);
		} while (m_RePortDownloadWrite);
	}

	////������ָ��
	//strcommand = dlg->CommandWriteUnit(6);
	//ClearCommError(reporthandler, &dwErrorFlags, &ComStat);
	//bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintReLog(L"��:" + strcommand);

	if (m_RePortDownloadMain == FALSE)
	{
		dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"ֹͣ");
		return 0;
	}
	Sleep(1500);

	//һֱ���뷢��testָ������ⲻ�����Ǵ������Ѿ��Ͽ���
	BOOL reporttestflag = TRUE;
	int reporttestcount = 0;
	char reportteststr[100];
	memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
	DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
	BOOL reporttestbReadStat, reporttestbWriteStat;
	CString reportteststrread, reportteststrcommand = L"AT^GT_CM=TEST\r\n";
	COMSTAT reporttestComStat;

	ClearCommError(reporthandler, &reporttestdwErrorFlags, &reporttestComStat);
	do
	{
		reporttestbWriteStat = WriteFile(reporthandler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

		Sleep(90);
		reporttestbReadStat = ReadFile(reporthandler, reportteststr, 100, &reporttestreadreal, 0);
		if (reporttestbReadStat)
		{
			reportteststrread = reportteststr;
			if (reportteststrread.Find(L"TEST_OK") >= 0)
			{
				reporttestcount = 0;
			}
		}
		reporttestcount++;
		if (reporttestcount == 5)
		{
			reporttestflag = FALSE;
		}
		Sleep(10);
		memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
		dlg->PrintReLog(L"��:" + reportteststrcommand);
		PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	} while (reporttestflag);

	dlg->DWThread = NULL;
	return 0;
}

//����λ���ڶ��߳�
UINT ReDownloadReadPortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;

	//���ڱ���
	char str[200];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//�������
	CString strread;
	CString strcount, strcounttemp;//��RID��IMEI��
	CString strtemp;
	int findcount1, findcount2,Reflag;//�и���������ַ�����

	m_RePortDownloadRead = TRUE;;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�

	do
	{
		Sleep(300);
		bReadStat = ReadFile(reporthandler, str, 199, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			strtemp = dlg->CommandReadUnit(dlg->CommandNo);
			if ((findcount1 = strread.Find(strtemp)) >= 0)
			{
				//��RID
				if (dlg->CommandNo == 1)
				{
					strcount = strread.Right(strread.GetLength() - findcount1 - 9);
					findcount2 = strcount.Find(L"\r\n");
					strcount = strcount.Left(findcount2);
					if (strcount != "")
					{
						strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
						dlg->SetDlgItemText(IDC_REPORT1RID_EDIT, strcount);
					}
					else if (strcount == "")
					{
						continue;
					}
				}
				//��IMEI
				else if (dlg->CommandNo == 2)
				{
					strcount = strread.Right(strread.GetLength() - findcount1 - 5);
					findcount2 = strcount.Find(L"\r\n");
					strcount = strcount.Left(findcount2);
					strcounttemp = strcount;
					if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
					{
						dlg->SetDlgItemText(IDC_REPORT1IMEI_EDIT, strcount);
						Sleep(50);
					}

					else
					{
						continue;
					}
				}
				else if (dlg->CommandNo == 3)
				{
					CString strReIMEI1, strReRID1;

					dlg->GetDlgItemText(IDC_REPORT1RID_EDIT, strReRID1);
					dlg->GetDlgItemText(IDC_REPORT1IMEI_EDIT, strReIMEI1);
					//if (LastReRID != strReRID1&&LastReIMEI != strReIMEI1)
					//{
					//	dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"���������");
					//}
					dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"���������");
				}
				else if (dlg->CommandNo == 5)
				{
					CString strReIMEI, strReRID;

					dlg->GetDlgItemText(IDC_REPORT1RID_EDIT, strReRID);
					dlg->GetDlgItemText(IDC_REPORT1IMEI_EDIT, strReIMEI);


					ADOManage ReAdomanage;
					ReAdomanage.ConnSQL();
					Reflag = ReAdomanage.SimDataReSql(strReRID, strReIMEI, strReOKFolderpath);//���ݷ���ֵȷ���Ƿ�ɹ�
					if (Reflag == 1)
					{
						dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"�����ɹ�");
						dlg->PrintReLog(L"�����ɹ�");
					}
					else if (Reflag == 2 || Reflag == 3)
					{
						//if (LastReRID != strReRID&&LastReIMEI != strReIMEI)
						//{
						//	dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"���践��");
						//}
						dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"���践��");
						dlg->PrintReLog(L"���践��");
					}
					else if (Reflag == 0)
					{
						dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"�޴˻���¼");
						dlg->PrintReLog(L"�޴˻���¼");
					}
					else if (Reflag == 4)
					{
						dlg->SetDlgItemText(IDC_REPORT1HINT_STATIC, L"����·������");
						dlg->PrintReLog(L"����·������");
					}

					//LastReRID = strReRID;
					//LastReIMEI = strReIMEI;

					ReAdomanage.CloseAll();
				}
				m_RePortDownloadRead = FALSE;
				m_RePortDownloadWrite = FALSE;
			}
		}
	} while (m_RePortDownloadRead);
	//dlg->SetRicheditText(L"��:" + strread, 0);
	dlg->PrintReLog(L"��:" + strread);

	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	dlg->DRThread = NULL;
	return 0;
}

//����λ���������߳�ȫ�ֱ���
void CReSimDataDownload::ReDownloadRestPortThread()
{
	m_RePortDownloadWrite1 = TRUE;
	m_RePortDownloadWrite2 = TRUE;
	m_RePortDownloadRead1 = TRUE;
	m_RePortDownloadRead2 = TRUE;
	m_RePortDownloadReadEnd2 = TRUE;

}

////����λ���ڹر��߳�ȫ�ֱ���
//void CReSimDataDownload::ReDownloadClosePortThread()
//{
//	m_TreadRePortDownloadWrite1 = FALSE;
//	m_TreadRePortDownloadWrite2 = FALSE;
//	m_TreadRePortDownloadRead1 = FALSE;
//	m_TreadRePortDownloadRead2 = FALSE;
//}


//��������

//��Ϣ��־����
void CReSimDataDownload::SetRicheditText(CString strMsg, int No)
{
	m_recurrentinformationRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

	switch (No)
	{
	case 0:
		m_recurrentinformationRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_recurrentinformationRichedit.LineScroll(m_recurrentinformationRichedit.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_recurrentinformationRichedit.SetSelectionCharFormat(cf);
		m_recurrentinformationRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_recurrentinformationRichedit.LineScroll(m_recurrentinformationRichedit.GetLineCount() - 1);
		break;
	default:
		break;
	}

	/*
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	if (strMsg == "127.0.0.1>>������Ϣ")
	{
	cf.crTextColor = RGB(255, 0, 0);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf); //�����richedit��ר�÷�����richedit��������Ϊ��־���
	m_currenttcp1RichEdit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("����\r\n"));
	}
	cf.crTextColor = RGB(255, 200, 100);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf);*/
}

//��ȡϵͳʱ��
CString CReSimDataDownload::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//��ȡ��־ʱ��
CString CReSimDataDownload::GetLogTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}

//��־��ӡ����
void CReSimDataDownload::PrintReLog(CString strMsg)
{
	CLog Relog;//��־�ļ�
	Relog.WriteLog(CStringA(L"��"+GetTime() + L"��") + CStringA(strMsg), ReLogName);
}

//��ʼ������
void CReSimDataDownload::fontinit()
{
	staticReHintfont.CreatePointFont(900, L"����");

	GetDlgItem(IDC_REPORT1HINT_STATIC)->SetFont(&staticReHintfont);
}

//�ı�������ɫ����Ϣ
HBRUSH CReSimDataDownload::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	CString str1;
	if (pWnd->GetDlgCtrlID() == IDC_REPORT1HINT_STATIC)
	{
		GetDlgItemText(IDC_REPORT1HINT_STATIC, str1);
		if (str1 == "ʧ�ܴ�����" || str1 == "�޴˻���¼" || str1 == "����·������" || str1 == "���践��"||str1=="�쳣���")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticReHintfont);
		}
		else if (str1 == "�����ɹ�")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticReHintfont);
		}
		else if (str1 == "�ȴ�����")
		{
			pDC->SetTextColor(RGB(65, 105, 225));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticReHintfont);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticReHintfont);
		}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//����˳�����ģʽ
void CReSimDataDownload::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;
	nRes = MessageBox(_T("ȷ��Ҫ�˳���"), _T("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDOK == nRes)
	{
		if (simreconnectflag == 1)
		{
			OnBnClickedReport1connectButton();
		}
		CDialogEx::OnCancel();
	}
}




//��������ʱ�������Ĵ���

////�̳߳ص���Ϣѭ��
//afx_msg LRESULT CReSimDataDownload::MainRePortThreadControl(WPARAM wParam, LPARAM lParam)
//{
//	switch (wParam)
//	{
//		//�����ǵ��ļ����ص�
//		//һ��ʼ���Ƿ���test���test�����д�Ͷ�ͬʱ��
//	case MainPort_RePort_Test:
//		//OpenThreadPoolTask(REPORT_WRITE1_THREAD);
//		//OpenThreadPoolTask(REPORT_READ1_THREAD);
//		break;
//		//Ȼ��ʼ����RID��IMEI���д�Ͷ��ֱ�
//	case MainPort_RePort_Write2:
//		//OpenThreadPoolTask(REPORT_WRITE2_THREAD);
//		break;
//	case MainPort_RePort_Read2:
//		//OpenThreadPoolTask(REPORT_READ2_THREAD);
//		break;
//	default:
//		break;
//	}
//	return 0;
//}
//
////����λtestָ���д�߳�
//void CReSimDataDownload::ReDownloadWrite1PortThread(LPVOID lpParam)
//{
//	CReSimDataDownload* dlg;
//	dlg = (CReSimDataDownload*)lpParam;
//	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
//
//	//���ڱ���
//	DWORD dwBytesWrite;
//	COMSTAT ComStat;
//	DWORD dwErrorFlags;
//	BOOL bWriteStat;
//
//	//��ָ���ñ���
//	CString strcommand = L"AT^GT_CM=TEST\r\n";
//
//	//һֱtest������������û��������
//	dlg->SetRePortEditEmpty();
//	dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
//	m_RePortDownloadWrite1 = TRUE;
//
//	ClearCommError(reporthandler, &dwErrorFlags, &ComStat);
//
//	Sleep(50);
//	do
//	{
//		dlg->SetRicheditText(L"��:" + strcommand, 0);
//		bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
//		Sleep(1500);
//
//		if (s_bReExit == TRUE)
//		{
//			dlg->ReDownloadRestPortThread();
//			dlg->ReDownloadClosePortThread();
//			return;
//		}
//	} while (m_RePortDownloadWrite1);
//	return;
//}
//
////����λtestָ��Ķ��߳�
//void CReSimDataDownload::ReDownloadWrite2PortThread(LPVOID lpParam)
//{
//	CReSimDataDownload* dlg;
//	dlg = (CReSimDataDownload*)lpParam;
//
//	//���ڱ���
//	char str[100];
//	memset(str, 0, sizeof(str) / sizeof(str[0]));;
//	DWORD readreal = 0;
//	BOOL bReadStat;
//
//	//�������
//	CString strread;
//
//	m_RePortDownloadRead1 = TRUE;;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�
//
//	do
//	{
//		Sleep(1520);
//		bReadStat = ReadFile(reporthandler, str, 100, &readreal, 0);
//		if (bReadStat)
//		{
//			strread = str;
//			if (strread.Find(L"TEST_OK") >= 0)
//			{
//				m_RePortDownloadRead1 = FALSE;
//				m_RePortDownloadWrite1 = FALSE;
//			}
//		}
//		if (s_bReExit == TRUE)
//		{
//			dlg->ReDownloadRestPortThread();
//			dlg->ReDownloadClosePortThread();
//			return;
//		}
//	} while (m_RePortDownloadRead1);
//	dlg->SetRicheditText(L"��:" + strread, 0);
//	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
//
//	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Write2, NULL);
//	return;
//}
//
////����λ��RID��IMEIָ���д�߳�
//void CReSimDataDownload::ReDownloadRead1PortThread(LPVOID lpParam)
//{
//	CReSimDataDownload* dlg;
//	dlg = (CReSimDataDownload*)lpParam;
//	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, Main_Hint1_Connected, NULL);
//	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
//
//	//���ڱ���
//	DWORD dwBytesWrite;
//	COMSTAT ComStat;
//	DWORD dwErrorFlags;
//	BOOL bWriteStat;
//	DWORD dwTotalSize;//�ļ��ܴ�С
//
//	//��ָ���ñ���
//	CString strcommand;
//	CString strfilewritecommand;
//
//
//	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Read2, NULL);
//
//	Sleep(100);
//	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
//	//�������˾Ϳ�ʼ������RID��IMEI��������������������û��Ӧ���Ǿͷ���Test�������¼���豸
//	for (int i = 0; i < 2; i++)
//	{
//		int count = 0;
//		if (i == 0)
//		{
//			strcommand = L"AT^GT_CM=ID,1\r\n";//��RID��һ������
//		}
//		else if (i == 1)
//		{
//			strcommand = L"AT^GT_CM=IMEI\r\n";//��IMEI��һ������
//		}
//
//		ClearCommError(reporthandler, &dwErrorFlags, &ComStat);
//
//		//Ȼ��ͬʱ�������߳�
//		m_RePortDownloadWrite2 = TRUE;
//
//		PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
//		do
//		{
//
//			dlg->SetRicheditText(L"��:" + strcommand, 0);
//			bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
//
//			if (count == 7)
//			{
//				//::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, Main_Hint1_Fail, NULL);
//				dlg->ReDownloadRestPortThread();
//				dlg->ReDownloadClosePortThread();
//				return;
//			}
//			count++;
//			Sleep(600);
//			if (s_bReExit == TRUE)
//			{
//				dlg->ReDownloadRestPortThread();
//				dlg->ReDownloadClosePortThread();
//				return;
//			}
//		} while (m_RePortDownloadWrite2);
//	}
//	return;
//}
//
////����λ��RID��IMEIָ��Ķ��߳�
//void CReSimDataDownload::ReDownloadRead2PortThread(LPVOID lpParam)
//{
//	CReSimDataDownload* dlg;
//	dlg = (CReSimDataDownload*)lpParam;
//	//���ڱ���
//	char str[100];
//	memset(str, 0, sizeof(str) / sizeof(str[0]));;
//	DWORD readreal = 0;
//	BOOL bReadStat;
//
//	//�������
//	CString strread;
//	CString strcount, strcounttemp;//��RID��IMEI��
//	CString strtemp;
//	int findcount1, findcount2;//�и���������ַ�����
//
//	for (int i = 0; i < 2; i++)
//	{
//		m_RePortDownloadRead2 = TRUE;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�
//		m_RePortDownloadReadEnd2 = TRUE;
//		int count = 0;
//		if (i == 0)
//		{
//			strtemp = L"Chip RID:";//��RID��һ������
//		}
//		else if (i == 1)
//		{
//			strtemp = L"IMEI:";//��IMEI��һ������
//		}
//		do
//		{
//			Sleep(620);
//			bReadStat = ReadFile(reporthandler, str, 100, &readreal, 0);
//			if (bReadStat)
//			{
//				strread = str;
//				if ((findcount1 = strread.Find(strtemp)) >= 0)
//				{
//					//��RID
//					if (i == 0)
//					{
//						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
//						findcount2 = strcount.Find(L"\r\n");
//						strcount = strcount.Left(findcount2);
//						if (strcount != "")
//						{
//							dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);
//						}
//						else if (strcount == "")
//						{
//							continue;
//						}
//					}
//					//��IMEI
//					else if (i == 1)
//					{
//						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
//						findcount2 = strcount.Find(L"\r\n");
//						strcount = strcount.Left(findcount2);
//						strcounttemp = strcount;
//						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
//						{
//							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);
//							//::PostMessage(MainReFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_IsExit, NULL);
//							Sleep(50);
//						}
//						else
//						{
//							continue;
//						}
//					}
//					m_RePortDownloadWrite2 = FALSE;
//					m_RePortDownloadRead2 = FALSE;
//				}
//			}
//		} while (m_RePortDownloadRead2);
//		if (m_RePortDownloadReadEnd2 == FALSE)
//		{
//			return;
//		}
//		if (s_bReExit == TRUE)
//		{
//			dlg->ReDownloadRestPortThread();
//			dlg->ReDownloadClosePortThread();
//			return;
//		}
//		dlg->SetRicheditText(L"��:" + strread, 0);
//	}
//	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
//
//	return;
//}


