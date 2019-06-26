
// MFCP6ComToolDlg.h : ͷ�ļ�
//

#pragma once
#include "RDAHostInterface.h"
#include "Log.h"
#include "afxwin.h"
#include "afxbutton.h"
#include "HistogramDlg.h"
#include "afxcmn.h"

// CMFCP6ComToolDlg �Ի���
class CMFCP6ComToolDlg : public CDialogEx
{
// ����
public:
	CMFCP6ComToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCP6COMTOOL_DIALOG };

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
	CButton m_MtkPortRadioControl;
	CButton m_SendOrderControl;
	CButton m_ListLoopSendControl;
	CButton m_RdaPortRadioControl;
	BOOL m_HexDisplayCheckValue;
	BOOL m_HexSendCheckValue;
	BOOL m_IsAddTimeCheckValue;
	int m_AutoSendTimeValue;
	BOOL m_AutoSendCheckValue;
	CButton m_AutoSendCheckControl;
	//int m_ListControlDelayTimeiValue;
	CEdit m_ListControlDelayTimeControl;
	CComboBox m_PortNoComboControl;
	CComboBox m_PortBaudComboControl;
	int m_PortRadioValue;
	BOOL m_WithEnterCheckValue;
	BOOL m_OutputLocalLogCheckValue;
	CFont fn;
	RDAHostInterface  RdaHostInterface;

	BOOL ListloopsendFlag;//˳���ͱ�־
	BOOL PortIsOpenFlag;//���ڿ��ذ�ť��־
	CString PortNo;//�������Ʊ���
	CString PortBaud;//���ڲ�����
	HANDLE Porthandler;//���ھ��

	BOOL InitPort();//��ʼ�����ڼ��ɺ���
	HANDLE InitCom(CString comName);//��ʼ������
	BOOL CloseCom(HANDLE handler);//�رմ���
	BOOL GetCommPort(CComboBox *pComboBox, CComboBox *pBaudBox);//��ȡ��ǰ���ںźͲ�����
	void FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO);//��ע�����Ѱ�Ҵ��ں�
	void SendAndReceiveComOrder(HANDLE PortHandler, CString SendOrder, CString &ReceiveData, CString &IsSuccessFlag);//����ָ���շ�
	void SendComOrder(HANDLE PortHandler, CString SendOrder);//���ڷ�ָ��
	void AutoSendComOrder();//��ʱ����ָ��
	void ReceiveComOrder(HANDLE PortHandler);//������ָ��
	void EnbleWindowIsOpenPort(BOOL Chooes);//�Ӵ��ڿ�����񣬽����뿪����ؿؼ�
	void ListLoopSendComOrder();

	void SendComOrderHandleFun(CString PortOrder);//���ڷ���ָ����ۺϴ�����
	void InitToolSetting();//��ʼ�����
	void IniLoadConig();//��ȡINI�ļ��Զ��������Ϣ
	void IniSaveConig();//INI�ļ��Զ�����
	BYTE *CStrToByte(int len, CString str);



	//��Ϣ��־ģ�麯���Լ�����
	CRichEditCtrl m_InformationRicheditControl;
	CRichEditCtrl m_PortOrderRicheditControl;
	void PrintLog(CString strMsg, int No);//������־
	void SetRicheditText(CString strMsg, int No);//��ʱ��־
	CString GetTime();
	CString GetLogTime();

	CString LogName;

	/*���ù���ģ������ͺ���*/
	CListCtrl m_ListSettingControl;
	CString ListHeaderName[8];
	CEdit m_ListControlEdit;
	CComboBox m_ListControlCombo;
	int m_Row, m_Colum;
	CString SettingSrc;

	void InitListSetting(CListCtrl *m_ListControl);//��ʼ�������б�
	void ListSettingInsert(CListCtrl *m_ListControl);//�����б��������
	void ListSettingUp(CListCtrl *m_ListControl);//�����б�ѡ���������ƶ�
	void ListSettingDown(CListCtrl *m_ListControl);//�����б�ѡ���������ƶ�
	void ListSettingCopy(CListCtrl *m_ListControl);//�����б���ѡ����
	void ListSettingDelete(CListCtrl *m_ListControl);//�����б�ɾ��ѡ����
	void ListSettingSave(CListCtrl *m_ListControl);//�����б���
	void ListSettingLoad(CListCtrl *m_ListControl);//�����б��ȡ
	BOOL ListSettingLoadFun(CString SettingSrc);//�����б��ȡ����

	afx_msg void OnBnClickedSettinginsertButton();
	afx_msg void OnBnClickedSettingupButton();
	afx_msg void OnBnClickedSettingdownButton();
	afx_msg void OnBnClickedSettingcopyButton();
	afx_msg void OnBnClickedSettingdeleteButton();
	afx_msg void OnBnClickedSettingsaveButton();
	afx_msg void OnNMDblclkBlesettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusListcontrolEdit();
	afx_msg void OnCbnKillfocusListcontrolCombo();

	/*GPSģ������ͺ���*/
	int m_CurSelTab;
	BOOL m_GPSIsOpenFlag;
	CListCtrl m_GPSDisplayListControl;
	CTabCtrl m_PageControlTabControl;
	CHistogramDlg m_HistogramPage;
	CStatic m_GPSHintControl;

	CString GetData(char* Serial_Order_Return, CString Start, CString End, int Count);
	void InitGPSList(CListCtrl *m_ListControl);//��ʼ��GPS�б�
	void InsertGPSList(CListCtrl *m_ListControl, CString GPSIDList[20], CString GPSSNRList[20], int GPSCount);//�������ݵ�GPS�б��У�ͬʱ��������ͼ������

	void InitTabControl();//��ʼ��Tabģ��
	afx_msg void OnTcnSelchangePagecontrolTab(NMHDR *pNMHDR, LRESULT *pResult);//�л�ѡ�

	/*��������*/
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClearinformationButton();
	afx_msg void OnBnClickedOpenportButton();
	afx_msg void OnBnClickedClearsendorderButton();
	afx_msg void OnBnClickedSendorderButton();
	afx_msg void OnCbnDropdownPortnoCombo();
	afx_msg void OnBnClickedMtkportRadio();
	afx_msg void OnBnClickedWithenterCheck();
	afx_msg void OnBnClickedOutputlocallogCheck();
	afx_msg void OnBnClickedSettingreadButton();
	afx_msg void OnNMClickBlesettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedListloopsendButton();
	afx_msg void OnBnClickedAutosendCheck();
	afx_msg void OnBnClickedHexdisplayCheck();
	afx_msg void OnBnClickedHexsendCheck();
	afx_msg void OnBnClickedIsaddtimeCheck();
	afx_msg void OnEnKillfocusListcontroldelaytimeEdit();
	afx_msg void OnNMCustomdrawBlesettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusAutosendtimeEdit();
	afx_msg void OnBnClickedIstaidouCheck();
	afx_msg void OnBnClickedSendgpsorderButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	BOOL m_IsTaiDouCheckValue;
	afx_msg void OnBnClickedCleargpslistButton();
	CButton m_SendGpsOrderControl;
};
