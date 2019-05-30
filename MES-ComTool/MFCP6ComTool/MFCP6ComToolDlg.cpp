
// MFCP6ComToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP6ComTool.h"
#include "MFCP6ComToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMFCP6ComToolDlg �Ի���



CMFCP6ComToolDlg::CMFCP6ComToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP6ComToolDlg::IDD, pParent)
	, m_WithEnterCheckValue(FALSE)
	, m_OutputLocalLogCheckValue(FALSE)
	, m_HexDisplayCheckValue(FALSE)
	, m_HexSendCheckValue(FALSE)
	, m_IsAddTimeCheckValue(FALSE)
	, m_AutoSendTimeValue(0)
	, m_AutoSendCheckValue(FALSE)
	, m_ListControlDelayTimeiValue(0)
	, ListloopsendFlag(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	PortNo = "";
	PortBaud = "";
	m_PortRadioValue = 0;
	PortIsOpenFlag = FALSE;
	SettingSrc = ".\\";
	ListHeaderName[0] = _T("ID");
	ListHeaderName[1] = _T("������");
	ListHeaderName[2] = _T("ָ��");
	ListHeaderName[3] = _T("˳��");
	ListHeaderName[4] = _T("��ʱ");
}

void CMFCP6ComToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Information_RICHEDIT, m_InformationRicheditControl);
	DDX_Control(pDX, IDC_PortOrder_RICHEDIT, m_PortOrderRicheditControl);
	DDX_Control(pDX, IDC_PortNo_COMBO, m_PortNoComboControl);
	DDX_Control(pDX, IDC_PortBaud_COMBO, m_PortBaudComboControl);
	DDX_Radio(pDX, IDC_MtkPort_RADIO, m_PortRadioValue);
	DDX_Check(pDX, IDC_WithEnter_CHECK, m_WithEnterCheckValue);
	DDX_Check(pDX, IDC_OutputLocalLog_CHECK, m_OutputLocalLogCheckValue);
	DDX_Control(pDX, IDC_BLESETTING_LIST, m_BleSettingControl);
	DDX_Control(pDX, IDC_ListControl_EDIT, m_ListControlEdit);
	DDX_Control(pDX, IDC_LISTCONTROL_COMBO, m_ListControlCombo);
	DDX_Control(pDX, IDC_MtkPort_RADIO, m_MtkPortRadioControl);
	DDX_Control(pDX, IDC_RdaPort_RADIO, m_RdaPortRadioControl);
	DDX_Control(pDX, IDC_SendOrder_BUTTON, m_SendOrderControl);
	DDX_Control(pDX, IDC_ListLoopSend_BUTTON, m_ListLoopSendControl);
	DDX_Check(pDX, IDC_HexDisplay_CHECK, m_HexDisplayCheckValue);
	DDX_Check(pDX, IDC_HexSend_CHECK, m_HexSendCheckValue);
	DDX_Check(pDX, IDC_IsAddTime_CHECK, m_IsAddTimeCheckValue);
	DDX_Text(pDX, IDC_AutoSendTime_EDIT, m_AutoSendTimeValue);
	DDV_MinMaxInt(pDX, m_AutoSendTimeValue, 0, 999999);
	DDX_Check(pDX, IDC_AutoSend_CHECK, m_AutoSendCheckValue);
	DDX_Control(pDX, IDC_AutoSend_CHECK, m_AutoSendCheckControl);
	DDX_Text(pDX, IDC_ListControlDelayTime_EDIT, m_ListControlDelayTimeiValue);
	DDX_Control(pDX, IDC_ListControlDelayTime_EDIT, m_ListControlDelayTimeControl);
}

BEGIN_MESSAGE_MAP(CMFCP6ComToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP6ComToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP6ComToolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ClearInformation_BUTTON, &CMFCP6ComToolDlg::OnBnClickedClearinformationButton)
	ON_BN_CLICKED(IDC_OpenPort_BUTTON, &CMFCP6ComToolDlg::OnBnClickedOpenportButton)
	ON_BN_CLICKED(IDC_ClearSendOrder_BUTTON, &CMFCP6ComToolDlg::OnBnClickedClearsendorderButton)
	ON_BN_CLICKED(IDC_SendOrder_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSendorderButton)
	ON_CBN_DROPDOWN(IDC_PortNo_COMBO, &CMFCP6ComToolDlg::OnCbnDropdownPortnoCombo)
	ON_BN_CLICKED(IDC_MtkPort_RADIO, &CMFCP6ComToolDlg::OnBnClickedMtkportRadio)
	ON_BN_CLICKED(IDC_RdaPort_RADIO, &CMFCP6ComToolDlg::OnBnClickedMtkportRadio)
	ON_BN_CLICKED(IDC_WithEnter_CHECK, &CMFCP6ComToolDlg::OnBnClickedWithenterCheck)
	ON_BN_CLICKED(IDC_OutputLocalLog_CHECK, &CMFCP6ComToolDlg::OnBnClickedOutputlocallogCheck)
	ON_BN_CLICKED(IDC_SETTINGINSERT_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettinginsertButton)
	ON_BN_CLICKED(IDC_SETTINGUP_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingupButton)
	ON_BN_CLICKED(IDC_SETTINGDOWN_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingdownButton)
	ON_BN_CLICKED(IDC_SETTINGCOPY_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingcopyButton)
	ON_BN_CLICKED(IDC_SETTINGDELETE_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingdeleteButton)
	ON_BN_CLICKED(IDC_SETTINGSAVE_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingsaveButton)
	ON_NOTIFY(NM_DBLCLK, IDC_BLESETTING_LIST, &CMFCP6ComToolDlg::OnNMDblclkBlesettingList)
	ON_EN_KILLFOCUS(IDC_ListControl_EDIT, &CMFCP6ComToolDlg::OnEnKillfocusListcontrolEdit)
	ON_CBN_KILLFOCUS(IDC_LISTCONTROL_COMBO, &CMFCP6ComToolDlg::OnCbnKillfocusListcontrolCombo)
	ON_BN_CLICKED(IDC_SettingRead_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingreadButton)
	ON_NOTIFY(NM_CLICK, IDC_BLESETTING_LIST, &CMFCP6ComToolDlg::OnNMClickBlesettingList)
	ON_BN_CLICKED(IDC_ListLoopSend_BUTTON, &CMFCP6ComToolDlg::OnBnClickedListloopsendButton)
	ON_BN_CLICKED(IDC_HexDisplay_CHECK, &CMFCP6ComToolDlg::OnBnClickedHexdisplayCheck)
	ON_BN_CLICKED(IDC_HexSend_CHECK, &CMFCP6ComToolDlg::OnBnClickedHexsendCheck)
	ON_BN_CLICKED(IDC_IsAddTime_CHECK, &CMFCP6ComToolDlg::OnBnClickedIsaddtimeCheck)
	ON_BN_CLICKED(IDC_AutoSend_CHECK, &CMFCP6ComToolDlg::OnBnClickedAutosendCheck)
	ON_EN_KILLFOCUS(IDC_ListControlDelayTime_EDIT, &CMFCP6ComToolDlg::OnEnKillfocusListcontroldelaytimeEdit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BLESETTING_LIST, &CMFCP6ComToolDlg::OnNMCustomdrawBlesettingList)
	ON_EN_KILLFOCUS(IDC_AutoSendTime_EDIT, &CMFCP6ComToolDlg::OnEnKillfocusAutosendtimeEdit)
END_MESSAGE_MAP()


// CMFCP6ComToolDlg ��Ϣ�������

BOOL CMFCP6ComToolDlg::OnInitDialog()
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
	InitToolSetting();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCP6ComToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCP6ComToolDlg::OnPaint()
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
HCURSOR CMFCP6ComToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*��ʼ����غ���*/

//��ʼ�����
void  CMFCP6ComToolDlg::InitToolSetting()
{
	BOOL RdaFlag = RdaHostInterface.RDADllInit();
	if (RdaFlag == FALSE)
	{
		SetRicheditText("RDA��ʼ��ʧ��", 1);
	}
	LogName = GetLogTime() + "PortLog";
	SettingSrc = "";
	OnCbnDropdownPortnoCombo();
	InitBleSetting(&m_BleSettingControl);
	IniLoadConig();
	if (PortBaud == "")
	   m_PortBaudComboControl.SetWindowTextA("115200");
	UpdateData(FALSE);
	EnbleWindowIsOpenPort(PortIsOpenFlag);
}

//��ȡINI�ļ��Զ��������Ϣ
void  CMFCP6ComToolDlg::IniLoadConig()
{
	CFileFind finder;
	CString IniFileName = ".\\ComConfig.ini";
	CString ValueStr, SettringStr,SettringTitle = "Config";
	int ValueInt;

	BOOL ifFind = finder.FindFile(IniFileName);//�ȼ���ļ��治����
	if (ifFind)
	{
		ValueInt = GetPrivateProfileInt(SettringTitle, _T("ComType"), 0, IniFileName);
		m_PortRadioValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsLogCheck"), 0, IniFileName);
		m_OutputLocalLogCheckValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsHexDisplayCheck"), 0, IniFileName);
		m_HexDisplayCheckValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsHexSendCheck"), 0, IniFileName);
		m_HexSendCheckValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("AutoSendTime"), 0, IniFileName);
		m_AutoSendTimeValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsEnterCheck"), 0, IniFileName);
		m_WithEnterCheckValue = ValueInt;

		GetPrivateProfileString(SettringTitle, _T("PortBaud"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		PortBaud = ValueStr;
		ValueStr.ReleaseBuffer();

		GetDlgItem(IDC_PortBaud_COMBO)->SetWindowTextA(PortBaud);


		GetPrivateProfileString(SettringTitle, _T("PortOrder"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		GetDlgItem(IDC_PortOrder_RICHEDIT)->SetWindowTextA(ValueStr);

		GetPrivateProfileString(SettringTitle, _T("PortNo"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		PortNo = ValueStr;
		ValueStr.ReleaseBuffer();
		int FindInt=m_PortNoComboControl.FindString(0, PortNo);
		if (FindInt >= 0)
		{
			m_PortNoComboControl.SetCurSel(FindInt);
		}

		GetPrivateProfileString(SettringTitle, _T("SettingSrc"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		SettingSrc = ValueStr;
		ValueStr.ReleaseBuffer();
		ListSettingLoadFun(SettingSrc);
	}

}

//INI�ļ��Զ�����
void  CMFCP6ComToolDlg::IniSaveConig()
{
	UpdateData(TRUE);
	CFileFind finder;
	CString IniFileName = ".\\ComConfig.ini";
	CString ValueStr, SettringStr, SettringTitle="Config";
	int ValueInt;

	int nSel1;
	nSel1 = m_PortNoComboControl.GetCurSel();
	if (nSel1 >= 0)
	{
		m_PortNoComboControl.GetLBText(nSel1, PortNo);
	}
	m_PortBaudComboControl.GetWindowTextA(PortBaud);

	ValueStr.Format("%d", m_PortRadioValue);
	WritePrivateProfileString(SettringTitle, _T("ComType"), ValueStr, IniFileName);

	GetDlgItem(IDC_PortOrder_RICHEDIT)->GetWindowTextA(ValueStr);
	WritePrivateProfileString(SettringTitle, _T("PortOrder"), ValueStr, IniFileName);

	ValueStr = PortNo;
	WritePrivateProfileString(SettringTitle, _T("PortNo"), ValueStr, IniFileName);

	ValueStr = PortBaud;
	WritePrivateProfileString(SettringTitle, _T("PortBaud"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_OutputLocalLogCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsLogCheck"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_WithEnterCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsEnterCheck"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_HexDisplayCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsHexDisplayCheck"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_HexSendCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsHexSendCheck"), ValueStr, IniFileName);

	ValueStr = SettingSrc;
	WritePrivateProfileString(SettringTitle, _T("SettingSrc"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_AutoSendTimeValue);
	WritePrivateProfileString(SettringTitle, _T("AutoSendTime"), ValueStr, IniFileName);
}



/*����ؼ�����*/
//���������ǰ����Ϣ����
void CMFCP6ComToolDlg::OnBnClickedClearinformationButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_InformationRicheditControl.SetReadOnly(FALSE);
	m_InformationRicheditControl.SetSel(0,-1);
	m_InformationRicheditControl.Clear();
	m_InformationRicheditControl.SetReadOnly(TRUE);
}

//���ڽ����߳�
UINT static __cdecl OpenComReceive_Thread(LPVOID pParam)
{
	CMFCP6ComToolDlg* Mead_Main_Win = (CMFCP6ComToolDlg*)pParam;
	Mead_Main_Win->ReceiveComOrder(Mead_Main_Win->Porthandler);
	return 0;
}

//�򿪵�ǰѡ��Ĵ��ںź���
void CMFCP6ComToolDlg::OnBnClickedOpenportButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(FALSE);
	BOOL IsOpenFlag = TRUE;
	if (PortIsOpenFlag == FALSE)
	{
		IsOpenFlag=InitPort();
		if (IsOpenFlag == FALSE)
		{
			GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(TRUE);
			return;
		}
		PortIsOpenFlag = TRUE;
		GetDlgItem(IDC_OpenPort_BUTTON)->SetWindowTextA("�رն˿�");
		AfxBeginThread(OpenComReceive_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else if (PortIsOpenFlag == TRUE)
	{
		IsOpenFlag = CloseCom(Porthandler);
		if (IsOpenFlag == FALSE)
		{
			GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(TRUE);
			SetRicheditText("�رն˿�ʧ��", 1);
			return;
		}
		PortIsOpenFlag = FALSE;
		GetDlgItem(IDC_OpenPort_BUTTON)->SetWindowTextA("�򿪶˿�");
		SetRicheditText("�رն˿ڳɹ�", 0);
	}

	GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(TRUE);
	EnbleWindowIsOpenPort(PortIsOpenFlag);
}

//�����ǰָ���
void CMFCP6ComToolDlg::OnBnClickedClearsendorderButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_PortOrderRicheditControl.SetSel(0, -1);
	m_PortOrderRicheditControl.Clear();
}

//�������ָ�ť
void CMFCP6ComToolDlg::OnBnClickedSendorderButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString PortOrderStr;
	m_PortOrderRicheditControl.GetWindowTextA(PortOrderStr);

	SendComOrderHandleFun(PortOrderStr);
}

//���ڷ���ָ����ۺϴ�����
void CMFCP6ComToolDlg::SendComOrderHandleFun(CString PortOrder)
{
	//���û�й��ϲ����س����оͼ���\r\n
	if (m_WithEnterCheckValue == 0)
	{
		PortOrder += "\r\n";
	}

	SetRicheditText("��->��" + PortOrder, 0);

	if (m_PortRadioValue == 0)
	{
		SendComOrder(Porthandler, PortOrder);
	}
	else if (m_PortRadioValue == 1)
	{
		RdaHostInterface.RDAComWriteData(0, PortOrder);
	}
}

//���ں�������
void CMFCP6ComToolDlg::OnCbnDropdownPortnoCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FindCommPort(&m_PortNoComboControl, PortNo, 0);
}

void CMFCP6ComToolDlg::OnBnClickedMtkportRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedWithenterCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedOutputlocallogCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedHexdisplayCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedHexsendCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedIsaddtimeCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}


UINT static __cdecl AutoSend_Thread(LPVOID pParam)
{
	BOOL flag;

	CMFCP6ComToolDlg* Mead_Main_Win = (CMFCP6ComToolDlg*)pParam;
	Mead_Main_Win->AutoSendComOrder();
	return 0;
}

//��ʱ����
void CMFCP6ComToolDlg::OnBnClickedAutosendCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_AutoSendCheckValue == TRUE)
	{
		AfxBeginThread(AutoSend_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void CMFCP6ComToolDlg::AutoSendComOrder()
{
	while (m_AutoSendCheckValue)
	{
		CString PortOrderStr;
		m_PortOrderRicheditControl.GetWindowTextA(PortOrderStr);
		UpdateData(TRUE);
		Sleep(m_AutoSendTimeValue);
		SendComOrderHandleFun(PortOrderStr);
	}
}

//�Ӵ��ڿ�����񣬽����뿪����ؿؼ�
void CMFCP6ComToolDlg::EnbleWindowIsOpenPort(BOOL Chooes)
{
	m_PortNoComboControl.EnableWindow(!Chooes);
	m_PortBaudComboControl.EnableWindow(!Chooes);
	m_MtkPortRadioControl.EnableWindow(!Chooes);
	m_RdaPortRadioControl.EnableWindow(!Chooes);
	m_SendOrderControl.EnableWindow(Chooes);
	m_ListLoopSendControl.EnableWindow(Chooes);
	m_AutoSendCheckControl.EnableWindow(Chooes);
}


/*����ͨ�ò���*/
//��ע�����Ѱ���Ѿ�ע�ᴮ�ں�Ȼ��չʾ����
void CMFCP6ComToolDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO)
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
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // �򿪴���ע���
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // ö�ٴ���
			{
				break;
			}
			if (nSel >= 0 && ComNo == LPSTR(commName))//������ϴ�ѡ�������ǾͲ��õ�ǰѡ��䶯
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
			pComboBox->AddString(LPSTR(commName));
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//��ʼ�����ڼ��ɺ���
BOOL CMFCP6ComToolDlg::InitPort()
{
	BOOL FindFlag=GetCommPort(&m_PortNoComboControl, &m_PortBaudComboControl),InitFlag=TRUE;
	
	if (FindFlag == FALSE)
	{
		return FALSE;
	}

	if (m_PortRadioValue == 0)
	{
		Porthandler = InitCom(PortNo);
	}
	else if (m_PortRadioValue == 1)
	{
		InitFlag=RdaHostInterface.RDAComInit(0, PortNo);
		if (InitFlag == TRUE)
		{
			SetRicheditText("RDA���ڳ�ʼ���ɹ���", 0);
		}
	}

	if (Porthandler == NULL || InitFlag == FALSE)
	{
		SetRicheditText("���ڳ�ʼ��ʧ��!���鴮���Ƿ�ռ�ã�", 1);
		return FALSE;
	}

	return TRUE;
}

//��ʼ������
HANDLE CMFCP6ComToolDlg::InitCom(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile("\\\\.\\" + comName,//COM��
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
	dcb.BaudRate = atoi(PortBaud);
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	SetRicheditText("MTK���ڳ�ʼ���ɹ���", 0);
	return hCom;
}

//�رմ���
BOOL CMFCP6ComToolDlg::CloseCom(HANDLE handler)
{
	if (m_PortRadioValue == 0)
	{
		return CloseHandle(handler);
	}
	else if (m_PortRadioValue == 1)
	{
		return RdaHostInterface.RDAComShutdown(0);
	}
	return TRUE;
}

//��ȡ��ǰ���ں�
BOOL CMFCP6ComToolDlg::GetCommPort(CComboBox *pComboBox, CComboBox *pBaudBox)
{
	int nSel1;
	nSel1 = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel1, PortNo);

	if (PortNo == "")
	{
		MessageBox("����ѡ�񴮿ںţ�", "��ʾ��Ϣ", NULL);
		return FALSE;
	}

	pBaudBox->GetWindowTextA(PortBaud);

	if (PortBaud == "")
	{
		MessageBox("����ѡ�����ʣ�", "��ʾ��Ϣ", NULL);
		return FALSE;
	}
	return TRUE;
}

//CStringתByte
BYTE * CMFCP6ComToolDlg::CStrToByte(int len, CString str)
{
	BYTE *Buf = new BYTE[len];
	CString StrSub;
	for (int iSrc = 0, iDst = 0; iDst<len; iSrc += 2, iDst++)
	{
		StrSub = str.Mid(iSrc, 2);
		Buf[iDst] = (BYTE)_tcstoul(StrSub, NULL, 16);//16��ʮ�����Ƶ���˼
	}
	return Buf;
}

//���ڷ�ָ��
void  CMFCP6ComToolDlg::SendComOrder(HANDLE PortHandler, CString SendOrder)
{
	PurgeComm(PortHandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//���ڱ���
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat, bReadStatus;
	char *buffer;

	ClearCommError(PortHandler, &dwErrorFlags, &ComStat);

	if (m_HexSendCheckValue == FALSE)
	{
		buffer = SendOrder.GetBuffer(SendOrder.GetLength());
		bWriteStat = WriteFile(PortHandler, buffer, SendOrder.GetLength(), &dwBytesWrite, NULL);
	}
	else if (m_HexSendCheckValue == TRUE)
	{
		//��תʮ������
		SendOrder.Replace(" ", "");
		SendOrder.Replace("\r\n", "");
		int len = SendOrder.GetLength() / 2-1;
		BYTE *pBData = new BYTE[len];
		pBData = CStrToByte(len, SendOrder);

		//����ʮ����������
		bWriteStat = WriteFile(PortHandler, (LPVOID)pBData, len, &dwBytesWrite, NULL);
	}
	Sleep(1);
}

//�����յ��ظ�
void  CMFCP6ComToolDlg::ReceiveComOrder(HANDLE PortHandler)
{
	CString ReceiveData="";
	DWORD dwBytesRead;
	BOOL bReadStatus;
	char *buffer,buf[1024];
	while (PortIsOpenFlag)
	{
		if (m_PortRadioValue == 0)
		{
			memset(buf, 0, sizeof(buf));
			buffer = buf;
			bReadStatus = ReadFile(PortHandler, buffer, 512, &dwBytesRead, NULL);
			if (dwBytesRead != 0)
			{
				ReceiveData = buffer;
			}
		}
		else if (m_PortRadioValue == 1)
		{
			ReceiveData = RDAComReceive[0];
		}

		if (m_HexDisplayCheckValue == FALSE)
		{
			if (ReceiveData != "")
			{
				SetRicheditText("��<-��" + ReceiveData, 0);
			}
			ReceiveData = "";
			RDAComReceive[0] = "";
		}
		else if (m_HexDisplayCheckValue == TRUE)
		{
			if (ReceiveData != "")
			{
				CString charVtemp1, charVtemp2, charVtemp3;
				unsigned char *str = (unsigned char*)ReceiveData.GetBuffer(0);
				for (int i = 0; i < ReceiveData.GetLength(); i++)
				{
					charVtemp1.Format(_T("%02X "), (unsigned char)str[i]);
					charVtemp3 += charVtemp1;
				}
				SetRicheditText("��<-��" + charVtemp3, 0);
			}
			RDAComReceive[0] = "";
			ReceiveData = "";
		}
		Sleep(50);
	}	
}

//����ָ���շ�
void CMFCP6ComToolDlg::SendAndReceiveComOrder(HANDLE PortHandler, CString SendOrder, CString &ReceiveData, CString &IsSuccessFlag)
{
	PurgeComm(PortHandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//���ڱ���
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwBytesRead;
	BOOL bWriteStat, bReadStatus;
	char *buffer, buf[1024];

	ClearCommError(PortHandler, &dwErrorFlags, &ComStat);

	buffer = SendOrder.GetBuffer(SendOrder.GetLength());
	bWriteStat = WriteFile(PortHandler, buffer, SendOrder.GetLength(), &dwBytesWrite, NULL);

	for (int i = 0; i < 6; i++)
	{

		memset(buf, 0, sizeof(buf));
		buffer = buf;
		bReadStatus = ReadFile(PortHandler, buffer, 512, &dwBytesRead, NULL);
		if (dwBytesRead != 0)
		{
			ReceiveData = buffer;
			IsSuccessFlag = "Success";
			return;

		}
		else
		{
			Sleep(100 * i);
			continue;
		}
	}
	IsSuccessFlag = "Falied";
	return;
}


/*��Ϣ��־ģ�麯��*/
//��Ϣ��־���ĺ���
void CMFCP6ComToolDlg::SetRicheditText(CString strMsg, int No)
{
	m_InformationRicheditControl.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

	CString a = "��<-��";

	if (m_OutputLocalLogCheckValue == TRUE)
	{
		PrintLog(strMsg, 0);
	}

	if (m_IsAddTimeCheckValue == FALSE)
	{
		strMsg = GetTime() + _T("\r\n  ") + strMsg + _T("\r\n");
	}
	else if (m_IsAddTimeCheckValue == TRUE)
	{
		strMsg = strMsg.Right(strMsg.GetLength()-a.GetLength());
	}

	switch (No)
	{
	case 0:
		m_InformationRicheditControl.ReplaceSel(strMsg);
		m_InformationRicheditControl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
		break;
	case 1:
		m_InformationRicheditControl.SetSelectionCharFormat(cf);
		m_InformationRicheditControl.ReplaceSel(strMsg);
		m_InformationRicheditControl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
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
CString CMFCP6ComToolDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d.%03d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	return strTime;
}

//��ȡ��־ʱ��
CString CMFCP6ComToolDlg::GetLogTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}

//��־��ӡ����
void CMFCP6ComToolDlg::PrintLog(CString strMsg, int No)
{
	CLog Relog0;//��־�ļ�

	switch (No)
	{
	case 0:
		Relog0.WriteLog(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"), LogName);
		break;
	default:
		break;
	}
}


/*���ù���ģ��*/
//��ʼ�������б�
void CMFCP6ComToolDlg::InitBleSetting(CListCtrl *m_ListControl)
{
	//��ʼ���б�ؼ�
	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_ListControl->GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_ListControl->SetExtendedStyle(m_ListControl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//ɾ�������еı�ͷ
	while (1) {
		if (!m_ListControl->DeleteColumn(0)) {
			break;
		}
	}

	// Ϊ�б���ͼ�ؼ������
	m_ListControl->InsertColumn(0, ListHeaderName[0], LVCFMT_CENTER, rect.Width() / 12 * 1.2, 0);
	m_ListControl->InsertColumn(1, ListHeaderName[1], LVCFMT_CENTER, rect.Width() / 12 * 3, 0);
	m_ListControl->InsertColumn(2, ListHeaderName[2], LVCFMT_CENTER, rect.Width() / 12 * 5.3, 0);
	m_ListControl->InsertColumn(3, ListHeaderName[3], LVCFMT_CENTER, rect.Width() / 12 * 1.2, 0);
	m_ListControl->InsertColumn(4, ListHeaderName[4], LVCFMT_CENTER, rect.Width() / 12 * 1.5, 0);
}

//����������а�ť
void CMFCP6ComToolDlg::OnBnClickedSettinginsertButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingInsert(&m_BleSettingControl);
}

//��������ƶ���ť
void CMFCP6ComToolDlg::OnBnClickedSettingupButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingUp(&m_BleSettingControl);
}

//��������ƶ���ť
void CMFCP6ComToolDlg::OnBnClickedSettingdownButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingDown(&m_BleSettingControl);
}

//������ư�ť
void CMFCP6ComToolDlg::OnBnClickedSettingcopyButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingCopy(&m_BleSettingControl);
}

//���ɾ����ť
void CMFCP6ComToolDlg::OnBnClickedSettingdeleteButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingDelete(&m_BleSettingControl);
}

//������水ť
void CMFCP6ComToolDlg::OnBnClickedSettingsaveButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingSave(&m_BleSettingControl);
}

//�����ȡ��ť
void CMFCP6ComToolDlg::OnBnClickedSettingreadButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListSettingLoad(&m_BleSettingControl);
}

UINT static __cdecl ListLoop_Thread(LPVOID pParam)
{
	BOOL flag;

	CMFCP6ComToolDlg* Mead_Main_Win = (CMFCP6ComToolDlg*)pParam;
	Mead_Main_Win->ListLoopSendComOrder();
	return 0;
}

void CMFCP6ComToolDlg::ListLoopSendComOrder()
{
	CString SendCountStr,SendNo;
	int SendCountint = 0, SendCountTemp;
	while (ListloopsendFlag)
	{
		int SendCountTempArray[100];
		int ListItemCountInt = m_BleSettingControl.GetItemCount();
		SendCountTemp = 0;
		for (int i = 0; i < ListItemCountInt; i++)
		{
			SendCountStr = m_BleSettingControl.GetItemText(i, 3);
			if (SendCountStr == "0")
			{
				continue;
			}
			else
			{
				SendCountTempArray[SendCountTemp] = atoi(SendCountStr);
				SendCountTemp++;
			}
		}

		int SendCountCurrentTempInt=0;
		for (int i = 0; i < SendCountTemp; i++)
		{
			if (SendCountTempArray[i] > SendCountint&&i > 0 && SendCountTempArray[i] != 0 && SendCountTempArray[i] != SendCountint)
			{
				if (SendCountCurrentTempInt>SendCountTempArray[i])
					SendCountCurrentTempInt = SendCountTempArray[i];
				else if (SendCountCurrentTempInt==0)
					SendCountCurrentTempInt = SendCountTempArray[i];
			}
			else if (SendCountTempArray[i] > SendCountint&&i == 0 && SendCountTempArray[i] != 0 && SendCountTempArray[i] != SendCountint)
			{
				SendCountCurrentTempInt = SendCountTempArray[i];
			}
		}
		
		if (SendCountCurrentTempInt == SendCountint || SendCountCurrentTempInt==0)
		{
			SendCountint = 0;
			continue;
		}
		SendCountint=SendCountCurrentTempInt;

		CString PortOrderStr, PortDelayStr;
		int ColorInt;
		for (int i = 0; i < ListItemCountInt; i++)
		{
			SendNo = m_BleSettingControl.GetItemText(i, 3);
			m_BleSettingControl.SetItemData(ColorInt, 0);
			if (SendCountint == atoi(SendNo))
			{
				PortOrderStr = m_BleSettingControl.GetItemText(i, 2);
				PortDelayStr = m_BleSettingControl.GetItemText(i, 4);
				m_BleSettingControl.SetItemData(i, 1);
				ColorInt = i;
				SendComOrderHandleFun(PortOrderStr);
				Sleep(atoi(PortDelayStr));
			}
		}
		Sleep(10);
	}
}

//���˳���Ͱ�ť�������Ǹ��߳�
void CMFCP6ComToolDlg::OnBnClickedListloopsendButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_ListLoopSend_BUTTON)->EnableWindow(FALSE);
	if (ListloopsendFlag==FALSE)
	{
		ListloopsendFlag = TRUE;
		AfxBeginThread(ListLoop_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		GetDlgItem(IDC_ListLoopSend_BUTTON)->SetWindowTextA("ֹͣ����");
	}
	else if (ListloopsendFlag == TRUE)
	{
		ListloopsendFlag = FALSE;
		GetDlgItem(IDC_ListLoopSend_BUTTON)->SetWindowTextA("˳����");
	}
	GetDlgItem(IDC_ListLoopSend_BUTTON)->EnableWindow(TRUE);
}

//�����б��������
void CMFCP6ComToolDlg::ListSettingInsert(CListCtrl *m_ListControl)
{
	int SelectCount = 0;
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);

	int SelectMark = m_ListControl->GetSelectionMark();

	//�жϵ�ǰ�б�ı�ѡ�е�����
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			SelectCount++;
		}
	}

	//ûѡ�к�ѡ�ж���Ĭ�ϲ��뵽ĩβ��ѡ��һ����Ĭ�ϲ��뵽��һ��֮��
	if (SelectMark == -1 || SelectCount>1 || SelectCount == 0)
	{
		m_ListControl->InsertItem(ListItemCountInt, ListItemCountStr);

		//�ò����и���
		for (int i = 0; i < ListItemCountInt; i++)
		{
			m_ListControl->SetItemState(i, 0, -1);
		}
		m_ListControl->SetItemState(ListItemCountInt, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_ListControl->SetFocus();
		m_ListControl->SetSelectionMark(ListItemCountInt);
	}
	else if (SelectMark != -1 && SelectCount == 1)
	{
		m_ListControl->InsertItem(SelectMark + 1, ListItemCountStr);

		CString TempSelectMark;
		for (int i = SelectMark, j = 0; i <= ListItemCountInt; i++, j++)
		{
			TempSelectMark.Format("%d", SelectMark + j);
			m_ListControl->SetItemText(i, 0, TempSelectMark);
		}

		//�ò����и���
		m_ListControl->SetItemState(SelectMark, 0, -1);
		m_ListControl->SetItemState(SelectMark + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_ListControl->SetFocus();
		m_ListControl->SetSelectionMark(SelectMark + 1);
	}
	m_ListControl->SetItemText(SelectMark + 1, 3, "0");
	m_ListControl->SetItemText(SelectMark + 1, 4, "0");
}

//�����б�ѡ���������ƶ�
void CMFCP6ComToolDlg::ListSettingUp(CListCtrl *m_ListControl)
{
	m_ListControl->SetFocus();

	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	CString TempA, TempB;
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);
	int TopMark = 0;


	//�жϵ�ǰ���˿��ƶ���λ�ã�����0Ϊ��������ô���ƶ�λ��Ϊ1������0��1���Ǹ�������ô���ƶ�λ�þ���2
	for (int i = 0; i < ListItemCountInt; i++)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			if (i == TopMark)
			{
				TopMark++;
				continue;
			}
			else if (TopMark == ListItemCountInt)
			{
				return;
			}
			//�������н��л���
			for (int j = 1; j < Columns; j++)
			{
				TempA = m_ListControl->GetItemText(i - 1, j);
				TempB = m_ListControl->GetItemText(i, j);
				m_ListControl->SetItemText(i, j, TempA);
				m_ListControl->SetItemText(i - 1, j, TempB);
			}
			//���ƶ��и���
			m_ListControl->SetItemState(i, 0, -1);
			m_ListControl->SetItemState(i - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_ListControl->SetSelectionMark(i - 1);
		}
	}
	m_ListControl->SetFocus();
}

//�����б�ѡ���������ƶ�
void CMFCP6ComToolDlg::ListSettingDown(CListCtrl *m_ListControl)
{
	m_ListControl->SetFocus();

	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	CString TempA, TempB;
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);
	int BottomMark = ListItemCountInt - 1;

	//�жϵ�ǰ�׶˿��ƶ���λ�ã�ԭ��ο�����
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			if (i == BottomMark)
			{
				BottomMark--;
				continue;
			}
			else if (BottomMark == 0)
			{
				return;
			}
			//�������н��л���
			for (int j = 1; j < Columns; j++)
			{
				TempA = m_ListControl->GetItemText(i, j);
				TempB = m_ListControl->GetItemText(i + 1, j);
				m_ListControl->SetItemText(i + 1, j, TempA);
				m_ListControl->SetItemText(i, j, TempB);
			}
			//���ƶ��и���
			m_ListControl->SetItemState(i, 0, -1);
			m_ListControl->SetItemState(i + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_ListControl->SetSelectionMark(i + 1);
		}
	}
	m_ListControl->SetFocus();
}

//�����б���ѡ����
void CMFCP6ComToolDlg::ListSettingCopy(CListCtrl *m_ListControl)
{
	m_ListControl->SetFocus();

	int SelectCount = 0;
	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);

	//�жϵ�ǰ�б�ı�ѡ�е���������������+ԭʼ������Ϊ�����б���������
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			SelectCount++;
		}
	}

	//�Ը����н��и���
	for (int i = 0; i < ListItemCountInt + SelectCount; i++)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			m_ListControl->InsertItem(i + 1, m_ListControl->GetItemText(i, 0));

			for (int j = 1; j < Columns; j++)
			{
				m_ListControl->SetItemText(i + 1, j, m_ListControl->GetItemText(i, j));
			}
			//�ø��Ƴ�������һ�и���
			m_ListControl->SetItemState(i, 0, -1);
			m_ListControl->SetItemState(i + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			i++;//��Ϊ����һ�У�������ʱ��iҪ��1
			m_ListControl->SetSelectionMark(i);
		}
	}
	m_ListControl->SetFocus();

	//����������
	CString TempSelectMark;
	for (int i = 0; i <= m_ListControl->GetItemCount(); i++)
	{
		TempSelectMark.Format("%d", i);
		m_ListControl->SetItemText(i, 0, TempSelectMark);
	}
}

//�����б�ɾ��ѡ����
void CMFCP6ComToolDlg::ListSettingDelete(CListCtrl *m_ListControl)
{
	////��ѯ���Ƿ���Ҫɾ��
	//if (MessageBox(TEXT("��ȷ��Ҫɾ��������"), TEXT("��ʾ"), MB_OKCANCEL) != IDOK)
	//{
	//	return;
	//}

	m_ListControl->SetFocus();

	int DeleteCount = 0;
	int ListItemCountInt = m_ListControl->GetItemCount();

	//�жϵ�ǰ�б�ı�ѡ����Ȼ�����ɾ��
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			m_ListControl->DeleteItem(i);
			DeleteCount++;
		}
	}

	//���ûɾ�����ݾͲ�Ҫ����������
	if (DeleteCount == 0)
	{
		return;
	}

	//����������
	CString TempSelectMark;
	for (int i = 0; i <= m_ListControl->GetItemCount(); i++)
	{
		TempSelectMark.Format("%d", i);
		m_ListControl->SetItemText(i, 0, TempSelectMark);
	}
	m_ListControl->SetSelectionMark(-1);//Ȼ����������һ���б���
}

//�����б���
void CMFCP6ComToolDlg::ListSettingSave(CListCtrl *m_ListControl)
{
	//���ù�����
	TCHAR szFilter[] = _T("����ָ�������ļ�(*.ini)|*.ini|");
	//������ļ��Ի���
	CFileDialog fileDlg(FALSE, SettingSrc, NULL, 0, szFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����
		SettingSrc = fileDlg.GetPathName();
	}
	else
	{
		return;
	}

	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	LVCOLUMN col;//��ȡ���������õ�
	col.mask = LVCF_TEXT;

	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);

	CString ListControlSingleData = "";

	CFileFind finder;
	CString ValueStr, StrSetting = "ListSetting", StrValue;
	int ValueInt;

	BOOL ifFind = finder.FindFile(SettingSrc);
	if (ifFind)
	{
		DeleteFile(SettingSrc);
	}

	//ȡ������������д��INI�ļ���Ȼ���ٽ�ָ������д��INI�ļ���
	WritePrivateProfileString("Config", "SettingCount", ListItemCountStr, SettingSrc);

	for (int i = 0; i < ListItemCountInt; i++)
	{
		StrSetting.Format("ListSetting%d", i);
		for (int j = 0; j < Columns; j++)
		{
			//ListControlSingleData += ListHeaderName[j] + ":";
			ListControlSingleData += m_ListControl->GetItemText(i, j) + "#$&";
		}
		WritePrivateProfileString("Config", StrSetting, ListControlSingleData, SettingSrc);
		ListControlSingleData = "";
	}

	MessageBox("������ϣ�", "��ʾ");
}

//�����б��ȡ
void CMFCP6ComToolDlg::ListSettingLoad(CListCtrl *m_ListControl)
{

	int ISSaveFlag = m_ListControl->GetItemCount();

	if (ISSaveFlag != 0)
	{
		//��ѯ���Ƿ���Ҫ����
		if (MessageBox(TEXT("��Ҫ��Ҫ����������"), TEXT("��ʾ"), MB_OKCANCEL) != IDOK)
		{

		}
		else
		{
			ListSettingSave(m_ListControl);
		}
	}

	//���ù�����
	TCHAR szFilter[] = _T("��ȡָ�������ļ�(*.ini)|*.ini|");
	//������ļ��Ի���
	CFileDialog fileDlg(TRUE, SettingSrc, NULL, 0, szFilter, this);
	fileDlg.m_ofn.lpstrInitialDir = SettingSrc;
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����
		SettingSrc = fileDlg.GetPathName();
	}
	else
	{
		return;
	}

	CString ListControlSingleData = "";
	int ListSettingCount=0;

	CFileFind finder;
	CString ValueStr, StrSetting = "ListSetting", StrValue;
	int ValueInt;

	BOOL ifFind = finder.FindFile(SettingSrc);
	if (ifFind)
	{
		ListSettingLoadFun(SettingSrc);
	}
}

BOOL CMFCP6ComToolDlg::ListSettingLoadFun(CString SettingSrc)
{
	CString ListControlSingleData = "";
	int ListSettingCount = 0;
	CString ValueStr, StrSetting = "ListSetting", StrValue;
	int ValueInt;
	ListSettingCount = GetPrivateProfileInt("Config", "SettingCount", -1, SettingSrc);

	if (SettingSrc == "")
	{
		return FALSE;
	}

	if (ListSettingCount == -1)
	{
		MessageBox("�����ļ����𻵣���ѡ�����������ļ���", "���棡");
		return FALSE;
	}
	m_BleSettingControl.DeleteAllItems();

	for (int i = 0; i < ListSettingCount; i++)
	{
		CString SettingCut;
		StrSetting.Format("ListSetting%d", i);
		GetPrivateProfileString("Config", StrSetting, _T(""), ListControlSingleData.GetBuffer(128), 128, SettingSrc);
		ListControlSingleData.ReleaseBuffer();

		SettingCut = ListControlSingleData.Left(ListControlSingleData.Find("#$&"));
		ListControlSingleData = ListControlSingleData.Right(ListControlSingleData.GetLength() - ListControlSingleData.Find("#$&") - 3);
		m_BleSettingControl.InsertItem(i, SettingCut);

		for (int j = 1; j < 5; j++)
		{
			SettingCut = ListControlSingleData.Left(ListControlSingleData.Find("#$&"));
			ListControlSingleData = ListControlSingleData.Right(ListControlSingleData.GetLength() - ListControlSingleData.Find("#$&") - 3);
			m_BleSettingControl.SetItemText(i, j, SettingCut);
		}
		ListControlSingleData = "";
	}
	return TRUE;
}

//˫���б�ʱ�������¼�
void CMFCP6ComToolDlg::OnNMDblclkBlesettingList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString PortOrderStr;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//��ѡ���е��к�

	if (m_Row == -1)
		return;
	
	m_Colum = pNMListView->iSubItem;//��ѡ���е��к�

	switch (pNMListView->iSubItem)
	{
	case 0:
		//if (PortIsOpenFlag)
		//{
		//	PortOrderStr = m_BleSettingControl.GetItemText(m_Row, 2);
		//	SendComOrderHandleFun(PortOrderStr);
		//}
		break;
	case 3:
	case 4:
		m_BleSettingControl.GetSubItemRect(m_Row, m_Colum, LVIR_LABEL, rc);//��ȡ������RECT��λ��
		m_ListControlDelayTimeControl.SetParent(&m_BleSettingControl);//ת������Ϊ�б���е�����
		m_ListControlDelayTimeControl.MoveWindow(rc);//�ƶ�Eidt�༭��RECT���ڵ�λ��
		m_ListControlDelayTimeControl.SetWindowText(m_BleSettingControl.GetItemText(m_Row, m_Colum));//���������������ʾ���༭����
		m_ListControlDelayTimeControl.ShowWindow(SW_SHOW);//��ʾEdit�ؼ�
		m_ListControlDelayTimeControl.SetFocus();//����Edit����
		m_ListControlDelayTimeControl.ShowCaret();//��ʾ���
		m_ListControlDelayTimeControl.SetSel(-1);//������ƶ������
		break;
	default:
		m_BleSettingControl.GetSubItemRect(m_Row, m_Colum, LVIR_LABEL, rc);//��ȡ������RECT��λ��
		m_ListControlEdit.SetParent(&m_BleSettingControl);//ת������Ϊ�б���е�����
		m_ListControlEdit.MoveWindow(rc);//�ƶ�Eidt�༭��RECT���ڵ�λ��
		m_ListControlEdit.SetWindowText(m_BleSettingControl.GetItemText(m_Row, m_Colum));//���������������ʾ���༭����
		m_ListControlEdit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ�
		m_ListControlEdit.SetFocus();//����Edit����
		m_ListControlEdit.ShowCaret();//��ʾ���
		m_ListControlEdit.SetSel(-1);//������ƶ������
		break;
	}

	//if (pNMListView->iSubItem != 0)
	//{
	//	m_BleSettingControl.GetSubItemRect(m_Row, m_Colum, LVIR_LABEL, rc);//��ȡ������RECT��λ��
	//	m_ListControlEdit.SetParent(&m_BleSettingControl);//ת������Ϊ�б���е�����
	//	m_ListControlEdit.MoveWindow(rc);//�ƶ�Eidt�༭��RECT���ڵ�λ��
	//	m_ListControlEdit.SetWindowText(m_BleSettingControl.GetItemText(m_Row,m_Colum));//���������������ʾ���༭����
	//	m_ListControlEdit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ�
	//	m_ListControlEdit.SetFocus();//����Edit����
	//	m_ListControlEdit.ShowCaret();//��ʾ���
	//	m_ListControlEdit.SetSel(-1);//������ƶ������
	//}

	*pResult = 0;
}

//����������
void CMFCP6ComToolDlg::OnNMClickBlesettingList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString PortOrderStr;

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//��ѡ���е��к�

	if (m_Row == -1)
		return;


	m_Colum = pNMListView->iSubItem;//��ѡ���е��к�

	if (m_Colum == 5)
		return;

	switch (pNMListView->iSubItem)
	{
	case 0:
		if (PortIsOpenFlag)
		{
			PortOrderStr = m_BleSettingControl.GetItemText(m_Row, 2);
			SendComOrderHandleFun(PortOrderStr);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CMFCP6ComToolDlg::OnNMCustomdrawBlesettingList(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NMCUSTOMDRAW nmCustomDraw = pNMCD->nmcd;

	switch (nmCustomDraw.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	{
		if (0 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 255, 255);
			pNMCD->clrText = RGB(0, 0, 0);
		}
		else if (1 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 0, 0);		//������ɫ
			pNMCD->clrText = RGB(255, 255, 255);		//������ɫ
		}
		else if (2 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(51, 153, 255);
			pNMCD->clrText = RGB(255, 255, 255);
		}
		else
		{
			//
		}
		break;
	}
	default:
	{
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//�����У���Ȼ��û��Ч��
	*pResult |= CDRF_NOTIFYITEMDRAW;		//�����У���Ȼ��û��Ч��
	return;
}

void CMFCP6ComToolDlg::OnEnKillfocusAutosendtimeEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str, judge;
	GetDlgItem(IDC_AutoSendTime_EDIT)->GetWindowText(str);//ȡ�ñ༭�������
	judge = str;
	judge.Trim("0123546789");
	if (judge != "")
	{
		UpdateData(FALSE);
		MessageBox("��������Ч����", NULL);
		return;
	}
	UpdateData(TRUE);
}

//�ƶ���꽹���
void CMFCP6ComToolDlg::OnEnKillfocusListcontrolEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_ListControlEdit.GetWindowText(str);//ȡ�ñ༭�������
	m_BleSettingControl.SetItemText(m_Row, m_Colum, str);//�������ݸ��µ��б��Ӧ������
	m_ListControlEdit.ShowWindow(SW_HIDE);//���ر༭��
}

//�ƶ���꽹���
void CMFCP6ComToolDlg::OnCbnKillfocusListcontrolCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_ListControlCombo.GetWindowText(str);//ȡ�ñ༭�������
	m_BleSettingControl.SetItemText(m_Row, m_Colum, str);//�������ݸ��µ��б��Ӧ������
	m_ListControlCombo.ShowWindow(SW_HIDE);//���ر༭��
}

//�ƶ���꽹���
void CMFCP6ComToolDlg::OnEnKillfocusListcontroldelaytimeEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str,judge;
	m_ListControlDelayTimeControl.GetWindowText(str);//ȡ�ñ༭�������
	judge = str;
	judge.Trim("0123546789");
	if (judge != "")
	{
		m_ListControlDelayTimeControl.ShowWindow(SW_HIDE);//���ر༭��
		MessageBox("��������Ч����",NULL);
		return;
	}

	m_BleSettingControl.SetItemText(m_Row, m_Colum, str);//�������ݸ��µ��б��Ӧ������
	m_ListControlDelayTimeControl.ShowWindow(SW_HIDE);//���ر༭��
}


/*��������*/
void CMFCP6ComToolDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

void CMFCP6ComToolDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int ISSaveFlag = m_BleSettingControl.GetItemCount();

	if (ISSaveFlag != 0)
	{
		//��ѯ���Ƿ���Ҫ����
		if (MessageBox(TEXT("��Ҫ��Ҫ����������"), TEXT("��ʾ"), MB_OKCANCEL) != IDOK)
		{

		}
		else
		{
			ListSettingSave(&m_BleSettingControl);
		}
	}

	INT_PTR nRes;
	nRes = MessageBox(_T("��ȷ��Ҫ�˳���"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;

	IniSaveConig();

	CDialogEx::OnCancel();
}


