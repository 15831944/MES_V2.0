// CLogin3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "Login3.h"


// CLogin3 �Ի���

IMPLEMENT_DYNAMIC(CLogin3, CDialog)

CLogin3::CLogin3(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin3::IDD, pParent)
{

}

CLogin3::~CLogin3()
{
}

void CLogin3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_name);
	DDX_Text(pDX, IDC_EDIT2, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CLogin3, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin3::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogin3::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLogin3 ��Ϣ�������

void CLogin3::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE) ;

	CString LimitsTemp="";

	if (m_s_name.IsEmpty() || m_s_pwd.IsEmpty())
	{	
		MessageBox("�û��������벻��Ϊ��","�û���¼��Ϣ",MB_ICONWARNING);	 	
	}
	else if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
	//else if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") == TRUE)
	{
		if((m_s_name!="")&& (CheckUser(m_s_name, m_s_pwd, &LimitsTemp) == TRUE))//�˺�1��Ϊ��
		{
			if (LimitsTemp == "")
			{
				AfxMessageBox("�˺�/���벻��ȷ��");
				return;
			}
			else
			{
				UserNameDB = m_s_name;
				OnOK();
			}
		}
	}
	else
	{
		if (g_ADCTFlag != 1)
		{
			// ��ƥ��
			MessageBox("�û��������벻��ȷ", "��ʾ", MB_ICONWARNING);
		}
	}
}


BOOL CLogin3::ConnecDB(CString m_server, CString m_db, CString m_user, CString m_pwd)
{
	if (1)//���ݿ��ַ��Ϊ�գ����ϴ�����
	{
		CString Conn = "";
		CString DB_FAIL = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", m_server, m_db, m_user, m_pwd);
		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			if (g_ADCTFlag == 1)
			{
				HWND ADCTHwnd = NULL;
				ADCTHwnd = ::FindWindow("ADCT", "AutoDownloadATETest");
				MessageBox(_T("�������ݿ�ʧ�ܣ��������ݿ⣬��ǰ���������رգ�"), _T("Warning"), MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK);
				::PostMessage(ADCTHwnd, WM_USER + 1001, 3, 99);
				OnCancel();
				return FALSE;
			}

			if (MessageBox("���ݿ⴦�ڶϿ�״̬��������", "��ܰȷ��", MB_OK | MB_OKCANCEL) == IDOK)
				DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
			else
				return FALSE;
		}

		if (DB_FAIL == "FAIL")
		{
			AfxMessageBox("�������ݿ�ʧ�ܣ���������.......");
			return FALSE;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//MD5 mb;
		//CString CpuOrderTemp = "AT01a#^GT_CM=TEST";
		//mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
		//CString CpuOrderMD5 = (mb.toString()).c_str();

	}
	return TRUE;
}
BOOL CLogin3::GetUserType(CAdoInterface& myado, CString Name, CString Password, CString *Limits)
{
	_variant_t var;
	CString FieldName, FieldValue;
	try
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password  + "'");
		//var= myado.m_pRec->GetCollect(_variant_t((long)5));
		FieldName.Format("UserType");
		var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
		if (var.vt != VT_NULL)
		{
			FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
			*Limits = FieldValue;
		}
		else
		{
			*Limits = "";
		}

	}
	catch (_com_error e)
	{
		*Limits = "";
		myado.CloseSheet();
		return FALSE;
	}
	myado.CloseSheet();
	return TRUE;
}

BOOL CLogin3::CheckUser(CString Name, CString Password, CString* Limits)
{
	if (1)//���ݿ��ַ��Ϊ�գ����ϴ�����
	{

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP = TRUE;

		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)//�����˺�
		{
			if (GetUserType(myado, Name, Password, Limits) == TRUE)
			{

			}
			else
			{
				return FALSE;
			}
				
		}
	}
	return TRUE;
}

void CLogin3::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CString LimitsTemp = "";
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
		//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		if (CheckUser(m_s_name, m_s_pwd, &LimitsTemp) == TRUE)
		{
			if (LimitsTemp.Find("&00") == -1)
			{
				AfxMessageBox("���˺�û�й���Ȩ��");
				return;
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
	CUserSetting UserS;
	UserS.DoModal();
}

BOOL CLogin3::OnInitDialog()
{
	CDialog::OnInitDialog();

    //���ſ������������Systembackdoor����ļ���ip���þͰ������ļ����������Ĭ�Ϲ���IP
	CFileFind finder;
	CString str;
	int ValueInt;

	BOOL ifFind = finder.FindFile(_T(".\\SystemSetting.ini"));
	if (ifFind)
	{
		ValueInt = GetPrivateProfileInt(_T("SimpleSetting"), _T("IsHideFlag"), 0, _T(".\\SystemSetting.ini"));
		g_IsHideFlag = ValueInt;
		ValueInt = GetPrivateProfileInt(_T("SimpleSetting"), _T("ADCTFlag"), 0, _T(".\\SystemSetting.ini"));
		g_ADCTFlag = ValueInt;
	}

	ifFind = finder.FindFile(_T(".\\Systembackdoor.ini"));
	if (ifFind)
	{

		GetPrivateProfileString(_T("DatabaseInfo"), _T("IP"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorIP = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Database"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorDatabase = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("User"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorUser = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorPassword = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Socket"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorSocket = str;
		str.ReleaseBuffer();
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����һʵ���Զ���½
	if (g_ADCTFlag == 1)
	{
		SetDlgItemText(IDC_EDIT1, "1");
		SetDlgItemText(IDC_EDIT2, "1");
		OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
