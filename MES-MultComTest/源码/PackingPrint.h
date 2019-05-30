#pragma once

#include "bartender.h"
#include "WriteIMEIDlg.h"

//�ж��Ƿ�����ļ���
#include "io.h"
// CPackingPrint �Ի���
#define MAX_ORDER_NUM 64

class CPackingPrint : public CDialog
{
	DECLARE_DYNAMIC(CPackingPrint)

public:
	CPackingPrint(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPackingPrint();

// �Ի�������
	enum { IDD = IDD_PACKING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	CString OrderDataName[MAX_ORDER_NUM];
	CString OrderData[MAX_ORDER_NUM];
	CString OrderDataRerurn[MAX_ORDER_NUM];
public:
	IBtApplication		btApp;														//DBtApplication��IBtApplication����quit����
	IBtFormat			btFormat;
	IBtFormats*			btFormats;
public:
	void  SaveConfig();
	void  LoadConfig();
	BOOL GetATSetting(CString SettingChar);
	void PrintFun(CString IMEIData,CString PhoneNum);//��ӡ����
	void PrintFunIni();//��ӡ����
	void PrintFunEnd();//��ӡ����
public:
	afx_msg void OnBnClickedButton2();//���´�ӡ
	afx_msg void OnBnClickedButtonstart1();
	// Bartenderģ���ļ�·��
	CString BtwFile;
	// ��ӡ��������
	CString PrintSetting;
	afx_msg void OnBnClickedButton3();
};
