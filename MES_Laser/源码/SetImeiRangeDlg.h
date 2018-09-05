#pragma once
#include "afxwin.h"


// CSetImeiRangeDlg �Ի���

class CSetImeiRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetImeiRangeDlg)

public:
	CSetImeiRangeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetImeiRangeDlg();

// �Ի�������
	enum { IDD = IDD_IMEIRANGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	int m_imeirangestartEdit;
	int m_imeirangeendEdit;

//�Զ������
public:
	CString m_imeirangestart;
	CString m_imeirangeend;
	int m_imeirangestartNo;
	int m_imeirangeendNo;

	afx_msg void OnEnKillfocusImeirangestartEdit();
	afx_msg void OnEnKillfocusImeirangeendEdit();
};
