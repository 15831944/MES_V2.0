
// MFCP3SIMPORTDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "ThreadPool.h"
#include "Task.h"

//����ָ�ť��д�̺߳���
UINT SendWritePortThread(LPVOID pParam);
UINT SendReadPortThread(LPVOID pParam);
UINT DownloadPortThread(LPVOID pParam);
extern HWND MainFormHWND;
extern CString strFolderpath, strOKFolderpath;
extern CString StrFolder[4];
extern CString strSingleFilePath;
extern int simstart1flag;
extern int simstart3flag;
extern int simstart2flag;
extern int simstart4flag;
extern CString SinglePortLogName;
extern CString Port1LogName;
extern CString Port2LogName;
extern CString Port3LogName;
extern CString Port4LogName;
extern CString LastPort1RID;
extern CString LastPort1IMEI;
extern CString LastPort2RID;
extern CString LastPort2IMEI;
extern CString LastPort3RID;
extern CString LastPort3IMEI;
extern CString LastPort4RID;
extern CString LastPort4IMEI;


// CMFCP3SIMPORTDlg �Ի���
class CMFCP3SIMPORTDlg : public CDialogEx
{
	// ����
public:
	CMFCP3SIMPORTDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_MFCP3SIMPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();


public:

	//��������뺯������
	CFont staticHint1font, staticHint2font, staticHint3font, staticHint4font;
	COLORREF clrcolor;
	void fontinit();//��ʼ������

	afx_msg LRESULT MainFontControl(WPARAM wParam, LPARAM lParam);//������ĵ���Ϣѭ��

	//��ʼ������ģ�麯���Լ�����
	int initconfigflag;
	CString m_simdatafolderPath, m_simdatafilePath;
	HANDLE port1handler, port2handler, port3handler, port4handler;

	afx_msg void OnBnClickedMultipledownloadRadio();
	afx_msg void OnBnClickedSingledownloadRadio();
	afx_msg void OnBnClickedOpensimdatafolderpathButton();
	afx_msg void OnBnClickedOpensimdatafilepathButton();
	void SetInitConfigWindow();

	//SIM����������ģ�麯���Լ�����
	int simconnect1flag,  simconnect2flag,  simconnect3flag,  simconnect4flag,  simallconnectflag, simallstartflag;
	CString strsinglecrc16, strport1crc16, strport2crc16, strport3crc16, strport4crc16;//��У�����õ�
	CString Com1No, Com2No, Com3No, Com4No;//�������Ʊ���
	int CommandNo;//ָ��͵ı�־

	//����������
	CComboBox m_portlist1Combo;
	CComboBox m_portlist2Combo;
	CComboBox m_portlist3Combo;
	CComboBox m_portlist4Combo;

	HANDLE InitCom(CString comName);//��ʼ������
	int CloseCom(HANDLE handler);//�رմ���
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);//��ȡ��ǰ���ں�
	void FindCommPort(CComboBox *pComboBox, CString &ComNo,int PortNO);//��ע�����Ѱ�Ҵ��ں�

	CTask ThreadControl;//�̳߳صĶ���
	CThreadPool* m_lpThreadPool;//�̳߳صĶ���
	
	afx_msg LRESULT MainPortThreadControl(WPARAM wParam, LPARAM lParam);//�̳߳ص���Ϣѭ��

	void OpenThreadPoolTask(int Command);//�̳߳صĿ����̺߳���
	void DownloadMainContralThread(LPVOID lpParam);//�̳߳ص������߳�

	CString CommandWriteUnit(int strcommandNo);//�ļ����������,0��TESTָ��,1��RIDָ��,2��IMEIָ��,3�ǿ�ʼ����ָ��,4�����ؽ���ָ��
	CString CommandReadUnit(int strcommandNo);//ͬ��
	
	//�����ڿؼ�ʹ�ܿ��ƺ���
	void Port1SetOtherWindowTrue();
	void Port1SetOtherWindowFalse();
	void PortSetOtherWindowTrue();
	void PortSetOtherWindowFalse();

	//��մ��ڵ�RID��IMEI�ȿ���
	void SetPort1EditEmpty();
	void SetPort2EditEmpty();
	void SetPort3EditEmpty();
	void SetPort4EditEmpty();


	//���������ӡ���ʼ�ʹ���������ť����¼�
	afx_msg void OnCbnDropdownPortlist1Combo();
	afx_msg void OnBnClickedPort1connectButton();
	afx_msg void OnBnClickedStartdownload1Button();
	afx_msg void OnBnClickedPort2connectButton();
	afx_msg void OnBnClickedStartdownload2Button();
	afx_msg void OnBnClickedPort3connectButton();
	afx_msg void OnBnClickedStartdownload3Button();
	afx_msg void OnBnClickedPort4connectButton();
	afx_msg void OnBnClickedStartdownload4Button();
	afx_msg void OnBnClickedAutomultipleconnectButton();
	afx_msg void OnBnClickedAutomultiplestartButton();
	afx_msg void OnCbnDropdownPortlist2Combo();
	afx_msg void OnCbnDropdownPortlist3Combo();
	afx_msg void OnCbnDropdownPortlist4Combo();


	//�����Ǹ������ڵ��߳�
	//���ļ�����
	void SingleDownloadWrite1Port1Thread(LPVOID lpParam);
	void SingleDownloadWrite2Port1Thread(LPVOID lpParam);
	void SingleDownloadWrite3Port1Thread(LPVOID lpParam);
	void SingleDownloadWrite4Port1Thread(LPVOID lpParam);
	void SingleDownloadRead1Port1Thread(LPVOID lpParam);
	void SingleDownloadRead2Port1Thread(LPVOID lpParam);
	void SingleDownloadRead3Port1Thread(LPVOID lpParam);
	void SingleDownloadRead4Port1Thread(LPVOID lpParam);
	void SingleDownloadRestPort1Thread();
	void SingleDownloadClosePort1Thread();

	//����1
	void DownloadWrite1Port1Thread(LPVOID lpParam);
	void DownloadWrite2Port1Thread(LPVOID lpParam);
	void DownloadWrite3Port1Thread(LPVOID lpParam);
	void DownloadWrite4Port1Thread(LPVOID lpParam);
	void DownloadRead1Port1Thread(LPVOID lpParam);
	void DownloadRead2Port1Thread(LPVOID lpParam);
	void DownloadRead3Port1Thread(LPVOID lpParam);
	void DownloadRead4Port1Thread(LPVOID lpParam);
	void DownloadRestPort1Thread();
	void DownloadClosePort1Thread();

	//����2
	void DownloadWrite1Port2Thread(LPVOID lpParam);
	void DownloadWrite2Port2Thread(LPVOID lpParam);
	void DownloadWrite3Port2Thread(LPVOID lpParam);
	void DownloadWrite4Port2Thread(LPVOID lpParam);
	void DownloadRead1Port2Thread(LPVOID lpParam);
	void DownloadRead2Port2Thread(LPVOID lpParam);
	void DownloadRead3Port2Thread(LPVOID lpParam);
	void DownloadRead4Port2Thread(LPVOID lpParam);
	void DownloadRestPort2Thread();
	void DownloadClosePort2Thread();

	//����3
	void DownloadWrite1Port3Thread(LPVOID lpParam);
	void DownloadWrite2Port3Thread(LPVOID lpParam);
	void DownloadWrite3Port3Thread(LPVOID lpParam);
	void DownloadWrite4Port3Thread(LPVOID lpParam);
	void DownloadRead1Port3Thread(LPVOID lpParam);
	void DownloadRead2Port3Thread(LPVOID lpParam);
	void DownloadRead3Port3Thread(LPVOID lpParam);
	void DownloadRead4Port3Thread(LPVOID lpParam);
	void DownloadRestPort3Thread();
	void DownloadClosePort3Thread();

	//����4
	void DownloadWrite1Port4Thread(LPVOID lpParam);
	void DownloadWrite2Port4Thread(LPVOID lpParam);
	void DownloadWrite3Port4Thread(LPVOID lpParam);
	void DownloadWrite4Port4Thread(LPVOID lpParam);
	void DownloadRead1Port4Thread(LPVOID lpParam);
	void DownloadRead2Port4Thread(LPVOID lpParam);
	void DownloadRead3Port4Thread(LPVOID lpParam);
	void DownloadRead4Port4Thread(LPVOID lpParam);
	void DownloadRestPort4Thread();
	void DownloadClosePort4Thread();


	//��Ϣ��־ģ�麯���Լ�����
	CRichEditCtrl m_currentinformationRichedit;
	void PrintLog(CString strMsg,int No);

	void SetRicheditText(CString strMsg, int No);
	CString GetTime();
	CString GetLogTime();

	//���ݿ����Ϻ����Լ�����
	afx_msg void OnBnClickedDbconfigButton();//���ݿ����ð�ť�¼�

	afx_msg LRESULT MainDataInsertControl(WPARAM wParam, LPARAM lParam);//���ݿ���Ϣѭ������

	int SimDataSingleNoIsExitFun(CString strfile);
	int SimDataNoIsExitFun(CString strfile);

	//����1��
	int SimDataLastStationFun1();
	int SimDataIsExitFun1();
	void SimDataOkInsertFun1();
	void SimDataErrorInsertFun1();

	//����2��
	int SimDataLastStationFun2();
	int SimDataIsExitFun2();
	void SimDataOkInsertFun2();
	void SimDataErrorInsertFun2();

	//����3��
	int SimDataLastStationFun3();
	int SimDataIsExitFun3();
	void SimDataOkInsertFun3();
	void SimDataErrorInsertFun3();

	//����4��
	int SimDataLastStationFun4();
	int SimDataIsExitFun4();
	void SimDataOkInsertFun4();
	void SimDataErrorInsertFun4();

	//��������
	void Delay(int time);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	//��ȡ������Ϣ�����ͺ���
	CString m_pcnameEdit;
	CString m_pcipEdit;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);
	afx_msg void OnBnClickedOpenremodleButton();
};

