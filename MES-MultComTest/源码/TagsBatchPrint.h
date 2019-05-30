#pragma once

//���������
#include "IMEIWrite_MulAT.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ComboListCtrl.h"

#include "winspool.h"

#define PRINTONCE_MAX  20000
// TagsBatchPrint �Ի���
class TagsBatchPrint : public CDialog
{
	DECLARE_DYNAMIC(TagsBatchPrint)

public:
	TagsBatchPrint(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TagsBatchPrint();

// �Ի�������
	enum { IDD = IDD_BATCH_PRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()


public:
	void AutoCreateTagsFunc();    //�Զ�����IMEI��ַ
	void AutoCreateTags(LPTSTR lpszTags);
	void AutoCreateTags14(LPTSTR lpszTags);
	__int64 ComputeCD(__int64 nImei);

	BOOL CheckIMEI(CString	m_IMEI);
public:
	// ��ֽ��ʼ��
	CString TagsStartNum;
	__int64 m_int64StartTags;
	__int64 m_int64DefaultTags;

	// ��ֽ����
	CString Tags_Count;
	int Tags_Count_int;

	// ��ǰ��
	CString TagsCurrNum;
	CString Tags_ALL;

	// ������
	CString TagsEndNum;
	__int64 m_int64EndTags;

public:
	CArray<wininfo,wininfo&> m_wininfoarray;
	wininfo m_currentwin;

	CString Tags_Input[PRINTONCE_MAX];											//һ�δ�ӡ������
	byte buf[32];																//��ǰ����
	CString Tags_LOG_CS;


	CWinThread* Print_Handle;
	
	JOB_INFO_2 *ppJobInfo;														// Pointer to be filled. 
	int pcJobs;																	// Count of jobs filled. 
    DWORD pStatus;																// Print Queue status. 

	HANDLE Current_handle;
	CString PrinterNameCS;

public:
	void Get_App_Running();//��ȡ�������е�Ӧ�� 
	void OnWndFocus(BOOL ShowWin,BOOL Print,CString IMEI_Print[PRINTONCE_MAX]);
	void Input_Data(CString IMEI_Print[PRINTONCE_MAX]);
	void Pause_Print();
	CString LoadConfig_Change();

	void LogShow_exchange(CString Msg_Log);

	void GetPrinters();
	BOOL GetJobs(HANDLE hPrinter,			// Handle to the printer.
                JOB_INFO_2 **ppJobInfo,		// Pointer to be filled. 
                int *pcJobs,				// Count of jobs filled. 
                DWORD *pStatus);			// Print Queue status.   

	BOOL TimerFunction();//��ʱ����
	BOOL PrintSpace(CString Count);//��ӡ�հ���ֽ
public:
	// ��ӡ�հ�ֽ���
	CString PrintBlankInterval;
	CEdit TagsCurrNumControl;
	CEdit Tags_CountControl;
	afx_msg void OnBnClickedButtonstart17();

public:
	//�Ż����ݴ洢��ָ�����
	CString *strContents;
	afx_msg void OnBnClickedButtonstart19();
	//CListCtrl Excell_show;
	CComboListCtrl Excell_show;
	CString		m_strExchangeFileName;//�����ļ�����·��
	long count_Tags;
	afx_msg void OnBnClickedButton2();
	CEdit Tags_ALL_Control;
	// ��ֽ���ݵ�λ��
	int TagsBits;
	// ��ӡ�ٶ�
	int PrintSpeed;
	afx_msg void OnBnClickedButtonstart15();
	//ֹͣ��־
	BOOL StopSign;
	BOOL Sequence;
	afx_msg void OnBnClickedButton21();
	CComboBox PrinterName;
	CEdit PrintStateShow;
	// ѡ���Ƿ�����IMEIУ��λ
	BOOL IMEI_CAL;
	afx_msg void OnBnClickedButton22();
};
