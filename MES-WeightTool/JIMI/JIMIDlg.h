
// JIMIDlg.h : ͷ�ļ�
//

#pragma once
#include "ADOManage.h"
#include "DBConfig.h"
#include "UserLogin.h"
#include "AutoCombox.h"
#include "NewButton.h"
#include <map>

#include "afxwin.h"

#include <sapi.h>//�������
#include <sphelper.h>//�������
#pragma comment(lib,"sapi.lib") //�������

using namespace std;


/*ȫ�ֱ���*/
extern BOOL Imei3Flag;

// CJIMIDlg �Ի���
class CJIMIDlg : public CDialogEx
{
	// ����
public:
	CJIMIDlg(CWnd* pParent = NULL);	// ��׼���캯��

									// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JIMI_DIALOG };
#endif

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

	//����Ҫʹ����Ķ���
public:
	ADOManage ado_SQL;

	//����ģ��
	HANDLE port1handler;
	HANDLE InitCom(CString comName);//��ʼ������
	CString sComPort;
	int CloseCom(HANDLE handler);//�رմ���
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);//��ȡ��ǰ���ں�
	void FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO);//��ע�����Ѱ�Ҵ��ں�
	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);//��ȡIP��ַ

	CWinThread * pComThread;	
	CWinThread * pWeightThread;//�����߳�
	CWinThread * pVoiceThread;//�����߳�
	static UINT ComRxThread(LPVOID pParam);//���ڽ��������߳�
	static UINT GetWeightThread(LPVOID pParam);//���������߳�
	static UINT VoiceThread(LPVOID pParam);//���������߳�
	BOOL Process();
	void GetWeightDEAL();//�����������
	bool bThreadEnd;//�ж��߳ǽ����ı���
	OVERLAPPED m_osRead; //���ڶ�ȡ����
	OVERLAPPED m_osWrite; //���ڷ��Ϳ���

	void FontInit();//��ʼ������
	CFont Font1;
	CFont Font2;
	CFont Font3; 
	CFont Font4;
	CFont Font5;

	//ϵͳ������;
	ISpVoice * pVoice;
	bool IniVoice();
	HRESULT hr;
	//������־
	void AddToRunList(CString str);
	void AddToErrorList(CString str);
	void PlayVoice(CString str);

	afx_msg void OnBnClickedBtnDbconfig();
	afx_msg void OnBnClickedOk();
	CListBox m_LIST_Run;
	CListBox m_LIST_Warning;

protected:
	afx_msg LRESULT OnRunlog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVoice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadweighportret(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnClearerror();
	CComboBox c_PortCom;
	afx_msg void OnDropdownComboCom();

	afx_msg void OnSelchangeZhidanCombo();

	//�ֶ�ѡ��ģ��
	CButton m_imeiSyllableCheck;
	CButton m_snSyllableCheck;
	CButton m_simSyllableCheck;
	CButton m_vipSyllableCheck;
	CButton m_iccidSyllableCheck;
	CButton m_batSyllableCheck;
	CButton m_macSyllableCheck;
	CButton m_equipmentSyllableCheck;
	CButton m_rfidSyllableCheck;
	CButton m_imei2SyllableCheck;

	//��ѡ��ģ��
	CButton m_simBindCheck;
	CButton m_vipBindCheck;
	CButton m_iccidBindCheck;
	CButton m_batBindCheck;
	CButton m_macBindCheck;
	CButton m_equipmentBindCheck;
	CButton m_rfidBindCheck;

public:
	bool readimei();////���ݶ����Ż�ȡIMEI��ʼ�źͽ�����
	CAutoCombox m_zhidanCombo;

	static map<int, CString>SyllableMap;//����û���ѡ�ֶ�
	static map<int, CString>BindMap;//����û���ѡ��

									//								//�༭�����
									//virtual BOOL PreTranslateMessage(MSG* pMsg);
									//BOOL IsNumber(const CString& strTest);//�ж����ֺ���ĸ
									//BOOL IsNumber2(const CString& strTest);//ֻ�ж�����
									//afx_msg void OnEnSetfocusImei2Edit();

									//��������
	CString strpcip, strpcname;//�������ƺ�ip
	int notype;//����ĿǰҪɨ������
	CString notypename[9];//�������IMEI��VIP���ַ��� ע�⣬notypenameĿǰ��Ϊ������Ķ���������������Ϊ�����ں����߼��Ķ�������������ģ�Ŀǰ����Գ�����Ӱ�죬������ʱ������
	CString strno1, strno2, strno3;//�������ɨ�����͵�ֵ������ѡ��SNʱ��������ŵľ���SN��
	CString strzhidan;//��ŵ�ǰѡ�е��Ƶ���

	CString strimeistart, strimeiend;//IMEI��ʼ�ͽ���

	afx_msg void OnBnClickedButtonUpdataorder();
	void InitComboBox();//��combox������ݿ��еĶ����ţ����°�ť�Ϳ������Զ���ʼ�������������
	void SetEditEmpty();//���༭����Ϊ��
	void RelationEnableWindow(BOOL chose);//ʹ�ֶκͰ󶨿ؼ�ʹ��
	void ChImei3EnableWindow(BOOL chose);//ʹ���жϲʺкͿ���IMEI�༭��ؼ�ʹ��
	void ImeiInputEnableWindow(BOOL chose);//ʹIMEI�����ؼ�ʹ��
	void CleanSyllableCheck();//����ֶ�ѡ��ѡ��
	void CleanBindCheck();//����󶨸�ѡ��
	void CleanImei3Check();//���IMEI��ѡ��

						   //���ݿ����ģ��
	CString SyllablestrSql;

	BOOL JudgeZhidan(CString zhidan);//�ж��Ƶ���
	void Savesyllable();//�����ֶ�ѡ��
	void Readsyllable(BOOL CheckEx);//��ȡ�ֶ�ѡ��
	void Savebind();//�����ѡ��
	void Readbind(BOOL CheckEx);//��ȡ��ѡ��
	void Saveconfig();//����һЩ����
	void Readconfig();//��ȡһЩ����

	BOOL LockFlag;//������־λ
	void OtherEnableWindow(BOOL chose);//ʹ�������á����жϲʺС����ݿ����ÿؼ�ʹ��

	afx_msg void OnBnClickedButtonComok2();
	afx_msg void OnKillfocusZhidanCombo();
	afx_msg void OnSelendokZhidanCombo();
	afx_msg void OnBnClickedSimsyllableCheck();
	afx_msg void OnBnClickedImeisyllableCheck();
	afx_msg void OnBnClickedSnsyllableCheck();
	afx_msg void OnBnClickedVipsyllableCheck();
	afx_msg void OnBnClickedIccidsyllableCheck();
	afx_msg void OnBnClickedBatsyllableCheck();
	afx_msg void OnBnClickedMacsyllableCheck();
	afx_msg void OnBnClickedEquipmentsyllableCheck();
	afx_msg void OnBnClickedRfidsyllableCheck();
	afx_msg void OnBnClickedSavesyllableButton();
	afx_msg void OnBnClickedReadsyllableButton();

	afx_msg void OnBnClickedSimbindCheck();
	afx_msg void OnBnClickedVipbindCheck();
	afx_msg void OnBnClickedBatbindCheck();
	afx_msg void OnBnClickedIccidbindCheck();
	afx_msg void OnBnClickedMacbindCheck();
	afx_msg void OnBnClickedEquipmentbindCheck();
	afx_msg void OnBnClickedRfidbindCheck();
	afx_msg void OnBnClickedSavebindButton();
	afx_msg void OnBnClickedReadbindButton();
	afx_msg void OnBnClickedOpenimei3editCheck();
	CButton m_OpenImei3EditCheck;
	//CNewButton m_StateView;
	//CNewButton m_OKView;
	//CNewButton m_OverView;
	//CNewButton m_NullView;
	//CNewButton m_DownView;
	//////////////////Ҫ��������ݵ�����////////////////
	CString m_disDownLimit;
	CString m_disUpLimit;
	CString m_DownLimit;
	CString m_UpLimit;
	CString m_PCIP;
	CString m_PCName;
	CString m_UserName;
	CString m_UserRight;//�û�Ȩ��

	CString m_WeightValue;//���ճ��صĽ��ֵ
	CString m_IMEIValue;//���ճ��ص�IMEI���ֵ
	CString m_OKValue;//���ճ��ص��жϽ��ֵ
	CString sResult;//��ת����
	int iValResult;//0������1��ƫ�᣻2��ƫ�أ�3������

				   //////////////////Ҫ��������ݵ�����////////////////

	CString m_oldWeightValue;//�������������
	CString m_newWeightValue;//�������������

	afx_msg void OnBnClickedLockButton();
	CString m_imei3Edit;
	CString m_imei2Edit;
	CString m_imei1Edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsNumber(const CString& strTest);//�ж����ֺ���ĸ
	BOOL IsNumber2(const CString& strTest);//ֻ�ж�����
	BOOL judgeimeirang(CString str, CString strimeistart, CString strimeiend);
	void RecordResult(CString sRemark);//��¼��������ݿ�
	void ToResultSheet();
	bool GetWeightValue();//���㴦����Ч������ֵ����ֵ��Ӧ��ֵ��������ݿ�
	void DealtSerialData(CString sSerialData);//��ȡ��ǰ�Ĵ��ڷ��ص���Ч������ֵ
	bool CheckParam();//������õĲ���
	void WidgetStatue(BOOL Show);//����Ȩ���������ʾ״̬

	afx_msg void OnBnClickedImei2syllableCheck();
	afx_msg void OnBnClickedButtonLogin();
};

