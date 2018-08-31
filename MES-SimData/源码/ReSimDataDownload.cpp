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

// CReSimDataDownload �Ի���
#define WM_MainRePortThreadControl WM_USER+1310

CString strReFolderpath, strReOKFolderpath, strReFolderFile, m_resimdatafolderPath;//���ļ���·��
HWND MainReFormHWND;//�����߳̾��
CString reComNo;
HANDLE reporthandler;
int simrestratflag = 0;

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
	ON_MESSAGE(WM_MainRePortThreadControl, &CReSimDataDownload::MainRePortThreadControl)
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
	MainReFormHWND = FindWindow(NULL, L"����ģʽ")->m_hWnd;

	//�̳߳س�ʼ��
	m_lpThreadPool = NULL;
	if (!m_lpThreadPool)
	{
		m_lpThreadPool = new CThreadPool();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//��ʼ��ģ�麯��
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
	if (m_resimdatafolderPath == L"")
	{
		MessageBox(L"��ѡ��SIM������·����", L"��ʾ��Ϣ", NULL);
		return;
	}

	reporthandler = parentdlg->InitCom(reComNo);

	if (reporthandler == NULL)
	{
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
			//SetRicheditText(L"�رմ���ʧ��", 1);
			MessageBox(L"�رմ���ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}

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
		OpenThreadPoolTask(PORT_REAUTO_THREAD);
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


//����λ�߳�
//�����̳߳غ���
void CReSimDataDownload::OpenThreadPoolTask(int Command)
{
	CTask* pTask = new CTask;
	pTask->SetCommand(Command);
	m_lpThreadPool->AddTask(pTask);
}

//�̳߳ص���Ϣѭ��
afx_msg LRESULT CReSimDataDownload::MainRePortThreadControl(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		//�����ǵ��ļ����ص�
		//һ��ʼ���Ƿ���test���test�����д�Ͷ�ͬʱ��
	case MainPort_RePort_Test:
		OpenThreadPoolTask(REPORT_WRITE1_THREAD);
		OpenThreadPoolTask(REPORT_READ1_THREAD);
		break;
		//Ȼ��ʼ����RID��IMEI���д�Ͷ��ֱ�
	case MainPort_RePort_Write2:
		OpenThreadPoolTask(REPORT_WRITE2_THREAD);
		break;
	case MainPort_RePort_Read2:
		OpenThreadPoolTask(REPORT_READ2_THREAD);
		break;
	default:
		break;
	}
	return 0;
}

//����λ�����߳�
void CReSimDataDownload::ReDownloadMainThread(LPVOID lpParam)
{
	strReFolderpath = strOKFolderpath.Left(strOKFolderpath.GetLength() - 3) + L"\\";

	if (!PathIsDirectory(strReFolderpath))
	{
		::CreateDirectory(strReFolderpath, NULL);//����Ŀ¼,���еĻ���Ӱ��
	}

	while (!s_bReExit)
	{
		//::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Test, NULL);
		
		
		Sleep(8000);
	}
}

//����λtestָ���д�߳�
void CReSimDataDownload::ReDownloadWrite1PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//���ڱ���
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//��ָ���ñ���
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//һֱtest������������û��������
	dlg->SetRePortEditEmpty();
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
	m_RePortDownloadWrite1 = TRUE;

	ClearCommError(reporthandler, &dwErrorFlags, &ComStat);

	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"��:" + strcommand, 0);
		bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			dlg->ReDownloadClosePortThread();
			return;
		}
	} while (m_RePortDownloadWrite1);

	return;
}

//����λtestָ��Ķ��߳�
void CReSimDataDownload::ReDownloadWrite2PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	//���ڱ���
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//�������
	CString strread;

	m_RePortDownloadRead1 = TRUE;;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�

	do
	{
		Sleep(1520);
		bReadStat = ReadFile(reporthandler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK") >= 0)
			{
				m_RePortDownloadRead1 = FALSE;
				m_RePortDownloadWrite1 = FALSE;
			}
		}
		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			dlg->ReDownloadClosePortThread();
			return;
		}
	} while (m_RePortDownloadRead1);
	dlg->SetRicheditText(L"��:" + strread, 0);
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Write2, NULL);
	return;
}

//����λ��RID��IMEIָ���д�߳�
void CReSimDataDownload::ReDownloadRead1PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, Main_Hint1_Connected, NULL);
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//���ڱ���
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//�ļ��ܴ�С

	//��ָ���ñ���
	CString strcommand;
	CString strfilewritecommand;


	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Read2, NULL);

	Sleep(100);
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//�������˾Ϳ�ʼ������RID��IMEI��������������������û��Ӧ���Ǿͷ���Test�������¼���豸
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//��RID��һ������
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//��IMEI��һ������
		}

		ClearCommError(reporthandler, &dwErrorFlags, &ComStat);

		//Ȼ��ͬʱ�������߳�
		m_RePortDownloadWrite2 = TRUE;

		PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
		do
		{

			dlg->SetRicheditText(L"��:" + strcommand, 0);
			bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				//::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, Main_Hint1_Fail, NULL);
				dlg->ReDownloadRestPortThread();
				dlg->ReDownloadClosePortThread();
				return;
			}
			count++;
			Sleep(600);
			if (s_bReExit == TRUE)
			{
				dlg->ReDownloadRestPortThread();
				dlg->ReDownloadClosePortThread();
				return;
			}
		} while (m_RePortDownloadWrite2);
	}
	return;
}

//����λ��RID��IMEIָ��Ķ��߳�
void CReSimDataDownload::ReDownloadRead2PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	//���ڱ���
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//�������
	CString strread;
	CString strcount, strcounttemp;//��RID��IMEI��
	CString strtemp;
	int findcount1, findcount2;//�и���������ַ�����

	for (int i = 0; i < 2; i++)
	{
		m_RePortDownloadRead2 = TRUE;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�
		m_RePortDownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//��RID��һ������
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//��IMEI��һ������
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(reporthandler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//��RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//��IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						strcounttemp = strcount;
						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);
							//::PostMessage(MainReFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_IsExit, NULL);
							Sleep(50);
						}
						else
						{
							continue;
						}
					}
					m_RePortDownloadWrite2 = FALSE;
					m_RePortDownloadRead2 = FALSE;
				}
			}
		} while (m_RePortDownloadRead2);
		if (m_RePortDownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			dlg->ReDownloadClosePortThread();
			return;
		}
		dlg->SetRicheditText(L"��:" + strread, 0);
	}
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	return;
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

//����λ���ڹر��߳�ȫ�ֱ���
void CReSimDataDownload::ReDownloadClosePortThread()
{
	m_TreadRePortDownloadWrite1 = FALSE;
	m_TreadRePortDownloadWrite2 = FALSE;
	m_TreadRePortDownloadRead1 = FALSE;
	m_TreadRePortDownloadRead2 = FALSE;
}


//��������
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
		if (str1 == "����ʧ��" || str1 == "�޴˻���¼" || str1 == "����·������" || str1 == "���践��")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticReHintfont);
		}
		else if (str1 == "�����ɹ�")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//��RGB��ı���ɫ 
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
	CDialogEx::OnCancel();
	}
}

