// Manager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP4ECurrentTool.h"
#include "MFCP4ECurrentToolDlg.h"
#include "Manager.h"
#include "afxdialogex.h"

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
	SetDlgItemText(IDC_PASSWORD_STATIC, L"����������:");
	SetDlgItemText(IDOK, L"ȷ��");
	SetDlgItemText(IDCANCEL, L"ȡ��");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CManager::OnBnClickedOk()
{
	CString password;
	GetDlgItemText(IDC_MANAGERPASSWORD_EDIT, password);

	if (password != L"jimi")
	{
		MessageBox(L"�������", L"��ʾ��Ϣ", NULL);
		return;
	}

	CDialogEx::OnOK();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void CManager::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
