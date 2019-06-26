// DBConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JIMI.h"
#include "DBConfig.h"
#include "afxdialogex.h"
#include "ADOManage.h"


// DBConfig �Ի���

IMPLEMENT_DYNAMIC(DBConfig, CDialogEx)

DBConfig::DBConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DATABASE, pParent)
	, m_serveripEdit(_T(""))
	, m_portEdit(_T(""))
	, m_usernameEdit(_T(""))
	, m_passwordEdit(_T(""))
	, m_seconddbnameEdit(_T(""))
	, m_firstdbnameEdit(_T(""))
	, m_secondformnameEdit(_T(""))
	, m_firstformnameEdit(_T(""))
{

}

DBConfig::~DBConfig()
{
}

void DBConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVERIP_EDIT, m_serveripEdit);
	DDX_Text(pDX, IDC_PORT_EDIT, m_portEdit);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_usernameEdit);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_passwordEdit);
	//DDX_Text(pDX, IDC_FIRSTDBNAME_EDIT, m_firstdbnameEdit);
	//DDX_Text(pDX, IDC_FIRSTFORMNAME_EDIT, m_firstformnameEdit);
	//DDX_Text(pDX, IDC_SECONDDBNAME_EDIT, m_seconddbnameEdit);
	//DDX_Text(pDX, IDC_SECONDFORMNAME_EDIT, m_secondformnameEdit);
}


BEGIN_MESSAGE_MAP(DBConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &DBConfig::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL DBConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	Readdbini();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// DBConfig ��Ϣ�������


void DBConfig::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//дINI
	CString str;
	ADOManage adoManage;
	CFileFind finder;
	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	GetDlgItemText(IDC_SERVERIP_EDIT, m_serveripEdit);
	adoManage.m_DataSource = m_serveripEdit;  //������IP
	GetDlgItemText(IDC_PORT_EDIT, m_portEdit);
	adoManage.m_Port = m_portEdit;        //�������˿�
	GetDlgItemText(IDC_USERNAME_EDIT, m_usernameEdit);
	adoManage.m_UserName = m_usernameEdit;    //�û���
	GetDlgItemText(IDC_PASSWORD_EDIT, m_passwordEdit);
	adoManage.m_Password = m_passwordEdit;    //����
	//GetDlgItemText(IDC_FIRSTDBNAME_EDIT, m_firstdbnameEdit);
	//adoManage.m_Firstdbname = m_firstdbnameEdit;    //��һ�����ݿ�����
	//GetDlgItemText(IDC_FIRSTFORMNAME_EDIT, m_firstformnameEdit);
	//adoManage.m_Firstformname = m_firstformnameEdit;  //��һ�����ݱ�����
	//GetDlgItemText(IDC_SECONDDBNAME_EDIT, m_seconddbnameEdit);
	//adoManage.m_Seconddbname = m_seconddbnameEdit;  //�ڶ������ݿ�����
	//GetDlgItemText(IDC_SECONDFORMNAME_EDIT, m_secondformnameEdit);
	//adoManage.m_Secondformname = m_secondformnameEdit;//�ڶ������ݱ�����

	if (!ifFind)
	{
		GetDlgItemText(IDC_SERVERIP_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_PORT_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Port"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_USERNAME_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("UserName"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_PASSWORD_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Password"), str, _T(".\\SystemInfo.ini"));
		//GetDlgItemText(IDC_FIRSTDBNAME_EDIT, str);
		//WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), str, _T(".\\SystemInfo.ini"));
		//GetDlgItemText(IDC_FIRSTFORMNAME_EDIT, str);
		//WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), str, _T(".\\SystemInfo.ini"));
		//GetDlgItemText(IDC_SECONDDBNAME_EDIT, str);
		//WritePrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), str, _T(".\\SystemInfo.ini"));
		//GetDlgItemText(IDC_SECONDFORMNAME_EDIT, str);
		//WritePrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), str, _T(".\\SystemInfo.ini"));
	}

	try
	{
		if (adoManage.ConndbSQL())
		{
			GetDlgItemText(IDC_SERVERIP_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_PORT_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Port"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_USERNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("UserName"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_PASSWORD_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Password"), str, _T(".\\SystemInfo.ini"));
			/*GetDlgItemText(IDC_FIRSTDBNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_FIRSTFORMNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_SECONDDBNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_SECONDFORMNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), str, _T(".\\SystemInfo.ini"));*/
			CDialogEx::OnCancel();
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
	CDialogEx::OnOK();
}

void DBConfig::Readdbini()
{
	CFileFind finder;
	CString str;

	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	if (ifFind)
	{
		GetPrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_serveripEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Port"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_portEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("UserName"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_usernameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_passwordEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_firstdbnameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_firstformnameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_seconddbnameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_secondformnameEdit = str;
		UpdateData(FALSE);
		str.ReleaseBuffer();
	}
	return;
}


