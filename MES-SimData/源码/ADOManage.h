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

	void SimDataOkInsertSql(CString SDIP,CString RID,CString IMEI,CString CID,CString ICCID,CString SDOperator);//SIM�����سɹ�����뺯��
	void SimDataErrorInsertSql(CString SDIP, CString RID, CString IMEI, CString SDOperator);//SIM������ʧ�ܺ���뺯��
	int SimDataLastStationSql(CString IMEI);//�����һ����λ�Ƿ�ͨ������
	int SimDataIsExitSql(CString RID,CString IMEI);//��ѯ�����Ƿ���ڲ������سɹ�����
	int SimDataNoIsExitSql(CString CID);//��ѯ���Ӻ��Ƿ��Ѿ����ع�����

	int SimDataReSql(CString RID, CString IMEI,CString strOKpath);//����λ������

	//ͨ�ú���
	CString GetTime();//��ȡ��ǰϵͳʱ��
	_RecordsetPtr GetRst();//�ɵ�ĳ�ű������õ�

};

