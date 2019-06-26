#pragma once

#import "msado15.dll"  no_namespace rename("EOF", "adoEOF")


class ADOManage
{
public:
	ADOManage();
	~ADOManage();

	//�Զ������
public:
	_ConnectionPtr m_pConnection;    //���Ӳ���
	_CommandPtr m_pCommand;          //SQL�������
	_RecordsetPtr m_pRecordSet;      //��¼������

	//�Զ��庯��
public:
	bool ConnSQL();//�������ݿ⺯��
	bool ConndbSQL();//�������ݿ⺯��2,���ݿ�����������������������
	bool CloseAll();//�ر����ݿ⺯��



	int CpImeiByNo(CString Syllablesqlstr, CString no, CString strzhidan);//���������Ŷ�Ѱ��IMEI
	int CpCaiheByImei(CString imei, CString ZhiDan);//����IMEIѰ�Ҳʺ�ʱ��
	int InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString ip, CString ScanType, CString errorreason, CString result, CString CHResult);//������ȷ���ݵ�����
	int InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString ip, CString ScanType, CString errorreason, CString result, CString CHResult);//����������ݵ�����
	int JudgeImei(CString imei);//�ж�IMEI���Ƿ����
	int JudgeZhidan(CString imei,CString Zhidan);//�ж��Ƶ����Ƿ�Ե���
	void Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID, int IMEI2);//���涩����ѡ����ֶ�
	void Readsyllable(CString order);//��ȡ������ѡ����ֶ�
	void Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID, int IMEI2);//���涩�����󶨵��ֶ�
	void Readbind(CString order);//��ȡ�������󶨵��ֶ�
	void SaveConfig(CString order, int imei3Flag, int IsSinglePcFlag);//����һЩ����
	void ReadConfig(CString order);//��ȡһЩ����
	_RecordsetPtr GetOrderNumber();//��ȡ������
	_RecordsetPtr GetIMEIByOrderNumber(CString ordernumber);//���ݶ����Ż�ȡIMEI��
	_RecordsetPtr JudgeOrderNumber(CString ordernumber);//�ж϶������Ƿ����

	CString ADOManage::GetTime();//��ȡ��ǰϵͳʱ��


	CString CheckUserAuthority(CString UserName,CString UserPassword);//ͨ������ֵ�Ǵ��û�Ȩ�ޣ�����CheckFail��ʾ�����ڴ��û�

	//Ŀǰ��δʹ�õĺ���
	_RecordsetPtr GetRst();

	//��ȡ���ݿ���Ϣ���������������ݿ��ʱ����Ȼ�ȡ��Щ����������
public:
	CString m_DataSource;  //������IP
	CString m_Port;        //�������˿�
	CString m_UserName;    //�û���
	CString m_Password;    //����
	CString m_Firstdbname;    //�ص����ݿ�����
	CString m_Seconddbname;  //�������ݿ�����
	CString m_Firstformname;  //�ص����ݱ�����
	CString m_Secondformname;//�������ݱ�����
	void GetDBINFO();//��ȡ���ݿ���Ϣ����

	//ȥ���ַ���ͷβ
	void ADOManage::trim(CString &str);

	CString CreateIMEI15(CString imei);
};

