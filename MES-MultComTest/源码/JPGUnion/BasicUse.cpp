// BasicUse.cpp: implementation of the CBasicUse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicUse.h"

#include <direct.h>		//_mkdir
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicUse::CBasicUse()
{

}

CBasicUse::~CBasicUse()
{

}

/*
* ȡ�õ�ǰӦ�ó�������·��
* ����:��
* ����ֵ CString�� 
*/
CString CBasicUse::GetApp_Path()
{
	CString strPath; 

    ::GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH)); 

    strPath.ReleaseBuffer(); 

	strPath.Format("%s",strPath);

    return strPath; 
}

/*
* ȡ�õ�ǰ����ʱ�䴮,Ŀ����Ϊ��SQL���ʱʹ��;
* ����:��
* ����ֵ CString�� 
*/
CString CBasicUse::GetCurDateTimeString(){
	CString strCurDatetime = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	strCurDatetime = tCurDatetime.Format("%Y-%m-%d %H:%M:%S");
	return strCurDatetime;
}

/*
* ȡ�����ڴ�,Ŀ����Ϊ��SQL���ʱʹ��;
* ����:�����ͱ���
* ����ֵ CString�� 
*/
CString CBasicUse::GetDateTimeString(CTime tDatetime)
{
	CString strCurDatetime = "";
	CTime tCurDatetime = tDatetime;
	strCurDatetime = tCurDatetime.Format("%Y-%m-%d %H:%M:%S");
	return strCurDatetime;
}

//����ʱ��ȡ�ô�
CString CBasicUse::GetCurDateTimeString_C(CTime tDatetime)
{
	CString strCurDatetime = "";
	CTime tCurDatetime = tDatetime;
	strCurDatetime = tCurDatetime.Format("%Y%m%d%H%M%S");
	return strCurDatetime;
}

/*
* ȡ�õ�ǰ���ڴ�yyyy-mm-dd
* ����:��
* ����ֵ CString��
*/
CString CBasicUse::GetCurDateString(){
	CString strCurDate = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	strCurDate = tCurDatetime.Format("%Y-%m-%d");
	return strCurDate;
}

//���ص�ǰ���
CString CBasicUse::GetCurYearString(){

	CString strCur = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	int intCur = tCurDatetime.GetYear();
	strCur.Format("%d",intCur);		//����ת�����ַ�����
	return strCur;

}

//���ص�ǰ���·�
CString CBasicUse::GetCurMonthString(){

	CString strCur = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	int intCur = tCurDatetime.GetMonth();
	strCur.Format("%d",intCur);		//����ת�����ַ�����
	return strCur;

}

//���ص�ǰ������
CString CBasicUse::GetCurDayString(){

	CString strCur = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	int intCur = tCurDatetime.GetDay();
	strCur.Format("%d",intCur);		//����ת�����ַ�����
	return strCur;

}

/*
* ȡ�õ�ǰ���ڴ���ʽ:yyyymmdd
* ����:��
* ����ֵ CString��
*/
CString CBasicUse::GetPicCurDateString(){
	CString strCurDate = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	strCurDate = tCurDatetime.Format("%Y%m%d");
	return strCurDate;
}

//����Ŀ¼
BOOL CBasicUse::CreatePath(CString strFilePath){
	
	BOOL ret = false;

	/*
	//�÷�ʽֻ�ܽ�һ��Ŀ¼
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	::CreateDirectory(strFilePath, &attrib);
	if (PathExists(strFilePath) == true){
		ret = true;
	}

	
	*/

	//�÷�ʽ���Խ����༶Ŀ¼

	//��Ŀ¼�Ѵ��ڣ�������
	if (PathExists(strFilePath) == TRUE){
		return true;
	 }

	CString pathname = strFilePath;

     if(pathname.Right(1) != "\\")

              pathname += "\\" ;

     int end = pathname.ReverseFind('\\');

     int pt = pathname.Find('\\');

     if (pathname[pt-1] == ':')

               pt = pathname.Find('\\', pt+1);

     CString path;

     while(pt != -1 && pt<=end)

     {

		path = pathname.Left(pt+1);

		_mkdir(path);

		pt = pathname.Find('\\', pt+1);

     }

	 if (PathExists(strFilePath) == TRUE){
		ret = true;
	 }

     return ret;
}

//Ŀ¼�Ƿ����
BOOL CBasicUse::PathExists(CString strFilePath){
	WIN32_FIND_DATA fd;
	BOOL ret = false;
    HANDLE hFind = FindFirstFile(strFilePath, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
		//Ŀ¼����
		ret = true;
    
    }
    FindClose(hFind);
	return ret;

}

//�ļ��Ƿ����
BOOL CBasicUse::FileExists(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName); 
}

//ɾ��ָ���ļ����������ļ�
BOOL CBasicUse::DeleteFilesInFolder(CString strFolder)
{
	BOOL res = FALSE;

	CFileFind ff;

	CString strPath = strFolder;	//
	
	if (strPath == ""){

		return FALSE;
	}

	if(strPath.Right(1) != "\\"){
		strPath += "\\";
	}

	strPath += "*.*";

	try{
		res = ff.FindFile(strPath);

		CString strFile = "";

		while(res){

			res = ff.FindNextFile();

			if (!ff.IsDots() && !ff.IsDirectory()){

				strFile = ff.GetFilePath();

				DeleteFile(strFile);
			}
		}

		res = TRUE;

	}catch(CFileException e)
	{
		//CString error;

		//TCHAR szCause[255];
		//error.Format("���ļ�����!\r\n������Ϣ��%s",e.GetErrorMessage(szCause, 255, 0));
	
		res = FALSE;
	}
	
	return res;
}

//jpg�ļ�����ת����.bmp��
CString CBasicUse::JPGToBMPString(CString strJPGFileName)
{
	CString retStr = "";

	retStr = strJPGFileName;

	retStr.MakeLower();

	int intLen = 0;

	intLen = retStr.Find(".jpg",0);

	if (intLen <= 0){

		return "";
	}

	retStr = retStr.Mid(0,intLen);

	retStr = retStr + ".bmp";

	retStr.Format("%s",retStr);

	return retStr;
}

//bmp�ļ�����ת����.jpg��
CString CBasicUse::BMPToJPGString(CString strBMPFileName)
{
	CString retStr = "";

	retStr = strBMPFileName;

	retStr.MakeLower();

	int intLen = 0;

	intLen = retStr.Find(".bmp",0);

	if (intLen <= 0){

		return "";
	}

	retStr = retStr.Mid(0,intLen);

	retStr = retStr + ".jpg";

	retStr.Format("%s",retStr);

	return retStr;
}

//����ָ��ʱ�䴮ȡ�����ڴ�yyyymmdd
CString CBasicUse::GetDate_Connect(CString strDatetime)
{

	CString strDate = "";

	COleDateTime tDatetime;		//�ô����CTime����

	tDatetime.ParseDateTime(strDatetime,0);

	int intYear = tDatetime.GetYear();

	int	intMonth = tDatetime.GetMonth();

	int intDay = tDatetime.GetDay();

	CString str = "";

	str.Format("%d",intYear);

	strDate = strDate + str;

	str.Format("%d",intMonth);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intDay);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	strDate.Format("%s",strDate);
	
	return strDate;
}

//����ָ��ʱ�䴮ȡ������ʱ�䴮yyyymmddhhmmss
CString CBasicUse::GetDateTime_Connect(CString strDatetime)
{

	CString strDate = "";

	COleDateTime tDatetime;		//�ô����CTime����

	tDatetime.ParseDateTime(strDatetime,0);

	int intYear = tDatetime.GetYear();

	int	intMonth = tDatetime.GetMonth();

	int intDay = tDatetime.GetDay();

	int intHour = tDatetime.GetHour();

	int intMinute = tDatetime.GetMinute();

	int intSecond = tDatetime.GetSecond();

	CString str = "";

	str.Format("%d",intYear);

	strDate = strDate + str;

	str.Format("%d",intMonth);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intDay);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	//h,m,s
	str.Format("%d",intHour);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intMinute);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intSecond);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	//
	strDate.Format("%s",strDate);
	
	return strDate;
}

/*��̬����dll
��������strDllName����̬��ȫ��
*/
HINSTANCE CBasicUse::LoadDll(CString strDllName){

	HINSTANCE dllinstance; 
	
	dllinstance=::LoadLibrary(strDllName); 

	//����޷��򿪣����˳�ϵͳ
	if(dllinstance==NULL)   
	{
		return NULL;
	}
	
	//FreeLibrary(hDLL);	//ж��.dll�ļ�

	return dllinstance;

}


//////////////ȫ�ֺ���/////////////////////////////////
//������0������"1" -> "01"///////////
CString GetMillFormat(int nMillNum,int nWidth)
{
	CString sReturn;
	sReturn.Format("%d",nMillNum);
	int nLen=strlen(sReturn);
	while (nLen<nWidth)
	{
		sReturn.Insert(0,"0");
		nLen=strlen(sReturn);
	}
	return sReturn ;
}

/////�õ�ʱ��ǰ׺��yyyymmddhhmmss_hm�� yyyy-mm-dd hh:mm:ss hm
CString GetFileNameRoot(char *sSource)
{
	CString sReturn ;
	COleDateTime t_timeA;//COleDateTime::GetCurrentTime();
	SYSTEMTIME  t_timeB;
	::GetSystemTime(&t_timeB);
	CString sT;
	sT=t_timeA.GetCurrentTime().Format(_T("%Y%m%d %H%M%S"))+"_"+GetMillFormat((int)t_timeB.wMilliseconds,3);
	sReturn.Format("%s",sT);
	
	CString sDBFlag;	//�õ����ڱ�׼��ʽ��
	sDBFlag=t_timeA.GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")) + ":" + GetMillFormat((int)t_timeB.wMilliseconds,3);
	wsprintf(sSource,"%s",sDBFlag);
	
	return sReturn ;		//�õ���������,���:�ĺ���Ǻ���
}

CString GetFileNameRoot()
{
	CString sReturn ;
	COleDateTime t_timeA;//COleDateTime::GetCurrentTime();
	SYSTEMTIME  t_timeB;
	::GetSystemTime(&t_timeB);
	CString sT;
	sT=t_timeA.GetCurrentTime().Format(_T("%Y%m%d %H%M%S"))+"_"+GetMillFormat((int)t_timeB.wMilliseconds,3);
	sReturn.Format("%s",sT);
	
	return sReturn ;
} 