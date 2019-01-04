
// ThumbnailBoxDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThumbnailBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThumbnailBoxDlg �Ի���




CThumbnailBoxDlg::CThumbnailBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThumbnailBoxDlg::IDD, pParent)
{
}

void CThumbnailBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThumbnailBoxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_TIMER()   //��ʱ��
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CThumbnailBoxDlg ��Ϣ�������

BOOL CThumbnailBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rcClient;
	GetClientRect(rcClient);
	CThumbnailBox* pThumbnailBox = new CThumbnailBox;
	/*for(int i = 1; i <= 1; i++)
	{
		TCHAR lpszImageName[64] = {0};
		//_stprintf(lpszImageName, TEXT("image\\%d (2).jpg"), i);//1 (2)
		//cstring str=_T("hello");
		//���Ƕ��ֽڵ�ʱ�� ����ЧΪ cstring str="hello";
		//��Ϊunicodeʱ������ЧΪcstring str=L"hello";
		if(pThumbnailBox->AddImage(_T(UnionPicPath))==FALSE)
		{
			_stprintf(lpszImageName, TEXT("image\\NULL.jpg"));
			pThumbnailBox->AddImage(lpszImageName);
		}
	}*/
	TCHAR lpszImageName[64] = {0};
	if(pThumbnailBox->AddImage(_T(PicPath0))==FALSE)
	{
		_stprintf(lpszImageName, TEXT("image\\NULL.jpg"));
		pThumbnailBox->AddImage(lpszImageName);
	}
	if(pThumbnailBox->AddImage(_T(PicPath1))==FALSE)
	{
		_stprintf(lpszImageName, TEXT("image\\NULL.jpg"));
		pThumbnailBox->AddImage(lpszImageName);
	}
	pThumbnailBox->Create(0, 0, rcClient.Width(), rcClient.Height(), this);

	MoveWindow(920,30,THUMBNAIL_BOX_WIDTH+20,THUMBNAIL_BOX_HEIGHT*2+20,TRUE);
	/*
	if(ShowPicTime>0)
		SetTimer(1, ShowPicTime, NULL);
	else
		SetTimer(1, 3000, NULL);
	*/
	CString Name;
	Name.Format("---------------------------------------------------------------------------��Ƭ%d------------------------------------------------------------------------------------",ShowHandleHun+1);
	SetWindowText(Name);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CThumbnailBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CThumbnailBoxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CThumbnailBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CThumbnailBoxDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
		case 1:
			KillTimer(1);
			//exit(0);
			//EndDialog(0);//SendMessage(WM_CLOSE,   0,   0);//OnCancel();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}