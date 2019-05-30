#pragma once

#include "AdoInterface.h"
#include "afxwin.h"
#include "reportctrl.h"
#include "afxcmn.h"
#include "Para1.h"

// CBT_Addr_Setting �Ի���


class CBT_Addr_Setting : public CDialog
{
	DECLARE_DYNAMIC(CBT_Addr_Setting)

public:
	CBT_Addr_Setting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBT_Addr_Setting();

// �Ի�������
	enum { IDD = IDD_ALLSETTING };
	CAdoInterface myado;
	CAdoInterface Mulmyado[16];
	//CPara1 m_para1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	//afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);  //�Զ�����
	//afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
/*
public:
	void ImportListData(int paraNum,PARAMETER paraArray[]);
	int LoadIniFile(CString strLoadFile,PARAMETER paraArray[]);
	void LoadParaList(CString strLoadFile);//װ�ز������������


	void SaveBLE_Setting(CString strLoadFile);//��������
	int  ExportListData(void);//(PARAMETER exportArray[]);	
	void SaveIniFile(CString strSaveFile,int paraNum,PARAMETER paraArray[]);*/


	DECLARE_MESSAGE_MAP()
public:
	// ������ַ��Ϣ
	CString BtAddrStart;
	CString BtAddrEnd;
	// �������ݿ�
	BOOL DB_CONNECT;
	// ���ݿ���Ϣ
	CString m_server;
	CString m_db;
	CString m_user;
	CString m_pwd;
	// �ϴ���Ϣ
	CString Order_Num_up;
	CString Machine_Type;
	CString Modulation_Tppe;
	CString Line_Number;
	CString Floor;
	CString Product_Address;
	CString Is_Return;
	CString Software_Version;
	CString Operator;
	CString BT_Addr_EndShow;
	CString Hardware_Version;
	// ѡ��IMEI1
	BOOL IMEI1_Choose;
	BOOL IMEI2_Choose;
	BOOL IMEI3_Choose;
	BOOL IMEI4_Choose;
	// д����MAC��ַ
	BOOL BT_MAC_ADDR_Choose;
	// WIFI_MAC��ַ
	BOOL WIFI_MAC_ADDR_Choose;
	// ���IMEI�Ƿ�Ϸ�
	BOOL IMEI_CheckValid_Choose;
	// ѡ���Ƿ��Զ�����IMEI
	BOOL AutoIMEI_Choose;
	// �������汾ѡ��
	BOOL Software_Version_Check_Choose;
	// ��ʱʱ������
	CString WatchDog_Time;
	// �Զ�����IMEI��ʱ�����õ�IMEI��ʼ�ŵĸ�14λ
	CString AutoIMEI_Start_Num;
	// ѡ�������ͬ��IMEI
	BOOL UseSameIMEI_Choose;
	// �Զ�����������ַѡ��
	BOOL AutoBTAddr_Choose;
	// WIFI��ʼ��ַǰ��λ
	CString AutoWIFIAd_Start_Num;
	// �Զ�����WIFI��ַ
	BOOL AutoWIFIAddr_Choose;
	// �ֻ�ģʽѡ��
	CComboBox TestMode_Control;
	CString TestMode_S;
	int TestMode_N;
	// WIFI��ʼ��ַ
	CString WIFIAddrStart;
	CString WIFIAddrEnd;
	// IMEI��ʼ��
	CString IMEIStart;
	CString IMEIEnd;
	// ���ݿ�������IMEI1
	BOOL IMEI_DB_Choose;
	// BT\WIFI_MAC ��ַ����IMEI1ѡ��
	BOOL MAC_DB_Choose;
	// �ϴ�MSN�����ݿ�ѡ��
	BOOL MSN_Upload_Choose;
	// �������ݿ����Ƿ��غ�
	BOOL Check_repeat_Choose;
	// ��ʼ����ʱ��ѡ���Ƿ��������ݿ�
	BOOL Initial_Connect_DB;
	// ���Գɹ����ϴ����ݵ����ݿ�
	BOOL UP_DB_Data;
	// ����������IMEI\BT_WIFI_MAC������ĶԱ�
	BOOL IMEI_MAC_Check_Choose;
	// ɨ��ǹģʽѡ��
	BOOL Scan_Choose;
	// ����ʹ��Ȩ�޿���
	CButton Software_Version_Check_Choose_Control;
	CButton IMEI_MAC_Check_Choose_Control;
	CButton IMEI_CheckValid_Choose_Control;
	// Ȩ�޺�
	CString ServerIP_MAC;
	// �Զ����ɺŶ�ʱ���Զ�ѭ����ʼд��
	BOOL Repeat_AutoStart;
	// ORACLE���ݿ�����ѡ��
	BOOL ORACLE_Connect_Choose;
	// ORACLE���ݿ��������ַ
	CString ORACLE_m_server;
	CString ORACLE_m_user;
	CString ORACLE_m_pwd;
	// ѡ����
	BOOL BARCODE_Choose;
	// IPServer_MACʹ��
	BOOL ChangeEN_Choose;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tab;
	afx_msg void OnBnClickedOk();
	// �Ƿ�����������
	//CButton Unlock_Code_Choose;
	BOOL Unlock_Code_Choose;
	// �������λ������
	CComboBox Unlock_code_Control;
	int Unlock_code_N;
	CString Unlock_code_S;
	// ����ѡ��
	BOOL NETCODE_Choose;
	// ����ֻΪ����
	BOOL NETCODE_Only_Num_Choose;
	// ���ϴ����ݿ�ѡ��
	BOOL JUST_UpDate_DB_Choose;
	// ����IMEI-MSN���е���ʹ�ñ�־
	BOOL UpdateIMEI_MSN_Choose;
	// ������ʼ��
	CString NetCodeStart;
	CString NetCodeEnd;
	// ��λ����ˮ��
	CString NetCode_SwiftNumber;
	// д�ñ��Ѿ�����IMEI,�����¶�Ӧ������
	BOOL IMEI_Exist_Update_NC_Choose;
	// ���������ѯ��ǰ�Ļ������Ա�
	BOOL Machine_Type_NC_CompareChoose;
public:
	void FileSelectWizard(TCHAR *pszDir);
	// LOGת�湲���ļ���
	CString DestFile;
	afx_msg void OnBnClickedButtonstart12();
};
