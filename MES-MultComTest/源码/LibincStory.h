#pragma once


// CLibincStory �Ի���
#include "PictureEx.h"//��ʾgif
class CLibincStory : public CDialog
{
	DECLARE_DYNAMIC(CLibincStory)

public:
	CLibincStory(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLibincStory();

// �Ի�������
	enum { IDD = IDD_DIALOGSTORY };

protected:
	HICON m_hIcon_Story;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	CPictureEx Story_Show;
};
