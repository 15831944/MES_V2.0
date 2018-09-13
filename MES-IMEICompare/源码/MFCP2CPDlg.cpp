
// MFCP2CPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP2CP.h"
#include "MFCP2CPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include "DBconfig.h"
#include "ADOManage.h"

#pragma comment(lib,"winmm.lib")

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
	, strzhidan(L"")
	, chjudgeflag(0)
	, strimeistart(L"")
	, strimeiend(L"")
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
	CString str1, str2,str3;
	int mm;
	int resultflag1, resultflag2;
	ADOManage adomanage;
	
	GetDlgItemText(IDC_IMEI1_EDIT, str1);
	if (str1 == "")
	{
		return CDialogEx::PreTranslateMessage(pMsg);
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
				if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					mm = IsNumber(str1);//�ж�str1�Ƿ���зǷ��ַ�

					//������зǷ��ַ���ֱ����ʾ�������
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
				}
				else
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					GetDlgItemText(IDC_IMEI2_EDIT, str2);

					//���ж��������Ƿ��Ǵ����֣����ǵĻ���ֱ�ӱ������ϴ����ݿ�
					mm = IsNumber(str1);
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					mm = IsNumber(str2);
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					//���ж��Ƿ����
					if (str1 == str2&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
					{
						adomanage.ConnSQL();

						if (adomanage.JudgeZhidan(str1,strzhidan))
						{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"�Ƶ��Ŵ���", L"0");
						adomanage.CloseAll();
						SetDlgItemText(IDC_HINT_STATIC, L"�Ƶ��Ŵ���");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
						}

						PlaySound(L"ͨ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						adomanage.InsertCorrectImei(strzhidan, str1, str2, strno1, strno2, strpcip, L"IMEI", L"NULL", L"1");
						SetDlgItemText(IDC_HINT_STATIC, L"ͨ��");
						adomanage.CloseAll();
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					//���Ⱦ��ϴ�ʧ������
					else
					{
						adomanage.ConnSQL();
						if (judgeimeirang(str2, strimeistart, strimeiend))
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, L"IMEI", L"IMEI�Ų�ƥ��", L"0");
							adomanage.CloseAll();
							SetDlgItemText(IDC_HINT_STATIC, L"ʧ��");
						}
						else
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, L"IMEI", L"�Ŷ��ڷ�Χ��", L"0");
							adomanage.CloseAll();
							SetDlgItemText(IDC_HINT_STATIC, L"�Ŷ��ڷ�Χ��");
						}
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
			}

			//����������û��ѡ���жϲʺ�����λ��������
			if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
			{
				adomanage.ConnSQL();//�������ݿ�

				//���û�ѡ��Ĳ���IMEI��ʱ�����Ҫ�����û����������ȥ������Ӧ��IMEI�ҳ���
				if (notype > 0)
				{
					GetDlgItemText(IDC_IMEI1_EDIT, strno1);
					resultflag1 = adomanage.CpImeiByNo(notypename[notype], strno1,strzhidan);//�����û����������ȥ��IMEI
					//����2����ɹ��ҵ�
					if (resultflag1 == 2)
					{
						str1 = adomanage.m_pRecordSet->GetCollect("IMEI");
						SetDlgItemText(IDC_IMEI1_EDIT, str1);
						adomanage.CloseAll();
					}
					//����3�������Ǹ�IMEI������0�ͷ���1������IMEI�����ڻ����û���������ݲ�����
					else if (resultflag1 == 3)
					{
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
					SetDlgItemText(IDC_IMEI1_EDIT, L"");
					SetDlgItemText(IDC_IMEI2_EDIT, L"");
					GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
					strno1 = L"NULL";
					return CDialogEx::PreTranslateMessage(pMsg);
				}

				SetDlgItemText(IDC_HINT_STATIC, L"�ȴ�");
			}
			else
			{
				GetDlgItemText(IDC_IMEI1_EDIT, str1);//�Ƚ���һ��IMEI���е����ݷŵ�str1��
				adomanage.ConnSQL();

				if (notype > 0)
				{
					GetDlgItemText(IDC_IMEI2_EDIT, strno2);

					//�������������ȥ����IMEI������0�������ڴ˺ŶΣ�����1��������IMEI������2����ɹ�������3��������Ǹ�IMEI��
					resultflag2 = adomanage.CpImeiByNo(notypename[notype], strno2, strzhidan);
					if (resultflag2 == 2)
					{
						str2 = adomanage.m_pRecordSet->GetCollect("IMEI");
						SetDlgItemText(IDC_IMEI2_EDIT, str2);
					}
					else if (resultflag2 == 0 || resultflag2 == 1)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
						adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						adomanage.CloseAll();
						strno1 = L"NULL";
						strno2 = L"NULL";
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
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					mm = IsNumber(str2);
					if (mm == FALSE)
					{
						PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"�������");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
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
						if (resultflag2 == 2)
						{
							PlaySound(L"ͨ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertCorrectImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"NULL", L"1");
							SetDlgItemText(IDC_HINT_STATIC, L"ͨ��");
						}
						else if (resultflag2 == 1)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"©��ʺ���", L"0");
							strno1 = L"NULL";
							strno2 = L"NULL";
							SetDlgItemText(IDC_HINT_STATIC, L"©��ʺ���");
						}
						else if (resultflag2 == 3)
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"�Ƶ��Ŵ���", L"0");
							strno1 = L"NULL";
							strno2 = L"NULL";
							SetDlgItemText(IDC_HINT_STATIC, L"�Ƶ��Ŵ���");
						}
						else
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
							strno1 = L"NULL";
							strno2 = L"NULL";
							SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
						}

						adomanage.CloseAll();
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					else
					{
						if (judgeimeirang(str2, strimeistart, strimeiend))
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"IMEI�Ų�ƥ��", L"0");
							SetDlgItemText(IDC_HINT_STATIC, L"ʧ��");
						}
						else
						{
							PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"�Ŷ��ڷ�Χ��", L"0");
							SetDlgItemText(IDC_HINT_STATIC, L"�Ŷ��ڷ�Χ��");
						}
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
				else if (mm == FALSE)
				{
					PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
					SetDlgItemText(IDC_HINT_STATIC, L"�������");
					SetDlgItemText(IDC_IMEI1_EDIT, L"");
					SetDlgItemText(IDC_IMEI2_EDIT, L"");
					GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
					strno1 = L"NULL";
					return CDialogEx::PreTranslateMessage(pMsg);
				}
				else
				{
					PlaySound(L"ʧ��_�Ա�ʧ��.wav", NULL, SND_FILENAME | SND_ASYNC);
					adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"����Ŷβ����ڻ����", L"0");
					SetDlgItemText(IDC_HINT_STATIC, L"�Ŷβ����ڻ����");
					SetDlgItemText(IDC_IMEI1_EDIT, L"");
					SetDlgItemText(IDC_IMEI2_EDIT, L"");
					GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
					strno1 = L"NULL";
					strno2 = L"NULL";
					return CDialogEx::PreTranslateMessage(pMsg);
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

//����ȫ�ǵ�ѡ�����¼�
void CMFCP2CPDlg::OnBnClickedImeiRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 0;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedSnRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 1;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/SN");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/SN");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedSimRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 2;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/SIM");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/SIM");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedVipRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 3;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/VIP");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/VIP");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedIccidRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 4;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/ICC");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/ICC");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedBatRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 5;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/BAT");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/BAT");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedMacRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 6;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/MAC");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/MAC");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
}

void CMFCP2CPDlg::OnBnClickedEquipmentRadio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	notype = 7;
	SetDlgItemText(IDC_IMEI1_STATIC, L"����IMEI/EQU");
	SetDlgItemText(IDC_IMEI2_STATIC, L"�ʺ�IMEI/EQU");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
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
	GetDlgItem(IDC_IMEI1_STATIC)->SetFont(&staticfont1);
	GetDlgItem(IDC_IMEI2_STATIC)->SetFont(&staticfont1);

	GetDlgItem(IDC_GROUP1_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_DOWNLOAD_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_WRITENUM_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_FUNTEST_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_COUPLING_CHECK)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP2_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_SN_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_VIP_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_BAT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_ICCID_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_MAC_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_EQUIPMENT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_BAT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_SIM_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEI_RADIO)->SetFont(&checkfont);

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
		if (str == "�������" || str == "ʧ��" || str == "©��ʺ���"||str=="�Ŷβ����ڻ����"||str=="�Ŷ��ڷ�Χ��"||str=="�Ƶ��Ŵ���")
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

//���¶�����ť
void CMFCP2CPDlg::OnBnClickedUpdateordernumberButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	InitComboBox();
	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(FALSE);
}

//�����ѡ��󴥷�����
void CMFCP2CPDlg::OnCbnSelchangeZhidanCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	readimei();
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
}

//����ǰ��س��󴥷�����
void CMFCP2CPDlg::OnCbnSelendokZhidanCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	readimei();
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"����");
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(TRUE);
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

//�ص��ж϶����Ų�������
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

//������������ʧȥ��������������
void CMFCP2CPDlg::OnCbnKillfocusZhidanCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str1;
	GetDlgItemText(IDC_ZHIDAN_COMBO, str1);

	if (JudgeZhidan(str1))
	{
		MessageBox(L"�����Ų����ڣ�", L"��ʾ��Ϣ", NULL);
		SetDlgItemText(IDC_ZHIDAN_COMBO, L"");
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
		SetDlgItemText(IDC_IMEI1_EDIT, L"");
		SetDlgItemText(IDC_IMEI2_EDIT, L"");
		strno1 = L"NULL";
		strno2 = L"NULL";
		InitComboBox();
		GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(FALSE);
		return;
	}
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(TRUE);
}

//���жϲʺ�����λ��ѡ�����
void CMFCP2CPDlg::OnBnClickedChjudgeCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (chjudgeflag == 0)
	{
		GetDlgItem(IDC_IMEI_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIP_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SN_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SIM_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EQUIPMENT_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAC_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_ICCID_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BAT_RADIO)->EnableWindow(FALSE);
		CButton* imeiradio = (CButton*)GetDlgItem(IDC_IMEI_RADIO);
		CButton* snradio = (CButton*)GetDlgItem(IDC_SN_RADIO);
		CButton* vipradio = (CButton*)GetDlgItem(IDC_VIP_RADIO);
		CButton* iccidradio = (CButton*)GetDlgItem(IDC_ICCID_RADIO);
		CButton* batradio = (CButton*)GetDlgItem(IDC_BAT_RADIO);
		CButton* macradio = (CButton*)GetDlgItem(IDC_MAC_RADIO);
		CButton* equipmentradio = (CButton*)GetDlgItem(IDC_EQUIPMENT_RADIO);
		CButton* simradio = (CButton*)GetDlgItem(IDC_SIM_RADIO);
		imeiradio->SetCheck(1);
		snradio->SetCheck(0);
		vipradio->SetCheck(0);
		iccidradio->SetCheck(0);
		batradio->SetCheck(0);
		macradio->SetCheck(0);
		equipmentradio->SetCheck(0);
		simradio->SetCheck(0);
		OnBnClickedImeiRadio();
		chjudgeflag = 1;
	}
	else if (chjudgeflag == 1)
	{
		GetDlgItem(IDC_IMEI_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIP_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SN_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SIM_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EQUIPMENT_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_MAC_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_ICCID_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BAT_RADIO)->EnableWindow(TRUE);
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


//��������
void CMFCP2CPDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}
