#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>

//ȫ�ֱ�������������г�ʼ��

ADOManage::ADOManage() :
m_DataSource(_T("")),
m_Port(_T("")),
m_UserName(_T("")),
m_Password(_T("")),
m_Firstdbname(_T("")),
m_Seconddbname(_T("")),
m_Firstformname(_T("")),
m_Secondformname(_T(""))
{
}


ADOManage::~ADOManage()
{
}

//�������ݿ�
bool ADOManage::ConnSQL()
{
	CoInitialize(NULL); //��ʼ��COM����           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//��ʼ��Connectionָ�� 
	GetDBINFO();//��ȡ���ݿ���Ϣ
	try
	{
		/*�����ݿ⡰SQLServer����������Ҫ�������ݿ����� */
		CString m_ConnectStr;//�����ַ���

		//Ҫע�������ַ���������ģʽ������Ӧ��ѡ������ģʽ
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Firstdbname;
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		::MessageBox(NULL, _T("���ݿ����Ӵ���"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
		return 0;
	}
	return 1;
}

//�������ݿ�
bool ADOManage::ConndbSQL()
{
	CoInitialize(NULL); //��ʼ��COM����           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//��ʼ��Connectionָ�� 
	try
	{
		/*�����ݿ⡰SQLServer����������Ҫ�������ݿ����� */
		CString m_ConnectStr;//�����ַ���

		//Ҫע�������ַ���������ģʽ������Ӧ��ѡ������ģʽ
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Firstdbname;
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		::MessageBox(NULL, _T("���ݿ����Ӵ���"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
		return 0;
	}
	return 1;
}

//�ر����ݿ�����
bool ADOManage::CloseAll()
{
	if (m_pRecordSet->GetState() == adStateOpen)
	{
		m_pRecordSet->Close();
	}
	if (m_pRecordSet)
	{
		m_pRecordSet.Release();
		m_pRecordSet = NULL;
	}
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return 1;
}

//��ȡ���ݿ���Ϣ
void ADOManage::GetDBINFO()
{
	CString str;

	GetPrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_DataSource = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Port"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Port = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("UserName"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_UserName = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Password = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Firstdbname = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Firstformname = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Seconddbname = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Secondformname = str;
	str.ReleaseBuffer();
}


_RecordsetPtr ADOManage::GetRst()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ�� 
	CString strSql;
	strSql = "Truncate table [testLD].[dbo].[testld]";
	//strSql = "select * from BLEStatus";//����ִ�е�SQL���

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//����ѯ���ݵ���m_pRecordset��������
	return m_pRecordSet;
}

//���������Ŷ�Ѱ��IMEI
int ADOManage::CpImeiByNo(CString noname,CString no)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	//_variant_t a;
	CString strSql;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [")+noname+_T("] =") + _T("'") + no + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//�������ڴ˺Ŷ�
	if (m_pRecordSet->adoEOF)
	{
		strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + no + _T("'");
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

		if (m_pRecordSet->adoEOF)
		{
			return 1;
		}
		else
		{
			return 3;//����3��������Ǹ�IMEI��
		}
		return 0;
	}


	return 2;//���涼û����ͷ���2����ɹ�

}

//�ж�IMEI���Ƿ����
int ADOManage::JudgeImei(CString imei)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t a;
	CString strSql;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//���涼û����ͷ���2����ɹ�
}

//����IMEIѰ�Ҳʺ�ʱ��
int ADOManage::CpCaiheByImei(CString imei)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	
	//����
	_variant_t a;
	CString strSql;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [IMEI],[CH_PrintTime] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	a = m_pRecordSet->GetCollect("CH_PrintTime");

	//����1�������ڲʺ�ʱ��
	if (a.vt == VT_NULL)
	{
		return 1;
	}

	return 2;//���涼û����ͷ���2����ɹ�
}

//������ȷ���ݵ�����
int ADOManage::InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//�����ݲ������
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2], [SECTIONNO1], [SECTIONNO2], [CPRESULT], [CPTIME], [CPTYPE],[CPERROR])values('")
		+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + no1 + _T("', '") + no2 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + notype + _T("', '") +errorreason +_T("')");//����ִ�е�SQL���
	try{
	m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{}

	//����Աȳɹ��Ǿ͸���һ�����ݣ�Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
	if (result=="1"&&Affectline.vt == 0)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',IMEI2='") + imei2 + _T("',SECTIONNO1='") + no1 + _T("',SECTIONNO2 ='") + no2 + _T("',CPTYPE ='") + notype+_T("', RECPTIME='") + GetTime() + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}

	return AffectLine;
}

//����������ݵ�����
int ADOManage::InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//�����ݲ������
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2], [SECTIONNO1], [SECTIONNO2], [CPRESULT], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR])values('")
		+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + no1 + _T("', '") + no2 + _T("', '") + result + _T("', '") + GetTime() + _T("', '")+L"1" + _T("', '") + notype + _T("', '") + errorreason + _T("')");//����ִ�е�SQL���
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
	}

	//����Ա���ʧ�ܵģ��Ǿ͸���һ��ʧ�ܴ���
	if (result == "0"&&Affectline.vt == 0)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'��") + errorreason + _T("',IMEI2='") + imei2 + _T("',SECTIONNO1='") + no1 + _T("',SECTIONNO2 ='") + no2 + _T("',CPTYPE ='") + notype + _T("', RECPTIME='") + GetTime() + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}

	return AffectLine;
}


//��ȡ��������
_RecordsetPtr ADOManage::GetOrderNumber()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam]");
	//CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[orderld]");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//���ݶ����Ż�ȡIMEI��
_RecordsetPtr ADOManage::GetIMEIByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//�ж϶������Ƿ����
_RecordsetPtr ADOManage::JudgeOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	return m_pRecordSet;

}


//��ȡ��ǰʱ�亯��
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}