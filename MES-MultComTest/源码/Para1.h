#pragma once

#define     MAXPARANUM       100

#include "ReportCtrl.h"
//#include "afxwin.h"
// CPara1 �Ի���
typedef struct
{
    CString paraID;     //ָ��Ψһ��ID��
    CString paraContent;//��Ӧ��ָ������

	bool     ifCheck;//�Ƿ�ѡ��
	CString showName;         //��ʾ���б��еĲ�����
    CString Low_Limit_Value;  //��������ֵ
	CString High_Limit_Value; //��������ֵ
	CString Other_ITEM;       //�����Զ�����
}PARAMETER;

class CPara1 : public CDialog
{
	DECLARE_DYNAMIC(CPara1)

public:
	CPara1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPara1();

// �Ի�������
	enum { IDD = IDD_CSR_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	CReportCtrl m_list_area_para;
//private:
	int numParameter;//��������
	CString fSectionName; // ini�ļ��ֶ���
	PARAMETER parameterArray[MAXPARANUM];//�洢���� ������
public:
	void ImportListData(int paraNum,PARAMETER paraArray[]);
	int LoadIniFile(CString strLoadFile,PARAMETER paraArray[]);
	void LoadParaList(CString strLoadFile);//װ�ز������������


	void SaveBLE_Setting(CString strLoadFile);//��������
	int  ExportListData(void);//(PARAMETER exportArray[]);	
	void SaveIniFile(CString strSaveFile,int paraNum,PARAMETER paraArray[]);
};

