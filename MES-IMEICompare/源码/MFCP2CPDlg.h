
// MFCP2CPDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "AutoCombox.h"

// CMFCP2CPDlg �Ի���
class CMFCP2CPDlg : public CDialogEx
{
	// ����
public:
	CMFCP2CPDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_MFCP2CP_DIALOG };

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

	//�ؼ�����
public:
	CStatic m_hintStatic;
	CStatic m_imei2Static;
	CStatic m_imei1Static;
	CString m_imei1Edit;
	CString m_imei2Edit;
	CButton m_couplingCheck;
	CButton m_writenumCheck;
	CButton m_downloadCheck;
	CButton m_funtestCheck;


	//��������뺯������
public:
	CFont editfont;
	CFont staticfont1;
	CFont staticfont2;
	CFont checkfont;
	COLORREF clrcolor;
	void fontinit();//��ʼ������



	//�༭�����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsNumber(const CString& strTest);//�ж����ֺ���ĸ
	BOOL IsNumber2(const CString& strTest);//ֻ�ж�����
	afx_msg void OnEnSetfocusImei2Edit();

	//��������
	CString strpcip, strpcname;//�������ƺ�ip
	int notype;//����ĿǰҪɨ������
	CString notypename[8];//�������IMEI��VIP���ַ���
	CString strno1,strno2;//�������ɨ�����͵�ֵ������ѡ��SNʱ��������ŵľ���SN��
	CString strzhidan;//��ŵ�ǰѡ�е��Ƶ���
	int chjudgeflag;//�ʺи�ѡ��ı�־
	CString strimeistart, strimeiend;


	//���ݿ��������
	void InitComboBox();
	BOOL JudgeZhidan(CString zhidan);

    //��������
	CString GetTime();//��ȡ��ǰʱ��
	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);//��ȡIP��ַ
	afx_msg void OnBnClickedOk();
	void readimei();
	BOOL judgeimeirang(CString str, CString strimeistart, CString strimeiend);

	//�����ǿؼ���ť��Ϣ����

    //���ݿ����ð�ť
	afx_msg void OnBnClickedDbconfigButton();

	//���°�ť
	afx_msg void OnBnClickedUpdateordernumberButton();

	//��ѡ��ť
	afx_msg void OnBnClickedImeiRadio();
	afx_msg void OnBnClickedSnRadio();
	afx_msg void OnBnClickedSimRadio();
	afx_msg void OnBnClickedVipRadio();
	afx_msg void OnBnClickedIccidRadio();
	afx_msg void OnBnClickedBatRadio();
	afx_msg void OnBnClickedMacRadio();
	afx_msg void OnBnClickedEquipmentRadio();

	//�Զ����������Լ���ص���Ϣ����
	CAutoCombox m_zhidanCombo;
	afx_msg void OnCbnSelchangeZhidanCombo();
	afx_msg void OnCbnSelendokZhidanCombo();
	afx_msg void OnCbnKillfocusZhidanCombo();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_caihejudgeCheck;
	afx_msg void OnBnClickedChjudgeCheck();
};
