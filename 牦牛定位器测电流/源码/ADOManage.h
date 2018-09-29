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

	//��ȡ���ݿ���Ϣ���������������ݿ��ʱ����Ȼ�ȡ��Щ����������,���û�д��ⲿ��ȡ�Ǿ������ʼ�������Զ���һ��
public:
	CString m_DataSource;  //������IP
	CString m_Port;        //�������˿�
	CString m_UserName;    //�û���
	CString m_Password;    //����
	CString m_Firstdbname;    //��һ�����ݿ�����
	CString m_Seconddbname;  //�ڶ������ݿ�����
	CString m_Firstformname;  //��һ�����ݱ�����
	CString m_Secondformname;//�ڶ������ݱ�����
	void GetDBINFO();//��ȡ���ݿ���Ϣ����


	//���¶����Զ��庯��
public:
	//���ݿ����Ӻ���Դ�ͷź���
	bool ConnSQL();//�������ݿ⺯��
	bool ConntestdbSQL1();//���ݿ�����������������������
	bool ConntestdbSQL2();//���ݿ�����������������������
	bool CloseDB();//�ر����ݿ�
	bool CloseAll();//�ͷ����ݿ���Դ

	//��ͨ�ú���
	_RecordsetPtr MOBAN(CString str1, CString str2);//ģ�溯��

	//���Խ����
	//���Ժ󽫽���Լ�RID�Ȳ��뵽���ݿ�,���Ѳ��Թ���ʧ�ܵĸ��½��Ϊ1
	void TestResultInsertSql(CString Rid, CString StandbyCurrent, CString StandbyAverage, CString SleepCurrent, CString SleepAverage, CString TestResult);
	//����RID�������ݣ��ҵ����Ҳ��Խ��Ϊͨ�����򷵻�0��ǰ������ʾ�Ѳ���ͨ�����鲻�����ݻ��߲鵽���ǲ��Խ��Ϊ��ͨ�����򷵻�1��ǰ�����¼�������
	int CheckTestResultByRid(CString Rid);

	//���ñ�
	//���������Ϣ
	void ConfigInsertSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime);
	//���ݻ��Ͳ��Ҹû�������
	_RecordsetPtr CheckConfigByModelNameSql(CString ModelName);
	//�������л���
	_RecordsetPtr CheckAllInConfigSql();
	//����������Ϣ
	void ConfigUpdataSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime);
	//���ݻ���ɾ����������
	void DeleteConfigByModelNameSql(CString ModelName);

	//ͨ�ú���
	CString GetTime();//��ȡ��ǰϵͳʱ��
	_RecordsetPtr GetRst();//�ɵ�ĳ�ű������õ�

};

