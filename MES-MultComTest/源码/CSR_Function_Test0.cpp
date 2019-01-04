// CSR_Function_Test.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "CSR_Function_Test.h"


// CCSR_Function_Test �Ի���
#include "math.h"

//IMPLEMENT_DYNAMIC(CCSR_Function_Test, CDialog)

CCSR_Function_Test::CCSR_Function_Test(CWnd* pParent /*=NULL*/)
	: CDialog(CCSR_Function_Test::IDD, pParent)
	, BLE_m_Result_C(_T(""))
	, BLE_Final_Result(_T(""))
{

}

CCSR_Function_Test::~CCSR_Function_Test()
{
}

void CCSR_Function_Test::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Port);
	DDX_Control(pDX, IDC_COMBO2, m_Baud);
	DDX_Control(pDX, IDC_RESULT, BLE_m_Result);
	DDX_Text(pDX, IDC_RESULT, BLE_m_Result_C);
	DDX_Text(pDX, IDC_STA_FINAL_RESULT, BLE_Final_Result);
	DDX_Control(pDX, IDC_STA_FINAL_RESULT, BLE_Final_Result_Control);
	DDX_Control(pDX, IDC_BUTTON2, STOP_Test_Control);
	DDX_Control(pDX, IDC_BUTTON1, START_Test_Control);
	DDX_Control(pDX, IDC_STA_FINAL_RESULT2, BLE_Operate_Guide_Control);
	DDX_Control(pDX, IDC_RESULT2, BLE_MAC_RSSI);
}


BEGIN_MESSAGE_MAP(CCSR_Function_Test, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CCSR_Function_Test::OnBnClickedButton1)
	ON_WM_CTLCOLOR()//�ı���ɫ
	ON_WM_TIMER()//�ֶ����
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CCSR_Function_Test::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCSR_Function_Test::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCSR_Function_Test::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON11, &CCSR_Function_Test::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON5, &CCSR_Function_Test::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON36, &CCSR_Function_Test::OnBnClickedButton36)
	ON_BN_CLICKED(IDCANCEL, &CCSR_Function_Test::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCSR_Function_Test::OnBnClickedOk)
END_MESSAGE_MAP()


//CBT_Addr_Setting BLE_Setdlg;//��Ϊȫ�ֱ��������������߳�ʹ�õ���ͬһ�� ʵ��  ��



// CCSR_Function_Test ��Ϣ�������

BOOL CCSR_Function_Test::OnInitDialog()//��ʼ������
{
	CDialog::OnInitDialog();
	//SetIcon(m_hIcon,TRUE);
	//SetIcon(m_hIcon,FALSE);
	//CWriteIMEIDlg*  BLE_pdlg=(CWriteIMEIDlg*)GetParent();

	//init baud
	m_Baud.AddString("2400");
	m_Baud.AddString("4800");
	m_Baud.AddString("9600");
	m_Baud.AddString("14400");
	m_Baud.AddString("19200");

	m_Baud.AddString("38400");
	m_Baud.AddString("56000");
	m_Baud.AddString("57600");
	m_Baud.AddString("115200");
	m_Baud.AddString("128000");

	m_Baud.AddString("256000");

	m_Baud.SetCurSel(0);

	//init port
	for( int i=1; i<256; i++ )
	{
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"),i);
		BOOL bSuccess=FALSE;
		HANDLE hPort=::CreateFile(sPort, GENERIC_READ|GENERIC_WRITE, 0, 0,
			OPEN_EXISTING, 0, 0);
		if( hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError=GetLastError();
			if( dwError == ERROR_ACCESS_DENIED)
				bSuccess=TRUE;
		}
		else
		{
			bSuccess=TRUE;
			CloseHandle(hPort);
		}
		if(  bSuccess )
		{
			CString str;
			str.Format("COM%d",i);
			m_Port.AddString(str);
		}
	}
	m_Port.SetCurSel(0);
	

	CFont* font2;
	font2 = new CFont;
	font2->CreateFont(25,8,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial"); 
	GetDlgItem(IDC_STA_FINAL_RESULT2)->SetFont(font2); 


	HBRUSH mBrush= NULL;//��ˢ
	CFont* font;
	font = new CFont;
	font->CreateFont(63,0,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial"); 

	GetDlgItem(IDC_STA_FINAL_RESULT)->SetFont(font);
	gColor = RGB(60,190,190);
	
	BLE_UI_LoadConfig();
	if(BLE_Setdlg.Initial_Connect_DB==TRUE)//�����ʼ���������ݿ⣬��Ȼ������Чָ�롰
	//if(BLE_pdlg->Initial_Connect_DB_M=="true")
	{
		CString Conn="";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",BLE_Setdlg.m_server,BLE_Setdlg.m_db,BLE_Setdlg.m_user,BLE_Setdlg.m_pwd);
		//Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",BLE_pdlg->m_server_M,BLE_pdlg->m_db_M,BLE_pdlg->m_user_M,BLE_pdlg->m_pwd_M);
		CString DB_FAIL=BLE_Setdlg.myado.ConnecDataLibrary(Conn,"","",adModeUnknown);
		if(DB_FAIL=="FAIL")
		{
			PrintError("�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......");
			return FALSE;
		}
		else
		{
			LogShow_exchange(250,"���ݿ����ӳɹ���\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}
	}
	else
	{
		LogShow_exchange(250,"û��ѡ�������ݿ⣡����\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
	}

	START_Test_Control.EnableWindow(FALSE);
	STOP_Test_Control.EnableWindow(FALSE);
	Initial_Handle=false;
	BLE_Final_Result_Control.SetWindowTextA("IDLE");
	BLE_Final_Result_Control.UpdateWindow();//�ı���ɫ

	BLE_Operate_Guide_Control.SetWindowTextA("���Ȳ�������Ӳ����CSR and TI chip��,\r\n�����ʼ����");
	BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ

	//RedrawWindow();
	Watch_Result=false;//��ʼ���Ϊfalse

	//ReIniCount=0;
	Test_Handle=NULL;
	SetTimer(1,5000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
HBRUSH CCSR_Function_Test::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)//�ı���ɫ
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
	switch(pWnd->GetDlgCtrlID())
	{
	case(IDC_STA_FINAL_RESULT):
		pDC->SetTextColor(gColor);
		break;
	case(IDC_STA_FINAL_RESULT2):
		pDC->SetTextColor(RGB(0,155,100));
		break;
	}
	return hbr;

}
BOOL CCSR_Function_Test::OPen_Serial_Port()//�򿪴���
{
	int debug=0;        //for debug use
	CString sPort,sBaud;//,sTemp;
	int port,baud;
	//get port
	m_Port.GetWindowText(sPort);
	sPort=sPort.Right(sPort.GetLength()-3);
	port=atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"),port);

	//get baud
	m_Baud.GetWindowText(sBaud);
	baud=atoi(sBaud);

	//open com port�򿪶˿�
	hPort=CreateFile(sPort, GENERIC_READ|GENERIC_WRITE,0, NULL, 
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if(hPort == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port.GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hPort=NULL;
		return FALSE;
	}

	//config the com port
	DCB dcb;
   	dcb.DCBlength = sizeof(DCB);
	GetCommState( hPort, &dcb ) ;
	SetupComm( hPort, 4096, 4096 ) ;
	PurgeComm(hPort,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.BaudRate = baud;     //57600(MT6205B), 115200 (MT6218B)			
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fAbortOnError = FALSE;
	dcb.XonChar = 0;
	dcb.XoffChar = 0;
	dcb.ErrorChar = 0;
	dcb.EofChar = 0;
	dcb.EvtChar = 0;
	SetCommState(hPort, &dcb);

	//set time out struct
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hPort, &timeouts);

	//SetCommMask(hPort, EV_RXCHAR|EV_TXEMPTY );//�����¼�����������

	//read and write the com port
	BOOL bReadStatus,bWriteStat;
	DWORD dwBytesWritten,dwBytesRead;
	char *buffer;
	char *p=NULL;
	CString command;
	char buf[1024];
	int timeout=1;
	int i;

	//write "AT"
	for(i=0;i<10;i++)
	{

		command.Empty();
		command.Format("at+act=0\r\n");
		buffer=command.GetBuffer(command.GetLength());
		bWriteStat = WriteFile( hPort, buffer, command.GetLength(), &dwBytesWritten,NULL );
		if( dwBytesWritten != command.GetLength() )
		{
			//PrintError("����û���Ӻã����鴮�ڣ�");
			CloseHandle(hPort);           //�رմ���
			
			return FALSE;
		}

		//receive response
		Sleep(200);
		memset(buf,0,sizeof(buf));
		buffer=buf;
		bReadStatus = ReadFile( hPort, buffer, 10, &dwBytesRead, NULL);
		if(dwBytesRead != 0)
		{
			p=strstr(buffer,"\r");
			if(p)
			{
				//BLE_m_Result.SetWindowText("==============================�ֻ��Ѿ�����������ͨ������......\r\n"+LOG_Show_Old);
				timeout=0;
				break;
			}
		}
		Sleep(100);
	}
	//check if failed
	if(timeout == 1)
	{
		//PrintError("����û���Ӻã����鴮�ڣ�");
		CloseHandle(hPort);//
		return FALSE;
	}
	//make sure rx data cleaned
	Sleep(100);
	//bReadStatus = ReadFile( hPort, buffer, 10, &dwBytesRead, NULL);
	bReadStatus = ReadFile( hPort, buffer, 256, &dwBytesRead, NULL);

	return TRUE;//����TRUE���򿪴�������
}

char*  CCSR_Function_Test::Send_Serial_Order(char** Vaule_Return,CString strCommand_Vaule)//ͨ�����ڷ�������
{
	int Vaule_Return_Count=-1;//�����ĸ���
	BOOL bReadStatus,bWriteStat;
	DWORD dwBytesWritten,dwBytesRead;
	char *buffer;
	char *p=NULL;
	CString command;
	char buf[1024];

	if(strCommand_Vaule=="IDLE")//������ʱ
	{
		//BLE_Operate_Guide_Control.SetWindowTextA("��ʱ�ȴ�...");
		//BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		Delay_Clock(800);//paraArray[i].Low_Limit_Value
		*Vaule_Return="DELAY_SUCCESS";
		return "DELAY_SUCCESS";
		
	}
	Sleep(50);
	for(int i=0;i<2;i++)//��η���
	{
		//send command
		command.Empty();
		//command.Format("AT+CGMR\r\n");
		CString LOG_Time;
		CTime t = CTime::GetCurrentTime();
		LOG_Time.Format("%04d%02d%02d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
		if("at+dt="==strCommand_Vaule)
			strCommand_Vaule+=LOG_Time;
		command=strCommand_Vaule+"\r\n";//ƴ������


		buffer=command.GetBuffer(command.GetLength());
		bWriteStat = WriteFile( hPort, buffer, command.GetLength(), &dwBytesWritten,NULL );
		if( dwBytesWritten != command.GetLength() )
		{
			Sleep(300);
			continue;
		}
		if(strCommand_Vaule=="at+off")//������ʱ
		{
			Delay_Clock(800);//paraArray[i].Low_Limit_Value
			*Vaule_Return="DELAY_SUCCESS";
			return "DELAY_SUCCESS";
			
		}
		//receive response
		Sleep(atol(paraArray[1].Low_Limit_Value));
		CString Port_Temp="";//���ڶ�ȡ���� �Ļ���
		for(int r=0;r<3;r++)
		{	
			memset(buf,0,sizeof(buf));
			buffer=buf;
			bReadStatus = ReadFile( hPort, buffer, 256, &dwBytesRead, NULL);
			if(dwBytesRead != 0)
			{
				p=strstr(buffer,"\r");
				if(p)
				{
					 CString strCommand_Vaule_Return;//���ڷ���ֵ
					 strCommand_Vaule_Return=Port_Temp+CString(buffer);
					 int   nPosStr = strCommand_Vaule_Return.Find(":");
					 CString  selPort = strCommand_Vaule_Return.Mid(nPosStr+1);
					 nPosStr = selPort.Find("OK");
					 if(nPosStr>=0)
						selPort= selPort.Left(nPosStr);	
					 selPort.Replace("\r","");
					 selPort.Replace("\n","");
					 selPort.Replace(",","_");
					 //��ȡ�����ĸ���
					 CString string_sn_r;
					 int npos=selPort.Find('"');
					 if(npos>=0)
					 {
						string_sn_r=selPort.Mid(npos+1);
						Vaule_Return_Count_CS=string_sn_r.Left(string_sn_r.Find('"'));
					 }
					 else
					 {
						Vaule_Return_Count_CS=selPort;
					 }
					 //char* Vaule_Return;
					 //*Vaule_Return=(LPTSTR)(LPCTSTR)Vaule_Return_Count_CS;
					 *Vaule_Return="Analysis_SUCCESS";
					 return "Analysis_SUCCESS";
					 //break;
				}
				else
				{
					//CloseHandle(hPort);//���ڷ���ʧ����رմ���
					//return "FAIL";
					Port_Temp+=CString(buffer);
					Sleep(200);
					continue;
				}
			}
			else
			{
				Sleep(200);
				continue;
			}
		}

	}
	//CloseHandle(hPort);//�ɹ��󲻹رմ���
	*Vaule_Return="FAIL";
	return "FAIL";
}
BOOL CCSR_Function_Test::CSR_Function_Test_Thread()
{
	static char* Serial_Order_Return;//���̵߳�ʱ���쳣
	CWriteIMEIDlg*  Pdlg=(CWriteIMEIDlg*)GetParent();
	if(OPen_Serial_Port()==TRUE)
	{
		//�򿪴��ڳɹ�
		LogShow_exchange(0,"�򿪴��ڳɹ���\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);//��һ����ʾ�������С�
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
	}
	else
	{
		//�򿪴���ʧ��
		PrintError("�򿪴���ʧ�ܣ�����");
		return FALSE;
	}
	int Try_Time=0;//���Դ���������жϣ�
	int All_Result[30]={1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };
	for(int i=1;i<30;i++)
	{
		int Try_Time_Real=0;
		do{
			if(paraArray[i].ifCheck!=true)//û�й�ѡ
			{
				All_Result[i]=1;
				break; //����do while
			}
			if((paraArray[i].showName=="д���ܲ��Ա�־")&&(All_Result[0]!=1))
				break;//����do while//�����ȫ��OK����д���ܲ��Ա�־

			Send_Serial_Order(&Serial_Order_Return,paraArray[i].Other_ITEM);
			CString Serial_Order_Return_CS_Show;
			if(Serial_Order_Return=="Analysis_SUCCESS")//���ݽ�������
				Serial_Order_Return_CS_Show=Vaule_Return_Count_CS;
			else if(((Serial_Order_Return=="FAIL")&&(paraArray[i].showName=="����")))//\
				||((paraArray[i].showName=="SOS����_INI_State")||(paraArray[i].showName=="SOS����")))//����ָ��ͺ���Ҫ�ȴ��Ż᷵��ֵ,���ǲ���SOS��������ֵΪATʱ
			{
				if(paraArray[i].showName=="����")
				{
					Vaule_Return_Count_CS="0";
					Serial_Order_Return_CS_Show="NULL";
				}
				Serial_Order_Return="Analysis_SUCCESS";
				
			}

			else
				Serial_Order_Return_CS_Show.Format("%s",Serial_Order_Return);
			LogShow_exchange(5,"���ڷ���ֵ:"+Serial_Order_Return_CS_Show,&BLE_Final_Result_Control,&BLE_m_Result,0);//�������ԣ�״̬������
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();

			bool Judge_Return;//�жϽ������ֵ��falseΪ������
			if(Serial_Order_Return!="FAIL")
			{
				//�ɹ������ݴ���
				if(Serial_Order_Return=="DELAY_SUCCESS")//��ʱ
				{
					All_Result[i]=1;
					break; //����do while 
				}
				if(Serial_Order_Return=="Analysis_SUCCESS")//���ݽ�������
				{
					Serial_Order_Return=(LPTSTR)(LPCTSTR)Vaule_Return_Count_CS;
					Judge_Return=BLE_Function_Judge(i,paraArray[i].showName,Serial_Order_Return);
				}
				//BLE_Operate_Guide_Control.SetWindowTextA("......");
				//BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				if(Judge_Return==true)
				{
					//������
					LogShow_exchange(5,"������Ŀ:"+paraArray[i].showName+"��꣡========������Գɹ�",&BLE_Final_Result_Control,&BLE_m_Result,0);//�������ԣ�״̬������
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
					All_Result[i]=1;
					break; //����do while
				}
				else
				{
					//ʧ�ܷ���
					Try_Time++;
					if((paraArray[0].ifCheck==true)&&(Try_Time>=atoi(paraArray[0].High_Limit_Value)))//ʧ����������
					{
						
						Send_Serial_Order(&Serial_Order_Return,"at+mfg=7");
						Send_Serial_Order(&Serial_Order_Return,"at+mfg=;");
						Send_Serial_Order(&Serial_Order_Return,"at+mfg==");
						Send_Serial_Order(&Serial_Order_Return,"at+mfg=0");
						PrintError("������Ŀ:"+paraArray[i].showName+"ʧ��,����ֹͣ������");
						OnBnClickedButton3();
						return FALSE; //��������ѭ��for
					}
					else
					{
						LogShow_exchange(5,"������Ŀ:"+paraArray[i].showName+"ʧ�ܣ����Ի��߼�����һ��Ŀ...",&BLE_Final_Result_Control,&BLE_m_Result,0);//�������ԣ�״̬������
						Log_Show_Global();
						BLE_m_Result.UpdateWindow();
						continue;//ѭ��do while
					}
				}
			}
			else
			{
				//ʧ�ܷ���
				Try_Time++;
				if((paraArray[0].ifCheck==true)&&(Try_Time>=atoi(paraArray[0].High_Limit_Value)))//ʧ����������
				{
					
					Send_Serial_Order(&Serial_Order_Return,"at+mfg=7");
					Send_Serial_Order(&Serial_Order_Return,"at+mfg=;");
					Send_Serial_Order(&Serial_Order_Return,"at+mfg==");
					Send_Serial_Order(&Serial_Order_Return,"at+mfg=0");
					PrintError("������Ŀ:"+paraArray[i].showName+"ʧ��,����ֹͣ������");
					OnBnClickedButton3();
					return FALSE; //��������ѭ��for
				}
				else
				{
					LogShow_exchange(5,"������Ŀ:"+paraArray[i].showName+"ʧ�ܣ����Ի��߼�����һ��Ŀ...",&BLE_Final_Result_Control,&BLE_m_Result,0);//�������ԣ�״̬������
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
					continue;//ѭ��do while
				}
			}
			
		}while(Try_Time<atoi(paraArray[0].High_Limit_Value));
		Try_Time=0;//������Դ�������
		BLE_Operate_Guide_Control.SetWindowTextA("......");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ

		All_Result[0]&=All_Result[i];
	}
	//delete SENSOR_Gravity;
	CString DB_FAIL="";
	if(All_Result[0]==1)
	{
		if(BLE_Setdlg.Initial_Connect_DB==TRUE)
		{
			CString Conn="";
			
			Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",BLE_Setdlg.m_server,BLE_Setdlg.m_db,BLE_Setdlg.m_user,BLE_Setdlg.m_pwd);//BLE
			if(BLE_Setdlg.myado.m_pCon==NULL)
			{
				DB_FAIL=BLE_Setdlg.myado.ConnecDataLibrary(Conn,"","",adModeUnknown);
			}
			if(BLE_Setdlg.myado.m_pCon->State==0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�Ͽ�������
			{
				LogShow_exchange(0,"=================================���ݿ�Ͽ�״̬����������........\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
				DB_FAIL=BLE_Setdlg.myado.ConnecDataLibrary(Conn,"","",adModeUnknown);//���ݿ�����Ͽ�������
			}
			else
			{
				LogShow_exchange(0,"=================================���ݿ�����������........\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
			}
			if(DB_FAIL=="FAIL")
			{
				PrintError("�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......");
				return FALSE;
			}

			//������ݿ��Ƿ���ڸ�BT-MAC��ַ,��ȥ��ͬ�����һ��
			if(BLE_MAC_Addr!="")
			{
				BLE_Setdlg.myado.OpenSheet("select BT_Addr from odm_check_test_BoardNum WHERE (BT_Addr ='"+BLE_MAC_Addr+"') AND (BoardNumber <> '"+SN_BLE+"')");
				BOOL BTMAC_Check_UP=BLE_Setdlg.myado.Find("BT_Addr='"+BLE_MAC_Addr+"'");
				BLE_Setdlg.myado.CloseSheet();
				if(BTMAC_Check_UP==TRUE)
				{
					PrintError("������ַ��"+BLE_MAC_Addr+"���ݿ��ظ�������");
					return FALSE;
				}
				else
				{
					LogShow_exchange(0,"������ַ��"+BLE_MAC_Addr+"���ݿ�����...",&BLE_Final_Result_Control,&BLE_m_Result,0);
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
				}
			}
			else
			{
				PrintError("������ַ��ȡʧ�ܣ�����");
				return FALSE;
			}
			//////////////////////////////////////////////////
			//������ݿ��Ƿ���ڸ�SN
			if(SN_BLE!="000000000000")
			{
				BLE_Setdlg.myado.OpenSheet("select BoardNumber from odm_check_test_BoardNum WHERE BoardNumber ='"+SN_BLE+"'");
				BOOL SN_Check_UP=BLE_Setdlg.myado.Find("BoardNumber='"+SN_BLE+"'");
				BLE_Setdlg.myado.CloseSheet();
			
				if(Cal_Sign!="1111")
				{
					PrintError("����û��У׼������ֻд��SN�š�");
					return FALSE;
				}
				else
				{
					LogShow_exchange(0,"������Ƶ��У׼...\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
				}

				CString Coupling_Sign_CS;
				Coupling_Sign_CS.Format("%d",Coupling_Sign);
				if(SN_Check_UP==TRUE)
				{
					if(paraArray[32].ifCheck==true)
					{
						LogShow_exchange(0,"���ݿ����SN�ţ�"+SN_BLE+"���¸�SN�Ŷ�Ӧ�����ݿ���Ϣ...",&BLE_Final_Result_Control,&BLE_m_Result,0);
						Log_Show_Global();
						BLE_m_Result.UpdateWindow();
						
						CString strSQL = "UPDATE odm_check_test_BoardNum SET BT_Addr = '"+BLE_MAC_Addr+"',CalibrMark = '"+Cal_Sign+"',CompreMark = '"+Coupling_Sign_CS+"'\
							 ,ordernum = '"+Function_Test_Order_Number+"',bversion = '"+Software_Version+"'\
							 ,Power_INI = '"+Power_INI+"',Power_Diff = '"+Power_Diff+"'\
							 ,Temperature = '"+Temperature+"',HeartRate = '"+HeartRate+"'\
							 ,Gravity = '"+SENSOR_Gravity_ALL+"' \
							 WHERE (BoardNumber = '"+SN_BLE+"') ";
						BLE_Setdlg.myado.OpenSheet("select * from odm_check_test_BoardNum");
						BOOL UP_FAIL=BLE_Setdlg.myado.Execute(strSQL);
						BLE_Setdlg.myado.CloseSheet();
						Sleep(500);
						BLE_Setdlg.myado.OpenSheet("select BoardNumber from odm_check_test_BoardNum WHERE BoardNumber ='"+SN_BLE+"'");
						BOOL ALL_Check_UP=BLE_Setdlg.myado.Find("BoardNumber='"+SN_BLE+"'");
						BLE_Setdlg.myado.CloseSheet();
						
						if((UP_FAIL==TRUE&&ALL_Check_UP==TRUE))
						{
							//m_Result.SetWindowText("�ɹ��������ݿ⣡����\r\n\r\n");
							PrintOK("�ɹ��������ݿ⣡����");
						}
						else
						{
							PrintError("�������ݿ�ʧ�ܣ�����������");
							return FALSE;
						}
					}
					else
					{
						PrintError("���ݿ�SN�ظ�������������");
						return FALSE;
					}
				}
				else
				{
					LogShow_exchange(0,"���ݿⲻ����SN�ţ�"+SN_BLE+"ֱ���ϴ�SN�Ŷ�Ӧ�����ݿ���Ϣ...",&BLE_Final_Result_Control,&BLE_m_Result,0);
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();

					CString strSQL = "Insert into odm_check_test_BoardNum(BoardNumber,BT_Addr,CalibrMark,CompreMark,ordernum,bversion,Power_INI,Power_Diff,Temperature,HeartRate,Gravity)\
						 values('"+SN_BLE+"','"+BLE_MAC_Addr+"','"+Cal_Sign+"','"+Coupling_Sign_CS+"','"+Function_Test_Order_Number+"','"+Software_Version+"','"+Power_INI+"','"+Power_Diff+"','"+Temperature+"','"+HeartRate+"','"+SENSOR_Gravity_ALL+"')";		
					BLE_Setdlg.myado.OpenSheet("select * from odm_check_test_BoardNum");
					BOOL UP_FAIL=BLE_Setdlg.myado.Execute(strSQL);
					BLE_Setdlg.myado.CloseSheet();
					Sleep(500);
					BLE_Setdlg.myado.OpenSheet("select BoardNumber from odm_check_test_BoardNum WHERE BoardNumber ='"+SN_BLE+"'");
					BOOL ALL_Check_UP=BLE_Setdlg.myado.Find("BoardNumber='"+SN_BLE+"'");
					BLE_Setdlg.myado.CloseSheet();
					
					if((UP_FAIL==TRUE&&ALL_Check_UP==TRUE))
					{
						PrintOK("�ɹ��ϴ����ݿ⣡����");
					}
					else
					{
						PrintError("�ϴ����ݿ�ʧ�ܣ�����������");
						return FALSE;
					}
				}
			}
			else
			{
				PrintError("CSRоƬû��SN�ţ���������Ϊ������ʧ����������");
				return FALSE;
			}

			//////////////////////////////////////////////////
			//////////////////////////////////////////////////
		}
		else
			PrintOK("��������Բ���OK����");
	}
	else
	{
		PrintError("��������Բ���FAIL������");
	}
	return TRUE;

}

void CCSR_Function_Test::LogShow_exchange(int State,CString Msg_Log,CEdit* Final_Result_Control,CEdit* m_Result,int WndChoose)     //����  �̱߳��� �ĺ���
{
	BLE_State=State;
	m_Result->GetWindowText(LOG_Show_Old);
	
	CTime cTime;
	cTime = CTime::GetCurrentTime();                                                         //�����Ե������ڵ��ļ�������������
	CString LogTime;
	LogTime.Format("    (%04d-%02d-%02d--%02d:%02d:%02d)",cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	if(WndChoose==0)
	{
		BLE_Msg_Log=LOG_Show_Old+"\r\n"+Msg_Log+LogTime;
		m_Result->SetWindowText(BLE_Msg_Log.Right(8092));
		BLE_m_Result.LineScroll(BLE_m_Result.GetLineCount());
	}
	else if(WndChoose==1)
	{
		if(Msg_Log.Find("���������豸�ɹ�")!=-1)
			BLE_Msg_Log=LOG_Show_Old+"\r\n"+Msg_Log+"\r\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n\r\n";
		else
			BLE_Msg_Log=LOG_Show_Old+"\r\n"+Msg_Log+"\r\n"+LogTime;
		m_Result->SetWindowText(BLE_Msg_Log.Right(1024));
		BLE_MAC_RSSI.LineScroll(BLE_MAC_RSSI.GetLineCount());
	}
	//m_Result->UpdateWindow();	
	if(State==0)
	{
		gColor = RGB(0,155,100);
		Final_Result_Control->SetWindowTextA("������");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==128)
	{
		gColor = RGB(255,0,0);
		Final_Result_Control->SetWindowTextA("����FAIL");
		Final_Result_Control->UpdateWindow(); 
	}
	else if(State==255)
	{
		gColor = RGB(0,255,0);
		Final_Result_Control->SetWindowTextA("����_OK");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==250)
	{
		gColor = RGB(60,190,190);
		Final_Result_Control->SetWindowTextA("IDLE");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==251)//У׼�ɹ�������۲�
	{
		gColor = RGB(0,155,100);
		Final_Result_Control->SetWindowTextA("������");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==5)//�������ԣ�״̬������
	{
		//gColor = RGB(0,155,100);
		//BLE_Final_Result_Control.SetWindowTextA("������");
		//BLE_Final_Result_Control.UpdateWindow();
		//RedrawWindow();
	}
	else
	{
		gColor = RGB(0,155,100);
		Final_Result_Control->SetWindowTextA("������");
		Final_Result_Control->UpdateWindow();
	}
	
}

void CCSR_Function_Test::Log_Show_Global(void)                          //����BLE  log��Ϣ�߳�
{
	//LogShow_Thread_Handle=AfxBeginThread(LogShow_Thread_Static_Global,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);//����1
}

UINT static __cdecl Measure_Function_BLE(LPVOID pParam)
{
	CCSR_Function_Test* Mead_Main_Win = (CCSR_Function_Test*)pParam;
	Mead_Main_Win->CSR_Function_Test_Thread();
	return 0;
}

void CCSR_Function_Test::OnBnClickedButton1()//��ʼ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Initial_Handle=true;
	Watch_Result=false;//ÿ�ο�ʼ��Ϊfalse
	//SENSOR_Gravity = new CStringArray[2];
	//SENSOR_Gravity->RemoveAll();
	SENSOR_Gravity[0][0]="";
	SENSOR_Gravity[1][0]="";
	SENSOR_Gravity[2][0]="";
	SENSOR_Gravity[0][1]="";
	SENSOR_Gravity[1][1]="";
	SENSOR_Gravity[2][1]="";
	SENSOR_Gravity_ALL="";
	Function_Test_Order_Number="";
	Software_Version="";
	BLE_MAC_Addr="";

	SN_BLE="";
	Cal_Sign="";
	Coupling_Sign=0;

	Power_INI=""; //��ʼ����
	Power_Diff="";//������
	//SENSOR_Gravity[3][2];
	Temperature="";
	HeartRate="";
	HeartRate_RL[0][0]="";
	HeartRate_RL[1][0]="";
	HeartRate_RL[0][1]="";
	HeartRate_RL[1][1]="";

	HeartRate_IL[0][0]="";
	HeartRate_IL[1][0]="";
	HeartRate_IL[0][1]="";
	HeartRate_IL[1][1]="";

	BLE_m_Result.SetWindowText("");
	BLE_m_Result.UpdateWindow();

	Measure_Thread_Handle=AfxBeginThread(Measure_Function_BLE,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL,0,0,NULL);
	gColor = RGB(0,155,100);
	BLE_Final_Result_Control.SetWindowTextA("������");
	BLE_Final_Result_Control.UpdateWindow();
	
	BLE_Operate_Guide_Control.SetWindowTextA("��������״̬...");
	BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ

	STOP_Test_Control.EnableWindow(TRUE);
	START_Test_Control.EnableWindow(FALSE);
	
	//RedrawWindow();
}

void CCSR_Function_Test::OnBnClickedButton2()//ֹͣ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Initial_Handle==true)
	{
		//BLE
		if(LogShow_Thread_Handle)			LogShow_Thread_Handle->SuspendThread();     //����LOG�߳�
		if(Measure_Thread_Handle)			Measure_Thread_Handle->SuspendThread();     //��������߳� ����

		if(LogShow_Thread_Handle)			TerminateThread(LogShow_Thread_Handle,2);
		if(Measure_Thread_Handle)			TerminateThread(Measure_Thread_Handle,3);
		if(LogShow_Thread_Handle)			TerminateThread(LogShow_Thread_Handle,4);
		if(Measure_Thread_Handle)			TerminateThread(Measure_Thread_Handle,5);
	}
	char* Serial_Order_Return;
	
	Send_Serial_Order(&Serial_Order_Return,"at+mfg=7");
	Send_Serial_Order(&Serial_Order_Return,"at+mfg=;");
	Send_Serial_Order(&Serial_Order_Return,"at+mfg==");
	Send_Serial_Order(&Serial_Order_Return,"at+mfg=0");
	PrintError("�������ԣ�����");
	BLE_Operate_Guide_Control.SetWindowTextA("Idle...");
	BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ

	STOP_Test_Control.EnableWindow(FALSE);
	START_Test_Control.EnableWindow(TRUE);
}

void CCSR_Function_Test::BLE_UI_LoadConfig()
{

    int nCommandNum   = 0;


    CString strCommand = _T("");
    CString strValue   = _T("");

    CString strName   = _T("");
    CStringArray strCommandAry;



         char szName[MAX_PATH] = _T("");
         CString  szPath       = _T("");
         GetModuleFileName(NULL, szName, MAX_PATH);
         szPath = szName;
         szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);

		 //CopyFile(szPath+"\\config\\limit.vbs","C:\\limit.vbs",FALSE);

		 CString strLoadFile=szPath+STRING_PATH_BLE;
		 CIniFile tmpread(strLoadFile);
		 CString fSectionName=tmpread.GetParaName();
 		 if(fSectionName=="BLE")
 		 {
			 // ����INI�ļ�
			ASSERT(!strLoadFile.IsEmpty());
			CIniFile m_fIniFile(strLoadFile);
			int i             = 0;
			char aText[140]={0};
			
			int nCommand      = 0;
			int nCommandNum   = 0;
			RVITEM rvi;

			CString strCommand = _T("");
			CString strValue        = _T("");//����������ֵ
			CString Check_item      = _T("");//�Ƿ�ѡ��
			CString strValue_low    = _T("");//��������ֵ
			CString strValue_high   = _T("");//��������ֵ
			CString strValue_OtherITEM   = _T("");//�����Զ���


			CStringArray strCommandAry;

			m_fIniFile.ReadSection(STRING_SECTION_BLE, strCommandAry);
			nCommandNum = strCommandAry.GetSize();

			if (nCommandNum == 0 )
			{
				CString strInfo;
				strInfo = _T("Imported script file don't have setting data!");
				AfxMessageBox(strInfo);
				return;
			}
			else
			{		
				for (i= 0; i< nCommandNum; i++)         //�������������
				{
					strCommand = strCommandAry[i];      //���
					paraArray[i].showName=strCommand;   //���������

					strValue   = m_fIniFile.ReadString(STRING_SECTION_BLE, strCommand, "");//��Ŷ�Ӧ������
					
					int npos=strValue.Find("@@");
					if(npos>=0)
					{
						Check_item  = strValue.Left(npos);
						strValue    = strValue.Mid(npos+2);
					}
					npos=strValue.Find("@@");
					if(npos>=0)
					{
						strValue_low  = strValue.Left(npos);
						strValue      = strValue.Mid(npos+2);
					}
					npos=strValue.Find("@@");
					if(npos>=0)
					{
						strValue_high  = strValue.Left(npos);
						strValue_OtherITEM = strValue.Mid(npos+2);
					}
					else
					{
						strValue_high  = strValue;
						strValue_OtherITEM="null";
					}

					paraArray[i].Low_Limit_Value = strValue_low;	//����������ֵ
					paraArray[i].High_Limit_Value =strValue_high;	//����������ֵ
					paraArray[i].Other_ITEM =strValue_OtherITEM;	//�����Զ���
					if(Check_item=="true")
						paraArray[i].ifCheck=true;           //ѡ��       
					else
						paraArray[i].ifCheck=false;          //ûѡ��       
						
				}
			}
		 }


         strLoadFile = szPath+STRING_PATH_CONFIG;
         CIniFile m_fIniFile(strLoadFile);

         m_fIniFile.ReadSection(STRING_SECTION_CONFIG, strCommandAry);
         nCommandNum = strCommandAry.GetSize();
		 CString INI_MSG_TEMP;
		 for(int i=0;i<nCommandNum;i++)
		 {
             strCommand = strCommandAry[i];//���
             if(strCommand == "Initial_Connect_DB_M")
             {
				 INI_MSG_TEMP= m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
				 if(INI_MSG_TEMP=="true")
					 BLE_Setdlg.Initial_Connect_DB=TRUE;
				 else
					 BLE_Setdlg.Initial_Connect_DB=FALSE;
             }
			 else if(strCommand == "m_server_M")
			 {
					   BLE_Setdlg.m_server = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }
			 else if(strCommand == "m_db_M")
			 {
					   BLE_Setdlg.m_db = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }
			 else if(strCommand == "m_user_M")
			 {
					   BLE_Setdlg.m_user = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }
			 else if(strCommand == "m_pwd_M")
			 {
			 		   BLE_Setdlg.m_pwd = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }

		 }
		 UpdateData(false);
	}


void CCSR_Function_Test::PrintError(CString cs)
{
	OnBnClickedButton3();
	LogShow_exchange(128,cs+"\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
	STOP_Test_Control.EnableWindow(FALSE);
	START_Test_Control.EnableWindow(TRUE);
	BLE_Operate_Guide_Control.SetWindowTextA("Idle...");
	BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
}

void CCSR_Function_Test::PrintOK(CString cs)
{
	OnBnClickedButton3();
	LogShow_exchange(255,cs+"\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
	STOP_Test_Control.EnableWindow(FALSE);
	START_Test_Control.EnableWindow(TRUE);
	BLE_Operate_Guide_Control.SetWindowTextA("Idle...");
	BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
}


UINT static __cdecl TIMER_DELAY(LPVOID pParam)     //��ʱ�߳�
{
	CCSR_Function_Test* TIMER_DELAY_Function=(CCSR_Function_Test*)pParam;

	TIMER_DELAY_Function->TimerID=TIMER_DELAY_Function->SetTimer(25,TIMER_DELAY_Function->Sleep_Idle_Time,NULL);
	return 0;
}
void CCSR_Function_Test::Delay_Clock(UINT mSecond)//��ʱ����ʱ
{
	if(TimerID!=NULL)	
		KillTimer(TimerID);
	
	RunCommandList.ResetEvent();//�ȴ�
	//AfxBeginThread(TIMER_DELAY,(LPVOID)this,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	WaitForSingleObject(RunCommandList.m_hObject,mSecond+500);
	//WaitForSingleObject(RunCommandList.m_hObject,INFINITE);
	Sleep(0);
	return;
}

void CCSR_Function_Test::OnTimer(UINT nIDEvent)//�����ֶ����ӣ���ʱ��
{
	switch(nIDEvent)
	{
	case 25:
		KillTimer(TimerID);
		AfxMessageBox("��ʱOK");
		RunCommandList.SetEvent();//����
		break;
	case 1:
			if(CSRBTHostApp.appCurState == APP_ERROR)
			{
				m_bStopFlag = TRUE;
				SetEvent(CSRBTHostApp.syncEvent);
				SetEvent(CSRBTHostApp.SearchDevEvent);
				SetEvent(CSRBTHostApp.ConnectDevEvent);
				SetEvent(CSRBTHostApp.GetInfEvent);
				SetEvent(CSRBTHostApp.WriteDataEvent);
				SetEvent(CSRBTHostApp.DisConnEvent);
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

int CCSR_Function_Test::CharToHexChar(char ch)
{
   if((ch>='0')&&(ch<='9'))
   {
       return ch-0x30;

   }
   else if((ch>='A')&&(ch<='F'))
   {
       return ch-'A'+10;
   }
   else if((ch>='a')&&(ch<='f'))
   {
       return ch-'a'+10;  
   }      
   else
   {
       return -1;
   }
}

bool CCSR_Function_Test::BLE_Function_Judge(int i,CString BLE_FT_Item,char* Serial_Order_Return)//�ж������Ƿ�����paraArray[i].showName
{
	int BLE_FT_Item_Int=0;
	CString Serial_Order_Return_CS;
	Serial_Order_Return_CS.Format("%s",Serial_Order_Return);

	int  WIFI_addr1[6];
	long WIFI_addr2=0;
	long WIFI_addr3=0;
	long WIFI_addr4=0;
	CString m_WIFI_Addr_Temp1,m_WIFI_Addr_Temp2;
	char m_WIFI_Addr_Temp3[5];

	

	if(BLE_FT_Item=="�����ܲ��Ա�־")
	{
		BLE_FT_Item_Int=1;
		Function_Test_Order_Number=paraArray[i].High_Limit_Value;
	}
	else if(BLE_FT_Item=="����汾")
	{
		BLE_FT_Item_Int=2;
		//Software_Version=paraArray[i].Low_Limit_Value;
		Software_Version=Serial_Order_Return_CS;
	}
	else if(BLE_FT_Item=="BT_MAC")
	{
		BLE_FT_Item_Int=3;
		BLE_MAC_Addr=Serial_Order_Return_CS.Mid(4);
		Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(4);
		for(int a=1;a<7;a++)                                   //BT MAC��ַ�Ľ���ֵ
		{
			m_WIFI_Addr_Temp1=paraArray[i].High_Limit_Value.Right(a);
			m_WIFI_Addr_Temp2=m_WIFI_Addr_Temp1.Left(1);
			//strncpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2,sizeof(m_WIFI_Addr_Temp2));		
			strcpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2);	
			WIFI_addr1[a-1]=CharToHexChar(m_WIFI_Addr_Temp3[0]);
			//WIFI_addr1[a-1]=atoi(m_WIFI_Addr_Temp3);
			WIFI_addr4+=WIFI_addr1[a-1]*(pow((double)16,(a-1)));//BT������ַ��6λ
		}

		for(int a=1;a<7;a++)                                  //BT MAC��ַ����ʼֵ
		{
			m_WIFI_Addr_Temp1=paraArray[i].Low_Limit_Value.Right(a);
			m_WIFI_Addr_Temp2=m_WIFI_Addr_Temp1.Left(1);
			//strncpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2,sizeof(m_WIFI_Addr_Temp2));		
			strcpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2);	
			WIFI_addr1[a-1]=CharToHexChar(m_WIFI_Addr_Temp3[0]);
			//WIFI_addr1[a-1]=atoi(m_WIFI_Addr_Temp3);
			WIFI_addr3+=WIFI_addr1[a-1]*(pow((double)16,(a-1)));
		}

		for(int a=1;a<7;a++)
		{
			m_WIFI_Addr_Temp1=Serial_Order_Return_CS.Right(a);
			m_WIFI_Addr_Temp2=m_WIFI_Addr_Temp1.Left(1);
			//strncpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2,sizeof(m_WIFI_Addr_Temp2));		
			strcpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2);	
			WIFI_addr1[a-1]=CharToHexChar(m_WIFI_Addr_Temp3[0]);
			//WIFI_addr1[a-1]=atoi(m_WIFI_Addr_Temp3);
			WIFI_addr2+=WIFI_addr1[a-1]*(pow((double)16,(a-1)));
		}
	}
	else if(BLE_FT_Item=="SN_��־λ")
	{
		BLE_FT_Item_Int=4;
		CString Serial_Order_Return_Temp;

		LimitOfTool=Serial_Order_Return_CS.Right(11);

		SN_BLE=Serial_Order_Return_CS.Left(12);
		Serial_Order_Return_Temp=Serial_Order_Return_CS.Mid(12);
		Cal_Sign=Serial_Order_Return_Temp.Left(4);

		Serial_Order_Return_Temp=Serial_Order_Return_Temp.Mid(4);
		Coupling_Sign=atoi(Serial_Order_Return_Temp.Left(4));
	}
	else if(BLE_FT_Item=="��ص���1")
	{
		BLE_FT_Item_Int=5;
	}
	else if(BLE_FT_Item=="������������")
		BLE_FT_Item_Int=21;
	else if(BLE_FT_Item=="����")
	{
		BLE_FT_Item_Int=6;
		SENSOR_Gravity_ALL=Serial_Order_Return_CS;
		int npos=Serial_Order_Return_CS.Find("gsensor_");
		Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(npos+8);
		for(int j=0;j<3;j++)
		{
			SENSOR_Gravity[j][1]=SENSOR_Gravity[j][0];
		}

		CString Serial_Order_Return_Temp;
		npos=Serial_Order_Return_CS.Find("_");
		if(npos>=0)
		{
			SENSOR_Gravity[0][0]=Serial_Order_Return_CS.Left(npos);
			Serial_Order_Return_Temp=Serial_Order_Return_CS.Mid(npos+1);

			npos=Serial_Order_Return_Temp.Find("_");
			if(npos>=0)
			{
				SENSOR_Gravity[1][0]=Serial_Order_Return_Temp.Left(npos);
				Serial_Order_Return_Temp=Serial_Order_Return_Temp.Mid(npos+1,3);
				SENSOR_Gravity[2][0]=Serial_Order_Return_Temp;

				LogShow_exchange(5,"����ֵ:"+SENSOR_Gravity[0][1]+"��"+SENSOR_Gravity[1][1]+"��"+SENSOR_Gravity[2][1]+"��"+SENSOR_Gravity[0][0]+"��"+SENSOR_Gravity[1][0]+"��"+SENSOR_Gravity[2][0],&BLE_Final_Result_Control,&BLE_m_Result,0);//�������ԣ�״̬������
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
			}
			else
			{
				//SENSOR_Gravity[1][0]="";
				//SENSOR_Gravity[2][0]="";
			}
		}
		else
		{
			//SENSOR_Gravity[0][0]="";//ֵ���ֲ���
			//SENSOR_Gravity[1][0]="";
			//SENSOR_Gravity[2][0]="";
		}

		BLE_Operate_Guide_Control.SetWindowTextA("����������ҡ�����..��׼������...");
		BLE_Operate_Guide_Control.UpdateWindow();				//�ı���������ɫ
		
		Delay_Clock(atol(paraArray[i-1].High_Limit_Value));		//�ȴ��������� ��ʱ��
	}
	else if(BLE_FT_Item=="�����жϿ�")
	{
		BLE_FT_Item_Int=19;
		BLE_Operate_Guide_Control.SetWindowTextA("���������жϣ�ҡ��������Ͽ���磩");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
	}
	else if(BLE_FT_Item=="�����жϹ�")
	{
		BLE_FT_Item_Int=20;
		BLE_Operate_Guide_Control.SetWindowTextA("�ر������жϣ�׼������...");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
	}
	else if(BLE_FT_Item=="������������")
		BLE_FT_Item_Int=22;
	else if(BLE_FT_Item=="�¶�")
	{
		BLE_FT_Item_Int=7;
		Temperature=Serial_Order_Return_CS;
	}
	else if(BLE_FT_Item=="����ֱ��У׼")
	{
		BLE_FT_Item_Int=23;
		HR_DC=Serial_Order_Return_CS;
		BLE_Operate_Guide_Control.SetWindowTextA("����ֱ��У׼�����ʵƲ�Ҫ���κ��ڵ�������");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
	}
	else if(BLE_FT_Item=="���ʺ��")
	{
		BLE_FT_Item_Int=17;
		HeartRate_RL[0][1]=HeartRate_RL[0][0];
		HeartRate_RL[1][1]=HeartRate_RL[1][0];

		int npos=Serial_Order_Return_CS.Find("adc_");
		Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(npos+4);
		npos=Serial_Order_Return_CS.Find("_");
		if(npos>=0)
		{
			HeartRate_RL[0][0]=Serial_Order_Return_CS.Left(npos);
			HeartRate_RL[1][0]=Serial_Order_Return_CS.Mid(npos+1,4);

			LogShow_exchange(5,"����ֵ:"+HeartRate_RL[0][1]+"��"+HeartRate_RL[1][1]+"��"+HeartRate_RL[0][0]+"��"+HeartRate_RL[1][0],&BLE_Final_Result_Control,&BLE_m_Result,0);//�������ԣ�״̬������
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}
		else
		{
			HeartRate_RL[0][0]=Serial_Order_Return_CS;//��һ��0��ʾΪ�������ڶ���0��ʾ��1������������ָ�����ֻ�н���  at+aio=0
			//HeartRate_RL[0][0]="";//ֵ���ֲ���
			//HeartRate_RL[1][0]="";
		}
		BLE_Operate_Guide_Control.SetWindowTextA("�������ʺ�ƣ�����  ��ָ  �ڵ���ɫ��...");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
	}
	else if(BLE_FT_Item=="���ʺ���")
	{
		BLE_FT_Item_Int=18;
		//HeartRate_IL[1]=HeartRate_IL[0];
		HeartRate_IL[0][1]=HeartRate_IL[0][0];
		HeartRate_IL[1][1]=HeartRate_IL[1][0];

		int npos=Serial_Order_Return_CS.Find("_");
		if(npos>=0)
		{
			HeartRate_IL[0][0]=Serial_Order_Return_CS.Left(npos);
			HeartRate_IL[1][0]=Serial_Order_Return_CS.Mid(npos+1);
		}
		else
		{
			HeartRate_IL[0][0]=Serial_Order_Return_CS;//��һ��0��ʾΪ��������ǰ�����ֱ�������ڶ���0��ʾ��1������������ָ�����ֻ��ֱ��  at+aio=1
			//HeartRate_IL[0][0]="";//ֵ���ֲ���
			//HeartRate_IL[1][0]="";
		}
		//BLE_Operate_Guide_Control.SetWindowTextA("�������ʺ���ƣ�������ڵ������...");
		//BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
	}
	else if(BLE_FT_Item=="����")
	{
		BLE_FT_Item_Int=8;
		HeartRate=Serial_Order_Return_CS;
		BLE_Operate_Guide_Control.SetWindowTextA("�������ʴ���������...");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
	}
	else if(BLE_FT_Item=="��ص���2")
	{
		BLE_FT_Item_Int=9;
		Power_Diff.Format("%f",atof(Serial_Order_Return_CS)-atof(Power_INI));
	}
	else if(BLE_FT_Item=="SOS����_INI_State")
	{
		BLE_FT_Item_Int=10;
		BLE_Operate_Guide_Control.SetWindowTextA("����SOS����..��ȷ�϶Ͽ� ��ӵ�Դ��");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		Delay_Clock(atol(paraArray[i].Low_Limit_Value));

	}
	else if(BLE_FT_Item=="SOS����")
	{
		BLE_FT_Item_Int=16;
		BLE_Operate_Guide_Control.SetWindowTextA("���� ������Ȼ���ɿ�..");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		Delay_Clock(atol(paraArray[i].Low_Limit_Value));
	}
	else if(BLE_FT_Item=="���")
	{
		BLE_FT_Item_Int=11;
		BLE_Operate_Guide_Control.SetWindowTextA("���������PASS.");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
	}
	else if(BLE_FT_Item=="LED��ON")
	{
		BLE_FT_Item_Int=12;
		BLE_Operate_Guide_Control.SetWindowTextA("��Ļȫ�ף���۲��Ƿ���Ļȫ�ף�");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
	}
	else if(BLE_FT_Item=="LED��OFF")
	{
		BLE_FT_Item_Int=13;
		BLE_Operate_Guide_Control.SetWindowTextA("��Ļȫ�ڣ���۲��Ƿ���Ļȫ�ڣ�");
		BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
	}
	else if(BLE_FT_Item=="д���ܲ��Ա�־")
		BLE_FT_Item_Int=14;
	else if(BLE_FT_Item=="�ָ���������")
		BLE_FT_Item_Int=15;

	else 
		BLE_FT_Item_Int=32;

	float Confirm=0.0;
	switch(BLE_FT_Item_Int)
	{
		case 1://�����ܲ��Ա�־
			if(atoi(Serial_Order_Return_CS)<(atoi(paraArray[i].High_Limit_Value)-1))//��������
				return false;
			break;
		case 2:
			if((Serial_Order_Return_CS.Find(paraArray[i].Low_Limit_Value))==-1)      //����汾
				return false;
			break;
		case 3:
			if((WIFI_addr2>WIFI_addr4)||(WIFI_addr2<WIFI_addr3))                     //������ַ
				return false;
			if(Serial_Order_Return_CS.GetLength() == 12)
			{
				for(int a=0;a<Serial_Order_Return_CS.GetLength();a++)
				{
					if((Serial_Order_Return_CS.GetAt(a) >='0' && Serial_Order_Return_CS.GetAt(a) <='9')  ||  (Serial_Order_Return_CS.GetAt(a) >='a' && Serial_Order_Return_CS.GetAt(a) <='f')||  (Serial_Order_Return_CS.GetAt(a) >='A' && Serial_Order_Return_CS.GetAt(a) <='F'))
					{					
					}
					else
					{
						return false;
					}
					
				}
			}
			else
				return false;
			break;
		case 4:                                                                    //SN��־λ
			if((SN_BLE=="000000000000")||(Cal_Sign=="0000")||(Coupling_Sign<(atoi(paraArray[i].High_Limit_Value)-1))||(LimitOfTool!=paraArray[i].Low_Limit_Value.Right(11)) )
				return false;
			break;
		case 5:                                                                    //��ص���1
			//if()
				//return false;
			Power_INI=Serial_Order_Return_CS;
			break;
		case 21: 
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//��ȷ
			{
				//������������
			}
			else
				return false;
			break;
		case 22: 
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//��ȷ
			{
				//������������
			}
			else
				return false;
			break;
		case 6:                                                                    //����
			//Confirm=atof((SENSOR_Gravity[0][0]));
			//Confirm=atof((SENSOR_Gravity[1][0]));
			//Confirm=atof((SENSOR_Gravity[2][0]));
			BLE_Operate_Guide_Control.SetWindowTextA("�������м����...");
			BLE_Operate_Guide_Control.UpdateWindow();
			if((SENSOR_Gravity[0][0]=="")||(SENSOR_Gravity[1][0]=="")||(SENSOR_Gravity[2][0]=="")\
				||(SENSOR_Gravity[0][1]=="")||(SENSOR_Gravity[1][1]=="")||(SENSOR_Gravity[2][1]=="")\
				||(abs(atof((SENSOR_Gravity[0][1]))-atof((SENSOR_Gravity[0][0])))<atof(paraArray[i].Low_Limit_Value))\
				||(abs(atof((SENSOR_Gravity[1][1]))-atof((SENSOR_Gravity[1][0])))<atof(paraArray[i].Low_Limit_Value))\
				||(abs(atof((SENSOR_Gravity[2][1]))-atof((SENSOR_Gravity[2][0])))<atof(paraArray[i].Low_Limit_Value)) )
				return false;
			break;
		case 19:
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//��ȷ
			{
			}
			else
			{
				BLE_Operate_Guide_Control.SetWindowTextA("�����жϿ����ԣ���Ͽ�USB��Դ��");
				BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				Delay_Clock(atol(paraArray[i].Low_Limit_Value));//�ȴ��ƶ��ж�
				return false;
			}
			break;
		case 20:
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//��ȷ
			{
				//BLE_Operate_Guide_Control.SetWindowTextA("..");
				//BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				//Delay_Clock(atol(paraArray[i].Low_Limit_Value));//�ȴ��ƶ��ж�
			}
			else
				return false;
			break;
		case 7:                                                                    //�¶�
			if((atof(Serial_Order_Return_CS)>atof(paraArray[i].High_Limit_Value))||(atof(Serial_Order_Return_CS)<atof(paraArray[i].Low_Limit_Value)))
				return false;
			break;
		case 8:                                                                    //����
			BLE_Operate_Guide_Control.SetWindowTextA("���ʼ����...");
			BLE_Operate_Guide_Control.UpdateWindow();
			
			if((atoi(Serial_Order_Return_CS)>atoi(paraArray[i].High_Limit_Value))||(atoi(Serial_Order_Return_CS)<atoi(paraArray[i].Low_Limit_Value)))
			{
				Delay_Clock(atol(paraArray[i+1].High_Limit_Value));//�ȴ����ʲ��� ��ʱ��
				return false;
			}
			break;
		case 23:
			if((atof(Serial_Order_Return_CS)>atof(paraArray[i].High_Limit_Value))||(atof(Serial_Order_Return_CS)<atof(paraArray[i].Low_Limit_Value)))
				return false;
			break;
		case 17:                                                                    //���ʺ��
			/*if(((atol(HeartRate_RL[0][0])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_RL[1][0])>atol(paraArray[i].High_Limit_Value))\
				||(atol(HeartRate_RL[0][1])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_RL[1][1])<atol(paraArray[i+1].High_Limit_Value)))\
				&&\
				((atol(HeartRate_RL[0][0])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_RL[1][0])<atol(paraArray[i+1].High_Limit_Value))\
				||(atol(HeartRate_RL[0][1])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_RL[1][1])>atol(paraArray[i].High_Limit_Value)))\
				)*/
			  //�����ڵ���Ȼ��û���ڵ�||
			  //��û�е��ڣ�Ȼ��û���ڵ�
			//if( ((atol(HeartRate_RL[0][0])>atol(paraArray[i].Low_Limit_Value))  &&(atol(HeartRate_RL[1][0])<atol(paraArray[i].High_Limit_Value))) \
				&&((atol(HeartRate_RL[0][1])<atol(paraArray[i+1].Low_Limit_Value))&&(atol(HeartRate_RL[1][1])>atol(paraArray[i+1].High_Limit_Value))) \
				|| \
				((atol(HeartRate_RL[0][1])>atol(paraArray[i].Low_Limit_Value))  &&(atol(HeartRate_RL[1][1])<atol(paraArray[i].High_Limit_Value))) \
				&&((atol(HeartRate_RL[0][0])<atol(paraArray[i+1].Low_Limit_Value))&&(atol(HeartRate_RL[1][0])>atol(paraArray[i+1].High_Limit_Value))) \
				)
			long test1,test2,T1,T2;
			test1=abs(atol(HeartRate_RL[0][0]) -atol(HeartRate_RL[0][1]));
			T1=atol(paraArray[i].Low_Limit_Value);

			test2=abs(atol(HeartRate_RL[1][0]) -atol(HeartRate_RL[1][1]));
			T2=atol(paraArray[i].High_Limit_Value);
			if( ( ( abs(atol(HeartRate_RL[0][0]) -atol(HeartRate_RL[0][1])) >=atol(paraArray[i].Low_Limit_Value) )  \
				&& ((HeartRate_RL[0][0]!="")&& (HeartRate_RL[0][1]!="")) )				 &&              
				( abs(atol(HeartRate_RL[1][0]) -atol(HeartRate_RL[1][1])) >=atol(paraArray[i].High_Limit_Value) ) \
				&& ((HeartRate_RL[1][0]!="")&& (HeartRate_RL[1][1]!=""))    //������ֱ���仯ֵ
			  )
			{

				BLE_Operate_Guide_Control.SetWindowTextA("���ʺ�ɫ��OK���ɿ��ڵ�");
				BLE_Operate_Guide_Control.UpdateWindow();
				Delay_Clock(500);//

			}
			else
			{
				Delay_Clock(atol(paraArray[i-1].High_Limit_Value));//�ȴ����ʺ�Ʋ���ʱ��
				return false;
			}
			break;
		case 18:                                                                    //���ʺ���
			/*if(((atol(HeartRate_IL[0][0])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_IL[1][0])>atol(paraArray[i].High_Limit_Value))\
				||(atol(HeartRate_IL[0][1])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_IL[1][1])<atol(paraArray[i+1].High_Limit_Value)))\
				&&\
				((atol(HeartRate_IL[0][0])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_IL[1][0])<atol(paraArray[i+1].High_Limit_Value))\
				||(atol(HeartRate_IL[0][1])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_IL[1][1])>atol(paraArray[i].High_Limit_Value)))\
				)
			{
				BLE_Operate_Guide_Control.SetWindowTextA("�ɿ��ڵ�.");
				BLE_Operate_Guide_Control.UpdateWindow();

				Delay_Clock(atol(paraArray[i-2].High_Limit_Value));//�ȴ����ʺ�Ʋ���ʱ��
				return false;
			}
			else
			{
				BLE_Operate_Guide_Control.SetWindowTextA("���ʺ����OK");
				BLE_Operate_Guide_Control.UpdateWindow();
				Delay_Clock(500);//
			}*/
			//�����ڵ���Ȼ��û���ڵ�||
			//��û�е��ڣ�Ȼ��û���ڵ�
			//if( ((atol(HeartRate_IL[0][0])>atol(paraArray[i-1].Low_Limit_Value))  &&(atol(HeartRate_IL[1][0])<atol(paraArray[i-1].High_Limit_Value))) \
				&&((atol(HeartRate_IL[0][1])<atol(paraArray[i].Low_Limit_Value))&&(atol(HeartRate_IL[1][1])>atol(paraArray[i].High_Limit_Value))) \
				|| \
				((atol(HeartRate_IL[0][1])>atol(paraArray[i-1].Low_Limit_Value))  &&(atol(HeartRate_IL[1][1])<atol(paraArray[i-1].High_Limit_Value))) \
				&&((atol(HeartRate_IL[0][0])<atol(paraArray[i].Low_Limit_Value))&&(atol(HeartRate_IL[1][0])>atol(paraArray[i].High_Limit_Value))) \
			  )
			/*if( ( abs(atol(HeartRate_IL[0][0]) -atol(HeartRate_IL[0][1])) >=atol(paraArray[i-1].High_Limit_Value) ) \
				&& ((HeartRate_IL[0][0]!="")&& (HeartRate_IL[0][1]!=""))) //ֱ���仯ֵ����
			{

				BLE_Operate_Guide_Control.SetWindowTextA("���ʺ����OK.");
				BLE_Operate_Guide_Control.UpdateWindow();
				Delay_Clock(500);//
				Send_Serial_Order(&Serial_Order_Return,"at+hrpower=0");
				Send_Serial_Order(&Serial_Order_Return,"at+hrpower=0");

				
			}
			else
			{
				Delay_Clock(atol(paraArray[i-2].High_Limit_Value));//�ȴ����ʺ�Ʋ���ʱ��
				BLE_Operate_Guide_Control.SetWindowTextA("�ɿ��ڵ�.");
				BLE_Operate_Guide_Control.UpdateWindow();
				return false;
			}*/
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)
			{
				Delay_Clock(500);//
			}
			else
				return false;
			break;
		case 9:                                                                    //������
			BLE_Operate_Guide_Control.SetWindowTextA("��������ɣ��ε��ⲿ��Դ�����Ե��...");
			BLE_Operate_Guide_Control.UpdateWindow();
			Delay_Clock(atol(paraArray[i+1].High_Limit_Value));//��ʱ��ʾ���Ե��
			if((atof(Serial_Order_Return_CS)-atof(Power_INI))<atof(paraArray[i].Low_Limit_Value))
				return false;
			break;
		case 10:                                                                   //SOS������ʼ״̬
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)
			{
				//BLE_Operate_Guide_Control.SetWindowTextA("..");
				//BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				//Delay_Clock(2000);//�ȴ���ʾ�ɿ�SOS����
			}
			else
				return false;
			break;

		case 16:                                                                   //SOS����SOS_LONG_PRESS
			if(((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)||((Serial_Order_Return_CS.Find("key_2"))!=-1))
			{
				BLE_Operate_Guide_Control.SetWindowTextA("SOS�����������");
				BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				Delay_Clock(100);//�ȴ���ʾ�ɿ�SOS����
			}
			else
				return false;
			break;

		case 11:                                                                   //���--����������������Ӧ�ж�
			if((Watch_Result==false)&&(paraArray[i].Low_Limit_Value=="true"))
				return false;
			else
				Watch_Result=false;//�������OK������־����
			break;
		case 12:                                                                   //LED��ON
			if(Watch_Result==false)
			{
				BLE_Operate_Guide_Control.SetWindowTextA("��Ļȫ������PASS!");
				BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
				return false;
			}
			else
				Watch_Result=false;//�������OK������־����
			break;
		case 13:                                                                   //LED��OFF
			if(Watch_Result==false)
			{
				BLE_Operate_Guide_Control.SetWindowTextA("��Ļȫ������PASS!");
				BLE_Operate_Guide_Control.UpdateWindow();//�ı���������ɫ
				Delay_Clock(atol(paraArray[i].High_Limit_Value));//�ȴ������ж� ��ʱ��
				return false;
			}
			else
				Watch_Result=false;//�������OK������־����
			break;
		case 14:                                                                   //д���ܲ��Ա�־
		
			break;
		case 15:                                                                   //�ָ���������
			if(Serial_Order_Return_CS==paraArray[i].High_Limit_Value)
			{
			}
			else
				return false;
			break;
		default :
				return true;
			break;
	}
	return true;
}
void CCSR_Function_Test::OnBnClickedButton3()//�رմ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL CloseHandle_Return;
	if((hPort != INVALID_HANDLE_VALUE)&&(hPort != NULL))
	{
		CloseHandle_Return=CloseHandle(hPort);
		if(CloseHandle_Return==TRUE)
			//AfxMessageBox("�رոô��ڳɹ���");
			;
		else
			;
			//AfxMessageBox("�رոô���ʧ�ܣ���");

	}
	else
	{
		AfxMessageBox("�ô���û�д򿪡�");
	}
}

void CCSR_Function_Test::OnBnClickedButton4()//PASS
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Watch_Result=true;//�ɹ�Ϊtrue
	RunCommandList.SetEvent();
}

void CCSR_Function_Test::OnBnClickedButton11()//FAIL
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Watch_Result=false;//ʧ��Ϊfalse
	RunCommandList.SetEvent();
}



UINT static __cdecl ThreadTestCSR(LPVOID pParam)
{
	CCSR_Function_Test* HostControl = (CCSR_Function_Test*)pParam;
	HostControl->StartSearch();
	return 0;
}
void CCSR_Function_Test::OnBnClickedButton5()
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	CSRBTHostApp.IniData();
	m_bStopFlag=FALSE;
	DeviceCountM=paraArray[3].High_Limit_Value;
	Test_Handle=AfxBeginThread(ThreadTestCSR,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);	
}
static int ReIniCount;
BOOL CCSR_Function_Test::StartSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(ReIniCount==0)
	{
		LogShow_exchange(0,"��ʼ��CSR-USB�����豸...",&BLE_Final_Result_Control,&BLE_m_Result,0);
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
		if(CSRBTHostApp.IniFunction()==-1)				//��ʼ��������
		{
			CSRBTHostApp.shutDown = TRUE;
			TerminateThread(CSRBTHostApp.Thread_Handle, -1);
			CSRBTHostApp.EndFunction();
			LogShow_exchange(128,"��ʼ��USB�����豸ʧ�ܣ�",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
			GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
			return FALSE;
		}
		else
		{
			ReIniCount=1;
			LogShow_exchange(5,"��ʼ��USB�����豸�ɹ�,��ʼ���������豸...",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}	
	}
	else
	{
		ReIniCount=2;
		LogShow_exchange(5,"USB�����豸�Ѿ�������ʼ������,��ʼ���������豸...",&BLE_Final_Result_Control,&BLE_m_Result,0);
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
	}
	START_Test_Control.EnableWindow(TRUE);

	while(m_bStopFlag==FALSE)
	{
		ResetEvent(CSRBTHostApp.syncEvent);
		ResetEvent(CSRBTHostApp.SearchDevEvent);
		ResetEvent(CSRBTHostApp.ConnectDevEvent);
		ResetEvent(CSRBTHostApp.GetInfEvent);
		ResetEvent(CSRBTHostApp.WriteDataEvent);
		ResetEvent(CSRBTHostApp.DisConnEvent);
		LogShow_exchange(5,"���������豸...",&BLE_Final_Result_Control,&BLE_m_Result,0);
		if(CSRBTHostApp.DiscovertDevice()==FALSE)
		{
			LogShow_exchange(5,"���������豸ʧ�ܣ�",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
			Sleep(1000);
			continue;
		}
		else
		{
			//��ʾ����������Ϣ
			CString RSSI_CS[MIN_DISCOVERED_DEVICES];
			CString BTMacALL[MIN_DISCOVERED_DEVICES];
			CString BTDeviceName[MIN_DISCOVERED_DEVICES];

			CString RSSI_CSTemp[MIN_DISCOVERED_DEVICES];
			CString BTMacALLTemp[MIN_DISCOVERED_DEVICES];
			CString BTDeviceNameTemp[MIN_DISCOVERED_DEVICES];
			
			for(int j=0;j<CSRBTHostApp.nDevicesDiscovered;j++)
			{
				RSSI_CS[j].Format("%d",CSRBTHostApp.RSSI[j]);
				BTMacALL[j].Format("%04X:%02X:%06X",CSRBTHostApp.devicesDiscovered[j].nAp,CSRBTHostApp.devicesDiscovered[j].uAp,CSRBTHostApp.devicesDiscovered[j].lAp);
				BTDeviceName[j].Format("%-10s",CSRBTHostApp.DeviceName[j]);
			}
			LogShow_exchange(5,"��ǿ��������...",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
			for(int j=0;j<CSRBTHostApp.nDevicesDiscovered;j++)
			{
				for(int i=j+1;i<CSRBTHostApp.nDevicesDiscovered;i++)
				{
					if(atoi(RSSI_CS[j])<atoi(RSSI_CS[i]))
					{
						RSSI_CSTemp[j]=RSSI_CS[j];
						BTMacALLTemp[j]=BTMacALL[j];
						BTDeviceNameTemp[j]=BTDeviceName[j];

						RSSI_CS[j]=RSSI_CS[i];
						BTMacALL[j]=BTMacALL[i];
						BTDeviceName[j]=BTDeviceName[i];

						RSSI_CS[i]     =RSSI_CSTemp[j];
						BTMacALL[i]    =BTMacALLTemp[j];
						BTDeviceName[i]=BTDeviceNameTemp[j];
					}
				}
			}
			BOOL OnceExe[2];
			OnceExe[0]=FALSE;
			OnceExe[1]=FALSE;
			for(int j=0;j<CSRBTHostApp.nDevicesDiscovered;j++)
			{
				if((atoi(paraArray[4].Low_Limit_Value)<=atoi(RSSI_CS[j]))&&(atoi(paraArray[4].High_Limit_Value)>=atoi(RSSI_CS[j])))
				{
					if(OnceExe[0]==FALSE)
					{
						OnceExe[0]=TRUE;
						LogShow_exchange(5,"=================><RSSI����豸>OKOK",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
					}
					LogShow_exchange(5,"Name:"+BTDeviceName[j]+"   MACAddr:"+BTMacALL[j]+"   rssi:"+RSSI_CS[j],&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
				}
				else
				{
					if(OnceExe[1]==FALSE)
					{
						OnceExe[1]=TRUE;
						LogShow_exchange(5,"=================><RSSI�쳣�豸>������",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
					}
					LogShow_exchange(5,"Name:"+BTDeviceName[j]+"   MACAddr:"+BTMacALL[j]+"   rssi:"+RSSI_CS[j],&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
				}
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
			}
			//Sleep(2000);
			LogShow_exchange(5,"���������豸�ɹ�...",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}
	}
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);	
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	START_Test_Control.EnableWindow(FALSE);
	LogShow_exchange(5,"���������Ѿ�����������",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
	return TRUE;
}
void CCSR_Function_Test::OnBnClickedButton36()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bStopFlag=TRUE;
	LogShow_exchange(5,"��ȴ��ϴ���������...",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
}

void CCSR_Function_Test::OnDestroy()
{
	CDialog::OnDestroy();	
}
void CCSR_Function_Test::OnClose()
{
	/*if(Test_Handle!=NULL)
	{
		if(WaitForSingleObject(Test_Handle->m_hThread, 5000) == WAIT_TIMEOUT)				//����
		{
			TerminateThread(Test_Handle->m_hThread, -1);
		}
		Test_Handle=NULL;
	}
	OnCancel();*/
}
void CCSR_Function_Test::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*OnClose();
	OnCancel();*/
}

void CCSR_Function_Test::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BLE_UI_LoadConfig();
	LogShow_exchange(5,"��ȡ�ɹ�",&BLE_Final_Result_Control,&BLE_m_Result,0);
	//OnOK();
}
