#pragma once

extern CString RDAComReceive[16];
static void RecvDataCallBack(int host_or_rs232, int commIndex, char * pContent);//�ص�����������ȫ�ֺ���

class RDAHostInterface
{
public:
	RDAHostInterface();
	~RDAHostInterface();

	HINSTANCE m_RDaHInst;
	BOOL ComInit[16];
	BOOL RDADllInit();//��ʼ������
	BOOL RDAComInit(int HandleNum,CString PortName);//���ڳ�ʼ��
	BOOL RDAComWriteData(int HandleNum, CString PortData);//д���ں���
	BOOL RDAComShutdown(int HandleNum);//�ش��ں���
	BOOL RDAComReconnect(int HandleNum);//�����������Ӻ���
	BOOL RDADeinitialization();//�ͷŶ�̬��



};

