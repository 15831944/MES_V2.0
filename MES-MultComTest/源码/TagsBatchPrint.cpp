// TagsBatchPrint.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "TagsBatchPrint.h"

//��ӡ������
#include "ZebraPrinter.h"
// TagsBatchPrint �Ի���
#include ".\\KeyboardHook\\keyboard.h"

#include "CSpreadSheet.h"

IMPLEMENT_DYNAMIC(TagsBatchPrint, CDialog)

TagsBatchPrint::TagsBatchPrint(CWnd* pParent /*=NULL*/)
	: CDialog(TagsBatchPrint::IDD, pParent)
	, TagsStartNum(_T(""))
	, Tags_Count(_T(""))
	, TagsCurrNum(_T(""))
	, Tags_ALL(_T(""))
	, TagsEndNum(_T(""))
	, PrintBlankInterval(_T("100"))
	, count_Tags(0)
	, TagsBits(14)
	, PrintSpeed(400)
	, IMEI_CAL(FALSE)
{

}

TagsBatchPrint::~TagsBatchPrint()
{
}

void TagsBatchPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMEIS, TagsStartNum);
	DDX_Text(pDX, IDC_EDIT1, Tags_Count);
	DDX_Text(pDX, IDC_IMEIC, TagsCurrNum);
	DDX_Text(pDX, IDC_EDIT2, Tags_ALL);
	DDX_Text(pDX, IDC_IMEIE, TagsEndNum);
	DDX_Text(pDX, IDC_EDIT17, PrintBlankInterval);
	DDX_Control(pDX, IDC_IMEIC, TagsCurrNumControl);
	DDX_Control(pDX, IDC_EDIT1, Tags_CountControl);
	DDX_Control(pDX, IDC_LIST2, Excell_show);
	DDX_Text(pDX, IDC_EDIT5, count_Tags);
	DDX_Control(pDX, IDC_EDIT2, Tags_ALL_Control);
	DDX_Text(pDX, IDC_EDIT14, TagsBits);
	DDX_Text(pDX, IDC_EDIT4, PrintSpeed);
	DDX_Control(pDX, IDC_COMBO2, PrinterName);
	DDX_Control(pDX, IDC_RESULT, PrintStateShow);
	DDX_Check(pDX, IDC_CHECK1, IMEI_CAL);
}


BEGIN_MESSAGE_MAP(TagsBatchPrint, CDialog)
	ON_WM_TIMER()   //��ʱ��
	ON_WM_SYSCOMMAND()//PostMessage
	ON_BN_CLICKED(IDC_BUTTON17, &TagsBatchPrint::OnBnClickedButtonstart17)
	ON_BN_CLICKED(IDC_BUTTON19, &TagsBatchPrint::OnBnClickedButtonstart19)
	ON_BN_CLICKED(IDC_BUTTON2, &TagsBatchPrint::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON15, &TagsBatchPrint::OnBnClickedButtonstart15)
	ON_BN_CLICKED(IDC_BUTTON21, &TagsBatchPrint::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BatchPrintOutput_BUTTON, &TagsBatchPrint::OnBnClickedButton22)
END_MESSAGE_MAP()


// TagsBatchPrint ��Ϣ�������


BOOL TagsBatchPrint::OnInitDialog()//��ʼ������
{
	CDialog::OnInitDialog();
	//set the IMEI input box font
	CFont* ffont;
	ffont = new CFont;
	ffont->CreateFont(36,0,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial"); 
	// Cause the label to use the new font
	GetDlgItem(IDC_IMEIS)->SetFont(ffont);  //
	GetDlgItem(IDC_IMEIE)->SetFont(ffont);

	GetDlgItem(IDC_IMEIC)->SetFont(ffont);


	Excell_show.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EDITLABELS);
	Excell_show.SetBkColor(RGB(255,255,255));
	Excell_show.SetTextColor(RGB(3,146,207));
	Excell_show.SetTextBkColor(RGB(255,255,255));

	Excell_show.DeleteAllItems();
	UpdateData(TRUE);

	//��ձ�ͷ
	while(Excell_show.DeleteColumn(0));
	//���ֶ���ʾ����
	Excell_show.InsertColumn(0, "No", LVCFMT_CENTER, 25);
	Excell_show.InsertColumn(1, "IMEI/SN", LVCFMT_LEFT, 250);

	CString strValidChars;//	
	Excell_show.SetReadOnlyColumns(0);//read only
	Excell_show.SetReadOnlyColumns(1);//read only

	//strValidChars = "0123456789.";
	//m_list2.SetColumnValidEditCtrlCharacters(strValidChars,2);//digital only edit	
	strValidChars = "";
	Excell_show.SetColumnValidEditCtrlCharacters(strValidChars,2);//none control edit 

	Excell_show.EnableVScroll(); 		
	Excell_show.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	strContents=NULL;
	Print_Handle=NULL;
	GetPrinters();
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void TagsBatchPrint::AutoCreateTagsFunc()
{
	///////////���ݺŶ�����IMEI//////////////////
	////////////////
	long TotalCount;
	CHAR szTemp[50]={0};
    _tcscpy(szTemp,TagsStartNum);								//	��ʼ��
    m_int64StartTags=  _ttoi64(szTemp);

	_tcscpy(szTemp,TagsEndNum);									//	������
    m_int64EndTags=  _ttoi64(szTemp);
	TotalCount=m_int64EndTags-m_int64StartTags+1;


	/////////////////֮��ΪEXCELL///////////////////
	Excell_show.DeleteAllItems();
	UpdateData(TRUE);
	//��ձ�ͷ
	while(Excell_show.DeleteColumn(0));
	Excell_show.InsertColumn(0, "No", LVCFMT_CENTER, 25);
	Excell_show.InsertColumn(1,"IMEI/SN",LVCFMT_LEFT,250);
	/////////////////֮��ΪEXCELL///////////////////
	if(strContents!=NULL)
	{
		delete []strContents;
		strContents=NULL;
	}
	strContents=new CString[PRINTONCE_MAX];

	for(int ini=0;ini<PRINTONCE_MAX;ini++)
		*(strContents+ini)="";

	if(TotalCount>PRINTONCE_MAX)
		TotalCount=PRINTONCE_MAX;
	if(TotalCount<0)
		TotalCount=0;
	Tags_Count_int=0;

	count_Tags=0;													//��ʾ����
	for(int i=0;i<TotalCount;i++)
	{
		m_int64DefaultTags=m_int64StartTags+Tags_Count_int;		//��ʼ��+�Ѿ���ӡ�ĺ��������㵱ǰ�ĺ�

		if(IMEI_CAL==TRUE)
			AutoCreateTags(szTemp);
		else
			AutoCreateTags14(szTemp);
		TagsCurrNum.Format(("%s"),szTemp);							//��ǰ��
		Tags_Count_int++;											//������1��Ϊ�´ο�ʼ׼��   

		*(strContents+i)=TagsCurrNum;

		Excell_show.InsertItem(i,"1",0);//������
		TRACE("   ��:%d, ��:%d,����:%s\n",i,0,*(strContents+i));

		CString Count_PID;
		Count_PID.Format("%d",i+1);
		Excell_show.SetItemText(i,0,Count_PID);

		Excell_show.SetItemText(i,1,*(strContents+i));
		count_Tags++;
		UpdateData(false);
	}

	Tags_ALL.Format("%ld",TotalCount);							//�ܹ���Ҫ��ӡ����ֽ
	UpdateData(false);
	///////////���ݺŶ�����IMEI//////////////////
}

void TagsBatchPrint::AutoCreateTags(LPTSTR lpszTags)
{
    __int64 int64CD = ComputeCD(m_int64DefaultTags);
    __int64 int64Tags = m_int64DefaultTags * 10 + int64CD;
    _i64tot(int64Tags, lpszTags, 10); //10����IMEIת�����ַ���
}
void TagsBatchPrint::AutoCreateTags14(LPTSTR lpszTags)
{
    __int64 int64Tags = m_int64DefaultTags;
    _i64tot(int64Tags, lpszTags, 10); //10����IMEIת�����ַ���
}

__int64 TagsBatchPrint::ComputeCD(__int64 nImei)
{
    int nTime = 0;
    __int64 nCD = 0;  
    while(nImei != 0)
    {
        __int64 d = nImei % 10;
        if(nTime % 2 == 0)//��IMEI��14λ��ż��λ  ����2�������λ����ʮλ��֮�ͣ�
        {
            d *= 2;
            if(d >= 10)
            {
                d = d % 10 + d / 10;
            }
        }   
        nCD += d;   //�ټ�����λ��
        nTime++;
        nImei /= 10;
    }  
    nCD %= 10;      //��������� �ĸ�λ��
    if(nCD != 0)    //�ø�λ����0 ����У��λΪ0������Ϊ10-�ø�λ��
    {
        nCD = 10 - nCD;
    }  
    return nCD;
}


BOOL TagsBatchPrint::CheckIMEI(CString	m_IMEI)
{
	int IMEI_length=m_IMEI.GetLength();

	for(int i=0;i<IMEI_length;i++)
	{
		if(IMEI_length!= 15||(m_IMEI.GetAt(i) <'0' || m_IMEI.GetAt(i) >'9'))
		{
			return FALSE;
		}
		
	}

	__int64 int64IMEI =  _ttoi64(m_IMEI);
    __int64 int64CD = ComputeCD(int64IMEI / 10);
    __int64 int64IMEICD = int64IMEI % 10;
	if(int64IMEICD != int64CD)
    {
		return FALSE;
    }
	return TRUE;
}






///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumWindowsProc_2(HWND hwnd,LPARAM lparam)//�оٴ���
{ 
	
	//Process::EnableDebugPrivilege(TRUE);
	char szClassName[256];
	HICON   hIcon;

	if (!IsWindowVisible(hwnd))
        return TRUE;
	
    //�����Ƿ�ɼ���
    if (!IsWindowEnabled(hwnd))
        return TRUE;
	
    //�����Ƿ� WS_POPUP �� WS_CAPTION ����
	//һЩ���л��Ĵ���ͬʱ���� WS_POPUP �� WS_CAPTION������� WS_POPUP ȴ�� WS_CAPTION ��Ӧ������
    //�� Spy++ �۲죬������ OneNote TrayIcon �ȳ����ͨ���˷�ʽ����
	LONG gwl_style = GetWindowLong(hwnd,GWL_STYLE);
    if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION))
        return TRUE;
	
    //�����Ƿ���и����ڣ�
	HWND hParent = (HWND)GetWindowLong(hwnd,GWL_HWNDPARENT);
    //�������Ƿ�ɼ��
    //�� Spy++ �۲죬�硰���С��Ի���ȱ�Ӧ�����б�ĳ�����һ�����صģ����� WS_DISABLED �ĸ�����
    /*if (IsWindowEnabled(hParent))
        return TRUE;
    //�������Ƿ���ӣ�
    if (IsWindowVisible(hParent))
        return TRUE;
	*/
    //������ Shell_TrayWnd ���������������������޷������ˡ�
	//��������ﵥ���г���
	

	GetClassName(hwnd,szClassName,255);
    if (!strcmp(szClassName,"Shell_TrayWnd"))
	return TRUE;


	wininfo infoptr;
	::GetWindowText(hwnd, infoptr.winbuf,99);//��ȡ��������

	if(!strcmp(infoptr.winbuf,"Windows ���������"))
		 return TRUE;

	if(!strcmp(infoptr.winbuf,"�ܶ��ľ���"))
		 return TRUE;

	if(infoptr.winbuf[0]==0)
		return TRUE;

	if(strcmp(infoptr.filename,"C:\\WINDOWS\\Explorer.EXE"))//��ȡ����ͼ����
	{
		hIcon   =   ExtractIcon((HINSTANCE) OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,infoptr.pid),  
								infoptr.filename,   0);
	}
	else
	{
		hIcon = (HICON)(::GetClassLong(hwnd,GCL_HICON));//��ȡ����ͼ����
	}

	infoptr.hwnd=hwnd;

	infoptr.winicon=hIcon;

	infoptr.sign=2;

	((TagsBatchPrint *)lparam)->m_wininfoarray.Add(infoptr);
	return TRUE ;
}


void TagsBatchPrint::Get_App_Running()//��ȡ�������е�Ӧ�� 
{
	EnumWindows((WNDENUMPROC)EnumWindowsProc_2,(long)this) ;

	int size=m_wininfoarray.GetSize();

	for(int i=0;i<size;i++)
	{
		m_currentwin=m_wininfoarray.GetAt(i);

		//CString IDName;
		//IDName.Format("%s",m_currentwin.winbuf);
		//int Result_compare0=IDName.Find("Printing");
		//int Result_compare0=IDName.Find("��ӡ");
		int Result_compare0=_stricmp("��ӡ",m_currentwin.winbuf);
		if(Result_compare0==0)
		{
			OnWndFocus(TRUE,TRUE,Tags_Input);//����ǰ��,���µĴ�����������
			break;
		}
	}
	
}


UINT static __cdecl Get_App_RunningFun(LPVOID pParam)
{
	TagsBatchPrint* Mead_Main_Win = (TagsBatchPrint*)pParam;
	Mead_Main_Win->Input_Data(Mead_Main_Win->Tags_Input);
	return 0;
}
UINT static __cdecl Get_App_PauseFun(LPVOID pParam)
{
	TagsBatchPrint* Mead_Main_Win = (TagsBatchPrint*)pParam;
	Mead_Main_Win->Pause_Print();
	return 0;
}

void TagsBatchPrint::OnWndFocus(BOOL ShowWin,BOOL Print,CString IMEI_Print[PRINTONCE_MAX])
{
	WINDOWPLACEMENT   ws;   
	ws.length   =   sizeof(WINDOWPLACEMENT);   
 	::GetWindowPlacement(m_currentwin.hwnd,   &ws);   
	if(ShowWin==FALSE)
	{
		switch   (ws.showCmd)   
		{   
		case   1://����; 
		
		case   3://ԭʼ����Ϊ���	
			//::ShowWindow(m_currentwin.hwnd,SW_SHOW);			//ԭʼ
			::ShowWindow(m_currentwin.hwnd,SW_NORMAL);			//�޸�
			//::ShowWindow(m_currentwin.hwnd,SW_SHOW);
			//::SetForegroundWindow(m_currentwin.hwnd);
			::SetWindowPos(m_currentwin.hwnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE   );  
			/*
			hWnd��Ҫ����Ĵ��ھ����
			fAltTab���Ƿ�ʹ��С���Ĵ��ڻ�ԭ��
			ʹ��˵����
			��Ҫ����С���Ĵ��ڻ�ԭ��ʹ������Ļ��ǰ��ֻҪ��fAltTab��������TRUE�Ϳ����ˡ�
			*/
			//::SwitchToThisWindow(m_currentwin.hwnd,FALSE);
			break;
		
		case   2://ԭʼ����Ϊ(��)С��;
			//::ShowWindow(m_currentwin.hwnd,SW_SHOWMAXIMIZED);								//SW_SHOWMINIMIZED
			::SetWindowPos(m_currentwin.hwnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE   );	//����
			break;
		default:
			::ShowWindow(m_currentwin.hwnd,SW_NORMAL);										//SW_SHOWMINIMIZED
			::SetWindowPos(m_currentwin.hwnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE   );	//����
			break;	
		}   
	}
	else
	{
		switch   (ws.showCmd)   
		{   
		case   1://����; 
		
		case   3://ԭʼ����Ϊ���	
			::ShowWindow(m_currentwin.hwnd,SW_NORMAL);			//�޸�
			::SetWindowPos(m_currentwin.hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE   );  
			break;
		
		case   2://ԭʼ����Ϊ(��)С��;
			//::ShowWindow(m_currentwin.hwnd,SW_SHOWMAXIMIZED);								//SW_SHOWMINIMIZED
			::SwitchToThisWindow(m_currentwin.hwnd,TRUE);
			::SetWindowPos(m_currentwin.hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE   );	//����
			break;
		default:
			::ShowWindow(m_currentwin.hwnd,SW_NORMAL);										//SW_SHOWMINIMIZED
			::SetWindowPos(m_currentwin.hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE   );	//����
			break;	
		}   
	}
	
	if(Print==TRUE)
	{
		Sleep(1000);
		if ( !SetHook ( TRUE ) )
		{
			AfxMessageBox("��װ���̹�ʧ��!���������!") ;
			return;
		}
		WritePrivateProfileString(PAUSE_SECTION_CONFIG,"PAUSE","FALSE",PAUSE_PATH_CONFIG);

		CString PrinterNameCS;
		PrinterName.GetWindowTextA(PrinterNameCS);
		if(PrinterNameCS=="")
		{
			AfxMessageBox("����ѡ���Ӧ��ӡ��!");
			return;
		}
		Print_Handle=AfxBeginThread(Get_App_RunningFun,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);
		//Input_Data(Tags_Input);
		//AfxBeginThread(Get_App_PauseFun,(LPVOID)this,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	//ģ��ctrl+S����
}
void TagsBatchPrint::Pause_Print()
{

}
void TagsBatchPrint::Input_Data(CString IMEI_Print[PRINTONCE_MAX])
{
	BOOL OnceProcess=FALSE;
	
	int size=m_wininfoarray.GetSize();

	PrinterName.GetWindowTextA(PrinterNameCS);
	if(PrinterNameCS=="")
		goto StopState;

	int Count=atoi(Tags_ALL)/atoi(PrintBlankInterval);
	if(atoi(Tags_ALL)%atoi(PrintBlankInterval)!=0)										//����������0
		Count++;
	if(Sequence==TRUE)
	{
		for(int space=0;space<Count;space++)
		{
			for(int b=0;b<32;b++)
			{
				buf[b]='\0';
			}
			for(int i=0;i<atoi(PrintBlankInterval);i++)
			{
				while((LoadConfig_Change()=="TRUE")&&(StopSign==FALSE))
				{
					LogShow_exchange("----------------------------->��Ϊ��ͣ״̬������");		
					Sleep(1000);
				} 

				if(i%5==0)
				{
					
					///////////////////////////////////////////////////////
					//��ȡ��ӡ����Ϣ
					if(OnceProcess==FALSE)
					{
						if(!OpenPrinterA((LPSTR)(LPCTSTR)PrinterNameCS,&Current_handle,NULL))
						{
							LogShow_exchange("----------------------------->�򿪴�ӡ��"+PrinterNameCS+"ʧ�ܣ�����");
							goto StopState;
						}
						OnceProcess=TRUE;
					}
					if(OnceProcess==TRUE)
					{
						do
						{
							GetJobs(Current_handle,&ppJobInfo,&pcJobs,&pStatus);
							CString WaitPage;
							CString PrintedPage;
							WaitPage.Format("%ld",(ppJobInfo->TotalPages));
							PrintedPage.Format("%ld",(ppJobInfo->PagesPrinted));
							if((ppJobInfo->TotalPages>3))
							{
								LogShow_exchange("----------------------------->�ȴ���ӡ����ӡ��...");
								LogShow_exchange("----------------------------->����ӡҳ��"+WaitPage);
								LogShow_exchange("----------------------------->�Ѵ�ӡҳ��"+PrintedPage);
								Sleep(1500);
							}
						}while((ppJobInfo->TotalPages<20000)&&(ppJobInfo->TotalPages>0)&&(ppJobInfo->PagesPrinted<20000)&&(ppJobInfo->PagesPrinted>0)&&(ppJobInfo->TotalPages>3)&&(StopSign==FALSE));
					}
					//��ȡ��ӡ����Ϣ
					/////////////////////////////////////////////////////////
					/*
					for(int c=0;c<size;c++)
					{
						m_currentwin=m_wininfoarray.GetAt(c);

						int Result_compare=_stricmp("��ӡ",m_currentwin.winbuf);
						if(Result_compare==0)
						{
							OnWndFocus(TRUE,FALSE,Tags_Input);//����ǰ��
						}
					}
					Sleep(500);
					*/
				}
				if(IMEI_Print[i+(space*atoi(PrintBlankInterval))]=="")
				{
					LogShow_exchange("----------------------------->��ӡ��ϣ�����");
					goto StopState;
				}
				memcpy(buf,IMEI_Print[i+(space*atoi(PrintBlankInterval))].GetBuffer(IMEI_Print[i+(space*atoi(PrintBlankInterval))].GetLength()),IMEI_Print[i+(space*atoi(PrintBlankInterval))].GetLength());  //��cstring����byte����
				//ģ������
				if(StopSign==TRUE)
				{
					LogShow_exchange("----------------------------->ֹͣ��ӡ������");
					goto StopState;
				}
				Tags_LOG_CS="";
				TimerFunction();
			}
			////////////////////////////////////////////////
			int Tags_LOG=(space+1)*atoi(PrintBlankInterval);

			Tags_LOG_CS.Format("%d",Tags_LOG);
			if(Tags_LOG_CS=="")
				Tags_LOG_CS="999";
			TimerFunction();

			TagsCurrNum=IMEI_Print[(space+1)*atoi(PrintBlankInterval)-1];						//��ǰ��
			Tags_Count.Format("%d",Tags_LOG);													//������ʾ
			//��ʾ����
			//TagsCurrNumControl.UpdateWindow();
			//Tags_CountControl.UpdateWindow();
			
		}
	}
	else
	{
		for(int space=Count-1;space>=0;space--)
		{
			for(int b=0;b<32;b++)
			{
				buf[b]='\0';
			}
			for(int i=atoi(PrintBlankInterval)-1;i>=0;i--)
			{
				while((LoadConfig_Change()=="TRUE")&&(StopSign==FALSE))
				{
					LogShow_exchange("----------------------------->��Ϊ��ͣ״̬������");	
					Sleep(1000);
				} 
				if(IMEI_Print[i+(space*atoi(PrintBlankInterval))]=="")
				{
					continue;
				}
				else if(OnceProcess==FALSE)
				{
					if(!OpenPrinterA((LPSTR)(LPCTSTR)PrinterNameCS,&Current_handle,NULL))
					{
						LogShow_exchange("----------------------------->�򿪴�ӡ��"+PrinterNameCS+"ʧ�ܣ�����");
						goto StopState;
					}
					OnceProcess=TRUE;
				}
				if(i%5==0)
				{
					///////////////////////////////////////////////////////
					//��ȡ��ӡ����Ϣ					
					if(OnceProcess==TRUE)
					{
						do
						{
							GetJobs(Current_handle,&ppJobInfo,&pcJobs,&pStatus);
							CString WaitPage;
							CString PrintedPage;
							WaitPage.Format("%ld",(ppJobInfo->TotalPages));
							PrintedPage.Format("%ld",(ppJobInfo->PagesPrinted));
							if((ppJobInfo->TotalPages>3))
							{
								LogShow_exchange("----------------------------->�ȴ���ӡ����ӡ��...");
								LogShow_exchange("----------------------------->����ӡҳ��"+WaitPage);
								LogShow_exchange("----------------------------->�Ѵ�ӡҳ��"+PrintedPage);
								Sleep(1500);
							}
						}while((ppJobInfo->TotalPages<20000)&&(ppJobInfo->TotalPages>0)&&(ppJobInfo->PagesPrinted<20000)&&(ppJobInfo->PagesPrinted>0)&&(ppJobInfo->TotalPages>3)&&(StopSign==FALSE));
					}
					//��ȡ��ӡ����Ϣ
					/////////////////////////////////////////////////////////
					/*					
					for(int c=0;c<size;c++)
					{
						m_currentwin=m_wininfoarray.GetAt(c);

						int Result_compare=_stricmp("��ӡ",m_currentwin.winbuf);
						if(Result_compare==0)
						{
							OnWndFocus(TRUE,FALSE,Tags_Input);//����ǰ��
						}
					}
					Sleep(500);
					*/
				}
				
				memcpy(buf,IMEI_Print[i+(space*atoi(PrintBlankInterval))].GetBuffer(IMEI_Print[i+(space*atoi(PrintBlankInterval))].GetLength()),IMEI_Print[i+(space*atoi(PrintBlankInterval))].GetLength());  //��cstring����byte����
				//ģ������
				if(StopSign==TRUE)
				{
					LogShow_exchange("----------------------------->ֹͣ��ӡ������");
					goto StopState;
				}
				Tags_LOG_CS="";
				TimerFunction();
			}
			////////////////////////////////////////////////
			int Tags_LOG=(space)*atoi(PrintBlankInterval);

			Tags_LOG_CS.Format("%d",Tags_LOG);
			if(Tags_LOG_CS=="")
				Tags_LOG_CS="999";
			TimerFunction();

			TagsCurrNum=IMEI_Print[(space+1)*atoi(PrintBlankInterval)-1];						//��ǰ��
			Tags_Count.Format("%d",Tags_LOG);													//������ʾ
			//��ʾ����
			//TagsCurrNumControl.UpdateWindow();
			//Tags_CountControl.UpdateWindow();
		}
	}
StopState:
	Print_Handle=NULL;

	SetHook ( FALSE );
	StopSign=TRUE;						//��ɺ󽫴˱�־��ΪTRUE

	for(int c=0;c<size;c++)
	{
		m_currentwin=m_wininfoarray.GetAt(c);

		int Result_compare=_stricmp("��ӡ",m_currentwin.winbuf);
		if(Result_compare==0)
		{
			OnWndFocus(FALSE,FALSE,Tags_Input);//����ǰ��
		}

		Result_compare=_stricmp("��ֽ������ӡ",m_currentwin.winbuf);
		if(Result_compare==0)
		{
			OnWndFocus(FALSE,FALSE,Tags_Input);//����ǰ��
		}
	}
	LogShow_exchange("----------------------------->������ӡ������������");
}

BOOL TagsBatchPrint::TimerFunction()//��ʱ����
{
	if(Tags_LOG_CS=="")
	{
		for(int j=0;j<32;j++)
		{
			if(buf[j]=='\0')
				break;

			if(StopSign==TRUE)//���꣬���ǻ�û��ӡ
			{
				return 0;
			}
			//keybd_event('A', 0, 0, 0); 
			//keybd_event('A', 0, KEYEVENTF_KEYUP, 0); 
			keybd_event(buf[j], 0, 0, 0); 
			keybd_event(buf[j], 0, KEYEVENTF_KEYUP, 0); 
			Sleep(10);
			
			//keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); 
			//keybd_event(VK_RETURN, 0, 0, 0); 
			//keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  
		}
		if(StopSign==TRUE)//���꣬���ǻ�û��ӡ
		{
			return 0;
		}
		keybd_event('\r', 0, 0, 0); 
		keybd_event('\r', 0, KEYEVENTF_KEYUP, 0); 
		Sleep(10);
		keybd_event('\n', 0, 0, 0); 
		keybd_event('\n', 0, KEYEVENTF_KEYUP, 0); 
		Sleep(100);
	}
	
	////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	/////////////��ӡ�հ�///////////////////////////////////
	else
	{
		byte Blank_buf[96]={'\0'};
		memcpy(Blank_buf,Tags_LOG_CS.GetBuffer(Tags_LOG_CS.GetLength()),Tags_LOG_CS.GetLength());
		for(int j=0;j<96;j++)
		{
			if(Blank_buf[j]=='\0')
				break;
			if(StopSign==TRUE)//���꣬���ǻ�û��ӡ
			{
				return 0;
			}
			keybd_event(Blank_buf[j], 0, 0, 0); 
			keybd_event(Blank_buf[j], 0, KEYEVENTF_KEYUP, 0); 
			Sleep(10);
		}
		if(StopSign==TRUE)//���꣬���ǻ�û��ӡ
		{
			return 0;
		}

		keybd_event('\r', 0, 0, 0); 
		keybd_event('\r', 0, KEYEVENTF_KEYUP, 0); 
		Sleep(10);
		keybd_event('\n', 0, 0, 0); 
		keybd_event('\n', 0, KEYEVENTF_KEYUP, 0); 
		////////////��ӡ�հ�/////////////////////////////////////
		/////////////////////////////////////////////////////////
	}
	Sleep(PrintSpeed);
	return 1;
}

void TagsBatchPrint::OnTimer(UINT nIDEvent)//��ʱ��
{
	switch(nIDEvent)
	{
		case 0:
			KillTimer(0);
			if((StopSign == FALSE))
			{
				TimerFunction();
			}
			else
			{

			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL TagsBatchPrint::PrintSpace(CString Count)//��ӡ�հ���ֽ
{
	CString SN="";
	////////////////////////////
	//�ϴ��ɹ�����ö�̬���ӡ
	CZebraPrinter PRINTER;
	//
	CString SSID_Name,Password;
	SSID_Name="�հ���ֽ����";
	Password=Count;

	int X_Position;		//����X��ƫ��
	int Y_Position;		//����Y��ƫ��
	int Height;			//�߶�
	int Width;			//���
	CString WordFont;		//����
	int X_Pos[10]={0};		//[0]--������ ֵƫ����  [1]--"WIFI���룺" �ַ�ƫ����  [2]--WIFI���� ֵƫ����  [3]--MSN ֵƫ����   [4]--IMEI  ֵƫ����  [5]--IMEI����ƫ����
	int Y_Pos[10]={0};		//[0]--MSN Y��ƫ����   [1]--IMEI Y��ƫ����  [2]--IMEI���� Y��ƫ���� 

	CString CH_Font;        //��������
	//�߶�30�����0��X��10��Y��200������D��XPssidvalue100��XPpwname200��XPpwvalue300��XPmsnvalue50��XPimeivalue50��XPbarcode400��YPmsn260��YPimei300��YPbarcode300��

	Height=26;
	Width=0;
	X_Position=35;
	Y_Position=35;

	WordFont="0";
	CH_Font="����";

	X_Pos[0]=0;
	X_Pos[1]=0;
	X_Pos[2]=0;
	X_Pos[3]=0;
	X_Pos[4]=0;
	X_Pos[5]=0;

	Y_Pos[0]=0;
	Y_Pos[1]=35;
	Y_Pos[2]=0;

	bool Print_Result=PRINTER.OnPrintFunction(Height,Width,X_Position,Y_Position,(LPSTR)(LPCSTR)SSID_Name,(LPSTR)(LPCSTR)Password,(LPSTR)(LPCSTR)SN,"",(LPSTR)(LPCSTR)WordFont,X_Pos,Y_Pos,(LPSTR)(LPCSTR)CH_Font);
	if(Print_Result==true)
	{
		//AfxMessageBox("��ӡ�ɹ���");
		return TRUE;
	}
	else
	{
		AfxMessageBox("��ӡʧ�ܣ�");
		return FALSE;
	}
	////////////////////////////
}

void TagsBatchPrint::OnBnClickedButtonstart17()//����IMEI
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	AutoCreateTagsFunc(); 
}


void TagsBatchPrint::OnBnClickedButtonstart19()//����TXT		//AutoCreateTagsFunc
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fDlg(TRUE,
                     _T("txt"),
                     NULL,
                     OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
                     _T("�����ļ� (*.txt)|*.txt|�����ļ� (*.*)|*.*||"),
                     this);

    // �򿪵�·��Ϊ���������ڵ�·��
    char szName[MAX_PATH] = _T("");
    CString  szPath       = _T("");

    GetModuleFileName(NULL, szName, MAX_PATH);

    szPath = szName;
    szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);
    fDlg.m_ofn.lpstrInitialDir = szPath;
	//translate_path=szPath;
    // �ı���⣺Open - means ����ű��ļ�
    fDlg.m_ofn.lpstrTitle = _T("���������ļ�:");
	//AfxMessageBox("wait");

    if ( IDOK == fDlg.DoModal() )
    {
        // ���ݵõ���·�����ļ���
        m_strExchangeFileName = fDlg.GetPathName(); 
		//AfxMessageBox(m_strExchangeFileName);
        // �˴�������֤�ļ�����Ч��
        ASSERT(!m_strExchangeFileName.IsEmpty());
		UpdateData(false);

		//////////////////////////////////////////////////
		//////////////////////////////////////////////////
		Excell_show.DeleteAllItems();
		UpdateData(TRUE);
		//��ձ�ͷ
		while(Excell_show.DeleteColumn(0));
		Excell_show.InsertColumn(0, "No", LVCFMT_CENTER, 25);
		Excell_show.InsertColumn(1,"IMEI/SN",LVCFMT_LEFT,250);

		/////////////////֮��ΪEXCELL///////////////////
		////////////////////////////////////////////////
		if(strContents!=NULL)
		{
			delete []strContents;
			strContents=NULL;
		}
		strContents=new CString[PRINTONCE_MAX];

		for(int ini=0;ini<PRINTONCE_MAX;ini++)
			*(strContents+ini)="";
		//���ļ�
		CStdioFile file;
		file.Open(m_strExchangeFileName,CFile::modeRead);
		//���ж�ȡ�ַ���
	    
		int i=0;
		count_Tags=0;
		do
		{    
			file.ReadString( *(strContents+i));
			if(*(strContents+i)=="")
			{
				Tags_ALL.Format("%d",count_Tags);								//������������ֵ����  ���ɼ�������
				Tags_ALL_Control.SetWindowTextA(Tags_ALL);
				return;
			}
			Excell_show.InsertItem(i,"1",0);//������
			TRACE("   ��:%d, ��:%d,����:%s\n",i,0,*(strContents+i));

			CString Count_PID;
			Count_PID.Format("%d",i+1);
			Excell_show.SetItemText(i,0,Count_PID);

			Excell_show.SetItemText(i,1,*(strContents+i));
			i++;
			count_Tags++;
			UpdateData(false);

		}while(( *(strContents+i-1)!="")&&(count_Tags<PRINTONCE_MAX));

		Tags_ALL.Format("%d",count_Tags);								//������������ֵ����  ���ɼ�������
		Tags_ALL_Control.SetWindowTextA(Tags_ALL);
    }
}




void TagsBatchPrint::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	Sequence=TRUE;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	for(int i=0;i<PRINTONCE_MAX;i++)
	{
		if(*(strContents+i)!="")
		{
			Tags_Input[i]=*(strContents+i);
			if(TagsBits!=Tags_Input[i].GetLength())
			{
				AfxMessageBox("����λ���쳣��");
				return;
			}
		}
		else
			Tags_Input[i]="";
	}
	StopSign=FALSE;
	m_wininfoarray.RemoveAll();//ִ�к󣬽������е����
	Get_App_Running();
	
	
}

void TagsBatchPrint::OnBnClickedButtonstart15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopSign=TRUE;
	
	Print_Handle=NULL;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON21)->EnableWindow(TRUE);
}

void TagsBatchPrint::OnBnClickedButton21()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	Sequence=FALSE;
	GetDlgItem(IDC_BUTTON21)->EnableWindow(FALSE);
	for(int i=0;i<PRINTONCE_MAX;i++)
	{
		if(*(strContents+i)!="")
		{
			Tags_Input[i]=*(strContents+i);
			if(TagsBits!=Tags_Input[i].GetLength())
			{
				AfxMessageBox("����λ���쳣��");
				return;
			}
		}
		else
			Tags_Input[i]="";
	}
	StopSign=FALSE;
	m_wininfoarray.RemoveAll();//ִ�к󣬽������е����
	Get_App_Running();
}


BOOL TagsBatchPrint::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nKey;
	/*if (pMsg->message==WM_KEYData)
	{
		if(pMsg->wParam==1)
		{
			AfxMessageBox("1");
			//�����߳�
			if(Print_Handle!=NULL)
				Print_Handle->SuspendThread();
			return TRUE;
		}
		else
		{
			AfxMessageBox("2");
			if(Print_Handle!=NULL)
				Print_Handle->ResumeThread();
			return TRUE;
		}
		return TRUE;
	}
	else */if(WM_KEYDOWN == pMsg->message ) 
	{ 
		nKey = (int) pMsg->wParam; 
		if(VK_ESCAPE == nKey ) 
		{
			return TRUE;
		}
		else if(VK_CONTROL == nKey ) 
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

CString TagsBatchPrint::LoadConfig_Change()
{

    int nCommandNum   = 0;
	CStringArray strCommandAry;

	CString strLoadFile = PAUSE_PATH_CONFIG;
	CIniFile m_fIniFile(strLoadFile);

	m_fIniFile.ReadSection(PAUSE_SECTION_CONFIG, strCommandAry);
	nCommandNum = strCommandAry.GetSize();
	for(int i=0;i<nCommandNum;i++)
	{
        if(strCommandAry[i] == "PAUSE")
        {
         	      return m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommandAry[i], "");
        }

	}
	return "";
}





void TagsBatchPrint::GetPrinters()
{
	int size=4096;
    unsigned long sizeNeeded=0;
    unsigned long numPrinters;
    PPRINTER_INFO_1 pPrinters;
    pPrinters=(PPRINTER_INFO_1)LocalAlloc((LMEM_FIXED/LMEM_ZEROINIT),size);
    int ret=EnumPrinters(PRINTER_ENUM_LOCAL,NULL,1,(LPBYTE)pPrinters,size,&sizeNeeded,&numPrinters);

    for(int i=0;i<(int)numPrinters;++i)
	{
		PrinterName.AddString(pPrinters[i].pName);
	}
	////////////////////////////////////////////
    LocalFree(pPrinters);
}

////////////////////////////////////////////////////////////////////////

BOOL TagsBatchPrint::GetJobs(HANDLE hPrinter,			// Handle to the printer.
                JOB_INFO_2 **ppJobInfo,					// Pointer to be filled. 
                int *pcJobs,							// Count of jobs filled. 
                DWORD *pStatus)							// Print Queue status.   

{

	DWORD               cByteNeeded,nReturned,cByteUsed;
    JOB_INFO_2          *pJobStorage = NULL;
    PRINTER_INFO_2       *pPrinterInfo = NULL;

    if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))			//Get the buffer size needed
    {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            return FALSE;
    }

    pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);			//Failure to allocate memory
    if (!(pPrinterInfo))
        return FALSE;

    if (!GetPrinterA(hPrinter,2,(LPBYTE)pPrinterInfo,cByteNeeded,&cByteUsed))
    {
        free(pPrinterInfo);											//Failure to access the printer
        pPrinterInfo = NULL;
        return FALSE;
    }

    if (!EnumJobs(hPrinter,0,pPrinterInfo->cJobs,2,NULL,0,(LPDWORD)&cByteNeeded,(LPDWORD)&nReturned))//Get job storage space
    {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            free(pPrinterInfo);
            pPrinterInfo = NULL;
            return FALSE;
        }
    }

    pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
    if (!pJobStorage)
    {
        //Failure to allocate Job storage space
        free(pPrinterInfo);
        pPrinterInfo = NULL;
        return FALSE;
    }

    ZeroMemory(pJobStorage, cByteNeeded);

    /* Get the list of jobs. */ 
    if (!EnumJobs(hPrinter,0,pPrinterInfo->cJobs,2,(LPBYTE)pJobStorage,cByteNeeded,(LPDWORD)&cByteUsed,(LPDWORD)&nReturned))
    {
        free(pPrinterInfo);
        free(pJobStorage);
        pJobStorage = NULL;
        pPrinterInfo = NULL;
        return FALSE;
    }

    // Return the information.
    *pcJobs = nReturned;
    *pStatus = pPrinterInfo->Status;
    *ppJobInfo = pJobStorage;
    free(pPrinterInfo);
    return TRUE;

}

void TagsBatchPrint::LogShow_exchange(CString Msg_Log)   
{
	CString LOG_Show_Old;
	PrintStateShow.GetWindowText(LOG_Show_Old);
	PrintStateShow.SetWindowText(Msg_Log+"\r\n"+LOG_Show_Old.Left(8092));
	PrintStateShow.UpdateWindow();
}

/*
BOOL IsPrinterError(HANDLE hPrinter)
{

    JOB_INFO_2  *pJobs;
    int         cJobs,
                i;
    DWORD       dwPrinterStatus;
    if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))				//Get the state information for the Printer Queue and the jobs in the Printer Queue.
        return FALSE;

    if (dwPrinterStatus &
        (PRINTER_STATUS_ERROR |
        PRINTER_STATUS_PAPER_JAM |
        PRINTER_STATUS_PAPER_OUT |
        PRINTER_STATUS_PAPER_PROBLEM |
        PRINTER_STATUS_OUTPUT_BIN_FULL |
        PRINTER_STATUS_NOT_AVAILABLE |
        PRINTER_STATUS_NO_TONER |
        PRINTER_STATUS_OUT_OF_MEMORY |
        PRINTER_STATUS_OFFLINE |
        PRINTER_STATUS_DOOR_OPEN))
    {
        return TRUE;
    }

    for (i=0; i < cJobs; i++)												//Find the Job in the Queue that is printing.
    {
        if (pJobs[i].Status & JOB_STATUS_PRINTING)
        {
			//If the job is in an error state,report an error for the printer.Code could be inserted here to attempt an interpretation of the pStatus member as well.
			if (pJobs[i].Status &amp;
                (JOB_STATUS_ERROR |
                JOB_STATUS_OFFLINE |
                JOB_STATUS_PAPEROUT |
                JOB_STATUS_BLOCKED_DEVQ))
			{
				return TRUE;
			}
        }
    }
    return FALSE;															//No error condition.
}


*/
void TagsBatchPrint::OnBnClickedButton22()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(count_Tags==0)
	{
		AfxMessageBox("û��ѡ��Ҫ���������ݣ�");
		return;
	}
	CString LOG_Time;
    CTime t = CTime::GetCurrentTime();
	LOG_Time.Format("%02d-%02d-%02d",t.GetHour(),t.GetMinute(),t.GetSecond());

	//system("mkdir \"foldername\"");
	// �򿪵�·��Ϊ���������ڵ�·��
    char szName[MAX_PATH] = _T("");
    CString  szPath       = _T("");

    GetModuleFileName(NULL, szName, MAX_PATH);

    szPath = szName;
    szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);

	AfxMessageBox(szPath+LOG_Time+"_Out.xls");
	CSpreadSheet SS(szPath+LOG_Time+"_Out.xls", "��������");//*(strContents+i)
	
	CStringArray *sampleArray;
	SS.BeginTransaction();
	
	// �������
	//��ȡ�ֶ���strName==================================
	//��ձ�ͷ
	//while(m_list1.DeleteColumn(0));
	//���ֶ���ʾ����
	sampleArray=new CStringArray[count_Tags+8];
	sampleArray->RemoveAll();
	sampleArray->Add("DATA");
	SS.AddHeaders(*sampleArray);
	////////////////////////////
	int tt=0; 
	for (tt=0;tt<count_Tags;tt++)
	{
			(sampleArray+tt)->RemoveAll();
			(sampleArray+tt)->Add(*(strContents+tt));	    
	}
	for (tt=0;tt<count_Tags;tt++)
	{
		SS.AddRow(*(sampleArray+tt));
	}
	SS.Commit();
	delete[] sampleArray;
	AfxMessageBox("����������ϣ�");
}
