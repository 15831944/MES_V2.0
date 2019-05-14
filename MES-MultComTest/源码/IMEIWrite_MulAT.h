#pragma once
#include "afxwin.h"

#include "afxmt.h"//CEvent
#include "CurrentTest.h"
#include "WriteIMEIDlg.h"

#include "PackingPrint.h"

#include "md5.h"

#include <vector>      
#include <map>
 //ͼƬ��ʾ
#include "ThumbnailBoxDlg.h"

#include <deque>

#include "TServerScoket.h"
#include "RDAHostInterface.h"

#include "PowerControlDlg.h"
#include "Aes.h"
#include "ImageDll.h"
#include <string.h>
#include <algorithm>
#include "Aes.h"
#include "shlwapi.h"
using namespace std;

// IMEIWrite_MulAT �Ի���
#define THREAD_NUM 16
#define PORTS_NUM 48
#define AntiDupData 10

#define WM_SEFOCS  WM_USER+111

static CRITICAL_SECTION GETPORT;

static CRITICAL_SECTION CPUCOMMUNICATE;

static CRITICAL_SECTION UseScanResult;

static CRITICAL_SECTION SPEAKVOICE;

static CRITICAL_SECTION SUCCFAILCOUNT;

static CRITICAL_SECTION UPDATEDB[THREAD_NUM/4];

static CRITICAL_SECTION WIFICOMMUNICATE[THREAD_NUM/2];

static CRITICAL_SECTION SHOWONEPIC;

#define STOPDELAY 200
#define ITEMMAX   100

#define PICDATAMAX 150000
typedef struct
{
	LPVOID WinHandle;
	int HandleNum;
	CEdit* m_Result;
	CEdit* Final_Result_Control;
	CThumbnailBoxDlg* picdlg;
	CString ChipRfIDBG;	//��̨Ҫ�ϴ�������
	BOOL DayTimeStatep; //�����ҹ״̬
	CString RecString;	//���ڽ��յ���ͼƬ����
}
PARA_Handle;

//���������
struct wininfo
{
	char winbuf[100];
	char filename[100];
	CString winname;
	unsigned long pid;
	HICON winicon;
	HWND hwnd;
	short sign;
};

//�ı䴰�ڴ�С
#include ".\ResizableLib\ResizableDialog.h"

//��������
#include <sapi.h>

//ͼƬ�ϳ�
#include ".\JPGUnion\BasicUse.h"
#include ".\JPGUnion\CombineImages.h"
#include ".\JPGUnion\BjfProcess.h"
#include ".\JPGUnion\PictureProcess.h"


/*����һ��������ȫ�ֺ���*/
//����
struct CopyDataMSG
{
	char Port[1024];//���ں�
	INT MessageNum;//�Զ���Ϣ
	char MessageChar[1024];//�����ַ���
};

//����
LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException);

/*����һ�������ܵ��������*/

class IMEIWrite_MulAT : public CResizableDialog
{
	DECLARE_DYNAMIC(IMEIWrite_MulAT)

public:
	IMEIWrite_MulAT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~IMEIWrite_MulAT();

// �Ի�������
	enum { IDD = IDD_MULTCOMTEST };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg HBRUSH IMEIWrite_MulAT::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void OnCbnSelchangeCombo2_Fresh();					//ˢ�´���
	void OnGetWebSetting();
	BOOL GetSettingFromDB(CAdoInterface& myado,CString m_server,CString m_db,CString m_user,CString m_pwd,CString SoftwareVer,CEdit* m_Result,CEdit* Final_Result_Control);
	BOOL GetATSetting(CString SettingChar);

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam); 

	afx_msg void OnTimer(UINT nIDEvent);
public:
	CString ATCommandList_CSDBCompare;
public:
	BOOL SIMisOK[THREAD_NUM];
	PARAMETER_MAIN paraArray[MAXPARANUM];

	CFont* font;
	CFont* fontsp;
	CFont* font_fail;
public:
	CEvent RunCommandList1;
	BOOL StopAutoStart;
	BOOL StopSign[THREAD_NUM];
public:
	void UI_LoadConfig();
	void AT_LoadConfig();
	void Delay_Clock(UINT mSecond);

	BOOL OnGetport();
	//void InitCOM(CComboBox* m_Port,CComboBox* m_Baud,int num);//��ʼ������
	//BOOL OPen_Serial_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,BOOL CPUChoose=FALSE);
	BOOL OPen_Serial_PortReadConstant(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,BOOL CPUChoose=FALSE);
	BOOL CheckConnect_Thread(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control);
/*	char*  Send_Serial_Order(CString* Vaule_Return,CString strCommand_Vaule,int HandleNum,char* EndSign,char* StartSign,int WaitTime=3,int HexFlag=0);*///��������Ҫ����ʮ���������ݣ�������һ��ʮ�����Ƶ�Ĭ�ϲ�����Ϊ1��ʱ��ͷ��ͺͽ���ʮ����������


	void InitCOM(CComboBox* m_Port, CComboBox* m_Baud, int num, BOOL RDAFlag = FALSE);

	BOOL OPen_Serial_Port(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum, BOOL CPUChoose = FALSE, BOOL RDAFlag = FALSE);

	char*  Send_Serial_Order(CString* Vaule_Return, CString strCommand_Vaule, int HandleNum, char* EndSign, char* StartSign, int WaitTime = 3, int HexFlag = 0, BOOL RDAFlag = FALSE);

	BOOL CloseHandleControl(HANDLE hObject, BOOL RDAFlag = FALSE,int HandleNum=255);


	void LogShow_exchange(CEdit* m_Result,CEdit* Final_Result_Control,int State,CString Msg_Log,int HandleNum,CString Category="-1",CString ChipRfIDbg=""); 
	bool IMEI_Function_Judge(int i,CString IMEI_FT_Item,char* Serial_Order_Return,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control);

	CString GetData(char* Serial_Order_Return,CString Start,CString End,int Count=1,int HandleNum=0);//��ȡ��Ч����

public:	
	BOOL WriteIMEIFunction_Thread(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CEdit* Data_Input_Control);
	__int64 ComputeCD(__int64 nImei);
	BOOL CheckIMEI(CString	m_IMEI);

	void EnableWindow_ALL(BOOL Choose);

	void EnableWindow_StartALL(BOOL Choose);
	void EnableWindow_Start(int HandleNum);
	void EnableWindow_Choose(int HandleNum);
	void SetFocus_Choose(int HandleNum);
	//void SetFocus_ChooseThread(int HandleNum);

	void Get_App_Running();//��ȡ�������е�Ӧ�� 
	void OnWndFocus(BOOL ShowWin,BOOL Print,CString IMEI_Print[THREAD_NUM]);
public:
	CArray<wininfo,wininfo&> m_wininfoarray;
	wininfo m_currentwin;
	CEvent        Save_Wait;
public:
	BOOL EnableMenu;

	BOOL COM_IniDone;
	CString COM_Number[PORTS_NUM];

	BOOL COM_State[THREAD_NUM+1];//����״̬ 
	BOOL Thread_State[THREAD_NUM];
public:
	COLORREF gColor[THREAD_NUM];
	
	CBT_Addr_Setting IMEI_Setdlg;
	//CBT_Addr_Setting IMEI_Setdlg2;
	//CBT_Addr_Setting IMEI_SetDB[THREAD_NUM];
	HANDLE hPort[THREAD_NUM+1];						//+1-->��Ƭ�����ƶ˿�
	CString Vaule_Return_Count_CS[THREAD_NUM+1];	//���յ��Ĳ���
	CWinThread* Thread_Handle[THREAD_NUM];

	CString IMEI_Input[THREAD_NUM];

	////////////////////////////////////////
	////////////////////////////////////////
public:
	// д���߳�1���
	CEdit Final_Result_Control1;
	// LOG��Ϣ
	CEdit m_Result1;

	CComboBox m_Port1;
	CComboBox m_Baud1;

	afx_msg void OnEnChangeImeia();
	CString IMEI_InputA;
	CEdit IMEI_InputA_Control;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();




	// �߳�2
	CEdit Final_Result_Control2;
	CEdit m_Result2;
	CComboBox m_Port2;
	CComboBox m_Baud2;

	afx_msg void OnEnChangeImeia2();
	CString IMEI_InputB;
	CEdit IMEI_InputB_Control;
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();


	// �߳�3
	CEdit Final_Result_Control3;
	CEdit m_Result3;
	CComboBox m_Port3;
	CComboBox m_Baud3;
	CString IMEI_InputC;
	CEdit IMEI_InputC_Control;
	afx_msg void OnEnChangeImeia3();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	// �߳�4
	CEdit Final_Result_Control4;
	CEdit m_Result4;
	CComboBox m_Port4;
	CComboBox m_Baud4;
	CString IMEI_InputD;
	CEdit IMEI_InputD_Control;
	afx_msg void OnEnChangeImeia4();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	// �߳�5
	CEdit Final_Result_Control5;
	CEdit m_Result5;
	CComboBox m_Port5;
	CComboBox m_Baud5;
	CString IMEI_InputE;
	CEdit IMEI_InputE_Control;
	afx_msg void OnEnChangeImeia5();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	// �߳�6
	CEdit Final_Result_Control6;
	CEdit m_Result6;
	CComboBox m_Port6;
	CComboBox m_Baud6;
	CString IMEI_InputF;
	CEdit IMEI_InputF_Control;
	afx_msg void OnEnChangeImeia6();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	// �߳�һ
	CButton StartA_Control;
	// �̶߳�
	CButton StartB_Control;
	// �߳�3
	CButton StartC_Control;
	// �߳���
	CButton StartD_Control;
	// �߳���
	CButton StartE_Control;
	// �߳���
	CButton StartF_Control;


	// ɨ��ǹ�˿�
	CComboBox ScanGunPort1;
	CComboBox ScanGunPort2;
	CComboBox ScanGunPort3;
	CComboBox ScanGunPort4;
	CComboBox ScanGunPort5;
	CComboBox ScanGunPort6;
	// ɨ��ǹ������
	CComboBox ScanGunBaud1;
	CComboBox ScanGunBaud2;
	CComboBox ScanGunBaud3;
	CComboBox ScanGunBaud4;
	CComboBox ScanGunBaud5;
	CComboBox ScanGunBaud6;
	// ɨ��ǹ����
	CComboBox ScanGunTpye;

	HANDLE hScanGun[THREAD_NUM];
	CString Vaule_ScanGun[THREAD_NUM];												//ɨ��ǹ����
	CString OtherData_Input[THREAD_NUM];
public:
	BOOL TestCheck;																	//����ɨ��ǹ
	CString PublicIMEI;
	BOOL CompareIMEI;																//�ȶ�IMEI
public:
	BOOL OPen_ScanGun_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum);		//��ɨ��ǹ�˿�
	BOOL Get_ScanGun_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,char* EndSign);								//��ȡɨ��ǹ�˿�����
	BOOL Check_ScanGun_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,CEdit* Data_Input_Control,BOOL ImeiChoose,int LengthLimit);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton33();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton35();
public:
	CPackingPrint PrintInt;
	BOOL PrintOn;//��ӡ���˳�ʼ��
	CString Data_FromMachine[THREAD_NUM];
	CString Data1_FromDB[THREAD_NUM];
	CString Data2_FromDB[THREAD_NUM];
	CString Data3_FromDB[THREAD_NUM];
public:
	afx_msg void OnBnClickedButton10();
	// ѡ���Ƿ��ӡ
	CComboBox PrintSelect;
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton40();
	// �����Ƿ�Ϊ�ȶ��˿�
	CComboBox PortType;
	afx_msg void OnBnClickedButton42();

public:
	CString MachineType_CS;				//��������
	CString ZhiDanCS;					//�Ƶ�
	CString ReturnCodeCS;				//��������
public:
	long TestTimeStart[THREAD_NUM];			//��ʼ��ͨ����ʱ��
	CFile cReport[THREAD_NUM];
	CString IMEIRecord[THREAD_NUM];			//IMEI
	CString ChipRfID[THREAD_NUM];
	CString Software_Version[THREAD_NUM];	//����汾
	CString Software_VersionFull;
	CString BTMac[THREAD_NUM];				//������ַ
	CString BTTestResult[THREAD_NUM];		//�������Խ��
	CString SimNumber[THREAD_NUM];			//SIM����
	CString NteTemperature[THREAD_NUM];		//�¶�
	CString GPSSnrAve[THREAD_NUM];			//GPSƽ��SNR
	CString InchargeI[THREAD_NUM];			//������
public:
	void WriteLogIni(int LogNum,CString ChipRfIDbg="");//дLOG��Ϣ
	void WriteLog(CString Text,int	  LogNum);
	void WriteLogEnd(int LogNum);
public:
	// ���ս��
	CEdit Final_Result_Control7;
	CEdit Final_Result_Control8;
	CEdit Final_Result_Control9;
	CEdit Final_Result_Control10;
	CEdit Final_Result_Control11;
	CEdit Final_Result_Control12;
	CEdit Final_Result_Control13;
	CEdit Final_Result_Control14;
	CEdit Final_Result_Control15;
	CEdit Final_Result_Control16;
	CEdit m_Result7;
	CEdit m_Result8;
	CEdit m_Result9;
	CEdit m_Result10;
	CEdit m_Result11;
	CEdit m_Result12;
	CEdit m_Result13;
	CEdit m_Result14;
	CEdit m_Result15;
	CEdit m_Result16;
	CComboBox m_Port7;
	CComboBox m_Port8;
	CComboBox m_Port9;
	CComboBox m_Port10;
	CComboBox m_Port11;
	CComboBox m_Port12;
	CComboBox m_Port13;
	CComboBox m_Port14;
	CComboBox m_Port15;
	CComboBox m_Port16;
	CComboBox m_Baud7;
	CComboBox m_Baud8;
	CComboBox m_Baud9;
	CComboBox m_Baud10;
	CComboBox m_Baud11;
	CComboBox m_Baud12;
	CComboBox m_Baud13;
	CComboBox m_Baud14;
	CComboBox m_Baud15;
	CComboBox m_Baud16;
	CButton Start7_Control;
	CButton Start8_Control;
	CButton Start9_Control;
	CButton Start10_Control;
	CButton Start11_Control;
	CButton Start12_Control;
	CButton Start13_Control;
	CButton Start14_Control;
	CButton Start15_Control;
	CButton Start16_Control;
	CButton StartALL_Control;
	CButton Stop1_Control;
	CButton Stop2_Control;
	CButton Stop3_Control;
	CButton Stop4_Control;
	CButton Stop5_Control;
	CButton Stop6_Control;
	CButton Stop7_Control;
	CButton Stop8_Control;
	CButton Stop9_Control;
	CButton Stop10_Control;
	CButton Stop11_Control;
	CButton Stop12_Control;
	CButton Stop13_Control;
	CButton Stop14_Control;
	CButton Stop15_Control;
	CButton Stop16_Control;
	CButton StopALL_Control;
	CEdit *m_ResultArray[16];
	CEdit *Final_Result_ControlArray[16];
	CButton *m_StopControlArray[16];
	CEdit m_ShowNumberPort1Control;
	CEdit m_ShowNumberPort2Control;
	CEdit m_ShowNumberPort3Control;
	CEdit m_ShowNumberPort4Control;
	CEdit m_ShowNumberPort5Control;
	CEdit m_ShowNumberPort6Control;
	CEdit m_ShowNumberPort7Control;
	CEdit m_ShowNumberPort8Control;
	CEdit *m_ShowNumberPortControlArray[8];
	afx_msg void OnBnClickedButtonstart7();
	afx_msg void OnBnClickedButtonstop7();
	afx_msg void OnBnClickedButtonstart8();
	afx_msg void OnBnClickedButtonstart9();
	afx_msg void OnBnClickedButtonstart10();
	afx_msg void OnBnClickedButtonstart11();
	afx_msg void OnBnClickedButtonstart12();
	afx_msg void OnBnClickedButtonstart13();
	afx_msg void OnBnClickedButtonstart14();
	afx_msg void OnBnClickedButtonstart15();
	afx_msg void OnBnClickedButtonstart16();
	afx_msg void OnBnClickedButtonstop8();
	afx_msg void OnBnClickedButtonstop9();
	afx_msg void OnBnClickedButtonstop10();
	afx_msg void OnBnClickedButtonstop11();
	afx_msg void OnBnClickedButtonstop12();
	afx_msg void OnBnClickedButtonstop13();
	afx_msg void OnBnClickedButtonstop14();
	afx_msg void OnBnClickedButtonstop15();
	afx_msg void OnBnClickedButtonstop16();
public:
	//��������
	HRESULT hr;
	ISpVoice*  pVoice;
	void Voice_Ini();
	void Voice_Speak(CString Text);
public:

public:
	BOOL GetExistReturnCode(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString *ExistMessage,CString ChipRfIDbg="");
	BOOL Data_UpdatePara(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL ErrorUpEnable=TRUE,CString ChipRfIDbg="");
	BOOL Data_UpdatePara2(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL ErrorUpEnable=TRUE,CString ChipRfIDbg="");			//SMT����
	BOOL Data_UpdatePara3(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, BOOL ErrorUpEnable = TRUE, CString ChipRfIDbg = "");			//�ϻ������
	BOOL Data_UpdateParaPre(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL* smt_Check);
	BOOL Data_UpdateError(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString Message,CString Category,CString ChipRfIDbg="");
	BOOL GetExistError(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString *ExistMessage,CString ChipRfIDbg="");
	// ��Ƭ������
public:
	BOOL CPUCommunication(CString CpuOrderName,int HandleNum,CString EndSign,CString StartSign,BOOL Enable,CEdit* m_Result=NULL,CEdit* Final_Result_Control=NULL);
public:
	HINSTANCE password_Handle;
public:
	void  SaveConfig_Change();
	void  LoadConfig_Change();

	unsigned short (WINAPI* GetNEWTTL_encrypt_16)(const unsigned char* pData, int nLength);
	bool  LoadDll();

	char HexToASCII(unsigned char  data_hex);
	void HexGroupToString(char *OutStrBuffer, unsigned char *InHexBuffer, unsigned int HexLength);//������ʾ���ڷ���ֵ

	BOOL StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);//stringתHEX "12AB"--->0x12 0xab
	unsigned int SeperateToHex(const char *src, unsigned char *des);							  //"12 AB"--->0x120xAB--->���ݿո�
	CString  ShowPassWord(CString InputID,CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum);

	char* AscToHexString(char* pInData, char* pOutData);											//"ab01234" -> "61 62 30 31 32 33 34 "
public:
	CComboBox CPUControlPort;
	CComboBox CPUControlBaud;
	afx_msg void OnBnClickedButtonconncpu();
	afx_msg void OnBnClickedButtondisconncpu();
	// ������
	CComboBox OrderNumbersControl;
	CComboBox GPS_SNRLimit;
	CString GPS_SNRLimitCS;
	// �ɹ�ʧ�ܼ���
	int SuccessCount;
	int FailCount;
	// ��������
	CComboBox ReturnCodeControl;
	// ��ѹ������
	CString SimulationCurrent[THREAD_NUM];
	CComboBox VIControlPort;
	CComboBox VIControlBaud;
	afx_msg void OnBnClickedButtonconncpu2();
	afx_msg void OnBnClickedButtondisconncpu2();
	// ��վѡ��
	CComboBox WorkStationControl;
	CString WorkStationCS;
public:
	CString	Hostname;
	CString Ipaddress;
	BOOL GetIp_Name(CString& m_hostname,CString& m_ipaddress);
	// ���SMT�Ƿ�©��
	BOOL CheckSMTChoose;
	afx_msg void OnBnClickedCheck39();
	BOOL RTestChoose;
	afx_msg void OnBnClickedCheck40();
	afx_msg void OnCbnSelchangeCombo53();
public:
	void OnBnClickedBtnCopy();
	void FileCopy();
	DWORD CopyFileExLBC(CString strSrcPath, CString strDestPath, CString strExt);
public:
	HANDLE hPicPort[THREAD_NUM];
	CString Vaule_Pic[THREAD_NUM];												//ͼ������

	BOOL DayTimeState[THREAD_NUM];												//FALSEҹ��,TRUE����
	CString PicPath[2][THREAD_NUM];												//0ҹ��,1����

	CEvent DoorWait[THREAD_NUM];
	BOOL ResetButtonWIFI[THREAD_NUM];											//��ťWIFI�ź�
	BOOL PicUnionOK[THREAD_NUM];												//ͼƬ�ϳɽ��

	CEvent SerialPic[THREAD_NUM];
	BOOL SerialPicOver[THREAD_NUM];

	int TimerHandleNum;//��ʱ����ʾͼƬ���߳�Number
	CThumbnailBoxDlg Picdlg[THREAD_NUM];
	CString UnionPicPath[THREAD_NUM];
public:
	//ͼƬ����
	BOOL GetPicFunction_Thread(int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,CString ChipRfIDbg,BOOL DayTimeStatep);
	void StartGetPic(int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control);

	BOOL ShowPicFunction_Thread(CString RecString,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,CString ChipRfIDbg,BOOL DayTimeStatep);
	void StartShowPic(int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CString RecString);
	BOOL Show_Pic_Data(CString RecString,CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,int HandleNum,CString StartSign,CString EndSign,CString ChipRfIDbg,BOOL DayTimeStatep);		//��ȡͼ��˿�����

	BOOL OPen_Pic_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum);		//��ͼ��˿�
	BOOL Get_Pic_Data(CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,int HandleNum,CString StartSign,CString EndSign,CString ChipRfIDbg,BOOL DayTimeStatep);		//��ȡͼ��˿�����

	BOOL Get_Wifi_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,CString StartSign,CString EndSign,BOOL InThread=FALSE);	//��ȡ��ťWIFI����

	BOOL HEX2JPG();
	BOOL HEX2JPG2();

	BYTE ConvertHexChar(BYTE ch);
	BYTE * StrDecToCHex(int &length, CString str);

	void JPG2HEX();
	BOOL HEX2JPG3(CString str,CString& m_sPath,CString NightDay,int HandleNum,CString ChipRfIDbg="");
	BOOL GetPicChoose;
	//ͼƬ�ϳ�
	BOOL Jpgunionjpg(CString szPathName1,CString szPathName2,CString strInfo,CString PicName,CString& strDestJpgName);
public:
	ULONG_PTR m_GdiplusToken;
	GdiplusStartupInput m_GdiplusStartupInput;
public:	
	afx_msg void OnBnClickedCheck41();
	// ��̨ȷ��
	BOOL BGConfirmChoose;
	CString BGShowPicTime[THREAD_NUM];//ͼƬ��̨��ʾʱ��
	int BGShowPicCount[THREAD_NUM];//ͼƬ��̨��ʾ����

	BOOL TestResult[THREAD_NUM];//�����̵߳Ľ��
	afx_msg void OnBnClickedCheck42();
	BOOL GPSDataTypeChoose;
	CComboBox WIFI_RSSILimit;
	CString WIFI_RSSILimitCS;
	afx_msg void OnBnClickedCheck43();
	afx_msg void OnCbnSelchangeCombo56();
public:
	BOOL RecoverVoltage;
	// ͼƬ��̬��ʾ
	BOOL PicStaticChoose;



	/*�������ܵ����Ϻ���*/

	void StartButtonGatherFun(int HandleNum);//����ʼ��ť��Ҫ��������̼��ϳ�һ������
	void StopButtonGatherFun(int HandleNum);//��ֹͣ��ť��Ҫ��������̼��ϳ�һ������

	BOOL DequeContinueControlFun(int HandleNum,deque<int> &ContinueDeq);//ͳһ�Զ��н��й����ڶ��������ݽ���֮ʱ�����ж������߳���û�б��رգ��Ƿ��ֵ���ǰ�����̹߳�����

	/*����һ��������*/
	
	//����ͨѶ��ص�
	protected:
		afx_msg LRESULT MSG_GetSimpleMessage(WPARAM wParam, LPARAM lParam);//��ȡ�Զ�����Ϣ�õ�ϵͳ��Ϣ����
		afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);//�������ͽṹ���
	public:
		CString m_SoftVersion;//����汾���������ȡ������ʹ�õ�
		int m_bVar;//��Ϣ����ʱ�ظ���ֵ���������ȡ������ʹ�õ�
		HWND ADCTHwnd;//ADCT�ľ��

		static map<CString, int>PortStatusMap;//��map������StopSign�������������ŵ��Ǵ��ںţ�ֵ�ŵ���StopSign�����Ӧ���±꣬���ô˺�������Կ���ͨ�����ں����ж϶˿��Ƿ���ռ��

		void MSG_SendSimpleMessage();//�Զ�����Ϣ���ͺ���,���;�����ܿس���
		void MSG_SendCopyDataMessage(CopyDataMSG *MessageStruct,int replyInt);//����CopyData�����ͽṹ��

		void ADCTInquire(CString PortNo,INT CommandNo);//ADCT���в�ѯʱ
		void ADCTSetup(CString PortNo, INT CommandNo,CString message);//ADCT��������ʱ
		void SendADCTTestResult(CString PortNo, CString message);//��ADCT���س����Ͳ��Խ��
		void SendADCTHint(CString PortNo, CString message,int MessageType);//��ADCT���س�������ʾ��Ϣ,MessageType,0����ʾ��Ϣ��1��������Ϣ��־��2�Ǵ�����Ϣ��3�ǵ�����
		afx_msg void OnClose();;//���������˳�
		//LONG WINAPI FreeEIM_UnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo); //�����쳣�˳�
		void AfxMessageBoxHint(CString str);//��Ϊ����һ�Ĺ�ϵ������Ҫ�������������ԭ���ĵ�����

		int InitSetting(CString Zhidan, CString Version);//ʵ���Զ���ȡ����
		int StartPortTest(CString PortNo);//ʵ�ִ��ڿ�ʼ
		int StopPortTest(CString PortNo);//ʵ�ִ��ڹر�
		


	/*����������������*/

    //ע�⣺�����з������Լ���Ƶ�����͸����ʽ����Dongle����������������
	public:
		int OrderNumber;//����ָ������
		int MacIntercept;//MAC��ַȡ��λ
		int OrderCount;//����ָ��ʹ���
		int ExitOrderNumber;//�����Ͽ�ָ������
		CString BleOrder[10][2];//�������ָ��
		CString ExitOrder[10][2];//��������Ͽ�ָ��
		int BleOrderCount[10];//�������ָ���ӳ�ʱ��
		int BleGetSettingFlag[16];//Ϊ���ô��ڱ��ص�ʱ���Ӻ���Ҳ���ص�������˱�־λ
		CString BleMacAddress[16];//�������Mac��ַ��
		deque<int> ComFreeDeq;//��ſ��еĴ���
		deque<int> ComoOccupancydDeq;//��ű�ռ�õĴ���
		CWinThread* BLESetting_Thread_Handle;//�����߳�

		int BLEGetSettingFlag;//�������������ȡ�����õı�־λ��������ɨ��ǹɨ��MAC��ַ�����������������Ӻ�ſ��Լ�����ȡ���ã�-1��ʾ��û��ȡ���ã�0�������ڻ�ȡ���ã�2��ʾ���MAC��������ʧ�ܣ�Ȼ��ͻ�ֱ�ӻ�ȡ����ʧ�ܣ�1��ʾ���ӳɹ����Լ�����ȡ����
		BOOL MacScanEnbleFlag;//Mac��ַ�Ƿ�����ɨ���־λ�����ΪFALSE����ʾ��ʱ���ڽ����������ӣ�������ɨ�룬��֮��������ɨ�벢���Ӵ�������ַ
		BOOL BLEGetSettingEndFlag;//����������Ƿ��Ѿ���ȡ��ɵı�־λ

		BOOL GetBluetoothCheckValue;//������ѡ�����

		afx_msg void OnBnClickedResetbleButton();//�����������õı�־λ��ʹ�������������»�ȡ����
		afx_msg void OnBnClickedBluetoothCheck();//������ѡ��������
		void GetBleOrder();//��ȡ���������ļ����ָ��ͬʱ��ʼ���������Եı���
		void BleStartPortTest(CString MACStr);//����ComFreeDeq���еĵ�һλ����ʼĳ���˿�
		void BleStopPortTest(int PortNo);//��Ϊ��ͨ��ɨ��ǹɨ�裬Ȼ�����MAC�����ڣ��������۳ɹ�����ʧ��֮��Ҫֹͣ�ô���
		void DeleteComoOccupancydDeq(int PortNo);//�Ӷ�����ɾ���Ѿ�������Ķ˿ں�
		void BluetoothCoreFunction(CString MACStr);//�����������Ժ��ĺ��������������ȡ����ʱ���������ӺͿ�ʼ����ʱ����������
		int BluetoothConnect(int PortNo, CString MACStr, CComboBox* m_Port, CComboBox* m_Baud);//�������Ӻ���
		void BluetoothHint(int PortNo, CString str);//�������Դ�����ʾ�õĺ���
		void BluetoothDisconnect(int PortNo);//�����Ͽ�����


	/*Dongle����������͸��ģ��*/

	//ע�⣺Dongle����������������з������Լ���Ƶ�����͸����ͨ��
	public:
		deque<int> DongleTestDataDeq;//����ǰ�������Զ˿��źöӣ�ȷ�����߳���ֻ��һ���߳��ڷ���DongleTestDataArray���飬�������ݷ��ʳ�ͻ
		//CString DongleTestDataArray[8];//��ŵ�ǰ�˿����ڲ��Ե�������ַ�����ڶ�·�������Ժ�����ɨ��ǹ����
		CString DongleInfo[16][2];//���MAC��ַ�����ƣ�MAC��ַ���ڵ���оƬID�ϴ����ݿ⣬�������ڷ���ָ��
		CString DongleInitCommand[16];//Dongle������������ʼ��ָ�Ԥ��16��ָ��λ
		CString DongleSPCommand[4][2];//Dongle��������������ָ�0Ϊ����ָ�1Ϊ�Ͽ�ָ�2Ϊ��ȡ����ָ�3Ϊ��ȡ�汾��
		int DongleSpCommandSleepTime[4];//Dongle��������������ָ���Ӧ�ĵȴ�ʱ��
		int DongleInitCommandNumber;//��ʼ��ָ������
		int DongleComStartCount;//Dongle�����˿ڿ�ʼ��Ŀ������ɨ��ǹ���Զ����䣬��ֹ��MAC��ַ���䵽���������ӵĶ˿�

		BOOL GetDongleCheckValue;//Dongle������ѡ�����
		BOOL DongleConnectFlag[16];//Dongle�������������ӱ�ʶ��FALSEΪδ���ӣ�TRUEΪ������
		BOOL DongleCheckRssiFlag[16];//Dongle�������������ǿ�ȱ�ʶ����ʼ����ΪFALSE��������������������ΪTRUE����ʱCheckConnect_Thread�����ͻ�ȥ�������ǿ�ȣ��ж������豸�Ƿ�����
		int DongleConnectCount[16];//��¼�������Ӵ�����Dongle�����������ﵽһ�����Ӵ�����������һ�£��建��
	

		int DongleSleepTime;//����ͣ��ʱ��
		int DongleConnectRssi, DongleDisonnectRssi;//��������ǿ�Ⱥ������Ͽ�ǿ��
		int DongleSDURTime;//����ɨ��ʱ��
		int DongleResetCount;//��������X�κ�Ͷ��������������³�ʼ��
		int DongleScanGunFlag;//����ɨ��ǹ��־λ��Ϊ��ʱ����������ɨ�룬Ϊ��ʱδ���䵽MAC��ַ�Ĵ����̻߳ᴦ�ڵȴ�״̬

		BOOL DongleInit(int HandleNum);//��ʼ������������
		void DongleInitSetting();//�������ã������ȣ���ʼ������ini�ļ�
		BOOL DongleScan(int HandleNum,int RssiStr,CString SoftModel);//�����Զ�ɨ������
		BOOL DongleConnect(int HandleNum);//��������
		BOOL DongleDisConnect(int HandleNum);//�Ͽ�����
		BOOL DongleScanGun(int HandleNum);//����ɨ��ǹ���ܺ���

		CString DongleValueAnalyze(CString CommandName,CString CommandValue);//����������ֵ���н���
		int DongleCommandNameToInt(CString CommandName);//������ת������һ����װ����(�ַ���תint������switch)

		CString DongleUnixTimeToDatatime(CString str);//ʱ���Unix timestampת��
		CString DongleHexToASCII(CString str);//�ַ���ʮ������תASCII��
		CString DongleTemperatureToCString(CString str);//�����¶�
		int HexToDec(CString str);//ʮ������תʮ����
		CString StampToDatetime(time_t nSrc);//����ת����ʱ���
		BYTE * IMEIWrite_MulAT::CStrToByte(int len, CString str);//CStringתByte

		CString Encrypt(CString plainText);//AES���ܺ���
		CString Decrypt(CString plainText);//AES���ܺ���


		afx_msg void OnBnClickedDonglebleCheck();
	
		
	/*Dongle����ɨ���ź�ǿ����������*/
		deque<int> DongleTestRssiDeq;//���������źţ������źŴ�������Ÿ��ӣ����������������������У������в�Ӳ���(��Ϊ�յ�����»����ڵڶ�λ)
		static map<CString, int>DongleRssiMap;//���������ַ���ź�
		BOOL DongleStatusArray[8];//�����Ƿ�ʼ��״̬��ֻҪ������1�ͱ�ʾ��Ҫ����������ȫ��Ϊ0��ʾҪֹͣ�������������DongleRssiMap
		CString DongleTestRssiInfoArray[8];//�������MAC��ַ

		BOOL DongleScanRssiFun(int HandleNum, CString SoftModel);//����ɨ�躯��
		BOOL DongleScanGunOnly(int HandleNum);//����ɨ��ǹ���ܺ���

		CString DongleRssiArray[24][3];//
		int DongleRssiArrayCount;//��������Ŀǰ���е�MAC��ַ����

		BOOL GetDongleScanCheckValue;

	/*��������ͷ��������*/
	public:
		typedef TServerScoket *PServer;
		PServer SocketServer[8];//����һ��Socket��ָ������
		UINT SocketServerPort[8];//Socket��Ķ˿ں�
		CString SocketServerPicName[3][8];//��������ͷͼƬ���֣�Ҳ�ɵ��ļ������֣�ͼƬ���·��Ϊ��.//ͼƬ����/����/�˿ں�/ͼƬ����

		BOOL ShowSocketPicFunction_Thread(CString RecString, int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, CString ChipRfIDbg, BOOL DayTimeStatep);
		void StartShowSocketPic(int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CString RecString);
		BOOL Show_SocketPic_Data(CString RecString, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, int HandleNum, CString StartSign, CString EndSign, CString ChipRfIDbg, BOOL DayTimeStatep);		//��ȡͼ��˿�����
		BOOL Get_SocketWifi_Data(CEdit* m_Result, CEdit* Final_Result_Control, int HandleNum, CString StartSign, CString EndSign, BOOL InThread = FALSE);	//ͨ��Socket��ȡ��ťWIFI����
		BOOL ImageAutoJudgeDarkCorner(int HandleNum,CString ImageSrc,int RGB,float Range);//ͼ�񰵽��жϣ�����1ͼ��·��������2������ֵ��ԽС�Ͷ�Խ�Ժ�ɫ���ж�Խ���ɣ�������3Ϊ��Χ����ȡ��ֵС�������Χ�����棬���򷵻ؼ�
		BOOL ImageAutoJudgeDefinition(int HandleNum,CString ImageSrc, float Range);//ͼ���������жϣ�����1ͼ��·��������2��Χ�����������Χ�����棬С���򷵻ؼ�

		void CreateDirectoryRecursionFun(CString Src, int StartCount=0);//�ݹ鴴��Ŀ¼
		CString GetDatetime();//��ȡ��������
		CString GetCurrenttime();//��ȡ��ǰʱ����
		CString GetTime();//��ȡϵͳʱ��


	/*����������������*/
		CString PowerControlInstrNameArray[2];//�̿ص�Դ�豸����PC�Ĵ�������������������̨����
		CString PowerControlRelayInstrName;//�̵������ں�
		int PowerControlRelayPortInstrName[8];//�̵����˿ں�
		CString BackUpPowerInstrName;//���õ�Դ���ں�
		double m_Voltage;//��ѹֵ
		double m_Range;//����
		BOOL RelayFlag, BackuppowerFlag;

		typedef CurrentTest *PCurrentTest;
		PCurrentTest CurrentTestArray[2];
		deque<int> Current1Deq,Current2Deq;//��ŵ�ǰ�����Լ����ڲ��Եĵ����˿�
		double PortCurrentVauleArray[8];//��Ÿ����˿ڵĵ���ֵ

		void PowerControlInitSetting();//��ʼ���̿ص�Դ��һЩ����������������ַ�ȣ�
		BOOL CurrentMainControlFun(int HandleNum);//�������Ժ��ĺ���
		void SetVoltageAndRangeVaule(CString ConfigItem,double &m_Voltage,double &m_Range);//���õ�ѹ������ֵ�����������������ڵ�ѹ�����̾ͻ�ȡ�����е�ֵ��������������ȡini�е�ֵ
		void SetHighVoltageAndDelayVaule(CString ConfigItem, double &m_HighVoltage, double &m_Delay);
		BOOL CurrentTestFun();//�����������̸�������
		BOOL VoltageTestFun();//��ѹ�������̸�������

		afx_msg void OnBnClickedPowercontrolsettingButton();


	/*�����ֶη��ظ�����*/

		CString AntiDupDataArray[16][AntiDupData];//��ʾ[���ں�][�ֶ�]
		CString AntiDupDataVauleArray[16][AntiDupData];//��ʾ[���ں�][ֵ] ,Ҳ�����ֶζ�Ӧ��ֵ

		CString AntiDupDataLinkOrder[16];//���Linkָ��
        int AntiDupDataLinkFlag[16][2];//0�б�ʾ�������link�Ƿ��Ƕ�Ӧ��ֵ(0�޴�ָ�1��ֵ��ȷ��2��ֵ����)��1�б�ʾ�Ƿ�ִ����дLINK��д�˵Ļ�ʧ�ܺ�Ҫ��Link��Ϊ��
		
		BOOL AntiDupDataSNUploadFlag[16];//��ʾ�Ƿ��ϴ�SN�ţ�ΪTRUEʱ�ϴ�SN��Gps_Data_AntiDup��
		BOOL AntiDupDataNoUploadFlag[16];//��ʾ�Ƿ��ϴ��ֶΣ�ΪTRUEʱ�ϴ�SN��Gps_Data_AntiDup��
	    int AntiDupDataNoUploadCount[16][1];//��ʾҪ�ϴ����ֶ�������ֵԽ���ʾҪ�ϴ����ֶ�Խ��

		void AntiDupDataInit(int HandleNum);//�ֶη��ظ����ܱ�����ʼ��

		//�ϴ��ͼ��Ҫ�ֿ�
		BOOL Data_AntiDupSNCheck(CAdoInterface& myado, int HandleNum, CString ChipIDStr);
		BOOL Data_AntiDupSNUpload(CAdoInterface& myado, int HandleNum, CString ChipIDStr);

		//�ϴ��ͼ��Ҫ�ֿ�
		BOOL Data_AntiDupDataNoCheck(CAdoInterface& myado, int HandleNum, CString ChipIDStr);
		BOOL Data_AntiDupDataNoUpload(CAdoInterface& myado, int HandleNum, CString ChipIDStr);


	/*DAM�̵�����������*/
		BOOL ExternalCircuit[16][9];
		CString ParaItemName[16];//��ǰ����������

		CString UART_CRC16_Work(unsigned char CRC_Buf[], int CRC_Leni);
		CString PowerSet(int Address, bool PowerAction);

		BOOL CloseExternalCircuit(int HandleNum);//�ر����ĺ���
		BOOL DamControlFun(int HandleNum, CString OrderNum);//���ͼ̵����������Ϊ��ʱ��ʾ���ͳɹ���Ϊ��ʱ��ʾ����ʧ��
		BOOL CheckAutoTestChoose;
		afx_msg void OnBnClickedCheck45();



		afx_msg void OnBnClickedDongleblescanCheck();

	/*RDAƽ̨��������*/
		RDAHostInterface RdaHostInterface;


		BOOL GetRDAHostCheckValue;

		void OnGetWebSetting_ThreadFun();//��ȡ�����̺߳���
		afx_msg void OnBnClickedRdahostCheck();
};
