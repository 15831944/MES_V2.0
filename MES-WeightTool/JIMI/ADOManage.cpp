#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>
#include "JIMIDLG.h"


//ȫ�ֱ�������������г�ʼ��

ADOManage::ADOManage() :
m_DataSource(_T("10.10.11.90")),
m_Port(_T("6033")),
m_UserName(_T("sa")),
m_Password(_T("jiminewpower")),
m_Firstdbname(_T("GPSTest")),
m_Seconddbname(_T("GPSTest")),
m_Firstformname(_T("Gps_WeightNetOrderResult")),
m_Secondformname(_T("Gps_ManuPrintParam"))
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
/*�����ݿ⡰SQLServer����������Ҫ�������ݿ����� */
	CString m_ConnectStr;//�����ַ���
	//Ҫע�������ַ���������ģʽ������Ӧ��ѡ������ģʽ
	m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Firstdbname;
	try
	{
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
	//GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	//ADOManage::m_Firstdbname = str;
	//ADOManage::m_Seconddbname = str;
	//str.ReleaseBuffer();
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
int ADOManage::CpImeiByNo(CString Syllablesqlstr, CString noname, CString no, CString strzhidan)
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

	try 
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		//��Ҫ���򣬹���ûʱ���
		//::MessageBox(NULL, _T("�Զ�ƥ���ֶι�ϵ����"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
	}

	////����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	//strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + noname + _T("] =") + _T("'") + no + _T("'") + _T("AND ZhiDan='") + strzhidan + _T("'");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	//�������ڴ˺Ŷ�
	if (m_pRecordSet->adoEOF)
	{
		//��Ҫ���򣬹���ûʱ���
		//::MessageBox(NULL, _T("Gps_ManuPrintParam�ֶ�δƥ�䵽��ӦIMEI"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
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

////////////�ѽ��д�������¼�ܱ�/////////
int ADOManage::adoToResultSheet(CString result, CString imei)//�ѽ����¼���ܱ�
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	_variant_t Affectline;
	CString strSql;
	try
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_TestResult] SET [WeightResult]=%s WHERE [IMEI]='") + imei + _T("'"), result);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		return 0;
	}
	return 1;
}

//������ȷ���ݵ�����
int ADOManage::InsertResultLog(CString zhidan, CString imei, CString weight,CString uplimit, CString downlimit, CString result, CString ip, CString username, CString remarkerror)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	remarkerror = GetTime() +_T(" ") + remarkerror + _T("!!!!!");//���������ʱ��
	//����
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;
	strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [IMEI]='") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (m_pRecordSet->adoEOF)//���û�и���Ͳ��룬�о�ˢ��
	{
		remarkerror = _T("1. ") + remarkerror;
		//�����ݲ������
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([ZhiDan],[IMEI], [Weight],[UpperLimit],[DownLimit], [RESULT], [RecordTime], [IP], [UserName],[Remark],[ReadTimes])values('")
			+ zhidan + _T("', '") + imei + _T("', '") + weight + _T("', '") + uplimit + _T("', '") + downlimit + _T("', '") + result + _T("', '")  + GetTime() + _T("', '") + ip + _T("', '") + username + _T("', '") + remarkerror +_T("', '1'") + _T(")");//����ִ�е�SQL���
		try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
			//��Ҫ���򣬹���ûʱ���
			//::MessageBox(NULL, _T("���ݿ������´���"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
		}
	}
	else//ˢ������
	{
		CString sRemark = m_pRecordSet->GetCollect("Remark");
		//��ɨͳ��
		CString str = m_pRecordSet->GetCollect("ReadTimes");//ɨ�����ͳ��
		int itimes = _wtoi(str.GetBuffer());
		itimes = itimes + 1;
		str.Format(_T("%d"), itimes);



		sRemark = str+_T(".��ɨ")+ remarkerror + sRemark;  //����ǰ�ɵĴ������ݣ��ۼ�������д��
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET Weight='") + weight + _T("',RESULT ='") + result + _T("', RecordTime='") + GetTime() + _T("',IP ='") + ip + _T("',UserName ='") + username + _T("',Remark='") + sRemark + _T("',ReadTimes ='") + str + _T("' where[IMEI] = '") + imei + _T("'");

		try {
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
			//��Ҫ���򣬹���ûʱ���
			//::MessageBox(NULL, _T("���ݿ�ˢ�´��󣬻����ݳ��ȹ���"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
		}
	}
	return AffectLine;
}

//��ȡ��������
_RecordsetPtr ADOManage::GetOrderNumber()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE Status='0' OR Status='1' order by ZhiDan" );
	//CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[orderld]");
	try
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		::MessageBox(NULL, _T("��ȡ������Ϣ����"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
	}
	
	return m_pRecordSet;
}

//���ݶ����Ż�ȡIMEI��
_RecordsetPtr ADOManage::GetIMEIByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	CString strSql = _T("SELECT [IMEIStart],[IMEIEnd] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");	
	try
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����

	}
	catch (_com_error &e)
	{
		::MessageBox(NULL, _T("��ȡ������Ϣ����"), _T("�쳣��Ϣ"), NULL); /*��ӡ���쳣ԭ��*/
	}
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


//���涩����ѡ����ֶκ���λ����
void ADOManage::Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID, int IMEI2, CString uplimit, CString downlimit, CString disuplimit,CString disdownlimit)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	//CString a;

	_variant_t Affectline;
//	CString strSql;

	CString strSql = _T("SELECT [IMEISyllable] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_WeightNetOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (m_pRecordSet->adoEOF)//���û�и���Ͳ��룬�о�ˢ��
	{
	  //�����ݲ������
		strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam]([ZhiDan],[IMEISyllable], [SNSyllable], [SIMSyllable], [VIPSyllable], [ICCIDSyllable], [BATSyllable], [MACSyllable], [EquipmentSyllable],[RFIDSyllable],[IMEI2Syllable],[UpperLimit],[DownLimit],[DisUpperLimit],[DisDownLimit])values('")
			+ order + _T("', %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s)"), IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2,uplimit,downlimit,disuplimit,disdownlimit);
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}
	else
	{
		//Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
	    //	if (Affectline.vt == 0)
		try
		{
			strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam] SET [IMEISyllable]=%d, [SNSyllable]=%d, [SIMSyllable]=%d, [VIPSyllable]=%d, [ICCIDSyllable]=%d, [BATSyllable]=%d, [MACSyllable]=%d, [EquipmentSyllable]=%d,[RFIDSyllable]=%d,[IMEI2Syllable]=%d,[UpperLimit]=%s,[DownLimit]=%s,[DisUpperLimit]=%s,[DisDownLimit]=%s WHERE [ZhiDan]='") + order + _T("'"), IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID, IMEI2, uplimit, downlimit, disuplimit, disdownlimit);
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}	
}

//��ȡ������ѡ����ֶ�
void ADOManage::Readsyllable(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
//	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	CString strSql = _T("SELECT * FROM [" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	try
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		AfxMessageBox(_T("��ȡ���ݿ�Gps_WeightNetOrderRelationParamʧ��"));
	}
	

}

//���涩�����󶨵��ֶ�
void ADOManage::Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	//����
	_variant_t Affectline;
	CString strSql;
	strSql = _T("SELECT [IMEISyllable] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_WeightNetOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (m_pRecordSet->adoEOF)//���û�и���Ͳ��룬�о�ˢ��
	{
		//�����ݲ������
		strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam]([ZhiDan],[SIMBind], [VIPBind], [ICCIDBind], [BATBind], [MACBind], [EquipmentBind],[RFIDBind])values('")
			+ order + _T("', %d,%d,%d,%d,%d,%d,%d)"), SIM, VIP, ICCID, BAT, MAC, Equipment,RFID);
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}
	else
	{
		//Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
	//	if (Affectline.vt == 0)
		try
		{
			strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam] SET [SIMBind]=%d, [VIPBind]=%d, [ICCIDBind]=%d, [BATBind]=%d, [MACBind]=%d, [EquipmentBind]=%d,[RFIDBind]=%d  WHERE [ZhiDan]='") + order + _T("'"), SIM, VIP, ICCID, BAT, MAC, Equipment,RFID);
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
	   {
		}

	}

	
}

//��ȡ�������󶨵��ֶ�
void ADOManage::Readbind(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
//	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
}

//����һЩ����
void ADOManage::SaveConfig(CString order, int imei3Flag)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��

	//����
	_variant_t Affectline;
	CString strSql;
	strSql = _T("SELECT [IMEISyllable] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_WeightNetOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	if (m_pRecordSet->adoEOF)//���û�и���Ͳ��룬�о�ˢ��
	{
		//�����ݲ������
		strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam]([ZhiDan],[IMEI3Flag])values('") + order + _T("', %d)"), imei3Flag);
		try {
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}
	else
	{
		//Affectline��insert�������ص�Ӱ�����������Ϊ0�������ʧ�ܣ�Ҳ����˵֮ǰ�Ѿ��������������
		//if (Affectline.vt == 0)
		try
		{
			strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam] SET [IMEI3Flag]=%d WHERE [ZhiDan]='") + order + _T("'"), imei3Flag);
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//ֱ��ִ�����
		}
		catch (_com_error &e)
		{
		}
	}
}

//��ȡһЩ����
void ADOManage::ReadConfig(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_WeightNetOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");
	try
	{
        m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
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

CString ADOManage::CheckUser(CString username, CString userpswd , CString &userright)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//��ʼ��Recordsetָ��
	//CString strSql = _T("SELECT UserDes From LUserAccount WHERE Name = '" )+ username + _T("' AND Password = '") + userpswd + _T("'");
	CString strSql = _T("SELECT * From LUserAccount WHERE Name = '") + username + _T("' AND Password = '") + userpswd + _T("'");
	try
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{

	}
	
	if (!m_pRecordSet->adoEOF)
	{
		userright = m_pRecordSet->GetCollect("UserType").bstrVal;
		return m_pRecordSet->GetCollect("UserDes").bstrVal;	
	} 
	return _T("");
}
//����IMEI�ж϶Աȹ�λ�ʺб�־λ�ж�
bool ADOManage::CheckCHRESULT(CString imei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//����
	_variant_t a;
	CString strSql, strzhidan;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [CHResult] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuCpParam] WHERE [IMEI1] =") + _T("'") + imei + _T("'");
	try
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		return false;
	}
	if (m_pRecordSet->adoEOF)
	{
		return false;
	}
	else
	{
		CString ret = m_pRecordSet->GetCollect("CHResult");
		//CString ret = m_pRecordSet->adoEOF;
		if ("1" == ret)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}
//����IMEI�ж϶Աȹ�λ��־λ
int ADOManage::CpCaiheByImei(CString imei, CString ZhiDan)
{
	//��ʼ��Recordsetָ��
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//����
	_variant_t a;
	CString strSql, strzhidan;

	//����IMEI�Ƿ���ڣ������ڷ���0����δ�ҵ�IMEI
	strSql = _T("SELECT [CPRESULT] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuCpParam] WHERE [IMEI1] =") + _T("'") + imei + _T("'");
	try
	{
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//ֱ��ִ�����
	}
	catch (_com_error &e)
	{
		//��Ҫ���򣬹���ûʱ���
		//AfxMessageBox(_T("Gps_ManuCpParam�޴�IMEI��ƥ�����"));
		return 2;
	}
	if (m_pRecordSet->adoEOF)
	{		
		return 2;
	}
	else
	{
		CString ret = m_pRecordSet->GetCollect("CPRESULT");
		if ("1" == ret)
		{
			return 1;
		}
		return 0;
	}
	return 2;//���涼û����ͷ���2����ɹ�
}