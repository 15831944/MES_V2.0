// DeleteDB_Data_Confirm.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "DeleteDB_Data_Confirm.h"


// CDeleteDB_Data_Confirm �Ի���

IMPLEMENT_DYNAMIC(CDeleteDB_Data_Confirm, CDialog)

CDeleteDB_Data_Confirm::CDeleteDB_Data_Confirm(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteDB_Data_Confirm::IDD, pParent)
	, m_s_pwd(_T(""))
{

}

CDeleteDB_Data_Confirm::~CDeleteDB_Data_Confirm()
{
}

void CDeleteDB_Data_Confirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CDeleteDB_Data_Confirm, CDialog)
	ON_BN_CLICKED(IDOK, &CDeleteDB_Data_Confirm::OnBnClickedOk)
END_MESSAGE_MAP()


// CDeleteDB_Data_Confirm ��Ϣ�������

void CDeleteDB_Data_Confirm::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PassWord_Right=false;
	UpdateData(TRUE) ;
	if (m_s_pwd.IsEmpty())
	{	
		MessageBox("�����Ϊ��","Ȩ����Ϣ",MB_ICONWARNING);	 	
	}
	else if (m_s_pwd=="88888666")
	{
		PassWord_Right=true;
		EndDialog(0);
	}
	else //if(m_s_name!="desay"|| m_s_pwd!="desay_unlock")
	{	
		   // ��ƥ��
		MessageBox("�����ȷ","��ʾ", MB_ICONWARNING);
	}
	//OnOK();
}
