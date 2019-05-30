#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDataBase �Ի���

class CDataBase : public CDialog
{
	DECLARE_DYNAMIC(CDataBase)

public:
	CDataBase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataBase();

// �Ի�������
	enum { IDD = IDD_DATABASE };

private:
	_ConnectionPtr m_pConn;
	_RecordsetPtr  m_pRS;
	CStringArray strArry;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	bool    SQL_Wrong_Excute;

	afx_msg void OnBnClickedButtonstart1();
	BOOL Ado(CString strConn);
	_RecordsetPtr&  GetRS(CString strSQL); //ִ��strSQL��SQL��䣬���ؼ�¼��
	CListBox m_list2;


	afx_msg void OnDblclkList2(); //list�ؼ�˫���¼�
	BOOL GetFieldsName(_RecordsetPtr RcdPtr, int nField, CString & strFieldName);   
	int  GetNum(CString strc);
	int  GetFieldsCount(_RecordsetPtr RcdPtr);
	CString m_str_list;
	CString m_str_list_Old;
	// д�ű���ֶ�
	CComboBox Section_Box_IMEIS;
	CString   Section_Box_IMEIS_CS;
	BOOL nSize_Record_Limit_Enable;
	long RECORD;
	CString RecordNumber;
	BOOL Time_Limit_Enable;
	// ��������
	CString Number;
	CString Start_Time;
	CString End_Time;
	// ������ʾ���
	CListCtrl m_list1;
	long RECORD_Show;

	//��ȡ�ֶ���
	int dataSize;
	CStringArray *strdataArray;//�����ַ�����ά����
	CString *strName;          //�ֶ����ַ���
	// ���ݿ��ѯ
	CButton Connect_DB_For_Find_Control;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton9();
	CButton Disconnect_DB_Control;

	_RecordsetPtr Execute(LPCTSTR lpszSQL, long lOptions);//ִ��SQL���
	afx_msg void OnBnClickedButton3();
	// ������IMEI�����ꡢ����
	BOOL Only_IMEI_NC_Machine;
	// �����������ݣ����ܱ�־λ
	BOOL AllNetCode_Choose;
	// д�����Ѿ��õı�־λ
	BOOL Set_WritedFlag;

	BOOL Wrong;
	// ���굼���ı�־λֵ
	CString NETCODE_Export_FlagValue;
	afx_msg void OnBnClickedButton4();
	CButton Clear_FlagValue_Control;
};
