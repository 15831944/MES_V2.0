//���������ݿ�Data_UpdatePara
BOOL IMEIWrite_MulAT::Data_UpdatePara(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control)
{
	_variant_t var;
	CString DB_FAIL;
	if(IMEI_Setdlg.Initial_Connect_DB==TRUE)//�����ʼ���������ݿ⣬��Ȼ������Чָ�롰
	{
		CString Conn="";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",IMEI_Setdlg.m_server,IMEI_Setdlg.m_db,IMEI_Setdlg.m_user,IMEI_Setdlg.m_pwd);

		if(myado.m_pCon==NULL)
		{
			CoInitialize(NULL); 
			DB_FAIL=myado.ConnecDataLibrary(Conn,"","",adModeUnknown);   
		}
		if(myado.m_pCon->State==0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result,Final_Result_Control,5,"�����������ݿ�.......",DataUpNum);
			DB_FAIL=myado.ConnecDataLibrary(Conn,"","",adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"���ݿ�����������...",DataUpNum);
		}
		if(DB_FAIL=="FAIL")
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......",DataUpNum,"���ܲ���:3-DB");
			return FALSE;
		}

		if((ChipRfID[DataUpNum]=="")||(Software_Version[DataUpNum]==""))
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"SN��������汾Ϊ��",DataUpNum);
			return FALSE;
		}
	
		///////////////////////////////////////////////////////////
		//�ϴ�����λ
		
		LogShow_exchange(m_Result,Final_Result_Control,0,"�鿴���ܱ�...",DataUpNum);

		myado.OpenSheet("select SN,Result,SoftModel,Version from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//��1
		BOOL Barcode_Check=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
		Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
		Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
		//Barcode_Check&=myado.Find("Result='1'");
		myado.CloseSheet();
		if(Barcode_Check==TRUE)
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"���ݿ��Ѿ��й��ܲ��Լ�¼������...",DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"���ݿⲻ���ڣ�ֱ���ϴ�...",DataUpNum);
		}
		
		BOOL UP_Barcode=TRUE;
		BOOL Barcode_Check_UP;
		if(Barcode_Check==TRUE)
		{
			
			//����--��ԭʼϵͳ���Թ�
			CString strSQL_Write_Barcode;
			myado.OpenSheet("select * from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//��1

			if(1)
			{
				strSQL_Write_Barcode="UPDATE dbo.Gps_AutoTest_Result SET Result='1' WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'";//��2
				UP_Barcode=myado.Execute(strSQL_Write_Barcode,&var);
			}
			myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_AutoTest_Result(SN,IMEI,Version,SoftModel,Result,Remark,TesterId)\
							 values('"+ChipRfID[DataUpNum]+"','','"+Software_Version[DataUpNum]+"','"+MachineType_CS+"','1','gps','lbc')";//��3
				
				
			myado.OpenSheet("select * from dbo.Gps_AutoTest_Result");
			UP_Barcode=myado.Execute(strSQL1,&var);
			myado.CloseSheet();
		}
		if(UP_Barcode==TRUE)
		{	
			//�ٴμ��
			Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,Result from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' AND Result ='1'");//��3
			Barcode_Check_UP=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("Result='1'");//��1
			myado.CloseSheet();
			//if(var.intVal!=1)
				//Barcode_Check_UP=FALSE;
			//�ٴμ��
		}	

		if((UP_Barcode==TRUE&&Barcode_Check_UP==TRUE))
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"SN:"+ChipRfID[DataUpNum]+"���ݿ��ϴ�����λ�ɹ�...",DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"SN:"+ChipRfID[DataUpNum]+"���ݿ��ϴ�����λʧ��",DataUpNum,"���ܲ���:3-DB");
			return FALSE;
		}

		//�ϴ�����λ
		///////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LogShow_exchange(m_Result,Final_Result_Control,0,"�鿴�����ܱ�...",DataUpNum);
		////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
		myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
		Barcode_Check=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
		Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
		Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
		Barcode_Check&=myado.Find("AutoTestResult='1'");
		myado.CloseSheet();
		if(Barcode_Check==TRUE)
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�������������������������������������������������������������...",DataUpNum);
			return TRUE;//��2
		}
		else
		{
			myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
			Barcode_Check=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
			Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			myado.CloseSheet();
			if(Barcode_Check==TRUE)
			{
				LogShow_exchange(m_Result,Final_Result_Control,0,"���ݿ��Ѿ��й��ܲ��Լ�¼������...",DataUpNum);
			}
			else
			{
				LogShow_exchange(m_Result,Final_Result_Control,0,"���ݿⲻ���ڣ�ֱ���ϴ�...",DataUpNum);
			}
		}
		////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
		UP_Barcode=TRUE;
		if(Barcode_Check==TRUE)
		{
			//����
			CString strSQL_Write_Barcode;
			myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

			if(1)
			{
				strSQL_Write_Barcode="UPDATE dbo.Gps_TestResult SET AutoTestResult = '1' WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'";
				UP_Barcode=myado.Execute(strSQL_Write_Barcode,&var);
			}
			myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,AutoTestResult)\
							 values('"+ChipRfID[DataUpNum]+"','','"+Software_Version[DataUpNum]+"','"+MachineType_CS+"','1')";	
				
				
			myado.OpenSheet("select * from dbo.Gps_TestResult");
			UP_Barcode=myado.Execute(strSQL1,&var);
			myado.CloseSheet();
		}
		if(UP_Barcode==TRUE)
		{
			//�ٴμ��
			Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,CoupleResult,WriteImeiResult,ParamDownloadResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' ");
			Barcode_Check_UP=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("AutoTestResult='1'");
			myado.CloseSheet();
			//�ٴμ��
		}	


		if((UP_Barcode==TRUE&&Barcode_Check_UP==TRUE))
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"SN:"+ChipRfID[DataUpNum]+"���ݿ��ϴ��ܱ�ɹ�...",DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"SN:"+ChipRfID[DataUpNum]+"���ݿ��ϴ��ܱ�ʧ��",DataUpNum,"���ܲ���:3-DB");
			return FALSE;
		}
		
	}
	else
	{
		LogShow_exchange(m_Result,Final_Result_Control,0,"SN:"+ChipRfID[DataUpNum]+"û�й�ѡ���ݿⲻ�ϴ�...",DataUpNum);
	}
	return TRUE;//LBCCheckIMEI
}
//���������ݿ�Data_UpdatePara