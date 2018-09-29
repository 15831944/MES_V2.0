
// MFCP4ECurrentToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP4ECurrentTool.h"
#include "MFCP4ECurrentToolDlg.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "DBconfig.h"
#include "ModelConfig.h"
#include "Log.h"
#include "Manager.h"

#include <string>
#include "IT6400.h"
#include <ctime>
#include <vector>
#include <windows.h>
#include "Keysight34461A.h"
#pragma comment(lib,"IT6400.lib")
#pragma comment(lib,"Keysight34461A.lib")
static char InstrName[] = "GPIB0::23::INSTR";//�豸����PC�Ĵ�����

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ȫ�ֱ���
map<int, CString> CMFCP4ECurrentToolDlg::CommandMap;//�����Ҫ���͵�ָ��
map<int, CString> CMFCP4ECurrentToolDlg::CommandReplyMap;//���ָ��Ļظ�

int CommandCount;//ָ�������
int CommandSendInterval;//ָ��ͼ��
int CommandReadInterval;//ָ���ȡ���
int StandbyInterval=18000;//�����ȴ�ʱ��
int SleepInterval=18000;//˯�ߵȴ�ʱ��
CString Port1LogName;//��־�ļ���
CString Currentper[5];//�����ε���
float StandbyAverage;//��������ƽ��ֵ
float SleepAverage;//˯�ߵ���ƽ��ֵ

volatile BOOL s_bExit;
volatile BOOL m_MainConrtolFlag;//�����̱߳�־λ
volatile BOOL m_Port1ReadFlag;
volatile BOOL m_Port1SendFlag;

volatile BOOL m_Port1SendFlag1;
volatile BOOL m_Port1SendFlag2;
volatile BOOL m_Port1ReadFlag1;
volatile BOOL m_Port1ReadFlag2;
volatile BOOL m_Port1ReadFlagEnd2;

static bool OutputEnable = false;//��֪�������õ�
uintptr_t* VisaNameOut = 0;//���ں�
double CurrRange = 3;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCP4ECurrentToolDlg �Ի���



CMFCP4ECurrentToolDlg::CMFCP4ECurrentToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP4ECurrentToolDlg::IDD, pParent)
	, m_StandbyUpValue(0)
	, m_StandbyCuValue(_T(""))
	, m_StandbyDownValue(0)
	, m_Port1RidValue(_T(""))
	, m_SleepUpValue(0)
	, m_SleepCuValue(_T(""))
	, m_SleepDownValue(0)
	, ConnectFlag(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCP4ECurrentToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT1_COMBO, m_Port1Combo);
	DDX_Control(pDX, IDC_MODEL_COMBO, m_ModelCombo);
	DDX_Control(pDX, IDC_CURRENTLOG_RICHEDIT, m_CurrentLog);
	DDX_Control(pDX, IDC_STANDBYUP_EDIT, m_StandbyUpControl);
	DDX_Text(pDX, IDC_STANDBYUP_EDIT, m_StandbyUpValue);
	DDX_Control(pDX, IDC_STANDBYCU_EDIT, m_StandbyCuControl);
	DDX_Text(pDX, IDC_STANDBYCU_EDIT, m_StandbyCuValue);
	DDX_Control(pDX, IDC_STANDBYDOWN_EDIT, m_StandbyDownControl);
	DDX_Text(pDX, IDC_STANDBYDOWN_EDIT, m_StandbyDownValue);
	DDX_Control(pDX, IDC_PORT1RID_EDIT, m_Port1RidControl);
	DDX_Text(pDX, IDC_PORT1RID_EDIT, m_Port1RidValue);
	DDX_Control(pDX, IDC_SLEEPUP_EDIT, m_SleepUpControl);
	DDX_Text(pDX, IDC_SLEEPUP_EDIT, m_SleepUpValue);
	DDX_Control(pDX, IDC_SLEEPCU_EDIT, m_SleepCuControl);
	DDX_Text(pDX, IDC_SLEEPCU_EDIT, m_SleepCuValue);
	DDX_Control(pDX, IDC_SLEEPDOWN_EDIT, m_SleepDownControl);
	DDX_Text(pDX, IDC_SLEEPDOWN_EDIT, m_SleepDownValue);
}

BEGIN_MESSAGE_MAP(CMFCP4ECurrentToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP4ECurrentToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP4ECurrentToolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP4ECurrentToolDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_MODELCONFIG_BUTTON, &CMFCP4ECurrentToolDlg::OnBnClickedModelconfigButton)
	ON_CBN_SELCHANGE(IDC_MODEL_COMBO, &CMFCP4ECurrentToolDlg::OnCbnSelchangeModelCombo)
	ON_BN_CLICKED(IDC_PORT1CONNECT_BUTTON, &CMFCP4ECurrentToolDlg::OnBnClickedPort1connectButton)
	ON_CBN_KILLFOCUS(IDC_MODEL_COMBO, &CMFCP4ECurrentToolDlg::OnCbnKillfocusModelCombo)
	ON_CBN_SELENDOK(IDC_MODEL_COMBO, &CMFCP4ECurrentToolDlg::OnCbnSelendokModelCombo)
	ON_CBN_DROPDOWN(IDC_PORT1_COMBO, &CMFCP4ECurrentToolDlg::OnCbnDropdownPort1Combo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCP4ECurrentToolDlg ��Ϣ�������

BOOL CMFCP4ECurrentToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ������������
	InitModelDBOperation();

	//��ʼ�������б�
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORT1_COMBO), Port1No, 1);

	//��־�ļ����Ƴ�ʼ��
	Port1LogName = GetLogTime() + L"Port1Log";

	//�����ʼ��
	fontinit();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCP4ECurrentToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCP4ECurrentToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCP4ECurrentToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*�ؼ�ʹ�ܺ���*/
//��ʼģ��ؼ�ʹ�ܺ���
void CMFCP4ECurrentToolDlg::InitEnableWindows(BOOL choose)
{
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(choose);
	GetDlgItem(IDC_MODELCONFIG_BUTTON)->EnableWindow(choose);
	GetDlgItem(IDC_MODEL_COMBO)->EnableWindow(choose);
	GetDlgItem(IDC_PORT1_COMBO)->EnableWindow(choose);
}

/*��ʼ��ģ��*/
//�������ð�ť
void CMFCP4ECurrentToolDlg::OnBnClickedModelconfigButton()
{
	INT_PTR nRes;
	CManager *manager = new CManager;
	nRes = manager->DoModal();

	if (IDCANCEL == nRes)
	{
		InitModelDBOperation();
		return;
	}

	else if (IDOK == nRes)
	{
		INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

		CModelConfig modelDlg;
		nRes = modelDlg.DoModal();  // �����Ի���

		if (IDCANCEL == nRes)
			return;
	}
}

//����������ѡ����ͺ����Ĳ���
void CMFCP4ECurrentToolDlg::OnCbnSelchangeModelCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FindModelDBOperation();
}

//����������ʧȥ��������Ĳ���
void CMFCP4ECurrentToolDlg::OnCbnKillfocusModelCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//FindModelDBOperation();
}

//����������OK��Ĳ���
void CMFCP4ECurrentToolDlg::OnCbnSelendokModelCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
/*	FindModelDBOperation()*/;
}

//���һ������ݿ����
void CMFCP4ECurrentToolDlg::FindModelDBOperation()
{
	ADOManage adomanageFind;
	CString ModelName, strtemp;
	float ftemp;
	int nSel;
	nSel = m_ModelCombo.GetCurSel();
	m_ModelCombo.GetLBText(nSel, ModelName);
	adomanageFind.ConnSQL();
	adomanageFind.CheckConfigByModelNameSql(ModelName);

	//��ȡ���������޵�ֵ��ע�⣬����û�и����͵�setdlgitem������ֻ����updatadata�ķ�ʽ�����µ����棩
	ftemp = adomanageFind.m_pRecordSet->GetCollect("StandbyUp");
	m_StandbyUpValue = ftemp;

	ftemp = adomanageFind.m_pRecordSet->GetCollect("StandbyDown");
	m_StandbyDownValue = ftemp;

	ftemp = adomanageFind.m_pRecordSet->GetCollect("SleepUP");
	m_SleepUpValue = ftemp;

	ftemp = adomanageFind.m_pRecordSet->GetCollect("SleepDown");
	m_SleepDownValue = ftemp;
	UpdateData(FALSE);

	CommandMap[0] = adomanageFind.m_pRecordSet->GetCollect("TestCommand").bstrVal;//�������ָ��
	CommandReplyMap[0] = adomanageFind.m_pRecordSet->GetCollect("TestCommandReply").bstrVal;

	CommandMap[1] = adomanageFind.m_pRecordSet->GetCollect("RidCommand").bstrVal;//��RIDָ��
	CommandReplyMap[1] = adomanageFind.m_pRecordSet->GetCollect("RidCommandReply").bstrVal;

	CommandMap[2] = adomanageFind.m_pRecordSet->GetCollect("StandbyCommand").bstrVal;//����ָ��
	CommandReplyMap[2] = adomanageFind.m_pRecordSet->GetCollect("StandbyCommandReply").bstrVal;

	CommandMap[3] = adomanageFind.m_pRecordSet->GetCollect("SleepCommand").bstrVal;//����ָ��
	CommandReplyMap[3] = adomanageFind.m_pRecordSet->GetCollect("SleepCommandReply").bstrVal;

	CommandCount = adomanageFind.m_pRecordSet->GetCollect("Count");//ָ�������
	CommandSendInterval = adomanageFind.m_pRecordSet->GetCollect("WriteTime");//ָ��ͼ��
	CommandReadInterval = adomanageFind.m_pRecordSet->GetCollect("ReadTime");//ָ���ȡ���

	adomanageFind.CloseAll();
}

//��ʼ���������������ݿ����
void CMFCP4ECurrentToolDlg::InitModelDBOperation()
{
	m_ModelCombo.ResetContent();
	ADOManage adomanageCombo;
	adomanageCombo.ConnSQL();
	adomanageCombo.m_pRecordSet = adomanageCombo.CheckAllInConfigSql();
	while (!adomanageCombo.m_pRecordSet->adoEOF)
	{
		m_ModelCombo.AddString(adomanageCombo.m_pRecordSet->GetCollect("ModelName").bstrVal);
		adomanageCombo.m_pRecordSet->MoveNext();
	}
	adomanageCombo.CloseAll();
}

//������ں��������ʱ����Զ����´��ں�
void CMFCP4ECurrentToolDlg::OnCbnDropdownPort1Combo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORT1_COMBO), Port1No, 1);
}

//������Ӱ�ť
void CMFCP4ECurrentToolDlg::OnBnClickedPort1connectButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetCommPort((CComboBox*)GetDlgItem(IDC_PORT1_COMBO), Port1No);//�Ȼ�ȡ��ǰ���ں�

	if (ConnectFlag == FALSE)
	{
		CString ModelName;
		GetDlgItemText(IDC_MODEL_COMBO,ModelName);
		//�������Ƿ��Ѿ�ѡ��
		if (ModelName == L"")
		{
			MessageBox(L"����ѡ�����", L"��ʾ��Ϣ", NULL);
			return;
		}

		Port1handler = InitCom(Port1No);
		if (Port1handler == NULL)
		{
			SetRicheditText(L"��������ʧ��", 1);
			PrintLog(L"��������ʧ��", 1);
			return;
		}

		SetDlgItemText(IDC_PORT1CONNECT_BUTTON, L"�Ͽ�");
		InitEnableWindows(FALSE);
		ConnectFlag = TRUE;

		//�����߳�
		ConfMeas(InstrName, CurrRange, VisaNameOut);
		s_bExit = FALSE;
		m_MainConrtolFlag = TRUE;
		MainThread = AfxBeginThread(MainControlThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else if (ConnectFlag == TRUE)
	{
		if (!CloseCom(Port1handler))
		{
			SetRicheditText(L"�رմ���ʧ��", 1);
			PrintLog(L"�رմ���ʧ��", 1);
			MessageBox(L"�رմ���1ʧ��", L"��ʾ��Ϣ", NULL);
			return;
		}

		SetRicheditText(L"�رմ��ڳɹ�", 0);
		PrintLog(L"�رմ��ڳɹ�", 1);

		SetDlgItemText(IDC_PORT1CONNECT_BUTTON, L"����");
		InitEnableWindows(TRUE);
		ConnectFlag = FALSE;

		s_bExit = TRUE;
		m_MainConrtolFlag = FALSE;
	}
}


/*����ͨ�ò���*/
//��ע�����Ѱ���Ѿ�ע�ᴮ�ں�Ȼ��չʾ����
void CMFCP4ECurrentToolDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO)
{
	//�Ȼ�ȡ��ǰ���ں�
	int cur = 0;
	int nSel;
	BOOL curflag = TRUE;
	nSel = pComboBox->GetCurSel();
	if (nSel >= 0)
	{
		pComboBox->GetLBText(nSel, ComNo);
	}

	//�����һ���б�
	pComboBox->ResetContent();

	HKEY hKey;
#ifdef _DEBUG
	ASSERT(pComboBox != NULL);
	pComboBox->AssertValid();
#endif
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // �򿪴���ע���
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPWSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // ö�ٴ���
			{
				break;
			}
			if (nSel >= 0 && ComNo == LPWSTR(commName))//������ϴ�ѡ�������ǾͲ��õ�ǰѡ��䶯
			{
				/*cur = i - PortNO+1;*/
				cur = i;
				curflag = FALSE;
			}
			//���������ǾͰ�˳���Զ�����һ�£����һ����־λʹ�ø������ڲ����cur�ظ���ֵ
			else
			{
				if (PortNO == 1 && i == 0 && curflag == TRUE)
				{
					cur = i;
				}
				else if (PortNO == 2 && i == 1 && curflag == TRUE)
				{
					cur = i;
				}
				else if (PortNO == 3 && i == 2 && curflag == TRUE)
				{
					cur = i;
				}
				else if (PortNO == 4 && i == 3 && curflag == TRUE)
				{
					cur = i;
				}
			}
			pComboBox->AddString(LPWSTR(commName));
			//if (PortNO == 1 && i >= 0)
			//{
			//	pComboBox->AddString(LPWSTR(commName)); // commName���Ǵ�������
			//}
			//else if (PortNO == 2 && i >= 1)
			//{
			//	pComboBox->AddString(LPWSTR(commName));
			//}
			//else if (PortNO == 3 && i >= 2)
			//{
			//	pComboBox->AddString(LPWSTR(commName));
			//}
			//else if (PortNO == 4 && i >= 3)
			//{
			//	pComboBox->AddString(LPWSTR(commName));
			//}
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			//SetRicheditText(L"�Ҳ������ڣ����⴮���豸�Ƿ�������⣡", 1);
			//MessageBox(L"�Ҳ������ڣ����⴮���豸�Ƿ�������⣡", L"��ʾ��Ϣ", NULL);
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//��ʼ������
HANDLE CMFCP4ECurrentToolDlg::InitCom(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"���ڳ�ʼ��ʧ�ܣ�", 1);
		//MessageBox(L"���ڳ�ʼ��ʧ�ܣ�", L"��ʾ��Ϣ", NULL);
		return NULL;
	}
	SetupComm(hCom, 100, 100); //���뻺����������������Ĵ�С����100
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 115200; //������Ϊ9600
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	SetRicheditText(L"���ڳ�ʼ���ɹ���", 0);
	return hCom;
}

//�رմ���
int CMFCP4ECurrentToolDlg::CloseCom(HANDLE handler)
{
	return CloseHandle(handler);
}

//��ȡ��ǰ���ں�
void CMFCP4ECurrentToolDlg::GetCommPort(CComboBox *pComboBox, CString &ComNo)
{
	int nSel;
	nSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel, ComNo);

	if (ComNo == "")
	{
		MessageBox(L"����ѡ�񴮿ںţ�", L"��ʾ��Ϣ", NULL);
	}
}


/*ָ�����̺߳���*/
//�����߳�
UINT MainControlThread(LPVOID lpParam)
{
	CMFCP4ECurrentToolDlg* dlg;
	dlg = (CMFCP4ECurrentToolDlg*)lpParam;

	dlg->DWThread = AfxBeginThread(SendPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);//��������ָ���߳�

	//���û�б�ֹͣ���Ǿ�һֱȥ����test�߳�
	while (m_MainConrtolFlag)
	{
		if (dlg->DWThread == NULL)
			dlg->DWThread = AfxBeginThread(SendPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		Sleep(4000);
	}

	dlg->MainThread = NULL;
	return 0;
}

//���ڷ���ָ���߳�
UINT SendPort1Thread(LPVOID lpParam)
{
	CMFCP4ECurrentToolDlg* dlg;
	dlg = (CMFCP4ECurrentToolDlg*)lpParam;

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//���ڱ���
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//��ָ���ñ���
	CString strcommand;

	//һֱtest������������û��������
PortTest:
	//�ȳ�ʼ��һЩ����
	StandbyAverage=0;
	SleepAverage=0;
	for (int i = 0; i < 5; i++)
	{
		Currentper[i]=L"";
	}
	m_Port1SendFlag = TRUE;
	dlg->CommandNo = 0;
	strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";
	
	//��ʼ�������Ƿ�����
	ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

	//ͬʱ�������߳�
	dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	//if (RePortAbnomal == TRUE)//��������������쳣��Σ�����ͣ�ٸ�2��ż���������
	//{
	//	Sleep(2000);
	//}

	//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"�ȴ�����");

	do
	{
		dlg->PrintLog(L"��:" + strcommand,1);
		dlg->SetRicheditText(L"��:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		if (s_bExit == TRUE)
		{
			dlg->RestPort1Thread();
			return 0;
		}
		Sleep(2000);
	} while (m_Port1SendFlag);

	if (m_MainConrtolFlag == FALSE)
	{
		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"ֹͣ");
		return 0;
	}
	dlg->SetPort1EditEmpty();
	dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"�����ӣ����Դ���������");

//	RePortAbnomal = FALSE;

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//�������˾Ϳ�ʼ���������
	for (int i = 1; i < 3; i++)
	{
		int count = 0;
		dlg->CommandNo = i;//�ȷ���RIDָ�Ȼ��shutdownָ��
		strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";
		ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

		//Ȼ��ͬʱ�������߳�
		m_Port1SendFlag = TRUE;
		dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		do
		{
			dlg->PrintLog(L"��:" + strcommand, 1);
			//dlg->SetRicheditText(L"��:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == CommandCount)//�趨ָ��Ͷ��ٴ�
			{
				//һֱ���뷢��testָ������ⲻ�����Ǵ������Ѿ��Ͽ���
				BOOL reporttestflag = TRUE, reportfailflag = TRUE;
				int reporttestcount = 0;
				char reportteststr[100];
				memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
				DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
				BOOL reporttestbReadStat, reporttestbWriteStat;
				CString reportteststrread, reportteststrcommand = CMFCP4ECurrentToolDlg::CommandMap[0] + L"\r\n";
				COMSTAT reporttestComStat;

				ClearCommError(dlg->Port1handler, &reporttestdwErrorFlags, &reporttestComStat);

				do
				{
					reporttestbWriteStat = WriteFile(dlg->Port1handler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

					Sleep(90);
					reporttestbReadStat = ReadFile(dlg->Port1handler, reportteststr, 100, &reporttestreadreal, 0);
					if (reporttestbReadStat)
					{
						reportteststrread = reportteststr;
						if (reportteststrread.Find(CMFCP4ECurrentToolDlg::CommandReplyMap[0]) >= 0)
						{
							if (reportfailflag == TRUE)
							{

								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"ʧ�ܴ�����");
								reportfailflag = FALSE;
							}
							reporttestcount = 0;
						}
						else
						{
							if (reportfailflag == TRUE)
							{

								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"�쳣���");
								reportfailflag = FALSE;
								/*RePortAbnomal = TRUE;*/
							}
						}
						reporttestcount = 0;
					}
					reporttestcount++;
					if (reporttestcount == 5)
					{
						reporttestflag = FALSE;
					}
					Sleep(10);
					memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
					dlg->PrintLog(L"��:" + reportteststrcommand, 1);
					PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (reporttestflag);

				Sleep(100);
				m_Port1SendFlag = FALSE;
				m_Port1ReadFlag = FALSE;
				goto PortTest;
			}
			count++;
			Sleep(CommandSendInterval);//ָ��ͼ��
		} while (m_Port1SendFlag);
	}

	//��һ���Ƕ���������
	Sleep(StandbyInterval);//ͣ��ʮ��
	StandbyAverage = Current() * 1000;
	ConfMeas(InstrName, CurrRange, VisaNameOut);//��ʼ��������ȷ���������������趨��״̬
	if (_isnan(StandbyAverage))
	{
		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"��ȡ������������ֵ�����������������������");
		goto PortTest;
	}
	else
	{
		CString standbyStr;
		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"���������Ѳ⣡�������豸��");
		standbyStr.Format(L"%.3f", StandbyAverage);
		dlg->SetDlgItemText(IDC_STANDBYCU_EDIT, standbyStr);
	}

	Sleep(1000);//��һ��

	//Ȼ������������Ƿ��Ѿ�����
	m_Port1SendFlag = TRUE;
	dlg->CommandNo = 0;
	strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";

	//��ʼ�������Ƿ�����
	ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

	//ͬʱ�������߳�
	dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	do
	{
		dlg->PrintLog(L"��:" + strcommand, 1);
		dlg->SetRicheditText(L"��:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		if (s_bExit == TRUE)
		{
			dlg->RestPort1Thread();
			return 0;
		}
		Sleep(2000);
	} while (m_Port1SendFlag);

	dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"����˯�ߵ����У�");

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//���ӵ��˾Ϳ�ʼ����˯�ߵ���
	for (int i = 1; i < 3; i++)
	{
		int count = 0;
		if (i == 1)
		{
			dlg->CommandNo = i;//�ȷ���RIDָ��
		}
		else if (i == 2)
		{
			dlg->CommandNo = i+1;//Ȼ��SLEEPָ��
		}
		strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";
		ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

		//Ȼ��ͬʱ�������߳�
		m_Port1SendFlag = TRUE;
		dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		do
		{
			dlg->PrintLog(L"��:" + strcommand, 1);
			//dlg->SetRicheditText(L"��:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == CommandCount)//�趨ָ��Ͷ��ٴ�
			{
				//һֱ���뷢��testָ������ⲻ�����Ǵ������Ѿ��Ͽ���
				BOOL reporttestflag = TRUE, reportfailflag = TRUE;
				int reporttestcount = 0;
				char reportteststr[100];
				memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
				DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
				BOOL reporttestbReadStat, reporttestbWriteStat;
				CString reportteststrread, reportteststrcommand = CMFCP4ECurrentToolDlg::CommandMap[0] + L"\r\n";
				COMSTAT reporttestComStat;

				ClearCommError(dlg->Port1handler, &reporttestdwErrorFlags, &reporttestComStat);

				do
				{
					reporttestbWriteStat = WriteFile(dlg->Port1handler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

					Sleep(90);
					reporttestbReadStat = ReadFile(dlg->Port1handler, reportteststr, 100, &reporttestreadreal, 0);
					if (reporttestbReadStat)
					{
						reportteststrread = reportteststr;
						if (reportteststrread.Find(CMFCP4ECurrentToolDlg::CommandReplyMap[0]) >= 0)
						{
							if (reportfailflag == TRUE)
							{

								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"ʧ�ܴ�����");
								reportfailflag = FALSE;
							}
							reporttestcount = 0;
						}
						else
						{
							if (reportfailflag == TRUE)
							{

								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"�쳣���");
								reportfailflag = FALSE;
								/*RePortAbnomal = TRUE;*/
							}
						}
						reporttestcount = 0;
					}
					reporttestcount++;
					if (reporttestcount == 5)
					{
						reporttestflag = FALSE;
					}
					Sleep(10);
					memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
					dlg->PrintLog(L"��:" + reportteststrcommand, 1);
					PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (reporttestflag);

				Sleep(100);
				m_Port1SendFlag = FALSE;
				m_Port1ReadFlag = FALSE;
				goto PortTest;
			}
			count++;
			Sleep(CommandSendInterval);//ָ��ͼ��
		} while (m_Port1SendFlag);
	}

	//�ڶ����Ƕ�˯�ߵ���
	Sleep(SleepInterval);//ͣ��15��
	SleepAverage = Current() * 1000;
	ConfMeas(InstrName, CurrRange, VisaNameOut);//��ʼ��������ȷ���������������趨��״̬
	if (_isnan(StandbyAverage) || _isnan(SleepAverage))
	{
		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"��ȡ������������ֵ�����������������������");
		goto PortTest;
	}
	else
	{
		CString sleepStr;
		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"PASS����������һ̨���ӣ�");
		sleepStr.Format(L"%.3f", SleepAverage);
		dlg->SetDlgItemText(IDC_SLEEPCU_EDIT, sleepStr);
	}


	////������ָ��
	//strcommand = dlg->CommandWriteUnit(6);
	//ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);
	//bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintLog(L"��:" + strcommand,1);

	if (m_MainConrtolFlag == FALSE)
	{

		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"ֹͣ");

		return 0;
	}
	Sleep(1500);

	dlg->DWThread = NULL;
	return 0;
}

//���ڶ��߳�
UINT ReadPort1Thread(LPVOID lpParam)
{
	CMFCP4ECurrentToolDlg* dlg;
	dlg = (CMFCP4ECurrentToolDlg*)lpParam;

	//���ڱ���
	char str[200];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//�������
	CString strread;
	CString strcount, strcounttemp;//��RID��IMEI��
	CString strtemp;
	int findcount1, findcount2, Reflag;//�и���������ַ�����

	m_Port1ReadFlag = TRUE;;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�

	do
	{
		Sleep(CommandSendInterval);//ָ������
		bReadStat = ReadFile(dlg->Port1handler, str, 199, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			strtemp = CMFCP4ECurrentToolDlg::CommandReplyMap[dlg->CommandNo];
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
						dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);
					}
					else if (strcount == "")
					{
						continue;
					}
				}
				m_Port1ReadFlag = FALSE;
				m_Port1SendFlag = FALSE;
			}
		}
	} while (m_Port1ReadFlag);
	//dlg->SetRicheditText(L"��:" + strread, 0);
	dlg->PrintLog(L"��:" + strread, 1);

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	dlg->DRThread = NULL;
	return 0;
}

//�����߳�ȫ�ֱ���
void CMFCP4ECurrentToolDlg::RestPort1Thread()
{
	m_Port1SendFlag1 = TRUE;
	m_Port1SendFlag2 = TRUE;
	m_Port1ReadFlag1 = TRUE;
	m_Port1ReadFlag2 = TRUE;
	m_Port1ReadFlagEnd2 = TRUE;
}

//���RID�͵����༭��
void CMFCP4ECurrentToolDlg::SetPort1EditEmpty()
{
	SetDlgItemText(IDC_PORT1RID_EDIT, L"");
	SetDlgItemText(IDC_STANDBYCU_EDIT, L"");
	SetDlgItemText(IDC_SLEEPCU_EDIT, L"");
}


/*LVҪ�õ��ĺ���*/
//�������Ժ���
bool PowerOn()
{
	uintptr_t* VisaNameOut = 0;//���ں�

	LVBoolean DefaSetInit = false;
	LVBoolean * DefaSetValid = &DefaSetInit;//Ĭ�ϳ�ʼ��


	float VSetValue = 3.0;
	LVBoolean VSetInit = false;
	LVBoolean *VsetValid = &VSetInit;//���õ�ѹ

	double VLimit = 4.0;
	LVBoolean EnVLimitSet = true;
	LVBoolean VLimitSetInit = false;
	LVBoolean *VLimitSetValid = &VLimitSetInit;//��ѹ����

	double VInit = 0;
	double *VMeasValue = &VInit;
	LVBoolean VoltMeasInit = false;
	LVBoolean *VMeasValid = &VoltMeasInit;//��ѹ����ֵ

	float CSetValue = 0.3;
	LVBoolean CSetInit = false;
	LVBoolean* CSetValid = &CSetInit;//����Ԥ��ֵ����

	LVBoolean EnableOutput = true;
	LVBoolean OutputInit = false;
	LVBoolean *OutputSetValid = &OutputInit;//�������
	OutputEnable = (bool)OutputInit;

	Defaultset(InstrName, VisaNameOut, DefaSetValid);
	VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
	VSet(InstrName, VSetValue, VsetValid);
	EnOutput(InstrName, EnableOutput, OutputSetValid);
	return OutputInit;
}

double average(vector<double>::const_iterator begin, vector<double>::const_iterator end, vector<double>::size_type elementsNum)
{
	//��ȡϵͳʱ��
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);


	double currAverage;
	double sum = 0;
	int i = 0;
	CString currstr;
	while (begin != end)
	{
		Currentper[i];
		sum = sum + (*begin);
		currstr.Format(strTime + _T("CU:%3f"), (*begin));
		Currentper[i] = currstr;
		begin++;
		i++;
	}
	currAverage = sum / elementsNum;
	return currAverage;
}

double Current()
{
	double CInit = 0;
	double *CMeasValue = &CInit;
	LVBoolean CMeasInit = false;
	LVBoolean *CMeasValid = &CMeasInit;//��������ֵ
	vector<double> cur_Arr; //��Ų��Ե�����ֵ������
	double curr_Average = 0;  //��ŵȴ�ģʽ�µ�����ƽ��ֵ

	/**********************************
	���Ե�����ƽ��ֵ
	***********************************/
	//������ʼ��
	//	ConfMeas(InstrName, CurrRange, VisaNameOut);

	for (int i = 0; i < 5; i++)
	{
		//������ʼ��
		ConfMeas(InstrName, CurrRange, VisaNameOut);
		Sleep(1000);
		CMeas(InstrName, CMeasValue, CMeasValid);

		if ((bool)CMeasInit)
		{
			cur_Arr.push_back(*CMeasValue);
			cout << *CMeasValue << endl;
		}

	}
	//������ʼ��
	ConfMeas(InstrName, CurrRange, VisaNameOut);
	curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());


	/**********************************
	���Ե�����ƽ��ֵ
	***********************************/
	return curr_Average;
}


/*���ݿ�ģ��*/
//���ݿ����ð�ť
void CMFCP4ECurrentToolDlg::OnBnClickedDbconfigButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

	CDBconfig dbconfigDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = dbconfigDlg.DoModal();  // �����Ի���

	if (IDCANCEL == nRes)
		return;
}


/*��־ģ��*/
//��ǰ��־����
void CMFCP4ECurrentToolDlg::SetRicheditText(CString strMsg, int No)
{
	m_CurrentLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

	switch (No)
	{
	case 0:
		m_CurrentLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_CurrentLog.LineScroll(m_CurrentLog.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_CurrentLog.SetSelectionCharFormat(cf);
		m_CurrentLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_CurrentLog.LineScroll(m_CurrentLog.GetLineCount() - 1);
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
CString CMFCP4ECurrentToolDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//��־��ӡ����
void CMFCP4ECurrentToolDlg::PrintLog(CString strMsg, int No)
{
	CLog Relog1;//��־�ļ�

	switch (No)
	{
	//case 0:
	//	Relog0.WriteLog(CStringA(L"��" + GetTime() + L"��") + CStringA(strMsg), SinglePortLogName);
	//	break;
	case 1:
		Relog1.WriteLog(CStringA(L"��" + GetTime() + L"��") + CStringA(strMsg), Port1LogName);
		break;
	//case 2:
	//	Relog2.WriteLog(CStringA(L"��" + GetTime() + L"��") + CStringA(strMsg), Port2LogName);
	//	break;
	//case 3:
	//	Relog3.WriteLog(CStringA(L"��" + GetTime() + L"��") + CStringA(strMsg), Port3LogName);
	//	break;
	//case 4:
	//	Relog4.WriteLog(CStringA(L"��" + GetTime() + L"��") + CStringA(strMsg), Port4LogName);
	//	break;
	default:
		break;
	}
}

//��ȡ��־ʱ��
CString CMFCP4ECurrentToolDlg::GetLogTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}


/*�����Լ���ɫ������*/
//������ĵ���Ϣѭ��
afx_msg LRESULT MainFontControl(WPARAM wParam, LPARAM lParam)
{

}

//��ʼ������
void CMFCP4ECurrentToolDlg::fontinit()
{
	staticHint1font.CreatePointFont(500, L"����");
	editfont.CreatePointFont(200, L"����");

	GetDlgItem(IDC_STANDBYCU_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_SLEEPCU_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_PORT1HINT_STATIC)->SetFont(&staticHint1font);
}

HBRUSH CMFCP4ECurrentToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	CString str1;
	if (pWnd->GetDlgCtrlID() == IDC_PORT1HINT_STATIC)
	{
		GetDlgItemText(IDC_PORT1HINT_STATIC, str1);
		if (str1 == "���������Ѳ⣡�������豸��")
		{
			m_brush.CreateSolidBrush(RGB(255, 165, 0));
			pDC->SetBkColor(RGB(255, 165, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			return m_brush;
		}
		else if (str1 == "PASS����������һ̨���ӣ�")
		{
			m_brush.CreateSolidBrush(RGB(50, 205, 50));
			pDC->SetBkColor(RGB(50, 205, 50));
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ
			return m_brush;
		}
		//else if (str1 == "�����ɹ�" || str1 == "PASS")
		//{
		//	pDC->SetTextColor(RGB(0, 255, 0));//��RGB��ı���ɫ 
		//	pDC->SelectObject(&staticReHintfont);
		//}
		//else if (str1 == "�ȴ�����" || str1 == "Ready")
		//{
		//	pDC->SetTextColor(RGB(65, 105, 225));//��RGB��ı���ɫ 
		//	pDC->SelectObject(&staticReHintfont);
		//}
		//else
		//{
		//	pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
		//	pDC->SelectObject(&staticReHintfont);
		//}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


/*��������*/
//ȷ����ť
void CMFCP4ECurrentToolDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

//�˳���ť
void CMFCP4ECurrentToolDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;
	nRes = MessageBox(_T("��ȷ��Ҫ�˳���"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;

	CDialogEx::OnCancel();
}




