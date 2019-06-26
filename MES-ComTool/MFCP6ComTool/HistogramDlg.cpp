// HistogramDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg �Ի���

/*ȫ�ֱ����ͺ���*/
CHistogramDlg *HistogramDlgHandle;


IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramDlg::IDD, pParent)
{
	m_RectName = NULL;
	m_RectValue = NULL;
	m_RectValue_draw = NULL;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHistogramDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg ��Ϣ�������
void CHistogramDlg::DrawPrev(int DrawTypeNum,CString *RectName, int *RectValue)
{
	if (m_RectName != NULL)
	{
		delete[] m_RectName;
		m_RectName = NULL;
	}

	if (m_RectValue != NULL)
	{
		delete [] m_RectValue;
		m_RectValue = NULL;
	}

	if (m_RectValue_draw != NULL)
	{
		delete[] m_RectValue_draw;
		m_RectValue_draw = NULL;
	}

	m_NumRect = DrawTypeNum;
	m_RectName = new CString[m_NumRect];
	m_RectValue = new int[m_NumRect];
	m_RectValue_draw = new int[m_NumRect];

	for (int i = 0; i<m_NumRect; i++){
		m_RectName[i] = RectName[i];
		m_RectValue[i] = RectValue[i];
		m_RectValue_draw[i] = RectValue[i];
	}

	m_MaxValue = m_RectValue[0];//ѡ��������ֵ�������ǵ�һ�����
	for (int i = 0; i < m_NumRect; i++){
		if (m_MaxValue < m_RectValue[i]){
			m_MaxValue = m_RectValue[i];
		}
	}

}

void CHistogramDlg::ClearPoint()
{
	if (m_RectName != NULL)
	{
		delete[] m_RectName;
		m_RectName = NULL;  
	}

	if (m_RectValue != NULL)
	{
		delete[] m_RectValue;
		m_RectValue = NULL;
	}

	if (m_RectValue_draw != NULL)
	{
		delete[] m_RectValue_draw;
		m_RectValue_draw = NULL;
	}
	InvalidateRect(NULL);
	UpdateWindow();
}

void CHistogramDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();

}


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	if (m_RectValue != NULL)
	{
		CRect rc; //����һ�����Σ�
		GetClientRect(rc); //��ȡ�Ի���ͻ����ľ��Σ�
		dc.FillSolidRect(rc, RGB(255, 255, 255));//����Ϊ��ɫ����
		rc.DeflateRect(20, 20); //��С���δ�С,��������ͼ�ͶԻ���ı��غϣ�
		int nSegWidth = rc.Width() / (m_NumRect*1.5); //����ÿ�����εĿ�ȣ�m_NumRect����Ҫ��������ͼ����������

		/////////////////////////////////////////////////////////////////�����������������������λ�߶ȣ�Ϊ��֤��λ�߶ȴ���1����m_MaxValue��С������¼��С����

		int count = 0;
		while (rc.Height() < (int)m_MaxValue){
			m_MaxValue /= 2;
			count++;
		}
		int nSegHeight = (int)(rc.Height() / m_MaxValue);//�������εĵ�λ�߶�

		/////////////////////////////////////////////////////////////////////��������Ҳ����С����

		for (int i = 0; i < m_NumRect; i++){
			for (int j = 0; j < count; j++){
				m_RectValue[i] /= 2;
			}
		}

		////////////////////////////////////////////////////////////////

		COLORREF crSeg = RGB(255, 0, 0); //������ɫ
		CBrush brush1(HS_FDIAGONAL, crSeg); //���û�ˢ1
		CBrush brush2(HS_BDIAGONAL, crSeg); //���û�ˢ2
		CPen pen(PS_INSIDEFRAME, 80, crSeg); //���û���
		CBrush* oldBrush = dc.SelectObject(&brush1);
		CPen* oldPen = dc.SelectObject(&pen);
		CRect rcSeg(rc); //����һ�����Σ������Ļ����ξ��ǻ������ı����ĸ߶ȶȣ����
		rcSeg.right = rcSeg.left + nSegWidth;

		CRect rcStr;
		for (int i = 0; i < m_NumRect; i++){
			if (i % 2)
				dc.SelectObject(&brush1);
			else
				dc.SelectObject(&brush2);
			rcSeg.top = rcSeg.bottom - (int)(m_RectValue[i] * nSegHeight) - 2;//����ÿ�����εĸ߶ȣ�
			dc.Rectangle(rcSeg);
			CString str;
			str.Format("%dDB", m_RectValue_draw[i]);//������ͼ���������
			dc.DrawText(str, rcSeg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rcStr = rcSeg;
			rcStr.top = rcStr.bottom + 2;
			rcStr.bottom += 20;
			dc.DrawText(m_RectName[i]+"��", rcStr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rcSeg.OffsetRect(nSegWidth + 10, 0); //���������ƣ�
		}
		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


BOOL CHistogramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HistogramDlgHandle = this;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
