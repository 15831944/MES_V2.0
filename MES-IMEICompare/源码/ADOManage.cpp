#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>
#include "MFCP2CPDlg.h"

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
int ADOManage::CpImeiByNo(CString Syllablesqlstr, CString no, CString strzhidan)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	//_variant_t a;
	CString strSql;

	//�����û�ѡ����ֶ�ȥ��IMEI��ת������
	strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuPrintParam] WHERE ZhiDan ='")+strzhidan+_T("' AND( ");
	strSql += Syllablesqlstr+L")";
	strSql.Replace(L"InputNumber",no);//Ȼ�������Ԥ���ַ����滻���ɾ��������

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	////����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	//strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + noname + _T("] =") + _T("'") + no + _T("'") + _T("AND ZhiDan='") + strzhidan + _T("'");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//�������ڴ˺Ŷ�
	if (m_pRecordSet->adoEOF)
	{
		//strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + no + _T("'");
		//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

		//if (m_pRecordSet->adoEOF)
		//{
		//	return 1;
		//}
		//else
		//{
		//	return 3;//����3��������Ǹ�IMEI��
		//}
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

	return 1;
}

//�ж��Ƶ����Ƿ�Ե���
int ADOManage::JudgeZhidan(CString imei,CString Zhidan)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t a;
	CString strSql;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [ZhiDan],[IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] ='") + imei + _T("' AND [ZhiDan]='") + Zhidan+_T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����


	//�Ҳ����Ǿͷ���1
	if (m_pRecordSet->adoEOF)
	{
		return 1;
	}

	return 0;//����0�ʹ���Ե���
}


//����IMEIѰ�Ҳʺ�ʱ��
int ADOManage::CpCaiheByImei(CString imei, CString ZhiDan)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	
	//����
	_variant_t a;
	CString aStr;
	CString strSql,strzhidan;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [ZhiDan],[IMEI],[CH_PrintTime] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	a = m_pRecordSet->GetCollect("CH_PrintTime");

	//����1�������ڲʺ�ʱ��

	if (a.vt == VT_NULL || a.vt == VT_EMPTY)
	{
		return 1;
	}

	aStr = (LPCSTR)_bstr_t(a);
	if (aStr == "")
	{
		return 1;
	}


	//����Ƶ��ŶԲ��ϣ��Ǿʹ����Ƶ��Ŵ���
	strzhidan = m_pRecordSet->GetCollect("ZhiDan");
	if (ZhiDan!=strzhidan)
	{
		return 3;
	}

	/*��ӡ���IMEI��SN��SIM��ICCID��MAC��Equipment��VIP��BAT,RFID*/
	/*�������IMEI3��SIM����IMEI4��ICCID����IMEI6��MAC����IMEI7��Equipment����IMEI8��VIP����IMEI9��BAT��,IMEI14(IMEI2)*/

	//�����ǲ�󶨵�
	//static map<int, CString>::iterator Syllableiter;
	map<int, CString>::iterator BindIter;
	_variant_t Bindtempvt;
	CString Bindtemp;
	strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet] WHERE [IMEI1] =") + _T("'") + imei + _T("'");
	//strSql = _T("SELECT [ZhiDan],[IMEI],[SIM],[VIP],[ICCID],[BAT],[MAC],[Equipment],[RFID] FROM [") + m_Firstdbname + _T("].[dbo].[Gps_ManuPrintParam] WHERE [IMEI] =") + _T("'") + imei + _T("'") + _T("AND ZhiDan='") + ZhiDan + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	if (m_pRecordSet->adoEOF)
	{
		return d_DataRelativeImeiCheck;
	}

	//����MAP�ĳ���������ѭ�������������Ӧ��ֵΪ""���Ǿʹ�����ȱ�ٰ󶨣���ʱ���ذ󶨵��ֶβ�����Ӧ�Ĵ���
	for (BindIter = CMFCP2CPDlg::BindMap.begin(); BindIter != CMFCP2CPDlg::BindMap.end(); BindIter++)
	{
		switch (BindIter->first)
		{
		case 0:
			//��ʺб�
			//Bindtemp = m_pRecordSet->GetCollect("SIM");
			//Bindtempvt = m_pRecordSet->GetCollect("SIM");
			//if (Bindtemp == "" || Bindtemp == " " || Bindtempvt.vt == VT_NULL)
			//{
			//	return d_SimBindCheck;
			//}

			//���������������ΪNULLֱ��תCString�ᱨ������Ҫ��ΪNULL��""���������
			Bindtempvt = m_pRecordSet->GetCollect("IMEI3");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_SimBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_SimBindCheck;
			}
			break;
		case 1:
			//Bindtemp = m_pRecordSet->GetCollect("VIP");
			//Bindtempvt = m_pRecordSet->GetCollect("VIP");
			//if (Bindtemp == "")
			//{
			//	return d_VipBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI8");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_VipBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_VipBindCheck;
			}
			break;
		case 2:
			//Bindtemp = m_pRecordSet->GetCollect("ICCID");
			//Bindtempvt = m_pRecordSet->GetCollect("ICCID");
			//if (Bindtemp == "")
			//{
			//	return d_IccidBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI4");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_IccidBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_IccidBindCheck;
			}
			break;
		case 3:
			//Bindtemp = m_pRecordSet->GetCollect("BAT");
			//Bindtempvt = m_pRecordSet->GetCollect("BAT");
			//if (Bindtemp == "")
			//{
			//	return d_BatBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI9");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_BatBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_BatBindCheck;
			}
			break;
		case 4:
			//Bindtemp = m_pRecordSet->GetCollect("MAC");
			//Bindtempvt = m_pRecordSet->GetCollect("MAC");
			//if (Bindtemp == "")
			//{
			//	return d_MacBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI6");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_MacBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_MacBindCheck;
			}
			break;
		case 5:
			//Bindtemp = m_pRecordSet->GetCollect("Equipment");
			//Bindtempvt = m_pRecordSet->GetCollect("Equipment");
			//if (Bindtemp == "")
			//{
			//	return d_EquipmentBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI7");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_EquipmentBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_EquipmentBindCheck;
			}
			break;
		case 6:
			//Bindtemp = m_pRecordSet->GetCollect("RFID");
			//Bindtempvt = m_pRecordSet->GetCollect("RFID");
			//if (Bindtemp == "")
			//{
			//	return d_RfidBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI13");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_RfidBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_RfidBindCheck;
			}
			break;
		case 7:

			Bindtempvt = m_pRecordSet->GetCollect("IMEI14");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_DataRelativeImeiBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_DataRelativeImeiBindCheck;
			}
			break;
		default:
			break;
		}

	}

	return 2;//���涼û����ͷ���2����ɹ�
}

//������ȷ���ݵ�����
int ADOManage::InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString ip, CString ScanType, CString errorreason, CString result, CString CHResult)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	_variant_t IMEIResultTempVT;
	CString IMEIResultTemp;

	//������ģʽ
	if (ScanType == "Single")
	{
		//�����ݲ������
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT], [CPTIME], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
			+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//����ִ�е�SQL���
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}

		//����ʧ���Ǿʹ���ԭ���������ˣ��Ǿ͸���һ�����ݣ�Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
		if (result == "1"&&Affectline.vt == 0)
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',ZhiDan = '") + zhidan + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CHResult='") + CHResult + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("', TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}
		}


		//����testresult���ű�
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '1'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
		try{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}

		return AffectLine;
	}
	//�����ģʽ
	else
	{
		if (ScanType == "MutiPC1")
		{
			//�����ݲ������
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT1], [CPTIME], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//����ִ�е�SQL���
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}

			//����ʧ���Ǿʹ���ԭ���������ˣ��Ǿ͸���һ�����ݣ�Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
			if (result == "1"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT1 = '1',ZhiDan = '") + zhidan + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CHResult='") + CHResult + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("', TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
				}
				catch (_com_error &e)
				{
				}
			}

			//�Ȳ�ѯ�Աȱ�����һ����־λ�Ƿ�Ϊ1��Ϊ1�˲��ܽ�������ı�־λ��Ϊ1

			try{
				strSql = _T("SELECT [IMEI1],[CPResult2] FROM [") + m_Firstdbname + _T("].[dbo].[Gps_ManuCpParam] WHERE [IMEI1] ='") + imei1 + _T("'");
				m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}

			//�Ҳ����Ǿͷ���1
			if (m_pRecordSet->adoEOF)
			{
				return 2;
			}
			else
			{
				IMEIResultTempVT = m_pRecordSet->GetCollect("CPResult2");
				if (IMEIResultTempVT.vt == VT_EMPTY || IMEIResultTempVT.vt == VT_NULL)
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
					}
					catch (_com_error &e)
					{
					}
					return 1;
				}
				IMEIResultTemp = (LPCSTR)_bstr_t(IMEIResultTempVT);
				if (IMEIResultTemp == "1")
				{
					//����testresult���ű�
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '1' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					return AffectLine;
				}
				else if (IMEIResultTemp == "0")
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
					}
					catch (_com_error &e)
					{
					}

					return AffectLine;
				}
			}
			return AffectLine;
		}
		else if (ScanType == "MutiPC2")
		{
			//�����ݲ������
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [SECTIONNO1],[SECTIONNO2], [CPRESULT2], [CPTIME], [CPTYPE],[CPERROR],[CHResult],[TesterID2])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//����ִ�е�SQL���
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			}
			catch (...)
			{
			}

			//����ʧ���Ǿʹ���ԭ���������ˣ��Ǿ͸���һ�����ݣ�Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
			if (result == "1"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT2 = '1',ZhiDan = '") + zhidan + _T("',SECTIONNO1='") + imei2 + _T("',SECTIONNO2='") + imei3 + _T("',CHResult='") + CHResult + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("', TesterID2='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
				}
				catch (...)
				{
				}
			}

			//�Ȳ�ѯ�Աȱ�����һ����־λ�Ƿ�Ϊ1��Ϊ1�˲��ܽ�������ı�־λ��Ϊ1

			try{
				strSql = _T("SELECT [IMEI1],[CPResult1] FROM [") + m_Firstdbname + _T("].[dbo].[Gps_ManuCpParam] WHERE [IMEI1] ='") + imei1 + _T("'");
				m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
			}
			catch (...)
			{
			}

			//�Ҳ����Ǿͷ���1
			if (m_pRecordSet->adoEOF)
			{
				return 2;
			}
			else
			{
				IMEIResultTempVT = m_pRecordSet->GetCollect("CPResult1");
				if (IMEIResultTempVT.vt == VT_EMPTY || IMEIResultTempVT.vt == VT_NULL)
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
					}
					catch (_com_error &e)
					{
					}
					return 1;
				}
				IMEIResultTemp = (LPCSTR)_bstr_t(IMEIResultTempVT);
				if (IMEIResultTemp == "1")
				{
					//����testresult���ű�
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '1' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
					}
					catch (_com_error &e)
					{
					}
					return AffectLine;
				}
				else if (IMEIResultTemp == "0")
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
					}
					catch (_com_error &e)
					{
					}
					return AffectLine;
				}
			}
			return AffectLine;
		}
	}

}

//����������ݵ�����
int ADOManage::InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString ip, CString ScanType, CString errorreason, CString result, CString CHResult)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	if (ScanType == "Single")
	{
		//�����ݲ������
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
			+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + L"1" + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//����ִ�е�SQL���
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}


		//����ʧ�ܾʹ���ԭ���������ˣ��Ǿ͸���һ��ʧ�ܴ���
		if (result == "0"&&Affectline.vt == 0)
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'��") + errorreason + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("',CHResult='") + CHResult + _T("',TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}
		}


		//����testresult���ű�
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
		try{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
		return AffectLine;
	}
	else
	{
		if (ScanType == "MutiPC1")
		{
			//�����ݲ������
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT1], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + L"1" + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//����ִ�е�SQL���
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}


			//����ʧ�ܾʹ���ԭ���������ˣ��Ǿ͸���һ��ʧ�ܴ���
			if (result == "0"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPRESULT1='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'��") + errorreason + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("',CHResult='") + CHResult + _T("',TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
				}
				catch (_com_error &e)
				{
				}
			}


			//����testresult���ű�
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}
			return AffectLine;
		}
		else if (ScanType == "MutiPC2")
		{
						//�����ݲ������
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [SECTIONNO1],[SECTIONNO2], [CPRESULT2], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR],[CHResult],[TesterID2])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + L"1" + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//����ִ�е�SQL���
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}


			//����ʧ�ܾʹ���ԭ���������ˣ��Ǿ͸���һ��ʧ�ܴ���
			if (result == "0"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPRESULT2='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'��") + errorreason + _T("',SECTIONNO1='") + imei2 + _T("',SECTIONNO2='") + imei3 + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("',CHResult='") + CHResult + _T("',TesterID2='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
				}
				catch (_com_error &e)
				{
				}
			}


			//����testresult���ű�
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
			}
			catch (_com_error &e)
			{
			}
			return AffectLine;
		}

	}
	
}


//��ȡ��������
_RecordsetPtr ADOManage::GetOrderNumber()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE Status='0' OR Status='1' order by ZhiDan" );
	//CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[orderld]");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//���ݶ����Ż�ȡIMEI��
_RecordsetPtr ADOManage::GetIMEIByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE (Status='0' OR Status='1') AND [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	return m_pRecordSet;
}

//�ж϶������Ƿ����
_RecordsetPtr ADOManage::JudgeOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	return m_pRecordSet;

}


//���涩����ѡ����ֶ�
void ADOManage::Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID,int IMEI2)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	//CString a;
	//a = CMFCP2CPDlg::SyllableMap[0];
	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam]([ZhiDan],[IMEISyllable], [SNSyllable], [SIMSyllable], [VIPSyllable], [ICCIDSyllable], [BATSyllable], [MACSyllable], [EquipmentSyllable],[RFIDSyllable],[IMEI2Syllable])values('")
		+ order + _T("', %d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"), IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
	}

	//Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
	if (Affectline.vt == 0)
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] SET [IMEISyllable]=%d, [SNSyllable]=%d, [SIMSyllable]=%d, [VIPSyllable]=%d, [ICCIDSyllable]=%d, [BATSyllable]=%d, [MACSyllable]=%d, [EquipmentSyllable]=%d,[RFIDSyllable]=%d,[IMEI2Syllable]=%d WHERE [ZhiDan]='") + order + _T("'"), IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
}

//��ȡ������ѡ����ֶ�
void ADOManage::Readsyllable(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

}

//���涩�����󶨵��ֶ�
void ADOManage::Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID, int IMEI2)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam]([ZhiDan],[SIMBind], [VIPBind], [ICCIDBind], [BATBind], [MACBind], [EquipmentBind],[RFIDBind],[IMEI2Bind])values('")
		+ order + _T("', %d,%d,%d,%d,%d,%d,%d,%d)"), SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
	}

	//Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
	if (Affectline.vt == 0)
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] SET [SIMBind]=%d, [VIPBind]=%d, [ICCIDBind]=%d, [BATBind]=%d, [MACBind]=%d, [EquipmentBind]=%d,[RFIDBind]=%d,[IMEI2Bind]=%d  WHERE [ZhiDan]='") + order + _T("'"), SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
}

//��ȡ�������󶨵��ֶ�
void ADOManage::Readbind(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
}

//����һЩ����
void ADOManage::SaveConfig(CString order, int imei3Flag, int IsSinglePcFlag)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	//����
	_variant_t Affectline;
	CString strSql;

	//�����ݲ������
	strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam]([ZhiDan],[IMEI3Flag],[PCModeFlag])values('") + order + _T("', %d,%d)"), imei3Flag, IsSinglePcFlag);

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
	}

	//Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
	if (Affectline.vt == 0)
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] SET [IMEI3Flag]=%d,[PCModeFlag]=%d WHERE [ZhiDan]='") + order + _T("'"), imei3Flag, IsSinglePcFlag);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
}

//��ȡһЩ����
void ADOManage::ReadConfig(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
}

//ͨ������ֵ�Ǵ��û�Ȩ�ޣ�����CheckFail��ʾ�����ڴ��û�
CString ADOManage::CheckUserAuthority(CString UserName, CString UserPassword)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	//����
	_variant_t Affectline;
	CString strSql;

	_variant_t AuthorityTempVT;
	CString AuthorityTemp;

	//�����ݲ������
	strSql.Format(L"SELECT  * FROM [GPSTest].[dbo].[LUserAccount] WHERE [Name]='%s' AND [Password]='%s'",UserName,UserPassword);

	try{
		m_pRecordSet=m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����

		//�Ҳ����Ǿͷ���1
		if (m_pRecordSet->adoEOF)
		{
			return L"CheckFail";
		}
		else
		{
			AuthorityTempVT = m_pRecordSet->GetCollect("UserType");
			if (AuthorityTempVT.vt == VT_EMPTY || AuthorityTempVT.vt == VT_NULL)
			{
				return L"";
			}
			AuthorityTemp = (LPCSTR)_bstr_t(AuthorityTempVT);

			return AuthorityTemp;
		}
	}
	catch (...)
	{
	}
}

//��ȡ��ǰʱ�亯��
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}