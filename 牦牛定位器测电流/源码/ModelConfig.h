#pragma once
#include "afxwin.h"
#include "AutoCombox.h"

// CModelConfig �Ի���

class CModelConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CModelConfig)

public:
	CModelConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModelConfig();

// �Ի�������
	enum { IDD = IDD_MODELCONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	/*�ؼ�ʹ�ܱ����ͺ���*/
	void ConfigVauleEnableWindows(BOOL choose);//�������õı༭��ʹ�ܺ���

	/*ģ��ؼ������ͺ���*/
	CAutoCombox m_SelectModelCombo;

	CEdit m_AddModelControl;
	CString m_AddModelValue;

	CEdit m_TestCommandControl;//����ָ��
	CString m_TestCommandValue;
	CEdit m_TestCommandReplyControl;
	CString m_TestCommandReplyValue;

	CEdit m_RidCommandControl;//��RIDָ��
	CString m_RidCommandValue;
	CEdit m_RidCommandReplyControl;
	CString m_RidCommandReplyValue;

	CEdit m_StandbyCommandControl;//����ָ��
	CString m_StandbyCommandValue;
	CEdit m_StandbyCommandReplyControl;
	CString m_StandbyCommandReplyValue;

	CEdit m_SleepCommandControl;//˯��ָ��
	CString m_SleepCommandValue;
	CEdit m_SleepCommandReplyControl;
	CString m_SleepCommandReplyValue;

	CEdit m_DBStandbyUpControl;//��������
	float m_DBStandbyUpValue;
	CEdit m_DBStandbyDownControl;//��������
	float m_DBStandbyDownValue;

	CEdit m_DBSleepUpControl;//˯������
	float m_DBSleepUpValue;
	CEdit m_DBSleepDownControl;//˯������
	float m_DBSleepDownValue;

	CEdit m_CommandCountControl;//ָ��ʹ���
	int m_CommandCountValue;
	CEdit m_CommandSendIntervalControl;//ָ��ͼ��
	int m_CommandSendIntervalValue;
	CEdit m_CommandReadIntervalControl;//ָ���ȡ���
	int m_CommandReadIntervalValue;

	void AddModelDBOperation(CString ModelName);//��ӻ������ݿ����
	void DeleteModelDBOperation(CString ModelName);//ɾ���������ݿ����
	void SaveModelDBOperation();//����������ݿ����
	void FindModelDBOperation();//���һ������ݿ����
	void InitModelDBOperation();//��ʼ���������������ݿ����

	afx_msg void OnBnClickedAddmodelButton();
	afx_msg void OnBnClickedDeletemodelButton();
	afx_msg void OnBnClickedSavemodelinfoButton();
	afx_msg void OnCbnKillfocusSelectmodelCombo();
	afx_msg void OnCbnSelendokSelectmodelCombo();
	afx_msg void OnCbnSelchangeSelectmodelCombo();


	/*���������ͺ���*/
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
