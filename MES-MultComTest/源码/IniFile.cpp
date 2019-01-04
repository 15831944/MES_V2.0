///////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////

//#include <Afx.h>
#include "stdafx.h"
#include <AfxColl.h>


#include "IniFile.H"

CIniFile::CIniFile(CString m_FName)
{
	m_Name = m_FName;
}

CString CIniFile::ReadString(CString m_Sec, CString m_Ident, CString m_Def)
{
	char Buffer[2048];

	GetPrivateProfileString(m_Sec, m_Ident,m_Def, Buffer, sizeof(Buffer), m_Name);
	return Buffer;
}

BOOL CIniFile::WriteString(CString m_Sec, CString m_Ident, CString m_Val)
{
	return WritePrivateProfileString(m_Sec, m_Ident, m_Val, m_Name);
}

BOOL CIniFile::ReadSections(CStringArray& m_Secs)
{
	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 16385);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	m_Secs.RemoveAll();
	
	if (GetPrivateProfileString(NULL, NULL, NULL, (char*) pvData, 16384, m_Name))
	{
        char *P = (char*) pvData;
        while (*P != 0)
		{
			m_Secs.Add(P);
			P += strlen(P) + 1;
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return m_Secs.GetSize() > 0;
}

BOOL CIniFile::ReadSection(CString m_Sec, CStringArray& m_Secs)
{
	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 16385);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	m_Secs.RemoveAll();
	
	if (GetPrivateProfileString(m_Sec, NULL, NULL, (char*) pvData, 16384, m_Name))
	{
        char *P = (char*) pvData;
        while (*P != 0)
		{
			m_Secs.Add(P);
			P += strlen(P) + 1;
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return m_Secs.GetSize() > 0;
}

CString CIniFile::GetParaName()
{
TCHAR       chSectionNames[2048]={0};       //���н�����ɵ��ַ�����
char * pSectionName; //�����ҵ���ĳ�������ַ������׵�ַ
int i;               //iָ������chSectionNames��ĳ��λ�ã���0��ʼ��˳�����
int j=0;             //j����������һ�������ַ������׵�ַ����ڵ�ǰi��λ��ƫ����
int count=0;         //ͳ�ƽڵĸ���

::GetPrivateProfileSectionNames(chSectionNames,2048,m_Name);  

for(i=0;i<2048;i++,j++)

{
      if(chSectionNames[0]=='\0')
       break;       //�����һ���ַ�����0����˵��ini��һ����Ҳû��
      if(chSectionNames[i]=='\0')
      {
       pSectionName=&chSectionNames[i-j]; //�ҵ�һ��0����˵��������ַ���ǰ������j��ƫ������
                                          //����һ���������׵�ַ   
       j=-1;                              //�ҵ�һ��������j��ֵҪ��ԭ����ͳ����һ��������ַ��ƫ����

                                          //����-1����Ϊ�����ַ��������һ���ַ�0����ֹ����������Ϊ������һ����

										  //�ڻ�ȡ������ʱ����Ի�ȡ�ý��м���ֵ��ǰ��������֪���ý�������Щ����
										  //AfxMessageBox(pSectionName);      //���ҵ�����ʾ����
       
       if(chSectionNames[i+1]==0)
       {
         break;      //���������ڵ��ַ�����0ʱ�������еĽ��������ҵ���ѭ����ֹ
       }
      }  

}

return pSectionName;

}


