// SetImeiRangeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCP1LD.h"
#include "SetImeiRangeDlg.h"
#include "afxdialogex.h"
#include "MFCP1LDDlg.h"

// CSetImeiRangeDlg �Ի���

IMPLEMENT_DYNAMIC(CSetImeiRangeDlg, CDialogEx)

CSetImeiRangeDlg::CSetImeiRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetImeiRangeDlg::IDD, pParent)
	, m_imeirangestartEdit(1)
	, m_imeirangeendEdit(1)
{

}

CSetImeiRangeDlg::~CSetImeiRangeDlg()
{
}

void CSetImeiRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMEIRANGESTART_EDIT, m_imeirangestartEdit);
	DDX_Text(pDX, IDC_IMEIRANGEEND_EDIT, m_imeirangeendEdit);
}


BEGIN_MESSAGE_MAP(CSetImeiRangeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetImeiRangeDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_IMEIRANGESTART_EDIT, &CSetImeiRangeDlg::OnEnKillfocusImeirangestartEdit)
	ON_EN_KILLFOCUS(IDC_IMEIRANGEEND_EDIT, &CSetImeiRangeDlg::OnEnKillfocusImeirangeendEdit)
END_MESSAGE_MAP()


// CSetImeiRangeDlg ��Ϣ�������
BOOL CSetImeiRangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMFCP1LDDlg *parent = (CMFCP1LDDlg *)GetParent();
	//�ȰѶ������������ó���Ȼ������ת��
	parent->m_imeistartshowonlyEdit.GetWindowText(m_imeirangestart);
	parent->m_imeiendshowonlyEdit.GetWindowText(m_imeirangeend);

	unsigned long long a = _atoi64(CStringA(m_imeirangestart));
	unsigned long long b = _atoi64(CStringA(m_imeirangeend));

	m_imeirangestartNo = 1;
	m_imeirangeendNo = b-a+1;

	//������ֺ��ٶ���ȥ
	SetDlgItemInt(IDC_IMEIRANGESTART_EDIT,m_imeirangestartNo);
	SetDlgItemInt(IDC_IMEIRANGEEND_EDIT, m_imeirangeendNo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//����ʼλ�༭�򱻸��ĺ����Ĳ���
void CSetImeiRangeDlg::OnEnKillfocusImeirangestartEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int imeistart, imeiend;
	imeistart=GetDlgItemInt(IDC_IMEIRANGESTART_EDIT);
	imeiend = GetDlgItemInt(IDC_IMEIRANGEEND_EDIT);
	if (imeistart >= m_imeirangestartNo&&imeistart<=imeiend)
	{
		return;
	}
	else
	{
		MessageBox(L"��ʼ�Ŷ����ô���",L"��ʾ��Ϣ",NULL);
		SetDlgItemInt(IDC_IMEIRANGESTART_EDIT,m_imeirangestartNo);
		return;
	}
}

//������λ�༭�򱻸��ĺ����Ĳ���
void CSetImeiRangeDlg::OnEnKillfocusImeirangeendEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int imeistart, imeiend;
	imeistart = GetDlgItemInt(IDC_IMEIRANGESTART_EDIT);
	imeiend = GetDlgItemInt(IDC_IMEIRANGEEND_EDIT);
	if (imeiend <= m_imeirangeendNo&&imeistart <= imeiend)
	{
		return;
	}
	else
	{
		MessageBox(L"�����Ŷ����ô���", L"��ʾ��Ϣ", NULL);
		SetDlgItemInt(IDC_IMEIRANGEEND_EDIT, m_imeirangeendNo);
		return;
	}
}

//���ȷ����ť��Ҫ���Ĳ�������ʼ���ǰ����ݴ���������
void CSetImeiRangeDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ٽ�IMEI�Ž��мӼ�Ȼ��ת����󶪻�ȥ
	CMFCP1LDDlg *parent = (CMFCP1LDDlg *)GetParent();
	int imeistart, imeiend;
	imeistart = GetDlgItemInt(IDC_IMEIRANGESTART_EDIT);
	imeiend = GetDlgItemInt(IDC_IMEIRANGEEND_EDIT);

	unsigned long long a = _atoi64(CStringA(m_imeirangestart));
	unsigned long long b;

	a = a + imeistart - 1;//����IMEI��ʼλ8006���Ŷ���ʼ����Ϊ1���Ǿ���8006+1-1=8006;
	b = a + imeiend - 1;//����Ŷ����ý���Ϊ300���Ǿ�����ʼλ8006+300-1=8295;����˵Ҫ�ص�1~300����Ҫ��300���ţ���8295-8006+1=300��û��ġ�

	m_imeirangestart = _ui64toa(a, CT2A(m_imeirangestart), 10);
	m_imeirangeend= _ui64toa(b,CT2A(m_imeirangeend),10);

	parent->m_imeistartEdit.SetWindowText(m_imeirangestart);
	parent->m_imeiendEdit.SetWindowText(m_imeirangeend);

	CDialogEx::OnCancel();
}

