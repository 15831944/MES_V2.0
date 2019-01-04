#pragma once
#include "AdoInterface.h"
//#include "md5.h"
#include "afxwin.h"

// CUserSetting �Ի���

class CUserSetting : public CDialog
{
	DECLARE_DYNAMIC(CUserSetting)

public:
	CUserSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserSetting();

// �Ի�������
	enum { IDD = IDD_USERSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	CString GetData(char* Serial_Order_Return, CString Start, CString End, int Count = 1, int HandleNum = 0);//��ȡ��Ч����
	afx_msg void OnTimer(UINT nIDEvent);//��ʱ��
public:
	CAdoInterface myado;
public:
	BOOL ConnecDB(CString m_server, CString m_db, CString m_user, CString m_pwd);
	BOOL GetUserType(CAdoInterface& myado, CString Name, CString Password, CString *Limits);
	BOOL UpdateUserToDb(CString Name, CString Password, CString Limits,BOOL ChangePw=FALSE, CString NewPassword="");
	BOOL DeleteUserFromDb(CString Name, CString Password);
public:
	CStringArray UserArray;
public:
	BOOL GetUsersFromDB();
	void ChangeCombo();
public:
	afx_msg void OnBnClickedButton1();
	// �û�
	CString UserName;
	CString Password;
	CString NewPassword;
	CString Limits;		//Ȩ��
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton10();
	CComboBox LimitsControl;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox AllUsersBox;
};
