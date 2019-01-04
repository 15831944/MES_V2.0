// CombineImages.h: interface for the CCombineImages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBINEIMAGES_H__18C220B4_89A5_4F07_996B_A7AC40C3EAD2__INCLUDED_)
#define AFX_COMBINEIMAGES_H__18C220B4_89A5_4F07_996B_A7AC40C3EAD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PictureProcess.h"

class CCombineImages  
{
public:
	CCombineImages();
	virtual ~CCombineImages();

	//�ϳ�ͼƬ����ں���,WOrHΪ�������ǰ�����ϳɣ�0-����1-����
	BOOL CombineImagesEntry(CString * strSourceNames, int nNum, LPCTSTR strDestName,int WOrH);

	//�Կ��Ʊ������д�����˽�б���˵��
	void SetpWnd(CWnd * pWnd);
	void SetAddTextFlag(BOOL bAddText);
	void SetTextInfo(CString strTextInfo);
private:

	//�����ϳ�����ͼƬ����BufLast
	BOOL CombineTwoImages(LPBYTE Buf1,LPBYTE Buf2,
						  LPBYTE BufLast,long BufLastLen,
					      long ImageWidthPer1,long ImageWidthPer2,
						  long lHeight1,long lHeight2);
	
////��Ա����////////////////////////////////////////////////
private:
	CPictureProcess m_PicProcess;		//ͼƬ���������

	CWnd * m_pWnd;			//���ݹ����Ĵ��ڵľ�����Ա������Ϣ��
	BOOL m_bAddText;				//��Ϣ�Ƿ���ӵ�ͼƬ��
	CString m_strTextInfo;	//���ӵ���Ϣ����
	int m_WOrH;				//�������Ǻ���ϳ�ͼƬ 0-����1-����
public:
	//�ϳ�״̬:1:���ںϳ�,�����������ϳ�,0-�ϳ���ϣ����Խ�����һ�κϳ�
	int m_nCombineFlag;
};

#endif // !defined(AFX_COMBINEIMAGES_H__18C220B4_89A5_4F07_996B_A7AC40C3EAD2__INCLUDED_)
