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
	CString m_Firstdbname;    //�ص����ݿ�����
	CString m_Seconddbname;  //�������ݿ�����
	CString m_Firstformname;  //�ص����ݱ�����
	CString m_Secondformname;//�������ݱ�����
	void GetDBINFO();//��ȡ���ݿ���Ϣ����


	//���¶����Զ��庯��
public:
	//���ݿ����Ӻ���Դ�ͷź���
	bool ConnSQL();//�������ݿ⺯��
	bool ConntestdbSQL();//���ݿ�����������������������
	bool ConntestdbSQL2();//���Եڶ������ݿ��õ�
	bool CloseDB();//�ر����ݿ�
	bool CloseAll();//�ͷ����ݿ���Դ

	//��ͨ�ú���
	_RecordsetPtr MOBAN(CString str1, CString str2);//ģ�溯��

	//��ʼ������ģ�������õĺ���
	_RecordsetPtr GetZhiDan();//��ȡ������
	_RecordsetPtr GetIMEIByZhiDan(CString strzhidan);//���ݶ����Ż�ȡIMEI��
	_RecordsetPtr JudgeZhiDan(CString strzhidan);//�ж϶������Ƿ����
    CString GetIMEICurrent(CString strzhidan);//��ȡ��ǰ�ص��imei�ź�ģ��·��
	int GetImeicount(CString strzhidan, CString imeistart, CString imeiend);//��ȡIMEI����
	

	//�ص�ϵͳģ�����ú���
	_RecordsetPtr UpdateIMEICurrent(CString strzhidan, CString imeicurrent);//�洢��ǰ�ص��imei��
	_RecordsetPtr AddNew(CString strpicp, CString strzhidan, CString strimei);//�����ص�������ݵ��ص����ݱ�
	_RecordsetPtr UpdateReld(CString strimei);//�����ص�ĸ��²���
	int JudgeIMEIExit(CString strImei);//�ж�IMEI�Ƿ����


	//���ݿ��ѯģ�����õĺ���
	_RecordsetPtr ShowInsertImeiByOrderNumber(CString ordernumber);//����Ӧ�Ķ��������Ѿ������IMEIչʾ����
	_RecordsetPtr ShowRepeatImeiByOrderNumber(CString ordernumber);//����Ӧ�Ķ��������Ѿ��ظ���IMEIչʾ����
	_RecordsetPtr ShowUnImeiByOrderNumber(CString ordernumber, CString imeistrat, CString imeiend);//����Ӧ�Ķ�������δ�����IMEIչʾ����

	//ͨ�ú���
	_RecordsetPtr GetRst();//�ɵ�ĳ�ű������õ�
	CString GetTime();//��ȡ��ǰϵͳʱ��
	CString CreateIMEI15(CString imei);//������IMEIУ��λ�õ�
};

