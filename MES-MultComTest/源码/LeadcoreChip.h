#pragma once

typedef enum _enErrorCode
{
	enNoneErr				= 0,			//û�д��� 
	enUnknownCmdErr			= 1,			//δ֪�������
	enMissingParamErr		= 2,			//ȱ�ٲ���
	enParamErr				= 3,			//��������
	enStateErr  			= 4,			//״̬����
	enParseCmdErr			= 5,			//����Command����
	enL1NoRspErr			= 6,			//L1û����Ӧ
	enL1CellSearchFailErr	= 7,			//L1����ʧ��
	enL1NotSupportErr		= 8,			/*L1��֧��*/
	enFlashAddrErr			= 9,			/*Flash��ַԽ��*/
	enUnknowErr				= 10,          /*δ֪����*/      

	ComNoRsp				=-1,			//��������Ӧ
	ComRspErr				=-2,			//������Ӧ����
	ComCheckErr				=-3,			//������ӦУ�������
	ComNoRspMsg				=-4,			//��������Ϣ��Ӧ
	ComRspMsgErr			=-5,			//������Ϣ��Ӧ����
	ComMsgCheckErr			=-6,			//������Ϣ��ӦУ�������
	ComRspTypeErr			=-7,			//������Ӧ���ʹ���
	ComRspLenErr			=-8,			//������Ӧ���ȴ���
	ComMsgLenErr			=-9,			//������Ϣ��Ӧ���ȴ���
	TblWrErr				=-10,			//APC/AGC���д���ݴ���
	IMEIWrErr				=-11,			//IMEI��д����
	DspRspTypeErr			=-20,			//Dsp��Ӧ���ʹ���(GSM)
	
	ThreadExited			=-100,			//�����߳��˳�
	SwVerErr				=-101,			//����汾����
	PreStationUnpass		=-102,			//δͨ��ǰһ���Ե����
	HwVerErr				=-103,			//Ӳ���汾����
}EnumErrorCode;
typedef enum LOSS_BAND
{
	TD2L = 0,
	TD2M,
	TD2H,
	GSM900L,
	GSM900M,
	GSM900H,
	DCSL,
	DCSM,
	DCSH,
	GSM850L,
	GSM850M,
	GSM850H,	
	PCSL,
	PCSM,
	PCSH,
	TD1L,
	TD1M,
	TD1H,
	LTE1L,
	LTE1M,
	LTE1H,
	LTE2L,
	LTE2M,
	LTE2H,
	LTEB1L,
	LTEB1M,
	LTEB1H,
	LTEB3L,
	LTEB3M,
	LTEB3H,
	LTEB7L,
	LTEB7M,
	LTEB7H,
	LTEB17L,
	LTEB17M,
	LTEB17H
}LossBand;



#define WM_SEND_MSG WM_USER+100
class Strategy
{
public:
	Strategy();
	virtual ~Strategy();
	virtual int AlgrithmInterface(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version);
};
class CLeadcoreChip:public Strategy
{
public:
	CLeadcoreChip(void); 
	virtual ~CLeadcoreChip(void);
	int AlgrithmInterface(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version); 

private:
	CWnd * LBC_pWnd;
public:
	static int Tstop;  
public:
	char* scanSN1,*scanIMEI1,*scanWIFI,*Software_Ver;									//ɨ�������SN\IMEI\MAC
	int Com_port;
protected:
	void SendMSG(char* msg);
public:
	int AssociateWnd(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version);

	EnumErrorCode Turn_on(char mode); 
	int RUN_TEST(void);     

	EnumErrorCode Com_init(int Rs232);
	void lowtoup(char *ch);

	EnumErrorCode AT_rd(char *Resp);
	EnumErrorCode TestRead_AT(int *data,char *Resp); 
	void STOP (void);
};
