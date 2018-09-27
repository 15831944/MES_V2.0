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



	int CpImeiByNo(CString Syllablesqlstr, CString noname, CString no, CString strzhidan);//���������Ŷ�Ѱ��IMEI
	int CpCaiheByImei(CString imei, CString ZhiDan);//����IMEIѰ�Ҳʺ�ʱ��
	int InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result);//������ȷ���ݵ�����
	int InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result);//����������ݵ�����
	int JudgeImei(CString imei);//�ж�IMEI���Ƿ����
	int JudgeZhidan(CString imei,CString Zhidan);//�ж��Ƶ����Ƿ�Ե���
	void Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment);//���涩����ѡ����ֶ�
	void Readsyllable(CString order);//��ȡ������ѡ����ֶ�
	void Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment);//���涩�����󶨵��ֶ�
	void Readbind(CString order);//��ȡ�������󶨵��ֶ�
	_RecordsetPtr GetOrderNumber();//��ȡ������
	_RecordsetPtr GetIMEIByOrderNumber(CString ordernumber);//���ݶ����Ż�ȡIMEI��
	_RecordsetPtr JudgeOrderNumber(CString ordernumber);//�ж϶������Ƿ����

	CString ADOManage::GetTime();//��ȡ��ǰϵͳʱ��



	/*
	//���ݿ��ѯģ�����õĺ���
	_RecordsetPtr ShowInsertImeiByOrderNumber(CString ordernumber);//����Ӧ�Ķ��������Ѿ������IMEIչʾ����
	_RecordsetPtr ShowRepeatImeiByOrderNumber(CString ordernumber);//����Ӧ�Ķ��������Ѿ��ظ���IMEIչʾ����
	_RecordsetPtr ShowUnImeiByOrderNumber(CString ordernumber, CString imeistrat, CString imeiend);//����Ӧ�Ķ�������δ�����IMEIչʾ����

	//���¶���ģ�����õĺ���
	int GetImeicount(CString zhidan, CString imeistart, CString imeiend);//��ȡIMEI����

	CString GetIMEICurrent(CString strordernumber);//��ȡ��ǰ�ص��imei��


	//ͨѶģ�����õĺ���
	_RecordsetPtr UpdateIMEICurrent(CString strordernumber, CString imeicurrent);//�洢��ǰ�ص��imei��
	_RecordsetPtr JudgeRepetition(CString strImei);//�ж�IMEI�Ƿ��ظ�
	_RecordsetPtr JudgeIMEIExit(CString strImei);//�ж������ص�ʱ��IMEI�Ƿ����
	int AddNew(CString picp, CString zhidan);//�����ص�������ݵ��ص����ݱ�
	int updatereld(CString zhidan);//�����ص�ĸ��²���
    */
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

