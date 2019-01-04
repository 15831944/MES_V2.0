
// ThumbnailBoxDlg.h : ͷ�ļ�
//

#pragma once
#include "thumbnailbox.h"
#include "resource.h"		

// CThumbnailBoxDlg �Ի���
class CThumbnailBoxDlg : public CDialog
{
// ����
public:
	CThumbnailBoxDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_THUMBNAILBOX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	afx_msg void OnTimer(UINT nIDEvent);
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
	CString UnionPicPath;
	CString PicPath0;
	CString PicPath1;
	int ShowPicTime;
	int ShowHandleHun; 
};
