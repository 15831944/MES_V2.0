#pragma once


// CLDStartDlg �Ի���

class CLDStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLDStartDlg)

public:
	CLDStartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLDStartDlg();

// �Ի�������
	enum { IDD = IDD_LDSTART_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
