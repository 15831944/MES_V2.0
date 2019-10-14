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

	CString CheckUser(CString username, CString userpswd ,CString &userright);//�û���ȷ��
	int CpCaiheByImei(CString imei, CString ZhiDan);//����IMEI�ж϶Աȹ�λ��־λ
	bool CheckCHRESULT(CString imei);//���Աȹ�λ�ʺ��ж�

	//CString 


	int CpImeiByNo(CString Syllablesqlstr, CString noname, CString no, CString strzhidan);//���������Ŷ�Ѱ��IMEI

	int adoToResultSheet(CString result, CString imei);//�ѽ����¼���ܱ�
	int InsertResultLog(CString zhidan, CString imei, CString weight, CString uplimit, CString downlimit, CString result, CString ip, CString username, CString remarkerror);//������ȷ���ݵ�����
	int JudgeImei(CString imei);//�ж�IMEI���Ƿ����
	int JudgeZhidan(CString imei,CString Zhidan);//�ж��Ƶ����Ƿ�Ե���
	void Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment,int RFID, int IMEI2, CString uplimit, CString downlimit, CString disuplimit, CString disdownlimit);//���涩����ѡ����ֶ�
	void Readsyllable(CString order);//��ȡ������ѡ����ֶ�
	void Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID);//���涩�����󶨵��ֶ�
	void Readbind(CString order);//��ȡ�������󶨵��ֶ�
	void SaveConfig(CString order, int imei3Flag);//����һЩ����
	void ReadConfig(CString order);//��ȡһЩ����
	_RecordsetPtr GetOrderNumber();//��ȡ������
	_RecordsetPtr GetIMEIByOrderNumber(CString ordernumber);//���ݶ����Ż�ȡIMEI��
	_RecordsetPtr JudgeOrderNumber(CString ordernumber);//�ж϶������Ƿ����

	CString ADOManage::GetTime();//��ȡ��ǰϵͳʱ��


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
	CString m_Firstformname;  //�������������
	CString m_Secondformname;//�������ݱ����ƣ��Զ�ƥ��IMEI��ƥ��ı��
	void GetDBINFO();//��ȡ���ݿ���Ϣ����

	//ȥ���ַ���ͷβ
	void ADOManage::trim(CString &str);

	CString CreateIMEI15(CString imei);
};

