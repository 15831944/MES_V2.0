#pragma once
#include "afxwin.h"
#include "WriteIMEIDlg.h"
/*
#include "stdafx.h"
#include "resource.h"
#include "WriteIMEIDlg.h"
*/
// CCSR_Function_Test �Ի���
#include "afxmt.h"//CEvent

#include "CSRBTHostControl.h"

class CCSR_Function_Test : public CDialog
{
	//DECLARE_DYNAMIC(CCSR_Function_Test)

public:
	CCSR_Function_Test(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCSR_Function_Test();

// �Ի�������
	enum { IDD = IDD_CSR_test };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL OPen_Serial_Port();
	char*  Send_Serial_Order(char** Vaule_Return,CString strCommand_Vaule);
	BOOL CSR_Function_Test_Thread();
	void LogShow_exchange(int State,CString Msg_Log,CEdit* Final_Result_Control,CEdit* m_Result,int WndChoose=0);     //����  �̱߳��� �ĺ���
	void Log_Show_Global(void);                           //����BLE  log��Ϣ�߳�

	void PrintError(CString cs);
	void PrintOK(CString cs);
	void Delay_Clock(UINT mSecond);

	int CharToHexChar(char ch);
	bool BLE_Function_Judge(int i,CString BLE_FT_Item,char* Serial_Order_Return);


	CBT_Addr_Setting BLE_Setdlg;
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
public:
	UINT TimerID;
	UINT Sleep_Idle_Time;
	//���ݿ��ϴ��ֶ�����
	CString Function_Test_Order_Number;
	CString Software_Version;
	CString BLE_MAC_Addr;

	CString LimitOfTool;
	CString SN_BLE;
	CString Cal_Sign;
	UINT    Coupling_Sign;

	CString Power_INI; //��ʼ����
	CString Power_Diff;//������

	CString SENSOR_Gravity[3][2];
	CString Temperature;
	CString HeartRate;
	CString SENSOR_Gravity_ALL;

	CString HeartRate_RL[2][2];//��ɫ��
	CString HeartRate_IL[2][2];//�����

	CString HR_DC;

public:
	bool Watch_Result;//���۹۲���

	CString Vaule_Return_Count_CS;  //���յ��Ĳ���


private:
	CEvent RunCommandList;//��ʼ״̬Ϊ��Ч�źţ���ʼ��һ��ָ��
	CWinThread* Measure_Thread_Handle;
	CWinThread* LogShow_Thread_Handle;
	COLORREF gColor;
	CString LOG_Show_Old;
	bool Initial_Handle;

	PARAMETER_MAIN paraArray[MAXPARANUM];
private:
	void BLE_UI_LoadConfig();

protected:
	afx_msg HBRUSH CCSR_Function_Test::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 

public:
	// ���ڶ˿�
	CComboBox m_Port;
	// ���ڲ�����
	CComboBox m_Baud;

	HANDLE hPort;
	// BLE���Խ��
	CEdit BLE_m_Result;
	CString BLE_m_Result_C;
	//LOG����
	CString BLE_Msg_Log;
	int BLE_State;
	// BLE���ܲ������ս��
	CString BLE_Final_Result;
	CEdit BLE_Final_Result_Control;
	afx_msg void OnBnClickedButton1();//��ʼ����
	CButton STOP_Test_Control;
	afx_msg void OnBnClickedButton2();//ֹͣ����
	CButton START_Test_Control;
	afx_msg void OnBnClickedButton3();//�رմ���
	// ������ʾ
	CEdit BLE_Operate_Guide_Control;
	afx_msg void OnBnClickedButton4(); //PASS
	afx_msg void OnBnClickedButton11();//FAIL

	afx_msg void OnBnClickedButton5();
public:
	BOOL StartSearch();//��ʼ��������
public:
	CCSRBTHostControl    CSRBTHostApp;
	CWinThread* Test_Handle;
	BOOL m_bStopFlag;
	//int ReIniCount;
	afx_msg void OnBnClickedButton36();
	// MAC_RSSI��¼
	CEdit BLE_MAC_RSSI;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
