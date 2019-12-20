// UserLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JIMI.h"
#include "UserLogin.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "JIMIDLG.h"


// UserLogin �Ի���

IMPLEMENT_DYNAMIC(UserLogin, CDialogEx)

UserLogin::UserLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, m_UserName(_T(""))
	, m_UserPswd(_T(""))
	, sUSER(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_JIMI);
}

UserLogin::~UserLogin()
{
}

void UserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UserName, m_UserName);
	DDX_Text(pDX, IDC_EDIT_UserPswd, m_UserPswd);
}


BEGIN_MESSAGE_MAP(UserLogin, CDialogEx)
	ON_BN_CLICKED(IDOK, &UserLogin::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// UserLogin ��Ϣ�������


void UserLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (_T("") == m_UserName || _T("") == m_UserPswd)
	{
		AfxMessageBox(_T("�˺����벻��Ϊ��"));
		return;
	}

	ADOManage adoManage;
	if (!adoManage.ConnSQL())
	{
		return;
	}	
	sUSER = adoManage.CheckUser(m_UserName, m_UserPswd);
	if (_T("") == sUSER)
	{
		AfxMessageBox(_T("�˺��������"));
		return;
	}

	UpdateData(FALSE);

	CDialogEx::OnOK();
}


BOOL UserLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	GetDlgItem(IDC_EDIT_UserName)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void UserLogin::OnPaint()
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
