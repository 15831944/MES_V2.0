
// MFCP4ECurrentToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "AutoCombox.h"
#include <map>

using namespace std;

//�̺߳���
UINT MainControlThread(LPVOID lpParam);//���������߳�

UINT SendPort1Thread(LPVOID lpParam);//����д�߳�
UINT ReadPort1Thread(LPVOID lpParam);//���ڶ��߳�

//��̬��Ҫ�õ��ĺ���
double Current();
bool PowerOn();

//ȫ�ֱ���
extern CString Currentper[5];//�����ε���
extern float StandbyAverage;//��������ƽ��ֵ
extern float SleepAverage1;//˯�ߵ���ƽ��ֵ
extern float SleepAverage2;//˯�ߵ���ƽ��ֵ
extern int CommandCount;//ָ�������
extern int CommandSendInterval;//ָ��ͼ��
extern int CommandReadInterval;//ָ���ȡ���
extern int StandbyInterval;//�����ȴ�ʱ��
extern int SleepInterval;//˯�ߵȴ�ʱ��
extern int StandbyInterval;//�����ȴ�ʱ��
extern int SleepInterval;//˯�ߵȴ�ʱ��
extern CString Port1LogName;//��־�ļ���



// CMFCP4ECurrentToolDlg �Ի���
class CMFCP4ECurrentToolDlg : public CDialogEx
{
// ����
public:
	CMFCP4ECurrentToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCP4ECURRENTTOOL_DIALOG };

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
	//��ȡ������Ϣ�����ͺ���
	CString m_pcnameEdit;
	CString m_pcipEdit;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);

	/*�ؼ�ʹ�ܱ����Լ�����*/
	BOOL ConnectFlag;//���͡����ں����ӱ�־λ
	
	void InitEnableWindows(BOOL choose);//��ʼģ��ؼ�ʹ�ܺ���

	/*��ʼ��ģ������Լ�����*/
	static map<int, CString>CommandMap;//�����Ҫ���͵�ָ��
	static map<int, CString>CommandReplyMap;//���ָ��Ļظ�
	CString Port1No;//�������Ʊ���
	HANDLE Port1handler;//���ھ��
	CFont staticHint1font,editfont;//�������
	COLORREF clrcolor;//��ɫ����
	CBrush m_brush;//ˢ�ӱ���

	//������ؼ�����
	CComboBox m_Port1Combo;
	CAutoCombox m_ModelCombo;

	//�༭��ؼ�����
	CEdit m_StandbyUpControl;
	float m_StandbyUpValue;
	CEdit m_StandbyCuControl;
	CString m_StandbyCuValue;
	CEdit m_StandbyDownControl;
	float m_StandbyDownValue;
	CEdit m_Port1RidControl;
	CString m_Port1RidValue;
	CEdit m_SleepUpControl;
	float m_SleepUpValue;
	CEdit m_SleepCuControl1;
	CString m_SleepCuValue1;
	CEdit m_SleepDownControl;
	float m_SleepDownValue;

	void fontinit();//��ʼ������
	afx_msg LRESULT MainFontControl(WPARAM wParam, LPARAM lParam);//������ĵ���Ϣѭ��
	void InitModelDBOperation();//��ʼ������������
	void FindModelDBOperation();//���һ�������
	HANDLE InitCom(CString comName);//��ʼ������
	int CloseCom(HANDLE handler);//�رմ���
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);//��ȡ��ǰ���ں�
	void FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO);//��ע�����Ѱ�Ҵ��ں�
	BOOL JudgeEu();//�жϵ���

	afx_msg void OnCbnSelchangeModelCombo();
	afx_msg void OnBnClickedModelconfigButton();
	afx_msg void OnBnClickedPort1connectButton();
	afx_msg void OnCbnKillfocusModelCombo();
	afx_msg void OnCbnSelendokModelCombo();
	afx_msg void OnCbnDropdownPort1Combo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	/*ָ�����̵߳ı����뺯��*/
	int CommandNo;//��ǰָ��
	CWinThread* DWThread;//����д�߳̾��
	CWinThread* DRThread;//����д�߳̾��
	CWinThread* MainThread;//���������߳̾��

	void RestPort1Thread();//�����̱߳���
	void SetPort1EditEmpty();//�������


	/*���ݿ�ģ������Լ�����*/

	//�������
	BOOL DBInsertOperation(CString ECIP, CString Rid, CString StandbyCurrent, CString StandbyAverage, CString SleepCurrent, CString SleepAverage1, CString SleepAverage2, CString TestResult);
	//�ж�RID�Ƿ��ظ�
	int DBJudgeOperation(CString Rid);
	
	afx_msg void OnBnClickedDbconfigButton();


	/*��־ģ������Լ�����*/
	CRichEditCtrl m_CurrentLog;

	void SetRicheditText(CString strMsg, int No); //��ǰ��־����
	void PrintLog(CString strMsg, int No);//��־��ӡ����
	CString GetTime();	//��ȡϵͳʱ��
	CString GetLogTime();	//��ȡ��־ʱ��


	/*���������Լ�����*/
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_SleepCuValue2;
	CEdit m_SleepCuControl2;
};
