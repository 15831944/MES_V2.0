#pragma once

#include "gdiplusinit.h"
#include <gdiplus.h>
using namespace Gdiplus;
class CPictureProcess
{
public:
	CPictureProcess(void);

	virtual ~CPictureProcess(void);

	//���ֵ��ӵ�λͼ
	void AddTextToBmp(CString sFile, CWnd* pWnd, BYTE *Buf, int Width, int Height, CString sInfo);
	
	//��ʾͼƬ:�ļ�
	void ShowPicture(CString strPicName,HWND hwndStill);

	//��ʾͼƬ:��
	void ShowPicture(BYTE *bytePicName,HWND hwndStill);

	//��ʾͼƬ-byte��
	void ShowPictureByByte(BYTE *Buf,HWND hwndStill,int Width,int Height);

	//bmp��jpgͼƬ�ļ�ת����������
	VARIANT ChangePicFileToBYTE(CString strPicName);
	//���ݿ�blobͼ��ת����bmp�ļ�
	BOOL DBStreamToBmpFile(VARIANT vDBStream,long lngStreamLength,CString strPicFile);

	//�õ�bmpͼƬ�ĳ������� ��*��*λ��
	BITMAPINFO GetBmpInfo(LPCTSTR lpszBmpFile);

	//�õ�bmpͼƬ��������
	void GetBmpBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer);
	
	//�õ�bmpͼƬ��ת������������Ա㱱�����ӳ���ʶ����...
	//�����ǵ��ö�̬�ⷽʽ�ģ����Ҫ������dllName
	void GetBmpVerBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer, CString dllName);


	//�����������浽ͼƬ��
	BOOL SaveImage(LPCTSTR lpszFile, LPBYTE newbuffer, long lWidth,long lHeight);	//�ϳ�ͼ����

	//ָ��bmp����������߶ȣ�lWidth��lHeightΪԭʼ���Ŀ��
	//Ŀ�����Ŀ��ԭʼ��һ����nHeightTimesΪԭʼ���߶ȵı���
	void ZoomImageHeight(LPBYTE sourcebuffer, LPBYTE destbuffer,long lWidth,long lHeight,int nHeightTimes);

	//bmp����߶Ⱥ󱣴�ͼƬ���Ա�鿴ԭʼͼƬ 288->576	nHeightTimesΪԭʼͼƬ�߶ȵı���
	BOOL ZoomImageHeight(LPCTSTR lpszFile, int nHeightTimes);
	
	//jpg�ļ�ת����bmp�������ر�ת�����ļ���������dll���õ�ͬ����bmp��
	CString ChangeJPGToBMP(CString strJPGFileName,CString dllName);


	CString ChangeJPGToBMP2(CString strJPGFileName);
	int GetCodecClsid(const WCHAR* format, CLSID* pClsid);//�������ʶ��

	//bmp�ļ�ת����jpg�������ر�ת�����ļ���������dll���õ�ͬ����jpg��
	CString ChangeBMPToJPG(CString strBMPFileName,CString dllName);
	CString ChangeBMPToJPG2(CString strBMPFileName);
public:
	//ULONG_PTR m_GdiplusToken;
	//GdiplusStartupInput m_GdiplusStartupInput;

};
