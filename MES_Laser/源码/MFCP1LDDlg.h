
// MFCP1LDDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "AutoCombox.h"
#include "afxcmn.h"
#include "MarkEzdDll.h"
#include "LDStartDlg.h"

//���ص������
UINT ThreadReadLdPort(LPVOID pParam); 

// CMFCP1LDDlg �Ի���
class CMFCP1LDDlg : public CDialogEx
{
private:
	CLDStartDlg *m_pCLDStartDlg;

// ����
public:
	CMFCP1LDDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMFCP1LDDlg();

// �Ի�������
	enum { IDD = IDD_MFCP1LD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CWinThread* LdReadPortThread;//�߳̾��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//���ƴ����пؼ����ʹ�ܺ�ʧȥʹ�ܵĺ���
	void InitInformationwindowtrue();
	void CRLDInformationwindowtrue();
	void LDSystemInformationwindowtrue();
	void InitInformationwindowfalse();
	void CRLDInformationwindowfalse();
	void LDSystemInformationwindowfalse();


	//��ȡ������Ϣ�����ͺ���
	CString m_pcnameEdit;
	CString m_pcipEdit;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);


	//��ʼ������ģ������Լ�����
	CAutoCombox m_zhidanautoCombo;
	CString ZhiDanNO;//�Ƶ���
	CEdit m_imeistartEdit;
	CEdit m_imeiendEdit;
	CEdit m_imeistartshowonlyEdit;
	CEdit m_imeiendshowonlyEdit;

	void InitComboBox();//�Ƶ��ų�ʼ��
	void GetImeiByZhidan();//�����Ƶ��Ż�ȡIMEI��ģ��·��
	void ZhiDanControl();//����ѡ�񶩵��ź�������Ϊ
	BOOL JudgeZhidan(CString zhidan);//�ж϶������Ƿ����

	afx_msg void OnBnClickedDbconfigButton();
	afx_msg void OnBnClickedSelectmodleButton();
	afx_msg void OnBnClickedOpenmodleButton();
	afx_msg void OnBnClickedUpdatezhidanButton();
	afx_msg void OnCbnKillfocusZhidanautoCombo();
	afx_msg void OnCbnSelchangeZhidanautoCombo();


	//�ص�ģʽѡ��ģ������Լ�����
	BOOL radioflag;//�ص�ģʽ�ı�־λ

	afx_msg void OnBnClickedImeiCheck();
	afx_msg void OnBnClickedNormolldRadio();
	afx_msg void OnBnClickedReldRadio();


	//��ǰ�ص���Ϣģ������Լ�����
	CString IMEIStaStart,IMEIStaEnd;//IMEI�Ŷη�Χ�ı���
	CString IMEIStaCRStart,IMEIStaCREnd;//�ص�ʱIMEI�ı���

	void GetDataNumber();//��ȡ������

	afx_msg void OnBnClickedOpenldsystemButton();
	afx_msg void OnBnClickedReadcurrentimeiButton();
	afx_msg void OnBnClickedReadimeiButton();
	afx_msg void OnEnKillfocusImeistartEdit();
	afx_msg void OnEnKillfocusImeiendEdit();


	//�ص�ϵͳģ������Լ�����
	MarkEzdDll markezd;	//�����ص������
	CString DllStr;//DLL�ķ���ֵת����CString
	int DllFlag;//DLL�ķ���ֵ
	int STEPLDFlag;//��̤����ת���ģʽ��־λ
	int ScanFlag;//ɨ��ǹģʽ��־λ
	int IMEI15Flag;//У��λ��־λ
	WORD LdPort;//�ص񴮿ڱ���
	CString LdVariableChar[8];//�ص���ֶη�����
	int LdVariableInt[8], LdVariableCount;//�ص���ֶζ�Ӧ��λ������

	CString CreateIMEI15(CString imei);//����IMEIУ��λ
	int FindLdName();//Ѱ���ص��������
	int ChangeLdName(CString LdVariable, CString strld);//�滻ָ���������Ƶ�ֵ
	int LdCore();//��ʼ�ص���
	void InsertData(CString strimei);//����\�����ص������²�������
	void ScanInsertData(CString strimei);//ɨ��ǹ�����ݿ�������
	void WindowTimeClose(CString str,int strtime);//������N������Զ��ر�

	afx_msg void OnBnClickedCloseldsystemButton();
	afx_msg void OnBnClickedLdoneceButton();
	afx_msg void OnBnClickedStepldButton();
	afx_msg void OnBnClickedScanmodeButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//��Ϣ��־ģ������Լ�����
	CRichEditCtrl m_currentRichedit;

	void SetRicheditText(CString strMsg, int No);//����ֵ������Ϣ��־����ʾ��ͬ����Ϣ
	CString GetTime();//��ȡ��ǰϵͳʱ��


	//���ݿ��ѯģ������Լ�����
	CListCtrl m_dbcurtainList;

	void ShowAllData();//��ʾ�Ѳ��������
	void ShowRepeatData();//��ʾ�ظ�������
	void ShowUnData();//��ʾδ�ص������

	afx_msg void OnBnClickedSelectalldataButton();
	afx_msg void OnBnClickedSelectrepetdataButton();
	afx_msg void OnBnClickedSelectmissingdataButton();
	afx_msg void OnBnClickedParityCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSetimeirangeButton();
};
