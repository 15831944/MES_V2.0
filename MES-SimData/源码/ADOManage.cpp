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


//SIM�����سɹ�����뺯��
void ADOManage::SimDataOkInsertSql(CString SDIP, CString RID, CString IMEI, CString CID, CString ICCID, CString SDOperator)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;


	//���ж����������ǲ��Ƿ�����
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("' AND SDRESULT = 2");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//�����õ��Ǿʹ������������Ƿ����ģ��ǽ������Ƿ������ݸ��£�ͬʱ������ҲҪ����
	if (!m_pRecordSet->adoEOF)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet]") + _T("SET IMEI3 = '") + CID + _T("',IMEI4='") + ICCID + _T("'where[IMEI1] = '") + IMEI + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����

		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET SDRESULT = '1',SDIP = '") + SDIP + _T("',CID='") + CID+_T("',RID='") + RID + _T("',ICCID='") + ICCID + _T("',SDOperator ='") + SDOperator + _T("',ReSDTime ='") + GetTime() + _T("',ReSDCount=ReSDCount+1 where[RID] = '") + RID + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		return ;
	}

	strSql = _T("SELECT [IMEI1] FROM [") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet] WHERE [IMEI1] = '") + IMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	if (m_pRecordSet->adoEOF)
	{
		//������Ƿ����ͽ����ݲ�����У�ͬʱ������ҲҪ��������
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet](SN, IMEI1, IMEI3, IMEI4, TestTime, _MASK_FROM_V2)values(NULL,'")
			+ IMEI + _T("', '") + CID + _T("', '") + ICCID + _T("', '") + GetTime() + _T("', NULL)");//����ִ�е�SQL���
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}
	else if (!m_pRecordSet->adoEOF)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet]") + _T("SET IMEI3 = '") + CID + _T("',IMEI4='") + ICCID + _T("'where[IMEI1] = '") + IMEI + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}


	//������Ƿ����ͽ����ݲ������
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([SDIP],[RID],[IMEI], [CID], [ICCID], [SDOperator], [SDTime], [SDRESULT],[ReSDCount])values('")
		+ SDIP + _T("', '") + RID + _T("', '") + IMEI + _T("', '") + CID + _T("', '") + ICCID + _T("', '") + SDOperator + _T("', '")  + GetTime() + _T("', 1,0)");//����ִ�е�SQL���
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
	}

	//����RID�Ƿ���ڣ�Ȼ���жϱ�־λ�Ƿ�Ϊ0��������ڱ�ʾ���ع�
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("' AND SDRESULT = 0");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��õ��͸���,Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ�������������ݵ�����ʧ�ܣ����ڻ����������سɹ���Ҫ��������
	if (!m_pRecordSet->adoEOF)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET SDRESULT = 1,SDIP = '") + SDIP + _T("',CID='") + CID + _T("',ICCID='") + ICCID + _T("',SDOperator ='") + SDOperator + _T("',SDTime ='") + GetTime() + _T("' where[IMEI] = '") + IMEI + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����

	}

}

//SIM������ʧ�ܺ���뺯��
void ADOManage::SimDataErrorInsertSql(CString SDIP, CString RID, CString IMEI, CString SDOperator)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//����RID�Ƿ����
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	if (m_pRecordSet->adoEOF)
	{
		//�����ݲ������
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([SDIP],[RID],[IMEI],[SDOperator], [SDTime], [SDRESULT],[ReSDCount])values('")
			+ SDIP + _T("', '") + RID + _T("', '") + IMEI + _T("', '") + SDOperator + _T("', '") + GetTime() + _T("', 0,0)");//����ִ�е�SQL���
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}
}

//�����һ����λ�Ƿ�ͨ������
int ADOManage::SimDataLastStationSql(CString IMEI)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t a;
	CString strSql;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [IMEI],[CoupleResult] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_TestResult] WHERE [IMEI] ='") + IMEI + _T("'AND CoupleResult = 1");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//�鲻���ͷ���0��ʾʧ��
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//����û����ͷ���1����ɹ�
}

//��ѯ�����Ƿ���ڲ������سɹ�����
int ADOManage::SimDataIsExitSql(CString RID,CString IMEI)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t a;
	CString strSql;
	//����RID�Ƿ���ڣ����ھͷ���0��ʾ���ع�
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("' AND SDRESULT = 1");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��õ��ͷ���0��ʾ�Ѿ����ع�
	if (!m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//����û����ͷ���1�����Ҳ���
}

//��ѯ���Ӻ��Ƿ��Ѿ����ع�����
int ADOManage::SimDataNoIsExitSql(CString CID)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t a;
	CString strSql;

	//����CID�Ƿ���ڣ����ھͷ���0��ʾ���ع�
	strSql = _T("SELECT [CID] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [CID] ='") + CID + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��õ��ͷ���0��ʾ�Ѿ����ع�
	if (!m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//����û����ͷ���1�����Ҳ���
}

//����λ������
int ADOManage::SimDataReSql(CString RID, CString IMEI,CString strOKpath)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t a;
	CString strSql,strCID;
	int flag;
	bool moveflag;
	//����RID�Ƿ����
	strSql = _T("SELECT [RID],[IMEI],[CID],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//��õ����Ƚ�����ת�Ƶ�SIM�������ؼ�¼��
	if (!m_pRecordSet->adoEOF)
	{
		flag = m_pRecordSet->GetCollect("SDRESULT");
		if (flag == 1)
		{
			strCID = m_pRecordSet->GetCollect("CID");

			//������Ҫд�ƶ��ļ��еĴ���
			
			moveflag = MoveFile(strOKpath + strCID + L"\\", strOKpath.Left(strOKpath.GetLength() - 3) + L"\\" + strCID + L"\\");
			if (moveflag == FALSE)
			{
				MoveFile(strOKpath.Left(strOKpath.GetLength() - 3) + L"\\" + strCID + L"\\",strOKpath + strCID + L"\\");
				return 4;
			}
			//Ȼ���ٽ����ݲ��뵽��һ�ű�
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[Gps_ManuReSimDataParam] select [SDIP], [RID], [IMEI], [CID], [ICCID], [SDOperator], [SDTime], [SDRESULT], [ReSDTime], [ReSDCount] from[") + m_Firstdbname + _T("].[dbo].[Gps_ManuSimDataParam] where[RID] = '") + RID + _T("' AND[IMEI] = '") + IMEI + _T("'");
			m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����


			//Ȼ�����һ��ԭ����
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET SDRESULT = '2',SDIP = NULL,CID=NULL,ICCID=NULL,SDOperator =NULL where[IMEI] = '") + IMEI + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet]") + _T("SET IMEI3 = NULL, IMEI4 = NULL where[IMEI1] = '") + IMEI + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

			return 1;
		}
		else if (flag == 2)//����2�ͱ�ʾ���Ѿ������ˣ��������뷵��
		{
			return 2;
		}
		else if (flag == 0)//����0��ʾ��������û���سɹ�����Ҳ���뷵��
		{
			return 3;
		}

	}

	return 0;//����������ͷ���0������̨����������û������
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