#pragma once


// CDeleteDB_Data_Confirm �Ի���

class CDeleteDB_Data_Confirm : public CDialog
{
	DECLARE_DYNAMIC(CDeleteDB_Data_Confirm)

public:
	CDeleteDB_Data_Confirm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeleteDB_Data_Confirm();

// �Ի�������
	enum { IDD = IDD_Delete_DB_Data };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	bool PassWord_Right;
	CString m_s_pwd;
};
