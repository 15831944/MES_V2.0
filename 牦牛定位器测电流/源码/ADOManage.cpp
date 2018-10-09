#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "MFCP4ECurrentToolDlg.h"

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


//���������ݿ����Ӻ��ͷ���Դ����

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
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		return 0;
	}
	return 1;
}

//�����������ݿ�
bool ADOManage::ConntestdbSQL1()
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
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		return 0;
	}

	MessageBox(NULL, L"���ݿ����ӳɹ�", L"��ʾ��Ϣ", NULL);

	return 1;
}

//�����������ݿ�
bool ADOManage::ConntestdbSQL2()
{
	CoInitialize(NULL); //��ʼ��COM����           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//��ʼ��Connectionָ�� 
	try
	{
		/*�����ݿ⡰SQLServer����������Ҫ�������ݿ����� */
		CString m_ConnectStr;//�����ַ���

		//Ҫע�������ַ���������ģʽ������Ӧ��ѡ������ģʽ
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Seconddbname;
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		return 0;
	}
	return 1;
}


//�ر����ݿ�
bool ADOManage::CloseDB()
{
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

//�ͷ�ȫ�������ݿ���Դ
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


//�����Ƿ�ͨ�ú���

//�����ģ�溯�����ܽ�һ���÷�
_RecordsetPtr ADOManage::MOBAN(CString str1, CString str2)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t affectline, strnull;//�����������ȡӰ�������õ�
	CString strSql;//������������ִ������õ�

	//�������
	strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + str1 + _T("] =") + _T("'") + str2 + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//Ϊ��ʹ���û����κ�����
	if (m_pRecordSet->adoEOF)
	{
		return m_pRecordSet;
	}

	//���Դ����ݼ���������Ȼ��ŵ�ĳ������
	m_pRecordSet->GetCollect("CH_PrintTime");

	//������ж�NULLҲҪ��_variant_t����
	strnull = m_pRecordSet->GetCollect("CH_PrintTime");

	//�����Ϳ����ж�ȡ�����������Ƿ�ΪNULL
	if (strnull.vt == VT_NULL)
	{
		return m_pRecordSet;
	}

	//�������
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan])values('") + str1 + _T("', '") + str2 + _T("')");//����ִ�е�SQL���

	//�������ļ�¼����Ψһֵ�����Ǿ͵���try��catch����֤����ը������ͨ��affectline��ʹ�����ǿ��Խ����������������������update
	try{
		m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		e.Description();
		//AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}


	//�������
	//�����Ǽ������ʧ�ܺ�Ž���
	if (affectline.vt == 0)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',IMEI2='") + str1 + _T("',SECTIONNO1='") + str2 + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//ֱ��ִ�����
	}

	return m_pRecordSet;//���涼û����ͷ���2����ɹ�

}


//���Ժ󽫽���Լ�RID�Ȳ��뵽���ݿ�,���Ѳ��Թ���ʧ�ܵĸ��½��Ϊ1
void ADOManage::TestResultInsertSql(CString ECIP, CString Rid, CString StandbyFiveCurrent, CString StandbyAverage, CString SleepFiveCurrent1, CString SleepAverage1, CString SleepFiveCurrent2, CString SleepAverage2, CString TestResult)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;
	int TResult;

	//�жϸ�Rid�Ƿ����ͨ��
	strSql = _T("SELECT *FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + Rid + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//Ϊ��ʹ���û����κ�����
	if (m_pRecordSet->adoEOF)
	{
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("](ECIP,Rid,StandbyFiveCurrent,StandbyAverage,SleepFiveCurrent1,SleepAverage1,SleepFiveCurrent2,SleepAverage2,TestResult,TestTime,ReTestNum)values('") + ECIP + _T("','") + Rid + _T("','")
			+ StandbyFiveCurrent + _T("', '") + StandbyAverage + _T("', '") + SleepFiveCurrent1 + _T("', '") + SleepAverage1 + _T("','") + SleepFiveCurrent2 + _T("', '") + SleepAverage2 + _T("','") + TestResult + _T("','") + GetTime() + _T("','0')");//����ִ�е�SQL���
		try
		{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
			e.Description();
			//AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		}
	}
	//���������
	else if (!m_pRecordSet->adoEOF) {
		//�ж�֮ǰ�Ĳ��Խ���Ƿ�Ϊ0
		TResult = m_pRecordSet->GetCollect("TestResult");
		if (TResult == 0)
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET TestResult = '") + TestResult + _T("',ECIP ='") + ECIP + _T("',StandbyFiveCurrent = '") + StandbyFiveCurrent + _T("',StandbyAverage = '") + StandbyAverage + _T("',SleepFiveCurrent1 ='") + SleepFiveCurrent1 + _T("',SleepAverage1='") + SleepAverage1 + _T("',SleepFiveCurrent2 ='") + SleepFiveCurrent2 + _T("',SleepAverage2 ='") + SleepAverage2 + _T("' Where Rid ='") + Rid + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		}
		else if (TResult == 1) 
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET ECIP ='") + ECIP + _T("',StandbyFiveCurrent = '") + StandbyFiveCurrent + _T("',StandbyAverage = '") + StandbyAverage + _T("',SleepFiveCurrent1 ='") + SleepFiveCurrent1 + _T("',SleepAverage1='") + SleepAverage1 + _T("',SleepFiveCurrent2 ='") + SleepFiveCurrent2 + _T("',SleepAverage2 ='") + SleepAverage2 + _T("',ReTestNum = ReTestNum + 1,ReTestTime ='") + GetTime() + _T("'Where Rid ='") + Rid + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		}
	}
}

//����RID�������ݣ��ҵ����Ҳ��Խ��Ϊͨ�����򷵻�0��ǰ������ʾ�Ѳ���ͨ�����鲻�����ݻ��߲鵽���ǲ��Խ��Ϊ��ͨ�����򷵻�1��ǰ�����¼�������
int ADOManage::CheckTestResultByRid(CString Rid)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;
	int TResult;

	//����Rid�������ݿ�
	strSql = _T("SELECT *FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + Rid + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//Ϊ��ʹ���û����κ�����
	if (m_pRecordSet->adoEOF)
	{
		return 1;
	}
	//���������
	else if (!m_pRecordSet->adoEOF) {
		//�ж�֮ǰ�Ĳ��Խ���Ƿ�Ϊ0
		TResult = m_pRecordSet->GetCollect("TestResult");
		if (TResult == 0)
		{
			return 1;
		}
		else if (TResult == 1)
		{
			return 0;
		}
	}
}

//���������Ϣ
void ADOManage::ConfigInsertSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//���ݻ������Ʋ������ݿ�
	strSql = _T("SELECT *FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ModelName]='") + ModelName + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	//Ϊ��ʹ���û����κ�����
	if (m_pRecordSet->adoEOF)
	{
		strSql.Format(_T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("](ModelName,StandbyUp,StandbyDown,SleepUP,SleepDown,TestCommand,TestCommandReply,RidCommand,RidCommandReply,StandbyCommand,StandbyCommandReply,SleepCommand,SleepCommandReply,Count,ReadTime,WriteTime)values('")
			+ ModelName + _T("', %g,%g,%g,%g,'") + TestCommand + _T("','") + TestCommandReply + _T("','") + RidCommand + _T("','") + RidCommandReply + _T("','") + StandbyCommand + _T("','") + StandbyCommandReply + _T("','") + SleepCommand + _T("','") + SleepCommandReply + _T("',%d,%d,%d)"), StandbyUp, StandbyDown, SleepUP, SleepDown, Count, ReadTime, WriteTime);
		try
		{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
			MessageBox(NULL, L"��ӳɹ�", L"��ʾ��Ϣ", NULL);
		}
		catch (_com_error &e)
		{
			e.Description();
			//AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
		}
	}
	else {
		MessageBox(NULL, L"�û����Ѵ���", L"��ʾ��Ϣ", NULL);
	}
}

//���ݻ��Ͳ��Ҹû�������
_RecordsetPtr ADOManage::CheckConfigByModelNameSql(CString ModelName)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ModelName] =") + _T("'") + ModelName + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	return m_pRecordSet;
}

//�������л���
_RecordsetPtr ADOManage::CheckAllInConfigSql()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [ModelName] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("]");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//����������Ϣ
void ADOManage::ConfigUpdataSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime) 
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));//��ʼ��Recordsetָ��
	//����
	_variant_t Affectline;
	CString strSql;

	strSql.Format(_T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] SET StandbyUp = %g,StandbyDown =%g,SleepUP = %g,SleepDown = %g,TestCommand ='") + TestCommand + _T("',TestCommandReply = '") + TestCommandReply + _T("',RidCommand = '") + RidCommand
		+ _T("',RidCommandReply ='") + RidCommandReply + _T("',StandbyCommand = '") + StandbyCommand + _T("',StandbyCommandReply='") + StandbyCommandReply + _T("',SleepCommand='") + SleepCommand + _T("',SleepCommandReply='") + SleepCommandReply + _T("',Count=%d,ReadTime=%d,WriteTime=%d WHERE ModelName='") + ModelName + _T("'"), StandbyUp, StandbyDown, SleepUP, SleepDown, Count, ReadTime, WriteTime);
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	if (Affectline.bVal == 1)
	{
		MessageBox(NULL, L"�������óɹ�", L"��ʾ��Ϣ", NULL);
	}
	if (Affectline.bVal == 0)
	{
		MessageBox(NULL, L"��������ʧ�ܣ��������ݿ���Ϣ", L"��ʾ��Ϣ", NULL);
	}

}

//���ݻ���ɾ����������
void ADOManage::DeleteConfigByModelNameSql(CString ModelName) 
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));//��ʼ��Recordsetָ��
	//����
	_variant_t Affectline;
	CString strSql;

	strSql = _T("DELETE FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ModelName]= '") + ModelName + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
}

/*������ͨ�ú���*/
//������ݿ�ĳ�ű�
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

//��ȡ��ǰʱ�亯��
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}