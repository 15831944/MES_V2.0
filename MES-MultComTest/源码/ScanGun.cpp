BOOL IMEIWrite_MulAT::OPen_ScanGun_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum)		//��ɨ��ǹ�˿�
{
	CString sPort,sBaud;
	int port,baud;
	//get port
	m_Port->GetWindowText(sPort);
	sPort=sPort.Right(sPort.GetLength()-3);
	port=atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"),port);

	//get baud
	m_Baud->GetWindowText(sBaud);
	baud=atoi(sBaud);

																								//open com port�򿪶˿�
	hScanGun[HandleNum]=CreateFile(sPort, GENERIC_READ|GENERIC_WRITE,0, NULL, 
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if(hScanGun[HandleNum] == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port->GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hScanGun[HandleNum]=NULL;
		return FALSE;
	}

	//config the com port
	DCB dcb;
   	dcb.DCBlength = sizeof(DCB);
	GetCommState( hScanGun[HandleNum], &dcb ) ;
	SetupComm( hScanGun[HandleNum], 4096, 4096 ) ;
	PurgeComm(hScanGun[HandleNum],PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.BaudRate = baud;															//57600(MT6205B), 115200 (MT6218B)			
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
	SetCommState(hScanGun[HandleNum], &dcb);

	//set time out struct
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hScanGun[HandleNum], &timeouts);

	//SetCommMask(hScanGun[HandleNum], EV_RXCHAR|EV_TXEMPTY );							//�����¼�����������

	//read and the com port
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char *buffer;
																					//make sure rx data cleaned
	Sleep(100);
	bReadStatus = ReadFile( hScanGun[HandleNum], buffer, 256, &dwBytesRead, NULL);

	return TRUE;																	//����TRUE���򿪴�������
}


BOOL IMEIWrite_MulAT::Get_ScanGun_Data(int HandleNum,char* EndSign)		//��ȡɨ��ǹ�˿�����
{
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char *buffer;
	//read and the com port
	CString Port_Temp="";//���ڶ�ȡ���� �Ļ���
	for(int r=0;r<8;r++)
	{	
		memset(buf,0,sizeof(buf));
		buffer=buf;
		bReadStatus = ReadFile( hScanGun[HandleNum], buffer, 512, &dwBytesRead, NULL);
		if(dwBytesRead != 0)
		{
			p=strstr(buffer,EndSign);
			if(p)
			{
				 CString strCommand_Vaule_Return;//���ڷ���ֵ
				 strCommand_Vaule_Return=Port_Temp+CString(buffer);
				
				 CString  selPort;
				 selPort=strCommand_Vaule_Return;
				 selPort.Replace("\r","");
				 selPort.Replace("\n","");
				 //��ȡ�����ĸ���
				 CString string_sn_r;
				 int npos=selPort.Find('"');
				 if(npos>=0)
				 {
					string_sn_r=selPort.Mid(npos+1);
					Vaule_ScanGun[HandleNum]=string_sn_r.Left(string_sn_r.Find('"'));
				 }
				 else
				 {
					Vaule_ScanGun[HandleNum]=selPort;
				 }
			}
			else
			{
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



BOOL IMEIWrite_MulAT::Check_ScanGun_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,CEdit* Data_Input_Control,BOOL ImeiChoose,int LengthLimit)
{
	if(ImeiChoose==TRUE)
	{
		int IMEI_length;
		CString ScanGunData;
		ScanGunData=Vaule_ScanGun[HandleNum];
		IMEI_length=ScanGunData.GetLength();
		for(int i=0;i<IMEI_length;i++)
		{
			if(IMEI_length!= 15||(ScanGunData.GetAt(i) <'0' || ScanGunData.GetAt(i) >'9'))
			{
				LogShow_exchange(m_Result,Final_Result_Control,128,"Please check IMEI_Input. Only 15 digits allowed!\r\n",HandleNum);
				return FALSE;
			}
			
		}
		if(CheckIMEI(ScanGunData)==FALSE)
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"IMEI_Input ���Ϸ�",HandleNum);
			return FALSE;
		}
		//Data_Input_Control->SetWindowTextA(ScanGunData);
		IMEI_Input[HandleNum]=ScanGunData;
		LogShow_exchange(m_Result,Final_Result_Control,0,"ɨ������IMEI:"+IMEI_Input[HandleNum]+"����...\r\n",HandleNum);
	}
	else
	{
		int IMEI_length;
		CString ScanGunData;
		ScanGunData=Vaule_ScanGun[HandleNum];
		IMEI_length=ScanGunData.GetLength();
		if(IMEI_length!= LengthLimit)
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"����ɨ�����������,λ��������",HandleNum);
			return FALSE;
		}
		////////////////////�������ж�///////////////////
		//Data_Input_Control->SetWindowTextA(ScanGunData);
		OtherData_Input[HandleNum]=ScanGunData;
		LogShow_exchange(m_Result,Final_Result_Control,0,"ɨ����������:"+OtherData_Input[HandleNum]+"λ������...\r\n",HandleNum);
	}
}

void IMEIWrite_MulAT::OnBnClickedButton3()//��ɨ��ǹ1
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sPort;
	m_Port1.GetWindowText(sPort);
	if(sPort=="")
	{
		AfxMessageBox("����ѡ��AT���ڶ˿ڣ�");
		return;
	}

	if(OPen_ScanGun_Port(&ScanGunPort1,&ScanGunBaud1,0)==FALSE)
	{
		AfxMessageBox("��ɨ��ǹ�˿�ʧ�ܣ�");
		return;
	}
	else
	{
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	}

	SetTimer(10,1000,NULL);
}
//�����̺߳����Ͷ�ʱ������

void IMEIWrite_MulAT::OnBnClickedButton25()//�˳�ʱҲҪ�ر�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(hScanGun[0]!=NULL)
	{
		BOOL Return=CloseHandle(hScanGun[0]);				//���Գɹ���
		if(Return==TRUE)
			LogShow_exchange(&m_Result1,&Final_Result_Control1,250,"�رոô��ڳɹ�����",0);
		else
		{
			LogShow_exchange(&m_Result1,&Final_Result_Control1,128,"�رոô���ʧ�ܣ���",0);
															//�رմ���ʧ��
		}
		hScanGun[0]=NULL;
	}
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
}