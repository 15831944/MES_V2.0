#pragma once


// CManager �Ի���

class CManager : public CDialogEx
{
	DECLARE_DYNAMIC(CManager)

public:
	CManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CManager();

	// �Ի�������
	enum { IDD = IDD_Manager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
