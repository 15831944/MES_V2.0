#pragma once


// CLogin2 �Ի���

class CLogin2 : public CDialog
{
	DECLARE_DYNAMIC(CLogin2)

public:
	CLogin2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin2();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_s_name;
	CString m_s_pwd;
	BOOL LANDING_OK;
	virtual BOOL OnInitDialog();
};
