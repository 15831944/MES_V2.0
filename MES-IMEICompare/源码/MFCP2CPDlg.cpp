
// MFCP2CPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP2CP.h"
#include "MFCP2CPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include "DBconfig.h"
#include "ADOManage.h"
#include "Manager.h"

#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*ȫ�ֱ���*/
//extern int SyllableArray[8];
//extern int BindArray[6] = {};

//extern map<int, CString>SyllableMap;
//extern map<int, CString>BindMap;

map<int, CString>CMFCP2CPDlg::SyllableMap;
map<int, CString>CMFCP2CPDlg::BindMap;
BOOL Imei3Flag;

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


// CMFCP2CPDlg �Ի���



CMFCP2CPDlg::CMFCP2CPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP2CPDlg::IDD, pParent)
	, m_imei1Edit(_T(""))
	, m_imei2Edit(_T(""))
	, strpcname(_T(""))
	, strpcip(_T(""))
	, notype(0)
	, strno1(L"NULL")
    , strno2(L"NULL")
	, strno3(L"NULL")
	, strzhidan(L"")
	, chjudgeflag(0)
	, strimeistart(L"")
	, strimeiend(L"")
	, LockFlag(FALSE)
	, m_imei3Edit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	notypename[0] = L"IMEI";
	notypename[1] = L"SN";
	notypename[2] = L"SIM";
	notypename[3] = L"VIP";
	notypename[4] = L"ICCID";
	notypename[5] = L"BAT";
	notypename[6] = L"MAC";
	notypename[7] = L"Equipment";
	SyllablestrSql = L"";
}

void CMFCP2CPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HINT_STATIC, m_hintStatic);
	DDX_Control(pDX, IDC_IMEI2_STATIC, m_imei2Static);
	DDX_Control(pDX, IDC_IMEI1_STATIC, m_imei1Static);
	DDX_Text(pDX, IDC_IMEI1_EDIT, m_imei1Edit);
	DDX_Text(pDX, IDC_IMEI2_EDIT, m_imei2Edit);
	DDX_Control(pDX, IDC_COUPLING_CHECK, m_couplingCheck);
	DDX_Control(pDX, IDC_WRITENUM_CHECK, m_writenumCheck);
	DDX_Control(pDX, IDC_DOWNLOAD_CHECK, m_downloadCheck);
	DDX_Control(pDX, IDC_FUNTEST_CHECK, m_funtestCheck);
	DDX_Control(pDX, IDC_ZHIDAN_COMBO, m_zhidanCombo);
	DDX_Control(pDX, IDC_CHJUDGE_CHECK, m_caihejudgeCheck);
	DDX_Control(pDX, IDC_IMEISYLLABLE_CHECK, m_imeiSyllableCheck);
	DDX_Control(pDX, IDC_SNSYLLABLE_CHECK, m_snSyllableCheck);
	DDX_Control(pDX, IDC_SIMSYLLABLE_CHECK, m_simSyllableCheck);
	DDX_Control(pDX, IDC_VIPSYLLABLE_CHECK, m_vipSyllableCheck);
	DDX_Control(pDX, IDC_ICCIDSYLLABLE_CHECK, m_iccidSyllableCheck);
	DDX_Control(pDX, IDC_BATSYLLABLE_CHECK, m_batSyllableCheck);
	DDX_Control(pDX, IDC_MACSYLLABLE_CHECK, m_macSyllableCheck);
	DDX_Control(pDX, IDC_EQUIPMENTSYLLABLE_CHECK, m_equipmentSyllableCheck);
	DDX_Control(pDX, IDC_SIMBIND_CHECK, m_simBindCheck);
	DDX_Control(pDX, IDC_VIPBIND_CHECK, m_vipBindCheck);
	DDX_Control(pDX, IDC_ICCIDBIND_CHECK, m_iccidBindCheck);
	DDX_Control(pDX, IDC_BATBIND_CHECK, m_batBindCheck);
	DDX_Control(pDX, IDC_MACBIND_CHECK, m_macBindCheck);
	DDX_Control(pDX, IDC_EQUIPMENTBIND_CHECK, m_equipmentBindCheck);
	DDX_Text(pDX, IDC_IMEI3_EDIT, m_imei3Edit);
	DDX_Control(pDX, IDC_OPENIMEI3EDIT_CHECK, m_OpenImei3EditCheck);
	DDX_Control(pDX, IDC_RFIDBIND_CHECK, m_rfidBindCheck);
	DDX_Control(pDX, IDC_RFIDSYLLABLE_CHECK, m_rfidSyllableCheck);
}

BEGIN_MESSAGE_MAP(CMFCP2CPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP2CPDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_IMEI2_EDIT, &CMFCP2CPDlg::OnEnSetfocusImei2Edit)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP2CPDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_IMEI_RADIO, &CMFCP2CPDlg::OnBnClickedImeiRadio)
	ON_BN_CLICKED(IDC_SN_RADIO, &CMFCP2CPDlg::OnBnClickedSnRadio)
	ON_BN_CLICKED(IDC_SIM_RADIO, &CMFCP2CPDlg::OnBnClickedSimRadio)
	ON_BN_CLICKED(IDC_VIP_RADIO, &CMFCP2CPDlg::OnBnClickedVipRadio)
	ON_BN_CLICKED(IDC_ICCID_RADIO, &CMFCP2CPDlg::OnBnClickedIccidRadio)
	ON_BN_CLICKED(IDC_BAT_RADIO, &CMFCP2CPDlg::OnBnClickedBatRadio)
	ON_BN_CLICKED(IDC_MAC_RADIO, &CMFCP2CPDlg::OnBnClickedMacRadio)
	ON_BN_CLICKED(IDC_EQUIPMENT_RADIO, &CMFCP2CPDlg::OnBnClickedEquipmentRadio)
	ON_BN_CLICKED(IDC_UPDATEORDERNUMBER_BUTTON, &CMFCP2CPDlg::OnBnClickedUpdateordernumberButton)
	ON_CBN_SELCHANGE(IDC_ZHIDAN_COMBO, &CMFCP2CPDlg::OnCbnSelchangeZhidanCombo)
	ON_CBN_SELENDOK(IDC_ZHIDAN_COMBO, &CMFCP2CPDlg::OnCbnSelendokZhidanCombo)
	ON_CBN_KILLFOCUS(IDC_ZHIDAN_COMBO, &CMFCP2CPDlg::OnCbnKillfocusZhidanCombo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHJUDGE_CHECK, &CMFCP2CPDlg::OnBnClickedChjudgeCheck)
	ON_BN_CLICKED(IDC_SAVESYLLABLE_BUTTON, &CMFCP2CPDlg::OnBnClickedSavesyllableButton)
	ON_BN_CLICKED(IDC_READSYLLABLE_BUTTON, &CMFCP2CPDlg::OnBnClickedReadsyllableButton)
	ON_BN_CLICKED(IDC_SAVEBIND_BUTTON, &CMFCP2CPDlg::OnBnClickedSavebindButton)
	ON_BN_CLICKED(IDC_READBIND_BUTTON, &CMFCP2CPDlg::OnBnClickedReadbindButton)
	ON_BN_CLICKED(IDC_SAVEBIND_BUTTON, &CMFCP2CPDlg::OnBnClickedSavebindButton)
	ON_BN_CLICKED(IDC_SIMBIND_CHECK, &CMFCP2CPDlg::OnBnClickedSimbindCheck)
	ON_BN_CLICKED(IDC_VIPBIND_CHECK, &CMFCP2CPDlg::OnBnClickedVipbindCheck)
	ON_BN_CLICKED(IDC_BATBIND_CHECK, &CMFCP2CPDlg::OnBnClickedBatbindCheck)
	ON_BN_CLICKED(IDC_ICCIDBIND_CHECK, &CMFCP2CPDlg::OnBnClickedIccidbindCheck)
	ON_BN_CLICKED(IDC_MACBIND_CHECK, &CMFCP2CPDlg::OnBnClickedMacbindCheck)
	ON_BN_CLICKED(IDC_EQUIPMENTBIND_CHECK, &CMFCP2CPDlg::OnBnClickedEquipmentbindCheck)
	ON_BN_CLICKED(IDC_IMEISYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedImeisyllableCheck)
	ON_BN_CLICKED(IDC_SNSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedSnsyllableCheck)
	ON_BN_CLICKED(IDC_VIPSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedVipsyllableCheck)
	ON_BN_CLICKED(IDC_SIMSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedSimsyllableCheck)
	ON_BN_CLICKED(IDC_BATSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedBatsyllableCheck)
	ON_BN_CLICKED(IDC_ICCIDSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedIccidsyllableCheck)
	ON_BN_CLICKED(IDC_EQUIPMENTSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedEquipmentsyllableCheck)
	ON_BN_CLICKED(IDC_MACSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedMacsyllableCheck)
	ON_BN_CLICKED(IDC_LOCK_BUTTON, &CMFCP2CPDlg::OnBnClickedLockButton)
	ON_BN_CLICKED(IDC_OPENIMEI3EDIT_CHECK, &CMFCP2CPDlg::OnBnClickedOpenimei3editCheck)
	ON_BN_CLICKED(IDC_RFIDSYLLABLE_CHECK, &CMFCP2CPDlg::OnBnClickedRfidsyllableCheck)
	ON_BN_CLICKED(IDC_RFIDBIND_CHECK, &CMFCP2CPDlg::OnBnClickedRfidbindCheck)
	ON_EN_SETFOCUS(IDC_IMEI1_EDIT, &CMFCP2CPDlg::OnEnSetfocusImei1Edit)
END_MESSAGE_MAP()


// CMFCP2CPDlg ��Ϣ�������

BOOL CMFCP2CPDlg::OnInitDialog()
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
	//radiobutton��������
	CButton* imeiradio = (CButton*)GetDlgItem(IDC_IMEI_RADIO);
	CButton* snradio = (CButton*)GetDlgItem(IDC_SN_RADIO);
	CButton* vpiradio = (CButton*)GetDlgItem(IDC_VIP_RADIO);
	CButton* iccidradio = (CButton*)GetDlgItem(IDC_ICCID_RADIO);
	CButton* batradio = (CButton*)GetDlgItem(IDC_BAT_RADIO);
	CButton* macradio = (CButton*)GetDlgItem(IDC_MAC_RADIO);
	CButton* equipmentradio = (CButton*)GetDlgItem(IDC_EQUIPMENT_RADIO);
	CButton* simradio = (CButton*)GetDlgItem(IDC_SIM_RADIO);

	//�����С
	fontinit();

	//��ʼ���ؼ�ѡ��
	imeiradio->SetCheck(1);

	//��ʼ������IP������
	GetLocalHostIPName(strpcname,strpcip);

	SetDlgItemText(IDC_PCNAME_EDIT, strpcname);
	SetDlgItemText(IDC_PCIP_EDIT, strpcip);

	//��ʼ��������
	InitComboBox();

	//Ĭ��IMEIһ��Ϊѡ��
	m_imeiSyllableCheck.SetCheck(BST_CHECKED);
	OnBnClickedImeisyllableCheck();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCP2CPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCP2CPDlg::OnPaint()
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
HCURSOR CMFCP2CPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�༭��ת�ƽ��㣬���ĺ���,���Ǹ�ϵͳ�Դ�����Ϣ���������ڼ���������Ķ�����ֻҪ�������κε����붼�ᴥ���������
BOOL CMFCP2CPDlg::PreTranslateMessage(MSG* pMsg)
{
	CString str1 = L"", str2 = L"", str3 = L"";//�ֱ�װ��IMEI1ɨ���IMEI2ɨ���IMEI3ɨ�����ַ���
	int mm;
	int resultflag1, resultflag2, resultflag3;//�ֱ�װ��IMEI1ɨ���IMEI2ɨ���IMEI3ɨ����һЩ��ѯ�����������Щ������ж��ɹ����
	ADOManage adomanage;

	//���Ϊ�վͲ�Ҫ����������ִ����
	if (Imei3Flag == TRUE)
	{
		GetDlgItemText(IDC_IMEI3_EDIT, str3);
		if (str3 == "")
		{
			return CDialogEx::PreTranslateMessage(pMsg);
		}
	}
	else if (Imei3Flag == FALSE)
	{
		GetDlgItemText(IDC_IMEI1_EDIT, str1);
		if (str1 == "")
		{
			return CDialogEx::PreTranslateMessage(pMsg);
		}
	}

	// TODO:  �ڴ����ר�ô����/����û���
	//���ϵĶ���������Ķ���
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;

		if (nVirtKey == VK_RETURN)
		{
			//����ǻس�����������Ҫ��������

			//���������ǹ�ѡ���жϲʺ�����λ��������
			if (chjudgeflag == 1)
			{
				if (pMsg->hwnd == GetDlgItem(IDC_IMEI3_EDIT)->GetSafeHwnd())
				{
					GetDlgItemText(IDC_IMEI3_EDIT, str3);
					mm = IsNumber(str3);//�ж�str3�Ƿ���зǷ��ַ�

					//������зǷ��ַ���ֱ����ʾ�������
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
					pMsg->wParam = VK_TAB;
					return CDialogEx::PreTranslateMessage(pMsg);
				}

				if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					mm = IsNumber(str1);//�ж�str1�Ƿ���зǷ��ַ�

					//������зǷ��ַ���ֱ����ʾ�������
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
					pMsg->wParam = VK_TAB;
					return CDialogEx::PreTranslateMessage(pMsg);
				}
				else if (pMsg->hwnd == GetDlgItem(IDC_IMEI2_EDIT)->GetSafeHwnd())
				{
					//�������ʱ��
					if (Imei3Flag == TRUE)
					{
						GetDlgItemText(IDC_IMEI1_EDIT, str1);
						GetDlgItemText(IDC_IMEI2_EDIT, str2);
						GetDlgItemText(IDC_IMEI3_EDIT, str3);

						//���ж��������Ƿ񲻴��Ƿ��ַ����ǵĻ���ֱ�ӱ������ϴ����ݿ�
						mm = IsNumber(str1);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						mm = IsNumber(str2);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						mm = IsNumber(str3);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}

						//���ж��Ƿ����
						if (str1 == str2&&str1 == str3&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend) && judgeimeirang(str3, strimeistart, strimeiend))
						{
							adomanage.ConnSQL();

							if (adomanage.JudgeZhidan(str1, strzhidan))
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�Ƶ��Ŵ���", L"0");
								adomanage.CloseAll();
								SetDlgItemText(IDC_HINT_STATIC, L"�Ƶ��Ŵ���");
								SetEditEmpty();//��ձ༭��
								GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
								return CDialogEx::PreTranslateMessage(pMsg);
							}

							PlaySound(L"ͨ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertCorrectImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, L"IMEI", L"NULL", L"1");
							SetDlgItemText(IDC_HINT_STATIC, L"ͨ��");
							adomanage.CloseAll();
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						//���Ⱦ��ϴ�ʧ������
						else
						{
							adomanage.ConnSQL();
							//�������붼�ڷ�Χ�ڣ��Ǵ���϶����Ǻ��벻ƥ��
							if (judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend) && judgeimeirang(str3, strimeistart, strimeiend))
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, L"IMEI", L"IMEI�Ų�ƥ��", L"0");
								adomanage.CloseAll();
								SetDlgItemText(IDC_HINT_STATIC, L"ʧ��");
							}
							else
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, L"IMEI", L"�Ŷ��ڷ�Χ��", L"0");
								adomanage.CloseAll();
								SetDlgItemText(IDC_HINT_STATIC, L"�Ŷ��ڷ�Χ��");
							}
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}
					//�������ʱ��
					else if (Imei3Flag == FALSE)
					{
						GetDlgItemText(IDC_IMEI1_EDIT, str1);
						GetDlgItemText(IDC_IMEI2_EDIT, str2);

						//���ж��������Ƿ񲻴��Ƿ��ַ����ǵĻ���ֱ�ӱ������ϴ����ݿ�
						mm = IsNumber(str1);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						mm = IsNumber(str2);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}

						//���ж��Ƿ����
						if (str1 == str2&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
						{
							adomanage.ConnSQL();

							if (adomanage.JudgeZhidan(str1, strzhidan))
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�Ƶ��Ŵ���", L"0");
								adomanage.CloseAll();
								SetDlgItemText(IDC_HINT_STATIC, L"�Ƶ��Ŵ���");
								SetEditEmpty();//��ձ༭��
								GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
								return CDialogEx::PreTranslateMessage(pMsg);
							}

							PlaySound(L"ͨ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertCorrectImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, L"IMEI", L"NULL", L"1");
							SetDlgItemText(IDC_HINT_STATIC, L"ͨ��");
							adomanage.CloseAll();
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						//���Ⱦ��ϴ�ʧ������
						else
						{
							adomanage.ConnSQL();
							if (judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, L"IMEI", L"IMEI�Ų�ƥ��", L"0");
								adomanage.CloseAll();
								SetDlgItemText(IDC_HINT_STATIC, L"ʧ��");
							}
							else
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, L"IMEI", L"�Ŷ��ڷ�Χ��", L"0");
								adomanage.CloseAll();
								SetDlgItemText(IDC_HINT_STATIC, L"�Ŷ��ڷ�Χ��");
							}
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}
				}
			}


			//����������û��ѡ���жϲʺ�����λ��������

			//���蹴ѡ�˵�����IMEI��
			if (pMsg->hwnd == GetDlgItem(IDC_IMEI3_EDIT)->GetSafeHwnd())
			{
				adomanage.ConnSQL();//�������ݿ�

				//���û�ѡ��Ĳ���IMEI��ʱ�����Ҫ�����û����������ȥ������Ӧ��IMEI�ҳ�������Ϊ������������notypeĿǰ�Ѿ�û����
				if (notype > 0)
				{
					GetDlgItemText(IDC_IMEI3_EDIT, strno3);
					resultflag3 = adomanage.CpImeiByNo(SyllablestrSql, notypename[notype], strno3, strzhidan);//�����û����������ȥ��IMEI

					//����2����ɹ��ҵ�
					if (resultflag3 == 2)
					{
						str3 = adomanage.m_pRecordSet->GetCollect("IMEI");
						SetDlgItemText(IDC_IMEI3_EDIT, str3);
						adomanage.CloseAll();
					}
				}

				GetDlgItemText(IDC_IMEI3_EDIT, str3);
				mm = IsNumber(str3);//�ж�str3�Ƿ���зǷ��ַ�

				//resultflag = adomanage.JudgeImei(str1);

				//������зǷ��ַ���ֱ����ʾ�������
				if (mm == FALSE)
				{
					PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
					SetDlgItemText(IDC_HINT_STATIC, L"�������");
					SetEditEmpty();//��ձ༭��
					GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
					return CDialogEx::PreTranslateMessage(pMsg);
				}

				SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
			}
			//���￪ʼ��ԭ�߼����жϵ�һ����͵ڶ���������

			//�������ʱ��
			if (Imei3Flag == TRUE)
			{
				if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
				{
					adomanage.ConnSQL();//�������ݿ�

					//���û�ѡ��Ĳ���IMEI��ʱ�����Ҫ�����û����������ȥ������Ӧ��IMEI�ҳ�������Ϊ������������notypeĿǰ�Ѿ�û����
					if (notype > 0)
					{
						GetDlgItemText(IDC_IMEI1_EDIT, strno1);
						resultflag1 = adomanage.CpImeiByNo(SyllablestrSql, notypename[notype], strno1, strzhidan);//�����û����������ȥ��IMEI

						//����2����ɹ��ҵ�
						if (resultflag1 == 2)
						{
							str1 = adomanage.m_pRecordSet->GetCollect("IMEI");
							SetDlgItemText(IDC_IMEI1_EDIT, str1);
							adomanage.CloseAll();
						}
					}

					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					mm = IsNumber(str1);//�ж�str1�Ƿ���зǷ��ַ�

					//resultflag = adomanage.JudgeImei(str1);

					//������зǷ��ַ���ֱ����ʾ�������
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
				}
				else if (pMsg->hwnd == GetDlgItem(IDC_IMEI2_EDIT)->GetSafeHwnd())
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);//�Ƚ���һ��IMEI���е����ݷŵ�str1��
					GetDlgItemText(IDC_IMEI3_EDIT, str3);//�ٽ�������IMEI���е����ݷŵ�str3��
					adomanage.ConnSQL();

					if (notype > 0)
					{
						GetDlgItemText(IDC_IMEI2_EDIT, strno2);

						//�������������ȥ����IMEI������0�������ڴ˺ŶΣ�����1��������IMEI������2����ɹ�������3��������Ǹ�IMEI�ţ�ֱ�Ӽ�������ִ����
						resultflag2 = adomanage.CpImeiByNo(SyllablestrSql, notypename[notype], strno2, strzhidan);
						if (resultflag2 == 2)
						{
							str2 = adomanage.m_pRecordSet->GetCollect("IMEI");
							SetDlgItemText(IDC_IMEI2_EDIT, str2);
						}
						//else if (resultflag2 == 0 || resultflag2 == 1)
						else if (resultflag2 == 0)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
							adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							adomanage.CloseAll();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}

					GetDlgItemText(IDC_IMEI2_EDIT, str2);

					//�����ʱ��IMEI�ţ����ж����Ƿ������֣����ǵĻ���ֱ�ӱ������ϴ����ݿ�
					if (notype == 0)
					{
						mm = IsNumber(str1);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							      GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						mm = IsNumber(str2);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							      GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						mm = IsNumber(str3);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}
					mm = IsNumber(str2);

					resultflag1 = adomanage.JudgeImei(str1);

					if (mm == TRUE&&resultflag1 == 1)
					{
						if (str1 == str2&&str2 == str3&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend) && judgeimeirang(str3, strimeistart, strimeiend))
						{
							resultflag2 = adomanage.CpCaiheByImei(str2, strzhidan);

							//���ݷ���ֵ����ʾ����
							switch (resultflag2)
							{
							case 1:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3,strno1, strno2, strpcip, notypename[notype], L"©��ʺ���", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"©��ʺ���");
								break;
							case 2:
								PlaySound(L"ͨ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertCorrectImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"NULL", L"1");
								SetDlgItemText(IDC_HINT_STATIC, L"ͨ��");
								break;
							case 3:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�Ƶ��Ŵ���", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�Ƶ��Ŵ���");
								break;
							case d_SimBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"SIM��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"SIM��ȱ��");
								break;
							case d_VipBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"VIP��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"VIP��ȱ��");
								break;
							case d_IccidBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"ICCID��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"ICCID��ȱ��");
								break;
							case d_BatBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"BAT��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"BAT��ȱ��");
								break;
							case d_MacBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"MAC��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"MAC��ȱ��");
								break;
							case d_EquipmentBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�豸��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�豸��ȱ��");
								break;
							case d_RfidBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"RFIDȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"RFIDȱ��");
								break;
							default:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
								break;
							}

							adomanage.CloseAll();
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						else
						{
							if (judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend) && judgeimeirang(str3, strimeistart, strimeiend))
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3,strno1, strno2, strpcip, notypename[notype], L"IMEI�Ų�ƥ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"ʧ��");
							}
							else
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3,strno1, strno2, strpcip, notypename[notype], L"�Ŷ��ڷ�Χ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�Ŷ��ڷ�Χ��");
							}
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}
					else if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetEditEmpty();//��ձ༭��
						      GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					else
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
						SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
			}
			//�������ʱ��
			else if (Imei3Flag == FALSE)
			{
				if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
				{
					adomanage.ConnSQL();//�������ݿ�

					//���û�ѡ��Ĳ���IMEI��ʱ�����Ҫ�����û����������ȥ������Ӧ��IMEI�ҳ�������Ϊ������������notypeĿǰ�Ѿ�û����
					if (notype > 0)
					{
						GetDlgItemText(IDC_IMEI1_EDIT, strno1);
						resultflag1 = adomanage.CpImeiByNo(SyllablestrSql, notypename[notype], strno1, strzhidan);//�����û����������ȥ��IMEI

						//����2����ɹ��ҵ�
						if (resultflag1 == 2)
						{
							str1 = adomanage.m_pRecordSet->GetCollect("IMEI");
							SetDlgItemText(IDC_IMEI1_EDIT, str1);
							adomanage.CloseAll();
						}
					}

					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					mm = IsNumber(str1);//�ж�str1�Ƿ���зǷ��ַ�

					//resultflag = adomanage.JudgeImei(str1);

					//������зǷ��ַ���ֱ����ʾ�������
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
				}
				else if (pMsg->hwnd == GetDlgItem(IDC_IMEI2_EDIT)->GetSafeHwnd())
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);//�Ƚ���һ��IMEI���е����ݷŵ�str1��
					adomanage.ConnSQL();

					if (notype > 0)
					{
						GetDlgItemText(IDC_IMEI2_EDIT, strno2);

						//�������������ȥ����IMEI������0�������ڴ˺ŶΣ�����1��������IMEI������2����ɹ�������3��������Ǹ�IMEI�ţ�ֱ�Ӽ�������ִ����
						resultflag2 = adomanage.CpImeiByNo(SyllablestrSql, notypename[notype], strno2, strzhidan);
						if (resultflag2 == 2)
						{
							str2 = adomanage.m_pRecordSet->GetCollect("IMEI");
							SetDlgItemText(IDC_IMEI2_EDIT, str2);
						}
						//else if (resultflag2 == 0 || resultflag2 == 1)
						else if (resultflag2 == 0)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
							adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							adomanage.CloseAll();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}

					GetDlgItemText(IDC_IMEI2_EDIT, str2);

					//�����ʱ��IMEI�ţ����ж����Ƿ������֣����ǵĻ���ֱ�ӱ������ϴ����ݿ�
					if (notype == 0)
					{
						mm = IsNumber(str1);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						mm = IsNumber(str2);
						if (mm == FALSE)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							SetDlgItemText(IDC_HINT_STATIC, L"�������");
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}
					mm = IsNumber(str2);

					resultflag1 = adomanage.JudgeImei(str1);

					if (mm == TRUE&&resultflag1 == 1)
					{
						if (str1 == str2&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
						{
							resultflag2 = adomanage.CpCaiheByImei(str2, strzhidan);

							//���ݷ���ֵ����ʾ����
							switch (resultflag2)
							{
							case 1:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"©��ʺ���", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"©��ʺ���");
								break;
							case 2:
								PlaySound(L"ͨ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertCorrectImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"NULL", L"1");
								SetDlgItemText(IDC_HINT_STATIC, L"ͨ��");
								break;
							case 3:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�Ƶ��Ŵ���", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�Ƶ��Ŵ���");
								break;
							case d_SimBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"SIM��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"SIM��ȱ��");
								break;
							case d_VipBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"VIP��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"VIP��ȱ��");
								break;
							case d_IccidBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"ICCID��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"ICCID��ȱ��");
								break;
							case d_BatBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"BAT��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"BAT��ȱ��");
								break;
							case d_MacBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"MAC��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"MAC��ȱ��");
								break;
							case d_EquipmentBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�豸��ȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�豸��ȱ��");
								break;
							case d_RfidBindCheck:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"RFIDȱ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"RFIDȱ��");
								break;
							default:
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
								break;
							}

							adomanage.CloseAll();
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
						else
						{
							if (judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"IMEI�Ų�ƥ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"ʧ��");
							}
							else
							{
								PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
								adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"�Ŷ��ڷ�Χ��", L"0");
								SetDlgItemText(IDC_HINT_STATIC, L"�Ŷ��ڷ�Χ��");
							}
							SetEditEmpty();//��ձ༭��
							GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
							return CDialogEx::PreTranslateMessage(pMsg);
						}
					}
					else if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					else
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						adomanage.InsertWrongImei(strzhidan, str1, str2, str3, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
						SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
						SetEditEmpty();//��ձ༭��
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
			}
			pMsg->wParam = VK_TAB;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//�ж�ɨ��������Ƿ�Ϊ���ֻ�����ĸ
BOOL CMFCP2CPDlg::IsNumber(const CString& strTest)
{
	CString str = strTest;

	// ������Ҫ�������Ƿ�Ҫȥ���ִ����˿ո�TAB���з�
	str.TrimLeft();
	str.TrimRight();

	if (str.IsEmpty())
		return FALSE;

	str.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	str.TrimRight(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	// ���ȥ�����ֺ���ĸ���ִ�Ϊ�գ�˵���ִ���ȫ������ĸ�����֣�������ǷǷ��ַ�
	// ����������һ���ַ�Ϊ0��ǰ���ټ�һ���ж�if(str.GetAt(0) != '0') return FALSE;
	//if ((str.GetAt(0) < '0'||str.GetAt(0)>'9')&& str!="") 
		//return FALSE;
	if (str.IsEmpty())
		return TRUE;

	return FALSE;
}

//�ж�ɨ��������Ƿ�Ϊ����
BOOL CMFCP2CPDlg::IsNumber2(const CString& strTest)
{
	CString str = strTest;

	// ������Ҫ�������Ƿ�Ҫȥ���ִ����˿ո�TAB���з�
	str.TrimLeft();
	str.TrimRight();

	if (str.IsEmpty())
		return FALSE;

	str.TrimLeft(L"0123456789");
	str.TrimRight(L"0123456789");
	// ���ȥ�����֣��ִ�Ϊ�գ�˵���ִ���ȫ��������
	// ����������һ���ַ�Ϊ0��ǰ���ټ�һ���ж�if(str.GetAt(0) != '0') return FALSE;
	//if ((str.GetAt(0) < '0'||str.GetAt(0)>'9')&& str!="") 
	//return FALSE;
	if (str.IsEmpty())
		return TRUE;

	return FALSE;
}

//��IEMI1�༭��û���ݵ�ʱ��̶�����IMEI1�༭��
void CMFCP2CPDlg::OnEnSetfocusImei2Edit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str1;
	GetDlgItemText(IDC_IMEI1_EDIT, str1);
	if (str1 == "")
	{
		GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	}

}

//��IEMI2�༭��û���ݵ�ʱ��̶�����IMEI3�༭��
void CMFCP2CPDlg::OnEnSetfocusImei1Edit()
{
	CString str3;
	GetDlgItemText(IDC_IMEI3_EDIT, str3);
	if (str3 == ""&&Imei3Flag==TRUE)
	{
		GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
	}

}

//�������IMEI���븴ѡ��
void CMFCP2CPDlg::OnBnClickedOpenimei3editCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_OpenImei3EditCheck.GetCheck())
	{
		Imei3Flag = TRUE;
	}
	else
	{
		Imei3Flag = FALSE;
	}
}

//���༭����Ϊ��
void CMFCP2CPDlg::SetEditEmpty()
{
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	SetDlgItemText(IDC_IMEI3_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	strno3 = L"NULL";
}

//���IMEI��ѡ��
void CMFCP2CPDlg::CleanImei3Check()
{
	m_OpenImei3EditCheck.SetCheck(0);
	OnBnClickedOpenimei3editCheck();
}


/*����ȫ�ǵ�ѡ�����¼�*/
void CMFCP2CPDlg::OnBnClickedImeiRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 0;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedSnRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 1;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/SN");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/SN");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedSimRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 2;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/SIM");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/SIM");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedVipRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 3;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/VIP");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/VIP");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedIccidRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 4;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/ICC");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/ICC");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedBatRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 5;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/BAT");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/BAT");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedMacRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 6;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/MAC");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/MAC");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedEquipmentRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 7;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/EQU");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/EQU");
	SetEditEmpty();//��ձ༭��
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}


/*�ֶ�ѡ��ģ��*/
//�������
void CMFCP2CPDlg::OnBnClickedSavesyllableButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Savesyllable();
}

//������¶�ȡ
void CMFCP2CPDlg::OnBnClickedReadsyllableButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Readsyllable(TRUE);
}

//����ֶ�ѡ��ѡ��
void CMFCP2CPDlg::CleanSyllableCheck()
{
	//m_imeiSyllableCheck.SetCheck(0);
	//OnBnClickedImeisyllableCheck();
	m_snSyllableCheck.SetCheck(0);
	OnBnClickedSnsyllableCheck();
	m_simSyllableCheck.SetCheck(0);
	OnBnClickedSimsyllableCheck();
	m_vipSyllableCheck.SetCheck(0);
	OnBnClickedVipsyllableCheck();
	m_iccidSyllableCheck.SetCheck(0);
	OnBnClickedIccidsyllableCheck();
	m_batSyllableCheck.SetCheck(0);
	OnBnClickedBatsyllableCheck();
	m_macSyllableCheck.SetCheck(0);
	OnBnClickedMacsyllableCheck();
	m_equipmentSyllableCheck.SetCheck(0);
	OnBnClickedEquipmentsyllableCheck();
	m_rfidSyllableCheck.SetCheck(0);
	OnBnClickedRfidsyllableCheck();
}


/*��ӡ���IMEI��SIM��ICCID��MAC��Equipment��VIP��BAT*/
//���IMEI�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedImeisyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�����ѡ�У�����ӽ�MAP������������������
	if (m_imeiSyllableCheck.GetCheck()==1)
	{
		SyllableMap[0] = L"IMEI";
	}
	else
	{
		SyllableMap.erase(0);
	}
}

//���SN�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedSnsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_snSyllableCheck.GetCheck()==1)
	{
		SyllableMap[1] = L"SN";
	}
	else
	{
		SyllableMap.erase(1);
	}

}

//���SIM�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedSimsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_simSyllableCheck.GetCheck())
	{
		SyllableMap[2] = L"SIM";
	}
	else
	{
		SyllableMap.erase(2);
	}
}

//���VIP�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedVipsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_vipSyllableCheck.GetCheck())
	{
		SyllableMap[3] = L"VIP";
	}
	else
	{
		SyllableMap.erase(3);
	}
}

//���ICCID�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedIccidsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_iccidSyllableCheck.GetCheck())
	{
		SyllableMap[4] = L"ICCID";
	}
	else
	{
		SyllableMap.erase(4);
	}
}

//���BAT�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedBatsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_batSyllableCheck.GetCheck())
	{
		SyllableMap[5] = L"BAT";
	}
	else
	{
		SyllableMap.erase(5);
	}
}

//���MAC�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedMacsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_macSyllableCheck.GetCheck())
	{
		SyllableMap[6] = L"MAC";
	}
	else
	{
		SyllableMap.erase(6);
	}
}

//���Equipmant�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedEquipmentsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_equipmentSyllableCheck.GetCheck())
	{
		SyllableMap[7] = L"Equipment";
	}
	else
	{
		SyllableMap.erase(7);
	}
}

//���RFID�ֶθ�ѡ��
void CMFCP2CPDlg::OnBnClickedRfidsyllableCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_rfidSyllableCheck.GetCheck())
	{
		SyllableMap[8] = L"RFID";
	}
	else
	{
		SyllableMap.erase(8);
	}
}


/*�󶨹�ϵģ��*/
//�������
void CMFCP2CPDlg::OnBnClickedSavebindButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Savebind();
}

//�����ȡ
void CMFCP2CPDlg::OnBnClickedReadbindButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Readbind(TRUE);

}

//����󶨸�ѡ��
void CMFCP2CPDlg::CleanBindCheck()
{
	m_simBindCheck.SetCheck(0);
	OnBnClickedSimbindCheck();
	m_vipBindCheck.SetCheck(0);
	OnBnClickedVipbindCheck();
	m_iccidBindCheck.SetCheck(0);
	OnBnClickedIccidbindCheck();
	m_batBindCheck.SetCheck(0);
	OnBnClickedBatbindCheck();
	m_macBindCheck.SetCheck(0);
	OnBnClickedMacbindCheck();
	m_equipmentBindCheck.SetCheck(0);
	OnBnClickedEquipmentbindCheck();
	m_rfidBindCheck.SetCheck(0);
	OnBnClickedRfidbindCheck();
}


/*�������IMEI3��SIM����IMEI4��ICCID����IMEI6��MAC����IMEI7��Equipment����IMEI8��VIP����IMEI9��BAT��*/
//���SIM�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedSimbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_simBindCheck.GetCheck())
	{
		BindMap[0] = L"IMEI3";
	}
	else
	{
		BindMap.erase(0);
	}
}

//���VIP�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedVipbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_vipBindCheck.GetCheck())
	{
		BindMap[1] = L"IMEI8";
	}
	else
	{
		BindMap.erase(1);
	}
}

//���ICCID�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedIccidbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_iccidBindCheck.GetCheck())
	{
		BindMap[2] = L"IMEI4";
	}
	else
	{
		BindMap.erase(2);
	}
}

//���BAT�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedBatbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_batBindCheck.GetCheck())
	{
		BindMap[3] = L"IMEI9";
	}
	else
	{
		BindMap.erase(3);
	}
}

//���MAC�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedMacbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_macBindCheck.GetCheck())
	{
		BindMap[4] = L"IMEI6";
	}
	else
	{
		BindMap.erase(4);
	}
}

//���Equipment�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedEquipmentbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_equipmentBindCheck.GetCheck())
	{
		BindMap[5] = L"IMEI7";
	}
	else
	{
		BindMap.erase(5);
	}
}

//���RFID�󶨸�ѡ��
void CMFCP2CPDlg::OnBnClickedRfidbindCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_rfidBindCheck.GetCheck())
	{
		BindMap[6] = L"IMEI8";
	}
	else
	{
		BindMap.erase(6);
	}
}

//������ť
void CMFCP2CPDlg::OnBnClickedLockButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (LockFlag == TRUE)
	{
		INT_PTR nRes;
		CManager *manager = new CManager;
		nRes = manager->DoModal();

		if (IDCANCEL == nRes)
		{
			return;
		}

		else if (IDOK == nRes)
		{
			OtherEnableWindow(TRUE);
			if (m_caihejudgeCheck.GetCheck() == 0)
			{
				RelationEnableWindow(TRUE);
				ChImei3EnableWindow(TRUE);
			}
			ImeiInputEnableWindow(FALSE);
			SetDlgItemText(IDC_LOCK_BUTTON, L"����");
			LockFlag = FALSE;
		}

		SetEditEmpty();//��ձ༭��
		SetDlgItemText(IDC_HINT_STATIC, L"����");
	}
	else if (LockFlag == FALSE)
	{
		//�ȱ�������
		OnBnClickedSavebindButton();
		OnBnClickedSavesyllableButton();
		Saveconfig();

		OtherEnableWindow(FALSE);
		RelationEnableWindow(FALSE);
		ChImei3EnableWindow(FALSE);
		ImeiInputEnableWindow(TRUE);
		SetDlgItemText(IDC_LOCK_BUTTON, L"����");
		LockFlag = TRUE;

		//����ѯ�ֶεĺ���sql�����ǰ���ɳ��������Ч��

		//�����ǲ��ֶε�
		map<int, CString>::iterator SyllableIter;
		//map<int, CString>::iterator BindIter;
		_variant_t Syllabletempvt;
		CString Syllabletemp;
		SyllablestrSql = L"";//������һ��
		notype = SyllableMap.size();

		//����MAP�ĳ���������ѭ������
		for (SyllableIter = CMFCP2CPDlg::SyllableMap.begin(); SyllableIter != CMFCP2CPDlg::SyllableMap.end(); SyllableIter++)
		{
			switch (SyllableIter->first)
			{
			case 0:
				SyllablestrSql += _T("IMEI= 'InputNumber' ");
				break;
			case 1:
				SyllablestrSql += _T("OR SN= 'InputNumber' ");
				break;
			case 2:
				SyllablestrSql += _T("OR SIM= 'InputNumber' ");
				break;
			case 3:
				SyllablestrSql += _T("OR VIP= 'InputNumber' ");
				break;
			case 4:
				SyllablestrSql += _T("OR ICCID= 'InputNumber' ");
				break;
			case 5:
				SyllablestrSql += _T("OR BAT= 'InputNumber' ");
				break;
			case 6:
				SyllablestrSql += _T("OR MAC= 'InputNumber' ");
				break;
			case 7:
				SyllablestrSql += _T("OR Equipment= 'InputNumber' ");
				break;
			case 8:
				SyllablestrSql += _T("OR RFID= 'InputNumber' ");
				break;
			default:
				break;
			}
		}
		if (Imei3Flag == TRUE)
		{
			GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
		}
		else if (Imei3Flag == FALSE)
		{
			GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
		}
	}
}

//ʹ�ֶκͰ󶨿ؼ�ʹ��
void CMFCP2CPDlg::RelationEnableWindow(BOOL chose)
{
	GetDlgItem(IDC_SIMBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_VIPBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_ICCIDBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_BATBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_MACBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_EQUIPMENTBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_RFIDBIND_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_SAVEBIND_BUTTON)->EnableWindow(chose);
	GetDlgItem(IDC_READBIND_BUTTON)->EnableWindow(chose);

	GetDlgItem(IDC_SNSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_SIMSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_VIPSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_ICCIDSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_BATSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_MACSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_EQUIPMENTSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_RFIDSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_SAVESYLLABLE_BUTTON)->EnableWindow(chose);
	GetDlgItem(IDC_READSYLLABLE_BUTTON)->EnableWindow(chose);
}

//ʹ���жϲʺкͿ���IMEI�༭��ؼ�ʹ��
void CMFCP2CPDlg::ChImei3EnableWindow(BOOL chose)
{
	GetDlgItem(IDC_CHJUDGE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_OPENIMEI3EDIT_CHECK)->EnableWindow(chose);
}

//ʹ�������á����жϲʺС����ݿ����ÿؼ�ʹ��
void CMFCP2CPDlg::OtherEnableWindow(BOOL chose)
{
	GetDlgItem(IDC_ZHIDAN_COMBO)->EnableWindow(chose);
	GetDlgItem(IDC_UPDATEORDERNUMBER_BUTTON)->EnableWindow(chose);
	GetDlgItem(IDC_CHJUDGE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(chose);
	GetDlgItem(IDC_OPENIMEI3EDIT_CHECK)->EnableWindow(chose);
}

//ʹIMEI�����ؼ�ʹ��
void CMFCP2CPDlg::ImeiInputEnableWindow(BOOL chose)
{
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(chose);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(chose);

	//��IMEI��ѡ��ѡ�е�ʱ��IMEIɨ���Żᱻ����
	if (Imei3Flag == TRUE)
	{
	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(chose);
	}
	else
	{
	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
	}
}


//�����ʼ������
void CMFCP2CPDlg::fontinit()
{
	editfont.CreatePointFont(400, L"����");
	staticfont1.CreatePointFont(400, L"����");
	staticfont2.CreatePointFont(1400, L"����");
	checkfont.CreatePointFont(150, L"����");

	GetDlgItem(IDC_HINT_STATIC)->SetFont(&staticfont2);
	GetDlgItem(IDC_IMEI1_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_IMEI2_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_IMEI3_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_IMEI1_STATIC)->SetFont(&staticfont1);
	GetDlgItem(IDC_IMEI2_STATIC)->SetFont(&staticfont1);
	GetDlgItem(IDC_IMEI3_STATIC)->SetFont(&staticfont1);

	GetDlgItem(IDC_GROUP1_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_DOWNLOAD_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_WRITENUM_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_FUNTEST_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_COUPLING_CHECK)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP2_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEI_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_SN_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_VIP_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_BAT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_ICCID_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_MAC_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_EQUIPMENT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_BAT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_SIM_RADIO)->SetFont(&checkfont);

	GetDlgItem(IDC_IMEISYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_SNSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_VIPSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_BATSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_ICCIDSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_MACSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_EQUIPMENTSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_BATSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_SIMSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_RFIDSYLLABLE_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_SAVESYLLABLE_BUTTON)->SetFont(&checkfont);
	GetDlgItem(IDC_READSYLLABLE_BUTTON)->SetFont(&checkfont);

	GetDlgItem(IDC_BIND_GROUP)->SetFont(&checkfont);
	GetDlgItem(IDC_SIMBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_VIPBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_BATBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_ICCIDBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_MACBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_EQUIPMENTBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_RFIDBIND_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_SAVEBIND_BUTTON)->SetFont(&checkfont);
	GetDlgItem(IDC_READBIND_BUTTON)->SetFont(&checkfont);

	GetDlgItem(IDC_LOCK_BUTTON)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP3_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_EDIT)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_EDIT)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP3_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_EDIT)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_EDIT)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP4_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_ZHIDAN1_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEISTART_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEIEND_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_ZHIDAN_COMBO)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEISTART_EDIT)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEIOVER_EDIT)->SetFont(&checkfont);
}

//�ı���ɫ�õĺ���
HBRUSH CMFCP2CPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CString str;
	// TODO:  �ڴ˸��� DC ���κ�����
	GetDlgItemText(IDC_HINT_STATIC, str);
	if (pWnd->GetDlgCtrlID() == IDC_HINT_STATIC)
	{
		if (str.Find(L"��ȱ��")!=-1||str == "�������" || str == "ʧ��" || str == "©��ʺ���"||str=="�Ŷβ����ڻ����"||str=="�Ŷ��ڷ�Χ��"||str=="�Ƶ��Ŵ���")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticfont2);
		}
		else if (str == "ͨ��")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticfont2);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//��RGB��ı���ɫ 
			pDC->SelectObject(&staticfont2);
		}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//��ȡ����IP�͵�ַ
int CMFCP2CPDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
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

//���ݿ����ð�ť
void CMFCP2CPDlg::OnBnClickedDbconfigButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

	CDBconfig dbconfigDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = dbconfigDlg.DoModal();  // �����Ի���

	if (IDCANCEL == nRes)
		return;
}


//���¶�����ť
void CMFCP2CPDlg::OnBnClickedUpdateordernumberButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	InitComboBox();
	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
	SetEditEmpty();//��ձ༭��
	SetDlgItemText(IDC_HINT_STATIC, L"����");
	RelationEnableWindow(FALSE);
	ChImei3EnableWindow(FALSE);
	ImeiInputEnableWindow(FALSE);
	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(FALSE);
	CleanSyllableCheck();
	CleanBindCheck();
	CleanImei3Check();
}

//�����ѡ��󴥷�����
void CMFCP2CPDlg::OnCbnSelchangeZhidanCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	readimei();
	SetEditEmpty();//��ձ༭��
	SetDlgItemText(IDC_HINT_STATIC, L"����");
	if (m_caihejudgeCheck.GetCheck() == 0)
	{
		RelationEnableWindow(TRUE);
	}
	ChImei3EnableWindow(TRUE);
	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(TRUE);
	CleanSyllableCheck();
	CleanBindCheck();
	CleanImei3Check();
	Readbind(FALSE);
	Readsyllable(FALSE);
	Readconfig();

	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
}

//����ǰ��س��󴥷�����
void CMFCP2CPDlg::OnCbnSelendokZhidanCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	readimei();
	SetEditEmpty();//��ձ༭��
	SetDlgItemText(IDC_HINT_STATIC, L"����");
	if (m_caihejudgeCheck.GetCheck() == 0)
	{
		RelationEnableWindow(TRUE);
	}
	ChImei3EnableWindow(TRUE);
	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(TRUE);
	CleanSyllableCheck();
	CleanBindCheck();
	CleanImei3Check();
	Readbind(FALSE);
	Readsyllable(FALSE);
	Readconfig();

	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
}

//���ݶ����Ż�ȡIMEI��ʼ�źͽ�����
void CMFCP2CPDlg::readimei()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strWea;
	int nSel;
	nSel = m_zhidanCombo.GetCurSel();
	m_zhidanCombo.GetLBText(nSel, strWea);

	SetDlgItemText(IDC_ZHIDAN_COMBO, strWea);//ѡ�е���һ˲����û�м�ʱ���µ��༭��ģ����Ե��Ȼ�ȡ��ʱ���ݺ���µ��༭����
	_RecordsetPtr GetIMEI;
	GetDlgItemText(IDC_ZHIDAN_COMBO, strzhidan);
	ADOManage adomanage;
	adomanage.ConnSQL();
	GetIMEI = adomanage.GetIMEIByOrderNumber(strzhidan);
	if (GetIMEI->adoEOF)
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
		adomanage.CloseAll();
		return;
	}

	//��IMEI��ȫ�ֺŶλ��ֵ���Ժ�ǵ���ӽ�������ȫ�ֺŶΣ�Ȼ�������Ŷε�ֵ���趨�ڰ��Ǳ���checkbox����
	strimeistart = GetIMEI->GetCollect("IMEIStart");
	strimeiend = GetIMEI->GetCollect("IMEIEnd");
	SetDlgItemText(IDC_IMEISTART_EDIT, strimeistart);
	SetDlgItemText(IDC_IMEIOVER_EDIT, strimeiend);
	adomanage.CloseAll();
}


//������������ʧȥ��������������
void CMFCP2CPDlg::OnCbnKillfocusZhidanCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str1;
	GetDlgItemText(IDC_ZHIDAN_COMBO, str1);

	if (JudgeZhidan(str1))
	{
		if (str1 != "")
		{
		MessageBox(L"�����Ų����ڣ�", L"��ʾ��Ϣ", NULL);
		}
		SetDlgItemText(IDC_ZHIDAN_COMBO, L"");
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
		SetEditEmpty();//��ձ༭��
		InitComboBox();
		RelationEnableWindow(FALSE);
		ChImei3EnableWindow(FALSE);
		GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(FALSE);
		CleanSyllableCheck();
		CleanBindCheck();
		CleanImei3Check();
		return;
	}
	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(TRUE);
	if (m_caihejudgeCheck.GetCheck() == 0)
	{
		RelationEnableWindow(TRUE);
		ChImei3EnableWindow(TRUE);
	}

}

//���жϲʺ�����λ��ѡ�����
void CMFCP2CPDlg::OnBnClickedChjudgeCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (chjudgeflag == 0)
	{
		//GetDlgItem(IDC_IMEI_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_VIP_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_SN_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_SIM_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EQUIPMENT_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_MAC_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_ICCID_RADIO)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BAT_RADIO)->EnableWindow(FALSE);
		//CButton* imeiradio = (CButton*)GetDlgItem(IDC_IMEI_RADIO);
		//CButton* snradio = (CButton*)GetDlgItem(IDC_SN_RADIO);
		//CButton* vipradio = (CButton*)GetDlgItem(IDC_VIP_RADIO);
		//CButton* iccidradio = (CButton*)GetDlgItem(IDC_ICCID_RADIO);
		//CButton* batradio = (CButton*)GetDlgItem(IDC_BAT_RADIO);
		//CButton* macradio = (CButton*)GetDlgItem(IDC_MAC_RADIO);
		//CButton* equipmentradio = (CButton*)GetDlgItem(IDC_EQUIPMENT_RADIO);
		//CButton* simradio = (CButton*)GetDlgItem(IDC_SIM_RADIO);
		//imeiradio->SetCheck(1);
		//snradio->SetCheck(0);
		//vipradio->SetCheck(0);
		//iccidradio->SetCheck(0);
		//batradio->SetCheck(0);
		//macradio->SetCheck(0);
		//equipmentradio->SetCheck(0);
		//simradio->SetCheck(0);
		//OnBnClickedImeiRadio();
		RelationEnableWindow(FALSE);
		chjudgeflag = 1;
	}
	else if (chjudgeflag == 1)
	{
		//GetDlgItem(IDC_IMEI_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_VIP_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SN_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SIM_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EQUIPMENT_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_MAC_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_ICCID_RADIO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BAT_RADIO)->EnableWindow(TRUE);
		RelationEnableWindow(TRUE);
		chjudgeflag = 0;
	}
}

//�ж�IMEI�Ƿ��ڷ�Χ��
BOOL CMFCP2CPDlg::judgeimeirang(CString str, CString strimeistart, CString strimeiend)
{
	//�����޷��ų����ͱ�������IMEI��ʼ����ֹ�����д�������Ҫ�Աȵ�IMEI��
	unsigned long long a = 0, b = 0, c = 0;

	//��������IMEI�ű�IMEI��ʼ�Ŷ�һλ���Ǿ�����У��λ��
	if (strimeistart.GetLength() == 14 && str.GetLength() == 15)
	{
		a = _atoi64(CStringA(strimeistart));
		b = _atoi64(CStringA(strimeiend));
		str = str.Left(str.GetLength() - 1);//�����Ҷ���str.GetLength() - 1���ǽ������ַ������ұ�һλɾ��
		c = _atoi64(CStringA(str));
		if (c >= a && c <= b)
		{
			return TRUE;
		}
		return FALSE;
	}
	//��������Ǿ���û��У��λ��
	else
	{
	    //strimeistart = strimeistart.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	    //strimeiend = strimeiend.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		//str = str.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		//a = _atoi64(CStringA(strimeistart));
		//b = _atoi64(CStringA(strimeiend));
		//c = _atoi64(CStringA(str));
		if (str >= strimeistart && str <= strimeiend&&str.GetLength()==strimeistart.GetLength())
		{
			return TRUE;
		}
		return FALSE;
	}
}


/*���ݿ��������*/
//�ж϶����Ų�������
BOOL CMFCP2CPDlg::JudgeZhidan(CString zhidan)
{
	_RecordsetPtr recordzhidan;
	int flag = 1;
	ADOManage adomanage;
	adomanage.ConnSQL();
	recordzhidan = adomanage.JudgeOrderNumber(zhidan);
	if (!recordzhidan->adoEOF)
	{
		flag = 0;
	}
	adomanage.CloseAll();
	return flag;
}

//��combox������ݿ��еĶ����ţ����°�ť�Ϳ������Զ���ʼ�������������
void CMFCP2CPDlg::InitComboBox()
{
	m_zhidanCombo.ResetContent();
	ADOManage adomanageon;
	adomanageon.ConnSQL();
	adomanageon.m_pRecordSet = adomanageon.GetOrderNumber();
	while (!adomanageon.m_pRecordSet->adoEOF)
	{
		m_zhidanCombo.AddString(adomanageon.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanageon.m_pRecordSet->MoveNext();
	}
	adomanageon.CloseAll();
}

//�����ֶ�ѡ��
void CMFCP2CPDlg::Savesyllable()
{
	BOOL IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID;
	IMEI = m_imeiSyllableCheck.GetCheck();
	SN = m_snSyllableCheck.GetCheck();
	SIM = m_simSyllableCheck.GetCheck();
	VIP = m_vipSyllableCheck.GetCheck();
	ICCID = m_iccidSyllableCheck.GetCheck();
	BAT = m_batSyllableCheck.GetCheck();
	MAC = m_macSyllableCheck.GetCheck();
	Equipment = m_equipmentSyllableCheck.GetCheck();
	RFID = m_rfidSyllableCheck.GetCheck();

	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.Savesyllable(strzhidan, IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID);
	adomanage.CloseAll();
}

//��ȡ�ֶ�ѡ��
void CMFCP2CPDlg::Readsyllable(BOOL CheckEx)
{
	_variant_t imeitemp;//������getcollect������

	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.Readsyllable(strzhidan);
	//���ж����޼�¼���м�¼����ֱ���ж�Ҫ��Ҫ����û��¼�������жϸ��ֶ��Ƿ�ΪNULL�����ж��Ƿ�Ҫ����
	if (adomanage.m_pRecordSet->adoEOF)
	{
		if (CheckEx)
		{
			MessageBox(L"�޴˶������ֶ�ѡ���¼��", L"��ʾ", NULL);
		}
		return;
	}
	else if(!adomanage.m_pRecordSet->adoEOF)
	{
		imeitemp = adomanage.m_pRecordSet->GetCollect("IMEISyllable");
		if (imeitemp.vt == VT_NULL)
		{
			if (CheckEx)
			{
				MessageBox(L"�޴˶������ֶ�ѡ���¼��", L"��ʾ", NULL);
				return;
			}
			else if (!CheckEx)
			{
				return;
			}
		}


	}
	//m_imeiSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("IMEISyllable"));
	//OnBnClickedImeisyllableCheck();

	//���ö�Ӧ�ؼ�״̬
	m_snSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("SNSyllable"));
	OnBnClickedSnsyllableCheck();
	m_simSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("SIMSyllable"));
	OnBnClickedSimsyllableCheck();
	m_vipSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("VIPSyllable"));
	OnBnClickedVipsyllableCheck();
	m_iccidSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("ICCIDSyllable"));
	OnBnClickedIccidsyllableCheck();
	m_batSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("BATSyllable"));
	OnBnClickedBatsyllableCheck();
	m_macSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("MACSyllable"));
	OnBnClickedMacsyllableCheck();
	m_equipmentSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("EquipmentSyllable"));
	OnBnClickedEquipmentsyllableCheck();
	m_rfidSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("RFIDSyllable"));
	OnBnClickedRfidsyllableCheck();
	adomanage.CloseAll();
}

//�����ѡ��
void CMFCP2CPDlg::Savebind()
{
	BOOL SIM, VIP, ICCID, BAT, MAC, Equipment, RFID;
	SIM = m_simBindCheck.GetCheck();
	VIP = m_vipBindCheck.GetCheck();
	ICCID = m_iccidBindCheck.GetCheck();
	BAT = m_batBindCheck.GetCheck();
	MAC = m_macBindCheck.GetCheck();
	Equipment = m_equipmentBindCheck.GetCheck();
	RFID = m_rfidBindCheck.GetCheck();

	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.Savebind(strzhidan, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID);
	adomanage.CloseAll();
}

//��ȡ��ѡ��
void CMFCP2CPDlg::Readbind(BOOL CheckEx)
{
	_variant_t imeitemp;//������getcollect������

	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.Readbind(strzhidan);
	//���ж����޼�¼���м�¼����ֱ���ж�Ҫ��Ҫ����û��¼�������жϸ��ֶ��Ƿ�ΪNULL�����ж��Ƿ�Ҫ����
	if (adomanage.m_pRecordSet->adoEOF)
	{
		if (CheckEx)
		{
			MessageBox(L"�޴˶����Ű󶨼�¼��", L"��ʾ", NULL);
		}
		return;
	}
	else if (!adomanage.m_pRecordSet->adoEOF)
	{
		imeitemp = adomanage.m_pRecordSet->GetCollect("SIMBind");
		if (imeitemp.vt == VT_NULL)
		{
			if (CheckEx)
			{
				MessageBox(L"�޴˶����Ű󶨼�¼��", L"��ʾ", NULL);
				return;
			}
			else if (!CheckEx)
			{
				return;
			}
		}
	}
	//���ö�Ӧ�ؼ�״̬
	m_simBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("SIMBind"));
	OnBnClickedSimbindCheck();
	m_vipBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("VIPBind"));
	OnBnClickedVipbindCheck();
	m_iccidBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("ICCIDBind"));
	OnBnClickedIccidbindCheck();
	m_batBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("BATBind"));
	OnBnClickedBatbindCheck();
	m_macBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("MACBind"));
	OnBnClickedMacbindCheck();
	m_equipmentBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("EquipmentBind"));
	OnBnClickedEquipmentbindCheck();
	m_rfidBindCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("RFIDBind"));
	OnBnClickedRfidbindCheck();
	adomanage.CloseAll();
}

//����һЩ����
void CMFCP2CPDlg::Saveconfig()
{
	BOOL imei3flag;
	imei3flag = m_OpenImei3EditCheck.GetCheck(); 

	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.SaveConfig(strzhidan, imei3flag);
	adomanage.CloseAll();
}

//��ȡһЩ����
void CMFCP2CPDlg::Readconfig()
{
	_variant_t imeitemp;//������getcollect������

	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.Readsyllable(strzhidan);
	//���ж����޼�¼���м�¼����ֱ���ж�Ҫ��Ҫ����û��¼�������жϸ��ֶ��Ƿ�ΪNULL�����ж��Ƿ�Ҫ����
	if (adomanage.m_pRecordSet->adoEOF)
	{
		return;
	}
	else if (!adomanage.m_pRecordSet->adoEOF)
	{
		imeitemp = adomanage.m_pRecordSet->GetCollect("IMEI3Flag");
		if (imeitemp.vt == VT_NULL)
		{
			return;
		}
	}

	//���ö�Ӧ�ؼ�״̬
	m_OpenImei3EditCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("IMEI3Flag"));
	OnBnClickedOpenimei3editCheck();
	adomanage.CloseAll();
}


/*��������*/
void CMFCP2CPDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

