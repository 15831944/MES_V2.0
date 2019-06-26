#pragma once
#include "resource.h"

// CHistogramDlg �Ի���

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistogramDlg();

// �Ի�������
	enum { IDD = IDD_Histogram };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	int m_NumRect;//���ڼ�¼��Ҫ������������
	int m_MaxValue;//��¼��Ҫ��������ͼ����ߵĸ߶�
	CString *m_RectName;//���ڼ�¼����ͼ�����ƣ�
	int *m_RectValue;//���ڼ�¼����ͼ��ֵ��
	int *m_RectValue_draw;//���ڱ���*m_RectValue�е����ݣ�

	void DrawPrev(int DrawTypeNum, CString *RectName,int *RectValue);
	void ClearPoint();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};