// CombineImages.cpp: implementation of the CCombineImages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombineImages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//���캯��
CCombineImages::CCombineImages()
{
	m_bAddText = FALSE;	
	m_strTextInfo = "";
	m_pWnd = NULL;
	m_nCombineFlag = 0;

}

//��������
CCombineImages::~CCombineImages()
{
	//��細�ھ��ɾ��
	if(m_pWnd !=NULL)
	{
		m_pWnd = NULL;
		delete m_pWnd;
	}
	
}

///////���û�����Ϣ////////////////////////////////////
void CCombineImages::SetpWnd(CWnd* pWnd)
{
	m_pWnd = pWnd;	
}

void CCombineImages::SetAddTextFlag(BOOL bAddText)
{
	m_bAddText = bAddText;
}

void CCombineImages::SetTextInfo(CString strTextInfo)
{
	m_strTextInfo = strTextInfo;
}
///////////////////////////////////////////////////


////////////////////////�ϳ�ͼƬ����ں���//////////////////////////////
/*
����:strSourceNames ����Ҫ�ϳɵ�bmpͼƬ������(ָ��,�ɶ�̬���������С)
	 nNum			���鳤��
	 strDestName	�ϳ��ļ���ȫ·��
	 WOrH			�������ǰ�����ϳɣ�0-����1-����
����: �ϳ�ͼƬ����ں��������ԶԶ��ſ����ͬ���߶Ȳ�ͬ��ͼƬ���кϳ�
����:BOOL��,TRUE Ϊ�ɹ���FALSEΪʧ��
*/
BOOL CCombineImages::CombineImagesEntry(CString * strSourceNames, 
										int nNum, 
										LPCTSTR strDestName,
										int WOrH)
{
	m_nCombineFlag = 1;				//��ʼ�ϳɱ�־

	BOOL bSuccess = FALSE;

	m_WOrH = WOrH;

	/////////��������ÿ��ͼƬ�Ļ�����Ϣ/////////////////////
	long * ImageSizePer;
	ImageSizePer = new long[nNum];		//ÿ��ͼƬ�ܳ�����

	long * ImageWidthPer;
	ImageWidthPer = new long[nNum];		//ͼƬ�������
	long * ImageHeightPer;
	ImageHeightPer = new long[nNum];	//ͼƬ�߶�����

	LPBYTE * lpImagePer;				//��ʱͼƬ����������
	lpImagePer = new LPBYTE[nNum];
	///////////////////////////////////////////////////////
		
	long lAllSize = 0;		//����ͼƬ�ĳ��ȣ��Ա�ϳ�ͼ��
	long lAllHeight= 0;		//�ϳ�ͼ�ĸ߶�
	long lAllWidth= 0;		//�ϳ�ͼ�Ŀ��

	CString strFileName = "";	//��ʱ�ļ���
	int i;

	////////////////�õ��������ݣ��Ա�ϳ�ͼƬ/////////////////////
	for(i = 0;i<nNum;i++)
	{
		strFileName = strSourceNames[i];
		BITMAPINFO bmpInfo = m_PicProcess.GetBmpInfo(strFileName);	//get bmp info

		if(bmpInfo.bmiHeader.biWidth <=0)
		{
			m_nCombineFlag = 0;		//��Ч���ϳɽ���
			AfxMessageBox("ͼƬ�����ڻ�ͼƬ��Ч��");
			return FALSE;
		}
		
		ImageWidthPer[i] = bmpInfo.bmiHeader.biWidth;		//��
		ImageHeightPer[i] = bmpInfo.bmiHeader.biHeight;		//��
		//��ÿ��ͼƬ��С�ŵ�������
		ImageSizePer[i] = ImageWidthPer[i] * ImageHeightPer[i] * bmpInfo.bmiHeader.biBitCount;
		
		//lAllHeight = lAllHeight + ImageHeightPer[i];	//�ϳ�ͼ�ܸ߶ȵ��ۼ�
		//lAllWidth = lAllWidth + ImageWidthPer[i];		//�ϳ�ͼ�ܿ�ȵ��ۼ�
		
		lAllSize = lAllSize + ImageSizePer[i];			//�ϳ�ͼ��С���ۼ�

		//�õ���ǰͼƬ��������
		LPBYTE lpImage;
		lpImage=new byte[ImageSizePer[i]];
		m_PicProcess.GetBmpBuffer(strFileName,lpImage);

		//������ָ��ͼƬ����������
		long lSizeTemp = ImageSizePer[i];
		lpImagePer[i] = new byte[lSizeTemp];
		memcpy(lpImagePer[i],lpImage,ImageSizePer[i]);

		//delete pointers
		if(lpImage != NULL)
		{
			lpImage = NULL;
			delete lpImage;
		}
	}
	
	//�����ϳ�ͼƬ�����õ��ϳ�ͼƬ������,ÿ�κϳ�2�ţ�ѭ���ϳ�////////////////////////
	//�ϳ� ��ҪlpImagePer[i], ImageSizeRowPer[i],ImageHeightPer[i], lAllSize 
	bSuccess = FALSE;
	long lAllSizeTemp = 0;	//��ʱ�ϳ�ͼ��С
	long lWidth,lHeight;	//�ϳ�ͼ����
	LPBYTE lpImageLast=NULL;		//���ϳ�ͼ������������

	int nFor = nNum - 1;	//ѭ�� nNum -1��,nNumΪͼƬ����,���3�ţ���ѭ��2��
	for(i=0;i<nFor;i++)
	{
		lAllSizeTemp = (ImageWidthPer[i] * ImageHeightPer[i] * 3) + 
				(ImageWidthPer[i+1] * ImageHeightPer[i+1] * 3);
		
		if(lpImageLast != NULL)
		{
			//���������ݣ���ɾ��
			lpImageLast = NULL;
			delete lpImageLast;
		}

		//����ռ�
		lpImageLast=new byte[lAllSizeTemp];
		if(m_WOrH == 0)		//����
		{
			lWidth = ImageWidthPer[i];
			lHeight = ImageHeightPer[i] + ImageHeightPer[i+1];
		}
		else		//����
		{
			lWidth = ImageWidthPer[i] + ImageWidthPer[i+1];
			lHeight = ImageHeightPer[i];
		
		}

		//��������ͼƬ�ĺϳ�
		bSuccess = CombineTwoImages(
					lpImagePer[i],lpImagePer[i+1],
					lpImageLast,lAllSizeTemp,
					ImageWidthPer[i],ImageWidthPer[i+1],
					ImageHeightPer[i],ImageHeightPer[i+1]
				);
		
		//�ѱ��κϳɵ���������Ϣ��i+1,����:ǰ�ߵ�1��2��ͼ�ϳɺ�
		//�Ѻϳ�ͼ����Ϣ��i+1,���´�ѭ��ʱ,��Ϊ�´εĵ�1��ͼ��Ϣ
		if((i+1) < nFor)
		{
			lpImagePer[i+1] = NULL;
			delete lpImagePer[i+1];
			lpImagePer[i+1] = new byte[lAllSizeTemp];
			memcpy(lpImagePer[i+1],lpImageLast,lAllSizeTemp);
			ImageWidthPer[i+1] = lWidth;
			ImageHeightPer[i+1] = lHeight;
		}
	}

	if(lpImageLast == NULL)
	{
		m_nCombineFlag = 0;		//��Ч���ϳɽ���
		return FALSE;			//��û�õ������򷵻�
	}

	//�Ƿ�������ֵ�ͼƬ,����ʱ��Ҫ��Щ��������ڿ�
	CString sInfo = m_strTextInfo;
	if(m_bAddText)
		m_PicProcess.AddTextToBmp((LPCTSTR)strDestName,m_pWnd,lpImageLast,lWidth,lHeight,sInfo);
		
	//�����bmp�ļ�
	m_PicProcess.SaveImage(strDestName, lpImageLast, lWidth,lHeight);
	
	/////delete pointers
	if(lpImageLast == NULL)
	{
		lpImageLast = NULL;
		delete lpImageLast;
	}

	for(i = 0;i<nNum;i++)
	{
		if(lpImagePer[i] != NULL)
		{
			lpImagePer[i] = NULL;
			delete lpImagePer[i] ;
		}
	}

	if(ImageSizePer !=NULL)
		delete [] ImageSizePer;

	if(ImageWidthPer !=NULL)
		delete [] ImageWidthPer;

	if(ImageHeightPer !=NULL)
		delete [] ImageHeightPer;
	
	Sleep(50);
	m_nCombineFlag = 0;		//�ϳ���� 
	
	return bSuccess;

}
/////////////////////////////////////////////////////////////

//�����ϳ�����ͼƬ����//////////////////////////////////////////
//Buf1,Buf2Ϊ2��ͼƬ����BufLastΪ�ϳ�����BufLastLenΪ�ϳ�����С
//ImageWidthPer1,ImageWidthPer2Ϊ2��ͼƬ���ԵĿ��
//lHeight1,lHeight2Ϊ2��ͼƬ�ĸ��Ը߶�
BOOL CCombineImages::CombineTwoImages(LPBYTE Buf1,LPBYTE Buf2,
									  LPBYTE BufLast,long BufLastLen,
									  long ImageWidthPer1,
									  long ImageWidthPer2,
									  long lHeight1,long lHeight2)
{
	
	int i,len;
	long lSizeAll1,lSizeAll2;
	if(m_WOrH == 0)		//����
	{
		len = ImageWidthPer1 * 3;		
		lSizeAll1 = len * lHeight1;		//��1��ͼ�Ĵ�С
		lSizeAll2 = len * lHeight2;		//��2��ͼ�Ĵ�С
	}
	else	//����
	{
		
		
		
	}
	

	if(BufLastLen != lSizeAll1 + lSizeAll2)
		return FALSE;
	
	if(m_WOrH == 0)		//����
	{
		for(i=0;i<lHeight1;i++)
		{
			//��1��ͼ���ڵ�2���ϱߣ������Ǵ��ϰ벿�ֿ�ʼ
			//lSizeAll2����˼�ǰѵ�1��ͼ�ƶ���ǰlSizeAll2����2��ͼƬ�ĳ��ȣ�
			memcpy(BufLast + i*len + lSizeAll2, Buf1+i*len,len);
		}
		for(i=0;i<lHeight2;i++)
		{
			//��2��ͼ
			memcpy(BufLast + i*len, Buf2+i*len,len);
		}
	}
	else
	{
		
	}
	
	return TRUE;
}
//////////////end of combine 2 pictures///////////////

