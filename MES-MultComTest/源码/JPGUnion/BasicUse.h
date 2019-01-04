// BasicUse.h: interface for the CBasicUse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICUSE_H__4593B362_44F0_4ECD_9982_FA88F21FD374__INCLUDED_)
#define AFX_BASICUSE_H__4593B362_44F0_4ECD_9982_FA88F21FD374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBasicUse  
{
public:
	CBasicUse();
	virtual ~CBasicUse();
	CString GetApp_Path();		//ȡ��Ӧ�ó�������Ŀ¼
	CString GetCurDateTimeString();	//���ص�ǰʱ��Ĵ�,�Ա�SQL�����
	CString GetDateTimeString(CTime tDatetime);	//����ʱ��Ĵ�,�Ա�SQL�����

	CString GetCurDateString();	//���ص�ǰ���ڵĴ�yyyy-mm-dd
	CString GetPicCurDateString();	//���ص�ǰ���ڵĴ�,�Ա㴴�������ļ���ʱ��yyyymmdd

	CString GetCurYearString();		//���ص�ǰ����ݴ�
	CString GetCurMonthString();	//���ص�ǰ���·ݴ�
	CString GetCurDayString();		//���ص�ǰ�����Ӵ�

	//����ָ��ʱ�䴮ȡ�����ڴ�yyyymmdd
	CString GetDate_Connect(CString strDatetime);

	//����ָ��ʱ�䴮ȡ������ʱ�䴮yyyymmddhhmmss
	CString GetDateTime_Connect(CString strDatetime);
	CString GetCurDateTimeString_C(CTime tDatetime);	//��ǰʱ���yyyymmddhhmmss

	BOOL CreatePath(CString strFilePath);		//����Ŀ¼
	BOOL PathExists(CString strFilePath);		//Ŀ¼�Ƿ����
	BOOL FileExists(CString strFileName);		//�ļ��Ƿ����
		
	CString JPGToBMPString(CString strJPGFileName);		//jpg�ļ�����ת����.bmp��
	CString BMPToJPGString(CString strBMPFileName);		//bmp�ļ�����ת����.jpg��

	HINSTANCE LoadDll(CString strDllName);			//��̬����dll

	//���ݿ�ͼ��ת����bmp�ļ�
	//����CPictureProcess�����ˣ�����ͼƬ������Ĺ���
	//BOOL DBStreamToBmpFile(VARIANT vDBStream,long lngStreamLength,CString strPicFile);

	//ɾ��ָ���ļ����������ļ�
	BOOL DeleteFilesInFolder(CString strFolder);

};

#endif // !defined(AFX_BASICUSE_H__4593B362_44F0_4ECD_9982_FA88F21FD374__INCLUDED_)
