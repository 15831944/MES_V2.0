// Login2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "Login2.h"


// CLogin2 �Ի���

IMPLEMENT_DYNAMIC(CLogin2, CDialog)

CLogin2::CLogin2(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin2::IDD, pParent)
{

}

CLogin2::~CLogin2()
{
}

void CLogin2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_name);
	DDX_Text(pDX, IDC_EDIT2, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CLogin2, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin2::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogin2 ��Ϣ�������


void CLogin2::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE) ;

	if (m_s_name.IsEmpty() || m_s_pwd.IsEmpty())
	{	
		LANDING_OK=FALSE;
		MessageBox("�û��������벻��Ϊ��","�û���¼��Ϣ",MB_ICONWARNING);	 	
	}
	else if (m_s_name=="xx"&& m_s_pwd=="12")
	{
		   LANDING_OK=TRUE;		   
		   UpdateData(true);      //���ؼ���ֵ��������
		   EndDialog(0);
		   
	}
	else //if(m_s_name!="desay"|| m_s_pwd!="desay_unlock")
	{	
		   // ��ƥ��
		LANDING_OK=FALSE;
		MessageBox("�û��������벻��ȷ","��ʾ", MB_ICONWARNING);
	}
	//OnOK();
}


BOOL CLogin2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����һʵ���Զ���½,�Զ�����
	if (g_ADCTFlag == 1)
	{
		SetDlgItemText(IDC_EDIT1, "xx");
		SetDlgItemText(IDC_EDIT2, "12");
		OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
