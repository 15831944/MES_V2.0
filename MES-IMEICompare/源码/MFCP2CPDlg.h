
// MFCP2CPDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "AutoCombox.h"
#include <map>
#include "Resource.h"

using namespace std;


/*ȫ�ֱ���*/
extern BOOL Imei3Flag;

//extern int SyllableArray[8];
//extern int BindArray[6];

//extern map<int, CString>SyllableMap;
//extern map<int, CString>BindMap;

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
	CString m_imei3Edit;
	CButton m_OpenImei3EditCheck;
	CButton m_couplingCheck;
	CButton m_writenumCheck;
	CButton m_downloadCheck;
	CButton m_funtestCheck;
	BOOL LockFlag;//������־λ

	void RelationEnableWindow(BOOL chose);//ʹ�ֶκͰ󶨿ؼ�ʹ��
	void ChImei3EnableWindow(BOOL chose);//ʹ���жϲʺкͿ���IMEI�༭��ؼ�ʹ��
	void OtherEnableWindow(BOOL chose);//ʹ�������á����жϲʺС����ݿ����ÿؼ�ʹ��
	void ImeiInputEnableWindow(BOOL chose);//ʹIMEI�����ؼ�ʹ��
	void SetEditEmpty();//���༭����Ϊ��
	void CleanImei3Check();//���IMEI��ѡ��

	//��������뺯������
public:
	CFont editfont;
	CFont staticfont1;
	CFont staticfont2;
	CFont checkfont;
	COLORREF clrcolor;
	void fontinit();//��ʼ������

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

	afx_msg void OnBnClickedSavesyllableButton();
	afx_msg void OnBnClickedReadsyllableButton();
	afx_msg void OnBnClickedImeisyllableCheck();
	afx_msg void OnBnClickedSnsyllableCheck();
	afx_msg void OnBnClickedVipsyllableCheck();
	afx_msg void OnBnClickedSimsyllableCheck();
	afx_msg void OnBnClickedBatsyllableCheck();
	afx_msg void OnBnClickedIccidsyllableCheck();
	afx_msg void OnBnClickedEquipmentsyllableCheck();
	afx_msg void OnBnClickedMacsyllableCheck();
	afx_msg void OnBnClickedRfidsyllableCheck();

	void CleanSyllableCheck();//����ֶ�ѡ��ѡ��

	//��ѡ��ģ��
	CButton m_simBindCheck;
	CButton m_vipBindCheck;
	CButton m_iccidBindCheck;
	CButton m_batBindCheck;
	CButton m_macBindCheck;
	CButton m_equipmentBindCheck;
	CButton m_rfidBindCheck;

	afx_msg void OnBnClickedSavebindButton();
	afx_msg void OnBnClickedReadbindButton();
	afx_msg void OnBnClickedSimbindCheck();
	afx_msg void OnBnClickedVipbindCheck();
	afx_msg void OnBnClickedBatbindCheck();
	afx_msg void OnBnClickedIccidbindCheck();
	afx_msg void OnBnClickedMacbindCheck();
	afx_msg void OnBnClickedEquipmentbindCheck();
	afx_msg void OnBnClickedRfidbindCheck();

	void CleanBindCheck();//����󶨸�ѡ��

	static map<int, CString>SyllableMap;//����û���ѡ�ֶ�
	static map<int, CString>BindMap;//����û���ѡ��

	//�༭�����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsNumber(const CString& strTest);//�ж����ֺ���ĸ
	BOOL IsNumber2(const CString& strTest);//ֻ�ж�����
	afx_msg void OnEnSetfocusImei2Edit();

	//��������
	CString strpcip, strpcname;//�������ƺ�ip
	int notype;//����ĿǰҪɨ������
	CString notypename[8];//�������IMEI��VIP���ַ��� ע�⣬notypenameĿǰ��Ϊ������Ķ���������������Ϊ�����ں����߼��Ķ�������������ģ�Ŀǰ����Գ�����Ӱ�죬������ʱ������
	CString strno1,strno2,strno3;//�������ɨ�����͵�ֵ������ѡ��SNʱ��������ŵľ���SN��
	CString strzhidan;//��ŵ�ǰѡ�е��Ƶ���
	int chjudgeflag;//�ʺи�ѡ��ı�־
	CString strimeistart, strimeiend;//IMEI��ʼ�ͽ���


	//���ݿ����ģ��
	CString SyllablestrSql;

	void InitComboBox();//��ʼ���Ƶ���������
	BOOL JudgeZhidan(CString zhidan);//�ж��Ƶ���
	void Savesyllable();//�����ֶ�ѡ��
	void Readsyllable(BOOL CheckEx);//��ȡ�ֶ�ѡ��
	void Savebind();//�����ѡ��
	void Readbind(BOOL CheckEx);//��ȡ��ѡ��
	void Saveconfig();//����һЩ����
	void Readconfig();//��ȡһЩ����


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

	afx_msg void OnBnClickedLockButton();
	afx_msg void OnBnClickedOpenimei3editCheck();

	afx_msg void OnEnSetfocusImei1Edit();
};
