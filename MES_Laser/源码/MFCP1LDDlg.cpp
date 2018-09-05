
// MFCP1LDDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP1LD.h"
#include "MFCP1LDDlg.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "DBConfig.h"
#include "SetImeiRangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

volatile BOOL m_LdPortRun;//������ȫ�ֱ�����ר�������̵߳�

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


// CMFCP1LDDlg �Ի���

CMFCP1LDDlg::CMFCP1LDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP1LDDlg::IDD, pParent)
	, m_pcnameEdit(_T(""))
	, m_pcipEdit(_T(""))
	, radioflag(TRUE)
	, STEPLDFlag(1)
	, ScanFlag(1)
	, IMEI15Flag(1)
	, m_pCLDStartDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
	//�Ƶ���ȫ�ֱ���
	ZhiDanNO = _T("");

	//IMEIȫ�ֱ���
	IMEIStaStart = _T("");
	IMEIStaEnd = _T("");
	IMEIStaCRStart = _T("");
	IMEIStaCREnd = _T("");

	//���ص��ֶεȸ���ʼֵ
	LdVariableChar[0] = _T("IMEI");
	LdVariableChar[1] = _T("SN");
	LdVariableChar[2] = _T("SIM");
	LdVariableChar[3] = _T("VIP");
	LdVariableChar[4] = _T("ICCID");
	LdVariableChar[5] = _T("BAT");
	LdVariableChar[6] = _T("MAC");
	LdVariableChar[7] = _T("Equipment number");

	for (int i = 0; i < 8; i++)
	{
	LdVariableInt[i] = -1;
	}
	LdVariableInt[0] = 0;
	LdVariableCount = 1;
}

CMFCP1LDDlg::~CMFCP1LDDlg()
{
	// �����ģ̬�Ի����Ѿ�������ɾ����   
	if (NULL != m_pCLDStartDlg)
	{
		// ɾ����ģ̬�Ի������   
		delete m_pCLDStartDlg;
	}
}

void CMFCP1LDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZHIDANAUTO_COMBO, m_zhidanautoCombo);
	DDX_Control(pDX, IDC_DBCURTAIN_LIST, m_dbcurtainList);
	DDX_Control(pDX, IDC_CURRENTTCP_RICHEDIT, m_currentRichedit);
	DDX_Control(pDX, IDC_IMEISTART_EDIT, m_imeistartEdit);
	DDX_Control(pDX, IDC_IMEIEND_EDIT, m_imeiendEdit);
	DDX_Control(pDX, IDC_IMEISTARTSHOWONLY_EDIT, m_imeistartshowonlyEdit);
	DDX_Control(pDX, IDC_IMEIENDSHOWONLY_EDIT, m_imeiendshowonlyEdit);
}

BEGIN_MESSAGE_MAP(CMFCP1LDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP1LDDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP1LDDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP1LDDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_SELECTMODEL_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectmodleButton)
	ON_BN_CLICKED(IDC_OPENMODEL_BUTTON, &CMFCP1LDDlg::OnBnClickedOpenmodleButton)
	ON_BN_CLICKED(IDC_UPDATEZHIDAN_BUTTON, &CMFCP1LDDlg::OnBnClickedUpdatezhidanButton)
	ON_CBN_KILLFOCUS(IDC_ZHIDANAUTO_COMBO, &CMFCP1LDDlg::OnCbnKillfocusZhidanautoCombo)
	ON_BN_CLICKED(IDC_NORMOLLD_RADIO, &CMFCP1LDDlg::OnBnClickedNormolldRadio)
	ON_BN_CLICKED(IDC_RELD_RADIO, &CMFCP1LDDlg::OnBnClickedReldRadio)
	ON_CBN_SELCHANGE(IDC_ZHIDANAUTO_COMBO, &CMFCP1LDDlg::OnCbnSelchangeZhidanautoCombo)
	ON_BN_CLICKED(IDC_READCURRENTIMEI_BUTTON, &CMFCP1LDDlg::OnBnClickedReadcurrentimeiButton)
	ON_BN_CLICKED(IDC_READIMEI_BUTTON, &CMFCP1LDDlg::OnBnClickedReadimeiButton)
	ON_EN_KILLFOCUS(IDC_IMEISTART_EDIT, &CMFCP1LDDlg::OnEnKillfocusImeistartEdit)
	ON_EN_KILLFOCUS(IDC_IMEIEND_EDIT, &CMFCP1LDDlg::OnEnKillfocusImeiendEdit)
	ON_BN_CLICKED(IDC_SELECTALLDATA_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectalldataButton)
	ON_BN_CLICKED(IDC_SELECTREPETDATA_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectrepetdataButton)
	ON_BN_CLICKED(IDC_SELECTMISSINGDATA_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectmissingdataButton)
	ON_BN_CLICKED(IDC_OPENLDSYSTEM_BUTTON, &CMFCP1LDDlg::OnBnClickedOpenldsystemButton)
	ON_BN_CLICKED(IDC_CLOSELDSYSTEM_BUTTON, &CMFCP1LDDlg::OnBnClickedCloseldsystemButton)
	ON_BN_CLICKED(IDC_LDONECE_BUTTON, &CMFCP1LDDlg::OnBnClickedLdoneceButton)
	ON_BN_CLICKED(IDC_STEPLD_BUTTON, &CMFCP1LDDlg::OnBnClickedStepldButton)
	ON_BN_CLICKED(IDC_IMEI_CHECK, &CMFCP1LDDlg::OnBnClickedImeiCheck)
	ON_BN_CLICKED(IDC_SCANMODE_BUTTON, &CMFCP1LDDlg::OnBnClickedScanmodeButton)
	ON_BN_CLICKED(IDC_PARITY_CHECK, &CMFCP1LDDlg::OnBnClickedParityCheck)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SETIMEIRANGE_BUTTON, &CMFCP1LDDlg::OnBnClickedSetimeirangeButton)
END_MESSAGE_MAP()


// CMFCP1LDDlg ��Ϣ�������

BOOL CMFCP1LDDlg::OnInitDialog()
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
	/*
	//��ȡģ���·��
	CString str;
	GetPrivateProfileString(_T("ModelPathName"), _T("modelpathname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	SetDlgItemText(IDC_MODELPATH_EDIT, str);
	str.ReleaseBuffer();
    */

	//��ʼ��������Ϣ
	GetLocalHostIPName(m_pcnameEdit, m_pcipEdit);//���ú������IP�ͼ��������
	SetDlgItemText(IDC_PCIP_EDIT, m_pcipEdit);//���������������ʾ�����ƿؼ���
	SetDlgItemText(IDC_PCNAME_EDIT, m_pcnameEdit);//���������IP��ʾ��IP�ؼ���

	//��ʼ������
	InitComboBox();
	
	//��ʼ���ؼ�
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(FALSE);
	CRLDInformationwindowfalse();
	LDSystemInformationwindowfalse();
	CheckDlgButton(IDC_IMEI_CHECK, 1);
	CheckDlgButton(IDC_NORMOLLD_RADIO, 1);
	CheckDlgButton(IDC_PARITY_CHECK, 1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCP1LDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCP1LDDlg::OnPaint()
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
HCURSOR CMFCP1LDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���ƴ����пؼ����ʹ�ܺ�ʧȥʹ�ܵĺ���
void CMFCP1LDDlg::InitInformationwindowtrue()
{
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(TRUE);
}

void CMFCP1LDDlg::CRLDInformationwindowtrue()
{
	GetDlgItem(IDC_IMEISTART_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEIEND_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_READCURRENTIMEI_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_READIMEI_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENLDSYSTEM_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTMODEL_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENMODEL_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTALLDATA_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTREPETDATA_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTMISSINGDATA_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SETIMEIRANGE_BUTTON)->EnableWindow(TRUE);
}

void CMFCP1LDDlg::LDSystemInformationwindowtrue()
{
	GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_UPDATEZHIDAN_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZHIDANAUTO_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(FALSE);
}

void CMFCP1LDDlg::InitInformationwindowfalse()
{
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);
}

void CMFCP1LDDlg::CRLDInformationwindowfalse()
{
	GetDlgItem(IDC_IMEISTART_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IMEIEND_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_READCURRENTIMEI_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_READIMEI_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENLDSYSTEM_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTMODEL_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENMODEL_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTALLDATA_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTREPETDATA_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTMISSINGDATA_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SETIMEIRANGE_BUTTON)->EnableWindow(FALSE);
}

void CMFCP1LDDlg::LDSystemInformationwindowfalse()
{
	GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_UPDATEZHIDAN_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_ZHIDANAUTO_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(TRUE);
}


//��ʼ������ģ��
//���ݿ����ð�ť
void CMFCP1LDDlg::OnBnClickedDbconfigButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

	CDBconfig dbconfigDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = dbconfigDlg.DoModal();  // �����Ի���

	if (IDCANCEL == nRes)
		return;
}

//ѡ��ģ�水ť
void CMFCP1LDDlg::OnBnClickedSelectmodleButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���ù�����
	TCHAR szFilter[] = _T("�ص�ģ���ļ�(*.ezd)|*.ezd|�����ļ�(*.*)|*.*||");
	//������ļ��Ի���
	CFileDialog fileDlg(TRUE, _T("ezd"), NULL, 0, szFilter, this);

	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_MODELPATH_EDIT, strFilePath);

		//��·����Ϣ���浽��Ϣ�ļ���
		//WritePrivateProfileString(_T("ModelPathName"), _T("modelpathname"), strFilePath, _T(".\\SystemInfo.ini"));
	}
}

//��ģ�水ť
void CMFCP1LDDlg::OnBnClickedOpenmodleButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;

	GetDlgItemText(IDC_MODELPATH_EDIT, strFilePath);
	ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOWNORMAL);
}

//��combox������ݿ��еĶ����ţ����°�ť�Ϳ������Զ���ʼ�������������
void CMFCP1LDDlg::InitComboBox()
{
	m_zhidanautoCombo.ResetContent();
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetZhiDan();
	while (!adomanage.m_pRecordSet->adoEOF)
	{
		m_zhidanautoCombo.AddString(adomanage.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanage.m_pRecordSet->MoveNext();
	}
	adomanage.CloseAll();
}

//�����Ƶ��Ż�ȡIMEI��ģ��·��
void CMFCP1LDDlg::GetImeiByZhidan()
{
	_variant_t imeitemp;//������getcollect������
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetIMEIByZhiDan(ZhiDanNO);
	SetDlgItemText(IDC_MODELPATH_EDIT, adomanage.m_pRecordSet->GetCollect("LD_template1").bstrVal);
	IMEIStaStart = adomanage.m_pRecordSet->GetCollect("IMEIStart").bstrVal;
	IMEIStaEnd = adomanage.m_pRecordSet->GetCollect("IMEIEnd").bstrVal;
	SetDlgItemText(IDC_IMEISTARTSHOWONLY_EDIT, IMEIStaStart);
	SetDlgItemText(IDC_IMEIENDSHOWONLY_EDIT, IMEIStaEnd);
	SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaStart);
	SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaEnd);

	//��_variant_t���Ͳ��ܼ����ж�����ֶ��Ƿ�Ϊ��
	imeitemp = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent");
	
	//������һ���ص����ΪNULL�ͽ���ʼ�ŷŵ���ǰ�ص�����У������Ϊ�վͽ����һ���ص�ŷ��뵽��ǰ�ص�������ʼ����
	if (imeitemp.vt == VT_NULL)
	{
		IMEIStaCRStart = IMEIStaStart;
		IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaStart);
	}
	else
	{
		IMEIStaCRStart = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent").bstrVal;
        IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);
	}

	adomanage.CloseAll();
}

//�����Ƶ��Ű�ť
void CMFCP1LDDlg::OnBnClickedUpdatezhidanButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_IMEISTARTSHOWONLY_EDIT, L"");
	SetDlgItemText(IDC_IMEIENDSHOWONLY_EDIT, L"");
	SetDlgItemText(IDC_MODELPATH_EDIT, L"");
	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	SetDlgItemText(IDC_IMEIEND_EDIT, L"");
	SetDlgItemText(IDC_CURRENTIMEI_EDIT, L"");
	SetDlgItemText(IDC_IMEINUMBER_EDIT, L"");
	SetDlgItemText(IDC_HDIMEINUMBER_EDIT, L"");
	SetDlgItemText(IDC_HDREIMEINUMBER_EDIT, L"");
	SetDlgItemText(IDC_SCANLD_EDIT, L"");

	InitComboBox();

	GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);

	CRLDInformationwindowfalse();
	LDSystemInformationwindowfalse();
}

//����ѡ�񶩵��ź�������Ϊ
void CMFCP1LDDlg::ZhiDanControl()
{
	//��ȡѡ��
	int nSel;
	nSel = m_zhidanautoCombo.GetCurSel();
	m_zhidanautoCombo.GetLBText(nSel, ZhiDanNO);
	SetDlgItemText(IDC_ZHIDANAUTO_COMBO, ZhiDanNO);//ѡ�е���һ˲����û�м�ʱ���µ��༭��ģ����Ե��Ȼ�ȡ��ʱ���ݺ���µ��༭����

	//�Ƶ��Ų����ھͽ���ĳЩ���ڲ��ҽ�LdVariable���ã��л��Ƶ���Ҳ����LdVariable
	if (JudgeZhidan(ZhiDanNO))
	{
		MessageBox(L"�Ƶ��Ų����ڣ�", L"��ʾ��Ϣ", NULL);
		SetDlgItemText(IDC_IMEISTARTSHOWONLY_EDIT, L"");
		SetDlgItemText(IDC_IMEIENDSHOWONLY_EDIT, L"");
		SetDlgItemText(IDC_MODELPATH_EDIT, L"");
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIEND_EDIT, L"");
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, L"");
		SetDlgItemText(IDC_IMEINUMBER_EDIT, L"");
		SetDlgItemText(IDC_HDIMEINUMBER_EDIT, L"");
		SetDlgItemText(IDC_HDREIMEINUMBER_EDIT, L"");
		SetDlgItemText(IDC_SCANLD_EDIT, L"");

		InitComboBox();

		GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_RELD_RADIO, 0);
		CheckDlgButton(IDC_IMEI_CHECK, 1);
		CheckDlgButton(IDC_NORMOLLD_RADIO, 1);
		CheckDlgButton(IDC_PARITY_CHECK, 1);

		CRLDInformationwindowfalse();
		LDSystemInformationwindowfalse();

		for (int i = 0; i < 8; i++)
		{
			LdVariableInt[i] = -1;
		}

		LdVariableInt[0] = 0;
		LdVariableCount = 1;

		return;
	}
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(TRUE);
	CheckDlgButton(IDC_RELD_RADIO, 0);
	CheckDlgButton(IDC_IMEI_CHECK, 1);
	CheckDlgButton(IDC_NORMOLLD_RADIO, 1);
	CheckDlgButton(IDC_PARITY_CHECK, 1);

	GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(TRUE);

	for (int i = 0; i < 8; i++)
	{
		LdVariableInt[i] = -1;
	}
	LdVariableInt[0] = 0;
	LdVariableCount = 1;

	CRLDInformationwindowtrue();
	GetImeiByZhidan();
	GetDataNumber();
}

//ѡ���Ƶ��ź����������
void CMFCP1LDDlg::OnCbnSelchangeZhidanautoCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ZhiDanControl();
}

//�Ƶ��ſ�ʧȥ��������������
void CMFCP1LDDlg::OnCbnKillfocusZhidanautoCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ZhiDanControl();
}

//�ж��Ƶ����Ƿ����
BOOL CMFCP1LDDlg::JudgeZhidan(CString zhidan)
{
	_RecordsetPtr recordzhidan;
	int flag = 1;
	ADOManage adomanage;
	adomanage.ConnSQL();
	recordzhidan = adomanage.JudgeZhiDan(zhidan);
	if (!recordzhidan->adoEOF)
	{
		flag = 0;
	}
	adomanage.CloseAll();
	return flag;
}

//��ȡ����IP�͵�ַ
int CMFCP1LDDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
{
	char szLocalName[256];//����
	WSADATA wsaData;//�����׽��ִ洢����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)//<span class="con">windows��ʼ��socket����⣬����2��0�İ汾��windows socket��̱�<img class="selectsearch-hide" id="selectsearch-icon" alt="����" src="http://img.baidu.com/img/iknow/qb/select-search.png" />���ȳ�ʼ�����������,�򷵻�0</span>
	{
		if (gethostname(szLocalName, sizeof(szLocalName)) != 0)//��������
		{
			sLocalName = _T("û��ȡ��");
			WSACleanup();
			return GetLastError();
		}
		else
		{
			sLocalName = szLocalName;//��sLocalName�����洢��õļ��������
			struct hostent FAR * lpHostEnt = gethostbyname(CStringA(sLocalName));
			if (lpHostEnt == NULL)//����
			{
				sIpAddress = _T("");
				WSACleanup();
				return GetLastError();
			}
			LPSTR lpAddr = lpHostEnt->h_addr_list[0];//��ȡIP��ַ
			if (lpAddr)
			{
				struct in_addr inAddr;
				memmove(&inAddr, lpAddr, 4);
				sIpAddress = inet_ntoa(inAddr);//ת��Ϊ��׼��ʽ
				if (sIpAddress.IsEmpty())
				{
					sIpAddress = _T("û��ȡ��");
					WSACleanup();
					return 1;
				}
			}
		}
	}
	WSACleanup();//����׽��ֳ�ʼ������
	return 0;
}


//�ص�ģʽѡ��ģ��
//��������ص�ģʽ
void CMFCP1LDDlg::OnBnClickedNormolldRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	radioflag = TRUE;
}

//��������ص�ģʽ
void CMFCP1LDDlg::OnBnClickedReldRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	radioflag = FALSE;
}

//���IMEI��ѡ��
void CMFCP1LDDlg::OnBnClickedImeiCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���IMEI��ѡ��ȡ������LdVariableInt�д���IMEI��0��־��Ϊ-1�������ѡ������VariableInt����������0��ȥ
	if (!((CButton *)GetDlgItem(IDC_IMEI_CHECK))->GetCheck())
	{
		for (int i = 0; i < 8; i++)
		{
			if (LdVariableInt[i] == 0)
			{
				LdVariableInt[i] = -1;
				LdVariableCount--;
				break;
			}
			i++;
		}
	}
	else
	{
		for (int i = 0; i < 8;i++)
		{
			if (LdVariableInt[i] == -1)
			{
				LdVariableInt[i] = 0;
				LdVariableCount++;
				break;
			}
			i++;
		}
	}
}

//�������IMEIУ��λ��ѡ��
void CMFCP1LDDlg::OnBnClickedParityCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�������IMEIУ��λ��ѡ��ȡ������IMEI15Flag��־��Ϊ0�������ѡ����Ϊ1
	if (!((CButton *)GetDlgItem(IDC_PARITY_CHECK))->GetCheck())
	{
		IMEI15Flag = 0;
	}
	else
	{
		IMEI15Flag = 1;
	}
}


//��ǰ�ص���Ϣģ��
//�������һ�ص��
void CMFCP1LDDlg::OnBnClickedReadcurrentimeiButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	_variant_t imeitemp;//������getcollect������
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetIMEIByZhiDan(ZhiDanNO);

	//��_variant_t���Ͳ��ܼ����ж�����ֶ��Ƿ�Ϊ��
	imeitemp = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent");

	//������һ���ص����ΪNULL�ͽ���ʼ�ŷŵ���ǰ�ص�����У������Ϊ�վͽ����һ���ص�ŷ��뵽��ǰ�ص�������ʼ����
	if (imeitemp.vt == VT_NULL)
	{
		IMEIStaCRStart = IMEIStaStart;
		IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaStart);
	}
	else
	{
		IMEIStaCRStart = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent").bstrVal;
		IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);
	}
	GetDataNumber();
	adomanage.CloseAll();
}

//������ã��򽫵�ǰ�ص���Ϣ�е�IMEI��ʼ�źͽ���������Ϊ�Ƶ�����ԭ����IMEI�ź���ʼ��
void CMFCP1LDDlg::OnBnClickedReadimeiButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetIMEIByZhiDan(ZhiDanNO);
	IMEIStaStart = adomanage.m_pRecordSet->GetCollect("IMEIStart").bstrVal;
	IMEIStaEnd = adomanage.m_pRecordSet->GetCollect("IMEIEnd").bstrVal;
	SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaStart);
	SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaEnd);
	SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaStart);
	IMEIStaCRStart = IMEIStaStart;
	IMEIStaCREnd = IMEIStaEnd;
	GetDataNumber();
	adomanage.CloseAll();
}

//�ж�IMEI��ʼ���Ƿ���ȷ
void CMFCP1LDDlg::OnEnKillfocusImeistartEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str1, str2, str3;

	unsigned long long a, b;
	GetDlgItemText(IDC_IMEISTART_EDIT, str1);
	GetDlgItemText(IDC_IMEIEND_EDIT, str2);
	if (str2 == "")
	{
		return;
	}
	if (str1 == "")
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("�Ŷβ���Ϊ�գ�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}
	a = _atoi64(CStringA(str1));
	b = _atoi64(CStringA(str2));

	if (a < _atoi64(CStringA(IMEIStaStart)))
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("�Ŷαȶ�������ʼ��С�����������ã�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}
	else if (a > _atoi64(CStringA(IMEIStaEnd)))
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("�Ŷαȶ����н����Ŵ����������ã�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}
	else if (a > b)
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("�Ŷαȵ�ǰ���õĽ����Ŵ����������ã�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}

	if (radioflag == FALSE)
	{
		ADOManage adomanage;
		adomanage.ConnSQL();
		str3 = CreateIMEI15(str1);
		adomanage.JudgeIMEIExit(str3);
		if (adomanage.m_pRecordSet->adoEOF)
		{
			MessageBox(L"���õ���ʼ�Ż�δ�ص����", L"��ʾ��Ϣ", NULL);
			SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
			return;
		}
		adomanage.CloseAll();
	}

	IMEIStaCRStart = str1;
	SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
	GetDataNumber();
}

//�ж�IMEI�������Ƿ���ȷ
void CMFCP1LDDlg::OnEnKillfocusImeiendEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str1, str2, str3;

	unsigned long long a, b;
	GetDlgItemText(IDC_IMEISTART_EDIT, str1);
	GetDlgItemText(IDC_IMEIEND_EDIT, str2);
	if (str2 == "")
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("�Ŷβ���Ϊ�գ�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}
	a = _atoi64(CStringA(str1));
	b = _atoi64(CStringA(str2));

	if (b < _atoi64(CStringA(IMEIStaStart)))
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("�Ŷαȶ�������ʼ��С�����������ã�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}
	else if (b > _atoi64(CStringA(IMEIStaEnd)))
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("�Ŷαȶ����н����Ŵ����������ã�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}
	else if (b < a)
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("�Ŷαȵ�ǰ���õ���ʼ��С�����������ã�"), _T("��ʾ��Ϣ"), NULL);
		return;
	}

	if (radioflag == FALSE)
	{
		ADOManage adomanage;
		adomanage.ConnSQL();
		str3 = CreateIMEI15(str2);
		adomanage.JudgeIMEIExit(str3);
		if (adomanage.m_pRecordSet->adoEOF)
		{
			MessageBox(L"���õĽ����Ż�δ�ص����", L"��ʾ��Ϣ", NULL);
			SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
			return;
		}
		adomanage.CloseAll();
	}

	IMEIStaCREnd = str2;
	SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
	GetDataNumber();
}

//��ȡ�ص�������
void CMFCP1LDDlg::GetDataNumber()
{
	CString str1, str2, strorder;
	int x, y, z;
	GetDlgItemText(IDC_IMEISTART_EDIT, str1);
	GetDlgItemText(IDC_IMEIEND_EDIT, str2);


	StrToIntEx((LPCTSTR)str1, STIF_DEFAULT, &x);
	StrToIntEx((LPCTSTR)str2, STIF_DEFAULT, &y);

	z = y - x + 1;

	SetDlgItemInt(IDC_IMEINUMBER_EDIT, z);


	ADOManage adomanage;
	adomanage.ConnSQL();
	int nSel;
	nSel = m_zhidanautoCombo.GetCurSel();
	m_zhidanautoCombo.GetLBText(nSel, strorder);
	int a = adomanage.GetImeicount(strorder, str1, str2);
	SetDlgItemInt(IDC_HDIMEINUMBER_EDIT, a);
	SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, 0);
	adomanage.CloseAll();
}

//�����ص�Ŷ�
void CMFCP1LDDlg::OnBnClickedSetimeirangeButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

	CSetImeiRangeDlg SetImeiRangeDlg;
	nRes = SetImeiRangeDlg.DoModal();

	//���Ի����Ǳߵ��ȷ���˳�����
	if (IDCANCEL == nRes)
	{
		GetDataNumber();
		return;
	}
	CDialogEx::OnCancel();
}


//�ص�ϵͳģ��
//���ص�ϵͳ��ť
void CMFCP1LDDlg::OnBnClickedOpenldsystemButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (radioflag == FALSE)
	{
		INT_PTR nRes;
		nRes = MessageBox(_T("��ȷ��Ҫ�����ص���"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
		// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
		if (IDCANCEL == nRes)
			return;
	}

	//�ж��Ƿ�ѡ������һ���ֶ�
	if (LdVariableCount == 0)
	{
		MessageBox(_T("������ѡ��һ���ص��ֶΣ�"), _T("������ʾ"), NULL);
		return;
	}


	//�ж�IMEI���Ƿ�Ϊ��
	CString str1, str2, str3, str4;

	GetDlgItemText(IDC_ZHIDANAUTO_COMBO, str1);
	GetDlgItemText(IDC_IMEISTART_EDIT, str2);
	GetDlgItemText(IDC_IMEIEND_EDIT, str3);
	if (str1 == "" || str2 == "" || str3 == "")
	{
		MessageBox(_T("IMEI��Ϣ��ȫ���붩���š�IMEI��ʼ�źͽ�������д��������"), _T("������ʾ"), NULL);
		return;
	}

	//���ж������ص�ĺŶ��Ƿ��Ѿ��ع�
	if (radioflag == FALSE)
	{
		ADOManage adomanage;
		adomanage.ConnSQL();
		str4 = CreateIMEI15(str2);
		if (!adomanage.JudgeIMEIExit(str4))
		{
			MessageBox(L"���õ���ʼ�Ż�δ�ص����", L"��ʾ��Ϣ", NULL);
			adomanage.CloseAll();
			return;
		}

		str4 = CreateIMEI15(str3);
		adomanage.JudgeIMEIExit(str4);
		if (!adomanage.JudgeIMEIExit(str4))
		{
			MessageBox(L"���õĽ����Ż�δ�ص����", L"��ʾ��Ϣ", NULL);
			adomanage.CloseAll();
			return;
		}

		adomanage.CloseAll();
	}

	//�ȳ�ʼ��
	//���ص��������ʱ���е㳤������һ����ģ̬�Ի�������ʾ����ϵͳ�������֮�������Ի����Զ��ر�
	if (NULL == m_pCLDStartDlg)
	{
		m_pCLDStartDlg = new CLDStartDlg();
		m_pCLDStartDlg->Create(IDD_LDSTART_DIALOG, this);
	}
	m_pCLDStartDlg->ShowWindow(SW_SHOW);
	WindowTimeClose(L"�ص�ϵͳ���������Ե�...", 100);
	//MessageBox(L"�ص�ϵͳ����ʱ������Ҫʮ���룬�����ĵȴ���", L"��ʾ��Ϣ", MB_OK);
	//SetRicheditText(L"�ص�ϵͳ���������У����Ե�...", 0);
	
	DllFlag = markezd.lmc1_Initial(L"g:\\VS_PROJECT\\EzCad", TRUE, AfxGetMainWnd()->m_hWnd);
	//DllFlag = markezd.lmc1_Initial(L"", FALSE, AfxGetMainWnd()->m_hWnd);

	//������Ϻ�رյ������Ǹ�ģ̬�Ի���
	CWnd* hWnd = FindWindow(NULL, _T("��ʾ��Ϣ"));
	if (hWnd)
	{
		hWnd->PostMessage(WM_CLOSE, NULL, NULL);
	}


	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"�ص�ϵͳ�����ɹ�", 0);
	}
	else
	{
		SetRicheditText(L"�ص�ϵͳ����ʧ�ܣ��������" + DllStr, 1);
		return;
	}

	//���ص�ģ��
	CString strmodlepath;
	GetDlgItemText(IDC_MODELPATH_EDIT, strmodlepath);

	DllFlag = markezd.lmc1_LoadEzdFile((LPTSTR)(LPCTSTR)(strmodlepath));//��CStringת����TCHAR*���ͣ�Ҫת����

	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"�ص�ģ���ȡ�ɹ�", 0);
	}
	else
	{
		SetRicheditText(L"�ص�ģ���ȡʧ��,����ģ��·���Ƿ����󣬴������" + DllStr, 1);
		OnBnClickedCloseldsystemButton();
		return;
	}

	
	//����ص�ģ�����Ƿ����ָ���������ƣ��˺����ҵõ��������ƻ᷵��0���Ҳ����ͷ���1��ֱ�ӹر��ص�ϵͳ
	if (FindLdName())
	{
		OnBnClickedCloseldsystemButton();
		return;
	}
    
    //Ȼ���ص�ϵͳһЩ�ؼ����ʹ��
	LDSystemInformationwindowtrue();
    //ͬʱ���õ�������һЩ�ؼ�
	CRLDInformationwindowfalse();
	InitInformationwindowfalse();

	SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, 0);//ÿ�ο���ϵͳ��Ҫ�������ص�������Ϊ0
}

//Ѱ���ص��������
int CMFCP1LDDlg::FindLdName()
{
	double a, b, c, d, e;
	//һ��һ��ȥ���Ƿ���ڶ������ƣ��Ҳ����ͷ���1���ҵõ��ͷ���0
	for (int i = 0; i < LdVariableCount; i++)
	{
		DllFlag = markezd.lmc1_GetEntSize((LPTSTR)(LPCTSTR)LdVariableChar[i], a, b, c, d, e);
		if (DllFlag != 0)
		{
			DllStr.Format(_T("%d"), DllFlag);
			SetRicheditText(L"�ص�ϵͳ����ʧ�ܣ��������" + DllStr, 1);
			MessageBox(L"�Ҳ�������" + LdVariableChar[i], _T("������Ϣ"), NULL);
			return 1;
		}
	}
	return 0;
}

//�滻ָ���������Ƶ�ֵ
int CMFCP1LDDlg::ChangeLdName(CString LdVariable, CString strld)
{
	DllFlag = markezd.lmc1_ChangeTextByName((LPTSTR)(LPCTSTR)LdVariable, (LPTSTR)(LPCTSTR)strld);
	DllStr.Format(_T("%d"), DllFlag);
	if (DllFlag == 0)
	{
		SetRicheditText(L"�ص���...", 0);
	}
	else
	{
		SetRicheditText(L"�滻�����ֵʧ�ܣ��������" + DllStr, 1);
		return 0;
	}
	return 1;
}

//��ʼ�ص���
int CMFCP1LDDlg::LdCore()
{
	CString StrImeiLd; //�ص��õı���
	int imeicount, imeirecount;
	ADOManage adomanage;
	adomanage.ConnSQL();

	//����У��λҪ���ֿ�
	if (IMEI15Flag == 1)
	{
	StrImeiLd = CreateIMEI15(IMEIStaCRStart);
	}
	else if (IMEI15Flag == 0)
	{
		StrImeiLd = IMEIStaCRStart;
	}

	//�ж��Ƶ����Ƿ�Ϊ��
	if (JudgeZhidan(ZhiDanNO))
	{
		MessageBox(L"�����Ų����ڣ�", L"��ʾ��Ϣ", NULL);
		adomanage.CloseDB();
		return 0;
	}

    //�������ص�������,����0����IMEI������,�Ͳ���ִ��while������1��ֱ�Ӹ�IMEI��+1��һֱ�����ظ�Ϊֹ
	while (adomanage.JudgeIMEIExit(StrImeiLd)&&radioflag==TRUE)
	{
		//���ж�IMEI�Ŷ��Ƿ��Ѿ�ȫ���ص���
		if (IMEIStaCRStart > IMEIStaCREnd)
		{
			SetRicheditText(_T("����IMEI�Ŷ���ȫ���ص����"), 0);
			MessageBox(L"����IMEI�Ŷ���ȫ���ص���ϣ�", L"��ʾ��Ϣ", NULL);
			OnBnClickedCloseldsystemButton();
			adomanage.CloseAll();
			return 0;
		}

		//������ݿ��Ѿ����ڣ�����ʾ�ظ���Ϣ������IMEI��+1
		SetRicheditText(_T("IMEI��") + StrImeiLd + _T("�Ѿ�����"), 1);

		unsigned long long imeiint = 0;
		imeiint = _atoi64(CStringA(IMEIStaCRStart)) + 1;
		IMEIStaCRStart = _ui64toa(imeiint, CT2A(IMEIStaCRStart), 10);

		//Ȼ����µ�ǰ�ص���Ϣ������У��λҪ���ֿ�
		if (IMEI15Flag == 1)
		{
			StrImeiLd = CreateIMEI15(IMEIStaCRStart);
			SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);//��ʾ��ǰҪ�ص��IMEI��
		}
		else if (IMEI15Flag == 0)
		{
			StrImeiLd = IMEIStaCRStart;
			SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);
		}
	}
	
	//�ȸ�ģ�渳ֵ
	for (int i = 0; i < LdVariableCount; i++)
	{
		switch (i)
		{
		case 0:
			ChangeLdName(LdVariableChar[i], StrImeiLd);
			break;
		case 1:
	        ChangeLdName(LdVariableChar[i], L"SN");
			break;
		default:
			break;
		}
	}

	//Ȼ��ʼ�ص�
	DllFlag = markezd.lmc1_Mark(TRUE);
	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		InsertData(StrImeiLd);//����\�����ص������²�������
		SetRicheditText(L"IMEI��:" + StrImeiLd + L"���ص����", 0);

		//�ص����IMEI��+1
		unsigned long long imeiint = 0;
		imeiint = _atoi64(CStringA(IMEIStaCRStart)) + 1;
		IMEIStaCRStart = _ui64toa(imeiint, CT2A(IMEIStaCRStart), 10);

		//Ȼ����µ�ǰ�ص���Ϣ
		StrImeiLd = CreateIMEI15(IMEIStaCRStart);
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);//��ʾ��ǰҪ�ص��IMEI��

		//�����Ѵ���������������������ص�����±�����������
		if (radioflag == TRUE)
		{
			imeicount = GetDlgItemInt(IDC_HDIMEINUMBER_EDIT);
			imeicount++;
			SetDlgItemInt(IDC_HDIMEINUMBER_EDIT, imeicount);
		}
		else
		{
			imeirecount = GetDlgItemInt(IDC_HDREIMEINUMBER_EDIT);
			imeirecount++;
			SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, imeirecount);
		}

		//�������ж�IMEI�Ŷ��Ƿ��Ѿ�ȫ���ص���
		if (IMEIStaCRStart > IMEIStaCREnd)
		{
			SetRicheditText(_T("����IMEI�Ŷ���ȫ���ص����"), 0);
			MessageBox(L"����IMEI�Ŷ���ȫ���ص���ϣ�", L"��ʾ��Ϣ", NULL);
			OnBnClickedCloseldsystemButton();
			adomanage.CloseAll();
			return 0;
		}
	}
	else
	{
		SetRicheditText(L"�ص�ʧ�ܣ��������" + DllStr, 1);
		adomanage.CloseAll();
		return 0;
	}
	return 1;
}

//�ص�һ�ΰ�ť
void CMFCP1LDDlg::OnBnClickedLdoneceButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	LdCore();
}

//��̤����ת���ģʽ��ť��δ��ɣ�����������ȥ���ԣ�
void CMFCP1LDDlg::OnBnClickedStepldButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (STEPLDFlag == 1)
	{
		SetDlgItemText(IDC_STEPLD_BUTTON, _T("ֹͣ��̤����ת���"));
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(FALSE);
		STEPLDFlag = 0;
		//�������߳�
		LdReadPortThread = AfxBeginThread(ThreadReadLdPort, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		return;
	}
	else
	{
		SetDlgItemText(IDC_STEPLD_BUTTON, _T("��̤����ת���ģʽ"));
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(TRUE);
		STEPLDFlag = 1;
		m_LdPortRun = FALSE;
		return;
	}
}

//��̤�������ת�����õ����߳�
UINT ThreadReadLdPort(LPVOID pParam)
{
	CMFCP1LDDlg* dlg;
	dlg = (CMFCP1LDDlg*)pParam;
	m_LdPortRun = TRUE;//ȫ�ֱ������������FALSE��ʱ��while�ͻ�����ѭ����Ȼ���˳�����߳�
	while (m_LdPortRun)
	{
		dlg->markezd.lmc1_ReadPort(dlg->LdPort);
		if (dlg->LdPort == 141)
		{
			dlg->OnBnClickedLdoneceButton();
		}
		Sleep(500);//ֹͣ0.5��Ȼ�������
	}
	
	return 0;
}

//�ر�ϵͳ��ť
void CMFCP1LDDlg::OnBnClickedCloseldsystemButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DllFlag = markezd.lmc1_Close();

	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"�ر��ص�ϵͳ�ɹ�", 0);
	}
	else
	{
		SetRicheditText(L"�ر��ص�ϵͳʧ�ܣ��������" + DllStr, 1);
		return;
	}

	//Ȼ���ص�ϵͳһЩ�ؼ����ʹ��
	LDSystemInformationwindowfalse();
	//ͬʱ���õ�������һЩ�ؼ�
	CRLDInformationwindowtrue();
	InitInformationwindowtrue();
}

//���ɨ��ǹģʽ��ť
void CMFCP1LDDlg::OnBnClickedScanmodeButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (ScanFlag == 1)
	{
		SetDlgItemText(IDC_SCANMODE_BUTTON, _T("ֹͣɨ��ǹģʽ"));
		GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(FALSE);
		ScanFlag = 0;
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
	}
	else
	{
		SetDlgItemText(IDC_SCANMODE_BUTTON, _T("����ɨ��ǹģʽ"));
		GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(TRUE);
		ScanFlag = 1;
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_SCANLD_EDIT, L"");
	}
}

//������Ӧ��������д
BOOL CMFCP1LDDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;

        //��ScanFlagΪ0��ʱ������ʱ�Ѿ�����ɨ��ǹģʽ
		if (nVirtKey == VK_RETURN&&ScanFlag == 0)
		{
			//����ǻس�����������Ҫ��������
			if (pMsg->hwnd == GetDlgItem(IDC_SCANLD_EDIT)->GetSafeHwnd())
			{
				CString strimei;
				GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
				GetDlgItemText(IDC_SCANLD_EDIT, strimei);
				ScanInsertData(strimei);
				SetDlgItemText(IDC_SCANLD_EDIT, L"");
				GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(TRUE);
				GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//����\�����ص������²�������
void CMFCP1LDDlg::InsertData(CString strimei)
{
	ADOManage adomanage;
	adomanage.ConnSQL();

	//����˿̵�ѡ���������ص��Ǿ�ִ�в�����䣬����������ص� �Ǿ�ִ�и������
	if (((CButton*)GetDlgItem(IDC_NORMOLLD_RADIO))->GetCheck() == TRUE)
	{
		adomanage.AddNew(m_pcipEdit, ZhiDanNO, strimei);
		if (IMEI15Flag == 1)
		{
			adomanage.UpdateIMEICurrent(ZhiDanNO, strimei.Left(strimei.GetLength() - 1));
		}
		else if (IMEI15Flag == 0)
		{
			adomanage.UpdateIMEICurrent(ZhiDanNO, strimei);
		}
	}
	else if (((CButton*)GetDlgItem(IDC_RELD_RADIO))->GetCheck() == TRUE)
	{
		adomanage.UpdateReld(strimei);
	}

	adomanage.CloseAll();
}

//ɨ��ǹ�����ݿ�������
void CMFCP1LDDlg::ScanInsertData(CString strimei)
{
	CString StrImeiTemp; //��ʱ����
	int imeicount, imeirecount;
	ADOManage adomanage;
	adomanage.ConnSQL();

	//����У��λҪ���ֿ�����У��λ���ð����һλ�ɵ�
	if (IMEI15Flag == 1)
	{
		StrImeiTemp = strimei.Left(strimei.GetLength() - 1);
	}
	else if (IMEI15Flag == 0)
	{
		StrImeiTemp = strimei;
	}

	//�ж��Ƶ����Ƿ�Ϊ��
	if (JudgeZhidan(ZhiDanNO))
	{
		WindowTimeClose(_T("�����Ų����ڣ�"), 1500);
		return ;
	}

	//(strimei == CreateIMEI15(StrImeiTemp) || IMEI15Flag == 1)�����棬��У��λ��ʱ���һ��Ϊ�棬������Ҫ��У��λ�жϣ���У��λ�Ϳ�StrImeiTemp����У��λ���Ƿ���strimei��ͬ
	if (!(StrImeiTemp <= IMEIStaEnd&&StrImeiTemp >= IMEIStaStart&&StrImeiTemp.GetLength() == IMEIStaStart.GetLength() && (strimei == CreateIMEI15(StrImeiTemp) || IMEI15Flag == 0)))
	{
		WindowTimeClose(_T("ɨ���IMEI�Ų��ںŶη�Χ�ڣ�"), 1500);
		return;
	}

	//�������ص�������,����0����IMEI������,����1�ʹ���IMEI�Ŵ���
	if (adomanage.JudgeIMEIExit(strimei) && radioflag == TRUE)
	{
		WindowTimeClose(_T("ɨ���IMEI���Ѵ��������ݿ��У�"), 1500);
		return;
	}
	else if ((!adomanage.JudgeIMEIExit(strimei)) && radioflag == FALSE)
	{
		WindowTimeClose(_T("ɨ���IMEI�Ų�δ�ص����"), 1500);
		return;
	}

	SetDlgItemText(IDC_CURRENTIMEI_EDIT, strimei);//��ʾ��ǰҪ�ص��IMEI��


	//�ȸ�ģ�渳ֵ
	for (int i = 0; i < LdVariableCount; i++)
	{
		switch (i)
		{
		case 0:
			ChangeLdName(LdVariableChar[i], strimei);
			break;
		case 1:
			ChangeLdName(LdVariableChar[i], L"SN");
			break;
		default:
			break;
		}
	}

	//Ȼ��ʼ�ص�
	DllFlag = markezd.lmc1_Mark(TRUE);
	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"ɨ���IMEI��:" + strimei + L"���ص����", 0);

		//�����Ѵ���������������������ص�����±�����������
		//����˿̵�ѡ���������ص��Ǿ�ִ�в�����䣬����������ص��Ǿ�ִ�и������
		if (radioflag == TRUE)
		{
			adomanage.AddNew(m_pcipEdit, ZhiDanNO, strimei);
			//adomanage.UpdateIMEICurrent(ZhiDanNO, strimei.Left(strimei.GetLength() - 1));
			imeicount = GetDlgItemInt(IDC_HDIMEINUMBER_EDIT);
			imeicount++;
			SetDlgItemInt(IDC_HDIMEINUMBER_EDIT, imeicount);
		}
		else
		{
			adomanage.UpdateReld(strimei);
			imeirecount = GetDlgItemInt(IDC_HDREIMEINUMBER_EDIT);
			imeirecount++;
			SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, imeirecount);
		}
	}
	else
	{
		WindowTimeClose(_T("�ص�ʧ�ܣ��������") + DllStr, 1500);
		adomanage.CloseDB();
		GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
		return ;
	}

	adomanage.CloseAll();
}

//����IMEIУ��λ
CString CMFCP1LDDlg::CreateIMEI15(CString imei)
{
	long long a = 0;
	int j = 0;
	char imeiChar[14];
	strncpy(imeiChar, CStringA(imei), sizeof(imeiChar));
	//	std::string sz2 = CT2A(imei.GetBuffer());

	int resultInt = 0;
	for (int i = 0; i < 14; i++)
	{
		int a = (int)imeiChar[i] - '0';
		i++;
		int temp = ((int)imeiChar[i] - '0') * 2;
		int b = temp < 10 ? temp : temp - 9;
		resultInt += a + b;
	}
	resultInt %= 10;
	GetLastError();
	resultInt = (resultInt == 0 ? 0 : 10 - resultInt);
	CString resultStr, abc;
	resultStr.Format(_T("%d"), resultInt);
	abc = imei + resultStr;
	imei.ReleaseBuffer();
	return abc;
}

//������N������Զ��ر�
void CMFCP1LDDlg::WindowTimeClose(CString str, int strtime)
{
	SetRicheditText(str, 1);
	SetTimer(1, strtime, NULL);//������ʱ��1,��ʱʱ����1.5��
	MessageBox(str + L"(�˵�������Զ��ر�)", L"��ʾ��Ϣ", NULL);
}

//��ʱ������
void CMFCP1LDDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd* hWnd = FindWindow(NULL, _T("��ʾ��Ϣ"));
	if (hWnd)
	{
		hWnd->PostMessage(WM_CLOSE, NULL, NULL);
	}
	KillTimer(1);   //�رն�ʱ��
	GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
	CDialogEx::OnTimer(nIDEvent);
}


//��Ϣ��־ģ��
//����ֵ������Ϣ��־����ʾ��ͬ����Ϣ
void CMFCP1LDDlg::SetRicheditText(CString strMsg, int No)
{
	m_currentRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//��������ṹ���ı�����Ҫ�����ֵ���ɫ������
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//���Ŀǰ��û������ɣ������Ҫ�����ͬ��ɫ������һ��Ҫʹ��richedit
	cf.crTextColor = RGB(255, 0, 0);//����Ϊ��ɫ

	switch (No)
	{
	case 0:
		m_currentRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentRichedit.LineScroll(m_currentRichedit.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_currentRichedit.SetSelectionCharFormat(cf);
		m_currentRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentRichedit.LineScroll(m_currentRichedit.GetLineCount() - 1);
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
CString CMFCP1LDDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//���ݿ��ѯ����ģ��
//�����ѯ���ݱ�ť
void CMFCP1LDDlg::OnBnClickedSelectalldataButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ʼ���б�ؼ�
	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_dbcurtainList.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_dbcurtainList.SetExtendedStyle(m_dbcurtainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//ɾ�������еı�ͷ
	while (1) {
		if (!m_dbcurtainList.DeleteColumn(0)) {
			break;
		}
	}

	// Ϊ�б���ͼ�ؼ������   
	m_dbcurtainList.InsertColumn(0, _T("IMEI��"), LVCFMT_CENTER, rect.Width() / 2, 0);
	m_dbcurtainList.InsertColumn(0, _T("�ص�ʱ��"), LVCFMT_CENTER, rect.Width() / 2, 0);
	ShowAllData();
}

//��ʾ�Ѳ��������
void CMFCP1LDDlg::ShowAllData()
{
	m_dbcurtainList.DeleteAllItems(); // ȫ�����
	CString strimeidata;
	int i = 0;

	ADOManage adomanage;
	adomanage.ConnSQL();
	
	adomanage.ShowInsertImeiByOrderNumber(ZhiDanNO);

	while (!adomanage.m_pRecordSet->adoEOF)
	{
		strimeidata = adomanage.m_pRecordSet->GetCollect("IMEI");
		m_dbcurtainList.InsertItem(i, strimeidata);
		strimeidata = adomanage.m_pRecordSet->GetCollect("InputTime");
		m_dbcurtainList.SetItemText(i, 1, strimeidata);
		adomanage.m_pRecordSet->MoveNext();
		i++;
	}

	adomanage.CloseAll();
}

//�����ѯ�ظ���ť
void CMFCP1LDDlg::OnBnClickedSelectrepetdataButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ʼ���б�ؼ�
	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_dbcurtainList.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_dbcurtainList.SetExtendedStyle(m_dbcurtainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//ɾ�������еı�ͷ
	while (1) {
		if (!m_dbcurtainList.DeleteColumn(0)) {
			break;
		}
	}

	// Ϊ�б���ͼ�ؼ������   
	m_dbcurtainList.InsertColumn(0, _T("IMEI��"), LVCFMT_CENTER, rect.Width() / 5 * 2, 0);
	m_dbcurtainList.InsertColumn(1, _T("�������ʱ��"), LVCFMT_CENTER, rect.Width() / 5 * 2, 0);
	m_dbcurtainList.InsertColumn(2, _T("�ظ�����"), LVCFMT_CENTER, rect.Width() / 5 * 1, 0);
	ShowRepeatData();
}

//��ʾ�ظ�������
void CMFCP1LDDlg::ShowRepeatData()
{
	m_dbcurtainList.DeleteAllItems(); // ȫ�����
	CString strimeidata;
	int i = 0;

	ADOManage adomanage;
	adomanage.ConnSQL();
	
	adomanage.ShowRepeatImeiByOrderNumber(ZhiDanNO);

	while (!adomanage.m_pRecordSet->adoEOF)
	{
		strimeidata = adomanage.m_pRecordSet->GetCollect("IMEI");
		m_dbcurtainList.InsertItem(i, strimeidata);
		strimeidata = adomanage.m_pRecordSet->GetCollect("ReLdImeiTime");
		m_dbcurtainList.SetItemText(i, 1, strimeidata);
		strimeidata = adomanage.m_pRecordSet->GetCollect("ReLdImeiNum");
		m_dbcurtainList.SetItemText(i, 2, strimeidata);
		adomanage.m_pRecordSet->MoveNext();
		i++;
	}

	adomanage.CloseAll();
}

//�����ѯδ�ص�Ű�ť
void CMFCP1LDDlg::OnBnClickedSelectmissingdataButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ʼ���б�ؼ�
	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_dbcurtainList.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_dbcurtainList.SetExtendedStyle(m_dbcurtainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//ɾ�������еı�ͷ
	while (1) {
		if (!m_dbcurtainList.DeleteColumn(0)) {
			break;
		}
	}

	// Ϊ�б���ͼ�ؼ������   
	m_dbcurtainList.InsertColumn(0, _T("�Ŷ���IMEIδ�ص��"), LVCFMT_CENTER, rect.Width(), 0);
	ShowUnData();
}

//��ʾδ�ص������
void CMFCP1LDDlg::ShowUnData()
{
	m_dbcurtainList.DeleteAllItems(); // ȫ�����
	CString strimeidata;
	int i = 0, imeicount;
	imeicount = GetDlgItemInt(IDC_IMEINUMBER_EDIT);

	if (imeicount >= 2000)
	{
		MessageBox(L"Ϊ��֤Ч�ʣ��뽫�Ŷ�������������2000����", L"��ʾ��Ϣ", NULL);
		return;
	}

	//���ſ�ʼ�����ݿ��н��в�ѯ���������ݲ��뵽�б���
	ADOManage adomanage;
	adomanage.ConnSQL();

	adomanage.ShowUnImeiByOrderNumber(ZhiDanNO, IMEIStaCRStart, IMEIStaCREnd);

	while (!adomanage.m_pRecordSet->adoEOF)
	{
		strimeidata = adomanage.m_pRecordSet->GetCollect("UNIMEI");
		m_dbcurtainList.InsertItem(i, strimeidata);
		adomanage.m_pRecordSet->MoveNext();
		i++;
	}

	adomanage.CloseAll();
}


//��������
//ȷ����ť
void CMFCP1LDDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

//�˳���ť
void CMFCP1LDDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;
	nRes = MessageBox(_T("��ȷ��Ҫ�˳���"), _T("��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;

	CDialogEx::OnCancel();
}

