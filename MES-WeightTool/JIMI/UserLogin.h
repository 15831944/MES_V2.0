#pragma once


// UserLogin �Ի���

class UserLogin : public CDialogEx
{
	DECLARE_DYNAMIC(UserLogin)

public:
	UserLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UserLogin();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_UserName;
	CString m_UserPswd;

	CString sUSER;
	CString sUSERRight;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
