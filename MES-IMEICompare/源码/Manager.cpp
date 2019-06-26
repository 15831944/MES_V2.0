// Manager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP2CP.h"
#include "MFCP2CPDlg.h"
#include "Manager.h"
#include "afxdialogex.h"
#include "ADOManage.h"

// CManager �Ի���

IMPLEMENT_DYNAMIC(CManager, CDialogEx)

CManager::CManager(CWnd* pParent /*=NULL*/)
: CDialogEx(CManager::IDD, pParent)
{

}

CManager::~CManager()
{
}

void CManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManager, CDialogEx)
	ON_BN_CLICKED(IDOK, &CManager::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CManager::OnBnClickedCancel)
END_MESSAGE_MAP()


// CManager ��Ϣ�������

BOOL CManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ������ģ��
	SetDlgItemText(IDC_PASSWORD_STATIC, L"����:");
	SetDlgItemText(IDOK, L"ȷ��");
	SetDlgItemText(IDCANCEL, L"ȡ��");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CManager::OnBnClickedOk()
{
	ADOManage adomanageCheckUser;
	CString name,password,ChcekResult;
	GetDlgItemText(IDC_USERNAME_EDIT, name);
	GetDlgItemText(IDC_MANAGERPASSWORD_EDIT, password);
	
	if (name == "" || password == "")
	{
		MessageBox(L"�˺Ż����벻��Ϊ�գ�", L"��ʾ��Ϣ", NULL);
		return;
	}
	adomanageCheckUser.ConnSQL();
	ChcekResult = adomanageCheckUser.CheckUserAuthority(name, password);
	adomanageCheckUser.CloseAll();
	if (ChcekResult == L"CheckFail")
	{
		MessageBox(L"�˺Ż��������", L"��ʾ��Ϣ", NULL);
		return;
	}
	else if (ChcekResult.Find(g_UserTypeNeed) != -1)
	{
		g_UserNameStr = name;
		g_CheckUserType = ChcekResult; 
		CDialogEx::OnOK();
	}
	else
	{
		MessageBox(L"��ǰ�˺�û��Ȩ�ޣ�", L"��ʾ��Ϣ", NULL);
		return;
	}


	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void CManager::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
