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
bool ADOManage::ConntestdbSQL()
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


//������������ǳ�ʼ������ģ�����õĺ���
//��ȡ��������
_RecordsetPtr ADOManage::GetZhiDan()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] order by ZhiDan");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//���ݶ����Ż�ȡIMEI�ź��ص�ģ��
_RecordsetPtr ADOManage::GetIMEIByZhiDan(CString strzhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//��ȡIMEI������
int ADOManage::GetImeicount(CString strzhidan, CString imeistart, CString imeiend)
{
	int imeicount;

	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	unsigned long long b = 0;
	b = _atoi64(CStringA(imeiend)) + 1;
	imeiend = _ui64toa(b, CT2A(imeiend), 10);
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT count(*) FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'") + _T("AND IMEI >=") + _T("'") + imeistart + _T("'") + _T("AND IMEI <=") + _T("'") + imeiend + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	imeicount = m_pRecordSet->GetCollect("");

	return imeicount;
}

//��ȡ��ǰ�ص��imei��
CString ADOManage::GetIMEICurrent(CString strzhidan)
{
	_variant_t a;
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [LDIMEICurrent] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	a = m_pRecordSet->GetCollect("LDIMEICurrent");

	if (a.vt == VT_NULL)
	{
		return L"";
	}

	return m_pRecordSet->GetCollect("LDIMEICurrent");
}

//�ж϶������Ƿ����
_RecordsetPtr ADOManage::JudgeZhiDan(CString strzhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	return m_pRecordSet;
}


//�ص�ϵͳģ�����õĺ���
//�����ص�������ݵ��ص����ݱ�
_RecordsetPtr ADOManage::AddNew(CString strpicp, CString strzhidan, CString strimei)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("](PCIP, InputTime, ZhiDan,IMEI)values('") + strpicp + _T("', '") + GetTime() + _T("', '") + strzhidan + _T("', '") + strimei + _T("')");

	//IMEI�Ŵ���Ψһֵ��һ����˵��������Ѿ������˷������ظ����жϣ����������һ�����գ�����������ظ���IMEI�ͱ��쳣
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//�����ص�ĸ��²���
_RecordsetPtr ADOManage::UpdateReld(CString strimei)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET ReLdImeiNum = ReLdImeiNum+1,ReLdImeiTime='") + GetTime() + _T("' where [IMEI] = '") + strimei + _T("'");

	//���ִ�д���ͻᱨ�쳣ԭ��
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}

	return m_pRecordSet;
}

//�ж�IMEI�Ƿ����
int ADOManage::JudgeIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [IMEI] =") + _T("'") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��������������Ϊ�գ���m_pRecordSet->adoEOF���ص���ture����ʱ��������0�����IMEI�����ڣ����򷵻�1����IMEI����
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//�洢��ǰ�ص��imei��
_RecordsetPtr ADOManage::UpdateIMEICurrent(CString strzhidan, CString imeicurrent)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	_variant_t Affectline;

	//��IMEI��+1
	unsigned long long imeiint = 0;
	imeiint = _atoi64(CStringA(imeicurrent)) + 1;
	imeicurrent = _ui64toa(imeiint, CT2A(imeicurrent), 10);

	//����һ�����һ��ɨ���IMEI�ţ�һ����˵�������ʧ�ܣ�Ҫ��ʧ���ˣ��ǿ϶��Ƕ����ű��ɵ���
	CString strSql = _T("UPDATE[") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("]") + _T("SET LDIMEICurrent = '") + imeicurrent + _T("' where ZhiDan = '") + strzhidan + _T("'");

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*��ӡ���쳣ԭ��*/
	}
	return m_pRecordSet;
}


//�����������ݿ��ѯģ�����õĺ���
//����Ӧ�Ķ��������Ѿ������IMEIչʾ����
_RecordsetPtr ADOManage::ShowInsertImeiByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [IMEI],[InputTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//����Ӧ�Ķ��������Ѿ��ظ���IMEIչʾ����
_RecordsetPtr ADOManage::ShowRepeatImeiByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [IMEI],[ReLdImeiNum],[ReLdImeiTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'") + _T("AND [ReLdImeiNum]>0");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//����Ӧ�Ķ�������δ�����IMEIչʾ����
_RecordsetPtr ADOManage::ShowUnImeiByOrderNumber(CString ordernumber, CString imeistart, CString imeiend)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	CString strSql1, strSql2, strSql3, imeitemp;

	//�����һ����ʱ��
	strSql3 = _T("truncate table[testLD].[dbo].[Gps_ManuUnLdParam]");
	m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//ֱ��ִ�����

	//Ȼ�����ݲ��뵽һ����ʱ����
	unsigned long long a, b;
	a = _atoi64(CStringA(imeistart));
	b = _atoi64(CStringA(imeiend));
	for (; a <= b; a++)
	{
		imeistart = _ui64toa(a, CT2A(imeistart), 10);
		imeitemp = CreateIMEI15(imeistart);
		strSql1 = _T("insert into[testLD].[dbo].[Gps_ManuUnLdParam](UNIMEI)values('") + imeitemp + _T("')");
		m_pConnection->Execute(_bstr_t(strSql1), NULL, adCmdText);
	}

	//���Ż�ȡ��Χ��û�е�����
	strSql2 = _T("select[UNIMEI] FROM[testLD].[dbo].[Gps_ManuUnLdParam] where[UNIMEI] not in(select[IMEI] FROM[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] where[zhidan] = '") + ordernumber + _T("')");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql2), NULL, adCmdText);//ֱ��ִ�����

	//��������ʱ��
	m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//ֱ��ִ�����

	return m_pRecordSet;
}


//������ͨ�ú���
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

//������IMEIУ��λ�õ�
CString ADOManage::CreateIMEI15(CString imei)
{
	long long a = 0;
	int j = 0;
	char imeiChar[14];
	strncpy(imeiChar, CStringA(imei), sizeof(imeiChar));
	//	std::string sz2 = CT2A(imei.GetBuffer());

	int resultInt = 0;
	for (int i = 0; i < 14; i++)
	{
		int a = (int)imeiChar[i] - '0';
		i++;
		int temp = ((int)imeiChar[i] - '0') * 2;
		int b = temp < 10 ? temp : temp - 9;
		resultInt += a + b;
	}
	resultInt %= 10;
	GetLastError();
	resultInt = (resultInt == 0 ? 0 : 10 - resultInt);
	CString resultStr, abc;
	resultStr.Format(_T("%d"), resultInt);
	abc = imei + resultStr;
	imei.ReleaseBuffer();
	return abc;
}