#pragma once
#include "afxwin.h"
#include "afxcmn.h"

UINT ReDownloadMainThread(LPVOID lpParam);//���������߳�

UINT ReDownloadWirtePortThread(LPVOID lpParam);//����д�߳�
UINT ReDownloadReadPortThread(LPVOID lpParam);//���ڶ��߳�

//extern CString LastReRID;
//extern CString LastReIMEI;
extern BOOL RePortAbnomal;
extern CString strFolderpath, strOKFolderpath, strFolderFile,m_resimdatafolderPath;//����ȫ��·��
extern HWND MainReFormHWND;//���߳̾��
extern HANDLE reporthandler;//���ھ��
extern CString reComNo;//���ں�
extern int simrestratflag;//��ʼ���ر�־λ
extern CString ReLogName;//������־�ļ���

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
	CMFCP3SIMPORTDlg *parentdlg = (CMFCP3SIMPORTDlg *)GetParent();//��ȡ������ָ��

	//��ʼ��ģ�麯���Լ�����
	CComboBox m_report1list1Combo;

	afx_msg void OnBnClickedReopensimdatafolderpathButton();
	afx_msg void OnCbnDropdownReportlist1Combo();
	afx_msg void OnBnClickedCancel();

	
	//����ģ�麯���Լ�����
	int simreconnectflag;
	int CommandNo;
	CWinThread* DWThread;//����д�߳̾��
	CWinThread* DRThread;//����д�߳̾��
	CWinThread* MainThread;//���������߳̾��

	CString CommandWriteUnit(int strcommandNo);//дָ��
	CString CommandReadUnit(int strcommandNo);//��ָ��
	void ReDownloadRestPortThread();//�����̱߳���
	void ReDownloadClosePortThread();//�ر��̱߳���
	void SetRePortEditEmpty();//�������

	afx_msg void OnBnClickedReport1connectButton();
	afx_msg void OnBnClickedRestart1Button();


	//������־�Լ���Ϣ��־��صĺ����Լ�����
	CRichEditCtrl m_recurrentinformationRichedit;

	void SetRicheditText(CString strMsg, int No);//��Ϣ��־
	void PrintReLog(CString strMsg);//������־
	CString GetTime();
	CString GetLogTime();


	//���庯���Լ�����
	CFont staticReHintfont;
	COLORREF clrcolor;

	void fontinit();//��ʼ������

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	
	//afx_msg LRESULT MainRePortThreadControl(WPARAM wParam, LPARAM lParam);
	//void ReDownloadWrite1PortThread(LPVOID lpParam);
	//void ReDownloadWrite2PortThread(LPVOID lpParam);
	//void ReDownloadRead1PortThread(LPVOID lpParam);
	//void ReDownloadRead2PortThread(LPVOID lpParam);
	//CTask ThreadControl;//�̳߳صĶ���
	//CThreadPool* m_lpThreadPool;//�̳߳صĶ���

	//void OpenThreadPoolTask(int Command);


};
