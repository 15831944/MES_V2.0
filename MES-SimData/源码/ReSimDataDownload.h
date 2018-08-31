#pragma once
#include "afxwin.h"
#include "afxcmn.h"

extern CString strFolderpath, strOKFolderpath, strFolderFile,m_resimdatafolderPath;
extern HWND MainReFormHWND;
extern HANDLE reporthandler;
extern CString reComNo;
extern int simrestratflag;

// CReSimDataDownload �Ի���

class CReSimDataDownload : public CDialogEx
{
	DECLARE_DYNAMIC(CReSimDataDownload)

public:
	CReSimDataDownload(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReSimDataDownload();

// �Ի�������
	enum { IDD = IDD_RESIMDATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedReopensimdatafolderpathButton();
	afx_msg void OnBnClickedReport1connectButton();
	afx_msg void OnBnClickedRestart1Button();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnDropdownReportlist1Combo();
	
	CComboBox m_report1list1Combo;
	CMFCP3SIMPORTDlg *parentdlg = (CMFCP3SIMPORTDlg *)GetParent();

	int simreconnectflag;

	void SetRePortEditEmpty();

	CFont staticReHintfont;
	COLORREF clrcolor;
	void fontinit();//��ʼ������
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//����λ�߳�
	afx_msg LRESULT MainRePortThreadControl(WPARAM wParam, LPARAM lParam);

	void ReDownloadMainThread(LPVOID lpParam);
	void ReDownloadWrite1PortThread(LPVOID lpParam);
	void ReDownloadWrite2PortThread(LPVOID lpParam);
	void ReDownloadRead1PortThread(LPVOID lpParam);
	void ReDownloadRead2PortThread(LPVOID lpParam);

	void ReDownloadRestPortThread();
	void ReDownloadClosePortThread();

	CTask ThreadControl;//�̳߳صĶ���
	CThreadPool* m_lpThreadPool;//�̳߳صĶ���

	void OpenThreadPoolTask(int Command);

	//���ݿ⺯��
	int SimDataReFun();

	afx_msg LRESULT MainReDataInsertControl(WPARAM wParam, LPARAM lParam);//���ݿ���Ϣѭ������

	void SetRicheditText(CString strMsg, int No);
	CString GetTime();

	CRichEditCtrl m_recurrentinformationRichedit;
};
