// IMEIWrite_MulAT.cpp : ʵ���ļ�
//
/*
void LogShow_exchange(CEdit* m_Result,CEdit* Final_Result_Control,int State,CString Msg_Log,int HandleNum,CString Category="-1",CString ChipRfIDbg=""); //�ص㿴
void WriteLogIni(int LogNum,CString ChipRfIDbg="");//дLOG��Ϣ
BOOL GetExistReturnCode(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString *ExistMessage,CString ChipRfIDbg="");
BOOL Data_UpdatePara(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL ErrorUpEnable=TRUE,CString ChipRfIDbg="");
BOOL Data_UpdatePara2(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL ErrorUpEnable=TRUE,CString ChipRfIDbg="");			//SMT����
BOOL Data_UpdateError(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString Message,CString Category,CString ChipRfIDbg="");
BOOL GetExistError(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString *ExistMessage,CString ChipRfIDbg="");
BOOL HEX2JPG3(CString str,CString& m_sPath,CString NightDay,int HandleNum,CString ChipRfIDbg="");
*/
#include "stdafx.h"
#include "WriteIMEI.h"
#include "IMEIWrite_MulAT.h"
#include "Login2.h"
#include <Windows.h>
//---------------------------------------------------//
#include <stdio.h>
#include <wtypes.h>
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"ImageDll.lib")

extern "C" int _imp___vsnprintf(
	char *buffer,
	size_t count,
	const char *format,
	va_list argptr
	)
{
	return vsnprintf(buffer, count, format, argptr);
}
//---------------------------------------------------//��vsnprintf���������һ��


//����һϵͳ��Ϣ�����궨��
#define WM_SimpleMessage WM_USER+1001

//����һȫ�ֱ���
map<CString, int>IMEIWrite_MulAT::PortStatusMap;
map<CString, int>IMEIWrite_MulAT::DongleRssiMap;

unsigned char key[] = { 0x20, 0x57, 0x2f, 0x52, 0x36, 0x4b, 0x3f, 0x47, 0x30, 0x50, 0x41, 0x58, 0x11, 0x63, 0x2d, 0x2b };

//ͼ��һ������ֵ�ҵ����ķ�����ģ���ຯ���Ķ��������Ҫ����ͬһ���ļ���
template<typename _MapType>
auto get_map_key_value(const _MapType& input_map, const decltype(input_map.begin()->second)& mapped_value) -> decltype(input_map.begin()->first)
{
	auto iter = std::find_if(input_map.begin(), input_map.end(), [mapped_value](const std::map<CString, int>::value_type item) {
		return (item.second == mapped_value);
	});

	if (iter == input_map.end())
	{
		return decltype(input_map.begin()->first)();
	}
	return iter->first;
}

//�ļ�����
#include "shlwapi.h"

#define GPSNUMBERMAX  16
#define SETTING_ITEM_MAX 72
// IMEIWrite_MulAT �Ի���

IMPLEMENT_DYNAMIC(IMEIWrite_MulAT, CDialog)

IMEIWrite_MulAT::IMEIWrite_MulAT(CWnd* pParent /*=NULL*/)
: CResizableDialog(IMEIWrite_MulAT::IDD, pParent)
, IMEI_InputA(_T(""))
, IMEI_InputB(_T(""))
, IMEI_InputC(_T(""))
, IMEI_InputD(_T(""))
, IMEI_InputE(_T(""))
, IMEI_InputF(_T(""))
, SuccessCount(0)
, FailCount(0)
, CheckSMTChoose(TRUE)
, RTestChoose(FALSE)
, GetPicChoose(FALSE)
, m_GdiplusToken(0)
, BGConfirmChoose(FALSE)
, GPSDataTypeChoose(FALSE)
, PicStaticChoose(FALSE)
, GetBluetoothCheckValue(FALSE)
, BLEGetSettingFlag(-1)
, BLEGetSettingEndFlag(0)
, GetDongleCheckValue(FALSE)
, CheckAutoTestChoose(FALSE)
, GetDongleScanCheckValue(FALSE)
, GetRDAHostCheckValue(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GdiplusStartup(&m_GdiplusToken, &m_GdiplusStartupInput, NULL);

	//���������һ�����ĸ���ҪĬ��ΪFALSE
	if (g_ADCTFlag == 1)
	{
		CheckSMTChoose = FALSE;
		GetPicChoose = FALSE;
		BGConfirmChoose = FALSE;
		PicStaticChoose = FALSE;
		CheckAutoTestChoose = FALSE;
	}

	//ΪSocket��ָ������ʵ���ͶԳ�ʼ����صı���
	for (int k = 0; k < 8; k++)
	{
		SocketServer[k] = new TServerScoket();
		SocketServerPort[k] = 10460 + k;
	}
	//SocketServerPicName[0] = _T("Port1");
	//SocketServerPicName[1] = _T("Port2");
	//SocketServerPicName[2] = _T("Port3");
	//SocketServerPicName[3] = _T("Port4");
	//SocketServerPicName[4] = _T("Port5");
	//SocketServerPicName[5] = _T("Port6");
	//SocketServerPicName[6] = _T("Port7");
	//SocketServerPicName[7] = _T("Port8");


	////Ϊ������������ʾ���ͳ�ʼ����ص�����
	//for (int i = 0; i < 8; i++)
	//{
	//	CurrentTestArray[i] = new CurrentTest();
	//}


	//m_ResultArray��Final_Result_ControlArray����Ϊ�˷��㴫��־�õ�
	m_ResultArray[0] = &m_Result1;
	m_ResultArray[1] = &m_Result2;
	m_ResultArray[2] = &m_Result3;
	m_ResultArray[3] = &m_Result4;
	m_ResultArray[4] = &m_Result5;
	m_ResultArray[5] = &m_Result6;
	m_ResultArray[6] = &m_Result7;
	m_ResultArray[7] = &m_Result8;
	m_ResultArray[8] = &m_Result9;
	m_ResultArray[9] = &m_Result10;
	m_ResultArray[10] = &m_Result11;
	m_ResultArray[11] = &m_Result12;
	m_ResultArray[12] = &m_Result13;
	m_ResultArray[13] = &m_Result14;
	m_ResultArray[14] = &m_Result15;
	m_ResultArray[15] = &m_Result16;

	Final_Result_ControlArray[0] = &Final_Result_Control1;
	Final_Result_ControlArray[1] = &Final_Result_Control2;
	Final_Result_ControlArray[2] = &Final_Result_Control3;
	Final_Result_ControlArray[3] = &Final_Result_Control4;
	Final_Result_ControlArray[4] = &Final_Result_Control5;
	Final_Result_ControlArray[5] = &Final_Result_Control6;
	Final_Result_ControlArray[6] = &Final_Result_Control7;
	Final_Result_ControlArray[7] = &Final_Result_Control8;
	Final_Result_ControlArray[8] = &Final_Result_Control9;
	Final_Result_ControlArray[9] = &Final_Result_Control10;
	Final_Result_ControlArray[10] = &Final_Result_Control11;
	Final_Result_ControlArray[11] = &Final_Result_Control12;
	Final_Result_ControlArray[12] = &Final_Result_Control13;
	Final_Result_ControlArray[13] = &Final_Result_Control14;
	Final_Result_ControlArray[14] = &Final_Result_Control15;
	Final_Result_ControlArray[15] = &Final_Result_Control16;

	m_StopControlArray[0] = &Stop1_Control;
	m_StopControlArray[1] = &Stop2_Control;
	m_StopControlArray[2] = &Stop3_Control;
	m_StopControlArray[3] = &Stop4_Control;
	m_StopControlArray[4] = &Stop5_Control;
	m_StopControlArray[5] = &Stop6_Control;
	m_StopControlArray[6] = &Stop7_Control;
	m_StopControlArray[7] = &Stop8_Control;
	m_StopControlArray[8] = &Stop9_Control;
	m_StopControlArray[9] = &Stop10_Control;
	m_StopControlArray[10] = &Stop11_Control;
	m_StopControlArray[11] = &Stop12_Control;
	m_StopControlArray[12] = &Stop13_Control;
	m_StopControlArray[13] = &Stop14_Control;
	m_StopControlArray[14] = &Stop15_Control;
	m_StopControlArray[15] = &Stop16_Control;

	m_StartControlArray[0] = &StartA_Control;
	m_StartControlArray[1] = &StartB_Control;
	m_StartControlArray[2] = &StartC_Control;
	m_StartControlArray[3] = &StartD_Control;
	m_StartControlArray[4] = &StartE_Control;
	m_StartControlArray[5] = &StartF_Control;
	m_StartControlArray[6] = &Start7_Control;
	m_StartControlArray[7] = &Start8_Control;
	m_StartControlArray[8] = &Start9_Control;
	m_StartControlArray[9] = &Start10_Control;
	m_StartControlArray[10] = &Start11_Control;
	m_StartControlArray[11] = &Start12_Control;
	m_StartControlArray[12] = &Start13_Control;
	m_StartControlArray[13] = &Start14_Control;
	m_StartControlArray[14] = &Start15_Control;
	m_StartControlArray[15] = &Start16_Control;

	m_ShowNumberPortControlArray[0] = &m_ShowNumberPort1Control;
	m_ShowNumberPortControlArray[1] = &m_ShowNumberPort2Control;
	m_ShowNumberPortControlArray[2] = &m_ShowNumberPort3Control;
	m_ShowNumberPortControlArray[3] = &m_ShowNumberPort4Control;
	m_ShowNumberPortControlArray[4] = &m_ShowNumberPort5Control;
	m_ShowNumberPortControlArray[5] = &m_ShowNumberPort6Control;
	m_ShowNumberPortControlArray[6] = &m_ShowNumberPort7Control;
	m_ShowNumberPortControlArray[7] = &m_ShowNumberPort8Control;

	//Dongle����һЩ��־λ�Ͳ������г�ʼ��
	for (int i = 0; i < 16; i++)
	{
		DongleConnectFlag[i] = FALSE;
		DongleConnectCount[i] = 0;
		DongleCheckRssiFlag[i] = FALSE;
		DongleInfo[i][0] = "";
		DongleInfo[i][1] = "";
	}

	for (int i = 0; i < 8; i++)
	{
		DongleTestRssiInfoArray[i] = "";
		DongleStatusArray[i] = FALSE;
	}
}

IMEIWrite_MulAT::~IMEIWrite_MulAT()
{
	DeleteCriticalSection(&GETPORT);
	DeleteCriticalSection(&CPUCOMMUNICATE);
	DeleteCriticalSection(&SHOWONEPIC);
	DeleteCriticalSection(&UseScanResult);
	DeleteCriticalSection(&SPEAKVOICE);

	DeleteCriticalSection(&SUCCFAILCOUNT);
	for (int i = 0; i<THREAD_NUM / 4; i++)
	{
		DeleteCriticalSection(&UPDATEDB[i]);
	}
	for (int j = 0; j<THREAD_NUM / 2; j++)
	{
		DeleteCriticalSection(&WIFICOMMUNICATE[j]);
	}
	FreeLibrary(password_Handle);
	GdiplusShutdown(m_GdiplusToken);
}

void IMEIWrite_MulAT::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Port1);
	DDX_Control(pDX, IDC_COMBO2, m_Baud1);
	DDX_Control(pDX, IDC_FINAL_RESULT1, Final_Result_Control1);
	DDX_Control(pDX, IDC_RESULT, m_Result1);
	DDX_Text(pDX, IDC_IMEIA, IMEI_InputA);
	DDX_Control(pDX, IDC_IMEIA, IMEI_InputA_Control);
	DDX_Control(pDX, IDC_FINAL_RESULT2, Final_Result_Control2);
	DDX_Control(pDX, IDC_RESULT2, m_Result2);
	DDX_Text(pDX, IDC_IMEIA2, IMEI_InputB);
	DDX_Control(pDX, IDC_IMEIA2, IMEI_InputB_Control);
	DDX_Control(pDX, IDC_COMBO4, m_Port2);
	DDX_Control(pDX, IDC_COMBO5, m_Baud2);
	DDX_Control(pDX, IDC_FINAL_RESULT3, Final_Result_Control3);
	DDX_Control(pDX, IDC_RESULT3, m_Result3);
	DDX_Control(pDX, IDC_COMBO6, m_Port3);
	DDX_Control(pDX, IDC_COMBO7, m_Baud3);
	DDX_Text(pDX, IDC_IMEIA3, IMEI_InputC);
	DDX_Control(pDX, IDC_IMEIA3, IMEI_InputC_Control);
	DDX_Control(pDX, IDC_FINAL_RESULT4, Final_Result_Control4);
	DDX_Control(pDX, IDC_RESULT4, m_Result4);
	DDX_Control(pDX, IDC_COMBO8, m_Port4);
	DDX_Control(pDX, IDC_COMBO9, m_Baud4);
	DDX_Text(pDX, IDC_IMEIA4, IMEI_InputD);
	DDX_Control(pDX, IDC_IMEIA4, IMEI_InputD_Control);
	DDX_Control(pDX, IDC_FINAL_RESULT5, Final_Result_Control5);
	DDX_Control(pDX, IDC_RESULT5, m_Result5);
	DDX_Control(pDX, IDC_COMBO10, m_Port5);
	DDX_Control(pDX, IDC_COMBO11, m_Baud5);
	DDX_Text(pDX, IDC_IMEIA5, IMEI_InputE);
	DDX_Control(pDX, IDC_IMEIA5, IMEI_InputE_Control);
	DDX_Control(pDX, IDC_FINAL_RESULT6, Final_Result_Control6);
	DDX_Control(pDX, IDC_RESULT6, m_Result6);
	DDX_Control(pDX, IDC_COMBO12, m_Port6);
	DDX_Control(pDX, IDC_COMBO13, m_Baud6);
	DDX_Text(pDX, IDC_IMEIA6, IMEI_InputF);
	DDX_Control(pDX, IDC_IMEIA6, IMEI_InputF_Control);
	DDX_Control(pDX, IDC_BUTTON1, StartA_Control);
	DDX_Control(pDX, IDC_BUTTON14, StartB_Control);
	DDX_Control(pDX, IDC_BUTTON16, StartC_Control);
	DDX_Control(pDX, IDC_BUTTON18, StartD_Control);
	DDX_Control(pDX, IDC_BUTTON20, StartE_Control);
	DDX_Control(pDX, IDC_BUTTON22, StartF_Control);
	DDX_Control(pDX, IDC_COMBO14, ScanGunPort1);
	DDX_Control(pDX, IDC_COMBO18, ScanGunPort2);
	DDX_Control(pDX, IDC_COMBO20, ScanGunPort3);
	DDX_Control(pDX, IDC_COMBO16, ScanGunPort4);
	DDX_Control(pDX, IDC_COMBO22, ScanGunPort5);
	DDX_Control(pDX, IDC_COMBO24, ScanGunPort6);
	DDX_Control(pDX, IDC_COMBO15, ScanGunBaud1);
	DDX_Control(pDX, IDC_COMBO19, ScanGunBaud2);
	DDX_Control(pDX, IDC_COMBO21, ScanGunBaud3);
	DDX_Control(pDX, IDC_COMBO17, ScanGunBaud4);
	DDX_Control(pDX, IDC_COMBO23, ScanGunBaud5);
	DDX_Control(pDX, IDC_COMBO25, ScanGunBaud6);
	DDX_Control(pDX, IDC_COMBO26, ScanGunTpye);
	DDX_Control(pDX, IDC_COMBO27, PrintSelect);
	DDX_Control(pDX, IDC_COMBO28, PortType);
	DDX_Control(pDX, IDC_FINAL_RESULT7, Final_Result_Control7);
	DDX_Control(pDX, IDC_FINAL_RESULT8, Final_Result_Control8);
	DDX_Control(pDX, IDC_FINAL_RESULT9, Final_Result_Control9);
	DDX_Control(pDX, IDC_FINAL_RESULT10, Final_Result_Control10);
	DDX_Control(pDX, IDC_FINAL_RESULT11, Final_Result_Control11);
	DDX_Control(pDX, IDC_FINAL_RESULT12, Final_Result_Control12);
	DDX_Control(pDX, IDC_FINAL_RESULT13, Final_Result_Control13);
	DDX_Control(pDX, IDC_FINAL_RESULT14, Final_Result_Control14);
	DDX_Control(pDX, IDC_FINAL_RESULT15, Final_Result_Control15);
	DDX_Control(pDX, IDC_FINAL_RESULT16, Final_Result_Control16);
	DDX_Control(pDX, IDC_RESULT7, m_Result7);
	DDX_Control(pDX, IDC_RESULT8, m_Result8);
	DDX_Control(pDX, IDC_RESULT9, m_Result9);
	DDX_Control(pDX, IDC_RESULT10, m_Result10);
	DDX_Control(pDX, IDC_RESULT11, m_Result11);
	DDX_Control(pDX, IDC_RESULT12, m_Result12);
	DDX_Control(pDX, IDC_RESULT13, m_Result13);
	DDX_Control(pDX, IDC_RESULT14, m_Result14);
	DDX_Control(pDX, IDC_RESULT15, m_Result15);
	DDX_Control(pDX, IDC_RESULT16, m_Result16);
	DDX_Control(pDX, IDC_COMBO29, m_Port7);
	DDX_Control(pDX, IDC_COMBO31, m_Port8);
	DDX_Control(pDX, IDC_COMBO33, m_Port9);
	DDX_Control(pDX, IDC_COMBO35, m_Port10);
	DDX_Control(pDX, IDC_COMBO37, m_Port11);
	DDX_Control(pDX, IDC_COMBO39, m_Port12);
	DDX_Control(pDX, IDC_COMBO41, m_Port13);
	DDX_Control(pDX, IDC_COMBO43, m_Port14);
	DDX_Control(pDX, IDC_COMBO45, m_Port15);
	DDX_Control(pDX, IDC_COMBO47, m_Port16);
	DDX_Control(pDX, IDC_COMBO30, m_Baud7);
	DDX_Control(pDX, IDC_COMBO32, m_Baud8);
	DDX_Control(pDX, IDC_COMBO34, m_Baud9);
	DDX_Control(pDX, IDC_COMBO36, m_Baud10);
	DDX_Control(pDX, IDC_COMBO38, m_Baud11);
	DDX_Control(pDX, IDC_COMBO40, m_Baud12);
	DDX_Control(pDX, IDC_COMBO42, m_Baud13);
	DDX_Control(pDX, IDC_COMBO44, m_Baud14);
	DDX_Control(pDX, IDC_COMBO46, m_Baud15);
	DDX_Control(pDX, IDC_COMBO48, m_Baud16);
	DDX_Control(pDX, IDC_BUTTONSTART7, Start7_Control);
	DDX_Control(pDX, IDC_BUTTONSTART8, Start8_Control);
	DDX_Control(pDX, IDC_BUTTONSTART9, Start9_Control);
	DDX_Control(pDX, IDC_BUTTONSTART10, Start10_Control);
	DDX_Control(pDX, IDC_BUTTONSTART11, Start11_Control);
	DDX_Control(pDX, IDC_BUTTONSTART12, Start12_Control);
	DDX_Control(pDX, IDC_BUTTONSTART13, Start13_Control);
	DDX_Control(pDX, IDC_BUTTONSTART14, Start14_Control);
	DDX_Control(pDX, IDC_BUTTONSTART15, Start15_Control);
	DDX_Control(pDX, IDC_BUTTONSTART16, Start16_Control);
	DDX_Control(pDX, IDC_COMBO49, CPUControlPort);
	DDX_Control(pDX, IDC_COMBO50, CPUControlBaud);
	DDX_Control(pDX, IDC_COMBO51, OrderNumbersControl);
	DDX_Control(pDX, IDOK, StartALL_Control);
	DDX_Control(pDX, IDCANCEL, StopALL_Control);
	DDX_Control(pDX, IDC_COMBO52, GPS_SNRLimit);
	DDX_Text(pDX, IDC_EDITSUCCOUNT, SuccessCount);
	DDX_Text(pDX, IDC_EDITFAILCOUNT, FailCount);
	DDX_Control(pDX, IDC_COMBO53, ReturnCodeControl);
	DDX_Control(pDX, IDC_COMBO54, VIControlPort);
	DDX_Control(pDX, IDC_COMBO55, VIControlBaud);
	DDX_Control(pDX, IDC_COMBO56, WorkStationControl);
	DDX_Check(pDX, IDC_CHECK39, CheckSMTChoose);
	DDX_Check(pDX, IDC_CHECK45, CheckAutoTestChoose);
	DDX_Check(pDX, IDC_CHECK40, RTestChoose);
	DDX_Check(pDX, IDC_CHECK41, GetPicChoose);
	DDX_Check(pDX, IDC_CHECK42, BGConfirmChoose);
	DDX_Check(pDX, IDC_CHECK43, GPSDataTypeChoose);
	DDX_Control(pDX, IDC_COMBO57, WIFI_RSSILimit);
	DDX_Check(pDX, IDC_CHECK44, PicStaticChoose);
	DDX_Check(pDX, IDC_BLUETOOTH_CHECK, GetBluetoothCheckValue);
	DDX_Check(pDX, IDC_DongleBle_CHECK, GetDongleCheckValue);
	DDX_Check(pDX, IDC_DongleBleScan_CHECK, GetDongleScanCheckValue);
	DDX_Control(pDX, IDC_BUTTON2, Stop1_Control);
	DDX_Control(pDX, IDC_BUTTON15, Stop2_Control);
	DDX_Control(pDX, IDC_BUTTON17, Stop3_Control);
	DDX_Control(pDX, IDC_BUTTON19, Stop4_Control);
	DDX_Control(pDX, IDC_BUTTON21, Stop5_Control);
	DDX_Control(pDX, IDC_BUTTON23, Stop6_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP7, Stop7_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP8, Stop8_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP9, Stop9_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP10, Stop10_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP11, Stop11_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP12, Stop12_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP13, Stop13_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP14, Stop14_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP15, Stop15_Control);
	DDX_Control(pDX, IDC_BUTTONSTOP16, Stop16_Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT1_EDIT, m_ShowNumberPort1Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT2_EDIT, m_ShowNumberPort2Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT3_EDIT, m_ShowNumberPort3Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT4_EDIT, m_ShowNumberPort4Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT5_EDIT, m_ShowNumberPort5Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT6_EDIT, m_ShowNumberPort6Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT7_EDIT, m_ShowNumberPort7Control);
	DDX_Control(pDX, IDC_SHOWNUMBERPORT8_EDIT, m_ShowNumberPort8Control);
	DDX_Check(pDX, IDC_RDAHost_CHECK, GetRDAHostCheckValue);
}


BEGIN_MESSAGE_MAP(IMEIWrite_MulAT, CResizableDialog)
	ON_WM_CTLCOLOR()//�ı���ɫ
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()//PostMessage

	ON_WM_CLOSE()

	ON_WM_RBUTTONUP()   //�Ҽ�����
	ON_WM_CONTEXTMENU() //�˵�

	ON_COMMAND(ID_FRESH_32771, OnCbnSelchangeCombo2_Fresh)//ˢ�¶˿�
	ON_COMMAND(ID_32776, OnGetWebSetting)//��ȡ��������
	ON_COMMAND(ID_32772, OnBnClickedButton6)

	ON_COMMAND(ID_32773START, OnBnClickedButton42)
	ON_COMMAND(ID_32777STOP, OnBnClickedButton40)

	ON_WM_TIMER()   //��ʱ��

	ON_EN_CHANGE(IDC_IMEIA, &IMEIWrite_MulAT::OnEnChangeImeia)
	ON_BN_CLICKED(IDC_BUTTON1, &IMEIWrite_MulAT::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &IMEIWrite_MulAT::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_IMEIA2, &IMEIWrite_MulAT::OnEnChangeImeia2)
	ON_BN_CLICKED(IDC_BUTTON14, &IMEIWrite_MulAT::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &IMEIWrite_MulAT::OnBnClickedButton15)
	ON_EN_CHANGE(IDC_IMEIA3, &IMEIWrite_MulAT::OnEnChangeImeia3)
	ON_BN_CLICKED(IDC_BUTTON16, &IMEIWrite_MulAT::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &IMEIWrite_MulAT::OnBnClickedButton17)
	ON_EN_CHANGE(IDC_IMEIA4, &IMEIWrite_MulAT::OnEnChangeImeia4)
	ON_BN_CLICKED(IDC_BUTTON18, &IMEIWrite_MulAT::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &IMEIWrite_MulAT::OnBnClickedButton19)
	ON_EN_CHANGE(IDC_IMEIA5, &IMEIWrite_MulAT::OnEnChangeImeia5)
	ON_BN_CLICKED(IDC_BUTTON20, &IMEIWrite_MulAT::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &IMEIWrite_MulAT::OnBnClickedButton21)
	ON_EN_CHANGE(IDC_IMEIA6, &IMEIWrite_MulAT::OnEnChangeImeia6)
	ON_BN_CLICKED(IDC_BUTTON22, &IMEIWrite_MulAT::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &IMEIWrite_MulAT::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON6, &IMEIWrite_MulAT::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &IMEIWrite_MulAT::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON3, &IMEIWrite_MulAT::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON25, &IMEIWrite_MulAT::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON26, &IMEIWrite_MulAT::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON27, &IMEIWrite_MulAT::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON28, &IMEIWrite_MulAT::OnBnClickedButton28)
	ON_BN_CLICKED(IDC_BUTTON29, &IMEIWrite_MulAT::OnBnClickedButton29)
	ON_BN_CLICKED(IDC_BUTTON30, &IMEIWrite_MulAT::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON31, &IMEIWrite_MulAT::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON32, &IMEIWrite_MulAT::OnBnClickedButton32)
	ON_BN_CLICKED(IDC_BUTTON33, &IMEIWrite_MulAT::OnBnClickedButton33)
	ON_BN_CLICKED(IDC_BUTTON34, &IMEIWrite_MulAT::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON35, &IMEIWrite_MulAT::OnBnClickedButton35)
	ON_BN_CLICKED(IDC_BUTTON10, &IMEIWrite_MulAT::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON37, &IMEIWrite_MulAT::OnBnClickedButton37)
	ON_BN_CLICKED(IDC_BUTTON38, &IMEIWrite_MulAT::OnBnClickedButton38)
	ON_BN_CLICKED(IDOK, &IMEIWrite_MulAT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &IMEIWrite_MulAT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON40, &IMEIWrite_MulAT::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_BUTTON42, &IMEIWrite_MulAT::OnBnClickedButton42)
	ON_BN_CLICKED(IDC_BUTTONSTART7, &IMEIWrite_MulAT::OnBnClickedButtonstart7)
	ON_BN_CLICKED(IDC_BUTTONSTOP7, &IMEIWrite_MulAT::OnBnClickedButtonstop7)
	ON_BN_CLICKED(IDC_BUTTONSTART8, &IMEIWrite_MulAT::OnBnClickedButtonstart8)
	ON_BN_CLICKED(IDC_BUTTONSTART9, &IMEIWrite_MulAT::OnBnClickedButtonstart9)
	ON_BN_CLICKED(IDC_BUTTONSTART10, &IMEIWrite_MulAT::OnBnClickedButtonstart10)
	ON_BN_CLICKED(IDC_BUTTONSTART11, &IMEIWrite_MulAT::OnBnClickedButtonstart11)
	ON_BN_CLICKED(IDC_BUTTONSTART12, &IMEIWrite_MulAT::OnBnClickedButtonstart12)
	ON_BN_CLICKED(IDC_BUTTONSTART13, &IMEIWrite_MulAT::OnBnClickedButtonstart13)
	ON_BN_CLICKED(IDC_BUTTONSTART14, &IMEIWrite_MulAT::OnBnClickedButtonstart14)
	ON_BN_CLICKED(IDC_BUTTONSTART15, &IMEIWrite_MulAT::OnBnClickedButtonstart15)
	ON_BN_CLICKED(IDC_BUTTONSTART16, &IMEIWrite_MulAT::OnBnClickedButtonstart16)
	ON_BN_CLICKED(IDC_BUTTONSTOP8, &IMEIWrite_MulAT::OnBnClickedButtonstop8)
	ON_BN_CLICKED(IDC_BUTTONSTOP9, &IMEIWrite_MulAT::OnBnClickedButtonstop9)
	ON_BN_CLICKED(IDC_BUTTONSTOP10, &IMEIWrite_MulAT::OnBnClickedButtonstop10)
	ON_BN_CLICKED(IDC_BUTTONSTOP11, &IMEIWrite_MulAT::OnBnClickedButtonstop11)
	ON_BN_CLICKED(IDC_BUTTONSTOP12, &IMEIWrite_MulAT::OnBnClickedButtonstop12)
	ON_BN_CLICKED(IDC_BUTTONSTOP13, &IMEIWrite_MulAT::OnBnClickedButtonstop13)
	ON_BN_CLICKED(IDC_BUTTONSTOP14, &IMEIWrite_MulAT::OnBnClickedButtonstop14)
	ON_BN_CLICKED(IDC_BUTTONSTOP15, &IMEIWrite_MulAT::OnBnClickedButtonstop15)
	ON_BN_CLICKED(IDC_BUTTONSTOP16, &IMEIWrite_MulAT::OnBnClickedButtonstop16)
	ON_BN_CLICKED(IDC_BUTTONCONNCPU, &IMEIWrite_MulAT::OnBnClickedButtonconncpu)
	ON_BN_CLICKED(IDC_BUTTONDISCONNCPU, &IMEIWrite_MulAT::OnBnClickedButtondisconncpu)
	ON_BN_CLICKED(IDC_BUTTONCONNCPU2, &IMEIWrite_MulAT::OnBnClickedButtonconncpu2)
	ON_BN_CLICKED(IDC_BUTTONDISCONNCPU2, &IMEIWrite_MulAT::OnBnClickedButtondisconncpu2)
	ON_BN_CLICKED(IDC_CHECK39, &IMEIWrite_MulAT::OnBnClickedCheck39)
	ON_BN_CLICKED(IDC_CHECK45, &IMEIWrite_MulAT::OnBnClickedCheck45)
	ON_BN_CLICKED(IDC_CHECK40, &IMEIWrite_MulAT::OnBnClickedCheck40)
	ON_CBN_SELCHANGE(IDC_COMBO53, &IMEIWrite_MulAT::OnCbnSelchangeCombo53)
	ON_BN_CLICKED(IDC_CHECK41, &IMEIWrite_MulAT::OnBnClickedCheck41)
	ON_BN_CLICKED(IDC_CHECK42, &IMEIWrite_MulAT::OnBnClickedCheck42)
	ON_BN_CLICKED(IDC_CHECK43, &IMEIWrite_MulAT::OnBnClickedCheck43)
	ON_CBN_SELCHANGE(IDC_COMBO56, &IMEIWrite_MulAT::OnCbnSelchangeCombo56)

	//����һϵͳ��Ϣ�����궨��
	ON_MESSAGE(WM_SimpleMessage, &IMEIWrite_MulAT::MSG_GetSimpleMessage)
	ON_WM_COPYDATA()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BLUETOOTH_CHECK, &IMEIWrite_MulAT::OnBnClickedBluetoothCheck)
	ON_BN_CLICKED(IDC_RESETBLE_BUTTON, &IMEIWrite_MulAT::OnBnClickedResetbleButton)
	ON_BN_CLICKED(IDC_DongleBle_CHECK, &IMEIWrite_MulAT::OnBnClickedDonglebleCheck)
	ON_BN_CLICKED(IDC_POWERCONTROLSETTING_BUTTON, &IMEIWrite_MulAT::OnBnClickedPowercontrolsettingButton)
	ON_BN_CLICKED(IDC_DongleBleScan_CHECK, &IMEIWrite_MulAT::OnBnClickedDongleblescanCheck)
	ON_BN_CLICKED(IDC_RDAHost_CHECK, &IMEIWrite_MulAT::OnBnClickedRdahostCheck)
END_MESSAGE_MAP()


// IMEIWrite_MulAT ��Ϣ�������

BOOL IMEIWrite_MulAT::OnInitDialog()//��ʼ������
{
	CResizableDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	COM_IniDone = FALSE;
	for (int j = 0; j<THREAD_NUM; j++)
	{
		hPort[j] = NULL;
		hScanGun[j] = NULL;
		hPicPort[j] = NULL;
		Vaule_Return_Count_CS[j] = "";

		BGShowPicTime[j] = "7000";
		BGShowPicCount[j] = 50;
	}
	hPort[THREAD_NUM] = NULL;
	for (int j = 0; j<PORTS_NUM; j++)
	{
		COM_Number[j] = "";
	}
	InitCOM(&m_Port1, &m_Baud1, 0);//��ʼ������
	InitCOM(&m_Port2, &m_Baud2, 1);//��ʼ������--LBC
	InitCOM(&m_Port3, &m_Baud3, 2);
	InitCOM(&m_Port4, &m_Baud4, 3);
	InitCOM(&m_Port5, &m_Baud5, 4);
	InitCOM(&m_Port6, &m_Baud6, 5);
	InitCOM(&m_Port7, &m_Baud7, 6);
	InitCOM(&m_Port8, &m_Baud8, 7);
	InitCOM(&m_Port9, &m_Baud9, 8);
	InitCOM(&m_Port10, &m_Baud10, 9);
	InitCOM(&m_Port11, &m_Baud11, 10);
	InitCOM(&m_Port12, &m_Baud12, 11);
	InitCOM(&m_Port13, &m_Baud13, 12);
	InitCOM(&m_Port14, &m_Baud14, 13);
	InitCOM(&m_Port15, &m_Baud15, 14);
	InitCOM(&m_Port16, &m_Baud16, 15);

	InitCOM(&CPUControlPort, &CPUControlBaud, 16);
	CPUControlBaud.SetWindowText("9600");
	InitCOM(&VIControlPort, &VIControlBaud, 17);

	InitCOM(&ScanGunPort1, &ScanGunBaud1, 18);//��ʼ��ɨ��ǹ����
	InitCOM(&ScanGunPort2, &ScanGunBaud2, 1);
	InitCOM(&ScanGunPort3, &ScanGunBaud3, 2);
	InitCOM(&ScanGunPort4, &ScanGunBaud4, 3);
	InitCOM(&ScanGunPort5, &ScanGunBaud5, 4);
	InitCOM(&ScanGunPort6, &ScanGunBaud6, 5);

	ScanGunTpye.SetCurSel(0);
	PrintSelect.SetCurSel(0);

	HBRUSH mBrush = NULL;//��ˢ ScanGunPort1

	font = new CFont;
	font->CreateFont(50, 0, 0, 0, 600, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"arial");

	fontsp = new CFont;
	fontsp->CreateFontA(30, 0, 0, 0, 600, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"arial");

	font_fail = new CFont;
	font_fail->CreateFont(33, 0, 0, 0, 600, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"arial");

	GetDlgItem(IDC_FINAL_RESULT1)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT2)->SetFont(font);//--LBC
	GetDlgItem(IDC_FINAL_RESULT3)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT4)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT5)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT6)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT7)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT8)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT9)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT10)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT11)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT12)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT13)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT14)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT15)->SetFont(font);
	GetDlgItem(IDC_FINAL_RESULT16)->SetFont(font);


	//set the IMEI input boxfont
	CFont* ffont;
	ffont = new CFont;
	ffont->CreateFont(20, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"arial");
	//GetDlgItem(IDC_IMEIA)->SetFont(ffont);		//IMEI1
	//GetDlgItem(IDC_IMEIA2)->SetFont(ffont);		//IMEI2--LBC
	//GetDlgItem(IDC_IMEIA3)->SetFont(ffont);
	//GetDlgItem(IDC_IMEIA4)->SetFont(ffont);
	//GetDlgItem(IDC_IMEIA5)->SetFont(ffont);
	//GetDlgItem(IDC_IMEIA6)->SetFont(ffont);
	GetDlgItem(IDC_EDITSUCCOUNT)->SetFont(ffont);
	GetDlgItem(IDC_EDITFAILCOUNT)->SetFont(ffont);

	AddAnchor(IDC_BUTTON42, TOP_LEFT);
	AddAnchor(IDC_BUTTON40, TOP_LEFT);
	AddAnchor(IDOK, TOP_LEFT);
	AddAnchor(IDCANCEL, TOP_LEFT);
	AddAnchor(IDC_BUTTON7, TOP_LEFT);
	AddAnchor(IDC_BUTTON6, TOP_LEFT);

	AddAnchor(IDC_COMBOTESTYYPE2, TOP_LEFT);
	AddAnchor(IDC_COMBOTESTYYPE3, TOP_LEFT);
	AddAnchor(IDC_COMBOTESTYYPE4, TOP_LEFT);

	AddAnchor(IDC_STATICSCAN, TOP_LEFT);
	AddAnchor(IDC_COMBO49, TOP_LEFT);
	AddAnchor(IDC_COMBO50, TOP_LEFT);
	AddAnchor(IDC_COMBOTESTYYPE, TOP_LEFT);
	AddAnchor(IDC_BUTTONCONNCPU, TOP_LEFT);
	AddAnchor(IDC_BUTTONDISCONNCPU, TOP_LEFT);
	AddAnchor(IDC_STATICCPU, TOP_LEFT);
	AddAnchor(IDC_COMBO14, TOP_LEFT);
	AddAnchor(IDC_COMBO15, TOP_LEFT);
	AddAnchor(IDC_SCANDATA_EDIT, TOP_LEFT4);
	AddAnchor(IDC_SCANDATA_STATIC, TOP_LEFT4);
	AddAnchor(IDC_BUTTON3, TOP_LEFT);
	AddAnchor(IDC_BUTTON25, TOP_LEFT);
	AddAnchor(IDC_RESETBLE_BUTTON, TOP_LEFT4);
	AddAnchor(IDC_STATICCONFIG, TOP_LEFT);
	AddAnchor(IDC_POWERCONTROLSETTING_BUTTON, TOP_LEFT);

	AddAnchor(IDC_STATICCPU2, TOP_LEFT);
	AddAnchor(IDC_COMBO54, TOP_LEFT);
	AddAnchor(IDC_COMBO55, TOP_LEFT);
	AddAnchor(IDC_BUTTONCONNCPU2, TOP_LEFT);
	AddAnchor(IDC_BUTTONDISCONNCPU2, TOP_LEFT);

	AddAnchor(IDC_STATICPIC, TOP_CENTER);
	AddAnchor(IDC_STATICORDER, TOP_CENTER);
	AddAnchor(IDC_COMBO51, TOP_CENTER);
	AddAnchor(IDC_COMBO28, TOP_CENTER);

	AddAnchor(IDC_STATICRETURN, TOP_CENTER);
	AddAnchor(IDC_COMBO53, TOP_CENTER);

	AddAnchor(IDC_COMBO56, TOP_CENTER);
	AddAnchor(IDC_CHECK39, TOP_CENTER);
	AddAnchor(IDC_CHECK40, TOP_CENTER);
	AddAnchor(IDC_CHECK41, TOP_CENTER);
	AddAnchor(IDC_CHECK42, TOP_CENTER);
	AddAnchor(IDC_CHECK44, TOP_CENTER);
	AddAnchor(IDC_CHECK45, TOP_CENTER);
	AddAnchor(IDC_BLUETOOTH_CHECK, TOP_CENTER);
	AddAnchor(IDC_DongleBle_CHECK, TOP_CENTER);
	AddAnchor(IDC_DongleBleScan_CHECK, TOP_CENTER);
	AddAnchor(IDC_RDAHost_CHECK, TOP_CENTER);
	AddAnchor(IDC_CHECK43, TOP_LEFT4);//AddAnchor(IDC_CHECK43, TOP_CENTER);

	AddAnchor(IDC_STATICSUCCOUNT, TOP_CENTER);
	AddAnchor(IDC_STATICFAILCOUNT, TOP_CENTER);
	AddAnchor(IDC_EDITSUCCOUNT, TOP_CENTER);
	AddAnchor(IDC_EDITFAILCOUNT, TOP_CENTER);

	AddAnchor(IDC_STATICSNR, TOP_LEFT4);
	AddAnchor(IDC_COMBO52, TOP_LEFT4, TOP_RIGHT);

	AddAnchor(IDC_STATICSNR2, TOP_LEFT4);
	AddAnchor(IDC_COMBO57, TOP_LEFT4, TOP_RIGHT);

	//���
	AddAnchor(IDC_STATICTHREAD1, TOP_LEFT, MIDDLE_LEFT2);
	AddAnchor(IDC_STATICTHREAD2, TOP_LEFT2, MIDDLE_LEFT3);
	AddAnchor(IDC_STATICTHREAD3, TOP_LEFT3, MIDDLE_LEFT4);
	AddAnchor(IDC_STATICTHREAD4, TOP_LEFT4, MIDDLE_LEFT5);
	AddAnchor(IDC_STATICTHREAD5, TOP_LEFT6, MIDDLE_LEFT7);
	AddAnchor(IDC_STATICTHREAD6, TOP_LEFT7, MIDDLE_LEFT8);
	AddAnchor(IDC_STATICTHREAD7, TOP_LEFT8, MIDDLE_LEFT9);
	AddAnchor(IDC_STATICTHREAD8, TOP_LEFT9, MIDDLE_LEFT10);
	AddAnchor(IDC_STATICTHREAD9, MIDDLE_LEFT, BOTTOM_LEFT2);
	AddAnchor(IDC_STATICTHREAD10, MIDDLE_LEFT2, BOTTOM_LEFT3);
	AddAnchor(IDC_STATICTHREAD11, MIDDLE_LEFT3, BOTTOM_LEFT4);
	AddAnchor(IDC_STATICTHREAD12, MIDDLE_LEFT4, BOTTOM_LEFT5);
	AddAnchor(IDC_STATICTHREAD13, MIDDLE_LEFT6, BOTTOM_LEFT7);
	AddAnchor(IDC_STATICTHREAD14, MIDDLE_LEFT7, BOTTOM_LEFT8);
	AddAnchor(IDC_STATICTHREAD15, MIDDLE_LEFT8, BOTTOM_LEFT9);
	AddAnchor(IDC_STATICTHREAD16, MIDDLE_LEFT9, BOTTOM_LEFT10);

	//���ս��
	AddAnchor(IDC_FINAL_RESULT1, TOP_LEFT, TOP_LEFT2);
	AddAnchor(IDC_FINAL_RESULT2, TOP_LEFT2, TOP_LEFT3);
	AddAnchor(IDC_FINAL_RESULT3, TOP_LEFT3, TOP_LEFT4);
	AddAnchor(IDC_FINAL_RESULT4, TOP_LEFT4, TOP_LEFT5);
	AddAnchor(IDC_FINAL_RESULT5, TOP_LEFT6, TOP_LEFT7);
	AddAnchor(IDC_FINAL_RESULT6, TOP_LEFT7, TOP_LEFT8);
	AddAnchor(IDC_FINAL_RESULT7, TOP_LEFT8, TOP_LEFT9);
	AddAnchor(IDC_FINAL_RESULT8, TOP_LEFT9, TOP_LEFT10);
	AddAnchor(IDC_FINAL_RESULT9, MIDDLE_LEFT, MIDDLE_LEFT2);
	AddAnchor(IDC_FINAL_RESULT10, MIDDLE_LEFT2, MIDDLE_LEFT3);
	AddAnchor(IDC_FINAL_RESULT11, MIDDLE_LEFT3, MIDDLE_LEFT4);
	AddAnchor(IDC_FINAL_RESULT12, MIDDLE_LEFT4, MIDDLE_LEFT5);
	AddAnchor(IDC_FINAL_RESULT13, MIDDLE_LEFT6, MIDDLE_LEFT7);
	AddAnchor(IDC_FINAL_RESULT14, MIDDLE_LEFT7, MIDDLE_LEFT8);
	AddAnchor(IDC_FINAL_RESULT15, MIDDLE_LEFT8, MIDDLE_LEFT9);
	AddAnchor(IDC_FINAL_RESULT16, MIDDLE_LEFT9, MIDDLE_LEFT10);

	//����Ŷ���ʾ
	AddAnchor(IDC_SHOWNUMBERPORT1_EDIT, TOP_LEFT, TOP_LEFT2);
	AddAnchor(IDC_SHOWNUMBERPORT2_EDIT, TOP_LEFT2, TOP_LEFT3);
	AddAnchor(IDC_SHOWNUMBERPORT3_EDIT, TOP_LEFT3, TOP_LEFT4);
	AddAnchor(IDC_SHOWNUMBERPORT4_EDIT, TOP_LEFT4, TOP_LEFT5);
	AddAnchor(IDC_SHOWNUMBERPORT5_EDIT, TOP_LEFT6, TOP_LEFT7);
	AddAnchor(IDC_SHOWNUMBERPORT6_EDIT, TOP_LEFT7, TOP_LEFT8);
	AddAnchor(IDC_SHOWNUMBERPORT7_EDIT, TOP_LEFT8, TOP_LEFT9);
	AddAnchor(IDC_SHOWNUMBERPORT8_EDIT, TOP_LEFT9, TOP_LEFT10);


	//LOG��ʾ
	AddAnchor(IDC_RESULT, TOP_LEFT, MIDDLE_LEFT2);
	AddAnchor(IDC_RESULT2, TOP_LEFT2, MIDDLE_LEFT3);
	AddAnchor(IDC_RESULT3, TOP_LEFT3, MIDDLE_LEFT4);
	AddAnchor(IDC_RESULT4, TOP_LEFT4, MIDDLE_LEFT5);
	AddAnchor(IDC_RESULT5, TOP_LEFT6, MIDDLE_LEFT7);
	AddAnchor(IDC_RESULT6, TOP_LEFT7, MIDDLE_LEFT8);
	AddAnchor(IDC_RESULT7, TOP_LEFT8, MIDDLE_LEFT9);
	AddAnchor(IDC_RESULT8, TOP_LEFT9, MIDDLE_LEFT10);
	AddAnchor(IDC_RESULT9, MIDDLE_LEFT, BOTTOM_LEFT2);
	AddAnchor(IDC_RESULT10, MIDDLE_LEFT2, BOTTOM_LEFT3);
	AddAnchor(IDC_RESULT11, MIDDLE_LEFT3, BOTTOM_LEFT4);
	AddAnchor(IDC_RESULT12, MIDDLE_LEFT4, BOTTOM_LEFT5);
	AddAnchor(IDC_RESULT13, MIDDLE_LEFT6, BOTTOM_LEFT7);
	AddAnchor(IDC_RESULT14, MIDDLE_LEFT7, BOTTOM_LEFT8);
	AddAnchor(IDC_RESULT15, MIDDLE_LEFT8, BOTTOM_LEFT9);
	AddAnchor(IDC_RESULT16, MIDDLE_LEFT9, BOTTOM_LEFT10);

	//�˿�
	AddAnchor(IDC_STATICPORT1, TOP_LEFT);
	AddAnchor(IDC_STATICPORT2, TOP_LEFT2);
	AddAnchor(IDC_STATICPORT3, TOP_LEFT3);
	AddAnchor(IDC_STATICPORT4, TOP_LEFT4);
	AddAnchor(IDC_STATICPORT5, TOP_LEFT6);
	AddAnchor(IDC_STATICPORT6, TOP_LEFT7);
	AddAnchor(IDC_STATICPORT7, TOP_LEFT8);
	AddAnchor(IDC_STATICPORT8, TOP_LEFT9);
	AddAnchor(IDC_STATICPORT9, MIDDLE_LEFT);
	AddAnchor(IDC_STATICPORT10, MIDDLE_LEFT2);
	AddAnchor(IDC_STATICPORT11, MIDDLE_LEFT3);
	AddAnchor(IDC_STATICPORT12, MIDDLE_LEFT5);
	AddAnchor(IDC_STATICPORT13, MIDDLE_LEFT6);
	AddAnchor(IDC_STATICPORT14, MIDDLE_LEFT7);
	AddAnchor(IDC_STATICPORT15, MIDDLE_LEFT8);
	AddAnchor(IDC_STATICPORT16, MIDDLE_LEFT9);

	AddAnchor(IDC_STATICBAUD1, TOP_LEFT);
	AddAnchor(IDC_STATICBAUD2, TOP_LEFT2);
	AddAnchor(IDC_STATICBAUD3, TOP_LEFT3);
	AddAnchor(IDC_STATICBAUD4, TOP_LEFT4);
	AddAnchor(IDC_STATICBAUD5, TOP_LEFT6);
	AddAnchor(IDC_STATICBAUD6, TOP_LEFT7);
	AddAnchor(IDC_STATICBAUD7, TOP_LEFT8);
	AddAnchor(IDC_STATICBAUD8, TOP_LEFT9);
	AddAnchor(IDC_STATICBAUD9, MIDDLE_LEFT);
	AddAnchor(IDC_STATICBAUD10, MIDDLE_LEFT2);
	AddAnchor(IDC_STATICBAUD11, MIDDLE_LEFT3);
	AddAnchor(IDC_STATICBAUD12, MIDDLE_LEFT5);
	AddAnchor(IDC_STATICBAUD13, MIDDLE_LEFT6);
	AddAnchor(IDC_STATICBAUD14, MIDDLE_LEFT7);
	AddAnchor(IDC_STATICBAUD15, MIDDLE_LEFT8);
	AddAnchor(IDC_STATICBAUD16, MIDDLE_LEFT9);

	AddAnchor(IDC_COMBO1, TOP_LEFT);
	AddAnchor(IDC_COMBO2, TOP_LEFT);
	AddAnchor(IDC_COMBO4, TOP_LEFT2);
	AddAnchor(IDC_COMBO5, TOP_LEFT2);
	AddAnchor(IDC_COMBO6, TOP_LEFT3);
	AddAnchor(IDC_COMBO7, TOP_LEFT3);
	AddAnchor(IDC_COMBO8, TOP_LEFT4);
	AddAnchor(IDC_COMBO9, TOP_LEFT4);
	AddAnchor(IDC_COMBO10, TOP_LEFT6);
	AddAnchor(IDC_COMBO11, TOP_LEFT6);
	AddAnchor(IDC_COMBO12, TOP_LEFT7);
	AddAnchor(IDC_COMBO13, TOP_LEFT7);
	AddAnchor(IDC_COMBO29, TOP_LEFT8);
	AddAnchor(IDC_COMBO30, TOP_LEFT8);
	AddAnchor(IDC_COMBO31, TOP_LEFT9);
	AddAnchor(IDC_COMBO32, TOP_LEFT9);
	AddAnchor(IDC_COMBO33, MIDDLE_LEFT);
	AddAnchor(IDC_COMBO34, MIDDLE_LEFT);
	AddAnchor(IDC_COMBO35, MIDDLE_LEFT2);
	AddAnchor(IDC_COMBO36, MIDDLE_LEFT2);
	AddAnchor(IDC_COMBO37, MIDDLE_LEFT3);
	AddAnchor(IDC_COMBO38, MIDDLE_LEFT3);
	AddAnchor(IDC_COMBO39, MIDDLE_LEFT5);
	AddAnchor(IDC_COMBO40, MIDDLE_LEFT5);
	AddAnchor(IDC_COMBO41, MIDDLE_LEFT6);
	AddAnchor(IDC_COMBO42, MIDDLE_LEFT6);
	AddAnchor(IDC_COMBO43, MIDDLE_LEFT7);
	AddAnchor(IDC_COMBO44, MIDDLE_LEFT7);
	AddAnchor(IDC_COMBO45, MIDDLE_LEFT8);
	AddAnchor(IDC_COMBO46, MIDDLE_LEFT8);
	AddAnchor(IDC_COMBO47, MIDDLE_LEFT9);
	AddAnchor(IDC_COMBO48, MIDDLE_LEFT9);


	AddAnchor(IDC_BUTTON1, TOP_LEFT);
	AddAnchor(IDC_BUTTON2, TOP_LEFT);
	AddAnchor(IDC_BUTTON14, TOP_LEFT2);
	AddAnchor(IDC_BUTTON15, TOP_LEFT2);
	AddAnchor(IDC_BUTTON16, TOP_LEFT3);
	AddAnchor(IDC_BUTTON17, TOP_LEFT3);
	AddAnchor(IDC_BUTTON18, TOP_LEFT4);
	AddAnchor(IDC_BUTTON19, TOP_LEFT4);
	AddAnchor(IDC_BUTTON20, TOP_LEFT6);
	AddAnchor(IDC_BUTTON21, TOP_LEFT6);
	AddAnchor(IDC_BUTTON22, TOP_LEFT7);
	AddAnchor(IDC_BUTTON23, TOP_LEFT7);
	AddAnchor(IDC_BUTTONSTART7, TOP_LEFT8);
	AddAnchor(IDC_BUTTONSTOP7, TOP_LEFT8);
	AddAnchor(IDC_BUTTONSTART8, TOP_LEFT9);
	AddAnchor(IDC_BUTTONSTOP8, TOP_LEFT9);
	AddAnchor(IDC_BUTTONSTART9, MIDDLE_LEFT);
	AddAnchor(IDC_BUTTONSTOP9, MIDDLE_LEFT);
	AddAnchor(IDC_BUTTONSTART10, MIDDLE_LEFT2);
	AddAnchor(IDC_BUTTONSTOP10, MIDDLE_LEFT2);
	AddAnchor(IDC_BUTTONSTART11, MIDDLE_LEFT3);
	AddAnchor(IDC_BUTTONSTOP11, MIDDLE_LEFT3);
	AddAnchor(IDC_BUTTONSTART12, MIDDLE_LEFT5);
	AddAnchor(IDC_BUTTONSTOP12, MIDDLE_LEFT5);
	AddAnchor(IDC_BUTTONSTART13, MIDDLE_LEFT6);
	AddAnchor(IDC_BUTTONSTOP13, MIDDLE_LEFT6);
	AddAnchor(IDC_BUTTONSTART14, MIDDLE_LEFT7);
	AddAnchor(IDC_BUTTONSTOP14, MIDDLE_LEFT7);
	AddAnchor(IDC_BUTTONSTART15, MIDDLE_LEFT8);
	AddAnchor(IDC_BUTTONSTOP15, MIDDLE_LEFT8);
	AddAnchor(IDC_BUTTONSTART16, MIDDLE_LEFT9);
	AddAnchor(IDC_BUTTONSTOP16, MIDDLE_LEFT9);

	AT_LoadConfig();

	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		EnableWindow_StartALL(FALSE);
	}
	else
	{

	}
	UI_LoadConfig();

	for (int i = 0; i<THREAD_NUM; i++)
	{
		gColor[i] = RGB(60, 190, 190);

		if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
		{
			if (1)
			{
				//���������Ӧ�ö�����Ҫ������if���
				LogShow_exchange(m_ResultArray[i], Final_Result_ControlArray[i], 251, "��ʼ����ɣ�\r\n", i);
				/*if (i == 0)
					LogShow_exchange(&m_Result1, &Final_Result_Control1, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 1)
					LogShow_exchange(&m_Result2, &Final_Result_Control2, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 2)
					LogShow_exchange(&m_Result3, &Final_Result_Control3, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 3)
					LogShow_exchange(&m_Result4, &Final_Result_Control4, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 4)
					LogShow_exchange(&m_Result5, &Final_Result_Control5, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 5)
					LogShow_exchange(&m_Result6, &Final_Result_Control6, 251, "��ʼ����ɣ�\r\n", i);

				if (i == 6)
					LogShow_exchange(&m_Result7, &Final_Result_Control7, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 7)
					LogShow_exchange(&m_Result8, &Final_Result_Control8, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 8)
					LogShow_exchange(&m_Result9, &Final_Result_Control9, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 9)
					LogShow_exchange(&m_Result10, &Final_Result_Control10, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 10)
					LogShow_exchange(&m_Result11, &Final_Result_Control11, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 11)
					LogShow_exchange(&m_Result12, &Final_Result_Control12, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 12)
					LogShow_exchange(&m_Result13, &Final_Result_Control13, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 13)
					LogShow_exchange(&m_Result14, &Final_Result_Control14, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 14)
					LogShow_exchange(&m_Result15, &Final_Result_Control15, 251, "��ʼ����ɣ�\r\n", i);
				if (i == 15)
					LogShow_exchange(&m_Result16, &Final_Result_Control16, 251, "��ʼ����ɣ�\r\n", i);*/
			}
		}
		else
		{
			LogShow_exchange(m_ResultArray[i], Final_Result_ControlArray[i], 250, "û��ѡ�������ݿ⣡\r\n", i);
			/*if (i == 0)
				LogShow_exchange(&m_Result1, &Final_Result_Control1, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 1)
				LogShow_exchange(&m_Result2, &Final_Result_Control2, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 2)
				LogShow_exchange(&m_Result3, &Final_Result_Control3, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 3)
				LogShow_exchange(&m_Result4, &Final_Result_Control4, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 4)
				LogShow_exchange(&m_Result5, &Final_Result_Control5, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 5)
				LogShow_exchange(&m_Result6, &Final_Result_Control6, 250, "û��ѡ�������ݿ⣡\r\n", i);

			if (i == 6)
				LogShow_exchange(&m_Result7, &Final_Result_Control7, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 7)
				LogShow_exchange(&m_Result8, &Final_Result_Control8, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 8)
				LogShow_exchange(&m_Result9, &Final_Result_Control9, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 9)
				LogShow_exchange(&m_Result10, &Final_Result_Control10, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 10)
				LogShow_exchange(&m_Result11, &Final_Result_Control11, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 11)
				LogShow_exchange(&m_Result12, &Final_Result_Control12, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 12)
				LogShow_exchange(&m_Result13, &Final_Result_Control13, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 13)
				LogShow_exchange(&m_Result14, &Final_Result_Control14, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 14)
				LogShow_exchange(&m_Result15, &Final_Result_Control15, 250, "û��ѡ�������ݿ⣡\r\n", i);
			if (i == 15)
				LogShow_exchange(&m_Result16, &Final_Result_Control16, 250, "û��ѡ�������ݿ⣡\r\n", i);*/
		}
		Sleep(20);
		Thread_State[i] = FALSE;
		COM_State[i] = FALSE;

		TestResult[i] = FALSE;
	}
	COM_State[THREAD_NUM] = FALSE;
	EnableWindow_Choose(0);
	EnableWindow_ALL(FALSE);

	PrintOn = FALSE;
	for (int p = 0; p<THREAD_NUM; p++)
	{
		Data3_FromDB[p] = "";
		Data2_FromDB[p] = "";
		Data1_FromDB[p] = "";
		StopSign[p] = FALSE;
	}

	StopAutoStart = TRUE;

	InitializeCriticalSection(&GETPORT);
	InitializeCriticalSection(&CPUCOMMUNICATE);
	InitializeCriticalSection(&SHOWONEPIC);
	InitializeCriticalSection(&UseScanResult);
	InitializeCriticalSection(&SPEAKVOICE);

	InitializeCriticalSection(&SUCCFAILCOUNT);
	for (int i = 0; i<THREAD_NUM / 4; i++)
	{
		InitializeCriticalSection(&UPDATEDB[i]);
	}

	for (int i = 0; i<THREAD_NUM / 2; i++)
	{
		InitializeCriticalSection(&WIFICOMMUNICATE[i]);
	}

	PortType.SetCurSel(1);

	TestCheck = FALSE;
	CompareIMEI = FALSE;
	UpdateData(FALSE);

	CMenu m_menu;
	m_menu.LoadMenu(IDR_SYSMENU);
	SetMenu(&m_menu);
	Voice_Ini();
	Voice_Speak("��ӭʹ��");
	SetTimer(31, 100, NULL);

	LoadDll();
	WorkStationControl.SetCurSel(1);

	//����һ��������
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CallBackCrashHandler); //�������ûص��������������ʱ������Ϣ������

	//����������������
	GetBleOrder();//��ȡ���������ļ����ָ��

	//���������������� 
	PowerControlInitSetting();//��һЩ�������г�ʼ��

	SetDlgItemText(IDC_COMBOTESTYYPE, "DAM�̵���");

	//Dongle������������
	DongleInitSetting();

	//1��SMT��2����װ��3���ϻ���
	if (g_ToolFlag == 1)
	{
		CheckSMTChoose = FALSE;
		CheckAutoTestChoose = FALSE;
		SetDlgItemText(IDC_COMBO56, "SMT����");
	}
	else if (g_ToolFlag == 2)
	{
		CheckSMTChoose = TRUE;
		SetDlgItemText(IDC_COMBO56, "��װ����");
	}
	else if (g_ToolFlag == 3)
	{
		CheckSMTChoose = TRUE;
		CheckAutoTestChoose = TRUE;
		SetDlgItemText(IDC_COMBO56, "�ϻ������");
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void IMEIWrite_MulAT::OnPaint()
{
	if (1)
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		dc.FillSolidRect(rect, RGB(210, 227, 199));
	}
	else
	{
		CDialog::OnPaint();
	}
}

void IMEIWrite_MulAT::Delay_Clock(UINT mSecond)//��ʱ����ʱ
{
	RunCommandList1.ResetEvent();//�ȴ�
	WaitForSingleObject(RunCommandList1.m_hObject, mSecond + 500);
	return;
}
void IMEIWrite_MulAT::UI_LoadConfig()
{

	for (int j = 0; j<ITEMMAX; j++)
	{
		paraArray[j].ifCheck = false;          //ûѡ�� 
		paraArray[j].Low_Limit_Value = "";	//����������ֵ
		paraArray[j].High_Limit_Value = "";	//����������ֵ
		paraArray[j].Other_ITEM = "";
	}
	int nCommandNum = 0;

	CString strCommand = _T("");
	CString strValue = _T("");

	CString strName = _T("");
	CStringArray strCommandAry;



	char szName[MAX_PATH] = _T("");
	CString  szPath = _T("");
	GetModuleFileName(NULL, szName, MAX_PATH);
	szPath = szName;
	szPath = szPath.Mid(0, szPath.ReverseFind('\\') + 1);

	CString strLoadFile = szPath + STRING_PATH_BLE;
	CIniFile tmpread(strLoadFile);
	CString fSectionName = tmpread.GetParaName();
	if (fSectionName == "BLE")
	{
		// ����INI�ļ�
		ASSERT(!strLoadFile.IsEmpty());
		CIniFile m_fIniFile(strLoadFile);
		int i = 0;
		char aText[140] = { 0 };

		int nCommand = 0;
		int nCommandNum = 0;
		RVITEM rvi;

		CString strCommand = _T("");
		CString strValue = _T("");//����������ֵ
		CString Check_item = _T("");//�Ƿ�ѡ��
		CString strValue_low = _T("");//��������ֵ
		CString strValue_high = _T("");//��������ֵ
		CString strValue_OtherITEM = _T("");//�����Զ���


		CStringArray strCommandAry;

		m_fIniFile.ReadSection(STRING_SECTION_BLE, strCommandAry);
		nCommandNum = strCommandAry.GetSize();

		if (nCommandNum == 0)
		{
			CString strInfo;
			strInfo = _T("Imported script file don't have setting data!");
			AfxMessageBox(strInfo);
			return;
		}
		else
		{
			for (i = 0; i< nCommandNum; i++)         //�������������
			{
				strCommand = strCommandAry[i];      //���
				paraArray[i].showName = strCommand;   //���������

				strValue = m_fIniFile.ReadString(STRING_SECTION_BLE, strCommand, "");//��Ŷ�Ӧ������

				int npos = strValue.Find("@@");
				if (npos >= 0)
				{
					Check_item = strValue.Left(npos);
					strValue = strValue.Mid(npos + 2);
				}
				npos = strValue.Find("@@");
				if (npos >= 0)
				{
					strValue_low = strValue.Left(npos);
					strValue = strValue.Mid(npos + 2);
				}
				npos = strValue.Find("@@");
				if (npos >= 0)
				{
					strValue_high = strValue.Left(npos);
					strValue_OtherITEM = strValue.Mid(npos + 2);
				}
				else
				{
					strValue_high = strValue;
					strValue_OtherITEM = "null";
				}

				paraArray[i].Low_Limit_Value = strValue_low;	//����������ֵ
				paraArray[i].High_Limit_Value = strValue_high;	//����������ֵ
				paraArray[i].Other_ITEM = strValue_OtherITEM;	//�����Զ���
				if (Check_item == "true")
					paraArray[i].ifCheck = true;           //ѡ��       
				else
					paraArray[i].ifCheck = false;          //ûѡ��       

			}
		}
	}
	UpdateData(false);
}
void IMEIWrite_MulAT::AT_LoadConfig()
{
	int nCommandNum = 0;

	CString strCommand = _T("");
	CStringArray strCommandAry;

	char szName[MAX_PATH] = _T("");
	CString  szPath = _T("");
	GetModuleFileName(NULL, szName, MAX_PATH);
	szPath = szName;
	szPath = szPath.Mid(0, szPath.ReverseFind('\\') + 1);

	CString strLoadFile = szPath + STRING_PATH_CONFIG;
	CIniFile m_fIniFile(strLoadFile);

	m_fIniFile.ReadSection(STRING_SECTION_CONFIG, strCommandAry);
	nCommandNum = strCommandAry.GetSize();
	CString INI_MSG_TEMP;
	for (int i = 0; i<nCommandNum; i++)
	{
		strCommand = strCommandAry[i];//���
		if (strCommand == "Initial_Connect_DB_M")
		{
			INI_MSG_TEMP = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			if (INI_MSG_TEMP == "true")
				IMEI_Setdlg.Initial_Connect_DB = TRUE;
			else
				IMEI_Setdlg.Initial_Connect_DB = FALSE;
		}
		//����һ�ģ���������Ϊ���������õ�Ӱ�죬�������Ҷ���IPΪ240��ȫ�ֱ���������
		else if (strCommand == "m_server_M")
		{
			//IMEI_Setdlg.m_server = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			IMEI_Setdlg.m_server = g_BackDoorIP;
		}
		else if (strCommand == "m_db_M")
		{
			//IMEI_Setdlg.m_db = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			IMEI_Setdlg.m_db = g_BackDoorDatabase;
		}
		else if (strCommand == "m_user_M")
		{
			//IMEI_Setdlg.m_user = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			IMEI_Setdlg.m_user = g_BackDoorUser;
		}
		else if (strCommand == "m_pwd_M")
		{
			//IMEI_Setdlg.m_pwd = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			IMEI_Setdlg.m_pwd = g_BackDoorPassword;
		}
		else if (strCommand == "DestFile_M")
		{
			IMEI_Setdlg.DestFile = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
		}
	}
}


BOOL IMEIWrite_MulAT::OnGetport()
{
	// TODO: Add your control notification handler code here
	EnterCriticalSection(&GETPORT);
	HKEY hKey;
	for (int j = 0; j<PORTS_NUM; j++)
	{
		COM_Number[j] = "";
	}
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm",
		NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // �򿪴���ע���
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName,
				&dwSize) == ERROR_NO_MORE_ITEMS) // ö�ٴ���
				break;

			COM_Number[i] = commName;
			i++;
			if (i >= PORTS_NUM)
				break;
		}
		if (i == 0)
		{
			LeaveCriticalSection(&GETPORT);
			return FALSE;
		}
		RegCloseKey(hKey);
	}
	else
	{
		LeaveCriticalSection(&GETPORT);
		return FALSE;
	}
	LeaveCriticalSection(&GETPORT);
	return TRUE;
}
void IMEIWrite_MulAT::InitCOM(CComboBox* m_Port, CComboBox* m_Baud, int num)//��ʼ������
{
	//init baud
	m_Baud->AddString("460800");
	m_Baud->AddString("512000");
	m_Baud->AddString("600000");
	m_Baud->AddString("2400");
	m_Baud->AddString("4800");
	m_Baud->AddString("9600");
	m_Baud->AddString("14400");
	m_Baud->AddString("19200");

	m_Baud->AddString("38400");
	m_Baud->AddString("56000");
	m_Baud->AddString("57600");
	m_Baud->AddString("115200");
	m_Baud->AddString("128000");

	m_Baud->AddString("256000");

	m_Baud->SetCurSel(0);

	//init port
	//int Count=0;
	BOOL COM1_EXIST = FALSE;
	if (COM_IniDone == FALSE)
	{
		HKEY hKey;
		if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm",
			NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // �򿪴���ע���
		{
			int i = 0;
			char portName[256], commName[256];
			DWORD dwLong, dwSize;
			while (1)
			{
				dwLong = dwSize = sizeof(portName);
				if (::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName,
					&dwSize) == ERROR_NO_MORE_ITEMS)	// ö�ٴ���
					break;

				m_Port->AddString(commName);			// commName���Ǵ�������
				COM_Number[i] = commName;


				if (COM_Number[i] == "COM1")
					COM1_EXIST = TRUE;

				i++;
				if (i >= PORTS_NUM)
					break;
			}
			if (m_Port->GetCount() == 0)
			{
				::AfxMessageBox("��HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm���Ҳ�������!!!");
			}
			RegCloseKey(hKey);
		}
		COM_IniDone = TRUE;
	}
	else
	{
		for (int j = 0; j<PORTS_NUM; j++)
		{
			if (COM_Number[j] != "")
				m_Port->AddString(COM_Number[j]);
			if (COM_Number[j] == "COM1")
				COM1_EXIST = TRUE;
		}
	}
	if (COM1_EXIST == TRUE)
		m_Port->SetCurSel(num + 1);
	else
		m_Port->SetCurSel(num);
}

HBRUSH IMEIWrite_MulAT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)//�ı���ɫ
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (pWnd->GetDlgCtrlID())
	{
	case(IDC_EDITSUCCOUNT) :
		pDC->SetTextColor(RGB(0, 255, 0));
		break;
	case(IDC_EDITFAILCOUNT) :
		pDC->SetTextColor(RGB(255, 0, 0));
		break;
	case(IDC_FINAL_RESULT1) :
		pDC->SetTextColor(gColor[0]);
		break;
	case(IDC_FINAL_RESULT2) :
		pDC->SetTextColor(gColor[1]);
		break;//--LBC
	case(IDC_FINAL_RESULT3) :
		pDC->SetTextColor(gColor[2]);
		break;
	case(IDC_FINAL_RESULT4) :
		pDC->SetTextColor(gColor[3]);
		break;
	case(IDC_FINAL_RESULT5) :
		pDC->SetTextColor(gColor[4]);
		break;
	case(IDC_FINAL_RESULT6) :
		pDC->SetTextColor(gColor[5]);
		break;
	case(IDC_FINAL_RESULT7) :
		pDC->SetTextColor(gColor[6]);
		break;
	case(IDC_FINAL_RESULT8) :
		pDC->SetTextColor(gColor[7]);
		break;
	case(IDC_FINAL_RESULT9) :
		pDC->SetTextColor(gColor[8]);
		break;
	case(IDC_FINAL_RESULT10) :
		pDC->SetTextColor(gColor[9]);
		break;
	case(IDC_FINAL_RESULT11) :
		pDC->SetTextColor(gColor[10]);
		break;
	case(IDC_FINAL_RESULT12) :
		pDC->SetTextColor(gColor[11]);
		break;
	case(IDC_FINAL_RESULT13) :
		pDC->SetTextColor(gColor[12]);
		break;
	case(IDC_FINAL_RESULT14) :
		pDC->SetTextColor(gColor[13]);
		break;
	case(IDC_FINAL_RESULT15) :
		pDC->SetTextColor(gColor[14]);
		break;
	case(IDC_FINAL_RESULT16) :
		pDC->SetTextColor(gColor[15]);
		break;

	case(IDC_RESULT) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT2) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;//--LBC
	case(IDC_RESULT3) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT4) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT5) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT6) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT7) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT8) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT9) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT10) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT11) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT12) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT13) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT14) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT15) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;
	case(IDC_RESULT16) :
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(210, 227, 199));
		break;

	}
	return hbr;

	/*
	���Ը��ݲ�ͬ�Ŀؼ����ͷ��ز�ͬ�Ļ�ˢ����ʵ�ֲ�ͬ�ؼ���ɫ������
	CTLCOLOR_BTN                ��ť�ؼ�
	CTLCOLOR_DLG                �Ի���
	CTLCOLOR_EDIT               �༭��
	CTLCOLOR_LISTBOX            �б�ؼ�
	CTLCOLOR_MSGBOX             ��Ϣ�ؼ�
	CTLCOLOR_SCROLLBAR          �������ؼ�
	CTLCOLOR_STATIC             ��̬�ؼ�
	*/
}



void IMEIWrite_MulAT::OnCbnSelchangeCombo2_Fresh()
{
	//int Date_Count;//ʱ������
	//Date_Count=m_Port.GetCount();
	UpdateData(TRUE);
	for (int del = 0; del<64; del++)
	{
		m_Port1.DeleteString(0);//
		m_Port2.DeleteString(0);//
		m_Port3.DeleteString(0);//
		m_Port4.DeleteString(0);//
		m_Port5.DeleteString(0);//
		m_Port6.DeleteString(0);//

		m_Baud1.DeleteString(0);//
		m_Baud2.DeleteString(0);//
		m_Baud3.DeleteString(0);//
		m_Baud4.DeleteString(0);//
		m_Baud5.DeleteString(0);//
		m_Baud6.DeleteString(0);//

		m_Port7.DeleteString(0);
		m_Port8.DeleteString(0);
		m_Port9.DeleteString(0);

		m_Port10.DeleteString(0);
		m_Port11.DeleteString(0);
		m_Port12.DeleteString(0);
		m_Port13.DeleteString(0);
		m_Port14.DeleteString(0);
		m_Port15.DeleteString(0);
		m_Port16.DeleteString(0);

		m_Baud7.DeleteString(0);
		m_Baud8.DeleteString(0);
		m_Baud9.DeleteString(0);

		m_Baud10.DeleteString(0);
		m_Baud11.DeleteString(0);
		m_Baud12.DeleteString(0);
		m_Baud13.DeleteString(0);
		m_Baud14.DeleteString(0);
		m_Baud15.DeleteString(0);
		m_Baud16.DeleteString(0);

		CPUControlPort.DeleteString(0);
		CPUControlBaud.DeleteString(0);

		VIControlPort.DeleteString(0);
		VIControlBaud.DeleteString(0);

		ScanGunPort1.DeleteString(0);//
		ScanGunPort2.DeleteString(0);//
		ScanGunPort3.DeleteString(0);//
		ScanGunPort4.DeleteString(0);//
		ScanGunPort5.DeleteString(0);//
		ScanGunPort6.DeleteString(0);//

		ScanGunBaud1.DeleteString(0);//
		ScanGunBaud2.DeleteString(0);//
		ScanGunBaud3.DeleteString(0);//
		ScanGunBaud4.DeleteString(0);//
		ScanGunBaud5.DeleteString(0);//
		ScanGunBaud6.DeleteString(0);//

	}
	COM_IniDone = FALSE;
	for (int j = 0; j<PORTS_NUM; j++)
	{
		COM_Number[j] = "";
	}
	InitCOM(&m_Port1, &m_Baud1, 0);//��ʼ������
	InitCOM(&m_Port2, &m_Baud2, 1);//��ʼ������--LBC
	InitCOM(&m_Port3, &m_Baud3, 2);
	InitCOM(&m_Port4, &m_Baud4, 3);
	InitCOM(&m_Port5, &m_Baud5, 4);
	InitCOM(&m_Port6, &m_Baud6, 5);

	InitCOM(&m_Port7, &m_Baud7, 6);
	InitCOM(&m_Port8, &m_Baud8, 7);
	InitCOM(&m_Port9, &m_Baud9, 8);
	InitCOM(&m_Port10, &m_Baud10, 9);
	InitCOM(&m_Port11, &m_Baud11, 10);
	InitCOM(&m_Port12, &m_Baud12, 11);
	InitCOM(&m_Port13, &m_Baud13, 12);
	InitCOM(&m_Port14, &m_Baud14, 13);
	InitCOM(&m_Port15, &m_Baud15, 14);
	InitCOM(&m_Port16, &m_Baud16, 15);

	InitCOM(&CPUControlPort, &CPUControlBaud, 16);
	InitCOM(&VIControlPort, &VIControlBaud, 17);

	InitCOM(&ScanGunPort1, &ScanGunBaud1, 18);//��ʼ��ɨ��ǹ����
	InitCOM(&ScanGunPort2, &ScanGunBaud2, 1);
	InitCOM(&ScanGunPort3, &ScanGunBaud3, 2);
	InitCOM(&ScanGunPort4, &ScanGunBaud4, 3);
	InitCOM(&ScanGunPort5, &ScanGunBaud5, 4);
	InitCOM(&ScanGunPort6, &ScanGunBaud6, 5);

	AfxMessageBox("ˢ�¶˿ڳɹ���");
	UpdateData(FALSE);
}



void IMEIWrite_MulAT::OnContextMenu(CWnd*, CPoint point)//�˵�
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));

	CMenu* pPopup = menu.GetSubMenu(0);

	if (EnableMenu)
		pPopup->EnableMenuItem(ID_FRESH_32771, MF_ENABLED);
	else
		pPopup->EnableMenuItem(ID_FRESH_32771, MF_DISABLED | MF_GRAYED);


	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y,
		pWndPopupOwner);


}
void IMEIWrite_MulAT::OnRButtonUp(UINT nFlags, CPoint point) //�Ҽ�����
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnRButtonUp(nFlags, point);
}

//�ػ����������ػ����ػ�ػ����ж��Ƿ��޸���ʱ�䡣 û�������ػ�����һ����־λ������Ҳ��Ҫ�����˺�
LRESULT IMEIWrite_MulAT::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)  //�ػ�ػ���Ϣ
{
	// TODO: Add your specialized code here and/or call the base class 
	switch (message)
	{
	case WM_CLOSE:
		if ((Thread_State[0] == TRUE) || (Thread_State[1] == TRUE) || (Thread_State[2] == TRUE) || (Thread_State[3] == TRUE) || (Thread_State[4] == TRUE) || (Thread_State[5] == TRUE) || \
			(Thread_State[6] == TRUE) || (Thread_State[7] == TRUE) || (Thread_State[8] == TRUE) || (Thread_State[9] == TRUE) || (Thread_State[10] == TRUE) || (Thread_State[11] == TRUE) || \
			(Thread_State[12] == TRUE) || (Thread_State[13] == TRUE) || (Thread_State[14] == TRUE) || (Thread_State[15] == TRUE))
		{
			AfxMessageBox("���ȵȴ��߳̽�����");
			return TRUE;
		}
		else
		{
			OnBnClickedButton25();
			OnBnClickedButtondisconncpu();
			OnOK();
		}

		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL IMEIWrite_MulAT::CheckConnect_Thread(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control)
{
	CString sPort;
	m_Port->GetWindowText(sPort);

	CString PortType_CS;
	PortType.GetWindowTextA(PortType_CS);

	if (sPort == "")
		return FALSE;
	//return TRUE;

	//static char* Serial_Order_Return;
	BOOL Return;
	int Port_OK_Try = 0;



	for (int num = 0; num<5; num++)
	{
		if (OnGetport() == TRUE)
			break;
		else
		{
			//LogShow_exchange(m_Result,Final_Result_Control,5,"�豸�˿��ѶϿ�",HandleNum);
			return FALSE;
		}
		Sleep(150);
	}
	BOOL Port_OK = FALSE;

	for (int num1 = 0; num1<PORTS_NUM; num1++)
	{
		if ((COM_Number[num1] == sPort) && (sPort != ""))
		{
			Port_OK = TRUE;
			break;
		}
	}
	if (Port_OK == FALSE)
	{
		Port_OK_Try++;
		//LogShow_exchange(m_Result,Final_Result_Control,5,"�豸�˿��ѶϿ�",HandleNum);
		return FALSE;
	}
	else
	{

	}
	Sleep(100);


	if (PortType_CS == "�ն������˿�")
		return TRUE;

	if (OPen_Serial_Port(m_Port, m_Baud, HandleNum,0,GetRDAHostCheckValue) == TRUE)//CheckConnect_Thread
	{
		//�򿪴��ڳɹ�
	}
	else
	{
		//�򿪴���ʧ��
		//LogShow_exchange(m_Result,Final_Result_Control,5,"�豸�ѶϿ����ӣ����Բ�����һ̨",HandleNum);
		return FALSE;
	}
	if (hPort[HandleNum] != NULL || GetRDAHostCheckValue == TRUE)
		Return = CloseHandleControl(hPort[HandleNum], GetRDAHostCheckValue, HandleNum);										//���Գɹ���
	hPort[HandleNum] = NULL;
	if (Return == TRUE)
	{

	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 5, "�رոô���ʧ�ܣ��ö˿��쳣�������������Թ��ߣ�������", HandleNum);
		return FALSE;													//�رմ���ʧ�ܣ������쳣
	}
	return TRUE;
}

BOOL IMEIWrite_MulAT::OPen_Serial_Port(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum, BOOL CPUChoose, BOOL RDAFlag)		//�򿪴���
{
	CString sPort, sBaud;
	int port, baud;
	//get port
	m_Port->GetWindowText(sPort);
	if (sPort == "")
	{
		AfxMessageBox("����ѡ��˿ڣ�");
		return FALSE;
	}

	sPort = sPort.Right(sPort.GetLength() - 3);
	port = atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"), port);

	//get baud
	m_Baud->GetWindowText(sBaud);
	baud = atoi(sBaud);

	/*�����RDAƽ̨�Ļ�����ô˴��ڳ�ʼ��*/
	if (RDAFlag == TRUE && CPUChoose == FALSE)
	{
		//����ԭʼ�߼����ȹش���-���ٴ򿪴���-��Ȼ��ATָ��
		BOOL ComInitFlag;
		CString RDAPort,RDACommand = "", RDAReturnOK="";
		int RDATimeout=1;

		m_Port->GetWindowText(RDAPort);

		RdaHostInterface.RDAComShutdown(HandleNum);
		ComInitFlag = RdaHostInterface.RDAComInit(HandleNum, RDAPort);
        if (ComInitFlag == FALSE)
		{
			return FALSE;
		}

		for (int i = 0; i < 3; i++)
		{
			RDACommand.Empty();
			if (paraArray[3].Low_Limit_Value != "")
			{
				int pos = paraArray[3].Low_Limit_Value.Find("**");
				if (pos >= 0)
				{
					RDACommand = paraArray[3].Low_Limit_Value.Left(pos) + "\r\n";
					RDAReturnOK = paraArray[3].Low_Limit_Value.Mid(pos + 2);
				}
				else
				{
					RDACommand.Format("AT^GT_CM=TEST\r\n");
					RDAReturnOK = "TEST_OK";
				}
			}
			else
			{
				RDACommand.Format("AT^GT_CM=TEST\r\n");
				RDAReturnOK = "TEST_OK";
			}

			if (RDAReturnOK == "")
				RDAReturnOK = "\r";

			//����RDA��д����
			BOOL WriteFlag = RdaHostInterface.RDAComWriteData(HandleNum, RDACommand);
			
			if (WriteFlag == FALSE)
			{
				Sleep(500);
				if (i == 2)
					RDATimeout = 1;
				continue;
			}

			CString WaitTimeCS;
			int wpos = paraArray[3].showName.Find("DL");
			if (wpos != -1)
				WaitTimeCS = paraArray[3].showName.Mid(wpos + 2);
			else
				WaitTimeCS = "0";
			//receive response
			Sleep(300 + atoi(WaitTimeCS));

			if (RDAComReceive[HandleNum].GetLength() > 0)
			{
				if (RDAComReceive[HandleNum].Find(RDAReturnOK) != -1)
				{
					RDATimeout = 0;
					break;
				}
			}
			Sleep(100);
		}

		RDAComReceive[HandleNum] = "";
		
		//����ֱ�ӷ���
		if (RDATimeout == 1)
		{
			CloseHandleControl(hPort[HandleNum], GetRDAHostCheckValue, HandleNum);
			return FALSE;
		}                                                         
		else if (RDATimeout == 0)
		{
			return TRUE;
		}

	}
	/*RDAƽ̨���ڳ�ʼ�����������*/

	if (hPort[HandleNum] != NULL)
	{
		CloseHandle(hPort[HandleNum]);
		hPort[HandleNum] = NULL;
	}
	//open com port�򿪶˿�
	hPort[HandleNum] = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPort[HandleNum] == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port->GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hPort[HandleNum] = NULL;
		return FALSE;
	}
	//LogShow_exchange(m_Result,Final_Result_Control,0,"�ɹ��������ھ��\r\n",HandleNum);

	//config the com port
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hPort[HandleNum], &dcb);
	SetupComm(hPort[HandleNum], PICDATAMAX, 4096);
	PurgeComm(hPort[HandleNum], PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.BaudRate = baud;													//57600(MT6205B), 115200 (MT6218B)			
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
	SetCommState(hPort[HandleNum], &dcb);

	//set time out struct
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	//timeouts.ReadTotalTimeoutMultiplier = 0;
	//timeouts.ReadTotalTimeoutConstant = 0;
	/*
	ReadTotalTimeoutConstant��һ�ζ�ȡ�������ݵĹ̶���ʱ��
	������һ�ζ�ȡ���ڵĲ����У��䳬ʱΪReadTotalTimeoutMultiplier���Զ�ȡ���ֽ����ټ��� ReadTotalTimeoutConstant��
	��ReadIntervalTimeout����ΪMAXDWORD������ReadTotalTimeoutMultiplier ��ReadTotalTimeoutConstant����Ϊ0����ʾ��ȡ�������������ش�������뻺�������ַ���
	*/
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 10;//OPen_Serial_Port
	//timeouts.ReadTotalTimeoutConstant = 500;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hPort[HandleNum], &timeouts);

	//������������ڻ�ȡ���õ�ʱ�򣬵��ⲽ��ֱ�ӷ���
	if (BLEGetSettingFlag == 0 || BLEGetSettingFlag == 1 ||(GetDongleScanCheckValue==TRUE&&HandleNum>7))
	{
		return TRUE;
	}

	//SetCommMask(hPort[HandleNum], EV_RXCHAR|EV_TXEMPTY );							//�����¼�����������

	//read and write the com port
	BOOL bReadStatus, bWriteStat;
	DWORD dwBytesWritten, dwBytesRead;
	char *buffer;
	char *p = NULL;
	CString command;
	CString ReturnOK;
	char buf[1024];
	int timeout = 1;
	int i;


	//��Ϊ������Dongle�������ӣ���������Ҫ��������������ӵ�ʱ��Ӧ�÷���AT+RADD?ָ��
	if (GetDongleCheckValue == FALSE)
	{
		//write "AT"
		for (i = 0; i < 3; i++)
		{
			command.Empty();
			if (CPUChoose == FALSE)
			{
				if (paraArray[3].Low_Limit_Value != "")
				{
					int pos = paraArray[3].Low_Limit_Value.Find("**");
					if (pos >= 0)
					{
						command = paraArray[3].Low_Limit_Value.Left(pos) + "\r\n";
						ReturnOK = paraArray[3].Low_Limit_Value.Mid(pos + 2);
					}
					else
					{
						command.Format("AT^GT_CM=TEST\r\n");
						ReturnOK = "TEST_OK";
					}
				}
				else
				{
					command.Format("AT^GT_CM=TEST\r\n");
					ReturnOK = "TEST_OK";
				}
			}
			else
			{
				//////
				CString CpuOrderTemp = "AT01a#^GT_CM=TEST";
				CpuOrderTemp = CpuOrderTemp.Mid(2);
				MD5 mb;
				mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
				CString CpuOrderMD5 = (mb.toString()).c_str();

				command = "AT" + CpuOrderTemp + CpuOrderMD5.Left(2) + "\r\n";
				ReturnOK = "TEST_OK";
				/////
			}


			if (ReturnOK == "")
				ReturnOK = "\r";
			buffer = command.GetBuffer(command.GetLength());
			bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
			if (dwBytesWritten != command.GetLength())
			{
				Sleep(500);
				continue;
			}

			CString WaitTimeCS;
			int wpos = paraArray[3].showName.Find("DL");
			if (wpos != -1)
				WaitTimeCS = paraArray[3].showName.Mid(wpos + 2);
			else
				WaitTimeCS = "0";
			//receive response
			Sleep(300 + atoi(WaitTimeCS));
			memset(buf, 0, sizeof(buf));
			buffer = buf;
			bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);
			if (dwBytesRead != 0)
			{
				//p=strstr(buffer,"\r");
				//p=strstr(buffer,"TEST_OK");
				p = strstr(buffer, (LPSTR)(LPCTSTR)ReturnOK);
				if (p)
				{
					timeout = 0;
					break;
				}
			}
			Sleep(100);
		}
	}
	else if (GetDongleCheckValue == TRUE)
	{
		//write "AT"
		if (StopSign[HandleNum] == TRUE&&DongleInfo[HandleNum][0]=="")
		{
			DongleDisConnect(HandleNum);
			for (i = 0; i < 3; i++)
			{
				command.Empty();

				command = "AT";
				ReturnOK = "OK";

				buffer = command.GetBuffer(command.GetLength());
				bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
				if (dwBytesWritten != command.GetLength())
				{
					Sleep(500);
					continue;
				}

				CString WaitTimeCS;
				int wpos = paraArray[3].showName.Find("DL");
				if (wpos != -1)
					WaitTimeCS = paraArray[3].showName.Mid(wpos + 2);
				else
					WaitTimeCS = "0";
				//receive response
				Sleep(300 + atoi(WaitTimeCS));
				memset(buf, 0, sizeof(buf));
				buffer = buf;
				bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);
				if (dwBytesRead != 0)
				{
					p = strstr(buffer, (LPSTR)(LPCTSTR)ReturnOK);
					if (p)
					{
						timeout = 0;
						break;
					}
				}
				Sleep(100);
			}
		}
		else if (StopSign[HandleNum] == FALSE)
		{
			if (DongleConnectFlag[HandleNum] == TRUE)
			{
				//Dongle����������������ʼ֮��������������Ӻ��ж��Ƿ����ӳɹ�
				//if (DongleInfo[HandleNum][0] != "")
				//{
				//	DongleDisConnect(HandleNum);
				//}
				DongleDisConnect(HandleNum);
				Sleep(500);

				//��������Ҫ��ֹͣ��ť���õ�����Ϊ������ɨ��������жϻ����׵�������������
				m_StopControlArray[HandleNum]->EnableWindow(FALSE);

				if (DongleScan(HandleNum, DongleConnectRssi, MachineType_CS) == FALSE)
				{
					//��������Ҫ��ֹͣ��ť���¼���
					m_StopControlArray[HandleNum]->EnableWindow(TRUE);
					return FALSE;
				}

				//��������Ҫ��ֹͣ��ť���¼���
				m_StopControlArray[HandleNum]->EnableWindow(TRUE);
			}

			//�����������һ������ͨ�ļ�������Ƿ����ӣ���һ�ֳɹ�����ʧ�ܺ��������Ƿ�����
			if (DongleCheckRssiFlag[HandleNum] == FALSE)
			{
				for (i = 0; i < 3; i++)
				{
					command.Empty();

					command = "AT+RADD?";
					if (DongleInfo[HandleNum][0] == "")
						break;
					ReturnOK = "OK+Get:0x" + DongleInfo[HandleNum][0];

					buffer = command.GetBuffer(command.GetLength());
					bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
					if (dwBytesWritten != command.GetLength())
					{
						Sleep(500);
						continue;
					}

					CString WaitTimeCS;
					int wpos = paraArray[3].showName.Find("DL");
					if (wpos != -1)
						WaitTimeCS = paraArray[3].showName.Mid(wpos + 2);
					else
						WaitTimeCS = "0";
					//receive response
					Sleep(300 + atoi(WaitTimeCS));
					memset(buf, 0, sizeof(buf));
					buffer = buf;
					bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);
					if (dwBytesRead != 0)
					{
						p = strstr(buffer, (LPSTR)(LPCTSTR)ReturnOK);
						if (p)
						{
							timeout = 0;
							break;
						}
					}
					Sleep(100);
				}
			}
			else if (DongleCheckRssiFlag[HandleNum] == TRUE)
			{
				for (i = 0; i < 3; i++)
				{
					command.Empty();

					command = "AT+RSSI?";
					if (DongleInfo[HandleNum][0] == "")
						break;
					ReturnOK = "RSSI Cancelled";

					buffer = command.GetBuffer(command.GetLength());
					bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
					if (dwBytesWritten != command.GetLength())
					{
						Sleep(500);
						continue;
					}

					Sleep(2500);
					
					//�ٷ�һ�Σ�AT+RSSI?�����β���ȡ����ǿ�ȵĲ�������Ȼ��һֱ��
					buffer = command.GetBuffer(command.GetLength());
					bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
					if (dwBytesWritten != command.GetLength())
					{
						Sleep(500);
						continue;
					}
					Sleep(1500);
					CString WaitTimeCS;
					int wpos = paraArray[3].showName.Find("DL");
					if (wpos != -1)
						WaitTimeCS = paraArray[3].showName.Mid(wpos + 2);
					else
						WaitTimeCS = "0";
					//receive response
					Sleep(300 + atoi(WaitTimeCS));
					memset(buf, 0, sizeof(buf));
					buffer = buf;
					bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);
					if (dwBytesRead != 0)
					{
			
						p = strstr(buffer, (LPSTR)(LPCTSTR)ReturnOK);
						CString RssiCheckStr(buffer), RssiStr = "RSSI(dB): ";
						int RssiInt = RssiCheckStr.Find(RssiStr), ReturnInt = RssiCheckStr.Find(ReturnOK);


						//ǿ�ȼ���ʱ��ֻ�е�ǿ�ȱ�DongleDisonnectRssi�趨��ǿ��ǿ��ʱ��ŷ���TRUE�������ա�ǿ�ȱ�DongleDisonnectRssi�������������FALSE���Ͽ�����
						if (RssiInt == -1 || ReturnInt == -1 || RssiCheckStr.Find("RSSI Start") == -1)
						{
							timeout = 1;
							break;
						}
						else if (RssiInt != -1)
						{
							CString RssiVauleStr;
							int RssiVauleInt;
							if (ReturnInt != -1)
							{
								RssiInt=RssiCheckStr.Find(RssiStr, ReturnInt - 16);//��ȡ���һ���ź�ǿ��
								RssiVauleStr = RssiCheckStr.Mid(RssiInt + 10, RssiCheckStr.Find("\r\n", RssiInt) - RssiInt - 10);
								RssiVauleInt = _ttoi(RssiVauleStr);
								if (RssiVauleInt > DongleDisonnectRssi)
								{
									timeout = 0;
									break;
								}
								else if (RssiVauleInt <= DongleDisonnectRssi)
								{
									timeout = 1;
									break;
								}
							}
						}

						timeout = 1;
						break;
					}
					Sleep(100);
				}
			}

		}
	}

	DongleConnectFlag[HandleNum] = FALSE;//����Dongle�������ӱ�ʶ
	DongleCheckRssiFlag[HandleNum] = FALSE;//����Dongle�������RSSI��ʶ

	if (timeout == 1)													//check if failed
	{
		if (GetDongleCheckValue == TRUE)//�ǵöϿ�����
		{
			DongleDisConnect(HandleNum);
		}

		if (hPort[HandleNum] != NULL)
			CloseHandle(hPort[HandleNum]);									//�򿪴���ʱ������ʧ��
		hPort[HandleNum] = NULL;
		return FALSE;
	}
	//make sure rx data cleaned
	Sleep(100);
	bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);

	return TRUE;														//����TRUE���򿪴�������
}
BOOL IMEIWrite_MulAT::OPen_Serial_PortReadConstant(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum, BOOL CPUChoose)		//�򿪴���
{
	CString sPort, sBaud;
	int port, baud;
	//get port
	m_Port->GetWindowText(sPort);
	if (sPort == "")
	{
		AfxMessageBox("����ѡ��˿ڣ�");
		return FALSE;
	}

	sPort = sPort.Right(sPort.GetLength() - 3);
	port = atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"), port);

	//get baud
	m_Baud->GetWindowText(sBaud);
	baud = atoi(sBaud);

	//open com port�򿪶˿�
	hPort[HandleNum] = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPort[HandleNum] == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port->GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hPort[HandleNum] = NULL;
		return FALSE;
	}
	//LogShow_exchange(m_Result,Final_Result_Control,0,"�ɹ��������ھ��\r\n",HandleNum);

	//config the com port
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hPort[HandleNum], &dcb);
	SetupComm(hPort[HandleNum], 4096, 4096);
	PurgeComm(hPort[HandleNum], PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.BaudRate = baud;													//57600(MT6205B), 115200 (MT6218B)			
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
	SetCommState(hPort[HandleNum], &dcb);

	//set time out struct
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	//timeouts.ReadTotalTimeoutMultiplier = 0;
	//timeouts.ReadTotalTimeoutConstant = 0;
	/*
	ReadTotalTimeoutConstant��һ�ζ�ȡ�������ݵĹ̶���ʱ��
	������һ�ζ�ȡ���ڵĲ����У��䳬ʱΪReadTotalTimeoutMultiplier���Զ�ȡ���ֽ����ټ��� ReadTotalTimeoutConstant��
	��ReadIntervalTimeout����ΪMAXDWORD������ReadTotalTimeoutMultiplier ��ReadTotalTimeoutConstant����Ϊ0����ʾ��ȡ�������������ش�������뻺�������ַ���
	*/
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 300;//OPen_Serial_PortReadConstant
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hPort[HandleNum], &timeouts);

	//SetCommMask(hPort[HandleNum], EV_RXCHAR|EV_TXEMPTY );							//�����¼�����������

	//read and write the com port
	BOOL bReadStatus, bWriteStat;
	DWORD dwBytesWritten, dwBytesRead;
	char *buffer;
	char *p = NULL;
	CString command;
	CString ReturnOK;
	char buf[1024];
	int timeout = 1;
	int i;


	//�����DAM�̵�����ʱ��
	CString TestType, WorkStation;
	GetDlgItem(IDC_COMBOTESTYYPE)->GetWindowTextA(TestType);
	GetDlgItem(IDC_COMBO56)->GetWindowTextA(WorkStation);
	if (TestType == "DAM�̵���")
	{
		if (WorkStation == "SMT����")
		{
			AfxMessageBox("DAM�̵�����SMT�л�δʵ�֣�");
			if (hPort[16] != NULL)
				CloseHandle(hPort[16]);									//�򿪴���ʱ������ʧ��
			hPort[16] = NULL;
			return FALSE;
		}
		BOOL DamFlag;
		DamFlag = DamControlFun(0, "��ͨ·");
		DamFlag = DamControlFun(0, "�ر�ͨ·");
		if (DamFlag == TRUE)
		{
			return TRUE;
		}
		else
		{
			if (hPort[16] != NULL)
				CloseHandle(hPort[16]);									//�򿪴���ʱ������ʧ��
			hPort[16] = NULL;
			return FALSE;
		}
	}

	//write "AT"
	for (i = 0; i<3; i++)
	{
		command.Empty();
		if (CPUChoose == FALSE)
		{
			if (paraArray[3].Low_Limit_Value != "")
			{
				int pos = paraArray[3].Low_Limit_Value.Find("**");
				if (pos >= 0)
				{
					command = paraArray[3].Low_Limit_Value.Left(pos) + "\r\n";
					ReturnOK = paraArray[3].Low_Limit_Value.Mid(pos + 2);
				}
				else
				{
					command.Format("AT^GT_CM=TEST\r\n");
					ReturnOK = "TEST_OK";
				}
			}
			else
			{
				command.Format("AT^GT_CM=TEST\r\n");
				ReturnOK = "TEST_OK";
			}
		}
		else
		{
			//////
			CString CpuOrderTemp = "AT01a#^GT_CM=TEST";
			CpuOrderTemp = CpuOrderTemp.Mid(2);
			MD5 mb;
			mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
			CString CpuOrderMD5 = (mb.toString()).c_str();

			command = "AT" + CpuOrderTemp + CpuOrderMD5.Left(2) + "\r\n";
			ReturnOK = "TEST_OK";
			/////
		}


		if (ReturnOK == "")
			ReturnOK = "\r";
		buffer = command.GetBuffer(command.GetLength());
		bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
		if (dwBytesWritten != command.GetLength())
		{
			Sleep(500);
			continue;
		}

		CString WaitTimeCS;
		int wpos = paraArray[3].showName.Find("DL");
		if (wpos != -1)
			WaitTimeCS = paraArray[3].showName.Mid(wpos + 2);
		else
			WaitTimeCS = "0";
		//receive response
		Sleep(300 + atoi(WaitTimeCS));
		memset(buf, 0, sizeof(buf));
		buffer = buf;
		bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);
		if (dwBytesRead != 0)
		{
			//p=strstr(buffer,"\r");
			//p=strstr(buffer,"TEST_OK");
			p = strstr(buffer, (LPSTR)(LPCTSTR)ReturnOK);
			if (p)
			{
				timeout = 0;
				break;
			}
		}
		Sleep(100);
	}

	if (timeout == 1)													//check if failed
	{
		if (hPort[HandleNum] != NULL)
			CloseHandle(hPort[HandleNum]);									//�򿪴���ʱ������ʧ��
		hPort[HandleNum] = NULL;
		return FALSE;
	}
	//make sure rx data cleaned
	Sleep(100);
	bReadStatus = ReadFile(hPort[HandleNum], buffer, 256, &dwBytesRead, NULL);


	return TRUE;														//����TRUE���򿪴�������
}
char*  IMEIWrite_MulAT::Send_Serial_Order(CString* Vaule_Return, CString strCommand_Vaule, int HandleNum, char* EndSign, char* StartSign, int WaitTime, int HexFlag, BOOL RDAFlag)//ͨ�����ڷ�������
{
	int Vaule_Return_Count = -1;															//�����ĸ���
	BOOL bReadStatus, bWriteStat;
	DWORD dwBytesWritten, dwBytesRead;
	char *buffer;
	char *p = NULL;
	CString command;
	char buf[1024];//char buf[PICDATAMAX];

	CString strCommand_Vaule_Return;													//���ڷ���ֵ

	if (strCommand_Vaule == "IDLE")														//������ʱ
	{
		*Vaule_Return = "DELAY_SUCCESS";
		return "DELAY_SUCCESS";

	}
	else if (strCommand_Vaule == "NULL")
	{
		*Vaule_Return = "Analysis_SUCCESS";
		return "Analysis_SUCCESS";
	}
	Sleep(10);
	for (int i = 0; i<1; i++)//��η���
	{
		//send command
		command.Empty();

		//�����ַ���
		if (HexFlag == 0&&GetRDAHostCheckValue==FALSE)
		{
			command = strCommand_Vaule;
			//Dongle������������������Ҫ\r\n
			if (GetDongleCheckValue == FALSE && (GetDongleScanCheckValue == FALSE||HandleNum < 8))
			{
				command = strCommand_Vaule + "\r\n";//ƴ������
			}


			buffer = command.GetBuffer(command.GetLength());
			bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);
			if (dwBytesWritten != command.GetLength())
			{
				Sleep(300);
				continue;
			}

			//receive response
			Sleep(atoi(paraArray[1].Low_Limit_Value) + WaitTime);
			CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���HexGroupToString
			for (int r = 0; r < 6; r++)
			{
				memset(buf, 0, sizeof(buf));
				buffer = buf;
				bReadStatus = ReadFile(hPort[HandleNum], buffer, 512, &dwBytesRead, NULL);//bReadStatus = ReadFile( hPort[HandleNum], buffer, PICDATAMAX, &dwBytesRead, NULL);//Send_Serial_Order
				if (dwBytesRead != 0)
				{
					if (strstr(EndSign, "NULL"))
						p = strstr(buffer, "\r");
					else
						p = strstr(buffer, EndSign);

					strCommand_Vaule_Return = Port_Temp + CString(buffer);
					if ((p) || (strCommand_Vaule_Return.Find(EndSign) != -1))//if((p)||(Port_Temp.Find(EndSign)!=-1))
					{
						CString  selPort;
						selPort = strCommand_Vaule_Return;

						Vaule_Return_Count_CS[HandleNum] = selPort;

						*Vaule_Return = "Analysis_SUCCESS";
						return "Analysis_SUCCESS";
					}
					else
					{
						Port_Temp += CString(buffer);
						Sleep(200 * r);
						continue;
					}
				}
				else
				{
					Sleep(200 * r);
					continue;
				}
			}
		}
		else if (HexFlag == 0 && GetRDAHostCheckValue == TRUE)
		{
			command = strCommand_Vaule;
			RDAComReceive[HandleNum] = "";
			BOOL WriteFlag = RdaHostInterface.RDAComWriteData(HandleNum, command);

			if (WriteFlag == FALSE)
			{
				Sleep(300);
				continue;
			}

			//receive response
			Sleep(atoi(paraArray[1].Low_Limit_Value) + WaitTime);
			CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���HexGroupToString
			for (int r = 0; r < 6; r++)
			{

				if (RDAComReceive[HandleNum].GetLength() != 0)
				{
					char *RDABuffer = (LPSTR)(LPCTSTR)RDAComReceive[HandleNum];
					if (strstr(EndSign, "NULL"))
						p = strstr(RDABuffer, "\r");
					else
						p = strstr(RDABuffer, EndSign);

					strCommand_Vaule_Return = Port_Temp + RDAComReceive[HandleNum];
					if ((p) || (strCommand_Vaule_Return.Find(EndSign) != -1))//if((p)||(Port_Temp.Find(EndSign)!=-1))
					{
						CString  selPort;
						selPort = strCommand_Vaule_Return;

						Vaule_Return_Count_CS[HandleNum] = selPort;
						RDAComReceive[HandleNum] = "";
						*Vaule_Return = "Analysis_SUCCESS";
						return "Analysis_SUCCESS";
					}
					else
					{
						Port_Temp += RDAComReceive[HandleNum];
						Sleep(200 * r);
						continue;
					}
				}
				else
				{
					Sleep(200 * r);
					continue;
				}
			}
		}
		//����ʮ����������(����ģʽ)
		else if (HexFlag == 1)
		{
			//��תʮ������
			command = (strCommand_Vaule + DongleInfo[HandleNum][1] + _T("00000000000000000000000000000000")).Left(32);//ָ��+���ƣ�Ȼ���ȡ32λ
			int len = command.GetLength() / 2;
			BYTE *pBData = new BYTE[len];
			pBData = CStrToByte(len, Encrypt(command));

			//����ʮ����������
			bWriteStat = WriteFile(hPort[HandleNum], (LPVOID)pBData, len, &dwBytesWritten, NULL);

			if (dwBytesWritten != len)
			{
				Sleep(300);
				continue;
			}

			//receive response

			unsigned char str[32];

			CString CommandReturn;
			CString CommandEndSign, CommandEndSignTemp;//ʮ�����Ƶķ���ֵ��Ҫ�и�
			int CommandCut[2];//��Ҫ�Է���ֵ���н�ȡ

			//����ֵ��ʽ���ӣ�060201,(7,20)
			CommandEndSignTemp = EndSign;
			CommandEndSign = CommandEndSignTemp.Left(CommandEndSignTemp.Find(","));
			CommandEndSignTemp = CommandEndSignTemp.Right(CommandEndSignTemp.GetLength() - CommandEndSignTemp.Find(",") - 2);
			CommandEndSignTemp = CommandEndSignTemp.Left(CommandEndSignTemp.GetLength() - 1);
			CommandCut[0] = atoi(CommandEndSignTemp.Left(CommandEndSignTemp.Find(",")));
			CommandCut[1] = atoi(CommandEndSignTemp.Right(CommandEndSignTemp.GetLength() - CommandEndSignTemp.Find(",") - 1));

			Sleep(atoi(paraArray[1].Low_Limit_Value) + WaitTime);
			CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���HexGroupToString
			for (int r = 0; r < 6; r++)
			{
			    memset(str, 0, sizeof(str) / sizeof(str[0]));
				bReadStatus = ReadFile(hPort[HandleNum], str, 32, &dwBytesRead, NULL);//bReadStatus = ReadFile( hPort[HandleNum], buffer, PICDATAMAX, &dwBytesRead, NULL);//Send_Serial_Order
				if (dwBytesRead != 0)
				{
					//if (strstr(EndSign, "NULL"))
					//	p = strstr(str, "\r");
					//else
					//	p = strstr(str, EndSign);
					CString charVtemp1, charVtemp2, charVtemp3;
					for (int i = 0; i < 16; i++)
					{
						charVtemp1.Format(_T("%02X"), (unsigned char)str[i]);
						charVtemp3 += charVtemp1;
					}
					CommandReturn = Decrypt(charVtemp3);
					strCommand_Vaule_Return = CommandReturn;

					if (strCommand_Vaule_Return.Find(CommandEndSign) != -1)//if((p)||(Port_Temp.Find(EndSign)!=-1))
					{
						CString  selPort;
						selPort = strCommand_Vaule_Return;

						//��ָ����н�ȡ,ע�⣬Э����ָ���Ǵ�1��ʼ����Midָ���Ǵ�0����,���Խ�ȡ(7,20)Ӧ���ǽ�ȡ��6������19���ַ�
						if (CommandCut[0] == CommandCut[1])
						{
							selPort = selPort.Mid(CommandCut[0] - 1, 1);//�����һ����ΪMid�Ǵ�0��ʼ��
						}
						else if (CommandCut[0] != CommandCut[1])
						{
							selPort = selPort.Mid(CommandCut[0] - 1, CommandCut[1] - CommandCut[0] + 1);//�����һ����ΪҪ����������
						}

						Vaule_Return_Count_CS[HandleNum] = selPort;

						*Vaule_Return = "Analysis_SUCCESS";
						return "Analysis_SUCCESS";
					}
					else
					{
						//Port_Temp += CString(buffer);
						Sleep(200 * r);
						continue;
					}
				}
				else
				{
					Sleep(200 * r);
					continue;
				}
			}
		}
		//����ʮ����������(��ͨģʽ)
		else if (HexFlag == 2)
		{
			//��תʮ������
			int len = strCommand_Vaule.GetLength() / 2;
			BYTE *pBData = new BYTE[len];
			pBData = CStrToByte(len,strCommand_Vaule);

			//����ʮ����������
			bWriteStat = WriteFile(hPort[HandleNum], (LPVOID)pBData, len, &dwBytesWritten, NULL);

			if (dwBytesWritten != len)
			{
				Sleep(300);
				continue;
			}

			//receive response

			unsigned char str[32];

			Sleep(atoi(paraArray[1].Low_Limit_Value) + WaitTime);
			CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���HexGroupToString
			for (int r = 0; r < WaitTime; r++)
			{
				memset(str, 0, sizeof(str) / sizeof(str[0]));
				bReadStatus = ReadFile(hPort[HandleNum], str, 32, &dwBytesRead, NULL);//bReadStatus = ReadFile( hPort[HandleNum], buffer, PICDATAMAX, &dwBytesRead, NULL);//Send_Serial_Order
				if (dwBytesRead != 0)
				{
					CString charVtemp1, charVtemp2, charVtemp3;
					for (int i = 0; i < 16; i++)
					{
						charVtemp1.Format(_T("%02X"), (unsigned char)str[i]);
						charVtemp3 += charVtemp1;
					}
					strCommand_Vaule_Return = charVtemp3;

					if (strCommand_Vaule_Return.Find(EndSign) != -1)//if((p)||(Port_Temp.Find(EndSign)!=-1))
					{
						CString  selPort;
						selPort = strCommand_Vaule_Return;

						Vaule_Return_Count_CS[HandleNum] = selPort;

						*Vaule_Return = "Analysis_SUCCESS";
						return "Analysis_SUCCESS";
					}
					else
					{
						//Port_Temp += CString(buffer);
						Sleep(200 * r);
						continue;
					}
				}
				else
				{
					Sleep(200 * r);
					continue;
				}
			}
		}
	}
	Vaule_Return_Count_CS[HandleNum] = strCommand_Vaule_Return;	//���ڷ���ֵ
	//CloseHandle(hPort[HandleNum]);							//�ɹ��󲻹رմ���---������ATָ���
	*Vaule_Return = "FAIL";
	return "FAIL";
}

//�رմ������Ϻ�����Ϊ�˼���RDA��MTKƽ̨������������1ΪҪ�رյľ��������2ΪRDAҪ�رյĴ�����ţ�Ĭ�ϵ�255ֻ�ǵ�ȱʡ�����ã�������3����RDA����MTK
BOOL IMEIWrite_MulAT::CloseHandleControl(HANDLE hObject, BOOL RDAFlag, int HandleNum)
{
	BOOL IsClose;
	if (RDAFlag == FALSE||HandleNum>16)
	{
		IsClose=CloseHandle(hObject);
	}
	else if (RDAFlag == TRUE)
	{
		IsClose = RdaHostInterface.RDAComShutdown(HandleNum);
	}

	return IsClose;
}


void IMEIWrite_MulAT::LogShow_exchange(CEdit* m_Result, CEdit* Final_Result_Control, int State, CString Msg_Log, int HandleNum, CString Category, CString ChipRfIDbg)
{
	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[HandleNum];
	}
	CTime showtime = CTime::GetCurrentTime();

	CString showtimeCS;
	showtimeCS.Format(" %02d:%02d:%02d", showtime.GetHour(), showtime.GetMinute(), showtime.GetSecond());

	CString LOG_Show_Old;
	m_ResultArray[HandleNum]->GetWindowText(LOG_Show_Old);
	m_ResultArray[HandleNum]->SetWindowText(Msg_Log + showtimeCS + "\r\n" + LOG_Show_Old.Left(8092));
	m_ResultArray[HandleNum]->UpdateWindow();

	CString HandleNumCS;
	HandleNumCS.Format("%02d", HandleNum + 1);
	if (State == 0)
	{
		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(0, 155, 100);
		else
			gColor[HandleNum] = RGB(0, 155, 100);
		Final_Result_ControlArray[HandleNum]->SetWindowTextA("����" + HandleNumCS);
		//����һ������Ϣ
		if (g_ADCTFlag == TRUE)
		{
			if (get_map_key_value(PortStatusMap, HandleNum) != "")
			{
				SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "����", 0);
				SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), Msg_Log, 1);/*.Left(Msg_Log.GetLength()-8)*/
			}
		}

		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		Thread_State[HandleNum] = TRUE;
	}
	else if (State == 128)
	{
		Final_Result_ControlArray[HandleNum]->SetFont(font);

		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(255, 0, 0);
		else
			gColor[HandleNum] = RGB(255, 0, 0);

		//��ʾ����Ĳ����������
		CString FailInfo = ParaItemName[HandleNum];

		if (FailInfo.Find("TR") != -1)
		{
			FailInfo = FailInfo.Right(FailInfo.GetLength() - FailInfo.Find("TR"));
			FailInfo.Replace("TR", "");
			//FailInfo = FailInfo.Left(FailInfo.Find("TR"));
		}

		if (FailInfo.Find("CR") != -1)
		{
			FailInfo = FailInfo.Right(FailInfo.GetLength() - FailInfo.Find("CR"));
			FailInfo.Replace("CR", "");
		}

		if (FailInfo.Find("DL") != -1)
		{
			FailInfo = FailInfo.Left(FailInfo.Find("DL"));
		}

		if (FailInfo.GetLength() > 12)
		{
			FailInfo = FailInfo.Left(12);
		}

		if (FailInfo == "")
		{
			Final_Result_ControlArray[HandleNum]->SetWindowTextA("ʧ��" + HandleNumCS);
			FailInfo += "ʧ��";
		}
		else
		{
			if (FailInfo.Find("©��") != -1 || FailInfo.Find("���ݿ�") != -1)
			{

			}
			else
				FailInfo += "ʧ��";
		
			Final_Result_ControlArray[HandleNum]->SetFont(font_fail);
			Final_Result_ControlArray[HandleNum]->SetWindowTextA(FailInfo + HandleNumCS);
		}
		//����һ������Ϣ
		if (g_ADCTFlag == TRUE)
		{
			SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "ʧ��", 0);
			SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), Msg_Log, 2);
		}


		Final_Result_Control->UpdateWindow();
		//Thread_State[HandleNum]=FALSE;
		if (Category != "-1")
			Data_UpdateError(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, Msg_Log, Category, ChipRfIDCurrent);


		BOOL FaillResult;
		CString TestType, WorkStation;
		GetDlgItem(IDC_COMBOTESTYYPE)->GetWindowTextA(TestType);
		if (TestType == "DAM�̵���")
		{
			FaillResult = CloseExternalCircuit(HandleNum);
		}
		else
		{
			FaillResult = CPUCommunication("ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
			if (RecoverVoltage == TRUE)CPUCommunication("���1��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
			FaillResult &= CPUCommunication("�ر�ͨ·", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
		}

		CString VoiceMessage;
		VoiceMessage.Format("%s"+FailInfo, HandleNumCS);
		//VoiceMessage.Format("%sʧ��", HandleNumCS);
		Voice_Speak(VoiceMessage);

		if (ChipRfIDbg == "")
		{
			ChipRfID[HandleNum] = "";
			//Software_Version[HandleNum]="";//�����������������Ϊ����汾��һ����
		}

		EnterCriticalSection(&SUCCFAILCOUNT);
		FailCount++;
		CString FailCountCS;
		if (BGConfirmChoose == TRUE)
			FailCountCS.Format("%d", FailCount / 2);
		else
			FailCountCS.Format("%d", FailCount);
		GetDlgItem(IDC_EDITFAILCOUNT)->SetWindowTextA(FailCountCS);
		LeaveCriticalSection(&SUCCFAILCOUNT);
	}
	else if (State == 255)
	{
		Final_Result_ControlArray[HandleNum]->SetFont(font);
		WriteLogIni(HandleNum, ChipRfIDCurrent);
		CString LogCS;
		LogCS = ChipRfIDCurrent + ";" + IMEIRecord[HandleNum] + ";" + BTMac[HandleNum] + ";" + SimNumber[HandleNum] + ";" + NteTemperature[HandleNum] + ";" + GPSSnrAve[HandleNum] + ";" + InchargeI[HandleNum] + ";" + BTTestResult[HandleNum] + ";";
		WriteLog(LogCS + "�ɹ���", HandleNum);
		WriteLogEnd(HandleNum);
		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(0, 255, 0);
		else
			gColor[HandleNum] = RGB(0, 255, 0);
		//Thread_State[HandleNum]=FALSE;
		BOOL SuccessResult=TRUE;
		CString TestType, WorkStation;
		GetDlgItem(IDC_COMBOTESTYYPE)->GetWindowTextA(TestType);
		if (TestType == "DAM�̵���")
		{
			SuccessResult = CloseExternalCircuit(HandleNum);
		}
		else
		{
			SuccessResult = CPUCommunication("�ɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
			if (RecoverVoltage == TRUE)CPUCommunication("���1��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
			SuccessResult &= CPUCommunication("�ر�ͨ·", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
		}

		Final_Result_ControlArray[HandleNum]->SetWindowTextA("�ɹ�" + HandleNumCS);
		//����һ������Ϣ
		if (g_ADCTFlag == TRUE)
		{
			SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "�ɹ�", 0);
		}

		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		if (SuccessResult == FALSE)
		{
			if (ChipRfIDbg != "")
				gColor[HandleNum + 8] = RGB(255, 0, 0);
			else
				gColor[HandleNum] = RGB(255, 0, 0);
			Final_Result_ControlArray[HandleNum]->SetFont(font_fail);
			Final_Result_ControlArray[HandleNum]->SetWindowTextA("�̵����ر�ʧ��" + HandleNumCS);
			Final_Result_ControlArray[HandleNum]->UpdateWindow();
			//����һ������Ϣ
			if (g_ADCTFlag == TRUE)
			{
				SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "�̵����ر�ʧ��", 0);
			}

			CString VoiceMessage;
			VoiceMessage.Format("%sʧ��", HandleNumCS);
			Voice_Speak(VoiceMessage);

			EnterCriticalSection(&SUCCFAILCOUNT);
			FailCount++;
			CString FailCountCS;
			if (BGConfirmChoose == TRUE)
				FailCountCS.Format("%d", FailCount / 2);
			else
				FailCountCS.Format("%d", FailCount);
			GetDlgItem(IDC_EDITFAILCOUNT)->SetWindowTextA(FailCountCS);
			LeaveCriticalSection(&SUCCFAILCOUNT);
		}
		else
		{
			CString VoiceMessage;
			VoiceMessage.Format("%s�ɹ�", HandleNumCS);
			Voice_Speak(VoiceMessage);
			EnterCriticalSection(&SUCCFAILCOUNT);
			SuccessCount++;
			CString SuccessCountCS;
			if (BGConfirmChoose == TRUE)
				SuccessCountCS.Format("%d", SuccessCount / 2);
			else
				SuccessCountCS.Format("%d", SuccessCount);
			GetDlgItem(IDC_EDITSUCCOUNT)->SetWindowTextA(SuccessCountCS);
			LeaveCriticalSection(&SUCCFAILCOUNT);
		}
		if (ChipRfIDbg == "")
		{
			ChipRfID[HandleNum] = "";
			//Software_Version[HandleNum]="";//�����������������Ϊ����汾��һ����
		}
	}
	else if (State == 2550)
	{
		Final_Result_ControlArray[HandleNum]->SetFont(font);

		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(0, 255, 0);
		else
			gColor[HandleNum] = RGB(0, 255, 0);

		//Thread_State[HandleNum]=FALSE;
		BOOL SuccessResult;
		CString TestType, WorkStation;
		GetDlgItem(IDC_COMBOTESTYYPE)->GetWindowTextA(TestType);
		if (TestType == "DAM�̵���")
		{
			SuccessResult = CloseExternalCircuit(HandleNum);
		}
		else
		{
			SuccessResult = CPUCommunication("�ɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
			if (RecoverVoltage == TRUE)CPUCommunication("���1��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
			SuccessResult &= CPUCommunication("�ر�ͨ·", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
		}
		Final_Result_ControlArray[HandleNum]->SetWindowTextA("");

		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		if (SuccessResult == FALSE)
		{
			if (ChipRfIDbg != "")
				gColor[HandleNum + 8] = RGB(255, 0, 0);
			else
				gColor[HandleNum] = RGB(255, 0, 0);
			Final_Result_Control->SetWindowTextA("�̵����ر�ʧ��" + HandleNumCS);
			//����һ������Ϣ
			if (g_ADCTFlag == TRUE)
			{
				SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "�̵����ر�ʧ��", 0);
			}
			Final_Result_ControlArray[HandleNum]->UpdateWindow();
			CString VoiceMessage;
			VoiceMessage.Format("%sʧ��", HandleNumCS);
			Voice_Speak(VoiceMessage);

			EnterCriticalSection(&SUCCFAILCOUNT);
			FailCount++;
			CString FailCountCS;
			if (BGConfirmChoose == TRUE)
				FailCountCS.Format("%d", FailCount / 2);
			else
				FailCountCS.Format("%d", FailCount);
			GetDlgItem(IDC_EDITFAILCOUNT)->SetWindowTextA(FailCountCS);
			LeaveCriticalSection(&SUCCFAILCOUNT);
		}
		else
		{
			CString VoiceMessage;
			VoiceMessage.Format("%s�Ѳ���", HandleNumCS);
			Voice_Speak(VoiceMessage);
			Final_Result_ControlArray[HandleNum]->SetWindowTextA("�Ѳ���");
			Final_Result_ControlArray[HandleNum]->UpdateWindow();
			EnterCriticalSection(&SUCCFAILCOUNT);
			SuccessCount++;
			CString SuccessCountCS;
			if (BGConfirmChoose == TRUE)
				SuccessCountCS.Format("%d", SuccessCount / 2);
			else
				SuccessCountCS.Format("%d", SuccessCount);
			GetDlgItem(IDC_EDITSUCCOUNT)->SetWindowTextA(SuccessCountCS);
			LeaveCriticalSection(&SUCCFAILCOUNT);
		}
		if (ChipRfIDbg == "")
		{
			ChipRfID[HandleNum] = "";
			//Software_Version[HandleNum]="";//�����������������Ϊ����汾��һ����
		}
	}
	else if (State == 250)
	{
		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(60, 190, 190);
		else
			gColor[HandleNum] = RGB(60, 190, 190);
		Final_Result_ControlArray[HandleNum]->SetWindowTextA("W");
		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		Thread_State[HandleNum] = FALSE;
		if (COM_State[HandleNum] == FALSE)
		{
			if (GetRDAHostCheckValue == TRUE)
			{
					if (RdaHostInterface.ComInit[HandleNum] == FALSE)
					{
						EnableWindow_Start(HandleNum);
					}
					else
					{
						RdaHostInterface.RDAComShutdown(HandleNum);
					}
				//m_StopControlArray[HandleNum]->EnableWindow(TRUE);
			}
			else
				EnableWindow_Start(HandleNum);
		}
	}
	else if (State == 251)
	{
		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(60, 190, 190);
		else
			gColor[HandleNum] = RGB(60, 190, 190);
		Final_Result_ControlArray[HandleNum]->SetWindowTextA("W");
		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		Thread_State[HandleNum] = FALSE;
		//EnableWindow_Start(HandleNum);
	}
	else if (State == 5)//�������ԣ�״̬������
	{
		Thread_State[HandleNum] = TRUE;
		if (g_ADCTFlag == TRUE)
		{
			SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), Msg_Log, 1);
		}
	}
	else if (State == 6)//������LOG��Ϣ����������
	{

	}
	else if (State == 256)
	{
		Thread_State[HandleNum] = FALSE;
		Final_Result_ControlArray[HandleNum]->SetWindowTextA("W");
		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		SetFocus_Choose(HandleNum);
	}
	else
	{
		if (ChipRfIDbg != "")
			gColor[HandleNum + 8] = RGB(0, 155, 100);
		else
			gColor[HandleNum] = RGB(0, 155, 100);
		Final_Result_ControlArray[HandleNum]->SetWindowTextA("����" + HandleNumCS);
		//����һ������Ϣ
		if (g_ADCTFlag == TRUE)
		{
			SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "����", 0);
		}
		Final_Result_ControlArray[HandleNum]->UpdateWindow();
		Thread_State[HandleNum] = TRUE;
	}

}
CString IMEIWrite_MulAT::GetData(char* Serial_Order_Return, CString Start, CString End, int Count, int HandleNum)
{
	CString DataGet;
	//CString DataGetTemp;
	int pos = -1;
	CString Serial_Order_Return_CS;
	Serial_Order_Return_CS.Format("%s", Serial_Order_Return);
	for (int i = 0; i<Count; i++)
	{
		if (i == Count - 1)
		{
			if ((Start == "NULL") || (Start == ""))
			{
				pos = Serial_Order_Return_CS.Find(End);
				if (pos >= 0)
				{
					DataGet = Serial_Order_Return_CS.Left(pos);
				}
				else
				{
					return Serial_Order_Return_CS;
				}
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(Start);
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + Start.GetLength());
					pos = Serial_Order_Return_CS.Find(End);
					if (pos >= 0)
					{
						DataGet = Serial_Order_Return_CS.Left(pos);
					}
					else
					{
						return Serial_Order_Return_CS;//return "NULL";
					}
				}
				else
				{
					return "NULL";
				}
			}
		}
		else
		{
			if ((Start == "NULL") || (Start == ""))
			{
				continue;
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(Start);
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + Start.GetLength());
					continue;
				}
				else
				{
					return "NULL";
				}
			}
		}

		return DataGet;
	}
	return "NULL";
}
bool IMEIWrite_MulAT::IMEI_Function_Judge(int i, CString IMEI_FT_Item, char* Serial_Order_Return, int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control)
{
	int IMEI_FT_Item_Int = 0;
	CString Serial_Order_Return_CS;
	Serial_Order_Return_CS.Format("%s", Serial_Order_Return);
	int pos = -1;


	CString GpsID[GPSNUMBERMAX] = { "" };
	CString GpsSNR[GPSNUMBERMAX] = { "" };
	CString GpsSNRTemp[GPSNUMBERMAX] = { "" };
	CString GpsIDTemp[GPSNUMBERMAX] = { "" };
	int GpsNumber = -1;
	int GpsMessage = -1;
	int GpsMessageFirst = -1;
	CString GpsSNRStandardLow[GPSNUMBERMAX] = { "" };
	CString GpsSNRStandardHigh[GPSNUMBERMAX] = { "" };
	CString SNRSetting;
	//��������
	CString RestartCount;
	CString RestartCountLow;
	CString RestartCountHigh;
	//������
	CString VBat;
	CString IBat;
	CString VCharge;

	//CString VBatSetting;
	//CString IBatSetting;
	//CString VChargeSetting;
	CString BBCSettingLow[3 * 2];
	CString BBCSettingHigh[3 * 2];
	CString BBCSetting;


	//���ֹ���һ����Ҫ��һ�������Ż�if else if��䣨ԭ�߼���������do while(0)������,һ����˵���²����Ӧ��ֻ�Ǵ���һ�Σ���Ϊԭ�߼���if elseif�����������˺���breakֱ������������޶Ƚ�ʡʱ��
	do{
		if (IMEI_FT_Item.Find("����") != -1)
		{
			if (IMEI_FT_Item.Find("��GPS����") != -1)
			{
				IMEI_FT_Item_Int = 1;

				pos = Serial_Order_Return_CS.Find("GPGSV");
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 5);
					GpsMessage = atoi(GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 1, HandleNum));//��ЧGPS�������� 
					LogShow_exchange(m_Result, Final_Result_Control, 0, "����GPS��Ϣ����===" + GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 1, HandleNum), HandleNum);

					GpsMessageFirst = atoi(GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 2, HandleNum));//��ЧGPS�������� 
					if (GpsMessageFirst != 1)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 0, "����GPS��ǰ��Ϣ�ǵ�һ��===" + GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 2, HandleNum), HandleNum);
						return false;
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 0, "����GPS��ǰ��Ϣ�ǵ�һ��===" + GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 2, HandleNum) + " ��ʼ����", HandleNum);
					}

					GpsNumber = atoi(GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 3, HandleNum));//��������
					LogShow_exchange(m_Result, Final_Result_Control, 0, "����GPS����:�ѵ���������===" + GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 3, HandleNum), HandleNum);
					for (int j = 0; j<GpsNumber; j++)
					{
						if (GPSDataTypeChoose == TRUE)
						{
							if ((GpsMessage >= 2) && ((j >= 4) && (j <= 7)))//��2����Ϣ
							{
								GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 4 + 4 * (j + 1), HandleNum);//4+===>ÿ����䣬���һ������4�����ų���1
								GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 4 + 4 * (j + 1) + 3, HandleNum);
							}
							else if ((GpsMessage >= 3) && ((j >= 8) && (j <= 11)))//��3����Ϣ
							{
								GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 8 + 4 * (j + 1), HandleNum);//8+===>ÿ����䣬���һ������4�����ų���2
								GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 8 + 4 * (j + 1) + 3, HandleNum);
							}
							else if ((GpsMessage >= 4) && ((j >= 12) && (j <= 15)))//��4����Ϣ
							{
								GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 12 + 4 * (j + 1), HandleNum);//12+===>ÿ����䣬���һ������4�����ų���3
								GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 12 + 4 * (j + 1) + 3, HandleNum);
							}
							else if ((GpsMessage >= 1) && (j <= 3))
							{
								GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 0 + 4 * (j + 1), HandleNum);//4+===>ÿ����䣬���һ������4�����ų���0
								GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 0 + 4 * (j + 1) + 3, HandleNum);
							}
						}
						else//����GPS���ݸ�ʽ
						{
							if ((GpsMessage >= 2) && ((j >= 4) && (j <= 7)))//��2����Ϣ
							{
								if ((GpsNumber <= 8) && (j == GpsNumber - 1))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 3 + 4 * (j + 1), HandleNum);//4+===>$GPGSV,2,2,7,				//3+===>ÿ����䣬���һ������3������
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 3 + 4 * (j + 1) + 3, HandleNum);
								}
								else if ((GpsNumber>8) && (j == 7))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 3 + 4 * (j + 1), HandleNum);//4+===>$GPGSV,2,2,7,
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 3 + 4 * (j + 1) + 3, HandleNum);
								}
								else
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 3 + 4 * (j + 1), HandleNum);//4+===>$GPGSV,2,2,7,
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 3 + 4 * (j + 1) + 3, HandleNum);
								}
							}
							else if ((GpsMessage >= 3) && ((j >= 8) && (j <= 11)))//��3����Ϣ
							{
								if ((GpsNumber <= 12) && (j == GpsNumber - 1))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 6 + 4 * (j + 1), HandleNum);									//6+===>ÿ����䣬���һ������3�����ų���2
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 6 + 4 * (j + 1) + 3, HandleNum);
								}
								else if ((GpsNumber>12) && (j == 11))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 6 + 4 * (j + 1), HandleNum);
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 6 + 4 * (j + 1) + 3, HandleNum);
								}
								else
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 6 + 4 * (j + 1), HandleNum);
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 6 + 4 * (j + 1) + 3, HandleNum);
								}
							}
							else if ((GpsMessage >= 4) && ((j >= 12) && (j <= 15)))//��4����Ϣ
							{
								if ((GpsNumber <= 16) && (j == GpsNumber - 1))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 9 + 4 * (j + 1), HandleNum);									//9+===>ÿ����䣬���һ������3�����ų���3
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 9 + 4 * (j + 1) + 3, HandleNum);
								}
								else if ((GpsNumber>16) && (j == 15))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 9 + 4 * (j + 1), HandleNum);
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 9 + 4 * (j + 1) + 3, HandleNum);
								}
								else
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 9 + 4 * (j + 1), HandleNum);
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 9 + 4 * (j + 1) + 3, HandleNum);
								}
							}
							else if ((GpsMessage >= 1) && (j <= 3))
							{
								if ((GpsNumber <= 4) && (j == GpsNumber - 1))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 4 * (j + 1), HandleNum);	//����GPGSV,��4������
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 4 * (j + 1) + 3, HandleNum);//����GPGSV��7������
								}
								else if ((GpsNumber>4) && (j == 3))
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 4 * (j + 1), HandleNum);	//����GPGSV,��4������
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", "*", 4 * (j + 1) + 3, HandleNum);//����GPGSV��7������
								}
								else
								{
									GpsID[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 4 * (j + 1), HandleNum);//����GPGSV,��4������
									GpsSNR[j] = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ",", ",", 4 * (j + 1) + 3, HandleNum);//����GPGSV��7������
								}
							}
						}
						//LogShow_exchange(m_Result,Final_Result_Control,0,"����GPS����:���Ǳ���= "+GpsID[j]+" �����= "+GpsSNR[j],HandleNum);
					}
					//���ź�ǿ������
					for (int j = 0; j<GpsNumber; j++)
					{
						for (int i = j + 1; i<GpsNumber; i++)
						{
							if (atoi(GpsSNR[j])<atoi(GpsSNR[i]))
							{
								GpsSNRTemp[j] = GpsSNR[j];
								GpsIDTemp[j] = GpsID[j];

								GpsSNR[j] = GpsSNR[i];
								GpsID[j] = GpsID[i];

								GpsSNR[i] = GpsSNRTemp[j];
								GpsID[i] = GpsIDTemp[j];
							}
						}
						LogShow_exchange(m_Result, Final_Result_Control, 0, "�����GPS����:���Ǳ���= " + GpsID[j] + " �����= " + GpsSNR[j], HandleNum);
					}
					//���ź�ǿ������

					break;
				}
				else
				{
					return false;
				}
			}
			else if (IMEI_FT_Item.Find("�ر�GPS����") != -1)
			{
				IMEI_FT_Item_Int = 2;

				break;
			}
			else if (IMEI_FT_Item.Find("152���ܲ���") != -1)
			{
				CString DataCountCS;
				int DataCount = Serial_Order_Return_CS.GetLength();
				DataCountCS.Format("%d", DataCount);
				LogShow_exchange(m_Result, Final_Result_Control, 0, "152���ܲ���������=" + DataCountCS, HandleNum);

				if (DataCount<atoi(paraArray[2].High_Limit_Value))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "���������쳣�������������������ο�(>" + paraArray[2].High_Limit_Value, HandleNum);
					return false;
				}

				RestartCount = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, "Recorded", 1, HandleNum);
				if (RestartCount == "NULL")
					RestartCount = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, "\r", 1, HandleNum);
				RestartCount.Replace("\r", "");
				RestartCount.Replace("\n", "");
				LogShow_exchange(m_Result, Final_Result_Control, 0, "�������� " + RestartCount, HandleNum);
				IMEI_FT_Item_Int = 3;

				break;
			}
			else if (IMEI_FT_Item.Find("�¶Ȳ���") != -1)
			{
				Serial_Order_Return_CS.Replace("\r", "");
				Serial_Order_Return_CS.Replace("\n", "");
				//IMEI_FT_Item_Int=10;
				CString Temperature = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, " C", 1, HandleNum);
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
				if ((atof(Temperature) < atof(CountLow)) || (atof(Temperature) > atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "�¶��쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "�¶�����..", HandleNum);
				}
				NteTemperature[HandleNum] = Serial_Order_Return_CS;

				break;
			}
			else if (IMEI_FT_Item.Find("������") != -1)
			{
				Serial_Order_Return_CS.Replace("\r", "");
				Serial_Order_Return_CS.Replace("\n", "");
				//IMEI_FT_Item_Int=11;
				CString iu = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, " A", 1, HandleNum);
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
				if ((atof(iu) < atof(CountLow)) || (atof(iu) > atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "�������쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "����������..", HandleNum);
				}
				InchargeI[HandleNum] = Serial_Order_Return_CS;

				break;
			}
			else if (IMEI_FT_Item.Find("BT����") != -1)
			{
				Serial_Order_Return_CS.Replace("\r", "");
				Serial_Order_Return_CS.Replace("\n", "");
				pos = Serial_Order_Return_CS.Find('"');
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);
					Serial_Order_Return_CS = Serial_Order_Return_CS.Left(Serial_Order_Return_CS.Find('"'));
				}
				else
				{
				}
				BTTestResult[HandleNum] = Serial_Order_Return_CS;

				break;
			}
			else if (IMEI_FT_Item.Find("��������") != -1)
			{
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);

				Vaule_Return_Count_CS[HandleNum].Format("%f", PortCurrentVauleArray[HandleNum]);

				if (((PortCurrentVauleArray[HandleNum])<atof(CountLow)) || ((PortCurrentVauleArray[HandleNum])>atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "����ֵ��" + Vaule_Return_Count_CS[HandleNum] + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "����ֵ��" + Vaule_Return_Count_CS[HandleNum] + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				}

				break;
			}
		}

		if (IMEI_FT_Item.Find("��ѹ") != -1)
		{
			if (IMEI_FT_Item.Find("�Աȵ�ѹ") != -1)
			{
				//IMEI_FT_Item_Int=12;
				CString Vext;
				if (Serial_Order_Return_CS.Find("Vdc") == -1)
					Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, '(', ')', 1, HandleNum);
				else
					Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, ':', "Vdc", 1, HandleNum);
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
				if ((atof(Vext) < atof(CountLow)) || (atof(Vext) > atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "����ѹ�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "����ѹ����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				}

				break;
			}
			else if (IMEI_FT_Item.Find("ֻ�е�ѹ") != -1)
			{
				//IMEI_FT_Item_Int=12;
				CString Vext;
				if (Serial_Order_Return_CS.Find(')') != -1)
					Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, ')', 1, HandleNum);
				else if (Serial_Order_Return_CS.Find('V') != -1)
					Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, 'V', 1, HandleNum);
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
				if ((atof(Vext) < atof(CountLow)) || (atof(Vext) > atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "ֻ�е�ѹ�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "ֻ�е�ѹ����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				}

				break;
			}
			else if (IMEI_FT_Item.Find("��ص�ѹ") != -1)//charge in, vBat=4.13V,iChr=0.20A,vChr=4.79V
			{
				if (Serial_Order_Return_CS.Find("VBAT") != -1)
				{
					VBat = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, "VBAT=", "V,", 1, HandleNum);
					IBat = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, "ICHR=", "A,", 1, HandleNum);
					VCharge = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, "VCHR=", "V", 1, HandleNum);
				}
				else if (Serial_Order_Return_CS.Find("EADC") != -1)
				{
					CString Temp = Serial_Order_Return_CS.Mid(Serial_Order_Return_CS.Find("EADC:") + 5, 25);

					Temp.Replace("\r", "");
					Temp.Replace("\n", "");
					Temp.Replace(" ", "");
					LogShow_exchange(m_Result, Final_Result_Control, 0, "Temp= " + Temp, HandleNum);
					VBat = GetData((LPSTR)(LPCTSTR)Temp, "NULL", ",", 1, HandleNum);
					IBat = GetData((LPSTR)(LPCTSTR)Temp, ",", ",", 3, HandleNum);
					VCharge = GetData((LPSTR)(LPCTSTR)Temp, ",", "NULL", 4, HandleNum);
				}
				else
				{
					VBat = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, "vBat=", "V,", 1, HandleNum);
					IBat = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, "iChr=", "A,", 1, HandleNum);
					VCharge = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, "vChr=", "V", 1, HandleNum);
				}
				LogShow_exchange(m_Result, Final_Result_Control, 0, "VBat= " + VBat, HandleNum);
				LogShow_exchange(m_Result, Final_Result_Control, 0, "IBat= " + IBat, HandleNum);
				LogShow_exchange(m_Result, Final_Result_Control, 0, "VCharge= " + VCharge, HandleNum);
				IMEI_FT_Item_Int = 4;

				break;
			}
		}

		if (IMEI_FT_Item.Find("RSSI") != -1)
		{
			if (paraArray[i].showName.Find("Dongle����RSSI���") != -1)
			{
				if (paraArray[i].showName.Find("��") != -1 && HandleNum < 8)
				{
					DongleStatusArray[HandleNum] = TRUE;
				}
				else if (paraArray[i].showName.Find("�ر�") != -1 && HandleNum < 8)
				{
					DongleStatusArray[HandleNum] = FALSE;
				}

				break;
			}
			else if (paraArray[i].showName.Find("Dongle����RSSI�ź�ǿ��") != -1)//ֱ�������豸��ֱ�Ӷ������豸�����źż��
			{
				if (GetDongleCheckValue == TRUE)
				{
					CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
					CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);

					//Vaule_Return_Count_CS[HandleNum].Format("%d", Vaule_Return_Count_CS[HandleNum]);

					if ((atoi(Vaule_Return_Count_CS[HandleNum]) < atoi(CountLow)) || (atoi(Vaule_Return_Count_CS[HandleNum]) > atoi(CountHigh)))
					{
						LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "RSSI�ź�ǿ�ȣ�" + Vaule_Return_Count_CS[HandleNum] + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
						CString str;
						Send_Serial_Order(&str, "AT+RSSI?", HandleNum, "Cancelled", "");
						return false;
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "RSSI�ź�ǿ�ȣ�" + Vaule_Return_Count_CS[HandleNum] + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					}
				}
				else if (GetDongleScanCheckValue == TRUE)
				{
					DongleTestRssiDeq.push_back(HandleNum);

					if (DequeContinueControlFun(HandleNum, DongleTestRssiDeq)==FALSE)
					{
						return false;
					}

					//��map��Ѱ���豸���Ҳ����ͷ���false
					CString MacStr,MacRssiStr;
					int MacRssiInt;
					m_ShowNumberPortControlArray[HandleNum]->GetWindowTextA(MacStr);

					map<CString, int>::iterator iter;
					iter = DongleRssiMap.find(MacStr);
					if (iter != DongleRssiMap.end())
					{
						MacRssiInt = iter->second;

						MacRssiStr.Format("%d", MacRssiInt);

						//�ҵ��豸��ʼ�жϴ�С
						CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
						CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);

						if (MacRssiInt < atoi(CountLow) || MacRssiInt > atoi(CountHigh))
						{
							LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "RSSI�ź�ǿ�ȣ�" + MacRssiStr + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
							DongleRssiMap.erase(iter);
							DongleTestRssiDeq.pop_front();
							return false;
						}
						else
						{
							LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "RSSI�ź�ǿ�ȣ�" + MacRssiStr + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
							DongleRssiMap.erase(iter);
							DongleTestRssiDeq.pop_front();
							break;
						}
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "�Ҳ�����Ӧ�豸��", HandleNum);
						DongleTestRssiDeq.pop_front();
						return false;
					}
				}
				break;
			}
			else if (IMEI_FT_Item.Find("RSSI:[") != -1)
			{
				//IMEI_FT_Item_Int=12;
				CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, ']', 1, HandleNum);
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
				if ((atof(Vext)<atof(CountLow)) || (atof(Vext)>atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "RSSI�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "RSSI����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				}

				break;
			}
			else if (IMEI_FT_Item.Find("SSIDRSSI") != -1)//SSIDRSSI
			{
				CString LowDataMul[THREAD_NUM];
				CString HighDataMul[THREAD_NUM];

				CString LimitSetting;
				if (WIFI_RSSILimitCS == "")
					LimitSetting = paraArray[i].High_Limit_Value;
				else
					LimitSetting = WIFI_RSSILimitCS;
				//CString LimitSetting=paraArray[i].High_Limit_Value;
				for (int j = 0; j<THREAD_NUM; j++)
				{
					if (j == 0)
					{
						LowDataMul[j] = GetData((LPSTR)(LPCTSTR)LimitSetting, "NULL", ",", 1, HandleNum);
						HighDataMul[j] = GetData((LPSTR)(LPCTSTR)LimitSetting, ",", ",", 1, HandleNum);
					}
					else
					{
						LowDataMul[j] = GetData((LPSTR)(LPCTSTR)LimitSetting, ",", ",", 2 * j, HandleNum);
						HighDataMul[j] = GetData((LPSTR)(LPCTSTR)LimitSetting, ",", ",", 2 * j + 1, HandleNum);
					}
				}

				//IMEI_FT_Item_Int=12;
				CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, ')', 1, HandleNum);
				//CString CountLow=GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value,"NULL",   ",",1,HandleNum);
				//CString CountHigh=GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value,",",   ",",1,HandleNum);
				//if((atof(Vext)<atof(CountLow))||(atof(Vext)>atof(CountHigh)))
				if ((atof(Vext)<atof(LowDataMul[HandleNum])) || (atof(Vext)>atof(HighDataMul[HandleNum])))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "RSSI�쳣�������ο�(>" + LowDataMul[HandleNum] + " <" + HighDataMul[HandleNum], HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "RSSI����..�ο�(>" + LowDataMul[HandleNum] + " <" + HighDataMul[HandleNum], HandleNum);
				}

				break;
			}
			else//������RSSI���
			{
				CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, ')', 1, HandleNum);
				CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
				CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
				if ((atof(Vext) < atof(CountLow)) || (atof(Vext) > atof(CountHigh)))
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "RSSI�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "RSSI����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				}

				break;
			}

		}


		if (IMEI_FT_Item.Find("IMEI���") != -1)
		{
			int pos = Serial_Order_Return_CS.Find(':');
			if (pos >= 0)
				Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);

			pos = Serial_Order_Return_CS.Find('"');
			if (pos >= 0)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);
				Serial_Order_Return_CS = Serial_Order_Return_CS.Left(Serial_Order_Return_CS.Find('"'));
			}
			else
			{
			}
			IMEIRecord[HandleNum] = Serial_Order_Return_CS;
			if (CompareIMEI == TRUE)
			{
				if (IMEI_Input[HandleNum] != IMEIRecord[HandleNum])
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "IMEI�ȶ�ʧ�ܣ�����������������\r\n" + IMEI_Input[HandleNum] + "!=" + IMEIRecord[HandleNum], HandleNum);
					return false;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "IMEI�ȶԳɹ�����\r\n" + IMEI_Input[HandleNum] + "==" + IMEIRecord[HandleNum], HandleNum);
				}
			}
			IMEI_FT_Item_Int = 8;

			break;
		}
		else if (IMEI_FT_Item.Find("оƬID") != -1)
		{
			CString LowLimitStr = paraArray[i].Low_Limit_Value;
			SIMisOK[HandleNum] = FALSE;

			IMEI_FT_Item_Int = 9;
			Serial_Order_Return_CS.Replace(" ", "");
			LowLimitStr.Replace(" ", "");
			//Serial_Order_Return_CS.Replace("\r", "");
			//Serial_Order_Return_CS.Replace("\n", "");
			//int pos=Serial_Order_Return_CS.Find("ChipRID:");
			int pos = Serial_Order_Return_CS.Find(LowLimitStr);
			if (pos >= 0)
				Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + LowLimitStr.GetLength());//Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(pos+8);

			if (Serial_Order_Return_CS.Find(":") != -1 && paraArray[i].Low_Limit_Value.Find(":") == -1)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Right(Serial_Order_Return_CS.GetLength() - Serial_Order_Return_CS.Find(":") - 1);
			}

			int FindCount = Serial_Order_Return_CS.Find("\r\n");
			if (FindCount != -1)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Left(FindCount);
			}

			Serial_Order_Return_CS.Replace("\r", "");
			Serial_Order_Return_CS.Replace("\n", "");


			CString FeiFaTest = Serial_Order_Return_CS;
			if (FeiFaTest.Trim("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() != 0)
			{
				LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "���ڷǷ��ַ�", HandleNum);
				return false;
			}


			if (IMEI_FT_Item.Find("оƬIDCounterX") != -1)
			{
			}
			else if (IMEI_FT_Item.Find("оƬIDCounter") != -1)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Left(atoi(IMEI_FT_Item.Right(2)));
				if (Serial_Order_Return_CS.GetLength() != atoi(IMEI_FT_Item.Right(2)))
					return false;
			}
			else
			{
				//Serial_Order_Return_CS = Serial_Order_Return_CS.Left(32);
				if (Serial_Order_Return_CS.GetLength() != 32)
					return false;
			}


			ChipRfID[HandleNum] = Serial_Order_Return_CS;//�߳�0��1


			//������Ҫȥ����ܱ������SN��Ҳ����оƬID����������²����򲻽��м��
			if (IMEI_FT_Item.Find("���") != -1 && WorkStationCS == "SMT����")
			{

				//����ֻ�������ݿ���
				EnterCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
				BOOL UpFlag = Data_AntiDupSNCheck(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, ChipRfID[HandleNum]);
				LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);

				//���²��Ե��������
				if (RTestChoose == TRUE&&UpFlag == FALSE)
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "���²��Բ�����", HandleNum);
					AntiDupDataSNUploadFlag[HandleNum] = FALSE;//�������ݿ�������
				}
				else if (RTestChoose == FALSE&&UpFlag == FALSE)
				{
					if (AntiDupDataLinkFlag[HandleNum][0] == 2 || AntiDupDataLinkFlag[HandleNum][0] == 0)//û������ָ���LINK������Բ��ϵ�����¶�����ԭ�߼�
					{
						return false;
					}
					else if (AntiDupDataLinkFlag[HandleNum][0] == 1)//�˻��Ѿ����Թ������ǲ����κβ���,��ʾ�Ѳ������
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�˻��Ѳ��Թ�", HandleNum);
						AntiDupDataSNUploadFlag[HandleNum] = FALSE;//�������ݿ�������
					}
				}

				//ֻҪ�������Ǿ�һ��Ҫ�ϴ�
				if (UpFlag == TRUE)
				{
					AntiDupDataSNUploadFlag[HandleNum] = TRUE;//���������ʶ������ͻ��ϴ�оƬID
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "оƬID�����ݿ��в�����", HandleNum);
				}
			}

			break;
		}
		else if (IMEI_FT_Item.Find("LINK���ظ�") != -1)//link��ʼĬ����3�����ǰҪд��6��������ǰĬ��д��9
		{
			CString LowLimitStr = paraArray[i].Low_Limit_Value;
			if (IMEI_FT_Item.Find("��") != -1)
			{
				Serial_Order_Return_CS.Replace(" ", "");
				LowLimitStr.Replace(" ", "");
				Serial_Order_Return_CS.Replace("\r", "");
				Serial_Order_Return_CS.Replace("\n", "");
				int pos = Serial_Order_Return_CS.Find(LowLimitStr);
				if (pos >= 0)
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + LowLimitStr.GetLength());//Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(pos+8);

				if (Serial_Order_Return_CS.Find(":") != -1)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Right(Serial_Order_Return_CS.GetLength() - Serial_Order_Return_CS.Find(":") - 1);
				}

				if (Serial_Order_Return_CS == paraArray[i].High_Limit_Value)
				{
					AntiDupDataLinkFlag[HandleNum][0] = 1;
				}
				else if (Serial_Order_Return_CS != paraArray[i].High_Limit_Value)
				{
					AntiDupDataLinkFlag[HandleNum][0] = 2;
				}
			}
			//����д��ָ�����Ҫ��ָ���ó����������Щдָ�һ��
			else if (IMEI_FT_Item.Find("д") != -1)
			{
				AntiDupDataLinkFlag[HandleNum][1] = 1;
				AntiDupDataLinkOrder[HandleNum] = paraArray[i].Other_ITEM;
				AntiDupDataLinkOrder[HandleNum].Replace(AntiDupDataLinkOrder[HandleNum].Right(AntiDupDataLinkOrder[HandleNum].GetLength() - AntiDupDataLinkOrder[HandleNum].Find(",") - 1), "3");
			}

			break;
		}
		else if (IMEI_FT_Item.Find("����汾") != -1)
		{
			pos = Serial_Order_Return_CS.Find(':');
			if (pos >= 0)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);

				pos = Serial_Order_Return_CS.Find(' ');
				if (pos >= 0)
					Serial_Order_Return_CS = Serial_Order_Return_CS.Left(pos);
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(']');
				if (pos >= 0)
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);

				pos = Serial_Order_Return_CS.Find('[');
				if (pos >= 0)
					Serial_Order_Return_CS = Serial_Order_Return_CS.Left(pos);
			}

			Software_Version[HandleNum] = Software_VersionFull;

			break;
		}
		else if (IMEI_FT_Item.Find("������ַ") != -1)
		{
			pos = Serial_Order_Return_CS.Find('"');
			if (pos >= 0)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);
				Serial_Order_Return_CS = Serial_Order_Return_CS.Left(Serial_Order_Return_CS.Find('"'));
			}
			else
			{
			}
			BTMac[HandleNum] = Serial_Order_Return_CS;

			break;
		}
		else if (IMEI_FT_Item.Find("��ѯIMSI") != -1)
		{
			pos = Serial_Order_Return_CS.Find('"');
			if (pos >= 0)
			{
				Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + 1);
				Serial_Order_Return_CS = Serial_Order_Return_CS.Left(Serial_Order_Return_CS.Find('"'));
			}
			else
			{
			}
			SimNumber[HandleNum] = Serial_Order_Return_CS;

			break;
		}
		else if (IMEI_FT_Item.Find("ģ�����") != -1)
		{
			SimulationCurrent[HandleNum].Replace("\r", "");
			SimulationCurrent[HandleNum].Replace("\n", "");
			CString iu = GetData((LPSTR)(LPCTSTR)SimulationCurrent[HandleNum], "", " A", 1, HandleNum);
			CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
			CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
			if ((atof(iu)<atof(CountLow)) || (atof(iu)>atof(CountHigh)))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, SimulationCurrent[HandleNum] + "ģ������쳣������" + iu + "�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				SimulationCurrent[HandleNum] = "";
				return false;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "ģ���������.." + iu, HandleNum);
				SimulationCurrent[HandleNum] = "";
			}
			InchargeI[HandleNum] = Serial_Order_Return_CS;

			break;
		}
		else if (IMEI_FT_Item.Find("����������") != -1)
		{
			pos = Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value);
			if (pos >= 0)
			{
				return false;
			}
			else
			{
			}

			break;
		}
		else if (IMEI_FT_Item.Find("��ȡ����") != -1)
		{
			//IMEI_FT_Item_Int=12;
			CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, ')', 1, HandleNum);
			CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
			CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
			if ((atof(Vext)<atof(CountLow)) || (atof(Vext)>atof(CountHigh)))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				return false;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
			}

			break;
		}
		else if (IMEI_FT_Item.Find("����һ") != -1)
		{
			//IMEI_FT_Item_Int=12;
			CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, ',', 1, HandleNum);
			CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
			CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
			if ((atof(Vext)<atof(CountLow)) || (atof(Vext)>atof(CountHigh)))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				return false;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
			}

			break;
		}
		else if (IMEI_FT_Item.Find("����������") != -1)
		{
			CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, 'T', 1, HandleNum);
			CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
			CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
			if ((atof(Vext)<atof(CountLow)) || (atof(Vext)>atof(CountHigh)))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				return false;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
			}

			break;
		}
		else if (IMEI_FT_Item.Find("SIM�����") != -1)
		{
			SIMisOK[HandleNum] = TRUE;

			break;
		}
		else if (IMEI_FT_Item.Find("���ڻ�ȡ16�����ַ���") != -1)//"���ڻ�ȡͼƬ"
		{
			char OutStringBuffer[PICDATAMAX] = { 0 };
			CString RecString;
			HexGroupToString(OutStringBuffer, (unsigned char *)(LPSTR)(LPCTSTR)Serial_Order_Return_CS, Serial_Order_Return_CS.GetLength());
			RecString.Format("%s", OutStringBuffer);//--------------------------------------------------------------ע�⿴�Ƿ���Ҫ����ո�
			StartShowPic(HandleNum, m_Result, Final_Result_Control, RecString);

			break;
		}
		else if (IMEI_FT_Item.Find("ֵ����") != -1)//**
		{
			CString strH = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "**", ",", 1, HandleNum);
			CString Vext = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, strH, 1, HandleNum);

			CString CountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
			CString CountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
			if ((atof(Vext)<atof(CountLow)) || (atof(Vext)>atof(CountHigh)))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "�쳣�������ο�(>" + CountLow + " <" + CountHigh, HandleNum);
				return false;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, IMEI_FT_Item + "����..�ο�(>" + CountLow + " <" + CountHigh, HandleNum);
			}

			break;
		}
		else if (paraArray[i].showName.Find("Socket�Զ��ж�") != -1)//�������Զ�ʶ��ͼƬ�õ�
		{
			BOOL ImageJudegeResult;
			if (paraArray[i].showName.Find("������") != -1)
			{
				double Range = 3.0;
				int RangeCut = paraArray[i].High_Limit_Value.Find("��Χ:");
				if (RangeCut != -1)
				{
					Range = atof(paraArray[i].High_Limit_Value.Right(paraArray[i].High_Limit_Value.GetLength() - RangeCut - 5));
				}

				if (paraArray[i].showName.Find("ҹ��") != -1)
				{
					if (PathFileExists(SocketServerPicName[0][HandleNum]) != TRUE)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, "ҹ��ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST");
						return false;
					}

					ImageJudegeResult = ImageAutoJudgeDefinition(HandleNum, SocketServerPicName[0][HandleNum], Range);

					if (ImageJudegeResult == FALSE)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, "ҹ��ͼƬ�Զ����Բ�ͨ��\r\n", HandleNum, "���ܲ���:2-TEST");
						return false;
					}
				}
				else if (paraArray[i].showName.Find("����") != -1)
				{
					if (PathFileExists(SocketServerPicName[1][HandleNum]) != TRUE)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, "����ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST");
						return false;
					}
					ImageJudegeResult = ImageAutoJudgeDefinition(HandleNum, SocketServerPicName[1][HandleNum], Range);
					if (ImageJudegeResult == FALSE)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, "����ͼƬ�Զ����Բ�ͨ��\r\n", HandleNum, "���ܲ���:2-TEST");
						return false;
					}
				}

			}
			else if (paraArray[i].showName.Find("����") != -1)
			{
				int RGB = 30;
				double Range = 0.0002;
				int RGBCut = paraArray[i].High_Limit_Value.Find("RGBֵ:");
				if (RGBCut != -1)
				{
					RGB = atoi(paraArray[i].High_Limit_Value.Mid(RGBCut + 6, paraArray[i].High_Limit_Value.Find(",") - RGBCut - 6));
				}
				int RangeCut = paraArray[i].High_Limit_Value.Find("��Χ:");
				if (RangeCut != -1)
				{
					Range = atoi(paraArray[i].High_Limit_Value.Right(paraArray[i].High_Limit_Value.GetLength() - RangeCut - 5));
				}

				if (PathFileExists(SocketServerPicName[2][HandleNum]) != TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "����ͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST");
					return false;
				}
				ImageJudegeResult = ImageAutoJudgeDarkCorner(HandleNum, SocketServerPicName[2][HandleNum], RGB, Range);
				if (ImageJudegeResult == FALSE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "����ͼƬ�Զ����Բ�ͨ��\r\n", HandleNum, "���ܲ���:2-TEST");
					return false;
				}
			}

			break;
		}
		//123456(PRE)15(LENGTH)123456789012345-123456789022345(SectionNumber)��ȡ���� //1��RID,�̶��ϴ�,2��Mac��ַ��Ψһ��,���������
		else if (paraArray[i].showName.Find("��ȡ����") != -1)
		{
			CString ItemName = paraArray[i].showName;

			CString LowLimitStr, PreStr, LengthStr, SectionNumberLowStr, SectionNumberHighStr, SectionNumberStr, NumberStr, DataNoStr;

			int DataNoInt;

			//�Ƚ�����ȡ����
			LowLimitStr = paraArray[i].Low_Limit_Value;
			Serial_Order_Return_CS.Replace(" ", "");
			LowLimitStr.Replace(" ", "");
			//Serial_Order_Return_CS.Replace("\r", "");
			//Serial_Order_Return_CS.Replace("\n", "");

			int pos = Serial_Order_Return_CS.Find(LowLimitStr);
			if (pos >= 0)
				NumberStr = Serial_Order_Return_CS.Mid(pos + LowLimitStr.GetLength());
			else
				return false;

			if (NumberStr.Find(":") != -1 && paraArray[i].Low_Limit_Value.Find(":") == -1)
			{
				CString ICount = NumberStr;
				int count = ICount.Replace(":", "");
				if (count != 5)
				{
					NumberStr = NumberStr.Right(NumberStr.GetLength() - NumberStr.Find(":") - 1);
				}
			}

			pos = NumberStr.Find('"');
			if (pos >= 0)
			{
				NumberStr = NumberStr.Mid(pos + 1);
				NumberStr = NumberStr.Left(NumberStr.Find('"'));
			}
			else
			{
			}

			pos = NumberStr.Find('<');
			if (pos >= 0)
			{
				NumberStr = NumberStr.Mid(pos + 1);
				NumberStr = NumberStr.Left(NumberStr.Find('>'));
			}
			else
			{
			}

			int FindCount = NumberStr.Find("\r\n");
			if (FindCount != -1)
			{
				NumberStr = NumberStr.Left(FindCount);
			}

			NumberStr.Replace("\r", "");
			NumberStr.Replace("\n", "");


			//NumberStr = GetData((LPSTR)(LPCTSTR)Serial_Order_Return_CS, paraArray[i].Low_Limit_Value, "", 1, HandleNum);

			//�ж�ǰ׺
			if (ItemName.Find("(PRE)") != -1)
			{
				PreStr = ItemName.Left(ItemName.Find("(PRE)"));

				if (NumberStr.Find(PreStr) != 0)//������ҳ���������0���Ǿʹ���ǰ׺�϶�����
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�ֶ�ǰ׺����", HandleNum);
					return false;
				}

				ItemName = ItemName.Right(ItemName.GetLength() - ItemName.Find("(PRE)") - 5);//��ȡ��PRE����
			}


			//�жϳ���
			if (ItemName.Find("(LENGTH)") != -1)
			{
				LengthStr = ItemName.Left(ItemName.Find("(LENGTH)"));

				if (NumberStr.GetLength() != atoi(LengthStr))//���Ȳ��Ծͷ��ش���
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�ֶγ��ȴ���", HandleNum);
					return false;
				}


				ItemName = ItemName.Right(ItemName.GetLength() - ItemName.Find("(LENGTH)") - 8);//��ȡ��LENGTH����
			}

			//�ж��Ƿ��ڷ�Χ��
			if (ItemName.Find("(SectionNumber)") != -1)
			{
				SectionNumberStr = ItemName.Left(ItemName.Find("(SectionNumber)"));

				SectionNumberHighStr = SectionNumberStr.Left(SectionNumberStr.Find("-"));

				SectionNumberLowStr = SectionNumberStr.Right(SectionNumberStr.GetLength() - SectionNumberStr.Find("-") - 1);

				if (NumberStr < SectionNumberHighStr || NumberStr >SectionNumberLowStr)//�������ֵ����С����Сֵ�ͷ��ش���
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�Ŷη�Χ����", HandleNum);
					return false;
				}

				ItemName = ItemName.Right(ItemName.GetLength() - ItemName.Find("(SectionNumber)") - 15);//��ȡ��SectionNumber����
			}


			if (WorkStationCS == "SMT����")
			{
				int count = 0;
				count = sscanf_s(CStringA(ItemName), "��ȡ����%d", &DataNoInt);//�ø�ʽ��������һ������
				if (count == 0)
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��ȡ������ʧ�ܣ���ȷ���Ƿ�Ϊ��ȡ����+����(�����ȡ����1)", HandleNum);
					return false;
				}


				DataNoStr.Format("Data%d", DataNoInt);//�ٸ�ʽ��һ�£�����ÿ�������ֶξͲ���Ҫ�ٸĴ���

				AntiDupDataNoUploadCount[HandleNum][0]++;//��ȡ����Ĵ���
				AntiDupDataArray[HandleNum][AntiDupDataNoUploadCount[HandleNum][0] - 1] = DataNoStr;//���ݴ�������ֶ���
				AntiDupDataVauleArray[HandleNum][AntiDupDataNoUploadCount[HandleNum][0] - 1] = NumberStr;//�ٽ�ֵȡ����


				//�ж����ݿ����Ƿ�������ݣ��������
				//�������²��Ե������ֱ�Ӳ��ֶ�,�ֶ�����Ѿ����ڣ�оƬID��ͬ�ͱ������ϴ����Ĵ��󣬲�ͬ�ͱ����ͬʱ�������ֶζ�Ӧ�󶨵�оƬID����������ھͲ���
				//��������²��ԣ�ֱ�Ӳ��ֶΣ��ֶ�����Ѿ����ڣ�оƬID��ͬ�Ͳ�����оƬID��ͬ��Ҫ�������ֶζ�Ӧ�󶨵�оƬID����������ֶβ����ڣ�оƬID�����ھͲ��룬оƬID���ھ�Ҫ��ȥ��һ�´�оƬID�Ƿ��Ѿ��󶨹��ֶΣ��󶨹��ͱ�ʧ��

				//��ʼ�������ݿ���
				EnterCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
				BOOL UpFlag = Data_AntiDupDataNoCheck(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, ChipRfID[HandleNum]);
				LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);

				if (UpFlag == FALSE)
				{
					AntiDupDataVauleArray[HandleNum][AntiDupDataNoUploadCount[HandleNum][0] - 1] = "";
					AntiDupDataArray[HandleNum][AntiDupDataNoUploadCount[HandleNum][0] - 1] = "";
					AntiDupDataNoUploadCount[HandleNum][0]--;
					return false;
				}
			}
			break;
		}
		else
			IMEI_FT_Item_Int = 32;

	} while(0);


	//�޳�ͻ�Ĺؼ���
	if (paraArray[i].showName.Find("�ֶ���ʾ") != -1)//��ʾ�����ֶ���
	{
		if (HandleNum < 8&&GetDongleCheckValue==FALSE)
		{
			CString LowLimitStr, PreStr, LengthStr, SectionNumberLowStr, SectionNumberHighStr, SectionNumberStr, NumberStr, DataNoStr;

			//�Ƚ�����ȡ����
			LowLimitStr = paraArray[i].Low_Limit_Value;
			Serial_Order_Return_CS.Replace(" ", "");
			LowLimitStr.Replace(" ", "");

			int pos = Serial_Order_Return_CS.Find(LowLimitStr);
			if (pos >= 0)
				NumberStr = Serial_Order_Return_CS.Mid(pos + LowLimitStr.GetLength());
			else
				return false;

			if (NumberStr.Find(":") != -1 && paraArray[i].Low_Limit_Value.Find(":") == -1)
			{
				CString ICount = NumberStr;
				int count = ICount.Replace(":", "");
				if (count != 5)
				{
					NumberStr = NumberStr.Right(NumberStr.GetLength() - NumberStr.Find(":") - 1);
				}
			}

			pos = NumberStr.Find('"');
			if (pos >= 0)
			{
				NumberStr = NumberStr.Mid(pos + 1);
				NumberStr = NumberStr.Left(NumberStr.Find('"'));
			}
			else
			{
			}

			pos = NumberStr.Find('<');
			if (pos >= 0)
			{
				NumberStr = NumberStr.Mid(pos + 1);
				NumberStr = NumberStr.Left(NumberStr.Find('>'));
			}
			else
			{
			}

			int FindCount = NumberStr.Find("\r\n");
			if (FindCount != -1)
			{
				NumberStr.Left(FindCount);
			}

			NumberStr.Replace("\r", "");
			NumberStr.Replace("\n", "");

			m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA(NumberStr);
		}
		else if (GetDongleCheckValue == TRUE)
		{
			m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA(Serial_Order_Return_CS);
		}
	}


	BOOL GPSresult = TRUE;
	switch (IMEI_FT_Item_Int)
	{
	case 1://GPS-30,-40,   -30,-40,   -30,-40,   -30,-40,   -30,-40,   -30,-40,   -30,-40,   //1,1    2,3   4,5  6,7
		if (GpsNumber<atoi(paraArray[3].High_Limit_Value))//������������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "��������������������" + paraArray[3].High_Limit_Value + " ��,����...", HandleNum);
			return false;
		}
		GPS_SNRLimit.GetWindowTextA(GPS_SNRLimitCS);

		if (GPS_SNRLimitCS == "")
			SNRSetting = paraArray[i].High_Limit_Value;
		else
			SNRSetting = GPS_SNRLimitCS;

		LogShow_exchange(m_Result, Final_Result_Control, 0, SNRSetting, HandleNum);
		SNRSetting.Replace(" ", "");
		for (int j = 0; j<THREAD_NUM; j++)
		{
			if (j == 0)
			{
				GpsSNRStandardLow[j] = GetData((LPSTR)(LPCTSTR)SNRSetting, "NULL", ",", 1, HandleNum);
				GpsSNRStandardHigh[j] = GetData((LPSTR)(LPCTSTR)SNRSetting, ",", ",", 1, HandleNum);
			}
			else
			{
				GpsSNRStandardLow[j] = GetData((LPSTR)(LPCTSTR)SNRSetting, ",", ",", 2 * j, HandleNum);
				GpsSNRStandardHigh[j] = GetData((LPSTR)(LPCTSTR)SNRSetting, ",", ",", 2 * j + 1, HandleNum);
			}
		}
		for (int j = 0; j<atoi(paraArray[3].High_Limit_Value); j++)
		{
			if ((atoi(GpsSNR[j])<atoi(GpsSNRStandardLow[HandleNum])) || (atoi(GpsSNR[j])>atoi(GpsSNRStandardHigh[HandleNum])))//GPS�����
			{
				GPSresult = FALSE;
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���Ǻ�" + GpsID[j] + " ������쳣�������ο�(>" + GpsSNRStandardLow[HandleNum] + " <" + GpsSNRStandardHigh[HandleNum], HandleNum);
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���Ǻ�" + GpsID[j] + " ��Ӧ�����������..", HandleNum);
			}
		}
		if (GPSresult == FALSE)
			return false;
		GPSSnrAve[HandleNum] = GpsSNR[0];
		break;
	case 2:
		//�ر�GPS����
		break;
	case 3:
		RestartCountLow = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, "NULL", ",", 1, HandleNum);
		RestartCountHigh = GetData((LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, ",", ",", 1, HandleNum);
		if ((atoi(RestartCount)<atoi(RestartCountLow)) || (atoi(RestartCount)>atoi(RestartCountHigh)))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���������쳣�������ο�(>" + RestartCountLow + " <" + RestartCountHigh, HandleNum);
			return false;
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "������������..", HandleNum);
		}
		break;
	case 4:
		//charge in, vBat=4.13V,iChr=0.20A,vChr=4.79V
		BBCSetting = paraArray[i].High_Limit_Value;
		BBCSetting.Replace(" ", "");
		for (int j = 0; j<6; j++)
		{
			if (j == 0)
			{
				BBCSettingLow[j] = GetData((LPSTR)(LPCTSTR)BBCSetting, "NULL", ",", 1, HandleNum);
				BBCSettingHigh[j] = GetData((LPSTR)(LPCTSTR)BBCSetting, ",", ",", 1, HandleNum);
			}
			else
			{
				BBCSettingLow[j] = GetData((LPSTR)(LPCTSTR)BBCSetting, ",", ",", 2 * j, HandleNum);
				BBCSettingHigh[j] = GetData((LPSTR)(LPCTSTR)BBCSetting, ",", ",", 2 * j + 1, HandleNum);
			}
		}
		if ((atof(VBat)<atof(BBCSettingLow[0])) || (atof(VBat)>atof(BBCSettingHigh[0])) \
			|| (atof(IBat)<atof(BBCSettingLow[1])) || (atof(IBat)>atof(BBCSettingHigh[1])) \
			|| (atof(VCharge)<atof(BBCSettingLow[2])) || (atof(VCharge)>atof(BBCSettingHigh[2])) \
			)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "��ص�ѹ������������ѹ�쳣�������ο�1(>" + BBCSettingLow[0] + " <" + BBCSettingHigh[0] + "(>" + BBCSettingLow[1] + " <" + BBCSettingHigh[1] + "(>" + BBCSettingLow[2] + " <" + BBCSettingHigh[2], HandleNum);
			if ((atof(VBat)<atof(BBCSettingLow[0 + 3])) || (atof(VBat)>atof(BBCSettingHigh[0 + 3])) \
				|| (atof(IBat)<atof(BBCSettingLow[1 + 3])) || (atof(IBat)>atof(BBCSettingHigh[1 + 3])) \
				|| (atof(VCharge)<atof(BBCSettingLow[2 + 3])) || (atof(VCharge)>atof(BBCSettingHigh[2 + 3])) \
				)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "��ص�ѹ������������ѹ�쳣�������ο�2(>" + BBCSettingLow[0 + 3] + " <" + BBCSettingHigh[0 + 3] + "(>" + BBCSettingLow[1 + 3] + " <" + BBCSettingHigh[1 + 3] + "(>" + BBCSettingLow[2 + 3] + " <" + BBCSettingHigh[2 + 3], HandleNum);
				return false;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "��ص�ѹ������������ѹ����..�ο�2(>" + BBCSettingLow[0 + 3] + " <" + BBCSettingHigh[0 + 3] + "(>" + BBCSettingLow[1 + 3] + " <" + BBCSettingHigh[1 + 3] + "(>" + BBCSettingLow[2 + 3] + " <" + BBCSettingHigh[2 + 3], HandleNum);
			}

		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "��ص�ѹ������������ѹ����..�ο�1(>" + BBCSettingLow[0] + " <" + BBCSettingHigh[0] + "(>" + BBCSettingLow[1] + " <" + BBCSettingHigh[1] + "(>" + BBCSettingLow[2] + " <" + BBCSettingHigh[2], HandleNum);
		}

		break;
	default:
		return true;
		break;
	}
	return true;
}

//ָ������������˺����������ж˿ڵ�ָ����ԣ�ͬʱ���һЩ����ָ����н���
BOOL IMEIWrite_MulAT::WriteIMEIFunction_Thread(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CEdit* Data_Input_Control)
{
	//---------------------------11
	CString PortType_CS;
	PortType.GetWindowTextA(PortType_CS);
	CTime showtime;

	/*�����ǲ��ֹ�������ǰҪ��������---�����￪ʼ*/

	//��ʼ��һ���������ź�ǿ���Ƿ�ʼ����ֵ
	if (HandleNum < 8)
	{
		DongleStatusArray[HandleNum] = FALSE;
		DongleTestRssiInfoArray[HandleNum] = "";
	}

	//���￪������ɨ��ǹ��ͨ��ɨ������������
	if (GetDongleCheckValue == TRUE)
	{
		if (DongleScanGun(HandleNum)==FALSE)
			return false;
	}

	//����ֻ��Ҫɨ������������Ҫ�����ӣ�����ǵڰ˸������Լ��Ժ����������ɨ��Ĺ���
	if (GetDongleScanCheckValue==TRUE)
	{
		CompareIMEI = FALSE;
		if (HandleNum <= 7)
		{
			if (DongleScanGunOnly(HandleNum) == FALSE)
				return false;
		}
		else if (HandleNum>7)
		{
			if (OPen_Serial_Port(m_Port, m_Baud, HandleNum) == TRUE)//����һ�δ��ڣ���������һֱ��
			{
				//�򿪴��ڳɹ�
				COM_State[HandleNum] = TRUE;
				LogShow_exchange(m_Result, Final_Result_Control, 0, "�򿪴��ڳɹ���\r\n", HandleNum);													//��һ����ʾ�������С�
			}
			else
			{
				//�򿪴���ʧ��
				COM_State[HandleNum] = FALSE;
				LogShow_exchange(m_Result, Final_Result_Control, 128, "�򿪴���ʧ�ܣ�\r\n", HandleNum);
				return false;
			}

			DongleInit(HandleNum);

			while (1)
			{
				BOOL DongleStatusCheck=FALSE;
				for (int i = 0; i < 8; i++)
				{
					DongleStatusCheck |= DongleStatusArray[i];
				}

				//�ж���ǰ�߸����ڵ�״̬��ֻҪ��һ������״̬��Ҫ��������ɨ���Ǿ�Ҫȥɨ�裬���ȫ�����ڶ�ΪFALSE���Ǿ����map
				if (DongleStatusCheck == TRUE)
				{
					DongleScanRssiFun(HandleNum, MachineType_CS);
				}
				else if (DongleStatusCheck == FALSE)
				{
					DongleRssiMap.clear();
				}
				Sleep(1000);
				if (StopSign[HandleNum] == TRUE)
				{
					return FALSE;
				}
			}
		}
	}

	AntiDupDataInit(HandleNum);//��ʼ���ֶη��ظ��ı���

    //��ʼ��һ�½���ʱҪ�رյļ̵��������ֵ
	for (int i = 0; i < 8; i++)
	{
		ExternalCircuit[HandleNum][i]=FALSE;
	}

	ParaItemName[HandleNum] = "";

	/*�����ǲ��ֹ�������ǰҪ��������---���������*/


	int ADCTWaitTimeout = 0, RDAWaitTimeOut = 0;;
	while (CheckConnect_Thread(m_Port, m_Baud, HandleNum, m_Result, Final_Result_Control) != TRUE)//
	{
		showtime = CTime::GetCurrentTime();

		CString showtimeCS;
		showtimeCS.Format("%02d:%02d:%02d", showtime.GetHour(), showtime.GetMinute(), showtime.GetSecond());

		if (StopSign[HandleNum] == TRUE)
		{
			if (GetRDAHostCheckValue == TRUE)
				RdaHostInterface.RDAComShutdown(HandleNum);
			return FALSE;
		}
		if (showtime.GetSecond() % 5>2)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "�ȴ�����", HandleNum);
			if (g_ADCTFlag == TRUE)
			{
				ADCTWaitTimeout++;
				if (ADCTWaitTimeout > g_WaitTimeoutFlag)
				{
					CString PortNum;
					switch (HandleNum)
					{
					case 0:
						GetDlgItemText(IDC_COMBO1, PortNum);
						break;
					case 1:
						GetDlgItemText(IDC_COMBO4, PortNum);
						break;
					case 2:
						GetDlgItemText(IDC_COMBO6, PortNum);
						break;
					case 3:
						GetDlgItemText(IDC_COMBO8, PortNum);
						break;
					case 4:
						GetDlgItemText(IDC_COMBO10, PortNum);
						break;
					case 5:
						GetDlgItemText(IDC_COMBO12, PortNum);
						break;
					case 6:
						GetDlgItemText(IDC_COMBO29, PortNum);
						break;
					case 7:
						GetDlgItemText(IDC_COMBO31, PortNum);
						break;
					case 8:
						GetDlgItemText(IDC_COMBO33, PortNum);
						break;
					case 9:
						GetDlgItemText(IDC_COMBO35, PortNum);
						break;
					case 10:
						GetDlgItemText(IDC_COMBO37, PortNum);
						break;
					case 11:
						GetDlgItemText(IDC_COMBO39, PortNum);
						break;
					case 12:
						GetDlgItemText(IDC_COMBO41, PortNum);
						break;
					case 13:
						GetDlgItemText(IDC_COMBO43, PortNum);
						break;
					case 14:
						GetDlgItemText(IDC_COMBO45, PortNum);
						break;
					case 15:
						GetDlgItemText(IDC_COMBO47, PortNum);
						break;
					default:
						break;
					}
					ADCTSetup(PortNum, 7, "ALL");
				}
				//else if (GetRDAHostCheckValue == TRUE)
				//{
				//	RDAWaitTimeOut++;
				//	if (RDAWaitTimeOut == 4)
				//	{
				//		RdaHostInterface.ComInit[HandleNum] = TRUE;
				//		RdaHostInterface.RDAComShutdown(HandleNum);
				//	}
				//}
			}
		}
		/*�����ǲ��ֹ�������ǰҪ��������---�����￪ʼ*/

		//���������������ɨ��ǹ
		if (GetDongleCheckValue == TRUE)
		{
			if (DongleScanGun(HandleNum) == FALSE)
				return false;
		}

		////���������������ɨ��ǹ�����ﲻӦ���ٿ���ɨ��ǹ��ֻ��ɨһ�εĻ��ᣩ
		//if (GetDongleScanCheckValue == TRUE)
		//{
		//	if (DongleScanGunOnly(HandleNum) == FALSE)
		//		return false;
		//}

		/*�����ǲ��ֹ�������ǰҪ��������---���������*/


		Sleep(1000);
	}


	if (GetDongleScanCheckValue == TRUE&&hScanGun[0] != NULL)
	{
		if (DongleTestDataDeq.size() != 0)
		{
			DongleTestDataDeq.pop_front();
		}
	}

	CTime TestTime = CTime::GetCurrentTime();
	TestTimeStart[HandleNum] = TestTime.GetHour() * 3600 + TestTime.GetMinute() * 60 + TestTime.GetSecond();

	CString DB_FAIL = "";

	CString ScanGunTpyeCS;
	ScanGunTpye.GetWindowTextA(ScanGunTpyeCS);


	CString Serial_Order_Return;//����

	int Port_OK_Try = 0;
	LogShow_exchange(m_Result, Final_Result_Control, 0, "���ڻ�ȡ�˿ڡ�����", HandleNum);
GETPort:
	for (int num = 0; num<8; num++)
	{
		if (OnGetport() == TRUE)
			break;
		Sleep(250);
	}
	BOOL Port_OK = FALSE;
	CString sPort;
	m_Port->GetWindowText(sPort);
	for (int num1 = 0; num1<PORTS_NUM; num1++)
	{
		if ((COM_Number[num1] == sPort) && (sPort != ""))
		{
			Port_OK = TRUE;
			break;
		}
	}
	if (Port_OK == FALSE)
	{
		Port_OK_Try++;
		if (Port_OK_Try<5)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ڻ�ȡ�˿�" + sPort + "...", HandleNum);
			Sleep(1000);
			goto GETPort;
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "��ǰ�˿�:" + sPort + "�����ڣ�����Ӳ������", HandleNum);
			//return FALSE;
			goto OVERDONE;
		}
	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 0, "�ɹ���ȡ�˿�" + sPort + "...", HandleNum);
	}
	if (Port_OK_Try>0)
		Sleep(1000);
	else
		Sleep(500);

	/////////////////////////////////////////////
	//ɨ��ǹ
	if (CompareIMEI == TRUE)
	{
		EnterCriticalSection(&UseScanResult);
		if (TestCheck == TRUE)//���ڽ�ͨ
		{
			//����״̬
			IMEI_Input[HandleNum] = PublicIMEI;
			LogShow_exchange(m_Result, Final_Result_Control, 0, "IMEI------------->OK��\r\n", HandleNum);
			SetTimer(30, 1000, NULL);
		}
		else
		{
			COM_State[HandleNum] = FALSE;
			LogShow_exchange(m_Result, Final_Result_Control, 128, "û��ɨ�赽IMEI������\r\n", HandleNum);
			LeaveCriticalSection(&UseScanResult);
			goto OVERDONE;//�򿪴���ʧ��
		}
		TestCheck = FALSE;
		LeaveCriticalSection(&UseScanResult);
	}
	//ɨ��ǹ
	/////////////////////////////////////////////
	if ((paraArray[1].High_Limit_Value == "ON") && (COM_State[HandleNum] == TRUE))//����һ�δ��ڣ���������һֱ��
	{
		LogShow_exchange(m_Result, Final_Result_Control, 5, "�����Ѿ���", HandleNum);
	}
	else
	{
			if (OPen_Serial_Port(m_Port, m_Baud, HandleNum,0,GetRDAHostCheckValue) == TRUE)//����һ�δ��ڣ���������һֱ��
			{
				//�򿪴��ڳɹ�
				COM_State[HandleNum] = TRUE;
				LogShow_exchange(m_Result, Final_Result_Control, 0, "�򿪴��ڳɹ���\r\n", HandleNum);												//��һ����ʾ�������С�
				Sleep(1);
				m_StartControlArray[HandleNum]->EnableWindow(FALSE);
			}
			else
			{
				//�򿪴���ʧ��
				COM_State[HandleNum] = FALSE;
				LogShow_exchange(m_Result, Final_Result_Control, 128, "�򿪴���ʧ�ܣ�\r\n", HandleNum);
				Sleep(1);
				m_StartControlArray[HandleNum]->EnableWindow(TRUE);
				//return FALSE;
				goto OVERDONE;//�򿪴���ʧ��
			}
	}
	/////////////////////////////////////////////////

	int Try_Time = 0;																												//���Դ���������жϣ�
	int All_Result[ITEMMAX] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    \
		, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int Try_Time_Real = atoi(paraArray[0].High_Limit_Value);
	for (int i = 1; i<ITEMMAX; i++)
	{
		if (StopSign[HandleNum] == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "��Ϊ��ֹ,����ֹͣ������", HandleNum, "���ܲ���:4-BORT");
			goto OVERDONE;//����ʧ�����йرմ���ѡ����
		}
		Try_Time_Real = atoi(paraArray[0].High_Limit_Value);

		Vaule_Return_Count_CS[HandleNum] = "";
		do
		{
			CString WaitCOMTimeCS;
			WaitCOMTimeCS = paraArray[0].Low_Limit_Value;//��ʱʱ��
			int posw = WaitCOMTimeCS.Find("(");
			if (posw >= 0)
			{
				WaitCOMTimeCS = WaitCOMTimeCS.Mid(posw + 1);

				posw = WaitCOMTimeCS.Find(")");
				if (posw >= 0)
					WaitCOMTimeCS = WaitCOMTimeCS.Left(posw);
			}
			else
				WaitCOMTimeCS = "-1";
			if (atoi(WaitCOMTimeCS)<0)
				WaitCOMTimeCS = "-1";

			CTime ScanTime = CTime::GetCurrentTime();
			int TimeDiff = (ScanTime.GetHour() * 3600 + ScanTime.GetMinute() * 60 + ScanTime.GetSecond() - TestTimeStart[HandleNum]) * 1;
			if ((WaitCOMTimeCS != "-1") && (TimeDiff>atoi(WaitCOMTimeCS)))//ɨ���ȴ�COM��ʱ
			{
				LogShow_exchange(m_Result, Final_Result_Control, 128, "���Գ�ʱ,����ֹͣ������", HandleNum, "���ܲ���:4-BORT");
				goto OVERDONE;//����ʧ�����йرմ���ѡ����
			}


			//////////////////////////////////////////
			if (StopSign[HandleNum] == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 128, "��Ϊ��ֹ,����ֹͣ������", HandleNum, "���ܲ���:4-BORT");
				goto OVERDONE;//����ʧ�����йرմ���ѡ����
			}
			if (paraArray[i].ifCheck != true)																						//û�й�ѡ
			{
				All_Result[i] = 1;
				break;																											//����do while
			}
			if ((paraArray[i].showName.Find("д���Ա�־") != -1) && (All_Result[0] != 1))
				break;																											//����do while//�����ȫ��OK����д���ܲ��Ա�־
			int tr = paraArray[i].showName.Find("TR");
			if (tr >= 0)
				Try_Time_Real = atoi(paraArray[i].showName.Left(2));
			if (Try_Time_Real<1)
				Try_Time_Real = atoi(paraArray[0].High_Limit_Value);

			//TR��ָ������ָ��ѭ�����Լ��Σ�CR�����ڿ��ư�ģ�����tr��ʱ��Ͳ��ܴ���CR
			if (tr == -1)
			{
				int cr = paraArray[i].showName.Find("CR");
				if (cr >= 0)
					Try_Time_Real = atoi(paraArray[i].showName.Left(2));
				if (Try_Time_Real < 1)
					Try_Time_Real = atoi(paraArray[0].High_Limit_Value);
			}

			CString WaitTimeCS;
			int wpos = paraArray[i].showName.Find("DL");
			if (wpos != -1)
				WaitTimeCS = paraArray[i].showName.Mid(wpos + 2);
			else
				WaitTimeCS = "0";

			ParaItemName[HandleNum] = paraArray[i].showName;

			if ((paraArray[i].showName.Find("��ֽ") != -1)||(paraArray[i].showName.Find("����") != -1) || (paraArray[i].showName.Find("��������") != -1) || (paraArray[i].showName.Find("��") != -1) || (paraArray[i].showName.Find("ȷ�ϱ�ʶ") != -1) || (paraArray[i].showName.Find("���ư�״̬") != -1))//
			{
				Final_Result_Control->SetFont(fontsp);
				gColor[HandleNum] = RGB(205, 0, 205);
				Final_Result_Control->SetWindowTextA(paraArray[i].showName);
				Final_Result_Control->UpdateWindow();
				//����һ������ʾ��Ϣ
				if (g_ADCTFlag == TRUE)
				{
					if (paraArray[i].showName.Find("����") != -1)
					{
						SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "����", 0);
					}
					else if (paraArray[i].showName.Find("��������") != -1)
					{
						SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "��������", 0);
					}
					else if (paraArray[i].showName.Find("��") != -1)
					{
						SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "��", 0);
					}
					else if (paraArray[i].showName.Find("ȷ�ϱ�ʶ") != -1)
					{
						SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "ȷ�ϱ�ʶ", 0);
					}
					else if (paraArray[i].showName.Find("���ư�״̬") != -1)
					{
						SendADCTHint(get_map_key_value(PortStatusMap, HandleNum), "���ư�״̬", 0);
					}
				}
				if (paraArray[i].showName.Find("��������") != -1)
					Voice_Speak("�δ�");
				if (paraArray[i].showName.Find("���ư�״̬") != -1)
				{
					TestResult[HandleNum] = TRUE;
					CString VoiceM;
					VoiceM.Format("��ȷ��%d", HandleNum);
					Final_Result_Control->SetWindowTextA(VoiceM);
					Final_Result_Control->UpdateWindow();
					Voice_Speak(VoiceM);
				}
			}
			else
			{
				Final_Result_Control->SetFont(font);
				LogShow_exchange(m_Result, Final_Result_Control, 0, "������...", HandleNum);
			}

			if (((paraArray[i].showName.Find("��������") != -1) || (paraArray[i].showName.Find("�ط�����") != -1) || (paraArray[i].showName.Find("����") != -1) || (paraArray[i].showName.Find("��") != -1)\
				|| (paraArray[i].showName.Find("���ư�") != -1) || (paraArray[i].showName.Find("ģ�����") != -1) || (paraArray[i].showName.Find("ͨ·") != -1) || (paraArray[i].showName.Find("������") != -1)\
				|| (paraArray[i].showName.Find("���ư�״̬") != -1) || (paraArray[i].showName.Find("���") != -1)) && (paraArray[i].showName.Find("Socket�����ж�") == -1))
			{
				BOOL Result = CPUCommunication(paraArray[i].showName, HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
				if (paraArray[i].showName.Find("���ư�״̬") != -1)//��ѯ���ư����״̬��ʱ��Ҫ���ѭ��
				{
					if (Result == FALSE)
					{
						Try_Time++;
						LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "��Ч����...", HandleNum);	//���ư������ȡ״̬
						if ((Try_Time<Try_Time_Real) && (((GetPicChoose == TRUE) && (PicUnionOK[HandleNum] == TRUE)) || (BGConfirmChoose == TRUE)))//�������ƣ�ͬʱ����ɼ���WIFI�ź������������̨ȷ�����Եȣ�
						{
							if (BGConfirmChoose == TRUE)//��̨ȷ��Ҳ���ܵ�̫��
							{
								if ((Try_Time>5) && ((GetPicChoose == TRUE) && (PicUnionOK[HandleNum] != TRUE)))
								{
									LogShow_exchange(m_Result, Final_Result_Control, 128, "��ȡͼƬʧ��,����ֹͣ������", HandleNum, "���ܲ���:2-TEST");
									goto OVERDONE;
								}
							}
							Sleep(atoi(WaitTimeCS));
							continue;//ѭ��do while
						}
						
						else
						{
							LogShow_exchange(m_Result, Final_Result_Control, 128, "���ư�״̬�仯��ʱ,����ֹͣ������", HandleNum, "���ܲ���:1-CPU");
							goto OVERDONE;
						}
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 5, "��ʼ��ʾͼƬ������", HandleNum);//����������ͼƬ�������쳣��������ʧ��
						//TestResult[HandleNum]=TRUE;//���Գɹ�,ͼƬ�߳̿��ܻ�Ȳ����߳̿죬����Ҫ����ǰ��
						DoorWait[HandleNum].SetEvent();
					}
				}
				else if (Result == FALSE)
				{
					if (paraArray[i].showName.Find("CR") != -1)
					{
						Try_Time++;
						LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "��Ч����...", HandleNum);	//���ư������ȡ״̬
						if (Try_Time < Try_Time_Real)
						{
							Sleep(atoi(WaitTimeCS));
							continue;//ѭ��do while
						}
					}

					LogShow_exchange(m_Result, Final_Result_Control, 128, "����ư�ͨ���쳣,����ֹͣ������", HandleNum, "���ܲ���:1-CPU");
					goto OVERDONE;//����ʧ�����йرմ���ѡ����
				}
				else
				{
					if (COM_State[THREAD_NUM] == TRUE)
						LogShow_exchange(m_Result, Final_Result_Control, 5, "����ư�ͨ������:" + paraArray[i].showName, HandleNum);
				}
			}
			else if ((paraArray[i].showName.Find("WIFI����") != -1))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 5, "�ȴ���һ��ȷ��...", HandleNum);
				EnterCriticalSection(&WIFICOMMUNICATE[HandleNum]);
				LogShow_exchange(m_Result, Final_Result_Control, 5, "��һ��ȷ�Ͻ���", HandleNum);
				LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
			}
			else if ((paraArray[i].showName.Find("Socket����ͼƬ") != -1))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 5, "�ȴ���һ��ȷ��...", HandleNum);
				EnterCriticalSection(&WIFICOMMUNICATE[HandleNum]);
				LogShow_exchange(m_Result, Final_Result_Control, 5, "��һ��ȷ�Ͻ���", HandleNum);
				LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);

				//SocketͼƬ�����￪ʼ׼������
				CString PicName;

				if (paraArray[i].showName.Find("ҹ��") != -1)
				{
					PicName.Format(_T("PicSrc\\%s\\Port%d\\%s-ҹ��-"+ChipRfID[HandleNum]+".jpg"), GetDatetime(), HandleNum + 1, GetCurrenttime());
					SocketServerPicName[0][HandleNum] = PicName;
					SocketServer[HandleNum]->OpenRecv(PicName);
				}
				else if (paraArray[i].showName.Find("����") != -1)
				{
					PicName.Format(_T("PicSrc\\%s\\Port%d\\%s-����-" + ChipRfID[HandleNum] + ".jpg"), GetDatetime(), HandleNum + 1, GetCurrenttime());
					SocketServerPicName[1][HandleNum] = PicName;
					SocketServer[HandleNum]->OpenRecv(PicName);
				}
				else if (paraArray[i].showName.Find("����") != -1)
				{
					PicName.Format(_T("PicSrc\\%s\\Port%d\\%s-����-" + ChipRfID[HandleNum] + ".jpg"), GetDatetime(), HandleNum + 1, GetCurrenttime());
					SocketServerPicName[2][HandleNum] = PicName;
					SocketServer[HandleNum]->OpenRecv(PicName);
				}
				CreateDirectoryRecursionFun(PicName);//�Զ������ļ���·���������ںʹ�������ͼƬ���й�������
			}
			else if ((paraArray[i].showName.Find("Socket�����ж�") != -1))
			{
				SocketServer[HandleNum]->OpenSimpleRecv();
				int count=0;
				int WaitCount = atoi(WaitTimeCS)/300;
				if (WaitTimeCS == "0")
				{
					WaitCount = 10;
				}
				
				while (1)
				{
					Sleep(1000);
					if (SocketServer[HandleNum]->m_SimpleData.Find(paraArray[i].Low_Limit_Value) != -1)
					{
						SocketServer[HandleNum]->CloseSimpleRecv();
						LogShow_exchange(m_Result, Final_Result_Control, 5, "ȷ�ϳɹ�", HandleNum);			//�������ԣ�״̬������
						break;
					}



					//Ĭ�ϳ���8��͵�����ʱ����
					count++;
					if (count >= WaitCount)
					{
						SocketServer[HandleNum]->CloseSimpleRecv();
						LogShow_exchange(m_Result, Final_Result_Control, 128, "�����жϳ�ʱ", HandleNum, "���ܲ���:2-TEST");
						goto OVERDONE;
					}
				}
			}
			else if ((paraArray[i].showName.Find("Socket�ж�ͼƬ") != -1))
			{
				BOOL Result;
				int count = 0;
				CEdit* m_ResultPic;
				CEdit* Final_Result_ControlPic;

				if (GetPicChoose == FALSE)//û�й�ѡ��ȡͼƬ
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "û�й�ѡ��ѡ<T9-T16��ȡͼƬ>", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}

				switch (HandleNum)
				{
				case 0:
					m_ResultPic = &m_Result9;
					Final_Result_ControlPic = &Final_Result_Control9;
					break;
				case 1:
					m_ResultPic = &m_Result10;
					Final_Result_ControlPic = &Final_Result_Control10;
					break;
				case 2:
					m_ResultPic = &m_Result11;
					Final_Result_ControlPic = &Final_Result_Control11;
					break;
				case 3:
					m_ResultPic = &m_Result12;
					Final_Result_ControlPic = &Final_Result_Control12;
					break;
				case 4:
					m_ResultPic = &m_Result13;
					Final_Result_ControlPic = &Final_Result_Control13;
					break;
				case 5:
					m_ResultPic = &m_Result14;
					Final_Result_ControlPic = &Final_Result_Control14;
					break;
				case 6:
					m_ResultPic = &m_Result15;
					Final_Result_ControlPic = &Final_Result_Control15;
					break;
				case 7:
					m_ResultPic = &m_Result16;
					Final_Result_ControlPic = &Final_Result_Control16;
					break;
				default:
					break;
				}

				CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_ResultPic, Final_Result_ControlPic);//��������ư��л��������
				CPUCommunication("ȷ�ϳɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_ResultPic, Final_Result_ControlPic);//��������ư��л��������
				EnterCriticalSection(&WIFICOMMUNICATE[HandleNum]);
				
				//���̷߳�ʽ����ͼƬ

				if (PathFileExists(PicPath[0][HandleNum]) != TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "ҹ��ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}
				if (PathFileExists(PicPath[1][HandleNum]) != TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "����ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}

				Picdlg[HandleNum].ShowPicTime = atoi(BGShowPicTime[HandleNum]);
				Picdlg[HandleNum].PicPath0 = PicPath[0][HandleNum];
				Picdlg[HandleNum].PicPath1 = PicPath[1][HandleNum];
				Picdlg[HandleNum].ShowHandleHun = HandleNum;
				TimerHandleNum = HandleNum;
				SetTimer(48, 1, NULL);

				//�����WIFI����Ҫ��ǰ����socketͨ��
				if ((paraArray[i].showName.Find("WIFI") != -1))
				{
					SocketServer[HandleNum]->OpenSimpleRecv();
				}

				Final_Result_Control->SetFont(fontsp);
				gColor[HandleNum] = RGB(205, 0, 205);
				Final_Result_Control->SetWindowTextA("��ȷ��");
				Final_Result_Control->UpdateWindow();

				//whileѭ�����ִ����ֱ��Ƕ����ư���Ϣ��WIFI��RESET��Ϣ�ʹ��ڵİ�����Ϣ�����Ƕ������ư��ʧ�ܰ�ť�ͻ�goto overdone
				while (1)
				{
					if ((paraArray[i].showName.Find("����") != -1))
					{
						if (CPUCommunication("ȷ�ϳɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_ResultPic, Final_Result_ControlPic) == TRUE)
						{
							LogShow_exchange(m_Result, Final_Result_Control, 5, "ȷ�ϳɹ�", HandleNum);			//�������ԣ�״̬������
							break;
						}
					}
					else if ((paraArray[i].showName.Find("WIFI") != -1))
					{
						if (SocketServer[HandleNum]->m_SimpleData.Find("RESET") != -1)
						{
							SocketServer[HandleNum]->CloseSimpleRecv();
							LogShow_exchange(m_Result, Final_Result_Control, 5, "ȷ�ϳɹ�", HandleNum);			//�������ԣ�״̬������
							break;
						}
					}
					else if ((paraArray[i].showName.Find("����") != -1))
					{
						Send_Serial_Order(&Serial_Order_Return, "", HandleNum, (LPSTR)(LPCTSTR)paraArray[i].Low_Limit_Value, (LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, atoi(WaitTimeCS));//ָ���Ϊ�վͱ�ʾֻ�Ƕ�ȡֵ������Ҫ����ָ��

						if (Vaule_Return_Count_CS[HandleNum].Find(paraArray[i].Other_ITEM) != -1)//��Ϊ��ȷ��ÿ�����͵İ����Ļظ�ֵ�Ƿ�һ�£�����Ҫ�������Ļظ�ֵ����Ϊ����ָ���ָ���ֵ����Ϊ�գ���ֹ���������жϷ���ֵʱ�޷�������ȷֵ��ע�⣬���ͼƬ�ж�����û��ʧ�ܵĻ���������һ���ǳɹ��ģ����Է���ֵҪ����Ϊ�գ���������������жϣ�
						{
							LogShow_exchange(m_Result, Final_Result_Control, 5, "ȷ�ϳɹ�", HandleNum);			//�������ԣ�״̬������
							break;
						}
					}

					Sleep(2000);

					Result = CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_ResultPic, Final_Result_ControlPic);
					if (Result == TRUE)
					{
						if ((paraArray[i].showName.Find("WIFI") != -1))
						{
							SocketServer[HandleNum]->OpenSimpleRecv();
						}
						Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);
						LogShow_exchange(m_Result, Final_Result_Control, 128, "ȷ��ʧ��", HandleNum, "���ܲ���:2-TEST");
						LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);//�˹��ж���ͼƬ���뿪�ٽ������������˿ڵĲ��Լ���
						goto OVERDONE;
					}

					//Ĭ�ϳ���8��͵�����ʱ����
					count++;
					if (count >= 4)
					{
						if ((paraArray[i].showName.Find("WIFI") != -1))
						{
							SocketServer[HandleNum]->OpenSimpleRecv();
						}
						Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);
						LogShow_exchange(m_Result, Final_Result_Control, 128, "ͼƬ�жϳ�ʱ", HandleNum, "���ܲ���:2-TEST");
						LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);//�˹��ж���ͼƬ���뿪�ٽ������������˿ڵĲ��Լ���
						goto OVERDONE;
					}
				}
				Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);
				LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);//�˹��ж���ͼƬ���뿪�ٽ������������˿ڵĲ��Լ���
			}
			else if ((paraArray[i].showName.Find("��ѹ��������") != -1))//��ѹ����
			{
				BOOL Flag = TRUE, Flag2 = TRUE;

				char *p1 = PowerControlInstrNameArray[0].GetBuffer();
				char *p2 = BackUpPowerInstrName.GetBuffer();

				SetVoltageAndRangeVaule(paraArray[i].Low_Limit_Value,m_Voltage,m_Range);

				double m_HighVoltage, m_Delay;
				SetHighVoltageAndDelayVaule(paraArray[i].High_Limit_Value, m_HighVoltage,m_Delay);

				if (RelayFlag == FALSE)
				{
					if (HandleNum > 0)
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��ʹ�ü̵����������ֻ�ܲ��Զ˿�1", HandleNum);
						goto OVERDONE;
					}

					Flag = CurrentTestArray[0]->PowerOn(p1, m_Voltage);

					if (Flag == FALSE)
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�̿ص�Դ����ʧ�ܣ�����̿ص�Դ", HandleNum);
						goto OVERDONE;

					}
					else if (Flag == TRUE)
					{
						CurrentTestArray[0]->PowerOn(p1, m_HighVoltage);
						Sleep(m_Delay);
						CurrentTestArray[0]->PowerOn(p1, m_Voltage);
					}
				}
				else if (RelayFlag == TRUE)
				{

					if (HandleNum >= 0 && HandleNum <= 3)
					{
						Flag = CurrentTestArray[0]->PowerOn(p1, m_Voltage);
						if (BackuppowerFlag == TRUE)
						{
							Flag2 = CurrentTestArray[0]->PowerOn(p2, m_Voltage);
						}

						if (Flag == FALSE || Flag2 == FALSE)
						{
							LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�������õ�Դ��ʼ��ʧ�ܣ�����̿ص�Դ�ͱ��õ�Դ", HandleNum);
							goto OVERDONE;
						}
						else if (Flag == TRUE)
						{

							Current1Deq.push_back(HandleNum);

							while (1)
							{
								if (Current1Deq.front() == HandleNum)
								{
									Flag = CurrentTestArray[0]->PowerOn(p2, atof(paraArray[i].High_Limit_Value));
									Sleep(3000);
									Flag2 = CurrentTestArray[0]->PowerOn(p2, m_Voltage);

									Current1Deq.pop_front();
									if (Flag==TRUE&&Flag2 == TRUE)
										break;
									else if (Flag == FALSE||Flag2 == FALSE)
									{
										LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��ѹ��������ʧ�ܣ���������", HandleNum);
										goto OVERDONE;
									}
								}
								Sleep(2000);
								if (StopSign[HandleNum] == TRUE)
								{
									deque<int>::iterator pos;
									pos = find(Current1Deq.begin(), Current1Deq.end(), HandleNum);
									if (pos != Current1Deq.end())
										Current1Deq.erase(pos);
									return FALSE;
								}

							}
						}
					}
					else if (HandleNum >= 4 && HandleNum <= 7)
					{
						Flag = CurrentTestArray[1]->PowerOn(p1, m_Voltage);

						if (BackuppowerFlag == TRUE)
						{
							Flag2 = CurrentTestArray[1]->PowerOn(p2, m_Voltage);
						}

						if (Flag == FALSE || Flag2 == FALSE)
						{
							LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�������õ�Դ��ʼ��ʧ�ܣ�����̿ص�Դ", HandleNum);
							goto OVERDONE;
						}
						else if (Flag == TRUE)
						{

							Current2Deq.push_back(HandleNum);

							while (1)
							{
								if (Current2Deq.front() == HandleNum)
								{
									Flag = CurrentTestArray[1]->PowerOn(p2, atof(paraArray[i].High_Limit_Value));
									Sleep(atoi(paraArray[i].Low_Limit_Value));
									Flag2 = CurrentTestArray[1]->PowerOn(p2, m_Voltage);

									Current2Deq.pop_front();
									if (Flag == TRUE&&Flag2 == TRUE)
										break;
									else if (Flag == FALSE || Flag2 == FALSE)
									{
										LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��ѹ��������ʧ�ܣ���������", HandleNum);
										goto OVERDONE;
									}
								}
								Sleep(2000);
								if (StopSign[HandleNum] == TRUE)
								{
									deque<int>::iterator pos;
									pos = find(Current2Deq.begin(), Current2Deq.end(), HandleNum);
									if (pos != Current2Deq.end())
										Current2Deq.erase(pos);
									return FALSE;
								}
							}
						}
					}
					else
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "����һ��Ӧ�˿�1-4������2��Ӧ�˿�5-8������ݴ˶�Ӧ��ϵ������", HandleNum);
						goto OVERDONE;
					}
				}
			}
			else if ((paraArray[i].showName.Find("��������") != -1))//�������ܲ��Ե���
			{
				BOOL Flag = TRUE, Flag2 = TRUE;
				PortCurrentVauleArray[HandleNum] = 0;
				SetVoltageAndRangeVaule(paraArray[i].Low_Limit_Value, m_Voltage, m_Range);
				char *p1 = PowerControlInstrNameArray[0].GetBuffer(), *p2 = PowerControlRelayInstrName.GetBuffer();

				if (RelayFlag == FALSE)
				{
					if (HandleNum > 0)
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��ʹ�ü̵����������ֻ�ܲ�˿�1���豸����", HandleNum);
						goto OVERDONE;
					}

					Flag = CurrentTestArray[0]->PowerOn(p1, m_Voltage);
					if (Flag == FALSE)
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��ʼ��ʧ�ܣ�����̿ص�Դ", HandleNum);
						goto OVERDONE;

					}
					else if (Flag == TRUE)
					{
						Current1Deq.push_back(HandleNum);

						while (1)
						{
							if (Current1Deq.front() == HandleNum)
							{
								Flag2=CurrentMainControlFun(HandleNum);
								if (Flag2==TRUE)
								break;
								else if (Flag2 == FALSE)
								{
									LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��������ʧ�ܣ���������", HandleNum);
									goto OVERDONE;
								}
							}
							Sleep(2000);
							if (StopSign[HandleNum] == TRUE)
							{
								deque<int>::iterator pos;
								pos = find(Current1Deq.begin(), Current1Deq.end(), HandleNum);
								if (pos != Current1Deq.end())
									Current1Deq.erase(pos);
								return FALSE;
							}
						}
					}
				}
				else if (RelayFlag == TRUE)
				{
					if (HandleNum >= 0 && HandleNum <= 3)
					{
						Flag = CurrentTestArray[0]->PowerOn(p1, m_Voltage);
						if (BackuppowerFlag == TRUE)
						{
							Flag2 = CurrentTestArray[0]->PowerOn(p2, m_Voltage);
						}

						if (Flag == FALSE || Flag2==FALSE)
						{
							LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�������õ�Դ��ʼ��ʧ�ܣ�����̿ص�Դ", HandleNum);
							goto OVERDONE;
						}
						else if (Flag == TRUE)
						{

							Current1Deq.push_back(HandleNum);

							while (1)
							{
								if (Current1Deq.front() == HandleNum)
								{
									Flag2 = CurrentMainControlFun(HandleNum);
									if (Flag2 == TRUE)
										break;
									else if (Flag2 == FALSE)
									{
										LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��������ʧ�ܣ���������", HandleNum);
										goto OVERDONE;
									}
								}
								Sleep(2000);
								if (StopSign[HandleNum] == TRUE)
								{
									deque<int>::iterator pos;
									pos = find(Current1Deq.begin(), Current1Deq.end(), HandleNum);
									if (pos != Current1Deq.end())
										Current1Deq.erase(pos);
									return FALSE;
								}

							}
						}
					}
					else if (HandleNum >= 4 && HandleNum <= 7)
					{
						Flag = CurrentTestArray[1]->PowerOn(p1, m_Voltage);
						if (BackuppowerFlag == TRUE)
						{
							Flag2 = CurrentTestArray[1]->PowerOn(p2, m_Voltage);
						}

						if (Flag == FALSE || Flag2 == FALSE)
						{
							LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "�������õ�Դ��ʼ��ʧ�ܣ�����̿ص�Դ", HandleNum);
							goto OVERDONE;
						}
						else if (Flag == TRUE)
						{

							Current2Deq.push_back(HandleNum);

							while (1)
							{
								if (Current2Deq.front() == HandleNum)
								{
									Flag2 = CurrentMainControlFun(HandleNum);
									if (Flag2 == TRUE)
										break;
									else if (Flag2 == FALSE)
									{
										LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��������ʧ�ܣ���������", HandleNum);
										goto OVERDONE;
									}
								}
								Sleep(2000);
								if (StopSign[HandleNum] == TRUE)
								{
									deque<int>::iterator pos;
									pos = find(Current2Deq.begin(), Current2Deq.end(), HandleNum);
									if (pos != Current2Deq.end())
										Current2Deq.erase(pos);
									return FALSE;
								}
							}
						}
					}
					else
					{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "����һ��Ӧ�˿�1-4������2��Ӧ�˿�5-8������ݴ˶�Ӧ��ϵ������", HandleNum);
						goto OVERDONE;
					}
				}
			}
			else if ((paraArray[i].showName.Find("WIFI����ͼƬ") != -1))
			{
				if (GetPicChoose == FALSE)//û�й�ѡ��ȡͼƬ
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "û�й�ѡ��ѡ<T9-T16��ȡͼƬ>", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}
				CEdit* m_ResultPic;
				CEdit* Final_Result_ControlPic;

				switch (HandleNum)
				{
				case 0:
					m_ResultPic = &m_Result9;
					Final_Result_ControlPic = &Final_Result_Control9;
					break;
				case 1:
					m_ResultPic = &m_Result10;
					Final_Result_ControlPic = &Final_Result_Control10;
					break;
				case 2:
					m_ResultPic = &m_Result11;
					Final_Result_ControlPic = &Final_Result_Control11;
					break;
				case 3:
					m_ResultPic = &m_Result12;
					Final_Result_ControlPic = &Final_Result_Control12;
					break;
				case 4:
					m_ResultPic = &m_Result13;
					Final_Result_ControlPic = &Final_Result_Control13;
					break;
				case 5:
					m_ResultPic = &m_Result14;
					Final_Result_ControlPic = &Final_Result_Control14;
					break;
				case 6:
					m_ResultPic = &m_Result15;
					Final_Result_ControlPic = &Final_Result_Control15;
					break;
				case 7:
					m_ResultPic = &m_Result16;
					Final_Result_ControlPic = &Final_Result_Control16;
					break;
				default:
					break;
				}


			/*	if (HandleNum == 0)
				{
					m_ResultPic = &m_Result9;
					Final_Result_ControlPic = &Final_Result_Control9;
				}
				else if (HandleNum == 1)
				{
					m_ResultPic = &m_Result10;
					Final_Result_ControlPic = &Final_Result_Control10;
				}
				else if (HandleNum == 2)
				{
					m_ResultPic = &m_Result11;
					Final_Result_ControlPic = &Final_Result_Control11;
				}
				else if (HandleNum == 3)
				{
					m_ResultPic = &m_Result12;
					Final_Result_ControlPic = &Final_Result_Control12;
				}
				else if (HandleNum == 4)
				{
					m_ResultPic = &m_Result13;
					Final_Result_ControlPic = &Final_Result_Control13;
				}
				else if (HandleNum == 5)
				{
					m_ResultPic = &m_Result14;
					Final_Result_ControlPic = &Final_Result_Control14;
				}
				else if (HandleNum == 6)
				{
					m_ResultPic = &m_Result15;
					Final_Result_ControlPic = &Final_Result_Control15;
				}
				else if (HandleNum == 7)
				{
					m_ResultPic = &m_Result16;
					Final_Result_ControlPic = &Final_Result_Control16;
				}*/

				StartGetPic(HandleNum, m_ResultPic, Final_Result_ControlPic);
			}
			else if ((paraArray[i].showName.Find("���ڻ�ȡͼƬ") != -1))
			{
				if (GetPicChoose == FALSE)//û�й�ѡ��ȡͼƬ
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "û�й�ѡ��ѡ<T9-T16��ȡͼƬ>", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}
				CEdit* m_ResultPic;
				CEdit* Final_Result_ControlPic;

				switch (HandleNum)
				{
				case 0:
					m_ResultPic = &m_Result9;
					Final_Result_ControlPic = &Final_Result_Control9;
					break;
				case 1:
					m_ResultPic = &m_Result10;
					Final_Result_ControlPic = &Final_Result_Control10;
					break;
				case 2:
					m_ResultPic = &m_Result11;
					Final_Result_ControlPic = &Final_Result_Control11;
					break;
				case 3:
					m_ResultPic = &m_Result12;
					Final_Result_ControlPic = &Final_Result_Control12;
					break;
				case 4:
					m_ResultPic = &m_Result13;
					Final_Result_ControlPic = &Final_Result_Control13;
					break;
				case 5:
					m_ResultPic = &m_Result14;
					Final_Result_ControlPic = &Final_Result_Control14;
					break;
				case 6:
					m_ResultPic = &m_Result15;
					Final_Result_ControlPic = &Final_Result_Control15;
					break;
				case 7:
					m_ResultPic = &m_Result16;
					Final_Result_ControlPic = &Final_Result_Control16;
					break;
				default:
					break;
				}

				DayTimeState[HandleNum] = TRUE;
				hPicPort[HandleNum] = hPort[HandleNum];//���ھ����ֵ

				BOOL bWriteStat;
				DWORD dwBytesWritten;
				char *buffer;
				CString command = paraArray[i].Other_ITEM + "\r\n";//ƴ������
				buffer = command.GetBuffer(command.GetLength());
				bWriteStat = WriteFile(hPort[HandleNum], buffer, command.GetLength(), &dwBytesWritten, NULL);//
				if (dwBytesWritten != command.GetLength())
				{
					Sleep(300);
					continue;
				}
				//receive response
				Sleep(atoi(WaitTimeCS));

				SerialPicOver[HandleNum] = FALSE;
				SerialPic[HandleNum].ResetEvent();
				DoorWait[HandleNum].ResetEvent();
				StartGetPic(HandleNum, m_ResultPic, Final_Result_ControlPic);
				WaitForSingleObject(SerialPic[HandleNum].m_hObject, 60 * 1000);//���ȴ�60��INFINITE
				if (SerialPicOver[HandleNum] == TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "��꣡========������Գɹ�", HandleNum);			//�������ԣ�״̬������
					All_Result[i] = 1;//����do while 
					break;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "������Ŀ:" + paraArray[i].showName + "ʧ��,����ֹͣ������", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;//����ʧ�����йرմ���ѡ����
				}
			}
			else if ((paraArray[i].showName.Find("WIFIȷ�ϱ�ʶ") != -1))
			{
				if (GetPicChoose == FALSE)//û�й�ѡ��ȡͼƬ
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "û�й�ѡ��ѡ<T9-T16��ȡͼƬ>", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}

				if (BGConfirmChoose == TRUE)//��̨ȷ�ϣ��򲻵���ȷ�Ͻ���
				{
					All_Result[i] = 1;
					BGShowPicTime[HandleNum] = WaitTimeCS;
					BGShowPicCount[HandleNum] = Try_Time_Real;
					break; //����do while 
				}

				CEdit* m_ResultPic;
				CEdit* Final_Result_ControlPic;

				switch (HandleNum)
				{
				case 0:
					m_ResultPic = &m_Result9;
					Final_Result_ControlPic = &Final_Result_Control9;
					break;
				case 1:
					m_ResultPic = &m_Result10;
					Final_Result_ControlPic = &Final_Result_Control10;
					break;
				case 2:
					m_ResultPic = &m_Result11;
					Final_Result_ControlPic = &Final_Result_Control11;
					break;
				case 3:
					m_ResultPic = &m_Result12;
					Final_Result_ControlPic = &Final_Result_Control12;
					break;
				case 4:
					m_ResultPic = &m_Result13;
					Final_Result_ControlPic = &Final_Result_Control13;
					break;
				case 5:
					m_ResultPic = &m_Result14;
					Final_Result_ControlPic = &Final_Result_Control14;
					break;
				case 6:
					m_ResultPic = &m_Result15;
					Final_Result_ControlPic = &Final_Result_Control15;
					break;
				case 7:
					m_ResultPic = &m_Result16;
					Final_Result_ControlPic = &Final_Result_Control16;
					break;
				default:
					break;
				}

				////////////////////////////////////////////////
				//ʧ�ܰ�ť
				BOOL Result = CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
				if (Result == TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "ȷ��ʧ��", HandleNum, "���ܲ���:2-TEST");
					goto OVERDONE;
				}
				//ʧ�ܰ�ť
				////////////////////////////////////////////////


				if (paraArray[i].showName.Find("ȷ�ϱ�ʶ") != -1)
				{
					Final_Result_Control->SetFont(fontsp);
					gColor[HandleNum] = RGB(205, 0, 205);
					Final_Result_Control->SetWindowTextA(paraArray[i].showName);	
					Final_Result_Control->UpdateWindow();
				}
				if (Get_Wifi_Data(m_ResultPic, Final_Result_ControlPic, HandleNum, "", "RESET") != TRUE)
				{
					Picdlg[HandleNum].ShowPicTime = atoi(WaitTimeCS);
					/*if(UnionPicPath[HandleNum]=="")
					{
					LogShow_exchange(m_Result,Final_Result_Control,128,"������Ŀ:"+paraArray[i].showName+"ʧ��,����ֹͣ������",HandleNum,"���ܲ���:2-TEST");
					goto OVERDONE;
					}
					Picdlg[HandleNum].UnionPicPath=UnionPicPath[HandleNum];*/
					Picdlg[HandleNum].PicPath0 = PicPath[0][HandleNum];
					Picdlg[HandleNum].PicPath1 = PicPath[1][HandleNum];
					Picdlg[HandleNum].DoModal();

					Try_Time++;
					LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "��Ч����...", HandleNum);	//���ư������ȡ״̬
					if ((Try_Time<Try_Time_Real) && ((GetPicChoose == TRUE) && (PicUnionOK[HandleNum] == TRUE)))//�������ƣ�ͬʱ����ɼ���WIFI�ź�����
					{

						continue;//ѭ��do while
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, "������Ŀ:" + paraArray[i].showName + "ʧ��,����ֹͣ������", HandleNum, "���ܲ���:2-TEST");
						goto OVERDONE;
					}
				}
				else
				{
					ResetButtonWIFI[HandleNum] = TRUE;
				}

			}
			/*if((paraArray[i].showName.Find("����")!=-1)||(paraArray[i].showName.Find("��������")!=-1)||(paraArray[i].showName.Find("��")!=-1)||(paraArray[i].showName.Find("ȷ�ϱ�ʶ")!=-1)||(paraArray[i].showName.Find("���ư�״̬")!=-1))//
			{
			Final_Result_Control->SetFont(fontsp);
			gColor[HandleNum] = RGB(205,0,205);
			Final_Result_Control->SetWindowTextA(paraArray[i].showName);
			Final_Result_Control->UpdateWindow();
			if(paraArray[i].showName.Find("��������")!=-1)
			Voice_Speak("�δ�");
			if(paraArray[i].showName.Find("���ư�״̬")!=-1)
			{
			CString VoiceM;
			VoiceM.Format("��ȷ��%d",HandleNum);
			Voice_Speak(VoiceM);
			}
			}
			else
			{
			Final_Result_Control->SetFont(font);
			LogShow_exchange(m_Result,Final_Result_Control,0,"������...",HandleNum);
			}*/

			CString PortOrder;
			PortOrder = paraArray[i].Other_ITEM;

			/*CString WaitTimeCS;
			int wpos=paraArray[i].showName.Find("DL");
			if(wpos!=-1)
			WaitTimeCS=paraArray[i].showName.Mid(wpos+2);
			else
			WaitTimeCS="0";*/

			if (paraArray[i].showName.Find("���ɰ汾") != -1)//оƬID
				PortOrder += ShowPassWord(ChipRfID[HandleNum], m_Result, Final_Result_Control, HandleNum);
			else if (paraArray[i].showName.Find("XX") != -1)
			{
				CString NewData;
				NewData.Format("%s%d", Ipaddress.Right(1), HandleNum);
				PortOrder.Replace("XX", NewData);
			}
			else if (paraArray[i].showName.Find("IP�˿�") != -1)//DC-09��������ͷ��IP�˿�
			{
				CString NewData;
				NewData.Format("%s,%d", Ipaddress.Right(Ipaddress.GetLength()-4),SocketServerPort[HandleNum]);
				PortOrder.Replace("IP�˿�", NewData);
			}
			else if (paraArray[i].Other_ITEM.Find("�ֶ���ʾ") != -1)//ɨ��ǹɨ������ʾ���ֶ�ȥ���ָ��
			{
				CString NewData;
				m_ShowNumberPortControlArray[HandleNum]->GetWindowTextA(NewData);
				PortOrder.Replace("�ֶ���ʾ", NewData);
			}

			CString Serial_Order_Return_CS_Show;

			//��ΪDongle�����������ɷ�Ϊʮ������ָ��ͺ�ATָ��ͣ�����ָ���Ҫ���������������������MAC��ַ��ΪоƬIDʱֻ��ͨ����������ATָ���ȡMAC��ַ
			if (GetDongleCheckValue == FALSE)
			{
				Send_Serial_Order(&Serial_Order_Return, PortOrder, HandleNum, (LPSTR)(LPCTSTR)paraArray[i].Low_Limit_Value, (LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, atoi(WaitTimeCS),0,GetRDAHostCheckValue);

				Serial_Order_Return_CS_Show = Vaule_Return_Count_CS[HandleNum];
			}
			else if (GetDongleCheckValue == TRUE)
			{
				if (paraArray[i].showName.Find("оƬID") != -1 || paraArray[i].showName.Find("�ź�ǿ��") != -1)
				{
					Send_Serial_Order(&Serial_Order_Return, PortOrder, HandleNum, (LPSTR)(LPCTSTR)paraArray[i].Low_Limit_Value, (LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, atoi(WaitTimeCS));

					Serial_Order_Return_CS_Show = Vaule_Return_Count_CS[HandleNum];
				}
				else
				{
				Send_Serial_Order(&Serial_Order_Return, PortOrder, HandleNum, (LPSTR)(LPCTSTR)paraArray[i].Low_Limit_Value, (LPSTR)(LPCTSTR)paraArray[i].High_Limit_Value, atoi(WaitTimeCS),1);

				Serial_Order_Return_CS_Show = DongleValueAnalyze(paraArray[i].showName, Vaule_Return_Count_CS[HandleNum]);//Ҫȥ��һ���Ƿ���Ҫ��������ת��
				}
			}

			LogShow_exchange(m_Result, Final_Result_Control, 5, "���ڷ���ֵ:" + Serial_Order_Return_CS_Show, HandleNum);				//�������ԣ�״̬������


			bool Judge_Return;																									//�жϽ������ֵ��falseΪ������
			if (Serial_Order_Return != "FAIL")
			{
				//�ɹ������ݴ���
				if (Serial_Order_Return == "DELAY_SUCCESS")																		//��ʱ
				{
					All_Result[i] = 1;
					break; //����do while 
				}
				if (Serial_Order_Return == "Analysis_SUCCESS")																		//���ݽ�������
				{
					Serial_Order_Return = Vaule_Return_Count_CS[HandleNum];
					Judge_Return = IMEI_Function_Judge(i, paraArray[i].showName, (LPTSTR)(LPCTSTR)Serial_Order_Return, HandleNum, m_Result, Final_Result_Control);
				}

				if (GetPicChoose == TRUE)
				{
					//Socket����ͼ���������жϽ��ս���
					if (SocketServer[HandleNum]->m_RecvFlag == TRUE)
					{
						SocketServer[HandleNum]->CloseRecv();
						if (SocketServer[HandleNum]->CreatePic() == FALSE)
						{
							SocketServerPicName[0][HandleNum] = "";
							SocketServerPicName[1][HandleNum] = "";
							LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "ʧ��,ͼ�񴴽�ʧ�ܣ�����ֹͣ������", HandleNum);
							goto OVERDONE;
						}

						//Ϊ�˲�Ӱ��ԭ�е�ͼƬչʾ�߼������ｫͼƬ�ŵ�ԭ�����ͼƬ��������
						if (paraArray[i].showName.Find("ҹ��") != -1)
						{
							PicPath[0][HandleNum] = SocketServerPicName[0][HandleNum];
						}
						else if (paraArray[i].showName.Find("����") != -1)
						{
							PicPath[1][HandleNum] = SocketServerPicName[1][HandleNum];
						}
					}
				}

				
				if (Judge_Return == true)
				{
					//������
					ParaItemName[HandleNum] = "";
					LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "��꣡========������Գɹ�", HandleNum);			//�������ԣ�״̬������
					All_Result[i] = 1;
					/*
					����˳��
					оƬID
					���ɰ汾
					����汾
					*/
					if (paraArray[i].showName.Find("����汾") != -1)//if(paraArray[i].showName.Find("оƬID")!=-1)
					{
						BOOL SMT_Check = FALSE;
						BOOL DBResult = Data_UpdateParaPre(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, &SMT_Check);
						if ((DBResult == TRUE) && (RTestChoose == FALSE))
						{
							if (COM_State[HandleNum] == TRUE)
							{

								if (paraArray[1].High_Limit_Value == "ON")
								{
									LogShow_exchange(m_Result, Final_Result_Control, 5, "���ֵ�ǰ���ڴ�", HandleNum);
								}
								else
								{
									//�����������ԵĻ����ɹ�֮���ֱ��ֹͣ
									if (GetBluetoothCheckValue == TRUE)
									{
										Sleep(2000);
										BleStopPortTest(HandleNum);
									}

									//Dongle��������Ҫ�ǵöϿ�
									if (GetDongleCheckValue == TRUE)
									{
										//if (DongleInfo[HandleNum][0] != "")
										//{
										//	DongleDisConnect(HandleNum);
										//	Sleep(3000);
										//}
										DongleConnectCount[HandleNum]++;
									}

									BOOL Return;
									if (hPort[HandleNum] != NULL||GetRDAHostCheckValue==TRUE)
										Return = CloseHandleControl(hPort[HandleNum], GetRDAHostCheckValue,HandleNum);				//���Գɹ���
									hPort[HandleNum] = NULL;
									if (Return == TRUE)
										LogShow_exchange(m_Result, Final_Result_Control, 5, "�رոô��ڳɹ�����", HandleNum);
									else
									{
										LogShow_exchange(m_Result, Final_Result_Control, 5, "�رոô���ʧ�ܣ���", HandleNum);
										//return FALSE; //�رմ���ʧ��
									}
									COM_State[HandleNum] = FALSE;
								}
							}
							if (SMT_Check == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 2550, "�ɹ�", HandleNum);

							if (g_ADCTFlag == FALSE)/*||GetDongleCheckValue==FALSE*/
							{
								if (GetDongleCheckValue == TRUE)
								{
									DongleCheckRssiFlag[HandleNum] = TRUE;//����������ź�ǿ�ȱ�־
								}
								while (CheckConnect_Thread(m_Port, m_Baud, HandleNum, m_Result, Final_Result_Control) == TRUE)//
								{
									if (GetDongleCheckValue == TRUE)
									{
										DongleCheckRssiFlag[HandleNum] = TRUE;//����������ź�ǿ�ȱ�־
									}
									if (StopSign[HandleNum] == TRUE)
									{
										if (GetRDAHostCheckValue == TRUE)
											RdaHostInterface.RDAComShutdown(HandleNum);
										return FALSE;
									}

									Sleep(300);
								}
							}

							//����ֶ���ʾ��
							if (HandleNum < 8)
							{
								DongleStatusArray[HandleNum] = FALSE;
								DongleTestRssiInfoArray[HandleNum] = "";
								m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA("");
							}

							EnableWindow_Start(HandleNum);
							LogShow_exchange(m_Result, Final_Result_Control, 256, "ѭ����ʼ", HandleNum);
							return TRUE;
						}
					}
					break; //����do while
				}
				else
				{
					//ʧ�ܷ���
					Try_Time++;
					if ((paraArray[0].ifCheck == true) && (Try_Time >= Try_Time_Real))							//ʧ����������
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, "������Ŀ:" + paraArray[i].showName + "ʧ��,����ֹͣ������", HandleNum, "���ܲ���:2-TEST");
						////////////////////////////////////
						/////////////////////////////////////
						//return FALSE; //��������ѭ��for
						goto OVERDONE;//����ʧ�����йرմ���ѡ����
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "ʧ�ܣ����Ի��߼�����һ��Ŀ...", HandleNum);	//�������ԣ�״̬������
						continue;//ѭ��do while
					}
				}
			}
			else
			{
				//ʧ�ܷ���
				Try_Time++;
				if ((paraArray[0].ifCheck == true) && (Try_Time >= Try_Time_Real))//ʧ����������
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, "������Ŀ:" + paraArray[i].showName + "ʧ��,����ֹͣ������", HandleNum, "���ܲ���:2-TEST");
					////////////////////////////////////
					/////////////////////////////////////
					//return FALSE; //��������ѭ��for
					goto OVERDONE;//����ʧ�����йرմ���ѡ����
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 5, "������Ŀ:" + paraArray[i].showName + "ʧ�ܣ����Ի��߼�����һ��Ŀ...", HandleNum);//�������ԣ�״̬������
					continue;//ѭ��do while
				}
			}
			////////////////////////////////////
			//���⴮�ڵĴ���
			if (PortType_CS == "�ն������˿�")
			{
				int t = 0;
				BOOL Port_OK = FALSE;
				do
				{
					if (OnGetport() == TRUE)
					{
						//BOOL Port_OK=FALSE;
						//CString sPort;
						m_Port->GetWindowText(sPort);
						for (int num1 = 0; num1<PORTS_NUM; num1++)
						{
							if ((COM_Number[num1] == sPort) && (sPort != ""))
							{
								Port_OK = TRUE;
								break;
							}
						}
					}
					if (Port_OK == FALSE)
						Sleep(1000);


					t++;
				} while ((Port_OK == FALSE) && (t<5));

				if ((t>1) && (hPort[HandleNum] != NULL || GetRDAHostCheckValue==TRUE))				//���ڲ��ȶ���ʱ��
				{
					CloseHandleControl(hPort[HandleNum],GetRDAHostCheckValue,HandleNum);				//���´򿪴���
					OPen_Serial_Port(m_Port, m_Baud, HandleNum, 0, GetRDAHostCheckValue);	//���ڲ��ȶ���ʱ��
				}
			}
			//���⴮�ڵĴ���
			////////////////////////////////////

		} while (Try_Time<Try_Time_Real);


		Try_Time = 0;//������Դ�������
		All_Result[0] &= All_Result[i];
	}


	if (All_Result[0] == 1)
	{
		UpdateData(TRUE);
		CString m_dbNew;
		//WorkStationControl.GetWindowTextA(WorkStationCS);
		//if(WorkStationCS=="")WorkStationControl.GetWindowTextA(WorkStationCS);
		if ((SIMisOK[HandleNum] != TRUE) && ((WorkStationCS != "��װ����") && (WorkStationCS != "�ϻ������") &&(WorkStationCS != "�з�����")))
		{
			//����һ���Ե�ʱ������Ȱ�����ע�͵�
			ParaItemName[HandleNum] = "SIM��©��";
			LogShow_exchange(m_Result, Final_Result_Control, 128, "©��SIM�����������SIM��������", HandleNum, "���ܲ���:2-TEST");
			goto OVERDONE;//����ʧ�����йرմ���ѡ����
		}
		//�ϴ����ݿ�
		BOOL DBResult = FALSE;
		if (BGConfirmChoose == TRUE)//��̨ȷ�ϣ�ͬʱ���ݿⲻ�����ϴ���������̨����
		{
			DBResult = TRUE;
		}
		else
		{
			EnterCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);

			/*�����ֶη��ظ�����---�����￪ʼ*/

			do{
				if (WorkStationCS == "�з�����")
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�з����ԣ����ϴ��ֶε����ظ����ݿ�", HandleNum);
					break;
				}

				if (AntiDupDataSNUploadFlag[HandleNum] == TRUE)
				{
					//����ֻ�������ݿ��ϴ�
					BOOL UpFlag = Data_AntiDupSNUpload(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, ChipRfID[HandleNum]);
					if (UpFlag == FALSE)
					{
						LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
						goto OVERDONE;
					}
				}

				if (AntiDupDataNoUploadFlag[HandleNum] == TRUE)
				{
					//����ֻ�������ݿ��ϴ�
					BOOL UpFlag = Data_AntiDupDataNoUpload(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, ChipRfID[HandleNum]);
					if (UpFlag == FALSE)
					{
						LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
						goto OVERDONE;
					}
				}
			} while (0);

			/*�����ֶη��ظ�����---�����������ʼ*/

			if (WorkStationCS == "SMT����")
			{
				DBResult = Data_UpdatePara2(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE);
				if (DBResult == FALSE)
					DBResult = Data_UpdatePara2(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control);
			}
			else if (WorkStationCS == "��װ����")
			{
				DBResult = Data_UpdatePara(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE);
				if (DBResult == FALSE)
				{
					Sleep(3000);
					DBResult = Data_UpdatePara(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control);
				}
			}
			else if (WorkStationCS == "�ϻ������")
			{
				DBResult = Data_UpdatePara3(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE);
				if (DBResult == FALSE)
				{
					Sleep(3000);
					DBResult = Data_UpdatePara3(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control);
				}
			}
			else if (WorkStationCS == "�з�����")
				DBResult = TRUE;
			else
			{
				LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
				LogShow_exchange(m_Result, Final_Result_Control, 128, "����ѡ��վλ������", HandleNum, "���ܲ���:2-TEST");
				goto OVERDONE;//����ʧ�����йرմ���ѡ����
			}
			LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
		}
		//////////////////////////////////////////////
		//////////////////////////////////////////////



		if (DBResult == FALSE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "DBResult==FALSE", HandleNum);
			goto OVERDONE;//����ʧ�����йرմ���ѡ����
		}
		else
		{
			if (COM_State[HandleNum] == TRUE)
			{

				if (paraArray[1].High_Limit_Value == "ON")
				{
					LogShow_exchange(m_Result, Final_Result_Control, 5, "���ֵ�ǰ���ڴ�", HandleNum);
				}
				else
				{
					//�����������ԵĻ����ɹ�֮���ֱ��ֹͣ
					if (GetBluetoothCheckValue == TRUE)
					{
						Sleep(2000);
						BleStopPortTest(HandleNum);
					}

					//Dongle��������Ҫ�ǵöϿ�
					if (GetDongleCheckValue == TRUE)
					{
						//if (DongleInfo[HandleNum][0] != "")
						//{
						//	DongleDisConnect(HandleNum);
						//	Sleep(3000);
						//}
						DongleConnectCount[HandleNum]++;
					}

					BOOL Return;
					if (hPort[HandleNum] != NULL || GetRDAHostCheckValue == TRUE)
						Return = CloseHandleControl(hPort[HandleNum], GetRDAHostCheckValue, HandleNum);					//���Գɹ���
					hPort[HandleNum] = NULL;
					if (Return == TRUE)
						LogShow_exchange(m_Result, Final_Result_Control, 5, "�رոô��ڳɹ�����", HandleNum);
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 5, "�رոô���ʧ�ܣ���", HandleNum);
						//return FALSE; //�رմ���ʧ��
					}
					COM_State[HandleNum] = FALSE;
				}
			}
			LogShow_exchange(m_Result, Final_Result_Control, 255, "�ɹ�", HandleNum);

			if (BGConfirmChoose == TRUE)//��̨ȷ�ϣ�����Ҫ�õ�ǰ�߳̽������ÿ�ʼ�µ��߳�
			{
			}
			else
			{
				if (g_ADCTFlag == FALSE )/*|| GetDongleCheckValue==FALSE*/
				{
					DongleCheckRssiFlag[HandleNum] = TRUE;//����������ź�ǿ�ȱ�־
					while (CheckConnect_Thread(m_Port, m_Baud, HandleNum, m_Result, Final_Result_Control) == TRUE)//
					{
						DongleCheckRssiFlag[HandleNum] = TRUE;//����������ź�ǿ�ȱ�־
						if (StopSign[HandleNum] == TRUE)
						{
							if (GetRDAHostCheckValue == TRUE)
								RdaHostInterface.RDAComShutdown(HandleNum);
							return FALSE;
						}
						Sleep(300);
					}
				}
			}

			//����ֶ���ʾ��
			if (HandleNum < 8)
			{
				DongleStatusArray[HandleNum] = FALSE;
				DongleTestRssiInfoArray[HandleNum] = "";
				m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA("");
			}

			EnableWindow_Start(HandleNum);
			LogShow_exchange(m_Result, Final_Result_Control, 256, "ѭ����ʼ", HandleNum);
			return TRUE;
		}
	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 5, "All_Result[0]!=1", HandleNum);
		goto OVERDONE;//���Խ��ʧ��
	}
OVERDONE:
	DoorWait[HandleNum].SetEvent();
	LogShow_exchange(m_Result, Final_Result_Control, 5, "DoorWait[HandleNum].SetEvent()2", HandleNum);
	if (COM_State[HandleNum] == TRUE)
	{

		if (paraArray[1].High_Limit_Value == "ON")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "���ֵ�ǰ���ڴ�", HandleNum);
		}
		else
		{
			//�����������ԵĻ���ʧ��֮���ֱ��ֹͣ
			if (GetBluetoothCheckValue == TRUE)
			{
				Sleep(2000);
				BleStopPortTest(HandleNum);
			}

			//Dongle��������Ҫ�ǵöϿ�
			if (GetDongleCheckValue == TRUE)
			{
				//if (DongleInfo[HandleNum][0] != "")
				//{
				//	DongleDisConnect(HandleNum);
				//	Sleep(3000);
				//}
				DongleConnectCount[HandleNum]++;
			}

			if (AntiDupDataLinkFlag[HandleNum][1] == 1)//�����д��LINK����Ҫ�ǵ�����д��3
			{
				Send_Serial_Order(&Serial_Order_Return, AntiDupDataLinkOrder[HandleNum], HandleNum, "OK", "",0, GetRDAHostCheckValue);
			}

			BOOL Return;
			if (hPort[HandleNum] != NULL || GetRDAHostCheckValue == TRUE)
				Return = CloseHandleControl(hPort[HandleNum], GetRDAHostCheckValue, HandleNum);						//������ɺ�
			hPort[HandleNum] = NULL;
			if (Return == TRUE)
				LogShow_exchange(m_Result, Final_Result_Control, 5, "�رոô��ڳɹ�����", HandleNum);
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 128, "�رոô���ʧ�ܣ���", HandleNum, "���ܲ���:0-�˿�");
				//return FALSE; //�رմ���ʧ��
			}
			COM_State[HandleNum] = FALSE;
		}
	}
	//int FailRestart=0;


	if (g_ADCTFlag == FALSE)/*||GetDongleCheckValue==FALSE*/
	{
		if (GetDongleCheckValue == TRUE)
		{
			DongleCheckRssiFlag[HandleNum] = TRUE;//����������ź�ǿ�ȱ�־
		}

		while (CheckConnect_Thread(m_Port, m_Baud, HandleNum, m_Result, Final_Result_Control) == TRUE)//
		{
			if (GetDongleCheckValue == TRUE)
			{
				DongleCheckRssiFlag[HandleNum] = TRUE;//����������ź�ǿ�ȱ�־
			}
			//if(FailRestart>10)
			if (StopSign[HandleNum] == TRUE)
			{
				if (GetRDAHostCheckValue == TRUE)
					RdaHostInterface.RDAComShutdown(HandleNum);
				return FALSE;
			}
			Sleep(300);
			//FailRestart++;
		}
	}

	//����ֶ���ʾ��
	if (HandleNum < 8)
	{
		DongleStatusArray[HandleNum] = FALSE;
		DongleTestRssiInfoArray[HandleNum] = "";
		m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA("");
	}

	EnableWindow_Start(HandleNum);
	LogShow_exchange(m_Result, Final_Result_Control, 256, "ѭ����ʼ", HandleNum);
	return FALSE;
	//---------------------------11
}

__int64 IMEIWrite_MulAT::ComputeCD(__int64 nImei)
{
	int nTime = 0;
	__int64 nCD = 0;
	while (nImei != 0)
	{
		__int64 d = nImei % 10;
		if (nTime % 2 == 0)//��IMEI��14λ��ż��λ  ����2�������λ����ʮλ��֮�ͣ�
		{
			d *= 2;
			if (d >= 10)
			{
				d = d % 10 + d / 10;
			}
		}
		nCD += d;   //�ټ�����λ��
		nTime++;
		nImei /= 10;
	}
	nCD %= 10;      //��������� �ĸ�λ��
	if (nCD != 0)    //�ø�λ����0 ����У��λΪ0������Ϊ10-�ø�λ��
	{
		nCD = 10 - nCD;
	}
	return nCD;
}
BOOL IMEIWrite_MulAT::CheckIMEI(CString	m_IMEI)
{
	__int64 int64IMEI = _ttoi64(m_IMEI);
	__int64 int64CD = ComputeCD(int64IMEI / 10);
	__int64 int64IMEICD = int64IMEI % 10;
	if (int64IMEICD != int64CD)
	{
		return FALSE;
	}
	return TRUE;
}

void IMEIWrite_MulAT::EnableWindow_ALL(BOOL Choose)
{
	GetDlgItem(IDC_COMBOTESTYYPE2)->EnableWindow(Choose);
	GetDlgItem(IDC_COMBOTESTYYPE3)->EnableWindow(Choose);
	GetDlgItem(IDC_COMBOTESTYYPE4)->EnableWindow(Choose);

	GetDlgItem(IDC_COMBOTESTYYPE)->EnableWindow(Choose);
	ReturnCodeControl.EnableWindow(Choose);

	WorkStationControl.EnableWindow(Choose);
	GetDlgItem(IDC_CHECK39)->EnableWindow(Choose);
	GetDlgItem(IDC_BLUETOOTH_CHECK)->EnableWindow(Choose);
	GetDlgItem(IDC_DongleBle_CHECK)->EnableWindow(Choose);
	GetDlgItem(IDC_DongleBleScan_CHECK)->EnableWindow(Choose);
	GetDlgItem(IDC_CHECK40)->EnableWindow(Choose);
	GetDlgItem(IDC_CHECK41)->EnableWindow(Choose);
	GetDlgItem(IDC_CHECK42)->EnableWindow(Choose);
	GetDlgItem(IDC_CHECK43)->EnableWindow(Choose);
	GetDlgItem(IDC_CHECK44)->EnableWindow(Choose);
	GetDlgItem(IDC_RDAHost_CHECK)->EnableWindow(Choose);
	GetDlgItem(IDC_CHECK45)->EnableWindow(Choose);
	GetDlgItem(IDC_BUTTONCONNCPU)->EnableWindow(Choose);
	GetDlgItem(IDC_BUTTONDISCONNCPU)->EnableWindow(Choose);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(Choose);
	GetDlgItem(IDC_BUTTON25)->EnableWindow(Choose);
	GetDlgItem(IDC_BUTTONCONNCPU)->EnableWindow(Choose);

	VIControlPort.EnableWindow(Choose);
	VIControlBaud.EnableWindow(Choose);
	WIFI_RSSILimit.EnableWindow(Choose);


	//����֮���ʹ��Ӧ�ò���Ҫ��if��������Ĵ��뿴��ȥ�ظ����ܴ�
	m_Port1.EnableWindow(Choose);
	m_Baud1.EnableWindow(Choose);
	m_Port2.EnableWindow(Choose);
	m_Baud2.EnableWindow(Choose);
	m_Port3.EnableWindow(Choose);
	m_Baud3.EnableWindow(Choose);
	m_Port4.EnableWindow(Choose);
	m_Baud4.EnableWindow(Choose);
	m_Port5.EnableWindow(Choose);
	m_Baud5.EnableWindow(Choose);
	m_Port6.EnableWindow(Choose);
	m_Baud6.EnableWindow(Choose);

	m_Port7.EnableWindow(Choose);
	m_Port8.EnableWindow(Choose);
	m_Port9.EnableWindow(Choose);
	m_Port10.EnableWindow(Choose);
	m_Port11.EnableWindow(Choose);
	m_Port12.EnableWindow(Choose);
	m_Port13.EnableWindow(Choose);
	m_Port14.EnableWindow(Choose);
	m_Port15.EnableWindow(Choose);
	m_Port16.EnableWindow(Choose);
	m_Baud7.EnableWindow(Choose);
	m_Baud8.EnableWindow(Choose);
	m_Baud9.EnableWindow(Choose);
	m_Baud10.EnableWindow(Choose);
	m_Baud11.EnableWindow(Choose);
	m_Baud12.EnableWindow(Choose);
	m_Baud13.EnableWindow(Choose);
	m_Baud14.EnableWindow(Choose);
	m_Baud15.EnableWindow(Choose);
	m_Baud16.EnableWindow(Choose);

	CPUControlPort.EnableWindow(Choose);
	CPUControlBaud.EnableWindow(Choose);

	ScanGunPort1.EnableWindow(Choose);
	ScanGunPort2.EnableWindow(Choose);
	ScanGunPort3.EnableWindow(Choose);
	ScanGunPort4.EnableWindow(Choose);
	ScanGunPort5.EnableWindow(Choose);
	ScanGunPort6.EnableWindow(Choose);
	ScanGunBaud1.EnableWindow(Choose);
	ScanGunBaud2.EnableWindow(Choose);
	ScanGunBaud3.EnableWindow(Choose);
	ScanGunBaud4.EnableWindow(Choose);
	ScanGunBaud5.EnableWindow(Choose);
	ScanGunBaud6.EnableWindow(Choose);

	ScanGunTpye.EnableWindow(Choose);
	PortType.EnableWindow(Choose);
	OrderNumbersControl.EnableWindow(Choose);
	GPS_SNRLimit.EnableWindow(Choose);

	EnableMenu = Choose;


	//if (Choose == TRUE)//ʹ�����ж˿�
	//{
	//	m_Port1.EnableWindow(TRUE);
	//	m_Baud1.EnableWindow(TRUE);
	//	m_Port2.EnableWindow(TRUE);
	//	m_Baud2.EnableWindow(TRUE);
	//	m_Port3.EnableWindow(TRUE);
	//	m_Baud3.EnableWindow(TRUE);
	//	m_Port4.EnableWindow(TRUE);
	//	m_Baud4.EnableWindow(TRUE);
	//	m_Port5.EnableWindow(TRUE);
	//	m_Baud5.EnableWindow(TRUE);
	//	m_Port6.EnableWindow(TRUE);
	//	m_Baud6.EnableWindow(TRUE);

	//	m_Port7.EnableWindow(TRUE);
	//	m_Port8.EnableWindow(TRUE);
	//	m_Port9.EnableWindow(TRUE);
	//	m_Port10.EnableWindow(TRUE);
	//	m_Port11.EnableWindow(TRUE);
	//	m_Port12.EnableWindow(TRUE);
	//	m_Port13.EnableWindow(TRUE);
	//	m_Port14.EnableWindow(TRUE);
	//	m_Port15.EnableWindow(TRUE);
	//	m_Port16.EnableWindow(TRUE);
	//	m_Baud7.EnableWindow(TRUE);
	//	m_Baud8.EnableWindow(TRUE);
	//	m_Baud9.EnableWindow(TRUE);
	//	m_Baud10.EnableWindow(TRUE);
	//	m_Baud11.EnableWindow(TRUE);
	//	m_Baud12.EnableWindow(TRUE);
	//	m_Baud13.EnableWindow(TRUE);
	//	m_Baud14.EnableWindow(TRUE);
	//	m_Baud15.EnableWindow(TRUE);
	//	m_Baud16.EnableWindow(TRUE);

	//	CPUControlPort.EnableWindow(TRUE);
	//	CPUControlBaud.EnableWindow(TRUE);

	//	ScanGunPort1.EnableWindow(TRUE);
	//	ScanGunPort2.EnableWindow(TRUE);
	//	ScanGunPort3.EnableWindow(TRUE);
	//	ScanGunPort4.EnableWindow(TRUE);
	//	ScanGunPort5.EnableWindow(TRUE);
	//	ScanGunPort6.EnableWindow(TRUE);
	//	ScanGunBaud1.EnableWindow(TRUE);
	//	ScanGunBaud2.EnableWindow(TRUE);
	//	ScanGunBaud3.EnableWindow(TRUE);
	//	ScanGunBaud4.EnableWindow(TRUE);
	//	ScanGunBaud5.EnableWindow(TRUE);
	//	ScanGunBaud6.EnableWindow(TRUE);

	//	ScanGunTpye.EnableWindow(TRUE);
	//	PortType.EnableWindow(TRUE);
	//	OrderNumbersControl.EnableWindow(TRUE);
	//	GPS_SNRLimit.EnableWindow(TRUE);


	//	EnableMenu = TRUE;
	//}
	//else
	//{
	//	m_Port1.EnableWindow(FALSE);
	//	m_Baud1.EnableWindow(FALSE);
	//	m_Port2.EnableWindow(FALSE);
	//	m_Baud2.EnableWindow(FALSE);
	//	m_Port3.EnableWindow(FALSE);
	//	m_Baud3.EnableWindow(FALSE);
	//	m_Port4.EnableWindow(FALSE);
	//	m_Baud4.EnableWindow(FALSE);
	//	m_Port5.EnableWindow(FALSE);
	//	m_Baud5.EnableWindow(FALSE);
	//	m_Port6.EnableWindow(FALSE);
	//	m_Baud6.EnableWindow(FALSE);

	//	m_Port7.EnableWindow(FALSE);
	//	m_Port8.EnableWindow(FALSE);
	//	m_Port9.EnableWindow(FALSE);
	//	m_Port10.EnableWindow(FALSE);
	//	m_Port11.EnableWindow(FALSE);
	//	m_Port12.EnableWindow(FALSE);
	//	m_Port13.EnableWindow(FALSE);
	//	m_Port14.EnableWindow(FALSE);
	//	m_Port15.EnableWindow(FALSE);
	//	m_Port16.EnableWindow(FALSE);
	//	m_Baud7.EnableWindow(FALSE);
	//	m_Baud8.EnableWindow(FALSE);
	//	m_Baud9.EnableWindow(FALSE);
	//	m_Baud10.EnableWindow(FALSE);
	//	m_Baud11.EnableWindow(FALSE);
	//	m_Baud12.EnableWindow(FALSE);
	//	m_Baud13.EnableWindow(FALSE);
	//	m_Baud14.EnableWindow(FALSE);
	//	m_Baud15.EnableWindow(FALSE);
	//	m_Baud16.EnableWindow(FALSE);

	//	CPUControlPort.EnableWindow(FALSE);
	//	CPUControlBaud.EnableWindow(FALSE);

	//	ScanGunPort1.EnableWindow(FALSE);
	//	ScanGunPort2.EnableWindow(FALSE);
	//	ScanGunPort3.EnableWindow(FALSE);
	//	ScanGunPort4.EnableWindow(FALSE);
	//	ScanGunPort5.EnableWindow(FALSE);
	//	ScanGunPort6.EnableWindow(FALSE);
	//	ScanGunBaud1.EnableWindow(FALSE);
	//	ScanGunBaud2.EnableWindow(FALSE);
	//	ScanGunBaud3.EnableWindow(FALSE);
	//	ScanGunBaud4.EnableWindow(FALSE);
	//	ScanGunBaud5.EnableWindow(FALSE);
	//	ScanGunBaud6.EnableWindow(FALSE);

	//	ScanGunTpye.EnableWindow(FALSE);
	//	PortType.EnableWindow(FALSE);
	//	OrderNumbersControl.EnableWindow(FALSE);
	//	GPS_SNRLimit.EnableWindow(FALSE);


	//	EnableMenu = FALSE;
	//}
}
void IMEIWrite_MulAT::EnableWindow_StartALL(BOOL Choose)
{
	OnBnClickedButton2();
	OnBnClickedButton15();
	OnBnClickedButton17();
	OnBnClickedButton19();
	OnBnClickedButton21();
	OnBnClickedButton23();
	OnBnClickedButtonstop7();
	OnBnClickedButtonstop8();
	OnBnClickedButtonstop9();
	OnBnClickedButtonstop10();
	OnBnClickedButtonstop11();
	OnBnClickedButtonstop12();
	OnBnClickedButtonstop13();
	OnBnClickedButtonstop14();
	OnBnClickedButtonstop15();
	OnBnClickedButtonstop16();
	if (1)
	{
		StartA_Control.EnableWindow(Choose);
		StartB_Control.EnableWindow(Choose);
		StartC_Control.EnableWindow(Choose);
		StartD_Control.EnableWindow(Choose);
		StartE_Control.EnableWindow(Choose);
		StartF_Control.EnableWindow(Choose);
		Start7_Control.EnableWindow(Choose);
		Start8_Control.EnableWindow(Choose);
		Start9_Control.EnableWindow(Choose);
		Start10_Control.EnableWindow(Choose);
		Start11_Control.EnableWindow(Choose);
		Start12_Control.EnableWindow(Choose);
		Start13_Control.EnableWindow(Choose);
		Start14_Control.EnableWindow(Choose);
		Start15_Control.EnableWindow(Choose);
		Start16_Control.EnableWindow(Choose);
		StartALL_Control.EnableWindow(Choose);
		StopALL_Control.EnableWindow(Choose);

		GetDlgItem(IDC_BUTTON2)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTON15)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTON17)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTON21)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTON23)->EnableWindow(Choose);
		GetDlgItem(IDC_RESETBLE_BUTTON)->EnableWindow(Choose);

		GetDlgItem(IDC_BUTTONSTOP7)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP8)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP9)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP10)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP11)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP12)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP13)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP14)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP15)->EnableWindow(Choose);
		GetDlgItem(IDC_BUTTONSTOP16)->EnableWindow(Choose);
	}

}
void IMEIWrite_MulAT::EnableWindow_Start(int HandleNum)
{
	switch (HandleNum)
	{
	case 0:
		StartA_Control.EnableWindow(TRUE);
		break;
	case 1:
		StartB_Control.EnableWindow(TRUE);
		break;
	case 2:
		StartC_Control.EnableWindow(TRUE);
		break;
	case 3:
		StartD_Control.EnableWindow(TRUE);
		break;
	case 4:
		StartE_Control.EnableWindow(TRUE);
		break;
	case 5:
		StartF_Control.EnableWindow(TRUE);
		break;
	case 6:
		Start7_Control.EnableWindow(TRUE);
		break;
	case 7:
		Start8_Control.EnableWindow(TRUE);
		break;
	case 8:
		if (GetPicChoose != TRUE)
			Start9_Control.EnableWindow(TRUE);
		break;
	case 9:
		if (GetPicChoose != TRUE)
			Start10_Control.EnableWindow(TRUE);
		break;
	case 10:
		if (GetPicChoose != TRUE)
			Start11_Control.EnableWindow(TRUE);
		break;
	case 11:
		if (GetPicChoose != TRUE)
			Start12_Control.EnableWindow(TRUE);
		break;
	case 12:
		if (GetPicChoose != TRUE)
			Start13_Control.EnableWindow(TRUE);
		break;
	case 13:
		if (GetPicChoose != TRUE)
			Start14_Control.EnableWindow(TRUE);
		break;
	case 14:
		if (GetPicChoose != TRUE)
			Start15_Control.EnableWindow(TRUE);
		break;
	case 15:
		if (GetPicChoose != TRUE)
			Start16_Control.EnableWindow(TRUE);
		break;
	default:
		StartA_Control.EnableWindow(TRUE);
		break;
	}
}
void IMEIWrite_MulAT::EnableWindow_Choose(int HandleNum)
{
	switch (HandleNum)
	{
	case 0:
		IMEI_InputA_Control.EnableWindow(TRUE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	case 1:
		IMEI_InputA_Control.EnableWindow(FALSE);
		IMEI_InputB_Control.EnableWindow(TRUE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	case 2:
		IMEI_InputA_Control.EnableWindow(FALSE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(TRUE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	case 3:
		IMEI_InputA_Control.EnableWindow(FALSE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(TRUE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	case 4:
		IMEI_InputA_Control.EnableWindow(FALSE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(TRUE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	case 5:
		IMEI_InputA_Control.EnableWindow(FALSE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(TRUE);
		break;
	case 6://�ص���һ��IMEI
		IMEI_InputA_Control.EnableWindow(TRUE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	default:
		IMEI_InputA_Control.EnableWindow(FALSE);
		IMEI_InputB_Control.EnableWindow(FALSE);
		IMEI_InputC_Control.EnableWindow(FALSE);
		IMEI_InputD_Control.EnableWindow(FALSE);
		IMEI_InputE_Control.EnableWindow(FALSE);
		IMEI_InputF_Control.EnableWindow(FALSE);
		break;
	}
}

void IMEIWrite_MulAT::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 0:
		if ((Thread_State[0] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(0);
			OnBnClickedButton1();
		}
		break;
	case 1:
		if ((Thread_State[1] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(1);
			OnBnClickedButton14();
		}
		break;
	case 2:
		if ((Thread_State[2] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(2);
			OnBnClickedButton16();
		}
		break;
	case 3:
		if ((Thread_State[3] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(3);
			OnBnClickedButton18();
		}
		break;
	case 4:
		if ((Thread_State[4] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(4);
			OnBnClickedButton20();
		}
		break;
	case 5:
		if ((Thread_State[5] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(5);
			OnBnClickedButton22();
		}
		break;

	case 6:
		if ((Thread_State[6] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(6);
			OnBnClickedButtonstart7();
		}
		break;
	case 7:
		if ((Thread_State[7] == FALSE) || (BGConfirmChoose == TRUE))
		{
			KillTimer(7);
			OnBnClickedButtonstart8();
		}
		break;
	case 8:
		if (Thread_State[8] == FALSE)
		{
			KillTimer(8);
			OnBnClickedButtonstart9();
		}
		break;
	case 9:
		if (Thread_State[9] == FALSE)
		{
			KillTimer(9);
			OnBnClickedButtonstart10();
		}
		break;
	case 10:
		if (Thread_State[10] == FALSE)
		{
			KillTimer(10);
			OnBnClickedButtonstart11();
		}
		break;
	case 11:
		if (Thread_State[11] == FALSE)
		{
			KillTimer(11);
			OnBnClickedButtonstart12();
		}
		break;
	case 12:
		if (Thread_State[12] == FALSE)
		{
			KillTimer(12);
			OnBnClickedButtonstart13();
		}
		break;
	case 13:
		if (Thread_State[13] == FALSE)
		{
			KillTimer(13);
			OnBnClickedButtonstart14();
		}
		break;
	case 14:
		if (Thread_State[14] == FALSE)
		{
			KillTimer(14);
			OnBnClickedButtonstart15();
		}
		break;
	case 15:
		if (Thread_State[15] == FALSE)
		{
			KillTimer(15);
			OnBnClickedButtonstart16();
		}
		break;

	case 30:
		KillTimer(30);
		if (TestCheck == TRUE)
		{
			Get_ScanGun_Data(&m_Result1, &Final_Result_Control1, 0, "\r");				//���ɨ��ǹ��������
			SetTimer(30, 300, NULL);
			return;
		}
		else
		{
			//�������ѡ�������Ű���ԭ�߼�������Ҫ����ȥcheckɨ��ǹ����
			if (Get_ScanGun_Data(&m_Result1, &Final_Result_Control1, 0, "\r") == TRUE&&GetBluetoothCheckValue == FALSE&&GetDongleCheckValue == FALSE&&GetDongleScanCheckValue == FALSE)	//ɨ��ǹ������
			{
				if (Check_ScanGun_Data(&m_Result1, &Final_Result_Control1, 0, NULL, TRUE, 15) == TRUE)			//IMEI��
				{
					//����ɨ��ǹ�������ˣ��ȴ�����OK
					TestCheck = TRUE;//��ɨ������IMEI��
				}
				else
				{
					SetTimer(30, 1000, NULL);
				}
			}
			else
			{
				SetTimer(30, 1000, NULL);
			}
		}
		break;
	case 31:
		KillTimer(31);
		LoadConfig_Change();
		//����һʵ���Զ���½,��������
		if (g_ADCTFlag == 1)
		{
			OnBnClickedButton6();//�������
			MSG_SendSimpleMessage();//���;�������س���

			if (g_IsHideFlag == 1)
			{
				ShowWindow(SW_SHOWMINIMIZED);
				ShowWindow(SW_HIDE);//���ش���
				//ShowWindow(SW_HIDE);//���ش���
				//ShowWindow(SW_HIDE);//���ش���
				//ShowWindow(SW_HIDE);//���ش���
			}

			break;
		}
		else if (g_ADCTFlag != 1)
		{
			//ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOWMAXIMIZED);
		}
		break;
	case 32:
		KillTimer(32);
		OnBnClickedBtnCopy();
		break;
	case 48:
		KillTimer(48);
		if (Picdlg[TimerHandleNum].DoModal() == -1)
		{
			AfxMessageBox("����ͼƬʧ��");
		}
		break;
	case 49:
		KillTimer(49);
		Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);//Picdlg[TimerHandleNum].DestroyWindow();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
void IMEIWrite_MulAT::SetFocus_Choose(int HandleNum)//�Զ���ʼ
{
	if (StopAutoStart == FALSE)
		SetTimer(HandleNum, 500, NULL);
}

BOOL IMEIWrite_MulAT::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey || VK_ESCAPE == nKey)
		{
			//pMsg->message = WM_KILLFOCUS;
			if (VK_ESCAPE == nKey)
			{
				//m_bInputValid = FALSE;    //��ʱ�ı༭�����Ч
				return TRUE;
			}

		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}




BOOL CALLBACK EnumWindowsProc_1(HWND hwnd, LPARAM lparam)//�оٴ���
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
	LONG gwl_style = GetWindowLong(hwnd, GWL_STYLE);
	if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION))
		return TRUE;

	//�����Ƿ���и����ڣ�
	HWND hParent = (HWND)GetWindowLong(hwnd, GWL_HWNDPARENT);
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


	GetClassName(hwnd, szClassName, 255);
	if (!strcmp(szClassName, "Shell_TrayWnd"))
		return TRUE;


	wininfo infoptr;
	::GetWindowText(hwnd, infoptr.winbuf, 99);//��ȡ��������

	if (!strcmp(infoptr.winbuf, "Windows ���������"))
		return TRUE;

	if (!strcmp(infoptr.winbuf, "�ܶ��ľ���"))
		return TRUE;

	if (infoptr.winbuf[0] == 0)
		return TRUE;

	if (strcmp(infoptr.filename, "C:\\WINDOWS\\Explorer.EXE"))//��ȡ����ͼ����
	{
		hIcon = ExtractIcon((HINSTANCE)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, infoptr.pid),
			infoptr.filename, 0);
	}
	else
	{
		hIcon = (HICON)(::GetClassLong(hwnd, GCL_HICON));//��ȡ����ͼ����
	}

	infoptr.hwnd = hwnd;

	infoptr.winicon = hIcon;

	infoptr.sign = 2;

	((IMEIWrite_MulAT *)lparam)->m_wininfoarray.Add(infoptr);
	return TRUE;
}

void IMEIWrite_MulAT::Get_App_Running()//��ȡ�������е�Ӧ�� 
{
	EnumWindows((WNDENUMPROC)EnumWindowsProc_1, (long)this);
	int size = m_wininfoarray.GetSize();
	for (int i = 0; i<size; i++)
	{
		m_currentwin = m_wininfoarray.GetAt(i);

		int Result_compare = _stricmp("���߳�IMEIд��", m_currentwin.winbuf);
		if (Result_compare == 0)
		{
		}
	}
	for (int i = 0; i<size; i++)
	{
		m_currentwin = m_wininfoarray.GetAt(i);

		CString IDName;
		IDName.Format("%s", m_currentwin.winbuf);
		int Result_compare0 = IDName.Find("Printing");
		if (Result_compare0 != -1)
		{
			Save_Wait.ResetEvent();//�ȴ�ģ�ⰴ����ɰ���
			OnWndFocus(TRUE, TRUE, IMEI_Input);//����ǰ��,���µĴ�����������
			WaitForSingleObject(Save_Wait.m_hObject, 6000);
			OnWndFocus(FALSE, FALSE, IMEI_Input);//������ã�
			break;
		}
	}
	for (int i = 0; i<size; i++)
	{
		m_currentwin = m_wininfoarray.GetAt(i);

		int Result_compare = _stricmp("���߳�IMEIд��", m_currentwin.winbuf);
		if (Result_compare == 0)
		{
			OnWndFocus(TRUE, FALSE, IMEI_Input);//����ǰ��
		}
	}
	m_wininfoarray.RemoveAll();//ִ�к󣬽������е����
}

void IMEIWrite_MulAT::OnWndFocus(BOOL ShowWin, BOOL Print, CString IMEI_Print[THREAD_NUM])
{
	WINDOWPLACEMENT   ws;
	ws.length = sizeof(WINDOWPLACEMENT);
	::GetWindowPlacement(m_currentwin.hwnd, &ws);
	if (ShowWin == FALSE)
	{
		switch (ws.showCmd)
		{
		case   1://����; 

		case   3://ԭʼ����Ϊ���	
			//::ShowWindow(m_currentwin.hwnd,SW_SHOW);			//ԭʼ
			::ShowWindow(m_currentwin.hwnd, SW_NORMAL);			//�޸�
			//::ShowWindow(m_currentwin.hwnd,SW_SHOW);
			//::SetForegroundWindow(m_currentwin.hwnd);
			::SetWindowPos(m_currentwin.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
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
			::SetWindowPos(m_currentwin.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);	//����
			break;
		default:
			::ShowWindow(m_currentwin.hwnd, SW_NORMAL);										//SW_SHOWMINIMIZED
			::SetWindowPos(m_currentwin.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);	//����
			break;
		}
	}
	else
	{
		switch (ws.showCmd)
		{
		case   1://����; 

		case   3://ԭʼ����Ϊ���	
			::ShowWindow(m_currentwin.hwnd, SW_NORMAL);			//�޸�
			::SetWindowPos(m_currentwin.hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			break;

		case   2://ԭʼ����Ϊ(��)С��;
			//::ShowWindow(m_currentwin.hwnd,SW_SHOWMAXIMIZED);								//SW_SHOWMINIMIZED
			::SwitchToThisWindow(m_currentwin.hwnd, TRUE);
			::SetWindowPos(m_currentwin.hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);	//����
			break;
		default:
			::ShowWindow(m_currentwin.hwnd, SW_NORMAL);										//SW_SHOWMINIMIZED
			::SetWindowPos(m_currentwin.hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);	//����
			break;
		}
	}
	if (Print == TRUE)
	{
		byte buf[THREAD_NUM][32] = { '\0' };
		for (int i = 0; i<THREAD_NUM; i++)
		{
			if (IMEI_Print[i] == "")
				continue;
			memcpy(buf[i], IMEI_Print[i].GetBuffer(IMEI_Print[i].GetLength()), IMEI_Print[i].GetLength());  //��cstring����byte����
			//ģ������
			Sleep(500);
			for (int j = 0; j<32; j++)
			{
				if (buf[i][j] == '\0')
					break;
				//keybd_event('A', 0, 0, 0); 
				//keybd_event('A', 0, KEYEVENTF_KEYUP, 0); 
				keybd_event(buf[i][j], 0, 0, 0);
				keybd_event(buf[i][j], 0, KEYEVENTF_KEYUP, 0);
				Sleep(5);


				//keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); 
				//keybd_event(VK_RETURN, 0, 0, 0); 
				//keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  
			}
			keybd_event('\r', 0, 0, 0);
			keybd_event('\r', 0, KEYEVENTF_KEYUP, 0);
			keybd_event('\n', 0, 0, 0);
			keybd_event('\n', 0, KEYEVENTF_KEYUP, 0);
			Sleep(100);
		}
	}
	Save_Wait.SetEvent();
	//ģ��ctrl+S����
}


void IMEIWrite_MulAT::OnEnChangeImeia()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//if(NETCODE_Choose_M=="true")
	{
		UpdateData(TRUE);
		UpdateData(false);//�ѱ�����ʾ�ڽ���

		int IMEI_length;
		IMEI_Input[0] = "";
		if (((IMEI_InputA.Find("\r") != -1) || (IMEI_InputA.Find("\n") != -1)))//ѡ����IMEI1  &&IMEI_length==15
		{
			EnableWindow_Choose(-1);
			//check whether digits
			IMEI_InputA.Replace("\r", "");
			IMEI_InputA.Replace("\n", "");
			IMEI_length = IMEI_InputA.GetLength();
			////////////////////�������ж�///////////////////
			//check whether digits
			if ((IMEI_length == 0))
			{
				EnableWindow_Choose(0);
				UpdateData(FALSE);//������Ļس����
				return;
			}
			//AfxMessageBox(IMEI_InputA);
			IMEI_InputA_Control.SetWindowTextA(IMEI_InputA);
			for (int i = 0; i<IMEI_length; i++)
			{
				if (IMEI_length != 15 || (IMEI_InputA.GetAt(i) <'0' || IMEI_InputA.GetAt(i) >'9'))
				{
					EnableWindow_Choose(0);
					LogShow_exchange(&m_Result1, &Final_Result_Control1, 128, "Please check IMEI_InputA. Only 15 digits allowed!\r\n", 0);
					IMEI_InputA_Control.SetFocus();
					IMEI_InputA_Control.SetSel(0, -1);
					return;
				}

			}
			if (CheckIMEI(IMEI_InputA) == FALSE)
			{
				EnableWindow_Choose(0);
				LogShow_exchange(&m_Result1, &Final_Result_Control1, 128, "IMEI_InputA ���Ϸ�", 0);
				IMEI_InputA_Control.SetFocus();
				IMEI_InputA_Control.SetSel(0, -1);
				return;
			}
			////////////////////�������ж�///////////////////
			IMEI_Input[0] = IMEI_InputA;
			LogShow_exchange(&m_Result1, &Final_Result_Control1, 0, "ɨ������IMEI:" + IMEI_Input[0] + "�ɹ�...\r\n", 0);

			OnBnClickedButton1();
		}
	}
}



UINT static __cdecl WriteIMEIFunction1(LPVOID pParam)
{
	BOOL flag;

	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	flag = Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port1), &(Mead_Main_Win->m_Baud1), 0, &(Mead_Main_Win->m_Result1), &(Mead_Main_Win->Final_Result_Control1), &(Mead_Main_Win->IMEI_InputA_Control));

	//��ADCT���س����Ͳ��Խ��
	if (g_ADCTFlag == 1)
	{
		//����Ǳ�ǿ��ֹͣ�ˣ��Ǿ�ֱ�ӷ��أ���Ҫ�����س�����Ϣ
		if (Mead_Main_Win->StopSign[0] == TRUE)
		{
			return 0;
		}

		CString str_Port;
		Mead_Main_Win->GetDlgItemText(IDC_COMBO1, str_Port);
		if (flag == TRUE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "1");
		}
		else if (flag == FALSE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "0");
		}
		Mead_Main_Win->OnBnClickedButton2();
	}
	return 0;
}

void IMEIWrite_MulAT::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 0;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	StopSign[ArraySign] = FALSE;
	StartA_Control.EnableWindow(FALSE);
	Thread_Handle[ArraySign] = AfxBeginThread(WriteIMEIFunction1, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	SetTimer(32, 5000, NULL);

	StartButtonGatherFun(ArraySign);

	////������������ͷSOCKET
	//if (GetPicChoose == TRUE)
	//{
	//	SocketServer[ArraySign]->StartServer(SocketServerPort[ArraySign]);
	//}

	////��ʼ��Dongle����
	//if (GetDongleCheckValue == TRUE)
	//{
	//	if (DongleConnectCount[ArraySign] == DongleResetCount || DongleConnectCount[ArraySign] == 0)
	//	{
	//		DongleConnectCount[ArraySign]=0;
	//		DongleInit(ArraySign);
	//	}
	//}

}

void IMEIWrite_MulAT::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 0;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	int HandleNum = 0;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	KillTimer(32);
	StopSign[ArraySign] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[ArraySign] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[0]=FALSE;
	LogShow_exchange(&m_Result1, &Final_Result_Control1, 250, "�������ԣ���", 0);

	StopButtonGatherFun(ArraySign);

	////ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	//if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[ArraySign] == 1)
	//{
	//	BleGetSettingFlag[ArraySign] = 0;
	//	BluetoothDisconnect(ArraySign);
	//}

	//if (0)
	//{
	//	BOOL Return = CloseHandle(hPort[ArraySign]);				//���Գɹ���
	//	if (Return == TRUE)
	//	{
	//	}
	//	else
	//	{
	//		AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
	//		return;
	//	}
	//	hPort[ArraySign] = NULL;
	//}

	////ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	//if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	//{
	//	DeleteComoOccupancydDeq(ArraySign);
	//}

	////�ر�Socket������
	//SocketServer[ArraySign]->CloseRecv();
	//SocketServer[ArraySign]->StopServer();

	////�ر�Dongle����
	//if (GetDongleCheckValue == TRUE&&DongleInfo[ArraySign][0] != "")
	//{
	//	DongleDisConnect(ArraySign);
	//}
}


//�߳�2

void IMEIWrite_MulAT::OnEnChangeImeia2()
{
	//if(NETCODE_Choose_M=="true")
	{
		UpdateData(TRUE);
		UpdateData(false);//�ѱ�����ʾ�ڽ���

		int IMEI_length;
		IMEI_Input[1] = "";
		if (((IMEI_InputB.Find("\r") != -1) || (IMEI_InputB.Find("\n") != -1)))//ѡ����IMEI1  &&IMEI_length==15
		{
			EnableWindow_Choose(-1);
			//check whether digits
			IMEI_InputB.Replace("\r", "");
			IMEI_InputB.Replace("\n", "");
			IMEI_length = IMEI_InputB.GetLength();
			////////////////////�������ж�///////////////////
			//check whether digits
			if ((IMEI_length == 0))
			{
				EnableWindow_Choose(1);
				UpdateData(FALSE);//������Ļس����
				return;
			}
			IMEI_InputB_Control.SetWindowTextA(IMEI_InputB);
			for (int i = 0; i<IMEI_length; i++)
			{
				if (IMEI_length != 15 || (IMEI_InputB.GetAt(i) <'0' || IMEI_InputB.GetAt(i) >'9'))
				{
					EnableWindow_Choose(1);
					LogShow_exchange(&m_Result2, &Final_Result_Control2, 128, "Please check IMEI_InputB. Only 15 digits allowed!\r\n", 1);
					IMEI_InputB_Control.SetFocus();
					IMEI_InputB_Control.SetSel(0, -1);
					return;
				}

			}
			if (CheckIMEI(IMEI_InputB) == FALSE)
			{
				EnableWindow_Choose(1);
				LogShow_exchange(&m_Result2, &Final_Result_Control2, 128, "IMEI_InputA ���Ϸ�", 1);
				IMEI_InputB_Control.SetFocus();
				IMEI_InputB_Control.SetSel(0, -1);
				return;
			}
			////////////////////�������ж�///////////////////
			IMEI_Input[1] = IMEI_InputB;
			LogShow_exchange(&m_Result2, &Final_Result_Control2, 0, "ɨ������IMEI:" + IMEI_Input[1] + "�ɹ�...\r\n", 1);

			OnBnClickedButton14();
		}
	}
}


UINT static __cdecl WriteIMEIFunction2(LPVOID pParam)
{
	BOOL flag;

	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	flag=Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port2), &(Mead_Main_Win->m_Baud2), 1, &(Mead_Main_Win->m_Result2), &(Mead_Main_Win->Final_Result_Control2), &(Mead_Main_Win->IMEI_InputB_Control));


	//��ADCT���س����Ͳ��Խ��
	if (g_ADCTFlag == 1)
	{
		//����Ǳ�ǿ��ֹͣ�ˣ��Ǿ�ֱ�ӷ��أ���Ҫ�����س�����Ϣ
		if (Mead_Main_Win->StopSign[1] == TRUE)
		{
			return 0;
		}

		CString str_Port;
		Mead_Main_Win->GetDlgItemText(IDC_COMBO4, str_Port);
		if (flag == TRUE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "1");
		}
		else if (flag == FALSE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "0");
		}
		Mead_Main_Win->OnBnClickedButton15();
	}
	return 0;
}

void IMEIWrite_MulAT::OnBnClickedButton14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 1;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	StopSign[ArraySign] = FALSE;
	StartB_Control.EnableWindow(FALSE);
	Thread_Handle[ArraySign] = AfxBeginThread(WriteIMEIFunction2, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	StartButtonGatherFun(ArraySign);


	////������������ͷSOCKET
	//if (GetPicChoose == TRUE)
	//{
	//	SocketServer[ArraySign]->StartServer(SocketServerPort[ArraySign]);
	//}


	////��ʼ��Dongle����
	//if (GetDongleCheckValue == TRUE)
	//{
	//	if (DongleConnectCount[ArraySign] == DongleResetCount || DongleConnectCount[ArraySign] == 0)
	//	{
	//		DongleConnectCount[ArraySign] = 0;
	//		DongleInit(ArraySign);
	//	}
	//}
}

void IMEIWrite_MulAT::OnBnClickedButton15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 1;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	int HandleNum = 1;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[ArraySign] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[ArraySign] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[1]=FALSE;
	LogShow_exchange(&m_Result2, &Final_Result_Control2, 250, "�������ԣ���", 1);

	StopButtonGatherFun(ArraySign);

	////ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	//if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[ArraySign] == 1)
	//{
	//	BleGetSettingFlag[ArraySign] = 0;
	//	BluetoothDisconnect(ArraySign);
	//}

	//if (0)
	//{
	//	BOOL Return = CloseHandle(hPort[ArraySign]);				//���Գɹ���
	//	if (Return == TRUE)
	//	{
	//	}
	//	else
	//	{
	//		AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
	//		return;
	//	}
	//	hPort[ArraySign] = NULL;
	//}
	////ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	//if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	//{
	//	DeleteComoOccupancydDeq(ArraySign);
	//}

	////�ر�Socket������
	//SocketServer[ArraySign]->CloseRecv();
	//SocketServer[ArraySign]->StopServer();

	////�ر�Dongle����
	//if (GetDongleCheckValue == TRUE&&DongleInfo[ArraySign][0]!="")
	//{
	//	DongleDisConnect(ArraySign);
	//}
}

void IMEIWrite_MulAT::OnEnChangeImeia3()
{
	//if(NETCODE_Choose_M=="true")
	{
		UpdateData(TRUE);
		UpdateData(false);//�ѱ�����ʾ�ڽ���

		int IMEI_length;
		IMEI_Input[2] = "";
		if (((IMEI_InputC.Find("\r") != -1) || (IMEI_InputC.Find("\n") != -1)))//ѡ����IMEI1  &&IMEI_length==15
		{
			EnableWindow_Choose(-1);
			//check whether digits
			IMEI_InputC.Replace("\r", "");
			IMEI_InputC.Replace("\n", "");
			IMEI_length = IMEI_InputC.GetLength();
			////////////////////�������ж�///////////////////
			//check whether digits
			if ((IMEI_length == 0))
			{
				EnableWindow_Choose(2);
				UpdateData(FALSE);//������Ļس����
				return;
			}
			IMEI_InputC_Control.SetWindowTextA(IMEI_InputC);
			for (int i = 0; i<IMEI_length; i++)
			{
				if (IMEI_length != 15 || (IMEI_InputC.GetAt(i) <'0' || IMEI_InputC.GetAt(i) >'9'))
				{
					EnableWindow_Choose(2);
					LogShow_exchange(&m_Result3, &Final_Result_Control3, 128, "Please check IMEI_InputC. Only 15 digits allowed!\r\n", 2);
					IMEI_InputC_Control.SetFocus();
					IMEI_InputC_Control.SetSel(0, -1);
					return;
				}

			}
			if (CheckIMEI(IMEI_InputC) == FALSE)
			{
				EnableWindow_Choose(2);
				LogShow_exchange(&m_Result3, &Final_Result_Control3, 128, "IMEI_InputC ���Ϸ�", 2);
				IMEI_InputC_Control.SetFocus();
				IMEI_InputC_Control.SetSel(0, -1);
				return;
			}
			////////////////////�������ж�///////////////////
			IMEI_Input[2] = IMEI_InputC;
			LogShow_exchange(&m_Result3, &Final_Result_Control3, 0, "ɨ������IMEI:" + IMEI_Input[2] + "�ɹ�...\r\n", 2);

			OnBnClickedButton16();
		}
	}
}

UINT static __cdecl WriteIMEIFunction3(LPVOID pParam)
{
	BOOL flag;

	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	flag = Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port3), &(Mead_Main_Win->m_Baud3), 2, &(Mead_Main_Win->m_Result3), &(Mead_Main_Win->Final_Result_Control3), &(Mead_Main_Win->IMEI_InputC_Control));


	//��ADCT���س����Ͳ��Խ��
	if (g_ADCTFlag == 1)
	{
		//����Ǳ�ǿ��ֹͣ�ˣ��Ǿ�ֱ�ӷ��أ���Ҫ�����س�����Ϣ
		if (Mead_Main_Win->StopSign[2] == TRUE)
		{
			return 0;
		}

		CString str_Port;
		Mead_Main_Win->GetDlgItemText(IDC_COMBO6, str_Port);
		if (flag == TRUE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "1");
		}
		else if (flag == FALSE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "0");
		}
		Mead_Main_Win->OnBnClickedButton17();
	}
	return 0;
}
void IMEIWrite_MulAT::OnBnClickedButton16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 2;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	StopSign[ArraySign] = FALSE;
	StartC_Control.EnableWindow(FALSE);
	Thread_Handle[ArraySign] = AfxBeginThread(WriteIMEIFunction3, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	
	StartButtonGatherFun(ArraySign);


	////������������ͷSOCKET
	//if (GetPicChoose == TRUE)
	//{
	//	SocketServer[ArraySign]->StartServer(SocketServerPort[ArraySign]);
	//}

	////��ʼ��Dongle����
	//if (GetDongleCheckValue == TRUE)
	//{
	//	if (DongleConnectCount[ArraySign] == DongleResetCount || DongleConnectCount[ArraySign] == 0)
	//	{
	//		DongleConnectCount[ArraySign] = 0;
	//		DongleInit(ArraySign);
	//	}
	//}

}

void IMEIWrite_MulAT::OnBnClickedButton17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 2;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	int HandleNum = 2;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[ArraySign] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[ArraySign] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[2]=FALSE;
	LogShow_exchange(&m_Result3, &Final_Result_Control3, 250, "�������ԣ���", 2);

	StopButtonGatherFun(ArraySign);

	////ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	//if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[ArraySign] == 1)
	//{
	//	BleGetSettingFlag[ArraySign] = 0;
	//	BluetoothDisconnect(ArraySign);
	//}

	//if (0)
	//{
	//	BOOL Return = CloseHandle(hPort[ArraySign]);				//���Գɹ���
	//	if (Return == TRUE)
	//	{
	//	}
	//	else
	//	{
	//		AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
	//		return;
	//	}
	//	hPort[ArraySign] = NULL;
	//}

	////ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	//if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	//{
	//	DeleteComoOccupancydDeq(ArraySign);
	//}

	////�ر�Socket������
	//SocketServer[ArraySign]->CloseRecv();
	//SocketServer[ArraySign]->StopServer();

	////�ر�Dongle����
	//if (GetDongleCheckValue == TRUE&&DongleInfo[ArraySign][0] != "")
	//{
	//	DongleDisConnect(ArraySign);
	//}
}

void IMEIWrite_MulAT::OnEnChangeImeia4()
{
	//if(NETCODE_Choose_M=="true")
	{
		UpdateData(TRUE);
		UpdateData(false);//�ѱ�����ʾ�ڽ���

		int IMEI_length;
		IMEI_Input[3] = "";
		if (((IMEI_InputD.Find("\r") != -1) || (IMEI_InputD.Find("\n") != -1)))//ѡ����IMEI1  &&IMEI_length==15
		{
			EnableWindow_Choose(-1);
			//check whether digits
			IMEI_InputD.Replace("\r", "");
			IMEI_InputD.Replace("\n", "");
			IMEI_length = IMEI_InputD.GetLength();
			////////////////////�������ж�///////////////////
			//check whether digits
			if ((IMEI_length == 0))
			{
				EnableWindow_Choose(3);
				UpdateData(FALSE);//������Ļس����
				return;
			}
			IMEI_InputD_Control.SetWindowTextA(IMEI_InputD);
			for (int i = 0; i<IMEI_length; i++)
			{
				if (IMEI_length != 15 || (IMEI_InputD.GetAt(i) <'0' || IMEI_InputD.GetAt(i) >'9'))
				{
					EnableWindow_Choose(3);
					LogShow_exchange(&m_Result4, &Final_Result_Control4, 128, "Please check IMEI_InputD. Only 15 digits allowed!\r\n", 3);
					IMEI_InputD_Control.SetFocus();
					IMEI_InputD_Control.SetSel(0, -1);
					return;
				}

			}
			if (CheckIMEI(IMEI_InputD) == FALSE)
			{
				EnableWindow_Choose(3);
				LogShow_exchange(&m_Result4, &Final_Result_Control4, 128, "IMEI_InputD ���Ϸ�", 3);
				IMEI_InputD_Control.SetFocus();
				IMEI_InputD_Control.SetSel(0, -1);
				return;
			}
			////////////////////�������ж�///////////////////
			IMEI_Input[3] = IMEI_InputD;
			LogShow_exchange(&m_Result4, &Final_Result_Control4, 0, "ɨ������IMEI:" + IMEI_Input[3] + "�ɹ�...\r\n", 3);

			OnBnClickedButton18();
		}
	}
}


UINT static __cdecl WriteIMEIFunction4(LPVOID pParam)
{
	BOOL flag;

	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	flag = Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port4), &(Mead_Main_Win->m_Baud4), 3, &(Mead_Main_Win->m_Result4), &(Mead_Main_Win->Final_Result_Control4), &(Mead_Main_Win->IMEI_InputD_Control));


	//��ADCT���س����Ͳ��Խ��
	if (g_ADCTFlag == 1)
	{
		//����Ǳ�ǿ��ֹͣ�ˣ��Ǿ�ֱ�ӷ��أ���Ҫ�����س�����Ϣ
		if (Mead_Main_Win->StopSign[3] == TRUE)
		{
			return 0;
		}

		CString str_Port;
		Mead_Main_Win->GetDlgItemText(IDC_COMBO8, str_Port);
		if (flag == TRUE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "1");
		}
		else if (flag == FALSE)
		{
			Mead_Main_Win->SendADCTTestResult(str_Port, "0");
		}
		Mead_Main_Win->OnBnClickedButton19();
	}
	return 0;
}

void IMEIWrite_MulAT::OnBnClickedButton18()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 3;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	StopSign[ArraySign] = FALSE;
	StartD_Control.EnableWindow(FALSE);
	Thread_Handle[ArraySign] = AfxBeginThread(WriteIMEIFunction4, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	StartButtonGatherFun(ArraySign);

	////������������ͷSOCKET
	//if (GetPicChoose == TRUE)
	//{
	//	SocketServer[ArraySign]->StartServer(SocketServerPort[ArraySign]);
	//}

	////��ʼ��Dongle����
	//if (GetDongleCheckValue == TRUE)
	//{
	//	if (DongleConnectCount[ArraySign] == DongleResetCount || DongleConnectCount[ArraySign] == 0)
	//	{
	//		DongleConnectCount[ArraySign] = 0;
	//		DongleInit(ArraySign);
	//	}
	//}
}

void IMEIWrite_MulAT::OnBnClickedButton19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 3;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	int HandleNum = 3;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[ArraySign] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[ArraySign] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[3]=FALSE;
	LogShow_exchange(&m_Result4, &Final_Result_Control4, 250, "�������ԣ���", 3);

	StopButtonGatherFun(ArraySign);

	////ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	//if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[ArraySign] == 1)
	//{
	//	BleGetSettingFlag[ArraySign] = 0;
	//	BluetoothDisconnect(ArraySign);
	//}

	//if (0)
	//{
	//	BOOL Return = CloseHandle(hPort[ArraySign]);				//���Գɹ���
	//	if (Return == TRUE)
	//	{
	//	}
	//	else
	//	{
	//		AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
	//		return;
	//	}
	//	hPort[ArraySign] = NULL;
	//}

	////ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	//if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	//{
	//	DeleteComoOccupancydDeq(ArraySign);
	//}

	////�ر�Socket������
	//SocketServer[ArraySign]->CloseRecv();
	//SocketServer[ArraySign]->StopServer();

	////�ر�Dongle����
	//if (GetDongleCheckValue == TRUE&&DongleInfo[ArraySign][0] != "")
	//{
	//	DongleDisConnect(ArraySign);
	//}
}

void IMEIWrite_MulAT::OnEnChangeImeia5()
{
	//if(NETCODE_Choose_M=="true")
	{
		UpdateData(TRUE);
		UpdateData(false);//�ѱ�����ʾ�ڽ���

		int IMEI_length;
		IMEI_Input[4] = "";
		if (((IMEI_InputE.Find("\r") != -1) || (IMEI_InputE.Find("\n") != -1)))//ѡ����IMEI1  &&IMEI_length==15
		{
			EnableWindow_Choose(-1);
			//check whether digits
			IMEI_InputE.Replace("\r", "");
			IMEI_InputE.Replace("\n", "");
			IMEI_length = IMEI_InputE.GetLength();
			////////////////////�������ж�///////////////////
			//check whether digits
			if ((IMEI_length == 0))
			{
				EnableWindow_Choose(4);
				UpdateData(FALSE);//������Ļس����
				return;
			}
			IMEI_InputE_Control.SetWindowTextA(IMEI_InputE);
			for (int i = 0; i<IMEI_length; i++)
			{
				if (IMEI_length != 15 || (IMEI_InputE.GetAt(i) <'0' || IMEI_InputE.GetAt(i) >'9'))
				{
					EnableWindow_Choose(4);
					LogShow_exchange(&m_Result5, &Final_Result_Control5, 128, "Please check IMEI_InputE. Only 15 digits allowed!\r\n", 4);
					IMEI_InputE_Control.SetFocus();
					IMEI_InputE_Control.SetSel(0, -1);
					return;
				}

			}
			if (CheckIMEI(IMEI_InputD) == FALSE)
			{
				EnableWindow_Choose(4);
				LogShow_exchange(&m_Result5, &Final_Result_Control5, 128, "IMEI_InputE ���Ϸ�", 4);
				IMEI_InputE_Control.SetFocus();
				IMEI_InputE_Control.SetSel(0, -1);
				return;
			}
			////////////////////�������ж�///////////////////
			IMEI_Input[4] = IMEI_InputE;
			LogShow_exchange(&m_Result5, &Final_Result_Control5, 0, "ɨ������IMEI:" + IMEI_Input[4] + "�ɹ�...\r\n", 4);

			OnBnClickedButton20();
		}
	}
}


UINT static __cdecl WriteIMEIFunction5(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port5), &(Mead_Main_Win->m_Baud5), 4, &(Mead_Main_Win->m_Result5), &(Mead_Main_Win->Final_Result_Control5), &(Mead_Main_Win->IMEI_InputE_Control));
	return 0;
}

void IMEIWrite_MulAT::OnBnClickedButton20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 4;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	StopSign[ArraySign] = FALSE;
	StartE_Control.EnableWindow(FALSE);
	Thread_Handle[ArraySign] = AfxBeginThread(WriteIMEIFunction5, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	StartButtonGatherFun(ArraySign);

	////������������ͷSOCKET
	//if (GetPicChoose == TRUE)
	//{
	//	SocketServer[ArraySign]->StartServer(SocketServerPort[ArraySign]);
	//}

	////��ʼ��Dongle����
	//if (GetDongleCheckValue == TRUE)
	//{
	//	if (DongleConnectCount[ArraySign] == DongleResetCount || DongleConnectCount[ArraySign] == 0)
	//	{
	//		DongleConnectCount[ArraySign] = 0;
	//		DongleInit(ArraySign);
	//	}
	//}
}

void IMEIWrite_MulAT::OnBnClickedButton21()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 4;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	int HandleNum = 4;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[ArraySign] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[ArraySign] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[4]=FALSE;
	LogShow_exchange(&m_Result5, &Final_Result_Control5, 250, "�������ԣ���", 4);

	StopButtonGatherFun(ArraySign);

	////ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	//if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[ArraySign] == 1)
	//{
	//	BleGetSettingFlag[ArraySign] = 0;
	//	BluetoothDisconnect(ArraySign);
	//}

	//if (0)
	//{
	//	BOOL Return = CloseHandle(hPort[ArraySign]);				//���Գɹ���
	//	if (Return == TRUE)
	//	{
	//	}
	//	else
	//	{
	//		AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
	//		return;
	//	}
	//	hPort[ArraySign] = NULL;
	//}

	////ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	//if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	//{
	//	DeleteComoOccupancydDeq(ArraySign);
	//}

	////�ر�Socket������
	//SocketServer[ArraySign]->CloseRecv();
	//SocketServer[ArraySign]->StopServer();

	////�ر�Dongle����
	//if (GetDongleCheckValue == TRUE&&DongleInfo[ArraySign][0] != "")
	//{
	//	DongleDisConnect(ArraySign);
	//}
}

void IMEIWrite_MulAT::OnEnChangeImeia6()
{
	//if(NETCODE_Choose_M=="true")
	{
		UpdateData(TRUE);
		UpdateData(false);//�ѱ�����ʾ�ڽ���

		int IMEI_length;
		IMEI_Input[5] = "";
		if (((IMEI_InputF.Find("\r") != -1) || (IMEI_InputF.Find("\n") != -1)))//ѡ����IMEI1  &&IMEI_length==15
		{
			EnableWindow_Choose(-1);
			//check whether digits
			IMEI_InputF.Replace("\r", "");
			IMEI_InputF.Replace("\n", "");
			IMEI_length = IMEI_InputF.GetLength();
			////////////////////�������ж�///////////////////
			//check whether digits
			if ((IMEI_length == 0))
			{
				EnableWindow_Choose(5);
				UpdateData(FALSE);//������Ļس����
				return;
			}
			IMEI_InputF_Control.SetWindowTextA(IMEI_InputF);
			for (int i = 0; i<IMEI_length; i++)
			{
				if (IMEI_length != 15 || (IMEI_InputF.GetAt(i) <'0' || IMEI_InputF.GetAt(i) >'9'))
				{
					EnableWindow_Choose(5);
					LogShow_exchange(&m_Result6, &Final_Result_Control6, 128, "Please check IMEI_InputF. Only 15 digits allowed!\r\n", 5);
					IMEI_InputF_Control.SetFocus();
					IMEI_InputF_Control.SetSel(0, -1);
					return;
				}

			}
			if (CheckIMEI(IMEI_InputD) == FALSE)
			{
				EnableWindow_Choose(5);
				LogShow_exchange(&m_Result6, &Final_Result_Control6, 128, "IMEI_InputF ���Ϸ�", 5);
				IMEI_InputF_Control.SetFocus();
				IMEI_InputF_Control.SetSel(0, -1);
				return;
			}
			////////////////////�������ж�///////////////////
			IMEI_Input[5] = IMEI_InputF;
			LogShow_exchange(&m_Result6, &Final_Result_Control6, 0, "ɨ������IMEI:" + IMEI_Input[5] + "�ɹ�...\r\n", 5);

			OnBnClickedButton22();
		}
	}
}

UINT static __cdecl WriteIMEIFunction6(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port6), &(Mead_Main_Win->m_Baud6), 5, &(Mead_Main_Win->m_Result6), &(Mead_Main_Win->Final_Result_Control6), &(Mead_Main_Win->IMEI_InputF_Control));
	return 0;
}

void IMEIWrite_MulAT::OnBnClickedButton22()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopSign[5] = FALSE;
	StartF_Control.EnableWindow(FALSE);
	Thread_Handle[5] = AfxBeginThread(WriteIMEIFunction6, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

void IMEIWrite_MulAT::OnBnClickedButton23()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 5;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[5] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[5] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[5]=FALSE;
	LogShow_exchange(&m_Result6, &Final_Result_Control6, 250, "�������ԣ���", 5);

	int PortHandle = 5;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(5);
	}
}

void IMEIWrite_MulAT::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLogin2 dig2;
	dig2.LANDING_OK = FALSE;
	dig2.DoModal();
	if (dig2.LANDING_OK == TRUE)
	{
		EnableWindow_ALL(TRUE);
	}
}

void IMEIWrite_MulAT::OnBnClickedButton7()
{
	//ShowPassWord("0F 88 BE 55 21 D4 DB ED D3 75 13 46 67 28 BF 63",&m_Result1,&Final_Result_Control1,0);
	//ShowPassWord("0F88BE5521D4DBEDD37513466728BF63",&m_Result1,&Final_Result_Control1,0);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EnableWindow_ALL(FALSE);
	IMEI_InputA_Control.SetWindowTextA("");
	IMEI_InputB_Control.SetWindowTextA("");
	IMEI_InputC_Control.SetWindowTextA("");
	IMEI_InputD_Control.SetWindowTextA("");
	IMEI_InputE_Control.SetWindowTextA("");
	IMEI_InputF_Control.SetWindowTextA("");

	CString ScanGunTpyeCS;
	ScanGunTpye.GetWindowTextA(ScanGunTpyeCS);
	EnableWindow_Choose(-1);
	SaveConfig_Change();
}



//�����������Կ�ʼ���ӵ��̺߳���
UINT static __cdecl TH_BleStartConnect_Control(LPVOID pParam)
{
	CString StrMac;
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;

	Mead_Main_Win->GetDlgItemText(IDC_SCANDATA_EDIT, StrMac);
	Mead_Main_Win->BluetoothCoreFunction(StrMac);

	return 0;
}

/*ɨ��ǹģ��*/

BOOL IMEIWrite_MulAT::OPen_ScanGun_Port(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum)		//��ɨ��ǹ�˿�
{
	CString sPort, sBaud;
	int port, baud;
	//get port
	m_Port->GetWindowText(sPort);
	sPort = sPort.Right(sPort.GetLength() - 3);
	port = atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"), port);

	//get baud
	m_Baud->GetWindowText(sBaud);
	baud = atoi(sBaud);

	//open com port�򿪶˿�
	hScanGun[HandleNum] = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hScanGun[HandleNum] == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port->GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hScanGun[HandleNum] = NULL;
		return FALSE;
	}

	//config the com port
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hScanGun[HandleNum], &dcb);
	SetupComm(hScanGun[HandleNum], 4096, 4096);
	PurgeComm(hScanGun[HandleNum], PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
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
	timeouts.ReadTotalTimeoutConstant = 0;//OPen_ScanGun_Port
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hScanGun[HandleNum], &timeouts);

	//SetCommMask(hScanGun[HandleNum], EV_RXCHAR|EV_TXEMPTY );							//�����¼�����������

	//read and the com port
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char buffer[256];

	Sleep(100);
	bReadStatus = ReadFile(hScanGun[HandleNum], buffer, 256, &dwBytesRead, NULL);		//make sure rx data cleaned

	return TRUE;																		//����TRUE���򿪴�������
}


BOOL IMEIWrite_MulAT::Get_ScanGun_Data(CEdit* m_Result, CEdit* Final_Result_Control, int HandleNum, char* EndSign)		//��ȡɨ��ǹ�˿�����
{
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char *buffer;
	char *p = NULL;
	char buf[1024];

	if (DongleTestDataDeq.size() != 0&&GetDongleScanCheckValue==TRUE)
	{
		CString ShowNumberStr;
		m_ShowNumberPortControlArray[DongleTestDataDeq.front()]->GetWindowText(ShowNumberStr);
		if (ShowNumberStr == "")
		{
			Final_Result_ControlArray[HandleNum]->SetFont(font);
			gColor[HandleNum] = RGB(0, 155, 100);
			Final_Result_ControlArray[DongleTestDataDeq.front()]->SetWindowTextA("��ɨ��");
		}
	}
	else if (GetDongleScanCheckValue == TRUE)
	{
		return TRUE;
	}

	//read and the com port
	CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���
	for (int r = 0; r<2; r++)
	{
		memset(buf, 0, sizeof(buf));
		buffer = buf;
		bReadStatus = ReadFile(hScanGun[HandleNum], buffer, 512, &dwBytesRead, NULL);
		if (dwBytesRead != 0)
		{
			if (strstr(EndSign, "NULL"))
				p = strstr(buffer, "\r");
			else
				p = strstr(buffer, EndSign);
			if (p)
			{
				CString strCommand_Vaule_Return, MAC;//���ڷ���ֵ
				strCommand_Vaule_Return = Port_Temp + CString(buffer);

				CString  selPort;
				selPort = strCommand_Vaule_Return;
				selPort.Replace("\r", "");
				selPort.Replace("\n", "");
				//��ȡ�����ĸ���
				CString string_sn_r;
				int npos = selPort.Find('"');
				if (npos >= 0)
				{
					string_sn_r = selPort.Mid(npos + 1);
					Vaule_ScanGun[HandleNum] = string_sn_r.Left(string_sn_r.Find('"'));
				}
				else
				{
					Vaule_ScanGun[HandleNum] = selPort;
				}


				//GetBluetoothCheckValueָ�Ƿ�ѡ����,MacScanEnbleFlagָ�Ƿ��ɨ��MAC��ַ

				if (GetBluetoothCheckValue == TRUE)
				{
					SetDlgItemText(IDC_SCANDATA_EDIT, Vaule_ScanGun[HandleNum].Right(MacIntercept));
					if (MacScanEnbleFlag == FALSE)
					{
						Vaule_ScanGun[HandleNum] = "";
						return TRUE;
					}
					else if (MacScanEnbleFlag == TRUE&&BLEGetSettingFlag == 1)
					{
						AfxBeginThread(TH_BleStartConnect_Control, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
					}
				}
				

				//if (GetBluetoothCheckValue == TRUE&&MacScanEnbleFlag == FALSE)
				//{
				//	Vaule_ScanGun[HandleNum] = "";
				//	return TRUE;
				//}
				//else if (GetBluetoothCheckValue == TRUE&&MacScanEnbleFlag == TRUE&&BLEGetSettingFlag == 1)
				//{
				//	AfxBeginThread(TH_BleStartConnect_Control, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

				//	//GetDlgItemText(IDC_SCANDATA_EDIT, MAC);
				//	//BluetoothCoreFunction(MAC);
				//}

				if (GetDongleScanCheckValue == TRUE)
				{
					if (Software_Version[0] == "")
					{
						//SetDlgItemText(IDC_SCANDATA_EDIT, Vaule_ScanGun[HandleNum]);
						Vaule_ScanGun[HandleNum] = "";
						return TRUE;
					}
					else if (Software_Version[0] != "")
					{
						if (DongleTestDataDeq.size() == 0)
						{
							GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("�޴��ڿɷ���");
							Vaule_ScanGun[HandleNum] = "";
							return TRUE;
						}

						CString ScanGunDataStr;
						GetDlgItem(IDC_SCANDATA_EDIT)->GetWindowTextA(ScanGunDataStr);

						if (ScanGunDataStr == "" || ScanGunDataStr == "�޴��ڿɷ���" || ScanGunDataStr == "MAC��ַ�Ѿ�ɨ��" || ScanGunDataStr == "ɨ���ٶȹ��죬����ɨ")
						{
							if (Vaule_ScanGun[HandleNum] != "")
							{
								for (int z = 0; z < 8; z++)
								{
									if (DongleTestRssiInfoArray[z] == Vaule_ScanGun[HandleNum])
									{
										GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("MAC��ַ�Ѿ�ɨ��");
										Vaule_ScanGun[HandleNum] = "";
										return TRUE;
									}
								}
								if (Vaule_ScanGun[HandleNum].GetLength() > 12)
								{
									Vaule_ScanGun[HandleNum] = Vaule_ScanGun[HandleNum].Left(12);
								}
								SetDlgItemText(IDC_SCANDATA_EDIT, Vaule_ScanGun[HandleNum]);
								DongleScanGunFlag = TRUE;
							}
							else if (Vaule_ScanGun[HandleNum] == "")
							{
								return TRUE;
							}
						}
						else if (ScanGunDataStr != "")
						{
							GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("ɨ���ٶȹ��죬����ɨ");
							Vaule_ScanGun[HandleNum] = "";
							return TRUE;
						}
					}
				}
				else if (GetDongleCheckValue == TRUE)
				{
					if (Software_Version[0] == "")
					{
						SetDlgItemText(IDC_SCANDATA_EDIT, Vaule_ScanGun[HandleNum]);
						Vaule_ScanGun[HandleNum] = "";
						return TRUE;
					}
					else if (Software_Version[0] != "")
					{
						if (DongleTestDataDeq.size() == 0)
						{
							GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("�޴��ڿɷ���");
							Vaule_ScanGun[HandleNum] = "";
							return TRUE;
						}

						CString ScanGunDataStr;
						GetDlgItem(IDC_SCANDATA_EDIT)->GetWindowTextA(ScanGunDataStr);

						if (ScanGunDataStr == "" || ScanGunDataStr == "�޴��ڿɷ���" || ScanGunDataStr == "MAC��ַ�Ѿ�ɨ��" || ScanGunDataStr == "ɨ���ٶȹ��죬����ɨ")
						{
							if (Vaule_ScanGun[HandleNum] != "")
							{
								for (int z = 0; z < 16; z++)
								{
									if (DongleInfo[z][0] == Vaule_ScanGun[HandleNum])
									{
										GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("MAC��ַ�Ѿ�ɨ��");
										Vaule_ScanGun[HandleNum] = "";
										return TRUE;
									}
								}
								if (Vaule_ScanGun[HandleNum].GetLength() > 12)
								{
									Vaule_ScanGun[HandleNum] = Vaule_ScanGun[HandleNum].Left(12);
								}
								SetDlgItemText(IDC_SCANDATA_EDIT, Vaule_ScanGun[HandleNum]);
								DongleScanGunFlag = TRUE;
							}
							else if (Vaule_ScanGun[HandleNum] == "")
							{
								return TRUE;
							}
						}
						else if (ScanGunDataStr != "")
						{
							GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("ɨ���ٶȹ��죬����ɨ");
							Vaule_ScanGun[HandleNum] = "";
							return TRUE;
						}
					}
				}

				//LogShow_exchange(m_Result,Final_Result_Control,5,"�ɹ���ȡɨ����\r\n"+Vaule_ScanGun[HandleNum],HandleNum);
				return TRUE;
			}
			else
			{
					Port_Temp += CString(buffer);
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
	return FALSE;
}


BOOL IMEIWrite_MulAT::Check_ScanGun_Data(CEdit* m_Result, CEdit* Final_Result_Control, int HandleNum, CEdit* Data_Input_Control, BOOL ImeiChoose, int LengthLimit)
{
	if (ImeiChoose == TRUE)
	{
		int IMEI_length;
		CString ScanGunData;
		ScanGunData = Vaule_ScanGun[HandleNum];
		IMEI_length = ScanGunData.GetLength();
		for (int i = 0; i<IMEI_length; i++)
		{
			if (IMEI_length != 15 || (ScanGunData.GetAt(i) <'0' || ScanGunData.GetAt(i) >'9'))
			{
				return FALSE;
			}

		}
		if (CheckIMEI(ScanGunData) == FALSE)
		{
			return FALSE;
		}
		//Data_Input_Control->SetWindowTextA(ScanGunData);
		//IMEI_Input[HandleNum]=ScanGunData;
		//LogShow_exchange(m_Result,Final_Result_Control,0,"ɨ������IMEI:"+ScanGunData+"����...\r\n",HandleNum);
		PublicIMEI = ScanGunData;
	}
	else
	{
		int IMEI_length;
		CString ScanGunData;
		ScanGunData = Vaule_ScanGun[HandleNum];
		IMEI_length = ScanGunData.GetLength();
		if (IMEI_length != LengthLimit)
		{
			return FALSE;
		}
		////////////////////�������ж�///////////////////
		//Data_Input_Control->SetWindowTextA(ScanGunData);
		OtherData_Input[HandleNum] = ScanGunData;
		//LogShow_exchange(m_Result,Final_Result_Control,0,"ɨ����������:"+OtherData_Input[HandleNum]+"λ������...\r\n",HandleNum);
	}
	return TRUE;
}
void IMEIWrite_MulAT::OnBnClickedButton3()//��ɨ��ǹ1
{
	if (OPen_ScanGun_Port(&ScanGunPort1, &ScanGunBaud1, 0) == FALSE)
	{
		AfxMessageBox("��ɨ��ǹ�˿�ʧ�ܣ�");
		return;
	}
	else
	{
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	}

	SetTimer(30, 1000, NULL);

	//���û�й�ѡ������ȥ�ж�IMEI
	if (GetBluetoothCheckValue == FALSE && GetDongleCheckValue==FALSE&&GetDongleScanCheckValue==FALSE)
	{
		CompareIMEI = TRUE;
	}
	else if (GetBluetoothCheckValue == TRUE || GetDongleCheckValue == TRUE)
	{
		AfxMessageBox("��ȡ����ǰ����ɨ��MAC��ַ��Mac��ַɨ�����ʱֻ�е�һ����Ч��");
	}
	else if (GetDongleScanCheckValue == TRUE)
	{
		AfxMessageBox("���ȴ򿪴��⴮�ڣ�ɨ��MAC��ַ�������Ϻ󣬲ſ���ɨ����һ��MAC��ַ");
	}

}

void IMEIWrite_MulAT::OnBnClickedButton25()
{
	KillTimer(30);
	if (hScanGun[0] != NULL)
	{
		BOOL Return = CloseHandle(hScanGun[0]);				//���Գɹ���
		if (Return == TRUE)
			LogShow_exchange(&m_Result1, &Final_Result_Control1, 250, "�رոô��ڳɹ�����", 0);
		else
		{
			LogShow_exchange(&m_Result1, &Final_Result_Control1, 128, "�رոô���ʧ�ܣ���", 0);
			//�رմ���ʧ��
		}
		hScanGun[0] = NULL;
	}
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	CompareIMEI = FALSE;
}


//ɨ��ǹ2
void IMEIWrite_MulAT::OnBnClickedButton26()
{

}

void IMEIWrite_MulAT::OnBnClickedButton27()
{

}

//ɨ��ǹ3
void IMEIWrite_MulAT::OnBnClickedButton28()
{

}

void IMEIWrite_MulAT::OnBnClickedButton29()
{

}

//ɨ��ǹ4
void IMEIWrite_MulAT::OnBnClickedButton30()
{

}

void IMEIWrite_MulAT::OnBnClickedButton31()
{

}

//ɨ��ǹ5
void IMEIWrite_MulAT::OnBnClickedButton32()
{

}

void IMEIWrite_MulAT::OnBnClickedButton33()
{

}
//ɨ��ǹ6
void IMEIWrite_MulAT::OnBnClickedButton34()
{

}

void IMEIWrite_MulAT::OnBnClickedButton35()
{

}

void IMEIWrite_MulAT::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PrintInt.DoModal();
}

void IMEIWrite_MulAT::OnBnClickedButton37()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PrintInt.PrintFunIni();
	GetDlgItem(IDC_BUTTON37)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON38)->EnableWindow(TRUE);
}

void IMEIWrite_MulAT::OnBnClickedButton38()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PrintInt.PrintFunEnd();
	GetDlgItem(IDC_BUTTON37)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON38)->EnableWindow(FALSE);
}

void IMEIWrite_MulAT::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	for (int i = 0; i<THREAD_NUM; i++)
	{
		StopSign[i] = FALSE;
	}
	StopAutoStart = FALSE;
	if (Thread_State[0] != TRUE)
		OnBnClickedButton1();
	if (Thread_State[1] != TRUE)
		OnBnClickedButton14();
	if (Thread_State[2] != TRUE)
		OnBnClickedButton16();
	if (Thread_State[3] != TRUE)
		OnBnClickedButton18();
	if (Thread_State[4] != TRUE)
		OnBnClickedButton20();
	if (Thread_State[5] != TRUE)
		OnBnClickedButton22();

	if (Thread_State[6] != TRUE)
		OnBnClickedButtonstart7();
	if (Thread_State[7] != TRUE)
		OnBnClickedButtonstart8();
	if (Thread_State[8] != TRUE)
		OnBnClickedButtonstart9();
	if (Thread_State[9] != TRUE)
		OnBnClickedButtonstart10();
	if (Thread_State[10] != TRUE)
		OnBnClickedButtonstart11();
	if (Thread_State[11] != TRUE)
		OnBnClickedButtonstart12();
	if (Thread_State[12] != TRUE)
		OnBnClickedButtonstart13();
	if (Thread_State[13] != TRUE)
		OnBnClickedButtonstart14();
	if (Thread_State[14] != TRUE)
		OnBnClickedButtonstart15();
	if (Thread_State[15] != TRUE)
		OnBnClickedButtonstart16();
}

void IMEIWrite_MulAT::OnBnClickedCancel()//ֹͣ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnCancel();
	for (int i = 0; i<THREAD_NUM; i++)
	{
		StopSign[i] = TRUE;
	}
	StopAutoStart = TRUE;

	if (Thread_State[0] == TRUE)OnBnClickedButton2();
	if (Thread_State[1] == TRUE)OnBnClickedButton15();
	if (Thread_State[2] == TRUE)OnBnClickedButton17();
	if (Thread_State[3] == TRUE)OnBnClickedButton19();
	if (Thread_State[4] == TRUE)OnBnClickedButton21();
	if (Thread_State[5] == TRUE)OnBnClickedButton23();

	if (Thread_State[6] == TRUE)OnBnClickedButtonstop7();
	if (Thread_State[7] == TRUE)OnBnClickedButtonstop8();
	if (Thread_State[8] == TRUE)OnBnClickedButtonstop9();
	if (Thread_State[9] == TRUE)OnBnClickedButtonstop10();
	if (Thread_State[10] == TRUE)OnBnClickedButtonstop11();
	if (Thread_State[11] == TRUE)OnBnClickedButtonstop12();
	if (Thread_State[12] == TRUE)OnBnClickedButtonstop13();
	if (Thread_State[13] == TRUE)OnBnClickedButtonstop14();
	if (Thread_State[14] == TRUE)OnBnClickedButtonstop15();
	if (Thread_State[15] == TRUE)OnBnClickedButtonstop16();

	if ((Thread_State[0] == TRUE) || (Thread_State[1] == TRUE) || (Thread_State[2] == TRUE) || (Thread_State[3] == TRUE) || (Thread_State[4] == TRUE) || (Thread_State[5] == TRUE) || \
		(Thread_State[6] == TRUE) || (Thread_State[7] == TRUE) || (Thread_State[8] == TRUE) || (Thread_State[9] == TRUE) || (Thread_State[10] == TRUE) || (Thread_State[11] == TRUE) || \
		(Thread_State[12] == TRUE) || (Thread_State[13] == TRUE) || (Thread_State[14] == TRUE) || (Thread_State[15] == TRUE))
	{
		AfxMessageBox("���ȵȴ��߳̽�����");
		return;
	}
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void IMEIWrite_MulAT::OnBnClickedButton40()//ֹͣ�Զ���ʼ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopAutoStart = TRUE;
}

void IMEIWrite_MulAT::OnBnClickedButton42()//�����Զ���ʼ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	StopAutoStart = FALSE;
}


void IMEIWrite_MulAT::WriteLogIni(int LogNum, CString ChipRfIDbg)
{
	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[LogNum];
	}
	CTime cTime;
	char m_Time_Date[128];
	char m_Time_Time[128];
	CString m_strModulePath;
	CString LogFile;

	char cReportPath[256] = { 0 };
	DWORD dwAttr;

	char file_name[_MAX_PATH] = { 0 };
	GetModuleFileName(NULL, file_name, _MAX_PATH);
	CString strFileName = file_name;
	int num = strFileName.ReverseFind('\\');
	CString LogNum_CS;
	LogNum_CS.Format("%d", LogNum);
	//LogNum_CS="0";
	m_strModulePath = strFileName.Left(num + 1) + LogNum_CS + "\\";

	cTime = CTime::GetCurrentTime();                                                         //�����Ե������ڵ��ļ�������������
	sprintf(m_Time_Date, "%s%04d%02d%02d", m_strModulePath, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());//��������6λ����һ��Char��
	//����������Ϊ���ֵ��ļ���
	dwAttr = GetFileAttributes(m_Time_Date);
	if (dwAttr == 0xFFFFFFFF)                                                                 //�ļ��в�����   
	{
		CreateDirectory(m_Time_Date, NULL);
	}

	sprintf(m_Time_Time, "%s-%02d%02d%02d", ChipRfIDCurrent, cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	//sprintf(cReportPath, "%s%04d%02d%02d/%s @@%s.txt",m_strModulePath,cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),IMEIRecord[LogNum],m_Time_Time);
	//sprintf(m_Time_Time, "%d-Log",LogNum);
	sprintf(cReportPath, "%s%04d%02d%02d/%s.txt", m_strModulePath, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), m_Time_Time);
	cReport[LogNum].Open(cReportPath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	//try{
	//	CFileException e;
	//	BOOL FileFlag = cReport[LogNum].Open(cReportPath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate, &e);
	//	AfxMessageBox(e.ReportError());
	//}
	//catch (CFileException &e)
	//{
	//	AfxMessageBox(e.ReportError());/*��ӡ���쳣ԭ��*/
	//}
	//CFile::modeCreate �ļ������ڣ����½������ڲ���û��CFile::modeNoTruncate�����ԭ�ļ�����CFile::modeNoTruncate�������ԭ�ļ������ѳ���ɾ��Ϊ0��
	/*if(!cReport[LogNum].Open(cReportPath,CFile::modeReadWrite))
	{
	cReport[LogNum].Open(cReportPath,CFile::modeCreate|CFile::modeReadWrite);//ԭ�ļ������ڣ����½�
	}*/
}
void IMEIWrite_MulAT::WriteLog(CString Text, int	  LogNum)
{
	cReport[LogNum].SeekToEnd();											//cReport[LogNum].Seek(0,CFile::end);
	cReport[LogNum].Write((LPSTR)(LPCSTR)Text, Text.GetLength());			//strlen(Lreport)
	cReport[LogNum].Write("\r\n", 2);
	//char ReadData[256]={0};
	//cReport[LogNum].SeekToBegin();
	//cReport[LogNum].Read(ReadData,256);
}

void IMEIWrite_MulAT::WriteLogEnd(int LogNum)
{
	cReport[LogNum].Close();
}


UINT static __cdecl WriteIMEIFunction7(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port7), &(Mead_Main_Win->m_Baud7), 6, &(Mead_Main_Win->m_Result7), &(Mead_Main_Win->Final_Result_Control7), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction8(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port8), &(Mead_Main_Win->m_Baud8), 7, &(Mead_Main_Win->m_Result8), &(Mead_Main_Win->Final_Result_Control8), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction9(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port9), &(Mead_Main_Win->m_Baud9), 8, &(Mead_Main_Win->m_Result9), &(Mead_Main_Win->Final_Result_Control9), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction10(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port10), &(Mead_Main_Win->m_Baud10), 9, &(Mead_Main_Win->m_Result10), &(Mead_Main_Win->Final_Result_Control10), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction11(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port11), &(Mead_Main_Win->m_Baud11), 10, &(Mead_Main_Win->m_Result11), &(Mead_Main_Win->Final_Result_Control11), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction12(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port12), &(Mead_Main_Win->m_Baud12), 11, &(Mead_Main_Win->m_Result12), &(Mead_Main_Win->Final_Result_Control12), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction13(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port13), &(Mead_Main_Win->m_Baud13), 12, &(Mead_Main_Win->m_Result13), &(Mead_Main_Win->Final_Result_Control13), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction14(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port14), &(Mead_Main_Win->m_Baud14), 13, &(Mead_Main_Win->m_Result14), &(Mead_Main_Win->Final_Result_Control14), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction15(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port15), &(Mead_Main_Win->m_Baud15), 14, &(Mead_Main_Win->m_Result15), &(Mead_Main_Win->Final_Result_Control15), NULL);
	return 0;
}
UINT static __cdecl WriteIMEIFunction16(LPVOID pParam)
{
	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->WriteIMEIFunction_Thread(&(Mead_Main_Win->m_Port16), &(Mead_Main_Win->m_Baud16), 15, &(Mead_Main_Win->m_Result16), &(Mead_Main_Win->Final_Result_Control16), NULL);
	return 0;
}
void IMEIWrite_MulAT::OnBnClickedButtonstart7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopSign[6] = FALSE;
	Start7_Control.EnableWindow(FALSE);
	Thread_Handle[6] = AfxBeginThread(WriteIMEIFunction7, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

void IMEIWrite_MulAT::OnBnClickedButtonstart8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopSign[7] = FALSE;
	Start8_Control.EnableWindow(FALSE);
	Thread_Handle[7] = AfxBeginThread(WriteIMEIFunction8, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

void IMEIWrite_MulAT::OnBnClickedButtonstart9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 8;
	StopSign[8] = FALSE;
	Start9_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)  //�������Ϊ����ͷ֮���Ʒ��ʱ��9-16��ֻ��Ϊͼ����ն˿�
	{
		HandleNum = 0;
		if (OPen_Pic_Port(&m_Port9, &m_Baud9, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start9_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[8] = AfxBeginThread(WriteIMEIFunction9, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 9;
	StopSign[9] = FALSE;
	Start10_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 1;
		if (OPen_Pic_Port(&m_Port10, &m_Baud10, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start10_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[9] = AfxBeginThread(WriteIMEIFunction10, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 10;
	StopSign[10] = FALSE;
	Start11_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 2;
		if (OPen_Pic_Port(&m_Port11, &m_Baud11, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start11_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[10] = AfxBeginThread(WriteIMEIFunction11, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 11;
	StopSign[11] = FALSE;
	Start12_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 3;
		if (OPen_Pic_Port(&m_Port12, &m_Baud12, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start12_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[11] = AfxBeginThread(WriteIMEIFunction12, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 12;
	StopSign[12] = FALSE;
	Start13_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 4;
		if (OPen_Pic_Port(&m_Port13, &m_Baud13, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start13_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[12] = AfxBeginThread(WriteIMEIFunction13, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 13;
	StopSign[13] = FALSE;
	Start14_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 5;
		if (OPen_Pic_Port(&m_Port14, &m_Baud14, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start14_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[13] = AfxBeginThread(WriteIMEIFunction14, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 14;
	StopSign[14] = FALSE;
	Start15_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 6;
		if (OPen_Pic_Port(&m_Port15, &m_Baud15, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start15_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[14] = AfxBeginThread(WriteIMEIFunction15, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstart16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 15;
	StopSign[15] = FALSE;
	Start16_Control.EnableWindow(FALSE);
	if (GetPicChoose == TRUE)
	{
		HandleNum = 7;
		if (OPen_Pic_Port(&m_Port16, &m_Baud16, HandleNum) == FALSE)
		{
			AfxMessageBox("��ͼ��˿�ʧ�ܣ�");
			Start16_Control.EnableWindow(TRUE);
		}
	}
	else
	{
		Thread_Handle[15] = AfxBeginThread(WriteIMEIFunction16, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}


void IMEIWrite_MulAT::OnBnClickedButtonstop7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 6;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result7, &Final_Result_Control7, 250, "�������ԣ���", HandleNum);

	int PortHandle = 6;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(6);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 7;
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result8, &Final_Result_Control8, 250, "�������ԣ���", HandleNum);

	int PortHandle = 7;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(7);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ArraySign = 8;//һ�������ǣ����ڴ˺���һЩ�����ͳһʹ��

	if (GetDongleScanCheckValue == TRUE&& (ArraySign > 7) && COM_State[ArraySign]==TRUE)
	{
		BOOL Return;
		if (hPort[ArraySign] != NULL)
			Return = CloseHandleControl(hPort[ArraySign], GetRDAHostCheckValue, ArraySign);				//���Գɹ���
		hPort[ArraySign] = NULL;
		if (Return == TRUE)
			LogShow_exchange(&m_Result9, &Final_Result_Control9, 5, "�رոô��ڳɹ�����", ArraySign);
		else
		{
			LogShow_exchange(&m_Result9, &Final_Result_Control9, 128, "�رոô���ʧ�ܣ���", ArraySign, "���ܲ���:0-�˿�");
			//return FALSE; //�رմ���ʧ��
		}
		COM_State[ArraySign] = FALSE;
	}

	int HandleNum = 8;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result9, &Final_Result_Control9, 250, "�������ԣ���", HandleNum);

	int PortHandle = 8;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start9_Control.EnableWindow(TRUE);
		PortHandle = 0;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(8);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 9;

	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result10, &Final_Result_Control10, 250, "�������ԣ���", HandleNum);

	int PortHandle = 9;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start10_Control.EnableWindow(TRUE);
		PortHandle = 1;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(9);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 10;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result11, &Final_Result_Control11, 250, "�������ԣ���", HandleNum);

	int PortHandle = 10;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start11_Control.EnableWindow(TRUE);
		PortHandle = 2;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(10);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 11;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result12, &Final_Result_Control12, 250, "�������ԣ���", HandleNum);

	int PortHandle = 11;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start12_Control.EnableWindow(TRUE);
		PortHandle = 3;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(11);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 12;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result13, &Final_Result_Control13, 250, "�������ԣ���", HandleNum);

	int PortHandle = 12;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start13_Control.EnableWindow(TRUE);
		PortHandle = 4;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(12);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 13;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result14, &Final_Result_Control14, 250, "�������ԣ���", HandleNum);

	int PortHandle = 13;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start14_Control.EnableWindow(TRUE);
		PortHandle = 5;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(13);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 14;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result15, &Final_Result_Control15, 250, "�������ԣ���", HandleNum);

	int PortHandle = 14;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start15_Control.EnableWindow(TRUE);
		PortHandle = 6;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(14);
	}
}

void IMEIWrite_MulAT::OnBnClickedButtonstop16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int HandleNum = 15;
	StopSign[HandleNum] = TRUE;
	Sleep(100);
	int i = 0;
	while (Thread_State[HandleNum] == TRUE)
	{
		Sleep(STOPDELAY);
		i++;
		if (i>1)
			break;
	};
	//COM_State[HandleNum]=FALSE;
	LogShow_exchange(&m_Result16, &Final_Result_Control16, 250, "�������ԣ���", HandleNum);

	int PortHandle = 15;

	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[PortHandle] == 1)
	{
		BleGetSettingFlag[PortHandle] = 0;
		BluetoothDisconnect(PortHandle);
	}

	if (GetPicChoose == TRUE)
	{
		Start16_Control.EnableWindow(TRUE);
		PortHandle = 7;
		if (hPicPort[PortHandle] == NULL)
			return;
		BOOL Return = CloseHandle(hPicPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPicPort[PortHandle] = NULL;
	}
	else if (0)
	{
		BOOL Return = CloseHandle(hPort[PortHandle]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[PortHandle] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(15);
	}

}

void IMEIWrite_MulAT::OnBnClickedButtonconncpu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (OPen_Serial_PortReadConstant(&CPUControlPort, &CPUControlBaud, THREAD_NUM, TRUE) == TRUE)
	{
		//�򿪴��ڳɹ�
		COM_State[THREAD_NUM] = TRUE;
		GetDlgItem(IDC_BUTTONCONNCPU)->EnableWindow(FALSE);
	}
	else
	{
		//�򿪴���ʧ��
		COM_State[THREAD_NUM] = FALSE;
		AfxMessageBox("�򿪶˿�ʧ�ܣ�");
	}
}

void IMEIWrite_MulAT::OnBnClickedButtondisconncpu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (hPort[THREAD_NUM] != NULL)
	{
		BOOL Return = CloseHandle(hPort[THREAD_NUM]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[THREAD_NUM] = NULL;
	}
	GetDlgItem(IDC_BUTTONCONNCPU)->EnableWindow(TRUE);
	COM_State[THREAD_NUM] = FALSE;
}

BOOL IMEIWrite_MulAT::CPUCommunication(CString CpuOrderName, int HandleNum, CString EndSign, CString StartSign, BOOL Enable, CEdit* m_Result, CEdit* Final_Result_Control)
{
	CString TestType, WorkStation;

	GetDlgItem(IDC_COMBO56)->GetWindowTextA(WorkStation);
	GetDlgItem(IDC_COMBOTESTYYPE)->GetWindowTextA(TestType);

	if (TestType != "DAM�̵���")
	{
		LogShow_exchange(m_Result, Final_Result_Control, 6, "��ʼ�뵥Ƭ��ͨ��0-'" + CpuOrderName + "'����", HandleNum);
	}

	EnterCriticalSection(&CPUCOMMUNICATE);

	if (Enable == FALSE)
	{
		LeaveCriticalSection(&CPUCOMMUNICATE);
		return TRUE;
	}
	
	if (TestType == "DAM�̵���")
	{
		if (WorkStation == "SMT����")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "DAM�̵�����SMT�л�δʵ�֣�", HandleNum);
			LeaveCriticalSection(&CPUCOMMUNICATE);
			return FALSE;
		}

		if ((CpuOrderName.Find("ͨ·") != -1) || (CpuOrderName.Find("���") != -1))
		{
			BOOL DamFlag = DamControlFun(HandleNum, CpuOrderName);

			if (DamFlag == FALSE)
			{
				LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�̵�������ʧ�ܣ�", HandleNum);
				LeaveCriticalSection(&CPUCOMMUNICATE);
				Vaule_Return_Count_CS[THREAD_NUM] = "";
				return FALSE;
			}
			else if (DamFlag == TRUE)
			{
				LeaveCriticalSection(&CPUCOMMUNICATE);
				Vaule_Return_Count_CS[THREAD_NUM] = "";
				return TRUE;
			}
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 6, CpuOrderName + "����Dam�̵�������", HandleNum);
			LeaveCriticalSection(&CPUCOMMUNICATE);
			return TRUE;
		}
		
	}
	
	LogShow_exchange(m_Result, Final_Result_Control, 6, "��ʼ�뵥Ƭ��ͨ��1-'" + CpuOrderName + "'����", HandleNum);
	int CpuOrderNum = -1;
	if (CpuOrderName == "�ɹ�")
		CpuOrderNum = 0;
	else if (CpuOrderName == "ʧ��")
		CpuOrderNum = 1;
	else if (CpuOrderName.Find("��������") != -1)
		CpuOrderNum = 2;
	else if (CpuOrderName.Find("�ط�����") != -1)
		CpuOrderNum = 3;
	else if (CpuOrderName.Find("SOS����") != -1)
		CpuOrderNum = 4;
	else if (CpuOrderName.Find("SOS�ذ���") != -1)
		CpuOrderNum = 5;
	else if (CpuOrderName.Find("����D") != -1)
		CpuOrderNum = 6;
	else if ((CpuOrderName.Find("����E") != -1) || (CpuOrderName.Find("��") != -1))
		CpuOrderNum = 7;
	else if (CpuOrderName.Find("���ư�״̬") != -1)
		CpuOrderNum = 14;
	else if (CpuOrderName.Find("���ư�") != -1)
		CpuOrderNum = 8;
	else if (CpuOrderName.Find("ģ�����") != -1)
		CpuOrderNum = 9;
	else if (CpuOrderName.Find("��ͨ·") != -1)
		CpuOrderNum = 10;
	else if (CpuOrderName.Find("�ر�ͨ·") != -1)
		CpuOrderNum = 11;
	else if (CpuOrderName.Find("��������") != -1)
	{
		CpuOrderNum = 12;
		DayTimeState[HandleNum] = TRUE;
	}
	else if (CpuOrderName.Find("��������") != -1)
	{
		CpuOrderNum = 13;
		DayTimeState[HandleNum] = FALSE;
	}
	else if (CpuOrderName.Find("ȷ��ʧ��") != -1)
		CpuOrderNum = 15;
	else if (CpuOrderName.Find("ȷ�ϳɹ�") != -1)
		CpuOrderNum = 16;
	else if (CpuOrderName.Find("���1��") != -1)
		CpuOrderNum = 17;
	else if (CpuOrderName.Find("���2��") != -1)
		CpuOrderNum = 18;
	else if (CpuOrderName.Find("���3��") != -1)
		CpuOrderNum = 19;
	else if (CpuOrderName.Find("���1��") != -1)
	{
		CpuOrderNum = 20;
		RecoverVoltage = TRUE;
	}
	else if (CpuOrderName.Find("���2��") != -1)
		CpuOrderNum = 21;
	else if (CpuOrderName.Find("���3��") != -1)
		CpuOrderNum = 22;
	else if (CpuOrderName.Find("���4��") != -1)
		CpuOrderNum = 23;
	else if (CpuOrderName.Find("���4��") != -1)
		CpuOrderNum = 24;
	else
	{
		LeaveCriticalSection(&CPUCOMMUNICATE);
		return FALSE;
	}

	/*
	��1#��ѹ1��AT+01a+#+Voltage_1_ON+У��  ���磺AT01a#Voltage_1_ONAC
	��1#��ѹ2��AT+01a+#+Voltage_2_ON+У��
	��1#��ѹ3��AT+01a+#+Voltage_3_ON+У��
	��2#��ѹ1��
	��1#��ѹ1��AT+01a+#+Voltage_1_OFF+У�� ���磺AT01a#Voltage_1_OFFFE
	��1#��ѹ2��AT+01a+#+Voltage_2_OFF+У��
	��1#��ѹ3��AT+01a+#+Voltage_3_OFF+У��
	*/
	//--���̵�--�����--��������--�ط�����--SOS����--SOS�ذ���--����1--����2--��ȡ���ն�--���ģ�����--��ͨ·--�ر�ͨ·--��������--��������--��״̬--��ť״̬
	CString CpuOrder[25] = { "CTNGL", "CTNRL", "TNB", "TFB", "TNK", "TFK", "TNDSV", "TNCSV", "GetCom", "Crt", "TNPW", "TFPW", "TNL", "TFL", "CheckDoor", "CheckButtonF", "CheckButtonP", \
		"Voltage_1_ON", "Voltage_2_ON", "Voltage_3_ON", "Voltage_1_OFF", "Voltage_2_OFF", "Voltage_3_OFF", "Voltage_4_ON", "Voltage_4_OFF" };
	CString Command_Position[16] = { "AT01a#", "AT02b#", "AT03c#", "AT04d#", "AT05e#", "AT06f#", "AT07g#", "AT08h#",
		"AT09i#", "AT10j#", "AT11k#", "AT12l#", "AT13m#", "AT14n#", "AT15o#", "AT16p#" };


	if (TestType == "��β��")
	{
		CpuOrder[0] = "TNGL";
		CpuOrder[1] = "TNRL";
	}


	CString TestType2;
	GetDlgItem(IDC_COMBOTESTYYPE2)->GetWindowTextA(TestType2);//����
	CString TestType3;
	GetDlgItem(IDC_COMBOTESTYYPE3)->GetWindowTextA(TestType3);//����
	CString TestType4;
	GetDlgItem(IDC_COMBOTESTYYPE4)->GetWindowTextA(TestType4);//����

	if (TestType2 == "")//������
	{
		if (CpuOrderNum <= 8)
		{
			LeaveCriticalSection(&CPUCOMMUNICATE);
			return TRUE;
		}
	}
	if (TestType3 == "�޵���")//��ģ�����
	{
		if (CpuOrderNum == 9)
		{
			LeaveCriticalSection(&CPUCOMMUNICATE);
			return TRUE;
		}
	}
	if (TestType4 == "")//�޿���
	{
		if ((CpuOrderNum == 10) || (CpuOrderNum == 11))
		{
			LeaveCriticalSection(&CPUCOMMUNICATE);
			return TRUE;
		}
	}

	CString CpuOrderComplete;
	CString CpuOrderTemp;
	CString Serial_Order_Return;//����

	//CpuOrderTemp=CpuOrder[CpuOrderNum]+Command_Position[HandleNum];//��λ��
	CpuOrderTemp = Command_Position[HandleNum] + CpuOrder[CpuOrderNum];//��λ��
	CpuOrderTemp = CpuOrderTemp.Mid(2);

	MD5 mb;
	mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
	CString CpuOrderMD5 = (mb.toString()).c_str();

	CpuOrderComplete = "AT" + CpuOrderTemp + CpuOrderMD5.Left(2);
	if ((m_Result != NULL) && (Final_Result_Control != NULL))
		LogShow_exchange(m_Result, Final_Result_Control, 6, "����ͨ�ŷ���ֵ:\r\n" + CpuOrderComplete, HandleNum);
	BOOL Result = FALSE;
	for (int i = 0; i<2; i++)
	{
		if (CpuOrderName.Find("ģ�����") != -1)
			Send_Serial_Order(&Serial_Order_Return, CpuOrderComplete, THREAD_NUM, "NULL", (LPSTR)(LPCTSTR)StartSign);
		else
		{
			Send_Serial_Order(&Serial_Order_Return, CpuOrderComplete, THREAD_NUM, "NULL", (LPSTR)(LPCTSTR)StartSign);//Send_Serial_Order(&Serial_Order_Return,CpuOrderComplete,THREAD_NUM,(LPSTR)(LPCTSTR)CpuOrder[CpuOrderNum],(LPSTR)(LPCTSTR)StartSign);
		}
		if ((m_Result != NULL) && (Final_Result_Control != NULL))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 6, "\r\n����ͨ�ŷ���ֵ:\r\n" + Vaule_Return_Count_CS[THREAD_NUM], HandleNum);
			LogShow_exchange(m_Result, Final_Result_Control, 6, "\r\n��������ֵ:\r\n" + CpuOrder[CpuOrderNum], HandleNum);
		}
		if (Serial_Order_Return == "Analysis_SUCCESS")
		{
			if ((m_Result != NULL) && (Final_Result_Control != NULL))
				LogShow_exchange(m_Result, Final_Result_Control, 6, "����ֵAnalysis_Succ", HandleNum);
			if (CpuOrderName.Find("ģ�����") != -1)
			{
				if (Vaule_Return_Count_CS[THREAD_NUM].Find("A") != -1)
				{
					Result = TRUE;
					SimulationCurrent[HandleNum] = Vaule_Return_Count_CS[THREAD_NUM];
					break;
				}
			}
			else if (Vaule_Return_Count_CS[THREAD_NUM].Find("R" + CpuOrderComplete) != -1)
			{
				if (CpuOrderName.Find("���ư�״̬") != -1)
				{
					if (Vaule_Return_Count_CS[THREAD_NUM].Find("Open") != -1)
					{
						Result = TRUE;
						break;
					}
					else
					{
						Result = FALSE;
						break;
					}
				}
				else if (CpuOrderName.Find("ȷ��") != -1)
				{
					if (Vaule_Return_Count_CS[THREAD_NUM].Find("Pressed") != -1)
					{
						Result = TRUE;
						break;
					}
					else
					{
						Result = FALSE;
						break;
					}
				}
				else
				{
					Result = TRUE;
					break;
				}
			}
		}
		else
		{
			if ((m_Result != NULL) && (Final_Result_Control != NULL))
				LogShow_exchange(m_Result, Final_Result_Control, 6, "����ֵAnalysis_Fail", HandleNum);
		}
	}
	LeaveCriticalSection(&CPUCOMMUNICATE);
	Vaule_Return_Count_CS[THREAD_NUM] = "";
	return Result;
}

BOOL IMEIWrite_MulAT::GetExistReturnCode(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, CString *ExistMessage, CString ChipRfIDbg)
{
	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[DataUpNum];
	}
	*ExistMessage = "";
	_variant_t var;
	CString FieldName, FieldValue;
	myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
	FieldName = "OtherTestSign";
	var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
	if (var.vt != VT_NULL)
	{
		FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
		LogShow_exchange(m_Result, Final_Result_Control, 5, "�������д����ֶ�:OtherTestSign���ɹ���ȡ:" + FieldValue, DataUpNum);
		*ExistMessage = FieldValue;

		myado.CloseSheet();
		return TRUE;
	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 5, "�������в����ڷ�����Ϣ", DataUpNum);

	}
	myado.CloseSheet();
	return FALSE;
}
//���������ݿ�Data_UpdatePara
BOOL IMEIWrite_MulAT::Data_UpdatePara(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, BOOL ErrorUpEnable, CString ChipRfIDbg)//---------0
{
	CString ProductType;
	if (GetRDAHostCheckValue == TRUE)
	{
		ProductType = "RDA";
	}
	else
	{
		ProductType = "MTK";
	}

	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[DataUpNum];
	}
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�����������ݿ�.......", DataUpNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ�����������...", DataUpNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if ((ChipRfIDCurrent == "") || (Software_Version[DataUpNum] == ""))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "SN��������汾Ϊ��", DataUpNum);
			return FALSE;
		}



		//////////////////////////////////////////////////
		//��������
		ReturnCodeControl.GetWindowTextA(ReturnCodeCS);
		if (ReturnCodeCS != "")
		{
			BOOL UP_Barcode = TRUE;
			BOOL Barcode_Check_UP;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�����ܱ�(����)...", DataUpNum);
			BOOL ReduceBarcode_Check = FALSE;
			////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
			myado.OpenSheet("select SN,SoftModel,Version,OtherTestSign from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
			BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
			Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
			Barcode_Check &=   myado.Find("Version='" + Software_Version[DataUpNum] + "'");
			ReduceBarcode_Check = Barcode_Check;

			myado.CloseSheet();

			CString ExistMessage = "";
			GetExistReturnCode(myado, DataUpNum, m_Result, Final_Result_Control, &ExistMessage, ChipRfIDCurrent);
			Barcode_Check &= (ExistMessage.Find(ReturnCodeCS) != -1);

			if (Barcode_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�(����)������������������������������������������������������������...", DataUpNum);
				return TRUE;//��2
			}
			else
			{
				/*
				myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
				Barcode_Check=myado.Find("SN='"+ChipRfIDCurrent+"'");
				Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
				Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
				myado.CloseSheet();
				*/
				Barcode_Check = ReduceBarcode_Check;
				if (Barcode_Check == TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������(����)...", DataUpNum);

				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�(����)...", DataUpNum);
				}
			}
			////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
			CTime ReturnTime;
			CString ReturnTimeCS;
			ReturnTime = CTime::GetCurrentTime();
			ReturnTimeCS.Format("%04d-%02d-%02d->", ReturnTime.GetYear(), ReturnTime.GetMonth(), ReturnTime.GetDay());


			UP_Barcode = TRUE;
			if (Barcode_Check == TRUE)
			{
				//����
				CString strSQL_Write_Barcode;
				//myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

				if (1)
				{
					strSQL_Write_Barcode = "UPDATE dbo.Gps_TestResult SET OtherTestSign = '" + ReturnTimeCS + ReturnCodeCS + ExistMessage + "',ZhiDan = '" + ZhiDanCS + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";
					UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
				}
				//myado.CloseSheet();
				LogShow_exchange(m_Result, Final_Result_Control, 0, "����sql:" + strSQL_Write_Barcode, DataUpNum);
				//����
			}
			else
			{
				//ֱ���ϴ�
				CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,OtherTestSign,ZhiDan)\
								  								 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','" + ReturnTimeCS + ReturnCodeCS + "','" + ZhiDanCS + "')";


				//myado.OpenSheet("select * from dbo.Gps_TestResult");
				UP_Barcode = myado.Execute(strSQL1, &var);
				//myado.CloseSheet();
				LogShow_exchange(m_Result, Final_Result_Control, 0, "����sql:" + strSQL1, DataUpNum);
			}
			if (UP_Barcode == TRUE)
			{
				//�ٴμ��
				/*
				Sleep(500);
				myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' ");
				Barcode_Check_UP=myado.Find("SN='"+ChipRfIDCurrent+"'");
				Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
				Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
				Barcode_Check_UP&=myado.Find("AutoTestResult='1'");
				myado.CloseSheet();
				*/
				if (var.intVal<1)//if(var.intVal!=1)
					Barcode_Check_UP = FALSE;
				else
					Barcode_Check_UP = TRUE;
				//�ٴμ��
			}
			if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ɹ�(����)...", DataUpNum);
			}
			else
			{
				if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ʧ��(����)", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
				return FALSE;
			}
			return TRUE;
		}
		//��������
		///////////////////////////////////////////////////////////




		///////////////////////////////////////////////////////////
		//�ϴ�����λ

		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴���ܱ�...", DataUpNum);

		myado.OpenSheet("select SN,Result,SoftModel,Version from dbo.Gps_AutoTest_Result WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");//��1
		BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		//Barcode_Check&=myado.Find("Result='1'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������...", DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�...", DataUpNum);
		}

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP;
		if (Barcode_Check == TRUE)
		{

			//����--��ԭʼϵͳ���Թ�
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//��1

			if (1)
			{
				strSQL_Write_Barcode = "UPDATE dbo.Gps_AutoTest_Result SET Result='1',ZhiDan = '" + ZhiDanCS + "',Computer = '" + Hostname + Ipaddress + "',TestSetting = '" + ATCommandList_CSDBCompare + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";//��2
				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_AutoTest_Result(SN,IMEI,Version,SoftModel,Result,Remark,TesterId,ZhiDan,Computer,TestSetting)\
							  							  							  							  							 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','1','" + ProductType + "','" + g_TesterIdStr + "','" + ZhiDanCS + "','" + Hostname + Ipaddress + "','" + ATCommandList_CSDBCompare + "')";//��3


			//myado.OpenSheet("select * from dbo.Gps_AutoTest_Result");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��
			/*Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,Result from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' AND Result ='1'");//��3
			Barcode_Check_UP=myado.Find("SN='"+ChipRfIDCurrent+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("Result='1'");//��1
			myado.CloseSheet();*/
			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}

		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ�����λ�ɹ�...", DataUpNum);
		}
		else
		{
			if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ�����λʧ��", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

		//�ϴ�����λ
		///////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�����ܱ�...", DataUpNum);
		BOOL ReduceBarcode_Check = FALSE;
		////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
		myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
		Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		ReduceBarcode_Check = Barcode_Check;
		Barcode_Check &= myado.Find("AutoTestResult='1'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�������������������������������������������������������������...", DataUpNum);
			//����һ������Ϣ
			if (g_ADCTFlag == TRUE)
			{
				SendADCTHint(get_map_key_value(PortStatusMap, DataUpNum), "�Ѳ���", 0);
			}
			return TRUE;//��2
		}
		else
		{
			/*
			myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
			Barcode_Check=myado.Find("SN='"+ChipRfIDCurrent+"'");
			Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			myado.CloseSheet();
			*/
			Barcode_Check = ReduceBarcode_Check;
			if (Barcode_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������...", DataUpNum);
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�...", DataUpNum);
			}
		}
		////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
		UP_Barcode = TRUE;
		if (Barcode_Check == TRUE)
		{
			//����
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

			if (1)
			{
				//strSQL_Write_Barcode="UPDATE dbo.Gps_TestResult SET AutoTestResult = '1' WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'";
				strSQL_Write_Barcode = "UPDATE dbo.Gps_TestResult SET AutoTestResult = '1',ZhiDan = '" + ZhiDanCS + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";
				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			LogShow_exchange(m_Result, Final_Result_Control, 0, "sql:" + strSQL_Write_Barcode, DataUpNum);
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,AutoTestResult,ZhiDan)\
							  							 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','1','" + ZhiDanCS + "')";


			//myado.OpenSheet("select * from dbo.Gps_TestResult");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��
			/*
			Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' ");
			Barcode_Check_UP=myado.Find("SN='"+ChipRfIDCurrent+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("AutoTestResult='1'");
			myado.CloseSheet();
			*/
			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}


		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ɹ�...", DataUpNum);
		}
		else
		{
			if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ʧ��", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "û�й�ѡ���ݿⲻ�ϴ�...", DataUpNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;//LBCCheckIMEI
}
//���������ݿ�Data_UpdatePara

//���������ݿ�Data_UpdatePara2
BOOL IMEIWrite_MulAT::Data_UpdatePara2(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, BOOL ErrorUpEnable, CString ChipRfIDbg)//---------0
{
	CString ProductType;
	if (GetRDAHostCheckValue == TRUE)
	{
		ProductType = "RDA";
	}
	else
	{
		ProductType = "MTK";
	}

	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[DataUpNum];
	}
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�����������ݿ�.......", DataUpNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ�����������...", DataUpNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}


		if ((ChipRfIDCurrent == "") || (Software_Version[DataUpNum] == ""))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "SN��������汾Ϊ��", DataUpNum);
			return FALSE;
		}

		///////////////////////////////////////////////////////////
		//�ϴ�����λ

		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴���ܱ�...", DataUpNum);

		myado.OpenSheet("select SN,Result,SoftModel,Version from dbo.Gps_AutoTest_Result2 WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");//��1
		BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		//Barcode_Check&=myado.Find("Result='1'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������...", DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�...", DataUpNum);
		}

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP;
		if (Barcode_Check == TRUE)
		{

			//����--��ԭʼϵͳ���Թ�
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.Gps_AutoTest_Result2 WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//��1

			if (1)
			{
				strSQL_Write_Barcode = "UPDATE dbo.Gps_AutoTest_Result2 SET Result='1',ZhiDan = '" + ZhiDanCS + "',Computer = '" + Hostname + Ipaddress + "',TestSetting = '" + ATCommandList_CSDBCompare + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";//��2
				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_AutoTest_Result2(SN,IMEI,Version,SoftModel,Result,Remark,TesterId,ZhiDan,Computer,TestSetting)\
							  							  							  							  							 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','1','" + ProductType + "','" + g_TesterIdStr + "','" + ZhiDanCS + "','" + Hostname + Ipaddress + "','" + ATCommandList_CSDBCompare + "')";//��3	

			//myado.OpenSheet("select * from dbo.Gps_AutoTest_Result2");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��
			/*Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,Result from dbo.Gps_AutoTest_Result2 WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' AND Result ='1'");//��3
			Barcode_Check_UP=myado.Find("SN='"+ChipRfIDCurrent+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("Result='1'");//��1
			myado.CloseSheet();*/
			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}

		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ�����λ�ɹ�...", DataUpNum);
		}
		else
		{
			if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ�����λʧ��", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

		//�ϴ�����λ
		///////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�����ܱ�...", DataUpNum);
		////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
		myado.OpenSheet("select SN,SoftModel,Version,FunctionResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
		Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		Barcode_Check &= myado.Find("FunctionResult='1'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�������������������������������������������������������������...", DataUpNum);
			return TRUE;//��2
		}
		else
		{
			myado.OpenSheet("select SN,SoftModel,Version,FunctionResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
			Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
			Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
			Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
			myado.CloseSheet();
			if (Barcode_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������...", DataUpNum);
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�...", DataUpNum);
			}
		}
		////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
		UP_Barcode = TRUE;
		if (Barcode_Check == TRUE)
		{
			//����
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

			if (1)
			{
				strSQL_Write_Barcode = "UPDATE dbo.Gps_TestResult SET FunctionResult = '1' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";
				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,FunctionResult)\
							  							 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','1')";


			//myado.OpenSheet("select * from dbo.Gps_TestResult");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��
			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}


		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ɹ�...", DataUpNum);
		}
		else
		{
			if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ʧ��", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "û�й�ѡ���ݿⲻ�ϴ�...", DataUpNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;//LBCCheckIMEI
}
//���������ݿ�Data_UpdatePara2


//���������ݿ�Data_UpdatePara3
BOOL IMEIWrite_MulAT::Data_UpdatePara3(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, BOOL ErrorUpEnable, CString ChipRfIDbg)//---------0
{
	CString ProductType;
	if (GetRDAHostCheckValue == TRUE)
	{
		ProductType = "RDA";
	}
	else
	{
		ProductType = "MTK";
	}

	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[DataUpNum];
	}
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�����������ݿ�.......", DataUpNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ�����������...", DataUpNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if ((ChipRfIDCurrent == "") || (Software_Version[DataUpNum] == ""))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "SN��������汾Ϊ��", DataUpNum);
			return FALSE;
		}



		//////////////////////////////////////////////////
		//��������
		ReturnCodeControl.GetWindowTextA(ReturnCodeCS);
		if (ReturnCodeCS != "")
		{
			BOOL UP_Barcode = TRUE;
			BOOL Barcode_Check_UP;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�����ܱ�(����)...", DataUpNum);
			BOOL ReduceBarcode_Check = FALSE;
			////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
			myado.OpenSheet("select SN,SoftModel,Version,OtherTestSign from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
			BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
			Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
			Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
			ReduceBarcode_Check = Barcode_Check;

			myado.CloseSheet();

			CString ExistMessage = "";
			GetExistReturnCode(myado, DataUpNum, m_Result, Final_Result_Control, &ExistMessage, ChipRfIDCurrent);
			Barcode_Check &= (ExistMessage.Find(ReturnCodeCS) != -1);

			if (Barcode_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�(����)������������������������������������������������������������...", DataUpNum);
				return TRUE;//��2
			}
			else
			{
				/*
				myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
				Barcode_Check=myado.Find("SN='"+ChipRfIDCurrent+"'");
				Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
				Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
				myado.CloseSheet();
				*/
				Barcode_Check = ReduceBarcode_Check;
				if (Barcode_Check == TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������(����)...", DataUpNum);

				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�(����)...", DataUpNum);
				}
			}
			////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
			CTime ReturnTime;
			CString ReturnTimeCS;
			ReturnTime = CTime::GetCurrentTime();
			ReturnTimeCS.Format("%04d-%02d-%02d->", ReturnTime.GetYear(), ReturnTime.GetMonth(), ReturnTime.GetDay());


			UP_Barcode = TRUE;
			if (Barcode_Check == TRUE)
			{
				//����
				CString strSQL_Write_Barcode;
				//myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

				if (1)
				{
					strSQL_Write_Barcode = "UPDATE dbo.Gps_TestResult SET OtherTestSign = '" + ReturnTimeCS + ReturnCodeCS + ExistMessage + "',ZhiDan = '" + ZhiDanCS + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";
					UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
				}
				//myado.CloseSheet();
				LogShow_exchange(m_Result, Final_Result_Control, 0, "����sql:" + strSQL_Write_Barcode, DataUpNum);
				//����
			}
			else
			{
				//ֱ���ϴ�
				CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,OtherTestSign,ZhiDan)\
								  								  								 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','" + ReturnTimeCS + ReturnCodeCS + "','" + ZhiDanCS + "')";


				//myado.OpenSheet("select * from dbo.Gps_TestResult");
				UP_Barcode = myado.Execute(strSQL1, &var);
				//myado.CloseSheet();
				LogShow_exchange(m_Result, Final_Result_Control, 0, "����sql:" + strSQL1, DataUpNum);
			}
			if (UP_Barcode == TRUE)
			{
				//�ٴμ��
				if (var.intVal<1)//if(var.intVal!=1)
					Barcode_Check_UP = FALSE;
				else
					Barcode_Check_UP = TRUE;
				//�ٴμ��
			}
			if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ɹ�(����)...", DataUpNum);
			}
			else
			{
				if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ʧ��(����)", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
				return FALSE;
			}
			return TRUE;
		}
		//��������
		///////////////////////////////////////////////////////////




		///////////////////////////////////////////////////////////
		//�ϴ�����λ

		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�ϻ����ܱ�...", DataUpNum);

		myado.OpenSheet("select SN,Result,SoftModel,Version from dbo.Gps_AutoTest_Result3 WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");//��1
		BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		//Barcode_Check&=myado.Find("Result='1'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ����ϻ����ܲ��Լ�¼������...", DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�...", DataUpNum);
		}

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP;
		if (Barcode_Check == TRUE)
		{

			//����--��ԭʼϵͳ���Թ�
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.Gps_AutoTest_Result3 WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//��1

			if (1)
			{
				strSQL_Write_Barcode = "UPDATE dbo.Gps_AutoTest_Result3 SET Result='1',ZhiDan = '" + ZhiDanCS + "',Computer = '" + Hostname + Ipaddress + "',TestSetting = '" + ATCommandList_CSDBCompare + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";//��2
				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_AutoTest_Result3(SN,IMEI,Version,SoftModel,Result,Remark,TesterId,ZhiDan,Computer,TestSetting)\
							  							  							  							  							 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','1','" + ProductType + "','"+g_TesterIdStr+"'','" + ZhiDanCS + "','" + Hostname + Ipaddress + "','" + ATCommandList_CSDBCompare + "')";//��3


			//myado.OpenSheet("select * from dbo.Gps_AutoTest_Result3");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��
			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}

		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ�����λ�ɹ�...", DataUpNum);
		}
		else
		{
			if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ�����λʧ��", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

		//�ϴ�����λ
		///////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�����ܱ�...", DataUpNum);
		BOOL ReduceBarcode_Check = FALSE;
		////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
		myado.OpenSheet("select SN,SoftModel,Version,GPSResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
		Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		ReduceBarcode_Check = Barcode_Check;
		Barcode_Check &= myado.Find("GPSResult='1'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�������������������������������������������������������������...", DataUpNum);
			//����һ������Ϣ
			if (g_ADCTFlag == TRUE)
			{
				SendADCTHint(get_map_key_value(PortStatusMap, DataUpNum), "�Ѳ���", 0);
			}
			return TRUE;//��2
		}
		else
		{

			Barcode_Check = ReduceBarcode_Check;
			if (Barcode_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��Ѿ��й��ܲ��Լ�¼������...", DataUpNum);
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿⲻ���ڣ�ֱ���ϴ�...", DataUpNum);
			}
		}
		////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
		UP_Barcode = TRUE;
		if (Barcode_Check == TRUE)
		{
			//����
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

			if (1)
			{
				//strSQL_Write_Barcode="UPDATE dbo.Gps_TestResult SET AutoTestResult = '1' WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'";
				strSQL_Write_Barcode = "UPDATE dbo.Gps_TestResult SET GPSResult = '1',ZhiDan = '" + ZhiDanCS + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";
				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			LogShow_exchange(m_Result, Final_Result_Control, 0, "sql:" + strSQL_Write_Barcode, DataUpNum);
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,GPSResult,ZhiDan)\
							  							  							 values('" + ChipRfIDCurrent + "','','" + Software_Version[DataUpNum] + "','" + MachineType_CS + "','1','" + ZhiDanCS + "')";


			//myado.OpenSheet("select * from dbo.Gps_TestResult");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��

			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}


		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ɹ�...", DataUpNum);
		}
		else
		{
			if (ErrorUpEnable == TRUE)LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "���ݿ��ϴ��ܱ�ʧ��", DataUpNum, "���ܲ���:3-DB", ChipRfIDCurrent);
			return FALSE;
		}

	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "û�й�ѡ���ݿⲻ�ϴ�...", DataUpNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;//LBCCheckIMEI
}
//���������ݿ�Data_UpdatePara3

//���������ݿ�Data_UpdateParaPre
BOOL IMEIWrite_MulAT::Data_UpdateParaPre(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, BOOL* smt_Check)//---------0
{
	CString ChipRfIDCurrent;
	ChipRfIDCurrent = ChipRfID[DataUpNum];
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";

		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�����������ݿ�.......", DataUpNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ�����������...", DataUpNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", DataUpNum, "���ܲ���:3-DB");
			return FALSE;
		}

		if ((ChipRfIDCurrent == "") || (Software_Version[DataUpNum] == ""))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "SN��������汾Ϊ��", DataUpNum);
			return FALSE;
		}



		//////////////////////////////////////////////////
		//��SMT����
		//WorkStationControl.GetWindowTextA(WorkStationCS);
		//if(WorkStationCS=="")WorkStationControl.GetWindowTextA(WorkStationCS);
		if ((CheckSMTChoose == TRUE) && ((WorkStationCS != "SMT����") && (WorkStationCS != "�з�����")))
		{
			//myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
			myado.OpenSheet("select SN,SoftModel,Version,FunctionResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'");
			BOOL SMT_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
			//SMT_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
			SMT_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
			SMT_Check &= myado.Find("FunctionResult='1'");
			myado.CloseSheet();

			*smt_Check = SMT_Check;
			if (SMT_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "SMT���ܲ���ͨ��...", DataUpNum);
			}
			else
			{
				myado.OpenSheet("select SN,SoftModel,Version,FunctionResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "'");
				BOOL SMT_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
				//SMT_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
				//SMT_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
				SMT_Check &= myado.Find("FunctionResult='1'");
				myado.CloseSheet();

				*smt_Check = SMT_Check;
				if (SMT_Check == TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "SMT���ܲ���ͨ��...", DataUpNum);
				}
				else
				{
					ParaItemName[DataUpNum] = "SMT����©��";
					LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "SMT����©�⣡����", DataUpNum, "���ܲ���:3-DB");
					return TRUE;
				}
			}
		}
		else
			*smt_Check = TRUE;

		if ((CheckAutoTestChoose == TRUE) && ((WorkStationCS != "SMT����") && (WorkStationCS != "��װ����") && (WorkStationCS != "�з�����")))//���ϻ�ǰ����
		{
			myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'");
			BOOL AutoTest_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
			//SMT_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
			AutoTest_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
			AutoTest_Check &= myado.Find("AutoTestResult='1'");
			myado.CloseSheet();

			*smt_Check = AutoTest_Check;
			if (AutoTest_Check == TRUE)
			{
				LogShow_exchange(m_Result, Final_Result_Control, 0, "�ϻ�ǰ���ܲ���ͨ��...", DataUpNum);
			}
			else
			{
				myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "'");
				BOOL AutoTest1_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
				AutoTest1_Check &= myado.Find("AutoTestResult='1'");
				myado.CloseSheet();

				*smt_Check = AutoTest1_Check;
				if (AutoTest1_Check == TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 0, "�ϻ�ǰ���ܲ���ͨ��...", DataUpNum);
				}
				else
				{
					ParaItemName[DataUpNum] = "�ϻ�ǰ©��";
					LogShow_exchange(m_Result, Final_Result_Control, 128, "SN:" + ChipRfIDCurrent + "�ϻ�ǰ����©�⣡����", DataUpNum, "���ܲ���:3-DB");
					return TRUE;
				}
			}
		}
		else
			*smt_Check = TRUE;
		//��SMT����
		/////////////////////////////////////////////////



		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴�����ܱ�...", DataUpNum);//Data_UpdateParaPre
		////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
		myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult,FunctionResult,GPSResult from dbo.Gps_TestResult WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
		BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		CString m_dbNew;
		//WorkStationControl.GetWindowTextA(WorkStationCS);
		//if(WorkStationCS=="")WorkStationControl.GetWindowTextA(WorkStationCS);
		if (WorkStationCS == "SMT����")
			Barcode_Check &= myado.Find("FunctionResult='1'");
		else if (WorkStationCS == "��װ����")
			Barcode_Check &= myado.Find("AutoTestResult='1'");
		else if (WorkStationCS == "�ϻ������")
			Barcode_Check &= myado.Find("GPSResult='1'");
		else if (WorkStationCS == "�з�����")
			Barcode_Check = FALSE;
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 128, "����ѡ��վλ������", DataUpNum, "���ܲ���:2-TEST");
			return FALSE;
		}
		///////////////////////////////////
		///////////////////////////////////
		myado.CloseSheet();

		ReturnCodeControl.GetWindowTextA(ReturnCodeCS);
		if (ReturnCodeCS != "")
			/////////////////////////////////////
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "�鿴��������...", DataUpNum);
			CString ExistMessage = "";
			GetExistReturnCode(myado, DataUpNum, m_Result, Final_Result_Control, &ExistMessage, ChipRfIDCurrent);
			Barcode_Check &= (ExistMessage.Find(ReturnCodeCS) != -1);
		}
		/////////////////////////////////////
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 0, "���ݿ��ܱ��Ѿ��У��˻��Ѿ����Թ�������������������������������������������������������������...", DataUpNum);
			
			//����һ������Ϣ
			if (g_ADCTFlag == TRUE)
			{
				SendADCTHint(get_map_key_value(PortStatusMap, DataUpNum), "�Ѳ���", 0);
			}
			return TRUE;//�Ѿ����Թ�
		}

	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 0, "SN:" + ChipRfIDCurrent + "û�й�ѡ���ݿⲻ�ϴ�...", DataUpNum);
	}
	return FALSE;
}
//���������ݿ�Data_UpdateParaPre
BOOL IMEIWrite_MulAT::Data_UpdateError(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, CString Message, CString Category, CString ChipRfIDbg)//LogShow_exchange(m_Result,Final_Result_Control,5
{
	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[DataUpNum];
	}
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�����������ݿ�.......", DataUpNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			//LogShow_exchange(m_Result,Final_Result_Control,5,"���ݿ�����������...",DataUpNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", DataUpNum);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if ((ChipRfIDCurrent == "") || (Software_Version[DataUpNum] == ""))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "SN��������汾Ϊ��", DataUpNum);
			return FALSE;
		}

		if (SuccessCount<80)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�տ�ʼ�ɹ���¼����80�����ڵ���״̬����ʱ���Ǵ���", DataUpNum);
			return TRUE;
		}
		///////////////////////////////////////////////////////////
		//�ϴ�������Ϣ

		//LogShow_exchange(m_Result,Final_Result_Control,5,"�鿴log��Ϣ��...",DataUpNum);

		myado.OpenSheet("select SN,SoftModel,Version from dbo.LTestLogMessage WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");//��1
		BOOL Barcode_Check = myado.Find("SN='" + ChipRfIDCurrent + "'");
		Barcode_Check &= myado.Find("SoftModel='" + MachineType_CS + "'");
		Barcode_Check &= myado.Find("Version='" + Software_Version[DataUpNum] + "'");
		myado.CloseSheet();
		CString ExistMessage = "";
		if (Barcode_Check == TRUE)
		{
			GetExistError(myado, DataUpNum, m_Result, Final_Result_Control, &ExistMessage, ChipRfIDCurrent);//GetExistError(myado,DataUpNum,m_Result,Final_Result_Control,&ExistMessage,ChipRfIDbg);
			//LogShow_exchange(m_Result,Final_Result_Control,5,"���ݿ��Ѿ��й��ܲ��Լ�¼������...",DataUpNum);
		}
		else
		{
			//LogShow_exchange(m_Result,Final_Result_Control,5,"���ݿⲻ���ڣ�ֱ���ϴ�...",DataUpNum);
		}


		CTime logtime;
		logtime = CTime::GetCurrentTime();

		CString DataUpNumCS;
		DataUpNumCS.Format("->%02d:%02d:%02d�߳�%02d", logtime.GetHour(), logtime.GetMinute(), logtime.GetSecond(), DataUpNum);

		Message += DataUpNumCS;
		int epos = ExistMessage.Find(Message);
		if (epos == -1)								//�µĴ���
			ExistMessage += ("\r\n+++" + Message);
		else										//�Ѿ����ڴ˴���
		{
			ExistMessage = ExistMessage.Left(epos) + Message + "|" + ExistMessage.Mid(epos + Message.GetLength());
		}

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP;

		//WorkStationControl.GetWindowTextA(WorkStationCS);
		//if(WorkStationCS=="")WorkStationControl.GetWindowTextA(WorkStationCS);
		if (WorkStationCS == "�з�����")
			return TRUE;

		if (Barcode_Check == TRUE)
		{
			CString strSQL_Write_Barcode;
			//myado.OpenSheet("select * from dbo.LTestLogMessage WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//��1

			if (1)
			{
				if (WorkStationCS == "SMT����")
					strSQL_Write_Barcode = "UPDATE dbo.LTestLogMessage SET ErrorMessage3='" + ExistMessage + "' ,ZhiDan='" + ZhiDanCS + "' ,CategoryNum3='" + Category + "',Computer3 = '" + Hostname + Ipaddress + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";//��2
				else if (WorkStationCS == "��װ����")
					strSQL_Write_Barcode = "UPDATE dbo.LTestLogMessage SET ErrorMessage1='" + ExistMessage + "' ,ZhiDan='" + ZhiDanCS + "' ,CategoryNum1='" + Category + "',Computer = '" + Hostname + Ipaddress + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";
				else if (WorkStationCS == "�ϻ������")
					strSQL_Write_Barcode = "UPDATE dbo.LTestLogMessage SET ErrorMessage2='" + ExistMessage + "' ,ZhiDan='" + ZhiDanCS + "' ,CategoryNum2='" + Category + "',Computer2 = '" + Hostname + Ipaddress + "' WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'";

				UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);
			}
			//myado.CloseSheet();
			//����
		}
		else
		{
			//ֱ���ϴ�
			CString strSQL1;
			if (WorkStationCS == "SMT����")
				strSQL1 = "Insert into dbo.LTestLogMessage(SN,SoftModel,Version,ZhiDan,ErrorMessage3,CategoryNum3,Computer3)\
						  							 values('" + ChipRfIDCurrent + "','" + MachineType_CS + "','" + Software_Version[DataUpNum] + "','" + ZhiDanCS + "','" + ExistMessage + "','" + Category + "','" + Hostname + Ipaddress + "')";//��3
			else if (WorkStationCS == "��װ����")
				strSQL1 = "Insert into dbo.LTestLogMessage(SN,SoftModel,Version,ZhiDan,ErrorMessage1,CategoryNum1,Computer)\
						  							 values('" + ChipRfIDCurrent + "','" + MachineType_CS + "','" + Software_Version[DataUpNum] + "','" + ZhiDanCS + "','" + ExistMessage + "','" + Category + "','" + Hostname + Ipaddress + "')";
			
			else if (WorkStationCS == "�ϻ������")
				strSQL1 = "Insert into dbo.LTestLogMessage(SN,SoftModel,Version,ZhiDan,ErrorMessage2,CategoryNum2,Computer)\
						  						  							 values('" + ChipRfIDCurrent + "','" + MachineType_CS + "','" + Software_Version[DataUpNum] + "','" + ZhiDanCS + "','" + ExistMessage + "','" + Category + "','" + Hostname + Ipaddress + "')";


			//myado.OpenSheet("select * from dbo.LTestLogMessage");
			UP_Barcode = myado.Execute(strSQL1, &var);
			//myado.CloseSheet();
		}
		if (UP_Barcode == TRUE)
		{
			//�ٴμ��
			/*Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version from dbo.LTestLogMessage WHERE SN ='"+ChipRfIDCurrent+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' ");//��3
			Barcode_Check_UP=myado.Find("SN='"+ChipRfIDCurrent+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			myado.CloseSheet();
			*/
			if (var.intVal<1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}

		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "SN:" + ChipRfIDCurrent + "�ϴ�������ϢOK...", DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "SN:" + ChipRfIDCurrent + "�ϴ�������ϢFAIL", DataUpNum);//LogShow_exchange(m_Result,Final_Result_Control,5
			return FALSE;
		}

		//�ϴ�������Ϣ
		///////////////////////////////////////////////////////////
	}
	return TRUE;
}

BOOL IMEIWrite_MulAT::GetExistError(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, CString *ExistMessage, CString ChipRfIDbg)//---------2
{
	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[DataUpNum];
	}
	_variant_t var;
	CString FieldName, FieldValue;
	myado.OpenSheet("select * from dbo.LTestLogMessage WHERE SN ='" + ChipRfIDCurrent + "' AND Version='" + Software_Version[DataUpNum] + "'AND SoftModel='" + MachineType_CS + "'");
	//var= myado.m_pRec->GetCollect(_variant_t((long)5));
	FieldName.Format("ErrorMessage%d", 1);
	var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
	if (var.vt != VT_NULL)
	{
		FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
		//LogShow_exchange(m_Result,Final_Result_Control,5,"�������д����ֶ�:ErrorMessage1���ɹ���ȡ:"+FieldValue,DataUpNum);
		*ExistMessage = FieldValue;
	}
	else
	{
		//LogShow_exchange(m_Result,Final_Result_Control,5,"�������в����ڴ�����Ϣ",DataUpNum);

	}
	myado.CloseSheet();//---------2
	return TRUE;
}

void IMEIWrite_MulAT::Voice_Ini()
{
	pVoice = NULL;
	if (FAILED(::CoInitialize(NULL)))
		return;
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	pVoice->SetVolume(100);	//0~100
	pVoice->SetRate(5);		//-10~+10
}
void IMEIWrite_MulAT::Voice_Speak(CString Text)
{
	EnterCriticalSection(&SPEAKVOICE);
	if (SUCCEEDED(hr))
	{
		hr = pVoice->Speak(Text.AllocSysString(), SPF_ASYNC, NULL);
	}
	LeaveCriticalSection(&SPEAKVOICE);
	//::CoUninitialize();
}




/*�˵���*/

//��ȡ�����̺߳���
UINT static __cdecl OnGetWebSetting_Thread(LPVOID pParam)
{
	BOOL flag;

	IMEIWrite_MulAT* Mead_Main_Win = (IMEIWrite_MulAT*)pParam;
	Mead_Main_Win->OnGetWebSetting_ThreadFun();
	return 0;
}

//����RDA͸���õ��ǻص������ķ�ʽ�����������ȡ����תΪ�߳�ģʽ
void IMEIWrite_MulAT::OnGetWebSetting_ThreadFun()
{
	CString Serial_Order_Return, Serial_Order_ReturnTemp;//Serial_Order_Return������ʹ�õĲ�����Serial_Order_ReturnTemp�Ǹ�����ָ���õĲ���
	CString OrderNumbersControlCS;
	CString StrMac;
	GetDlgItemText(IDC_SCANDATA_EDIT, StrMac);

	UpdateData(TRUE);
	if (GetBluetoothCheckValue == TRUE || (GetDongleCheckValue == TRUE&&hScanGun[0] != NULL))
	{
		if (hScanGun[0] == NULL)
		{
			AfxMessageBox("���ȴ򿪴���ɨ��ǹ��");
			goto WEBGETFAILE;
		}
		if (StopSign[0] == FALSE)
		{
			AfxMessageBox("���ȹرմ����̣߳�");
			goto WEBGETFAILE;
		}
		if (Thread_State[0] == TRUE)
		{
			AfxMessageBox("�߳�1�����Ѿ���");
			goto WEBGETFAILE;
		}
		if (StrMac == "")
		{
			AfxMessageBox("����ɨ��MAC��ַ");
			goto WEBGETFAILE;
		}
	}


	if (GetIp_Name(Hostname, Ipaddress) == FALSE)
	{
		AfxMessageBoxHint("��ȡ������Ϣʧ�ܣ���");
		goto WEBGETFAILE;
	}
	else
	{
		AfxMessageBoxHint(Hostname + Ipaddress);
	}

	OrderNumbersControl.GetWindowTextA(OrderNumbersControlCS);
	ReturnCodeControl.GetWindowTextA(ReturnCodeCS);
	if (OrderNumbersControlCS == "")
	{
		AfxMessageBoxHint("�������붩����");
		goto WEBGETFAILE;
	}
	OrderNumbersControlCS += ReturnCodeCS;

	//�����������Ե����ӻ�����������ж�
	if (GetBluetoothCheckValue == TRUE)
	{
		//AfxMessageBox("����ʾ��ɨ����ɨ��MAC��ַ��");

		BLEGetSettingFlag = 0;
		//BluetoothHint(0,"��ɨ");
		//while (BLEGetSettingFlag == 0)
		//{
		//	Get_ScanGun_Data(&m_Result1, &Final_Result_Control1, 0, "\r");
		//	Sleep(500);
		//}
		BluetoothCoreFunction(StrMac);

		if (BLEGetSettingFlag == 2 || BLEGetSettingFlag == -1)
		{
			if (BLEGetSettingFlag == 2)
			{
				AfxMessageBox("��������ʧ�ܣ�����͸��ģ���������豸");
			}

			goto WEBGETFAILE;
		}
		BluetoothHint(0, "����");
	}

	//Dongle�����������ʼ������
	if (GetDongleCheckValue == TRUE || GetDongleScanCheckValue == TRUE)
	{
		DongleInitSetting();
	}

	//���������һ��ʱ����β�ִ�У�ֱ��ʹ��ADCT���س������İ汾��
	if (g_ADCTFlag != 1)
	{
		if (Thread_State[0] == TRUE)
		{
			AfxMessageBox("�߳�1�����Ѿ���");
			goto WEBGETFAILE;
		}
		if (OPen_Serial_Port(&m_Port1, &m_Baud1, 0, 0, GetRDAHostCheckValue) == TRUE)//OnGetWebSetting
		{
			//Dongle����������������ʱ��������������Ӻͳ�ʼ��������ʧ�ܾ�ֱ���˳�
			if (GetDongleCheckValue == TRUE)
			{
				DongleConnectFlag[0] = TRUE;
				DongleInit(0);

				//ɨ��ǹҪ�����︳ֵ
				if (hScanGun[0] != NULL)
					GetDlgItem(IDC_SCANDATA_EDIT)->GetWindowTextA(DongleInfo[0][0]);

				if (DongleScan(0, DongleConnectRssi, "") == FALSE)
				{
					if (hScanGun[0] != NULL)
						DongleInfo[0][0] = "";
					AfxMessageBox("��������ʧ�ܣ�����͸��ģ���������豸");
					goto WEBGETFAILE;
				}
			}
		}
		else
		{
			AfxMessageBox("�򿪴���ʧ��");
			goto WEBGETFAILE;
		}

		//Ҫ����һ���Ƿ��õ��������豸
		if (GetDongleCheckValue == FALSE)
		{
			Send_Serial_Order(&Serial_Order_Return, "AT^GT_CM=VERSION", 0, "NULL", "", 500, GetDongleCheckValue, GetRDAHostCheckValue);
		}
		else if (GetDongleCheckValue == TRUE)
		{
			Send_Serial_Order(&Serial_Order_Return, DongleSPCommand[3][0], 0, (LPSTR)(LPCTSTR)DongleSPCommand[3][1], "", DongleSpCommandSleepTime[3], 1);
			if (Serial_Order_Return == "FAIL")
				goto WEBGETFAILE;
			CString VersionTemp = Vaule_Return_Count_CS[0];
			DongleDisConnect(0);
			Vaule_Return_Count_CS[0] = DongleValueAnalyze("�ַ�ת��", VersionTemp);//��ס�汾��Ҫ�����ַ�ת��
		}

		/*
		int pos=Vaule_Return_Count_CS[0].Find(':');
		if(pos>=0)
		{
		Vaule_Return_Count_CS[0]=Vaule_Return_Count_CS[0].Mid(pos+1);
		pos=Vaule_Return_Count_CS[0].Find(' ');
		if(pos>=0)
		Software_Version[0]=Vaule_Return_Count_CS[0].Left(pos);
		else
		Software_Version[0]=Vaule_Return_Count_CS[0].Left(22);
		}
		else
		*/
		//////////////////////////////////////////////////
		{
			int pos = Vaule_Return_Count_CS[0].Find(']');
			if (pos >= 0)
				Vaule_Return_Count_CS[0] = Vaule_Return_Count_CS[0].Mid(pos + 1);
			pos = Vaule_Return_Count_CS[0].Find('[');
			if (pos >= 0)
				Software_Version[0] = Vaule_Return_Count_CS[0].Left(pos);
			else
				Software_Version[0] = Vaule_Return_Count_CS[0];
		}


		Software_Version[0].Replace("\r", "");
		Software_Version[0].Replace("\n", "");

		//����������������ԣ��������ȡ�汾�ź󣬾Ϳ��Է��������Ͽ�ָ����Ͽ�͸��ģ���������������
		if (GetBluetoothCheckValue == TRUE)
		{
			BluetoothDisconnect(0);
		}

		BOOL Return;
		if (hPort[0] != NULL || GetRDAHostCheckValue == TRUE)
			Return = CloseHandleControl(hPort[0], GetRDAHostCheckValue, 0);					//���Գɹ���				//���Գɹ���
		hPort[0] = NULL;
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBoxHint("�رմ���ʧ�ܣ����²���USB�ɽ������");
		}
	}
	//���ϵ����������ִ�У�����һ�Ļ�ֱ�ӻ�ȡ�汾
	else if (g_ADCTFlag == 1)
	{
		Software_Version[0] = m_SoftVersion;
	}

	if (Serial_Order_Return != "FAIL")//�ɹ�
	{
		if (Software_Version[0] != "")
		{
			UpdateData(TRUE);
			CString m_dbNew;
			WorkStationControl.GetWindowTextA(WorkStationCS);
			if (WorkStationCS == "")WorkStationControl.GetWindowTextA(WorkStationCS);
			if (WorkStationCS == "SMT����")
				m_dbNew = "NetMarkIMEI";
			else if (WorkStationCS == "��װ����")
				m_dbNew = "NetMarkIMEI2";
			else if (WorkStationCS == "�ϻ������")
				m_dbNew = "NetMarkIMEI2";
			else if (WorkStationCS == "�з�����")
				m_dbNew = "NetMarkIMEI3";
			else
			{
				AfxMessageBoxHint("����ѡ��վλ��");
				goto WEBGETFAILE;
			}
			if (GetSettingFromDB(IMEI_Setdlg.myado, IMEI_Setdlg.m_server, m_dbNew, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd, Software_Version[0] + OrderNumbersControlCS, &m_Result1, &Final_Result_Control1) == TRUE)
			{
				Software_VersionFull = Software_Version[0];

				PowerControlInitSetting();

				AfxMessageBoxHint("��ȡ�ɹ������Կ�ʼ������");
				//else if (g_ADCTFlag == 1)
				//{
				//	SendADCTHint("")
				//}
				EnableWindow_StartALL(TRUE);
				m_bVar = 1;//ֻ�н��뵽���������������ȡ�ɹ�

				//Dongle�����ǵ����һ��ɨ��ǹ����
				if (GetDongleCheckValue == TRUE)
				{
					if (hScanGun[0] != NULL)
						GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("");
				}

				//ֻ�������������ԲŻ�ִ�����
				if (GetBluetoothCheckValue == TRUE)
				{
					BLEGetSettingEndFlag = 1;
					AfxMessageBox("ע�⣡ֻ�е��˿���ʾ��ɨʱ�ſ���ɨ��MAC��ַ����");
					MacScanEnbleFlag = TRUE;
					BluetoothHint(0, "��ɨ");
				}
				return;
			}
		}
	}
	else
	{
		AfxMessageBoxHint("��ȡ����汾ʧ�ܣ���");
		goto WEBGETFAILE;
	}

	//��ΪҪ��Ӻܶ���ͬ�Ķ���������ʧ�ܵ�ʱ������ĳ�����goto�ķ���
WEBGETFAILE:
	if (GetBluetoothCheckValue == TRUE)
	{
		BLEGetSettingEndFlag = 0;
		BLEGetSettingFlag = -1;
		MacScanEnbleFlag = TRUE;
		BluetoothHint(0, "W");
	}
	//if (GetDongleCheckValue == TRUE)
	//{
	//	DongleDisConnect(0);
	//}
	StartA_Control.EnableWindow(FALSE);
	m_bVar = 0;
	return;
}

//��ȡ���ã������ȡ
void IMEIWrite_MulAT::OnGetWebSetting()
{
	AfxBeginThread(OnGetWebSetting_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

BOOL IMEIWrite_MulAT::GetSettingFromDB(CAdoInterface& myado, CString m_server, CString m_db, CString m_user, CString m_pwd, CString SoftwareVer, CEdit* m_Result, CEdit* Final_Result_Control)//---------3
{
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		CString DB_FAIL = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", m_server, m_db, m_user, m_pwd);
		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�����������ݿ�.......", 0);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "���ݿ�����������...", 0);
		}

		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 250, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", 0);
			return FALSE;//GetSettingFromDB
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (SoftwareVer == "")
		{
			LogShow_exchange(m_Result, Final_Result_Control, 250, "SN��������汾Ϊ��", 0);
			return FALSE;
		}
		////////////////////////////////���RFID�����͡�����汾�Ƿ��Ѿ�����//////////////////////////////
		CString DBLog;

		myado.OpenSheet("select * from [" + m_db+"].[dbo].[TestSystemSettingFunc] WHERE SoftWare='" + SoftwareVer + "'");
		BOOL Barcode_Check = myado.Find("SoftWare='" + SoftwareVer + "'");
		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_Result, Final_Result_Control, 5, "�������д��ڴ�����汾:" + SoftwareVer + "����ʼ��ȡ����...", 0);
		}
		else
		{
			LogShow_exchange(m_Result, Final_Result_Control, 250, "�������в����ڴ�����汾:" + SoftwareVer, 0);
			myado.CloseSheet();
			return FALSE;
		}
		////////////////////////////////���RFID�Ƿ��Ѿ�����//////////////////////////////
		BOOL UP_Barcode = TRUE;
		//BOOL Barcode_Check_UP;
		if (Barcode_Check == TRUE)
		{
			_variant_t var;
			CString FieldName, FieldValue;
			myado.OpenSheet("select * from [" + m_db + "].[dbo].[TestSystemSettingFunc] WHERE SoftWare='" + SoftwareVer + "'");
			var = myado.m_pRec->GetCollect(_variant_t((long)1));
			if (var.vt != VT_NULL)
			{
				FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
				if (g_ADCTFlag == FALSE)
				LogShow_exchange(m_Result, Final_Result_Control, 5, "�������д����ֶ�:MachineName���ɹ���ȡ:" + FieldValue, 0);

				MachineType_CS = FieldValue;
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 250, "�������в������ֶ�:MachineName", 0);
				myado.CloseSheet();
				return FALSE;
			}
			var = myado.m_pRec->GetCollect(_variant_t((long)2));
			if (var.vt != VT_NULL)
			{
				FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
				if (g_ADCTFlag == FALSE)
				LogShow_exchange(m_Result, Final_Result_Control, 5, "�������д����ֶ�:Station���ɹ���ȡ:" + FieldValue, 0);
				CString MachineNameC;
				int pos = FieldValue.Find("}}");
				if (pos >= 0)
				{
					MachineNameC = FieldValue.Left(pos);
					FieldValue = FieldValue.Mid(pos + 2);
				}
				else
				{
					MachineNameC = "NULL";
				}
				if (MachineType_CS != MachineNameC)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 250, "�������л��ͻ�ȡʧ��", 0);
					myado.CloseSheet();
					return FALSE;
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 5, "�������л��ͻ�ȡ�ɹ�:" + MachineType_CS, 0);
				}
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 250, "�������в����ڴ˶�Ӧ����", 0);
				myado.CloseSheet();
				return FALSE;
			}
			int OrderNUM = atoi(FieldValue.Left(2));
			int OrderNUMTemp = 0;
			//CString ATCommandList_CSDBCompare;
			ATCommandList_CSDBCompare = FieldValue.Mid(2);
			CString ATCommandList_CSDB = "";

			int i = 0;
			for (i = 0; i<SETTING_ITEM_MAX; i++)//for(i=0;i<32;i++)
			{

				FieldName.Format("Setting%d", i);
				var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
				if (var.vt != VT_NULL)
				{
					FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
					if (g_ADCTFlag==FALSE)
					LogShow_exchange(m_Result, Final_Result_Control, 5, "�������д����ֶ�:" + FieldName + "���ɹ���ȡ:" + FieldValue, 0);

					if (FieldValue != "")
						ATCommandList_CSDB += (FieldValue);
					else
						break;
				}
				else
					break;

			}
			if (1)
			{
				if (g_ADCTFlag != 1 && GetBluetoothCheckValue == FALSE)
				{
					AfxMessageBox(ATCommandList_CSDB + "\r\n==\r\n" + ATCommandList_CSDBCompare);
					CString NUMtem, NUMtem2;
					NUMtem.Format("%d", i);
					NUMtem2.Format("%d", OrderNUM);
					AfxMessageBox(NUMtem + "==" + NUMtem2);
				}
				if (OrderNUM != i)
				{
					myado.CloseSheet();
					return FALSE;
				}
				else if (ATCommandList_CSDB != ATCommandList_CSDBCompare)
				{
					myado.CloseSheet();
					LogShow_exchange(m_Result, Final_Result_Control, 250, "�������������ȡʧ��" + ATCommandList_CSDB + "==" + ATCommandList_CSDBCompare, 0);
					return FALSE;
				}
				else
				{
					myado.CloseSheet();
					GetATSetting(ATCommandList_CSDB);
					LogShow_exchange(m_Result, Final_Result_Control, 256, "...", 0);
					LogShow_exchange(m_Result, Final_Result_Control, 5, "��ע�⣡����\r\n��ע�⣡����\r\n��ע�⣡����\r\n��ǰ����Ϊ��" + paraArray[2].Low_Limit_Value, 0);
					if (g_ADCTFlag != 1 && GetBluetoothCheckValue == FALSE)
					{
						AfxMessageBox(paraArray[2].Low_Limit_Value);
					}
					return TRUE;
				}
			}
			myado.CloseSheet();
		}
	}
	else
	{
		LogShow_exchange(m_Result, Final_Result_Control, 250, "û�����÷�����", 0);
		return FALSE;
	}
	return FALSE;//myado---------3
}


BOOL IMEIWrite_MulAT::GetATSetting(CString SettingChar)
{
	CString Check_item = _T("");//�Ƿ�ѡ��
	CString strValue_low = _T("");//��������ֵ
	CString strValue_high = _T("");//��������ֵ
	CString strValue_OtherITEM = _T("");//ATָ��

	CString strValue;
	//SettingChar.Replace(" ","");
	SettingChar.Replace("\r", "");
	SettingChar.Replace("\n", "");
	strValue = SettingChar;
	for (int i = 0; i<ITEMMAX; i++)
	{
		int npos = strValue.Find("@@");
		if (npos >= 0)
		{
			Check_item = strValue.Left(npos);
			Check_item = Check_item.Mid(8);
			strValue = strValue.Mid(npos + 2);
		}
		else
		{
			Check_item = "";
		}
		npos = strValue.Find("@@");
		if (npos >= 0)
		{
			strValue_low = strValue.Left(npos);
			strValue = strValue.Mid(npos + 2);
		}
		else
		{
			strValue_low = "";
		}
		npos = strValue.Find("@@");
		if (npos >= 0)
		{
			strValue_high = strValue.Left(npos);
			strValue = strValue.Mid(npos + 2);
		}
		else
		{
			strValue_high = "";
		}
		npos = strValue.Find("}}");
		if (npos >= 0)
		{
			strValue_OtherITEM = strValue.Left(npos);
			strValue = strValue.Mid(npos + 2);
		}
		else
		{
			strValue_OtherITEM = "";
		}

		npos = Check_item.Find("=");
		if (npos>0)
		{
			paraArray[i].showName = Check_item.Left(npos);
		}
		else
		{
			paraArray[i].showName = Check_item;
		}
		paraArray[i].Low_Limit_Value = strValue_low;	//����������ֵ
		paraArray[i].High_Limit_Value = strValue_high;	//����������ֵ
		paraArray[i].Other_ITEM = strValue_OtherITEM;	//�����Զ���
		if ((Check_item != "") && (Check_item.Find("false") == -1))
			paraArray[i].ifCheck = true;					//ѡ��       
		else
			paraArray[i].ifCheck = false;					//ûѡ��
	}
	return TRUE;
}


void  IMEIWrite_MulAT::SaveConfig_Change()
{
	char szName[MAX_PATH] = _T("");
	CString  szPath = _T("");

	GetModuleFileName(NULL, szName, MAX_PATH);

	szPath = szName;
	szPath = szPath.Mid(0, szPath.ReverseFind('\\') + 1);


	CString strLoadFile = szPath + STRING_PATH_CONFIG;

	GPS_SNRLimit.GetWindowTextA(GPS_SNRLimitCS);
	WIFI_RSSILimit.GetWindowTextA(WIFI_RSSILimitCS);
	// д��INI�ļ�
	CIniFile m_fIniFile(strLoadFile);
	m_fIniFile.WriteString(STRING_SECTION_CONFIG, "GPS_SNRLimit", GPS_SNRLimitCS);
	m_fIniFile.WriteString(STRING_SECTION_CONFIG, "WIFI_RSSILimit", WIFI_RSSILimitCS);
}

void IMEIWrite_MulAT::LoadConfig_Change()
{

	int nCommandNum = 0;


	CString strCommand = _T("");
	CString strValue = _T("");

	CString strName = _T("");
	CStringArray strCommandAry;



	char szName[MAX_PATH] = _T("");
	CString  szPath = _T("");
	GetModuleFileName(NULL, szName, MAX_PATH);
	szPath = szName;
	szPath = szPath.Mid(0, szPath.ReverseFind('\\') + 1);
	CString strLoadFile = szPath + STRING_PATH_CONFIG;
	CIniFile m_fIniFile(strLoadFile);

	m_fIniFile.ReadSection(STRING_SECTION_CONFIG, strCommandAry);
	nCommandNum = strCommandAry.GetSize();
	for (int i = 0; i<nCommandNum; i++)
	{
		strCommand = strCommandAry[i];//���
		if (strCommand == "GPS_SNRLimit")
		{
			GPS_SNRLimitCS = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			GPS_SNRLimit.SetWindowTextA(GPS_SNRLimitCS);
		}
		else if (strCommand == "WIFI_RSSILimit")
		{
			WIFI_RSSILimitCS = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			WIFI_RSSILimit.SetWindowTextA(WIFI_RSSILimitCS);
		}
	}
	UpdateData(false);

}

bool IMEIWrite_MulAT::LoadDll()
{
	password_Handle = LoadLibrary("NEWTTL_encrypt.dll");
	if (!password_Handle)
	{
		AfxMessageBox("Load NEWTTL_encrypt.dll fail!");
		return false;
	}
	GetNEWTTL_encrypt_16 = (unsigned short int(__stdcall *)(const unsigned char* pData, int nLength))GetProcAddress(password_Handle, "GetNEWTTL_encrypt_16");
	return true;
}

char  IMEIWrite_MulAT::HexToASCII(unsigned char  data_hex)
{
	char  ASCII_Data;
	ASCII_Data = data_hex & 0x0F;
	if (ASCII_Data<10)
		ASCII_Data = ASCII_Data + 0x30;			//��0--9��
	else
		ASCII_Data = ASCII_Data + 0x37;			//��A--F��
	return ASCII_Data;
}

void IMEIWrite_MulAT::HexGroupToString(char *OutStrBuffer, unsigned char *InHexBuffer, unsigned int HexLength)
{
	unsigned int i, k = 0;
	for (i = 0; i<HexLength; i++)
	{
		OutStrBuffer[k++] = HexToASCII((InHexBuffer[i] >> 4) & 0x0F);
		OutStrBuffer[k++] = HexToASCII(InHexBuffer[i] & 0x0F);
	}
	OutStrBuffer[k] = '\0';
}

BOOL IMEIWrite_MulAT::StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength)
{
	unsigned int i, k = 0;
	unsigned char HByte, LByte;
	if (strLength % 2 != 0)
		return FALSE;

	for (i = 0; i<strLength; i = i + 2)
	{
		if (InStrBuffer[i] >= '0' && InStrBuffer[i] <= '9')
		{
			HByte = InStrBuffer[i] - '0';
		}
		else if (InStrBuffer[i] >= 'A' && InStrBuffer[i] <= 'F')
		{
			HByte = InStrBuffer[i] - 'A' + 10;
		}
		else
		{
			HByte = InStrBuffer[i];
			return FALSE;
		}
		HByte = HByte << 4;
		HByte = HByte & 0xF0;
		if (InStrBuffer[i + 1] >= '0' && InStrBuffer[i + 1] <= '9')
		{
			LByte = InStrBuffer[i + 1] - '0';
		}
		else if (InStrBuffer[i + 1] >= 'A' && InStrBuffer[i + 1] <= 'F')
		{
			LByte = InStrBuffer[i + 1] - 'A' + 10;
		}
		else
		{
			LByte = InStrBuffer[i];
			return FALSE;
		}
		OutHexBuffer[k++] = HByte | LByte;
	}
	return TRUE;
}

/*
Function Name:unsigned int   SeperateToHex(const char *src, unsigned char *des)
Tasks: ASCII to Hex converter

ASCII Sheet Number
A-Z:65-90
a-z:97-122
0-9:48-57
space:32
enter:
*/
unsigned int IMEIWrite_MulAT::SeperateToHex(const char *src, unsigned char *des)
{
	unsigned int i;
	unsigned int Src_Length;
	unsigned int Des_Length;
	unsigned char tmp_h;
	unsigned char tmp_l;
	tmp_h = 0x00u;
	tmp_l = 0x00u;
	unsigned int flag = 0;
	Des_Length = 0;
	//Src_Length=sizeof(src)/sizeof(char) ;//�м䲻���пո�
	//Src_Length=sizeof(src);
	Src_Length = strlen(src);
	for (i = 0; i<Src_Length; i++)
	{
		if (*src == 32)///space
		{
			if (2 == flag)///
			{
				*des++ = (tmp_h << 4) + (tmp_l & 0x0F);
				Des_Length++;
			}
			else if (1 == flag)
			{
				*des++ = tmp_h;
				Des_Length++;
			}
			else///in case of more than 2 spaces
			{
				;
			}
			tmp_h = 0x00u;
			tmp_l = 0x00u;
			src++;
			flag = 0;
		}
		else if (*src>47 && *src<58)///0-9
		{
			if (0 == flag)
			{
				tmp_h = 0x00 + *src - 48;
				flag = 1;
			}
			else
			{
				tmp_l = 0x00 + *src - 48;
				flag = 2;
			}

			if (i == Src_Length - 1)//���û�пո�
			{
				if (2 == flag)///
				{
					*des++ = (tmp_h << 4) + (tmp_l & 0x0F);
					Des_Length++;
				}
				else if (1 == flag)
				{
					*des++ = tmp_h;
					Des_Length++;
				}
				else///in case of more than 2 spaces
				{
					;
				}
				tmp_h = 0x00u;
				tmp_l = 0x00u;
				src++;
				flag = 0;
			}
			src++;
		}

		else if (*src>64 && *src<71)///A-F,
		{
			if (0 == flag)
			{
				tmp_h = 0x0A + *src - 65;
				flag = 1;
			}
			else
			{
				tmp_l = 0x0A + *src - 65;
				flag = 2;
			}
			if (i == Src_Length - 1)//���û�пո�
			{
				if (2 == flag)///
				{
					*des++ = (tmp_h << 4) + (tmp_l & 0x0F);
					Des_Length++;
				}
				else if (1 == flag)
				{
					*des++ = tmp_h;
					Des_Length++;
				}
				else///in case of more than 2 spaces
				{
					;
				}
				tmp_h = 0x00u;
				tmp_l = 0x00u;
				src++;
				flag = 0;
			}
			src++;
		}
		else if (*src>96 && *src<103)///a-f
		{
			if (0 == flag)
			{
				tmp_h = 0x0a + *src - 97;
				flag = 1;
			}
			else
			{
				tmp_l = 0x0a + *src - 97;
				flag = 2;
			}
			if (i == Src_Length - 1)//���û�пո�
			{
				if (2 == flag)///
				{
					*des++ = (tmp_h << 4) + (tmp_l & 0x0F);
					Des_Length++;
				}
				else if (1 == flag)
				{
					*des++ = tmp_h;
					Des_Length++;
				}
				else///in case of more than 2 spaces
				{
					;
				}
				tmp_h = 0x00u;
				tmp_l = 0x00u;
				src++;
				flag = 0;
			}
			src++;
		}
		else
		{
			;
		}
	}
	return Des_Length;///return HEX Data length
}


CString IMEIWrite_MulAT::ShowPassWord(CString InputID, CEdit* m_Result, CEdit* Final_Result_Control, int HandleNum)
{
	unsigned short PassWord = 0;
	CString PassWordCS;
	//unsigned char* in=(unsigned char*)(LPCTSTR)InputID;
	//unsigned char in[]={0x0F,0x88,0xBE,0x55,0x21,0xD4,0xDB,0xED,0xD3,0x75,0x13,0x46,0x67,0x28,0xBF,0x63};
	unsigned char OutHexBuffer[256] = { 0 };
	//int Length=SeperateToHex((LPSTR)(LPCTSTR)InputID, OutHexBuffer);
	int Length = InputID.GetLength() / 2;
	StringToHexGroup(OutHexBuffer, (LPSTR)(LPCTSTR)InputID, InputID.GetLength());

	PassWord = GetNEWTTL_encrypt_16(OutHexBuffer, Length);
	PassWordCS.Format("%d", PassWord);
	LogShow_exchange(m_Result, Final_Result_Control, 0, "PASSWORD= " + PassWordCS, HandleNum);
	return PassWordCS;
}

//��ascii��ʽ���ִ�ת����16������
//����:"ab01234" -> "61 62 30 31 32 33 34 "
char* IMEIWrite_MulAT::AscToHexString(char* pInData, char* pOutData)
{
	//char* pIn = pInData;
	//char* pOut = pOutData;
	char pIn[256] = { 0 };
	char pOut[256] = { 0 };
	strcpy(pIn, pInData);

	int hexValue = 0;
	for (int i = 0; i<strlen(pIn); i++)
	{
		hexValue = pIn[i];
		sprintf(pOut + 3 * i, "%02X ", hexValue);
	}
	return pOut;
}
void IMEIWrite_MulAT::OnBnClickedButtonconncpu2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void IMEIWrite_MulAT::OnBnClickedButtondisconncpu2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL IMEIWrite_MulAT::GetIp_Name(CString& m_hostname, CString& m_ipaddress)
{
	char 	szHostname[256];
	BOOL	bGetIpInfo_Success = FALSE;

	m_hostname = "";
	m_ipaddress = "";

	//��ȡIP��Ϣ��
	if (gethostname(szHostname, 256) == 0)
	{
		// get host adresses
		struct hostent * phost;
		int i;

		phost = gethostbyname(szHostname);
		m_hostname = szHostname;

		for (i = 0; phost != NULL && phost->h_addr_list[i] != NULL; i++)
		{
			int j;
			int h_length = 4;

			CString CStr_Tmp;
			CStr_Tmp.Format("%u", i + 1);
			m_ipaddress += "IP";
			m_ipaddress += CStr_Tmp;
			m_ipaddress += ":";

			for (j = 0; j<h_length; j++)
			{
				CString addr;

				if (j > 0)
					m_ipaddress += ".";

				addr.Format("%u", (unsigned int)((unsigned char*)phost->h_addr_list[i])[j]);
				m_ipaddress += addr;
			}
			break;//��ȡһ��IP����
		}
		bGetIpInfo_Success = TRUE;
	}

	return	bGetIpInfo_Success;
}

void IMEIWrite_MulAT::OnBnClickedCheck39()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void IMEIWrite_MulAT::OnBnClickedCheck45()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void IMEIWrite_MulAT::OnBnClickedCheck40()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void IMEIWrite_MulAT::OnBnClickedRdahostCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	BOOL InitFlag;

	//ΪTRUE��ִ�г�ʼ����ΪFALSE���ͷŶ�̬��
	if (GetRDAHostCheckValue == TRUE)
	{
		InitFlag=RdaHostInterface.RDADllInit();
		if (InitFlag == FALSE)
		{
			MessageBox("RDAƽ̨͸��ģ���ʼ��ʧ�ܣ�",NULL);
			GetRDAHostCheckValue == FALSE;
			UpdateData(FALSE);
		}
	}
	else if (GetRDAHostCheckValue == FALSE)
	{
		RdaHostInterface.RDADeinitialization();
	}
}

void IMEIWrite_MulAT::OnCbnSelchangeCombo53()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}


UINT static __cdecl ThreadCopy(LPVOID pParam)
{
	IMEIWrite_MulAT* HostControl = (IMEIWrite_MulAT*)pParam;
	HostControl->FileCopy();
	return 0;
}
void IMEIWrite_MulAT::OnBnClickedBtnCopy()
{
	// TODO: Add your control notification handler code here
	if ((IMEI_Setdlg.DestFile != "") && (PathFileExists(IMEI_Setdlg.DestFile)))
		AfxBeginThread(ThreadCopy, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

}
void IMEIWrite_MulAT::FileCopy()
{
	CString strSrc, strDest, strExt;

	char szName[MAX_PATH] = _T("");
	CString  szPath = _T("");
	GetModuleFileName(NULL, szName, MAX_PATH);
	szPath = szName;
	strSrc = szPath.Mid(0, szPath.ReverseFind('\\') + 1);

	strDest = IMEI_Setdlg.DestFile;
	strExt = "*";
	//TCHAR pszMsg[MAX_PATH];
	if (!strSrc.IsEmpty() && !strDest.IsEmpty() && !strExt.IsEmpty())
	{
		//CTime cTime = CTime::GetCurrentTime();
		//CString strTime = cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		//CString Data = cTime.Format(_T("%Y-%m-%d %H-%M-%S\\"));
		//CString Data = cTime.Format(_T("%Y-%m\\"));
		CopyFileExLBC(strSrc, strDest, strExt);
	}
	SetTimer(32, 60000, NULL);
}

DWORD IMEIWrite_MulAT::CopyFileExLBC(CString strSrcPath, CString strDestPath, CString strExt)
{
	if (!PathFileExists(strDestPath))
	{
		if (!CreateDirectory(strDestPath, NULL))
			return GetLastError();
	}

	CFileFind ff;
	CString szDir = strSrcPath;
	CString strSize = _T("");
	if (szDir.Right(1) != _T("\\"))
		szDir += _T("\\");
	szDir += _T("*.*");

	BOOL res = ff.FindFile(szDir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots()) // Folder 
		{
			CString strFolderPath = ff.GetFilePath();
			CString strFolderName = ff.GetFileName();
			CString strDestSubFolder = strDestPath + _T("\\") + strFolderName;
			if (!PathFileExists(strDestSubFolder))
			{
				if (!CreateDirectory(strDestSubFolder, NULL))
					return GetLastError();
			}
			CopyFileExLBC(strFolderPath, strDestSubFolder, strExt);
		}
		else if (!ff.IsDirectory() && !ff.IsDots()) // File
		{
			// Judge extend name
			CString strFileName = ff.GetFileName();
			CString strFileExt = (strrchr(strFileName, _T('.')) + 1);
			if (strExt == _T("*"))
				strFileExt = strExt;
			if (strFileExt == strExt)
			{
				// Copy files
				CString strFilePath;
				strFilePath = ff.GetFilePath();
				CString strNewFilePath;
				strNewFilePath = strDestPath + _T("\\") + strFileName;
				/*if (!::CopyFile(strFilePath, strNewFilePath, FALSE))
				{
				DWORD dwError = GetLastError();
				if (dwError == ERROR_SHARING_VIOLATION)
				{ continue; }
				return dwError;
				}
				*/
				if (!::CopyFile(strFilePath, strNewFilePath, TRUE))//bFailIfExists Long�������ΪTRUE�����㣩����ôһ��Ŀ���ļ��Ѿ����ڣ��������û�ʧ�ܡ�����Ŀ���ļ�����д
				{
					DWORD dwError = GetLastError();
					if (dwError == ERROR_SHARING_VIOLATION)
					{
						continue;
					}
					continue;//return dwError;
				}
			}
		}
	}
	ff.Close();

	return ERROR_SUCCESS;
}


/*ͼƬ����ģ��*/
/*��������ͷ׷���¹��ܣ�����Socket���������ܺ�ͼ�����ܣ�����Socket���������ܴ���ԭ�еĴ��ڴ��书��*/

//��ȡͼƬ����
BOOL IMEIWrite_MulAT::GetPicFunction_Thread(int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, CString ChipRfIDbg, BOOL DayTimeStatep)//WriteIMEIFunction_Thread
{
	EnterCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	BOOL Result = Get_Pic_Data(m_Result, Final_Result_Control, picdlg, HandleNum, "FFD8", "FFD9", ChipRfIDbg, DayTimeStatep);
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	return Result;
}

PARA_Handle PicHandle;
UINT static __cdecl GetPicFunction(LPVOID pParam)
{
	PARA_Handle* Mead_Main_Win = (PARA_Handle*)pParam;
	IMEIWrite_MulAT* MulAT_Win = (IMEIWrite_MulAT*)Mead_Main_Win->WinHandle;
	MulAT_Win->GetPicFunction_Thread(Mead_Main_Win->HandleNum, Mead_Main_Win->m_Result, Mead_Main_Win->Final_Result_Control, Mead_Main_Win->picdlg, Mead_Main_Win->ChipRfIDBG, Mead_Main_Win->DayTimeStatep);
	return 0;
}
void IMEIWrite_MulAT::StartGetPic(int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control)
{
	LogShow_exchange(m_Result, Final_Result_Control, 0, "��ʼ����ͼƬ�߳�\r\n", HandleNum, "-1", ChipRfID[HandleNum]);
	PicHandle.WinHandle = this;
	PicHandle.HandleNum = HandleNum;
	PicHandle.m_Result = m_Result;
	PicHandle.Final_Result_Control = Final_Result_Control;
	PicHandle.picdlg = &Picdlg[HandleNum];
	PicHandle.ChipRfIDBG = ChipRfID[HandleNum];
	PicHandle.DayTimeStatep = DayTimeState[HandleNum];
	AfxBeginThread(GetPicFunction, (LPVOID)&PicHandle, THREAD_PRIORITY_BELOW_NORMAL, 0, 0, NULL);
	LogShow_exchange(m_Result, Final_Result_Control, 0, "����ͼƬ�߳����\r\n", HandleNum, "-1", ChipRfID[HandleNum]);
}

//��ʾͼƬ����
BOOL IMEIWrite_MulAT::ShowPicFunction_Thread(CString RecString, int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, CString ChipRfIDbg, BOOL DayTimeStatep)//WriteIMEIFunction_Thread
{
	EnterCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	BOOL Result = Show_Pic_Data(RecString, m_Result, Final_Result_Control, picdlg, HandleNum, "FFD8", "FFD9", ChipRfIDbg, DayTimeStatep);
	LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	return Result;
}
UINT static __cdecl ShowPicFunction(LPVOID pParam)
{
	PARA_Handle* Mead_Main_Win = (PARA_Handle*)pParam;
	IMEIWrite_MulAT* MulAT_Win = (IMEIWrite_MulAT*)Mead_Main_Win->WinHandle;//������
	MulAT_Win->ShowPicFunction_Thread(Mead_Main_Win->RecString, Mead_Main_Win->HandleNum, Mead_Main_Win->m_Result, Mead_Main_Win->Final_Result_Control, Mead_Main_Win->picdlg, Mead_Main_Win->ChipRfIDBG, Mead_Main_Win->DayTimeStatep);
	return 0;
}
void IMEIWrite_MulAT::StartShowPic(int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CString RecString)
{
	LogShow_exchange(m_Result, Final_Result_Control, 0, "��ʼ����ͼƬ�߳�\r\n", HandleNum, "-1", ChipRfID[HandleNum]);
	PicHandle.WinHandle = this;
	PicHandle.HandleNum = HandleNum;
	PicHandle.m_Result = m_Result;
	PicHandle.Final_Result_Control = Final_Result_Control;
	PicHandle.picdlg = &Picdlg[HandleNum];
	PicHandle.ChipRfIDBG = ChipRfID[HandleNum];
	PicHandle.DayTimeStatep = DayTimeState[HandleNum];
	PicHandle.RecString = RecString;
	AfxBeginThread(ShowPicFunction, (LPVOID)&PicHandle, THREAD_PRIORITY_BELOW_NORMAL, 0, 0, NULL);
	LogShow_exchange(m_Result, Final_Result_Control, 0, "����ͼƬ�߳����\r\n", HandleNum, "-1", ChipRfID[HandleNum]);
}

BOOL IMEIWrite_MulAT::Show_Pic_Data(CString RecString, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, int HandleNum, CString StartSign, CString EndSign, CString ChipRfIDbg, BOOL DayTimeStatep)		//��ȡͼ��˿�����
{
	LogShow_exchange(m_Result, Final_Result_Control, 0, "��ʼ��һ��0\r\n", HandleNum, "-1", ChipRfIDbg);
	//char OutStringBuffer[PICDATAMAX]={0};

	CString NightDayValue;
	if (DayTimeStatep == TRUE)
		NightDayValue = "����";
	else
		NightDayValue = "ҹ��";
	CTime cTime;
	CString TimeShow;

	PicUnionOK[HandleNum] = FALSE;
	UnionPicPath[HandleNum] = "";				//Ҫ������һ��·��---------��ֹ���κϳ�ʧ��
	PicPath[DayTimeStatep][HandleNum] = "";	//Ҫ�������ͺ�ҹ��·��---��ֹ���λ�ȡʧ��

	cTime = CTime::GetCurrentTime();
	TimeShow.Format("-%02d%02d%02d", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	LogShow_exchange(m_Result, Final_Result_Control, 0, TimeShow + "��ʼ��һ��1\r\n", HandleNum, "-1", ChipRfIDbg);

	if (1)
	{
		cTime = CTime::GetCurrentTime();
		TimeShow.Format("-%02d%02d%02d", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

		//HexGroupToString(OutStringBuffer,(unsigned char *)buffer,dwBytesRead);
		//RecString.Format("%s",OutStringBuffer);//--------------------------------------------------------------ע�⿴�Ƿ���Ҫ����ո�
		LogShow_exchange(m_Result, Final_Result_Control, 5, NightDayValue + TimeShow + "��ʼ����ͼƬ����\r\n", HandleNum, "-1", ChipRfIDbg);

		RecString.Replace(" ", "");

		int npos = RecString.Right(4).Find(EndSign);//FF D9             Right(4)-----�����м�Ҳ��FFD9
		if (npos >= 0)
		{
			RecString = RecString.Left(npos + EndSign.GetLength());
			CString  selPort;
			selPort = RecString;

			npos = selPort.Find(StartSign);//FF D8 
			if (npos >= 0)
			{
				Vaule_Pic[HandleNum] = selPort.Mid(npos);
				LogShow_exchange(m_Result, Final_Result_Control, 5, NightDayValue + TimeShow + "�ɹ���ȡͼƬ����\r\n", HandleNum, "-1", ChipRfIDbg);
			}
			else
			{
				LogShow_exchange(m_Result, Final_Result_Control, 128, NightDayValue + TimeShow + "ͼƬ�����쳣\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
				goto ERRORPRO;
			}


			HEX2JPG3(Vaule_Pic[HandleNum], PicPath[DayTimeStatep][HandleNum], NightDayValue, HandleNum, ChipRfIDbg);
			LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "��16��������ת����JPGͼƬ\r\n", HandleNum, "-1", ChipRfIDbg);
			if (DayTimeStatep == TRUE)//��ҹ�ģ���ɰ������պ�����ͼƬƴ��
			{
				if (PathFileExists(PicPath[0][HandleNum]) != TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "ҹ��ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
					//goto ERRORPRO;
				}
				if (PathFileExists(PicPath[1][HandleNum]) != TRUE)
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, TimeShow + "����ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
					goto ERRORPRO;
				}

				if (1)//if(Jpgunionjpg(PicPath[0][HandleNum],PicPath[1][HandleNum],ChipRfIDbg,ChipRfIDbg,UnionPicPath[HandleNum])==TRUE)
				{
					PicUnionOK[HandleNum] = TRUE;
					LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "�ɹ��ϳ�ͼƬ\r\n", HandleNum, "-1", ChipRfIDbg);

					if (BGConfirmChoose == TRUE)//��̨��ʼȷ�ϣ�1���ȴ������ư�״̬����2������ȷ�Ͻ��棬3����ť���ϴ����ݣ��̴߳���оƬID��ͼƬ��ʾʱ�������,4��ʧ�ܰ�ť
					{
						/////////////////////////////////////////////////////////////
						//�ȴ��Ŵ򿪣��ſ�ʼ��ʾͼƬ
						//WaitForSingleObject(DoorWait[HandleNum].m_hObject,60*1000);//�ȴ�60��INFINITE
						WaitForSingleObject(DoorWait[HandleNum].m_hObject, INFINITE);
						DoorWait[HandleNum].ResetEvent();
						int t = 0;
						while (CPUCommunication("���ư�״̬", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control) == FALSE)
						{

							if (StopSign[HandleNum + 8] == TRUE)
								goto ERRORPRO;
							Sleep(2000);
							t++;
							if ((t>3) && (TestResult[HandleNum] == FALSE))
								break;
						}

						//�ȴ��Ŵ�
						/////////////////////////////////////////////////////////////
						if (TestResult[HandleNum] == FALSE)
						{
							LogShow_exchange(m_Result, Final_Result_Control, 128, TimeShow + "�����߳�ʧ�ܣ�����\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
							goto ERRORPRO;
						}
						else
						{
							TestResult[HandleNum] = FALSE;//��һ�ֲ��Խ�����¸�ֵ
						}


						LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "���߳�ͼƬ��ʼ��ʾ\r\n", HandleNum, "-1", ChipRfIDbg);
						EnterCriticalSection(&SHOWONEPIC);
						int i = 0;

						////////////////////////////////////////////////////
						//���ö�ʱ����ʾͼƬ
						if (PicStaticChoose == TRUE)
						{
							Picdlg[HandleNum].ShowPicTime = atoi(BGShowPicTime[HandleNum]);
							if ((PicPath[0][HandleNum] == "") || PicPath[1][HandleNum] == "")
							{
								LogShow_exchange(m_Result, Final_Result_Control, 5, "������ͼƬ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
								//LeaveCriticalSection(&SHOWONEPIC);
								//goto ERRORPRO;
							}
							Picdlg[HandleNum].PicPath0 = PicPath[0][HandleNum];
							Picdlg[HandleNum].PicPath1 = PicPath[1][HandleNum];
							Picdlg[HandleNum].ShowHandleHun = HandleNum;
							TimerHandleNum = HandleNum;
							SetTimer(48, 1, NULL);
						}
						//���ö�ʱ����ʾͼƬ
						/////////////////////////////////////////////////////
						CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);//������ǰ���
						CPUCommunication("ȷ�ϳɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
						while (i<BGShowPicCount[HandleNum])
						{
							////////////////////////////////////////////////
							//ʧ�ܰ�ť
							BOOL Result = CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
							if (Result == TRUE)
							{
								LogShow_exchange(m_Result, Final_Result_Control, 128, "ȷ��ʧ��", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
								Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
								LeaveCriticalSection(&SHOWONEPIC);
								goto ERRORPRO;
							}
							//ʧ�ܰ�ť
							////////////////////////////////////////////////


							////////////////////////////////////////////////
							//ѭ���ȴ�WIFI��ť
							Final_Result_Control->SetFont(fontsp);
							gColor[HandleNum + 8] = RGB(205, 0, 205);
							Final_Result_Control->SetWindowTextA("��ȷ��");
							Final_Result_Control->UpdateWindow();

							if (Get_Wifi_Data(m_Result, Final_Result_Control, HandleNum, "", "RESET", TRUE) != TRUE)
							{
								if (PicStaticChoose == FALSE)
								{
									Picdlg[HandleNum].ShowPicTime = atoi(BGShowPicTime[HandleNum]);
									if ((PicPath[0][HandleNum] == "") || PicPath[1][HandleNum] == "")
									{
										LogShow_exchange(m_Result, Final_Result_Control, 5, "������ͼƬ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
										//LeaveCriticalSection(&SHOWONEPIC);
										//goto ERRORPRO;
									}
									Picdlg[HandleNum].PicPath0 = PicPath[0][HandleNum];
									Picdlg[HandleNum].PicPath1 = PicPath[1][HandleNum];
									if (Picdlg[HandleNum].DoModal() == -1)
									{
										LogShow_exchange(m_Result, Final_Result_Control, 128, "��������ʧ��", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
										LeaveCriticalSection(&SHOWONEPIC);
										goto ERRORPRO;
									}
								}
								else
								{
									Sleep(1500);
								}
							}
							else//�ϴ�����
							{
								BOOL DBResult = FALSE;
								EnterCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
								if (WorkStationCS == "SMT����")
								{
									DBResult = Data_UpdatePara2(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE, ChipRfIDbg);
									if (DBResult == FALSE)
									{
										Sleep(3000);
										DBResult = Data_UpdatePara2(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, TRUE, ChipRfIDbg);
									}
								}
								else if (WorkStationCS == "��װ����")
								{
									DBResult = Data_UpdatePara(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE, ChipRfIDbg);
									if (DBResult == FALSE)
									{
										Sleep(3000);
										DBResult = Data_UpdatePara(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, TRUE, ChipRfIDbg);
									}
								}
								else if (WorkStationCS == "�ϻ������")
								{
									DBResult = Data_UpdatePara3(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE, ChipRfIDbg);
									if (DBResult == FALSE)
									{
										Sleep(3000);
										DBResult = Data_UpdatePara3(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, TRUE, ChipRfIDbg);
									}
								}
								else if (WorkStationCS == "�з�����")
								{
									DBResult = TRUE;
								}
								else
								{
									LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
									LogShow_exchange(m_Result, Final_Result_Control, 128, "����ѡ��վλ������", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
									Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
									LeaveCriticalSection(&SHOWONEPIC);
									goto ERRORPRO;
								}
								LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);

								if (DBResult == FALSE)
								{
									LogShow_exchange(m_Result, Final_Result_Control, 128, "�ϴ�����ʧ��", HandleNum, "���ܲ���:3-DB", ChipRfIDbg);
									Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
									LeaveCriticalSection(&SHOWONEPIC);
									goto ERRORPRO;
								}
								else
								{
									LogShow_exchange(m_Result, Final_Result_Control, 255, "�ɹ�", HandleNum, "-1", ChipRfIDbg);
									//Sleep(2000);
									//LogShow_exchange(m_Result,Final_Result_Control,0,TimeShow+"�ȴ���һ̨\r\n",HandleNum,"-1",ChipRfIDbg);
									Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//
									Sleep(1000);
									LeaveCriticalSection(&SHOWONEPIC);
									return TRUE;
								}
							}
							//ѭ���ȴ�WIFI��ť
							////////////////////////////////////////////////
							i++;
							if (i == BGShowPicCount[HandleNum])//���һ���ˣ�Ҫʧ��
							{
								Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
								LeaveCriticalSection(&SHOWONEPIC);
								LogShow_exchange(m_Result, Final_Result_Control, 128, "ȷ�ϳ�ʱ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
								goto ERRORPRO;
							}
						}
					}
					else
					{
						return TRUE;
					}
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, TimeShow + "�ϳ�ͼƬʧ��\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
					return FALSE;
				}
			}
			else
			{
				//ReadFile( hPicPort[HandleNum], buffer, PICDATAMAX, &dwBytesRead, NULL);
				//return TRUE;//ҹ��ģʽ�ɹ�
			}
		}
	}
ERRORPRO:
	//if(BGConfirmChoose==TRUE)Sleep(2000);
	LogShow_exchange(m_Result, Final_Result_Control, 128, "��ȡͼƬ��ʱ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
	return FALSE;
}


//��ͼ��˿�OPen_ScanGun_Port
BOOL IMEIWrite_MulAT::OPen_Pic_Port(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum)		
{
	CString sPort, sBaud;
	int port, baud;
	//get port
	m_Port->GetWindowText(sPort);
	sPort = sPort.Right(sPort.GetLength() - 3);
	port = atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"), port);

	//get baud
	m_Baud->GetWindowText(sBaud);
	baud = atoi(sBaud);

	//open com port�򿪶˿�
	hPicPort[HandleNum] = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPicPort[HandleNum] == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port->GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hPicPort[HandleNum] = NULL;
		return FALSE;
	}

	//config the com port
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hPicPort[HandleNum], &dcb);
	SetupComm(hPicPort[HandleNum], PICDATAMAX, 4096);
	PurgeComm(hPicPort[HandleNum], PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
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
	SetCommState(hPicPort[HandleNum], &dcb);

	//set time out struct
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;//OPen_Pic_Port
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hPicPort[HandleNum], &timeouts);

	//SetCommMask(hPicPort[HandleNum], EV_RXCHAR|EV_TXEMPTY );							//�����¼�����������

	//read and the com port
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char buffer[256];

	Sleep(100);
	int i = 0;
	do
	{
		bReadStatus = ReadFile(hPicPort[HandleNum], buffer, 10000, &dwBytesRead, NULL);	//make sure rx data cleaned
		i++;
	} while ((dwBytesRead != 0) && i<10);

	return TRUE;																		//����TRUE���򿪴�������
}

BOOL IMEIWrite_MulAT::Get_Wifi_Data(CEdit* m_Result, CEdit* Final_Result_Control, int HandleNum, CString StartSign, CString EndSign, BOOL InThread)	//��ȡ��ťWIFI����---WIFIȷ�ϱ�ʶ
{
	if (CPUCommunication("ȷ�ϳɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control) == TRUE)
		return TRUE;
	if (InThread == FALSE)EnterCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char *buffer;
	char *p = NULL;
	char buf[1024];

	ResetButtonWIFI[HandleNum] = FALSE;
	//read and the com port
	CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���
	for (int r = 0; r<1; r++)
	{
		memset(buf, 0, sizeof(buf));
		buffer = buf;
		bReadStatus = ReadFile(hPicPort[HandleNum], buffer, 512, &dwBytesRead, NULL);
		if (dwBytesRead != 0)
		{
			if (strstr(EndSign, "NULL"))
				p = strstr(buffer, "\r");
			else
				p = strstr(buffer, EndSign);//RESET
			if (p)
			{
				ResetButtonWIFI[HandleNum] = TRUE;
				CString WIFILog;
				WIFILog.Format("%s\r\n", buffer);
				LogShow_exchange(m_Result, Final_Result_Control, 5, "�յ�WIFI����:" + WIFILog, HandleNum);
				if (InThread == FALSE)LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
				return TRUE;
			}
			else
			{
				Port_Temp += CString(buffer);
				Sleep(200 * r);
				continue;
			}
		}
		else
		{
			Sleep(200 * r);
			continue;
		}
	}
	if (InThread == FALSE)LeaveCriticalSection(&WIFICOMMUNICATE[HandleNum]);
	return FALSE;
}
BOOL IMEIWrite_MulAT::Get_Pic_Data(CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, int HandleNum, CString StartSign, CString EndSign, CString ChipRfIDbg, BOOL DayTimeStatep)		//��ȡͼ��˿�����
{
	LogShow_exchange(m_Result, Final_Result_Control, 0, "��ʼ��һ��0\r\n", HandleNum, "-1", ChipRfIDbg);
	BOOL bReadStatus;
	DWORD dwBytesRead;
	char *buffer;
	char *p = NULL;
	char buf[PICDATAMAX];
	char OutStringBuffer[PICDATAMAX] = { 0 };
	CString RecString = "";

	CString NightDayValue;
	if (DayTimeStatep == TRUE)
		NightDayValue = "����";
	else
		NightDayValue = "ҹ��";
	CTime cTime;
	CString TimeShow;
	//read and the com port
	CString Port_Temp = "";//���ڶ�ȡ���� �Ļ���

	PicUnionOK[HandleNum] = FALSE;
	UnionPicPath[HandleNum] = "";				//Ҫ������һ��·��---------��ֹ���κϳ�ʧ��
	PicPath[DayTimeStatep][HandleNum] = "";	//Ҫ�������ͺ�ҹ��·��---��ֹ���λ�ȡʧ��

	cTime = CTime::GetCurrentTime();
	TimeShow.Format("-%02d%02d%02d", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	LogShow_exchange(m_Result, Final_Result_Control, 0, TimeShow + "��ʼ��һ��1\r\n", HandleNum, "-1", ChipRfIDbg);

	int r = 0;
	for (r = 0; r<11; r++)
	{
		memset(buf, 0, sizeof(buf));
		buffer = buf;
		bReadStatus = ReadFile(hPicPort[HandleNum], buffer, PICDATAMAX, &dwBytesRead, NULL);
		if (dwBytesRead != 0)
		{
			cTime = CTime::GetCurrentTime();
			TimeShow.Format("-%02d%02d%02d", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

			HexGroupToString(OutStringBuffer, (unsigned char *)buffer, dwBytesRead);
			RecString.Format("%s", OutStringBuffer);//--------------------------------------------------------------ע�⿴�Ƿ���Ҫ����ո�
			LogShow_exchange(m_Result, Final_Result_Control, 5, NightDayValue + TimeShow + "��ʼ����ͼƬ����\r\n", HandleNum, "-1", ChipRfIDbg);

			RecString.Replace(" ", "");
			WriteLogIni(HandleNum);
			WriteLog(RecString, HandleNum);
			WriteLogEnd(HandleNum);
			//if(strstr(EndSign,"NULL"))
			//p=strstr(OutStringBuffer,"\r");
			//else
			//p=strstr(OutStringBuffer,EndSign);
			//if(p)
			//int npos=RecString.Right(4).Find(EndSign);//FF D9             Right(4)-----�����м�Ҳ��FFD9
			//int npos=RecString.Right(32).Find(EndSign);
			int npos = RecString.Right(36).Find(EndSign);
			if (npos >= 0)
			{
				SerialPicOver[HandleNum] = TRUE; SerialPic[HandleNum].SetEvent();
				//RecString=RecString.Left(npos+EndSign.GetLength());                //�����м�Ҳ��FFD9
				CString strCommand_Vaule_Return;//���ڷ���ֵ
				strCommand_Vaule_Return = Port_Temp + RecString;

				CString  selPort;
				selPort = strCommand_Vaule_Return;
				//selPort.Replace("\r","");
				//selPort.Replace("\n","");

				npos = selPort.Find(StartSign);//FF D8 
				if (npos >= 0)
				{
					Vaule_Pic[HandleNum] = selPort.Mid(npos);
					LogShow_exchange(m_Result, Final_Result_Control, 5, NightDayValue + TimeShow + "�ɹ���ȡͼƬ����\r\n", HandleNum, "-1", ChipRfIDbg);
				}
				else
				{
					LogShow_exchange(m_Result, Final_Result_Control, 128, NightDayValue + TimeShow + "ͼƬ�����쳣\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
					goto ERRORPRO;
				}


				HEX2JPG3(Vaule_Pic[HandleNum], PicPath[DayTimeStatep][HandleNum], NightDayValue, HandleNum, ChipRfIDbg);
				LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "��16��������ת����JPGͼƬ\r\n", HandleNum, "-1", ChipRfIDbg);
				if (DayTimeStatep == TRUE)//��ҹ�ģ���ɰ������պ�����ͼƬƴ��
				{
					if (PathFileExists(PicPath[0][HandleNum]) != TRUE)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "ҹ��ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
						//goto ERRORPRO;
					}
					if (PathFileExists(PicPath[1][HandleNum]) != TRUE)
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, TimeShow + "����ģʽͼƬ������\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
						goto ERRORPRO;
					}

					if (1)//if(Jpgunionjpg(PicPath[0][HandleNum],PicPath[1][HandleNum],ChipRfIDbg,ChipRfIDbg,UnionPicPath[HandleNum])==TRUE)
					{
						PicUnionOK[HandleNum] = TRUE;
						LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "�ɹ��ϳ�ͼƬ\r\n", HandleNum, "-1", ChipRfIDbg);

						if (BGConfirmChoose == TRUE)//��̨��ʼȷ�ϣ�1���ȴ������ư�״̬����2������ȷ�Ͻ��棬3����ť���ϴ����ݣ��̴߳���оƬID��ͼƬ��ʾʱ�������,4��ʧ�ܰ�ť
						{
							/*
							CEdit* m_ResultPic;
							CEdit* Final_Result_ControlPic;

							if(HandleNum==0)
							{
							m_ResultPic=&m_Result9;
							Final_Result_ControlPic=&Final_Result_Control9;
							}
							else if(HandleNum==1)
							{
							m_ResultPic=&m_Result10;
							Final_Result_ControlPic=&Final_Result_Control10;
							}
							else if(HandleNum==2)
							{
							m_ResultPic=&m_Result11;
							Final_Result_ControlPic=&Final_Result_Control11;
							}
							else if(HandleNum==3)
							{
							m_ResultPic=&m_Result12;
							Final_Result_ControlPic=&Final_Result_Control12;
							}
							else if(HandleNum==4)
							{
							m_ResultPic=&m_Result13;
							Final_Result_ControlPic=&Final_Result_Control13;
							}
							else if(HandleNum==5)
							{
							m_ResultPic=&m_Result14;
							Final_Result_ControlPic=&Final_Result_Control14;
							}
							else if(HandleNum==6)
							{
							m_ResultPic=&m_Result15;
							Final_Result_ControlPic=&Final_Result_Control15;
							}
							else if(HandleNum==7)
							{
							m_ResultPic=&m_Result16;
							Final_Result_ControlPic=&Final_Result_Control16;
							}
							*/
							/////////////////////////////////////////////////////////////
							//�ȴ��Ŵ򿪣��ſ�ʼ��ʾͼƬ
							//WaitForSingleObject(DoorWait[HandleNum].m_hObject,60*1000);//�ȴ�60��INFINITE
							WaitForSingleObject(DoorWait[HandleNum].m_hObject, INFINITE);
							DoorWait[HandleNum].ResetEvent();
							int t = 0;
							while (CPUCommunication("���ư�״̬", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control) == FALSE)
							{

								if ((StopSign[HandleNum + 8] == TRUE) && (t>3))
									goto ERRORPRO;
								Sleep(2000);
								t++;
								if ((t>3) && (TestResult[HandleNum] == FALSE))
									break;
							}

							//�ȴ��Ŵ�
							/////////////////////////////////////////////////////////////
							if (TestResult[HandleNum] == FALSE)
							{
								LogShow_exchange(m_Result, Final_Result_Control, 128, TimeShow + "�����߳�ʧ�ܣ�����\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
								goto ERRORPRO;
							}
							else
							{
								TestResult[HandleNum] = FALSE;//��һ�ֲ��Խ�����¸�ֵ
							}


							LogShow_exchange(m_Result, Final_Result_Control, 5, TimeShow + "���߳�ͼƬ��ʼ��ʾ\r\n", HandleNum, "-1", ChipRfIDbg);
							EnterCriticalSection(&SHOWONEPIC);
							int i = 0;

							////////////////////////////////////////////////////
							//���ö�ʱ����ʾͼƬ
							if (PicStaticChoose == TRUE)
							{
								Picdlg[HandleNum].ShowPicTime = atoi(BGShowPicTime[HandleNum]);
								if ((PicPath[0][HandleNum] == "") || PicPath[1][HandleNum] == "")
								{
									LogShow_exchange(m_Result, Final_Result_Control, 5, "������ͼƬ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
									//LeaveCriticalSection(&SHOWONEPIC);
									//goto ERRORPRO;
								}
								Picdlg[HandleNum].PicPath0 = PicPath[0][HandleNum];
								Picdlg[HandleNum].PicPath1 = PicPath[1][HandleNum];
								Picdlg[HandleNum].ShowHandleHun = HandleNum;
								TimerHandleNum = HandleNum;
								SetTimer(48, 1, NULL);
							}
							//���ö�ʱ����ʾͼƬ
							/////////////////////////////////////////////////////
							CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);//������ǰ���
							CPUCommunication("ȷ�ϳɹ�", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
							while (i<BGShowPicCount[HandleNum])
							{
								////////////////////////////////////////////////
								//ʧ�ܰ�ť
								BOOL Result = CPUCommunication("ȷ��ʧ��", HandleNum, "", "", COM_State[THREAD_NUM], m_Result, Final_Result_Control);
								if (Result == TRUE)
								{
									LogShow_exchange(m_Result, Final_Result_Control, 128, "ȷ��ʧ��", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
									Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
									LeaveCriticalSection(&SHOWONEPIC);
									goto ERRORPRO;
								}
								//ʧ�ܰ�ť
								////////////////////////////////////////////////


								////////////////////////////////////////////////
								//ѭ���ȴ�WIFI��ť
								Final_Result_Control->SetFont(fontsp);
								gColor[HandleNum + 8] = RGB(205, 0, 205);
								Final_Result_Control->SetWindowTextA("��ȷ��");
								Final_Result_Control->UpdateWindow();

								if (Get_Wifi_Data(m_Result, Final_Result_Control, HandleNum, "", "RESET", TRUE) != TRUE)
								{
									if (PicStaticChoose == FALSE)
									{
										Picdlg[HandleNum].ShowPicTime = atoi(BGShowPicTime[HandleNum]);
										if ((PicPath[0][HandleNum] == "") || PicPath[1][HandleNum] == "")
										{
											LogShow_exchange(m_Result, Final_Result_Control, 5, "������ͼƬ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
											//LeaveCriticalSection(&SHOWONEPIC);
											//goto ERRORPRO;
										}
										Picdlg[HandleNum].PicPath0 = PicPath[0][HandleNum];
										Picdlg[HandleNum].PicPath1 = PicPath[1][HandleNum];
										if (Picdlg[HandleNum].DoModal() == -1)
										{
											LogShow_exchange(m_Result, Final_Result_Control, 128, "��������ʧ��", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
											LeaveCriticalSection(&SHOWONEPIC);
											goto ERRORPRO;
										}
									}
									else
									{
										Sleep(1500);
									}
								}
								else//�ϴ�����
								{
									BOOL DBResult = FALSE;
									EnterCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
									if (WorkStationCS == "SMT����")
									{
										DBResult = Data_UpdatePara2(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE, ChipRfIDbg);
										if (DBResult == FALSE)
										{
											Sleep(3000);
											DBResult = Data_UpdatePara2(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, TRUE, ChipRfIDbg);
										}
									}
									else if (WorkStationCS == "��װ����")
									{
										DBResult = Data_UpdatePara(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE, ChipRfIDbg);
										if (DBResult == FALSE)
										{
											Sleep(3000);
											DBResult = Data_UpdatePara(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, TRUE, ChipRfIDbg);
										}
									}
									else if (WorkStationCS == "�ϻ������")
									{
										DBResult = Data_UpdatePara3(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, FALSE, ChipRfIDbg);
										if (DBResult == FALSE)
										{
											Sleep(3000);
											DBResult = Data_UpdatePara3(IMEI_Setdlg.Mulmyado[HandleNum], HandleNum, m_Result, Final_Result_Control, TRUE, ChipRfIDbg);
										}
									}
									else if (WorkStationCS == "�з�����")
									{
										DBResult = TRUE;
									}
									else
									{
										LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);
										LogShow_exchange(m_Result, Final_Result_Control, 128, "����ѡ��վλ������", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
										Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
										LeaveCriticalSection(&SHOWONEPIC);
										goto ERRORPRO;
									}
									LeaveCriticalSection(&UPDATEDB[HandleNum % (THREAD_NUM / 8)]);

									if (DBResult == FALSE)
									{
										LogShow_exchange(m_Result, Final_Result_Control, 128, "�ϴ�����ʧ��", HandleNum, "���ܲ���:3-DB", ChipRfIDbg);
										Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
										LeaveCriticalSection(&SHOWONEPIC);
										goto ERRORPRO;
									}
									else
									{
										LogShow_exchange(m_Result, Final_Result_Control, 255, "�ɹ�", HandleNum, "-1", ChipRfIDbg);
										//Sleep(2000);
										//LogShow_exchange(m_Result,Final_Result_Control,0,TimeShow+"�ȴ���һ̨\r\n",HandleNum,"-1",ChipRfIDbg);
										Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//
										Sleep(1000);
										LeaveCriticalSection(&SHOWONEPIC);
										return TRUE;
									}
								}
								//ѭ���ȴ�WIFI��ť
								////////////////////////////////////////////////
								i++;
								if (i == BGShowPicCount[HandleNum])//���һ���ˣ�Ҫʧ��
								{
									Picdlg[TimerHandleNum].SendMessage(WM_CLOSE);//Picdlg[TimerHandleNum].EndDialog(0);
									LeaveCriticalSection(&SHOWONEPIC);
									LogShow_exchange(m_Result, Final_Result_Control, 128, "ȷ�ϳ�ʱ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
									goto ERRORPRO;
								}
							}
						}
						else
						{
							return TRUE;
						}
					}
					else
					{
						LogShow_exchange(m_Result, Final_Result_Control, 128, TimeShow + "�ϳ�ͼƬʧ��\r\n", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
						return FALSE;
					}
				}
				else
				{
					ReadFile(hPicPort[HandleNum], buffer, PICDATAMAX, &dwBytesRead, NULL);
					return TRUE;//ҹ��ģʽ�ɹ�
				}
			}
			else
			{
				Port_Temp += RecString;//CString(buffer);
				Sleep(200 * r);
				continue;
			}
		}
		else
		{
			Sleep(200 * r);
			continue;
		}
	}
ERRORPRO:
	//if(BGConfirmChoose==TRUE)Sleep(2000);
	SerialPicOver[HandleNum] = TRUE; SerialPic[HandleNum].SetEvent();
	if (r >= 11)
	{
		LogShow_exchange(m_Result, Final_Result_Control, 128, "��ȡͼƬ��ʱ", HandleNum, "���ܲ���:2-TEST", ChipRfIDbg);
	}
	else
	{
		//LogShow_exchange(m_Result,Final_Result_Control,0,TimeShow+"�ȴ���һ̨\r\n",HandleNum,"-1",ChipRfIDbg);
	}
	return FALSE;
}


/***********************************************************************************
fp:  ԴͼƬ��jpg->bin����
out: ���ͼƬ��Ascii��
dstFile:�������ɵ�ͼƬ bin����->jpg

r ��ֻ���ļ������ļ�������ڡ�
r+ �򿪿ɶ�д���ļ������ļ�������ڡ�
rb+ ��д��һ���������ļ���ֻ�����д���ݡ�
rt+ ��д��һ���ı��ļ����������д��
w ��ֻд�ļ������ļ��������ļ�������Ϊ0�������ļ����ݻ���ʧ�����ļ��������������ļ���
w+ �򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������������ļ���
a �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�����ļ�ԭ�ȵ����ݻᱻ������
a+ �Ը��ӷ�ʽ�򿪿ɶ�д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������
wb ֻд�򿪻��½�һ���������ļ���ֻ����д���ݡ�
wb+ ��д�򿪻���һ���������ļ����������д��
wt+ ��д�򿪻��Ž���һ���ı��ļ��������д��
at+ ��д��һ���ı��ļ�������������ı�ĩ׷�����ݡ�
ab+ ��д��һ���������ļ�������������ļ�ĩ׷�����ݡ�
***********************************************************************************/

BOOL IMEIWrite_MulAT::HEX2JPG()
{
	FILE *fp, *out, *dstFile;
	long int size;
	unsigned char ch1, ch2;
	long int i;

	if ((out = fopen("JpgToBin.txt", "wb+")) == NULL)
	{
		printf("can't open the txt file\n");
		return FALSE;
	}
	if ((dstFile = fopen("Recv.jpg", "wb+")) == NULL)
	{
		printf("can't open the dst file\n");
		return FALSE;
	}
	rewind(fp);
	bool FormatRight = false;
	for (i = 0; i<size; i++)
	{
		ch1 = fgetc(fp);
		if (ch1 == 0xff)
		{
			ch2 = fgetc(fp);
			if (ch2 == 0xd8)
			{
				fputc(ch1, dstFile);															//fpΪ�ļ�ָ�룬����ֵ��ִ��fopen()���ļ�ʱ��õġ�
				fputc(ch2, dstFile);
				FormatRight = true;
				break;
			}
		}
	}
	if (FormatRight)
	{
		for (; i<size; i++)
		{
			ch1 = fgetc(fp);
			fputc(ch1, dstFile);
			if (ch1 == 0xff)
			{
				ch2 = fgetc(fp);
				if (ch2 == 0xd9)
					break;
				else
					fputc(ch2, dstFile);
			}
		}
	}
	fclose(out);
	fclose(dstFile);

	return TRUE;
}



/*Socket���յ�����Ƶ���ݰ������洢��source.txt�ļ��У�Ȼ�������ļ�����FFD8��FFD9֮������ݣ�д��dest.jpg�ļ��У�
���ı��ļ��ж�FFD8��FFD9֮������ݣ����浽һ��ͼƬ��*/
BOOL IMEIWrite_MulAT::HEX2JPG2()
{
	unsigned char chBuf[PICDATAMAX] = { 0 };
	unsigned char chBegin[2] = { 0xFF, 0xD8 };
	unsigned char chEnd[2] = { 0xFF, 0xD9 };
	int iLen = 0;
	int iBegin = 0;
	int iEnd = 0;

	FILE* fp = fopen("source.txt", "rb+"); //��д��ʽ�򿪶������ļ�
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END); //�ļ�ָ�붨���ļ���β
		iLen = ftell(fp);       //��ȡ�ļ�����  
		fseek(fp, 0, SEEK_SET); //��λ���ļ���ʼλ��  
		fread(chBuf, 1, iLen, fp);
		int ioffset = 0;        //ƫ����,һ���ƶ������ֽ�

		while (1)
		{
			//���� FF D8
			if (!memcmp(chBuf + ioffset, chBegin, 2))
			{
				iBegin = ioffset;
			}

			//���� FF D9
			if (!memcmp(chBuf + ioffset, chEnd, 2))
			{
				iEnd = ioffset;
				break;
			}

			ioffset += 2;
		}
	}
	fclose(fp);

	fp = fopen("dest.jpg", "wb");
	if (NULL != fp)
	{
		fwrite(chBuf + iBegin, iEnd - iBegin + 2, 1, fp);
	}
	fclose(fp);
	return TRUE;
}



// ��ͼƬ�ļ���16������ʾ
void IMEIWrite_MulAT::JPG2HEX()
{
	// TODO: Add your control notification handler code here
	CString m_sPath;
	CFileDialog dlg(TRUE, "jpg", "*.jpg",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"JPEG�ļ�(*.jpg)|*.jpg|JPEG�ļ�(*.JPG)|*.JPG||", NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_sPath = dlg.GetPathName();
		Invalidate();
	}
	if (!m_sPath.IsEmpty())
	{
		CString tmp, str;
		CFile file;
		file.Open(m_sPath, CFile::typeBinary | CFile::modeRead | CFile::shareDenyNone);
		DWORD dwSize = (DWORD)file.GetLength();
		BYTE buf[PICDATAMAX];
		int count = file.Read(buf, PICDATAMAX);
		file.Close();
		for (int i = 0; i<count; i++)
		{
			tmp.Format("%02X", buf[i]);
			if (strlen(tmp) > 2)
				tmp = tmp.Mid(strlen(tmp) - 2);
			str += tmp;
		}
	}
}

// ��16��������ת����JPGͼƬ
BOOL IMEIWrite_MulAT::HEX2JPG3(CString str, CString& m_sPath, CString NightDay, int HandleNum, CString ChipRfIDbg)
{
	// TODO: Add your control notification handler code here
	CString ChipRfIDCurrent;
	if (ChipRfIDbg != "")
	{
		ChipRfIDCurrent = ChipRfIDbg;
	}
	else
	{
		ChipRfIDCurrent = ChipRfID[HandleNum];
	}
	CTime cTime;
	char m_Time_Date[128];
	char m_Time_Time[128];
	CString m_strModulePath;
	CString LogFile;

	char cReportPath[256] = { 0 };
	DWORD dwAttr;

	char file_name[_MAX_PATH] = { 0 };
	GetModuleFileName(NULL, file_name, _MAX_PATH);
	CString strFileName = file_name;
	int num = strFileName.ReverseFind('\\');

	CString LogNum_CS;
	LogNum_CS.Format("%d", HandleNum);

	m_strModulePath = strFileName.Left(num + 1) + LogNum_CS + "\\";

	cTime = CTime::GetCurrentTime();                                                         //�����Ե������ڵ��ļ�������������
	sprintf(m_Time_Date, "%s%04d%02d%02d", m_strModulePath, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());//��������6λ����һ��Char��
	//����������Ϊ���ֵ��ļ���
	dwAttr = GetFileAttributes(m_Time_Date);
	if (dwAttr == 0xFFFFFFFF)                                                                 //�ļ��в�����   
	{
		CreateDirectory(m_Time_Date, NULL);
	}

	sprintf(m_Time_Time, "%s-%s-%02d%02d%02d", ChipRfIDCurrent, NightDay, cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	m_sPath.Format("%s\\%s.jpg", m_Time_Date, m_Time_Time);


	if (!m_sPath.IsEmpty())
	{
		CFile file;
		file.Open(m_sPath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);

		int dwSize = str.GetLength() / 2;
		BYTE *buf = StrDecToCHex(dwSize, str);
		file.Write(buf, dwSize);
		file.Close();
		//AfxMessageBox("ת���ɹ���");
	}
	return TRUE;
}

// 16���������ַ�ת��
BYTE IMEIWrite_MulAT::ConvertHexChar(BYTE ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else
		return -1;
}

// CStringתBYTE*
BYTE * IMEIWrite_MulAT::StrDecToCHex(int &length, CString str)
{
	BYTE	tmpByte = 0x00;
	int		slen = str.GetLength();

	length = 0;

	//BYTE * h_data = new BYTE[102400];
	BYTE * h_data = new BYTE[slen];

	::memset(h_data, '\0', sizeof(h_data));

	//for (int i=0, j=0; i<102400,j<slen; i++,j++)
	for (int i = 0, j = 0; i<slen, j<slen; i++, j++)
	{
		if (str[j] == ' ')
			++j;
		tmpByte = str[j];
		h_data[i] = ConvertHexChar(tmpByte) << 4;				//����4λ
		if (str[++j] == ' ')
			++j;
		tmpByte = str[j];
		h_data[i] = h_data[i] + (ConvertHexChar(tmpByte) & 0xF);	//ȡ��4λȻ����ӡ�
		length = i + 1;
	}

	return h_data;
}


BOOL IMEIWrite_MulAT::Jpgunionjpg(CString szPathName1, CString szPathName2, CString strInfo, CString PicName, CString& strDestJpgName)
{
	CBasicUse m_bUse;
	CString m_dllNameBmpToJpg;		//ָ����̬�������
	//��ȡ��Ӧ�ó���Ŀ¼�������ڶԻ��򴰿ڱ��򿪺�Ӧ�ó���Ŀ¼�ᱻ��
	CString strCurPath;//CString strCurPath= m_bUse.GetApp_Path();
	char file_name[_MAX_PATH] = { 0 };
	GetModuleFileName(NULL, file_name, _MAX_PATH);
	CString strFileName = file_name;
	int num = strFileName.ReverseFind('\\');

	strCurPath = strFileName.Left(num + 1);


	m_dllNameBmpToJpg = strCurPath + "\\PictureProcessDll.dll";
	CPictureProcess picProcess;


	int nPicCounts = 2;
	CArray<CString, CString> arrPic;
	arrPic.Add(szPathName1);
	arrPic.Add(szPathName2);

	//SetCurrentDirectory(strCurPath);	//�������û�EXE��������Ŀ¼���Ա㶯̬������
	//�õ�Ŀ¼
	CString strPicPath = arrPic.GetAt(0);
	//int nFindPos = strPicPath.ReverseFind('\\');
	int nFindPos = strPicPath.ReverseFind('.');
	strPicPath = strPicPath.Mid(0, nFindPos);

	//JPG TO BMP ���ŵ�Ӧ�ó���Ŀ¼�����ʱĿ¼tempPic��
	CString * sFilePath;

	int nLen = arrPic.GetSize();	//ͼƬ����
	sFilePath = new CString[nLen];
	for (int i = 0; i<nLen; i++)
	{
		sFilePath[i] = arrPic.GetAt(i);
		//��ת��BMP��֮�󷵻ص��ļ�Ŀ¼��Ӧ�ó���Ŀ¼����ʱĿ¼�£���Ϊ��ʱ�ļ�BMP��
		//sFilePath[i] = picProcess.ChangeJPGToBMP(sFilePath[i],m_dllNameBmpToJpg);
		sFilePath[i] = picProcess.ChangeJPGToBMP2(sFilePath[i]);
	}


	//
	//CString strDestBmpName= strPicPath+"\\"+PicName+".bmp";		//�ϳ�bmpͼ����
	//strDestJpgName= strPicPath+"\\"+PicName+".jpg";				//jpgͼ����
	CString strDestBmpName = strPicPath + "ALL.bmp";		//�ϳ�bmpͼ����
	strDestJpgName = strPicPath + "ALL.jpg";				//jpgͼ����

	/////////�ϳ�BMPͼƬ////////////////////////////////
	CCombineImages CombineImages;
	CombineImages.SetpWnd(this);
	CombineImages.SetAddTextFlag(TRUE);
	CombineImages.SetTextInfo(strInfo);

	/////�ϳ�
	//CombineImages.CombineImagesEntry(sFilePath,nLen,strDestBmpName,1);	//������ϳ�
	if (CombineImages.CombineImagesEntry(sFilePath, nLen, strDestBmpName, 0) == FALSE)
	{
		strDestJpgName = "";
		return FALSE;
	}
	Sleep(10);
	//delete tempPicĿ¼�µ�����bmp
	m_bUse.DeleteFilesInFolder("tempPic");
	//bmp to jpg
	//picProcess.ChangeBMPToJPG(strDestBmpName,m_dllNameBmpToJpg);
	picProcess.ChangeBMPToJPG2(strDestBmpName);
	Sleep(10);

	//��ʾ�ϳɵ�BMP���ؼ���
	::DeleteFile(strDestBmpName);
	//HWND hwndStill = NULL;
	//m_Pic.MoveWindow(20,40,1000,200,TRUE);
	//GetDlgItem(IDC_PIC, &hwndStill);
	//picProcess.ShowPicture(strDestBmpName,hwndStill);
	return TRUE;
}


//Dongle������ѡ��
void IMEIWrite_MulAT::OnBnClickedDonglebleCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

void IMEIWrite_MulAT::OnBnClickedDongleblescanCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//��ȡͼƬ��ѡ��
void IMEIWrite_MulAT::OnBnClickedCheck41()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//��̨ȷ�ϸ�ѡ��
void IMEIWrite_MulAT::OnBnClickedCheck42()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//ͼ�ȸ�ѡ��
void IMEIWrite_MulAT::OnBnClickedCheck43()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//��װ��SMT���з�����������
void IMEIWrite_MulAT::OnCbnSelchangeCombo56()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}


/*�������ܵ����Ϻ���*/

//����ʼ��ť��Ҫ��������̼��ϳ�һ������
void IMEIWrite_MulAT::StartButtonGatherFun(int HandleNum)
{
	//������������ͷSOCKET
	if (GetPicChoose == TRUE)
	{
		if (SocketServer[HandleNum]->m_isServerOpen != FALSE)
		{
			SocketServer[HandleNum]->StopServer();
		}

		SocketServer[HandleNum]->StartServer(SocketServerPort[HandleNum]);
	}

	//��ʼ��Dongle����
	if (GetDongleCheckValue == TRUE)
	{
		if (DongleConnectCount[HandleNum] == DongleResetCount || DongleConnectCount[HandleNum] == 0)
		{
			DongleConnectCount[HandleNum] = 0;
			DongleInit(HandleNum);
		}
	}
}

//��ֹͣ��ť��Ҫ��������̼��ϳ�һ������
void IMEIWrite_MulAT::StopButtonGatherFun(int HandleNum)
{
	//ֹͣʱ��������������Ӿ͸��������ͶϿ�ָ��
	if (GetBluetoothCheckValue == TRUE&&BleGetSettingFlag[HandleNum] == 1)
	{
		BleGetSettingFlag[HandleNum] = 0;
		BluetoothDisconnect(HandleNum);
	}

	if (GetDongleScanCheckValue == TRUE&&HandleNum < 8)
	{
		DongleStatusArray[HandleNum] = FALSE;
		DongleTestRssiInfoArray[HandleNum] = "";
		if (hScanGun[0] != NULL)
		{
			deque<int>::iterator pos;
			pos = find(DongleTestDataDeq.begin(), DongleTestDataDeq.end(), HandleNum);
			if (pos != DongleTestDataDeq.end())
				DongleTestDataDeq.erase(pos);
		}
	}
	
	if (0)
	{
		BOOL Return = CloseHandle(hPort[HandleNum]);				//���Գɹ���
		if (Return == TRUE)
		{
		}
		else
		{
			AfxMessageBox("�ر�ʧ�ܣ�");												//�رմ���ʧ��
			return;
		}
		hPort[HandleNum] = NULL;
	}

	//ֹͣʱɾ��ռ�ö����ж�Ӧ�Ķ˿ں�
	if (GetBluetoothCheckValue == TRUE || g_ADCTFlag == 1)
	{
		DeleteComoOccupancydDeq(HandleNum);
	}

	//�ر�Socket������
	if (GetPicChoose == TRUE)
	{
		SocketServer[HandleNum]->CloseRecv();
		SocketServer[HandleNum]->StopServer();
	}

	//�ر�Dongle����
	if (GetDongleCheckValue == TRUE&&DongleInfo[HandleNum][0] != "")
	{
		DongleDisConnect(HandleNum);
	}

	if (HandleNum < 8)
	{
		DongleTestRssiInfoArray[HandleNum] = "";
		m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA("");
	}
}

//ͳһ�Զ��н��й����ڶ��������ݽ���֮ʱ�����ж������߳���û�б��رգ��Ƿ��ֵ���ǰ�����̹߳�����
BOOL IMEIWrite_MulAT::DequeContinueControlFun(int HandleNum, deque<int> &ContinueDeq)
{
	for (int i = 0; i < 25; i++)//��Ҫ���ó���ѭ��,25*400=10�볬ʱ
	{
		if (ContinueDeq.front() == HandleNum)
		{
			return TRUE;
		}
		Sleep(400);
		if (StopSign[HandleNum] == TRUE || i == 24)
		{
            //����Ҫע�⣬��Ϊ�����Ƕ�̬�ģ�����ɾ��ǰҪȷ��������û�з����仯(��Ϊ����ǿ��ֹͣ����������Կ��ܻ��������߳�ͬʱ��ִ��ɾ������)
			int DeqCount1, DeqCount2;
			deque<int>::iterator pos;
			for (int j=0; j < 3; j++)
			{
				DeqCount1 = ContinueDeq.size();
				pos = find(ContinueDeq.begin(), ContinueDeq.end(), HandleNum);
				
				DeqCount2 = ContinueDeq.size();
				if (DeqCount1 == DeqCount2)
				{
					ContinueDeq.erase(pos);
					break;
				}
				else if (DeqCount1 != DeqCount2)
				{
					continue;
				}
			}
			return FALSE;
		}
	}
	return TRUE;
}

/*����һ��������*/

/*����ͨ��ģ��*/
//�Զ���ϵͳ��Ϣ����������ֹ����ͷ������ӣ�Ŀǰ��û�õ���
afx_msg LRESULT IMEIWrite_MulAT::MSG_GetSimpleMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 3:
		g_ExitFlag = 0;
		exit(0);
		break;
	default:
		break;
	}
	return 0;
}

//�Զ���ϵͳ��Ϣ���ͺ������������;�������س����
void IMEIWrite_MulAT::MSG_SendSimpleMessage()
{
	ADCTHwnd = NULL;
	ADCTHwnd = ::FindWindow("ADCT", "AutoDownloadATETest");
	::SendMessage(ADCTHwnd, WM_SimpleMessage, 3, (LPARAM)this->m_hWnd);
	//ADCTInquire("ALL", 1);
}

//ͨ��COPYDATA���ͽṹ��
void IMEIWrite_MulAT::MSG_SendCopyDataMessage(CopyDataMSG *MessageStruct, int replyInt)
{
	COPYDATASTRUCT copyData;
	copyData.dwData = replyInt;//0�Ƿ��ͣ�1�ǻظ�
	copyData.cbData = sizeof(CopyDataMSG);
	copyData.lpData = (PVOID)MessageStruct;
	Sleep(1);
	::SendMessage(ADCTHwnd, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	//if (IsSuccess==FALSE)
	//	::SendMessage(::FindWindow("ADCT", "AutoDownloadATETest"), WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	//::SendMessageTimeout(::FindWindow("ADCT", "AutoDownloadATETest"), WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData, SMTO_ABORTIFHUNG, 10000, 0);
}

//ͨ��COPYDATA���սṹ��
BOOL IMEIWrite_MulAT::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pCopyDataStruct->cbData > 0)
	{
		CopyDataMSG *copydataMsg = new CopyDataMSG;
		copydataMsg = (CopyDataMSG *)pCopyDataStruct->lpData;
		CString MSGPort = copydataMsg->Port, MSGNum, MSGMessageChar = copydataMsg->MessageChar;

		//0������ADCT���ؽ��в�ѯ��1������ADCT��������Ҫ��������
		switch (pCopyDataStruct->dwData)
		{
		case 0:
			ADCTInquire(copydataMsg->Port, copydataMsg->MessageNum);
			break;
		case 1:
			ADCTSetup(copydataMsg->Port, copydataMsg->MessageNum, copydataMsg->MessageChar);
			break;
		default:
			break;
		}
		MSGNum.Format("%d", copydataMsg->MessageNum);
		LogShow_exchange(m_ResultArray[8], Final_Result_ControlArray[8], 0, "Recevi:Port:" + MSGPort + "MessageNum:" + MSGNum + "MessageChar:" + MSGMessageChar, 8);
	}
	return CResizableDialog::OnCopyData(pWnd, pCopyDataStruct);
}


//ADCT���в�ѯʱ
void IMEIWrite_MulAT::ADCTInquire(CString PortNo, INT CommandNo)
{
	CopyDataMSG *copydataMsg = new CopyDataMSG;
	strcpy(copydataMsg->Port, (LPCTSTR)PortNo);
	copydataMsg->MessageNum = CommandNo;

	//0�����⴮��״̬��1��ʾ��ѯ��ǰ���ڻ��ڲ���
	switch (CommandNo)
	{
	case 0:
		copydataMsg->MessageNum = 5;
		if (StopSign[PortStatusMap[PortNo]])//StopSignΪFLASEʱ��ʾ���ڱ�ռ�ã�ΪTRUEʱ��ʾ�ô���Ϊֹͣ״̬
		{
			strcpy(copydataMsg->MessageChar, (LPCTSTR)"1");
		}
		else
		{

			strcpy(copydataMsg->MessageChar, (LPCTSTR)"0");
		}
		break;
	case 1:
		copydataMsg->MessageNum = 4;
		strcpy(copydataMsg->MessageChar, (LPCTSTR)"OK");
		break;
	default:
		break;
	}
	CString MSGPort = copydataMsg->Port, MSGNum, MSGMessageChar = copydataMsg->MessageChar;
	MSGNum.Format("%d", copydataMsg->MessageNum);
	LogShow_exchange(m_ResultArray[8], Final_Result_ControlArray[8], 0, "SendInquire:Port:" + MSGPort + "MessageNum:" + MSGNum + "MessageChar:" + MSGMessageChar, 8);

	MSG_SendCopyDataMessage(copydataMsg, 1);

	if (copydataMsg != NULL)
	{
		delete copydataMsg;
		copydataMsg = NULL;
	}
}

//ADCT��������ʱ
void IMEIWrite_MulAT::ADCTSetup(CString PortNo, INT CommandNo, CString message)
{
	int ResultFlag;//�������ֵ
	CString Str_ResultFlag;
	CopyDataMSG *copydataMsg = new CopyDataMSG;
	strcpy(copydataMsg->Port, (LPCTSTR)PortNo);
	copydataMsg->MessageNum = CommandNo;
	int count = 0;


	//CommandNo��Status������0�ǳ�ʼ����1�Ǵ��ڿ�ʼ��2�Ǵ��ڹرգ�3���������²��ԣ�4��̩����5���ն��ȶ�����������
	switch (CommandNo)
	{
	case 0:
		ResultFlag = InitSetting(message.Left(message.Find("@@")), message.Right(message.GetLength() - message.Find("@@") - 2));//��ȡ�����źͰ汾��
		Str_ResultFlag.Format("%d", ResultFlag);
		strcpy(copydataMsg->MessageChar, (LPCTSTR)Str_ResultFlag);//���ݳ�ʼ�����������0����1
		//if (g_IsHideFlag == 1)
		//{
		//	ShowWindow(SW_HIDE);//���ش���
		//	ShowWindow(SW_HIDE);//���ش���win7ϵͳ����Ҫִ�ж��
		//	//UpdateWindow();
		//}
		break;
	case 1:

		//���ö������ж��ĸ��˿�û��ռ�ã�һ����˵ComFreeDeq�Ķ��׿϶��ǿ��еģ�Ȼ����mapȥ��ŵ�ǰCOM�Ŵ����ĸ��˿��У�����������StopSign[PortStatusMap[PortNo]]�жϴ˶˿�״̬
		if (StopSign[ComFreeDeq[0]] == TRUE)
		{
			PortStatusMap[PortNo] = ComFreeDeq[0];
			ComoOccupancydDeq.push_front(ComFreeDeq[0]);
			ComFreeDeq.pop_front();
			ResultFlag = StartPortTest(PortNo);//ʵ�ִ��ڿ�ʼ

			//������ڿ�ʼʧ��
			if (ResultFlag == 0)
			{
				if (!ComoOccupancydDeq.empty())
				{
					//�ȴ�ռ�ö�����ɾ����Ӧ�Ķ˿�
					deque<int>::iterator pos;
					pos = find(ComoOccupancydDeq.begin(), ComoOccupancydDeq.end(), PortStatusMap[PortNo]);
					ComoOccupancydDeq.erase(pos);
					//Ȼ���ٽ������·ŵ����ж����в�����
					ComFreeDeq.push_front(PortStatusMap[PortNo]);
					std::sort(ComFreeDeq.begin(), ComFreeDeq.end());
				}
			}

		}
		else
		{
			ResultFlag = 0;//���16�����ڶ���ռ���ˣ�ֱ�ӻظ�0
		}

		Str_ResultFlag.Format("%d", ResultFlag);
		strcpy(copydataMsg->MessageChar, (LPCTSTR)Str_ResultFlag);//���ݴ����Ƿ��ܹ���ʼ������0����1		
		break;
	case 2:
		if (PortNo == "ALL")
		{
			OnBnClickedCancel();
			strcpy(copydataMsg->MessageChar, (LPCTSTR)"1");//���ݴ����Ƿ��ܹ��رվ�����0����1
			break;
		}


		do{
			ResultFlag = StopPortTest(PortNo);//ʵ�ִ���ǿ�ƹر�

			if (ResultFlag == 1)
			{
				break;
			}
			count++;
			Sleep(500);
			if (count > 2)
			{
				break;
			}
		} while (0);

		Str_ResultFlag.Format("%d", ResultFlag);
		strcpy(copydataMsg->MessageChar, (LPCTSTR)Str_ResultFlag);//���ݴ����Ƿ��ܹ��رվ�����0����1
		break;
	case 3://���²���
		if (message == "0")
		{
			RTestChoose = FALSE;
		}
		else if (message == "1")
		{
			RTestChoose = TRUE;
		}
		//if (RTestChoose == FALSE)
		//{
		//	RTestChoose = TRUE;
		//}
		//else if (RTestChoose == TRUE)
		//{
		//	RTestChoose = FALSE;
		//}
		strcpy(copydataMsg->MessageChar, (LPCTSTR)"ALL");
		UpdateData(FALSE);
		break;
	case 4://̩��
		if (message == "0")
		{
			GPSDataTypeChoose = FALSE;
		}
		else if (message == "1")
		{
			GPSDataTypeChoose = TRUE;
		}
		strcpy(copydataMsg->MessageChar, (LPCTSTR)"ALL");
		UpdateData(FALSE);
		break;
	case 5://�ն��ȶ��˿ں������˿�
		if (message == "0")
		{
			SetDlgItemText(IDC_COMBO28,"�ն�---�ȶ��˿�");
		}
		else if (message == "1")
		{
			SetDlgItemText(IDC_COMBO28, "�ն�---�����˿�");
		}
		strcpy(copydataMsg->MessageChar, (LPCTSTR)"ALL");
		UpdateData(FALSE);
		break;
	case 6:
		OnClose();
		strcpy(copydataMsg->MessageChar, (LPCTSTR)"ALL");
		if (g_ExitFlag == 0)
			return;
		break;
	case 7:
		g_WaitTimeoutFlag = atoi(message);
		strcpy(copydataMsg->MessageChar, (LPCTSTR)"ALL");
		break;
	default:
		break;
	}

	CString MSGPort = copydataMsg->Port, MSGNum, MSGMessageChar = copydataMsg->MessageChar;
	MSGNum.Format("%d", copydataMsg->MessageNum);
	LogShow_exchange(m_ResultArray[8], Final_Result_ControlArray[8], 0, "SendSetUp:Port:" + MSGPort + "MessageNum:" + MSGNum + "MessageChar:" + MSGMessageChar, 8);

	UpdateWindow();

	MSG_SendCopyDataMessage(copydataMsg, 1);//1����ظ������س���

	if (copydataMsg != NULL)
	{
		delete copydataMsg;
		copydataMsg = NULL;
	}
}

//��ADCT���س����Ͳ��Խ��
void IMEIWrite_MulAT::SendADCTTestResult(CString PortNo, CString message)
{
	CopyDataMSG *copydataMsg = new CopyDataMSG;
	strcpy(copydataMsg->Port, (LPCTSTR)PortNo);
	copydataMsg->MessageNum = 3;
	strcpy(copydataMsg->MessageChar, (LPCTSTR)message);

	MSG_SendCopyDataMessage(copydataMsg, 1);

	if (copydataMsg != NULL)
	{
		delete copydataMsg;
		copydataMsg = NULL;
	}
}

//��ADCT���س�������ʾ��Ϣ
void IMEIWrite_MulAT::SendADCTHint(CString PortNo, CString message, int MessageType)
{
	CopyDataMSG *copydataMsg = new CopyDataMSG;
	strcpy(copydataMsg->Port, (LPCTSTR)PortNo);
	copydataMsg->MessageNum = MessageType;
	strcpy(copydataMsg->MessageChar, (LPCTSTR)message);

	MSG_SendCopyDataMessage(copydataMsg, 0);

	if (copydataMsg != NULL)
	{
		delete copydataMsg;
		copydataMsg = NULL;
	}
}

//�����쳣�˳���΢���Դ���API�������򴥷��쳣ʱ�����ȵ��ô˳���
LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)
//LONG WINAPI IMEIWrite_MulAT::FreeEIM_UnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
	if (g_ADCTFlag == 1)
	{
		IMEIWrite_MulAT imeidlg;
		//AfxMessageBox("Exception");	//���ûص������ɹ�

		//CopyDataMSG *copydataMsg = new CopyDataMSG;

		//strcpy(copydataMsg->Port, (LPCTSTR)"ALL");
		//copydataMsg->MessageNum = 5;
		//strcpy(copydataMsg->MessageChar, (LPCTSTR)"98");

		//imeidlg.MSG_SendCopyDataMessage(copydataMsg, 0);

		//if (copydataMsg != NULL)
		//{
		//	delete copydataMsg;
		//	copydataMsg = NULL;
		//}
		HWND ADCTHwnd = NULL;
		ADCTHwnd = ::FindWindow("ADCT", "AutoDownloadATETest");
		::SendMessage(ADCTHwnd, WM_SimpleMessage, 3, 98);

	}

	return EXCEPTION_CONTINUE_SEARCH;//���ر��ص������Ĵ�����
	//return EXCEPTION_EXECUTE_HANDLER;
}

//���������˳�
void IMEIWrite_MulAT::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (g_ADCTFlag == 1)
	{
		//CopyDataMSG *copydataMsg = new CopyDataMSG;

		//strcpy(copydataMsg->Port, (LPCTSTR)"ALL");
		//copydataMsg->MessageNum = 5;
		//strcpy(copydataMsg->MessageChar, (LPCTSTR)"99");

		//MSG_SendCopyDataMessage(copydataMsg, 0);

		//if (copydataMsg != NULL)
		//{
		//	delete copydataMsg;
		//	copydataMsg = NULL;
		//}
		HWND ADCTHwnd = NULL;
		ADCTHwnd = ::FindWindow("ADCT", "AutoDownloadATETest");
		::SendMessage(ADCTHwnd, WM_SimpleMessage, 3, 99);

	}

	//RDA��Ʒ�ǵ��˳���ʱ���ͷŶ�̬��
	if (GetRDAHostCheckValue == TRUE)
	{
		for (int i = 0; i < 16; i++)
		{
			if (RdaHostInterface.ComInit[i] == FALSE)
			{
				continue;
			}
			switch (i)
			{
			case 0:
				OnBnClickedButton2();
				break;
			case 1:
				OnBnClickedButton15();
				break;
			case 2:
				OnBnClickedButton17();
				break;
			case 3:
				OnBnClickedButton19();
				break;
			case 4:
				OnBnClickedButton21();
				break;
			case 5:
				OnBnClickedButton23();
				break;
			case 6:
				OnBnClickedButtonstop7();
				break;
			case 7:
				OnBnClickedButtonstop8();
				break;
			case 8:
				OnBnClickedButtonstop9();
				break;
			case 9:
				OnBnClickedButtonstop10();
				break;
			case 10:
				OnBnClickedButtonstop11();
				break;
			case 11:
				OnBnClickedButtonstop12();
				break;
			case 12:
				OnBnClickedButtonstop13();
				break;
			case 13:
				OnBnClickedButtonstop14();
				break;
			case 14:
				OnBnClickedButtonstop15();
				break;
			case 15:
				OnBnClickedButtonstop16();
				break;
			}
		}
		Sleep(100);
		RdaHostInterface.RDADeinitialization();
	}

	CResizableDialog::OnClose();
}

//ʵ���Զ���ȡ����
int IMEIWrite_MulAT::InitSetting(CString Zhidan, CString Version)
{
	SetDlgItemText(IDC_COMBO51, Zhidan);
	SetDlgItemText(IDC_COMBO56, "SMT����");
	m_SoftVersion = Version;
	OnGetWebSetting();

	return m_bVar;
}

//ʵ�ִ��ڿ�ʼ
int IMEIWrite_MulAT::StartPortTest(CString PortNo)
{
	int flag;

	//if (StopSign[0] == TRUE)
	//{
	//	PortStatusMap[PortNo] = 0;
	//}
	//else if (StopSign[1] == TRUE)
	//{
	//	PortStatusMap[PortNo] = 1;
	//}
	//else if (StopSign[2] == TRUE)
	//{
	//	PortStatusMap[PortNo] = 2;
	//}
	//else if (StopSign[3] == TRUE)
	//{
	//	PortStatusMap[PortNo] = 3;

	//}
	//else
	//{
	//	return 0;//��������ĸ����ڶ���ռ���ˣ�ֱ�ӻظ�0
	//}

	//Ȼ��ֱ�ӿ�ʼ��ǰ���õĶ˿�
	switch (PortStatusMap[PortNo])
	{
	case 0:
		SetDlgItemText(IDC_COMBO1, PortNo);
		OnBnClickedButton1();
		break;
	case 1:
		SetDlgItemText(IDC_COMBO4, PortNo);
		OnBnClickedButton14();
		break;
	case 2:
		SetDlgItemText(IDC_COMBO6, PortNo);
		OnBnClickedButton16();
		break;
	case 3:
		SetDlgItemText(IDC_COMBO8, PortNo);
		OnBnClickedButton18();
		break;
	case 4:
		SetDlgItemText(IDC_COMBO10, PortNo);
		OnBnClickedButton20();
		break;
	case 5:
		SetDlgItemText(IDC_COMBO12, PortNo);
		OnBnClickedButton22();
		break;
	case 6:
		SetDlgItemText(IDC_COMBO29, PortNo);
		OnBnClickedButtonstart7();
		break;
	case 7:
		SetDlgItemText(IDC_COMBO31, PortNo);
		OnBnClickedButtonstart8();
		break;
	case 8:
		SetDlgItemText(IDC_COMBO33, PortNo);
		OnBnClickedButtonstart9();
		break;
	case 9:
		SetDlgItemText(IDC_COMBO35, PortNo);
		OnBnClickedButtonstart10();
		break;
	case 10:
		SetDlgItemText(IDC_COMBO37, PortNo);
		OnBnClickedButtonstart11();
		break;
	case 11:
		SetDlgItemText(IDC_COMBO39, PortNo);
		OnBnClickedButtonstart12();
		break;
	case 12:
		SetDlgItemText(IDC_COMBO41, PortNo);
		OnBnClickedButtonstart13();
		break;
	case 13:
		SetDlgItemText(IDC_COMBO43, PortNo);
		OnBnClickedButtonstart14();
		break;
	case 14:
		SetDlgItemText(IDC_COMBO45, PortNo);
		OnBnClickedButtonstart15();
		break;
	case 15:
		SetDlgItemText(IDC_COMBO47, PortNo);
		OnBnClickedButtonstart16();
		break;
	default:
		break;
	}

	//����ö˿ڳɹ����������ͷ���1�����򷵻�0
	if (StopSign[PortStatusMap[PortNo]] == FALSE)
	{
		flag = 1;
	}
	else if (StopSign[PortStatusMap[PortNo]] == TRUE)
	{
		flag = 0;
	}
	return flag;
}

//ʵ�ִ��ڹر�
int IMEIWrite_MulAT::StopPortTest(CString PortNo)
{
	int flag;

	if (PortStatusMap.count(PortNo) == 0)
	{
		return 1;
	}

	//ֱ�ӹرյ�ǰ�˿�
	switch (PortStatusMap[PortNo])
	{
	case 0:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButton2();
		break;
	case 1:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButton15();
		break;
	case 2:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButton17();
		break;
	case 3:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButton19();
		break;
	case 4:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButton21();
		break;
	case 5:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButton23();
		break;
	case 6:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop7();
		break;
	case 7:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop8();
		break;
	case 8:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop9();
		break;
	case 9:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop10();
		break;
	case 10:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop11();
		break;
	case 11:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop12();
		break;
	case 12:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop13();
		break;
	case 13:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop14();
		break;
	case 14:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop15();
		break;
	case 15:
		if (Thread_State[PortStatusMap[PortNo]] == TRUE)
		OnBnClickedButtonstop16();
		break;
	default:
		return 1;//���û�ҵ����ھʹ���������ڴ���û��ʼ����ֱ�ӷ��ؾ��С�
		break;
	}

	//����ö˿ڳɹ����رգ��ͷ���1�����򷵻�0
	if (StopSign[PortStatusMap[PortNo]] == TRUE)
	{
		flag = 1;
	}
	else if (StopSign[PortStatusMap[PortNo]] == FALSE)
	{
		flag = 0;
	}
	else
	{
		flag = 2;
		CString i;
		i.Format("%d", PortStatusMap[PortNo]);
		LogShow_exchange(m_ResultArray[0], Final_Result_ControlArray[0], 6, PortNo +"||"+ i, 0);
	}
	return flag;
}

//��Ϊ����һ�Ĺ�ϵ������Ҫ�������������ԭ���ĵ�����
void IMEIWrite_MulAT::AfxMessageBoxHint(CString str)
{
	if (g_ADCTFlag == TRUE)
	{
		SendADCTHint("ALL", str, 3);
	}
	else if (g_ADCTFlag == FALSE)
	{
		AfxMessageBox(str);
	}
}


/*����������������*/

//���������ѡ���ʱ��
void IMEIWrite_MulAT::OnBnClickedBluetoothCheck()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//��ȡ���������ļ����ָ��ͳ�ʼ��һЩ����
void IMEIWrite_MulAT::GetBleOrder()
{
	CFileFind finder;

	BOOL ifFind = finder.FindFile(_T(".\\BluetoothSetting.ini"));
	if (ifFind)
	{
		//��ini�����ļ���Ϣ�Ķ���������
		int IntValue;
		IntValue = GetPrivateProfileInt(_T("BluetoothOrder"), _T("OrderNumber"), 3, _T(".\\BluetoothSetting.ini"));
		OrderNumber = IntValue;//ָ������
		IntValue = GetPrivateProfileInt(_T("BluetoothInfo"), _T("ExitOrderNumber"), 2, _T(".\\BluetoothSetting.ini"));
		ExitOrderNumber = IntValue;//ָ������
		IntValue = GetPrivateProfileInt(_T("BluetoothInfo"), _T("MacIntercept"), 12, _T(".\\BluetoothSetting.ini"));
		MacIntercept = IntValue;//MACӦ��ȡ��λ
		IntValue = GetPrivateProfileInt(_T("BluetoothInfo"), _T("OrderCount"), 2, _T(".\\BluetoothSetting.ini"));
		OrderCount = IntValue;//����ָ��ʹ���

		CString StrValue, stt;
		CString StrSetting = "Setting";
		//��ȡ��������ָ��
		for (int i = 0; i < OrderNumber; i++)
		{
			StrSetting.Format("Setting%d", i);
			GetPrivateProfileString(_T("BluetoothOrder"), StrSetting, _T(""), StrValue.GetBuffer(50), 50, _T(".\\BluetoothSetting.ini"));
			StrValue.ReleaseBuffer();//����releasebuffer�Ļ����ᵼ��left�Ⱥ����޷�ʹ�á�
			BleOrder[i][0] = StrValue.Left(StrValue.Find("@@"));
			StrValue = StrValue.Right(StrValue.GetLength() - StrValue.Find("@@") - 2);
			BleOrder[i][1] = StrValue.Left(StrValue.Find("##"));
			BleOrderCount[i] = _ttoi(StrValue.Right(StrValue.GetLength() - StrValue.Find("##") - 2));
		}

		//��ȡ�����Ͽ�ָ��
		for (int i = 0; i < ExitOrderNumber; i++)
		{
			StrSetting.Format("ExitSetting%d", i);
			GetPrivateProfileString(_T("BluetoothInfo"), StrSetting, _T(""), StrValue.GetBuffer(50), 50, _T(".\\BluetoothSetting.ini"));
			StrValue.ReleaseBuffer();//����releasebuffer�Ļ����ᵼ��left�Ⱥ����޷�ʹ�á�
			ExitOrder[i][0] = StrValue.Left(StrValue.Find("@@"));
			ExitOrder[i][1] = StrValue.Right(StrValue.GetLength() - StrValue.Find("@@") - 2);
		}
	}


	for (int i = 0; i < 16; i++)
	{
		StopSign[i] = 1;
	}

	ComFreeDeq = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	MacScanEnbleFlag = TRUE;//ʹ��ǰɨ��ǹ�ܹ�����MAC��ַ
	//std::sort(ComFreeDeq.begin(), ComFreeDeq.end());//��������������õ�

}

//���ݿ��ж���ComFreeDeq����λ����ʼĳ���˿�
void IMEIWrite_MulAT::BleStartPortTest(CString MACStr)
{
	//�Ƚ����ж��еĵ�һ���˿ںŸ�ռ�ö��У�Ȼ���ٽ�����˿ں��߳�����
	ComoOccupancydDeq.push_front(ComFreeDeq[0]);
	ComFreeDeq.pop_front();
	BluetoothHint(ComFreeDeq[0], "����");

	switch (ComoOccupancydDeq[0])
	{
	case 0:
		if (BluetoothConnect(0, MACStr, &m_Port1, &m_Baud1) == 1)
		{
			OnBnClickedButton1();
			BluetoothHint(1, "��ɨ");
		}
		else
		{
			BluetoothHint(1, "����");
			BluetoothHint(0, "��ɨ");
			OnBnClickedButton2();
		}
		break;
	case 1:
		if (BluetoothConnect(1, MACStr, &m_Port2, &m_Baud2) == 1)
		{
			OnBnClickedButton14();
			BluetoothHint(2, "��ɨ");
		}
		else
		{
			BluetoothHint(2, "����");
			BluetoothHint(1, "��ɨ");
			OnBnClickedButton15();
		}
		break;
	case 2:
		if (BluetoothConnect(2, MACStr, &m_Port3, &m_Baud3) == 1)
		{
			OnBnClickedButton16();
			BluetoothHint(3, "��ɨ");
		}
		else
		{
			BluetoothHint(3, "����");
			BluetoothHint(2, "��ɨ");
			OnBnClickedButton17();
		}
		break;
	case 3:
		OnBnClickedButton18();
		break;
	case 4:
		OnBnClickedButton20();
		break;
	case 5:
		OnBnClickedButton22();
		break;
	case 6:
		OnBnClickedButtonstart7();
		break;
	case 7:
		OnBnClickedButtonstart8();
		break;
	case 8:
		OnBnClickedButtonstart9();
		break;
	case 9:
		OnBnClickedButtonstart10();
		break;
	case 10:
		OnBnClickedButtonstart11();
		break;
	case 11:
		OnBnClickedButtonstart12();
		break;
	case 12:
		OnBnClickedButtonstart13();
		break;
	case 13:
		OnBnClickedButtonstart14();
		break;
	case 14:
		OnBnClickedButtonstart15();
		break;
	case 15:
		OnBnClickedButtonstart16();
		break;
	default:
		break;
	}
}

//�Ӷ�����ɾ���Ѿ�������Ķ˿ں�
void IMEIWrite_MulAT::DeleteComoOccupancydDeq(int PortNo)
{
	if (!ComoOccupancydDeq.empty())
	{
		//�ȴ�ռ�ö�����ɾ����Ӧ�Ķ˿�
		deque<int>::iterator pos;
		pos = find(ComoOccupancydDeq.begin(), ComoOccupancydDeq.end(), PortNo);
		ComoOccupancydDeq.erase(pos);
		BluetoothHint(ComFreeDeq[0], "W");
		//Ȼ���ٽ������·ŵ����ж����в�����
		ComFreeDeq.push_front(PortNo);
		std::sort(ComFreeDeq.begin(), ComFreeDeq.end());
		if (g_ADCTFlag != 1)
		{
			BluetoothHint(ComFreeDeq[0], "��ɨ");

		}
		else if (g_ADCTFlag == 1)
		{
			if (PortStatusMap.count(get_map_key_value(PortStatusMap, PortNo)) > 0)
			{
				PortStatusMap.erase(get_map_key_value(PortStatusMap, PortNo));
			}
		}
	}
}

//�������ں��ĺ��������������ȡ����ʱ���������ӺͿ�ʼ����ʱ����������
void IMEIWrite_MulAT::BluetoothCoreFunction(CString MACStr)
{
	MacScanEnbleFlag = FALSE;//�������Ƚ�ɨ��ǹ�����ݻ�ȡʧЧ

	UpdateData(TRUE);
	//���жϻ�ȡ������û,BLEGetSettingFlag������-1����û��ȡ���ã�0�������ڻ�ȡ������Ҫɨ��MAC��ַȥ����������1�����������ӳɹ�������ȥ��ȡ�����ˣ�2������������ʧ��Ȼ��Ҫȥ��ȡ������
	if (BLEGetSettingFlag != 1)
	{
		if (BLEGetSettingFlag == -1)
		{
			AfxMessageBox("���Ȼ�ȡ���ã���");
			MacScanEnbleFlag = TRUE;
		}
		else if (BLEGetSettingFlag == 0)
		{
			BLEGetSettingFlag = BluetoothConnect(0, MACStr, &m_Port1, &m_Baud1);
		}
	}
	//����Ѿ���ȡ���ã��ǽ�����ɨ��MAC��ַ��ʱ���Ӧ��ֱ�ӿ�ʼ�����ˣ���ʱҪ���ж�MacScanEnbleFlag��ֵ������ΪFALSE��ʱ���ʾ��ʱ���������������ӣ��������ٽ���MAC��ַ��ɨ����
	else if (GetBluetoothCheckValue == TRUE&&BLEGetSettingFlag == 1)
	{
		//BluetoothHint(ComFreeDeq[0], "��ɨ");
		BleStartPortTest(MACStr);
	}

	if (BLEGetSettingEndFlag == 1)
	{
		MacScanEnbleFlag = TRUE;//ֻ�����û�ȡ�������������Զ����Ե��������ӲŻ������ｫɨ��ǹ�����ݻ�ȡ��Ϊ��Ч������ʱ�̴˱�־λ�����û�ȡ������Ϊ1����-1��
	}

	SetDlgItemText(IDC_SCANDATA_EDIT, "");
}

//�������Ӻ���
int IMEIWrite_MulAT::BluetoothConnect(int PortNo, CString MACStr, CComboBox* m_Port, CComboBox* m_Baud)
{
	BleGetSettingFlag[PortNo] = 1;

	BluetoothHint(PortNo, "�ȴ�");

	int ReturnFlag;
	if (OPen_Serial_Port(m_Port, m_Baud, PortNo) == TRUE)//OnGetWebSetting
	{
	}
	else
	{
		AfxMessageBox("�򿪴���ʧ��");
		ReturnFlag = -1;
		return ReturnFlag;
	}

	CString Serial_Order_BleReturn, StrBleOrderSend, StrBleOrderReturn;//Ϊ�˷�ֹԭ��ָ���ܵ���Ⱦ���������������ʱ������װ��ָ��

	//��ʱ�������������ļ��������ָ��ȥ������������ָ��,��ָ��ȫ���������Ҷ��ɹ��󣬾ͽ�BLEGetSettingFlag��Ϊ1��ʾ���ӳɹ��ˣ�������Ϊ2
	for (int i = 0, j = 0; i < OrderNumber; i++)
	{
		StrBleOrderSend = BleOrder[i][0];
		StrBleOrderReturn = BleOrder[i][1];

		if (StrBleOrderSend.Find("MACValue"))
			StrBleOrderSend.Replace("MACValue", MACStr);

		//�����⵽��ǰ�߳�ΪTRUE���Ǿʹ����ʱ�˿��Ѿ����ص���
		if (BleGetSettingFlag[PortNo] == 0 && BLEGetSettingFlag == 1)
		{
			return -1;
		}

		Send_Serial_Order(&Serial_Order_BleReturn, StrBleOrderSend, PortNo, "NULL", "", BleOrderCount[i]);

		//������߼��ǣ����贮�ڶ����������ݲ��ԣ��һ�û����ָ���ķ��ʹ�����������i-1�����·��͸�ָ���������ڳɹ��ˣ��ͽ�j��Ϊ0
		if (Vaule_Return_Count_CS[PortNo].Find(StrBleOrderReturn) != -1)
		{
			if (i == (OrderNumber - 1))
			{
				ReturnFlag = 1;
			}
			j = 0;
		}
		else if (Vaule_Return_Count_CS[PortNo] != StrBleOrderReturn)
		{
			if (j < OrderCount){
				i = i - 1;
				j = j + 1;
				continue;
			}
			ReturnFlag = 2;
			BluetoothHint(0, "��ʧ��");
			LogShow_exchange(&m_Result1, &Final_Result_Control1, 6, "��������ʧ�ܣ������ն˺�͸��ģ��", 0);
			Sleep(2000);
			break;
		}

	}

	//Ҫ�ǵùرմ���
	BOOL Return;
	if (hPort[PortNo] != NULL)
		Return = CloseHandleControl(hPort[PortNo], GetRDAHostCheckValue, PortNo);
	hPort[PortNo] = NULL;
	if (Return == TRUE)
	{
	}
	else
	{
		AfxMessageBox("�رմ���ʧ�ܣ����²���USB�ɽ������");
	}

	//BleGetSettingFlag[PortNo] = 0;

	return ReturnFlag;
}

//�����Ͽ�����
void IMEIWrite_MulAT::BluetoothDisconnect(int PortNo)
{
	CString Serial_Order_BleReturn;
	//��ʱ�������������ļ��������ָ��ȥ������������ָ��,��ָ��ȫ���������Ҷ��ɹ��󣬾ͽ�BLEGetSettingFlag��Ϊ1��ʾ���ӳɹ��ˣ�������Ϊ2
	for (int i = 0, j = 0; i < ExitOrderNumber; i++)
	{
		Send_Serial_Order(&Serial_Order_BleReturn, ExitOrder[i][0], PortNo, "NULL", "", 500);
	}
}

//��Ϊ��ͨ��ɨ��ǹɨ�裬Ȼ�����MAC�����ڣ����������ڲ��Գɹ�����ʧ��֮��Ҫֹͣ�ô���
void IMEIWrite_MulAT::BleStopPortTest(int PortNo)
{
	switch (PortNo)
	{
	case 0:
		OnBnClickedButton2();
		break;
	case 1:
		OnBnClickedButton15();
		break;
	case 2:
		OnBnClickedButton17();
		break;
	case 3:
		OnBnClickedButton19();
		break;
	case 4:
		OnBnClickedButton21();
		break;
	case 5:
		OnBnClickedButton23();
		break;
	case 6:
		OnBnClickedButtonstop7();
		break;
	case 7:
		OnBnClickedButtonstop8();
		break;
	case 8:
		OnBnClickedButtonstop9();
		break;
	case 9:
		OnBnClickedButtonstop10();
		break;
	case 10:
		OnBnClickedButtonstop11();
		break;
	case 11:
		OnBnClickedButtonstop12();
		break;
	case 12:
		OnBnClickedButtonstop13();
		break;
	case 13:
		OnBnClickedButtonstop14();
		break;
	case 14:
		OnBnClickedButtonstop15();
		break;
	case 15:
		OnBnClickedButtonstop16();
		break;
	default:
		break;
	}
}

//�������Դ�����ʾ�õĺ���
void IMEIWrite_MulAT::BluetoothHint(int PortNo, CString str)
{
	if (str.Find("ʧ��") != -1)
	{
		gColor[PortNo] = RGB(255, 0, 0);
	}
	else
	{
		gColor[PortNo] = RGB(122, 197, 205);
	}
	switch (PortNo)
	{
	case 0:
		Final_Result_Control1.SetWindowTextA(str);
		break;
	case 1:
		Final_Result_Control2.SetWindowTextA(str);
		break;
	case 2:
		Final_Result_Control3.SetWindowTextA(str);
		break;
	case 3:
		Final_Result_Control4.SetWindowTextA(str);
		break;
	case 4:
		Final_Result_Control5.SetWindowTextA(str);
		break;
	case 5:
		Final_Result_Control6.SetWindowTextA(str);
		break;
	case 6:
		Final_Result_Control7.SetWindowTextA(str);
		break;
	case 7:
		Final_Result_Control8.SetWindowTextA(str);
		break;
	case 8:
		Final_Result_Control9.SetWindowTextA(str);
		break;
	case 9:
		Final_Result_Control10.SetWindowTextA(str);
		break;
	case 10:
		Final_Result_Control11.SetWindowTextA(str);
		break;
	case 11:
		Final_Result_Control12.SetWindowTextA(str);
		break;
	case 12:
		Final_Result_Control13.SetWindowTextA(str);
		break;
	case 13:
		Final_Result_Control14.SetWindowTextA(str);
		break;
	case 14:
		Final_Result_Control15.SetWindowTextA(str);
		break;
	case 15:
		Final_Result_Control16.SetWindowTextA(str);
		break;
	default:
		break;
	}
	UpdateWindow();
}

//�����������õı�־λ��ʹ�������������»�ȡ����
void IMEIWrite_MulAT::OnBnClickedResetbleButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BLEGetSettingFlag = -1;
	BLEGetSettingEndFlag = 0;
	Software_Version[0] = "";
	MessageBox("�����»�ȡ����", "��ʾ");
}


/*Dongle����������,͸��ģ��*/
//��ʼ������
BOOL IMEIWrite_MulAT::DongleInit(int HandleNum)
{
	CString Vaule_Return, strCommand_Vaule;
	//CString Command[9]={"AT+DISCON","AT+RENEW","AT+BAUD4","AT+TXPW7","AT+SDUR3","AT+SERVFEE7","AT+CHTX36F5","AT+CHRX36F6","AT+RESET"};
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "���³�ʼ��Dongle�����У����Ե�", HandleNum);
	
	for (int i = 0; i < DongleInitCommandNumber; i++)
		Send_Serial_Order(&Vaule_Return, DongleInitCommand[i], HandleNum, "", "", 100);

	Sleep(DongleSleepTime);

	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "Dongle������ʼ�����", HandleNum);
	
	return true;
}

//�������ã������ȣ���ʼ������ini�ļ�
void IMEIWrite_MulAT::DongleInitSetting()
{
	//������ʼ��ָ��
	DongleResetCount = 30;

	DongleInitCommandNumber = 6;

	DongleInitCommand[0] = "AT+RENEW";
	DongleInitCommand[1] = "AT+SDUR3";
	DongleInitCommand[2] = "AT+SERVFEE7";
	DongleInitCommand[3] = "AT+CHTX36F5";
	DongleInitCommand[4] = "AT+CHRX36F6";
	DongleInitCommand[5] = "AT+RESET";

	//��������
	DongleConnectRssi = -40;
	DongleDisonnectRssi = -60;
	DongleSleepTime = 1500;
	DongleSDURTime = 5000;

	//��������ָ������
	DongleSPCommand[0][0] = "AT+CON";
	DongleSPCommand[0][1] = "Chars Found";
	DongleSpCommandSleepTime[0] = 7500;
	DongleSPCommand[1][0] = "AT+DISCON";
	DongleSPCommand[1][1] = "LOST";
	DongleSpCommandSleepTime[1] = 1000;
	DongleSPCommand[2][0] = "06010101";
	DongleSPCommand[2][1] = "0207,(7,20)";
	DongleSpCommandSleepTime[2] = 1500;
	DongleSPCommand[3][0] = "FBF60101";
	DongleSPCommand[3][1] = "FBF701,(7,30)";
	DongleSpCommandSleepTime[3] = 500;

	CFileFind finder;
	CString ValueStr, StrSetting = "InitSetting", StrValue;
	int ValueInt;


	BOOL ifFind = finder.FindFile(_T(".\\DongleBleSetting.ini"));
	if (ifFind)
	{
		//������ʼ��ָ��
		ValueInt = GetPrivateProfileInt(_T("DongleInitCommand"), _T("DongleInitCommandNumber"), 6, _T(".\\DongleBleSetting.ini"));
		DongleInitCommandNumber = ValueInt;

		for (int i = 0; i < DongleInitCommandNumber; i++)
		{
			StrSetting.Format("InitSetting%d", i);
			GetPrivateProfileString(_T("DongleInitCommand"), StrSetting, _T(""), StrValue.GetBuffer(50), 50, _T(".\\DongleBleSetting.ini"));
			DongleInitCommand[i] = StrValue;
			StrValue.ReleaseBuffer();
		}


		//��������
		ValueInt = GetPrivateProfileInt(_T("DongleSetting"), _T("DongleConnectRssi"), -40, _T(".\\DongleBleSetting.ini"));
		DongleConnectRssi = ValueInt;
		ValueInt = GetPrivateProfileInt(_T("DongleSetting"), _T("DongleDisonnectRssi"), -60, _T(".\\DongleBleSetting.ini"));
		DongleDisonnectRssi = ValueInt;
		ValueInt = GetPrivateProfileInt(_T("DongleSetting"), _T("DongleSleepTime"), 1500, _T(".\\DongleBleSetting.ini"));
		DongleSleepTime = ValueInt;
		ValueInt = GetPrivateProfileInt(_T("DongleSetting"), _T("DongleSDURTime"), 5000, _T(".\\DongleBleSetting.ini"));
		DongleSDURTime = ValueInt;


		//��������ָ������
		for (int i = 0; i < 4; i++)
		{
			StrSetting.Format("SPSetting%d", i);
			GetPrivateProfileString(_T("DongleSPCommand"), StrSetting, _T(""), StrValue.GetBuffer(50), 50, _T(".\\DongleBleSetting.ini"));
			StrValue.ReleaseBuffer();//����releasebuffer�Ļ����ᵼ��left�Ⱥ����޷�ʹ�á�
			DongleSPCommand[i][0] = StrValue.Left(StrValue.Find("@@"));
			StrValue = StrValue.Right(StrValue.GetLength() - StrValue.Find("@@") - 2);
			DongleSPCommand[i][1] = StrValue.Left(StrValue.Find("##"));
			DongleSpCommandSleepTime[i] = _ttoi(StrValue.Right(StrValue.GetLength() - StrValue.Find("##") - 2));
		}
	}
	return ;
}

//�����Զ�ɨ������
BOOL IMEIWrite_MulAT::DongleScan(int HandleNum, int RssiStr, CString SoftModel)
{
	CString Vaule_Return, strCommand_Vaule;
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "����ɨ�������豸�У����Ե�", HandleNum);
	Send_Serial_Order(&Vaule_Return, "AT+SCAN?", HandleNum, "Devices Found:", "", DongleSDURTime);

	if (Vaule_Return == "FAIL")
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "ɨ��ʧ�ܣ�", HandleNum);
		return FALSE;
	}

	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "ɨ��ɹ���", HandleNum);


	//�������ɨ��ǹ���Ǿ�ֱ�ӽ�������
	if (hScanGun[0]!=NULL)
	{ 
		if (DongleConnect(HandleNum) == FALSE)
		{
			return FALSE;
		}
		Sleep(DongleSleepTime);
		return TRUE;
	}

	//��ʼ�����ȡ�����豸
	CString BleTemp, InfoTemp, BleTime, BleInfo[20][3];

	int InfoCut, CutTime, RSSICount = 0, RSSIMAX = -100, RSSITemp;

	BleTemp = Vaule_Return_Count_CS[HandleNum];
	InfoCut = BleTemp.Find(_T("Devices Found:"));

	//��ȡ���豸����
	BleTime = BleTemp.Right(BleTemp.GetLength() - InfoCut - 14);
	CutTime = atoi(CStringA(BleTime.TrimLeft(_T(" "))));

	//�Ѷ������Ϣ�����
	//BleTemp = BleTemp.Right(BleTemp.GetLength() - BleTemp.Find("Scanning..."));
	//BleTemp = BleTemp.Left(BleTemp.Find(("\r\n"), BleTemp.Find("Devices Found:")));

	//����ȡ����Ϣ�����и�
	BleTemp = BleTemp.Left(InfoCut);
	BleTemp = BleTemp.Right(BleTemp.GetLength() - 11);

	////����ȡ����Ϣ����������(��ɸѡ�ն˻��Ͱ汾)
	//for (int i = 0; i < CutTime; i++)
	//{
	//	CString StrTemp;
	//	StrTemp.Format(_T("\r\n%d: 0x"), i);
	//	BleTemp = BleTemp.Right(BleTemp.GetLength() - BleTemp.Find(StrTemp) - 7);
	//	InfoTemp = BleTemp.Left(BleTemp.Find(_T("\r\n")));

	//	for (int j = 0; j < 3; j++)
	//	{
	//		int CutTemp;
	//		CutTemp = InfoTemp.Find(_T(","));

	//		//�����źŵ�ʱ��ͶԱ�һ�£��ҵ��ź���ǿ��
	//		if (j == 1)
	//		{
	//			RSSITemp = _ttoi(InfoTemp);
	//			if (j == 1 && RSSITemp > RSSIMAX)
	//			{
	//				RSSIMAX = RSSITemp;
	//				RSSICount = i;
	//			}
	//		}

	//		if (CutTemp == -1)
	//		{
	//			BleInfo[i][j] = InfoTemp;
	//			if (j != 2)
	//			{
	//				BleInfo[i][2] = _T("NULL");
	//			}
	//			break;
	//		}
	//		BleInfo[i][j] = InfoTemp.Left(InfoTemp.Find(_T(",")));
	//		InfoTemp = InfoTemp.Right(InfoTemp.GetLength() - InfoTemp.Find(_T(",")) - 2);
	//	}
	//}

	//����Ӧ�ÿ�ס����,���豸�Ŷ����ӣ�ͬʱ���ӵĻ��������������
	DongleTestDataDeq.push_back(HandleNum);

	while (1)
	{
		if (DongleTestDataDeq.front() == HandleNum)
		{
			break;
		}
		Sleep(100);
		if (StopSign[HandleNum] == TRUE)
		{
			deque<int>::iterator pos;
			pos = find(DongleTestDataDeq.begin(), DongleTestDataDeq.end(), HandleNum);
			DongleTestDataDeq.erase(pos);
			return FALSE;
		}
	}


	//����ȡ����Ϣ����������
	int Devices = 0;
	for (int i = 0; i < CutTime; i++)
	{
		CString StrTemp;
		StrTemp.Format(_T("\r\n%d: 0x"), i);
		BleTemp = BleTemp.Right(BleTemp.GetLength() - BleTemp.Find(StrTemp) - 7);
		InfoTemp = BleTemp.Left(BleTemp.Find(_T("\r\n")));

		//ɸѡһ���ն�����
		if (InfoTemp.Find(SoftModel) == -1)
			continue;

		//ɸ��ȫ��Dongle�������Ѿ�����MAC��ַ
		BOOL IsContinueFlag=FALSE;
		for (int z = 0; z < 16; z++)
		{
			if (InfoTemp.Find(DongleInfo[z][0]) !=-1 && DongleInfo[z][0]!="")
			{
				IsContinueFlag = TRUE;
			}
		}

		if (IsContinueFlag == TRUE)
			continue;

		//����ſ�ʼ���ҵ����豸��������
		for (int j = 0; j < 3; j++)
		{
			int CutTemp;
			CutTemp = InfoTemp.Find(_T(","));

			//�����źŵ�ʱ��ͶԱ�һ�£��ҵ��ź���ǿ��
			if (j == 1)
			{
				RSSITemp = _ttoi(InfoTemp);
				if (j == 1 && RSSITemp > RSSIMAX)
				{
					RSSIMAX = RSSITemp;
					RSSICount = Devices;
				}
			}

			if (CutTemp == -1)
			{
				BleInfo[Devices][j] = InfoTemp;
				if (j != 2)
				{
					BleInfo[Devices][2] = _T("NULL");
				}
				break;
			}
			BleInfo[Devices][j] = InfoTemp.Left(InfoTemp.Find(_T(",")));
			InfoTemp = InfoTemp.Right(InfoTemp.GetLength() - InfoTemp.Find(_T(",")) - 2);
		}
		Devices++;
	}

	//�ڹ涨��Χ�ھ�����
	if (RSSIMAX >= RssiStr)
	{
		DongleInfo[HandleNum][0] = BleInfo[RSSICount][0];
		DongleTestDataDeq.pop_front();
		if (DongleConnect(HandleNum) == FALSE)
		{
			return FALSE;
		}
		Sleep(DongleSleepTime);
		return TRUE;
	}
	DongleTestDataDeq.pop_front();
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, BleInfo[RSSICount][1]+",δ���ҵ��ź�ǿ�ȴ��������豸��", HandleNum);
	return FALSE;
}

//��������
BOOL IMEIWrite_MulAT::DongleConnect(int HandleNum)
{
	CString Vaule_Return, strCommand_Vaule,Result;

	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "���������豸�У����Եȣ�", HandleNum);
	
	Send_Serial_Order(&Vaule_Return, DongleSPCommand[0][0] + DongleInfo[HandleNum][0], HandleNum, (LPSTR)(LPCTSTR)DongleSPCommand[0][1], "", DongleSpCommandSleepTime[0]);

	if (Vaule_Return == "FAIL")
	{
		DongleInfo[HandleNum][0] = "";
		DongleInfo[HandleNum][1] = "";
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�����豸����ʧ�ܣ�", HandleNum);
		return FALSE;
	}


	//������֮���ȡ����
	Send_Serial_Order(&Vaule_Return, DongleSPCommand[2][0], HandleNum, (LPSTR)(LPCTSTR)DongleSPCommand[2][1], "", DongleSpCommandSleepTime[2], 1);

	if (Vaule_Return == "FAIL")
	{
		DongleInfo[HandleNum][0] = "";
		DongleInfo[HandleNum][1] = "";
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "���ƻ�ȡʧ�ܣ�", HandleNum);
		DongleDisConnect(HandleNum);
		return FALSE;
	}

	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�����豸���ӳɹ���", HandleNum);
	
	DongleInfo[HandleNum][1] = Vaule_Return_Count_CS[HandleNum];
	return TRUE;
}

//�Ͽ�����
BOOL IMEIWrite_MulAT::DongleDisConnect(int HandleNum)
{
	CString Vaule_Return, strCommand_Vaule;
	Send_Serial_Order(&Vaule_Return, DongleSPCommand[1][0], HandleNum, (LPSTR)(LPCTSTR)DongleSPCommand[1][1], "", DongleSpCommandSleepTime[1]);
	if (Vaule_Return == "FAIL")
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�����豸�Ѿ����ߣ�", HandleNum);
		DongleInfo[HandleNum][0] = "";
		DongleInfo[HandleNum][1] = "";
		return FALSE;
	}
	//Send_Serial_Order(&Vaule_Return, "AT + RESET", HandleNum, "RESET", "",1000);
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�����豸�Ͽ��ɹ���", HandleNum);
	
	DongleInfo[HandleNum][0] = "";
	DongleInfo[HandleNum][1] = "";
	Sleep(DongleSleepTime);
	return TRUE;
}

//����ɨ��ǹ���ܺ���
BOOL IMEIWrite_MulAT::DongleScanGun(int HandleNum)
{
	DongleConnectFlag[HandleNum] = TRUE;//��Dongle��������

	if (Software_Version[0] == "")
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "���Ȼ�ȡ���ã�", HandleNum);
		return FALSE;
	}

	if (hScanGun[0] != NULL)
	{
		//����Ӧ�ÿ�ס����,���豸�Ŷ����ӣ�ͬʱ���ӵĻ��������������
		DongleTestDataDeq.push_back(HandleNum);

		while (1)
		{
			//ɨ��ǹ����Ϣ���˾ͻ�ȡ
			if (DongleTestDataDeq.front() == HandleNum&&DongleScanGunFlag == TRUE)
			{
				GetDlgItem(IDC_SCANDATA_EDIT)->GetWindowTextA(DongleInfo[HandleNum][0]);
				m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA(DongleInfo[HandleNum][0]);
				DongleScanGunFlag = FALSE;
				DongleTestDataDeq.pop_front();
				GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("");
				break;
			}
			Sleep(100);
			if (StopSign[HandleNum] == TRUE)
			{
				deque<int>::iterator pos;
				pos = find(DongleTestDataDeq.begin(), DongleTestDataDeq.end(), HandleNum);
				DongleTestDataDeq.erase(pos);
				return FALSE;
			}
		}
	}
	return TRUE;
}

//����������ֵ���н���
CString IMEIWrite_MulAT::DongleValueAnalyze(CString CommandName, CString CommandValue)
{
	CString StrTemp;
	
	//0����Ҫת��,1��Ҫ16����ת��,2��Ҫ�ַ�ת��,3��Ҫ�¶�ת��,4��Ҫʱ��ת����1970ʱ�����
	switch (DongleCommandNameToInt(CommandName))
	{
	case 0:
		StrTemp=CommandValue;
		break;
	case 1:
		StrTemp.Format("%d", HexToDec(CommandValue));
		break;
	case 2:
		StrTemp = DongleHexToASCII(CommandValue);
		break;
	case 3:
		StrTemp = DongleTemperatureToCString(CommandValue);
		break;
	case 4:
		StrTemp = DongleUnixTimeToDatatime(CommandValue);
		break;
	}

	return StrTemp;
}

//������ת������һ����װ����
int IMEIWrite_MulAT::DongleCommandNameToInt(CString CommandName)
{
	if (CommandName.Find("ʮ������") != -1)
	{
		return 1;
	}
	else if (CommandName.Find("�ַ�ת��") != -1)
	{
		return 2;
	}
	else if (CommandName.Find("�¶�ת��") != -1)
	{
		return 3;
	}
	else if (CommandName.Find("ʱ��ת��") != -1)
	{
		return 4;
	}

	return 0;
}

/*����ĺ�������Dongle�������������׵Ĳ�Ʒ������ֵת���õ�*/

//����CT12�¶�
CString IMEIWrite_MulAT::DongleTemperatureToCString(CString str)
{
	CString StrDot, StrC,StrTemp;
	float FDot, FC;
	StrC = str.Right(2);
	FC = (float)HexToDec(StrC)*2.56;
	StrDot = str.Left(2);
	FDot = (float)HexToDec(StrDot) / 100;
	StrTemp.Format("%.2f", FC + FDot);
	return StrTemp;
}

//ʱ���ת��
CString IMEIWrite_MulAT::DongleUnixTimeToDatatime(CString str)
{
	CString StrSub, StrTemp = "";
	for (int i = 0; i < 4; i++)
	{
		StrSub = str.Left(8 - i * 2);
		StrTemp = StrTemp + StrSub.Right(2);
	}
	return StampToDatetime(_tcstoul(StrTemp, NULL, 16));
}

//�ַ�ת��
CString IMEIWrite_MulAT::DongleHexToASCII(CString str)
{
	CString StrSub1, StrSub2, StrTemp = "";
	int IntSub, count;
	count = str.GetLength() / 2;

	for (int i = 0; i < count; i++)
	{
		StrSub1 = str.Left(2);
		str = str.Right(str.GetLength() - 2);
		IntSub = strtol(StrSub1, NULL, 16);
		StrSub2.Format("%c", IntSub);
		StrTemp += StrSub2;
	}

	return StrTemp;
}

//����ת����ʱ���
CString IMEIWrite_MulAT::StampToDatetime(time_t nSrc)
{
	char *a = new char[50];
	CString time;
	struct tm p;
	p = *localtime(&nSrc);
	strftime(a, 1000, "%Y-%m-%d %H:%M:%S", &p);
	time = CA2CT(a);
	delete[]a;
	return time;
}

//ʮ������תʮ����
int IMEIWrite_MulAT::HexToDec(CString str)
{
	int n;
	n = _tcstoul(str, NULL, 16);
	return n;
}

//CStringתByte
BYTE * IMEIWrite_MulAT::CStrToByte(int len, CString str)
{
	BYTE *Buf = new BYTE[len];
	CString StrSub;
	for (int iSrc = 0, iDst = 0; iDst<len; iSrc += 2, iDst++)
	{
		StrSub = str.Mid(iSrc, 2);
		Buf[iDst] = (BYTE)_tcstoul(StrSub, NULL, 16);//16��ʮ�����Ƶ���˼
	}
	return Buf;
}

/*Dongle�������������׵Ĳ�Ʒ������ֵת���������*/

int byte2hex(const unsigned char *input, unsigned long inlen, unsigned char *output, unsigned long *outlen)
{
	static const char *num = "0123456789ABCDEF";
	unsigned long i = 0;
	unsigned char *p = output;
	if (*outlen < inlen * 2)
	{
		*outlen = inlen * 2;
		return -1;
	}
	for (i = 0; i < inlen; ++i)
	{
		*p++ = num[input[i] >> 4];
		*p++ = num[input[i] & 0x0F];
	}
	*outlen = p - output;
	return 0;
}

long hex2int(const string& hexStr)
{
	char *offset;
	if (hexStr.length() > 2)
	{
		if (hexStr[0] == '0' && hexStr[1] == 'x')
		{
			return strtol(hexStr.c_str(), &offset, 0);
		}
	}
	return strtol(hexStr.c_str(), &offset, 16);
}

//���ܺ���
CString IMEIWrite_MulAT::Encrypt(CString plainText)
{
	AES_Init(key);
	string dataTemp;
	CString szSub;
	unsigned char orgdata[16];
	unsigned char outputT[16];
	CString dataTempC;
	CString dataTempCOut;

	//for (int iSrc = 0, iDst = 0; iDst<16; iSrc += 2, iDst++)
	//{
	//	szSub = plainText.Mid(iSrc, 2);
	//	orgdata[iDst] = (BYTE)_tcstoul(szSub, NULL, 16);
	//}


	for (int i = 0; i < 16; i++)
	{
		dataTemp = plainText.Mid(i * 2, 2);
		orgdata[i] = hex2int(dataTemp);
	}

	AES_Encrypt(orgdata, outputT, 16, NULL);
	for (int i = 0; i < 16; i++)
	{
		dataTempC.Format("%02X", outputT[i]);
		dataTempCOut += dataTempC;
	}
	return dataTempCOut;
}

//���ܺ���
CString IMEIWrite_MulAT::Decrypt(CString plainText)
{
	AES_Init(key);
	string dataTemp;
	unsigned char orgdata[16];
	unsigned char outputT[16] = { 0 };
	CString dataTempC;
	CString dataTempCOut;

	for (int i = 0; i < 16; i++)
	{
		dataTemp = plainText.Mid(i * 2, 2);
		orgdata[i] = hex2int(dataTemp);
	}

	AES_Decrypt(outputT, orgdata, 16, NULL);
	for (int i = 0; i < 16; i++)
	{
		dataTempC.Format("%02X", outputT[i]);
		dataTempCOut += dataTempC;
	}
	return dataTempCOut;
}


/*Dongle����ɨ���ź�ǿ����������*/

//����ɨ�躯��
BOOL  IMEIWrite_MulAT::DongleScanRssiFun(int HandleNum, CString SoftModel)
{
	CString Vaule_Return, strCommand_Vaule;
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "����ɨ�������豸�У����Ե�", HandleNum);
	Send_Serial_Order(&Vaule_Return, "AT+SCAN?", HandleNum, "Devices Found:", "", DongleSDURTime);

	if (Vaule_Return == "FAIL")
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "ɨ��ʧ�ܣ�", HandleNum);
		return TRUE;
	}

	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "ɨ��ɹ���", HandleNum);

	//��ʼ�����ȡ�����豸
	CString BleTemp, InfoTemp, BleTime, BleInfo[30][3];

	int InfoCut, CutTime, RSSICount = 0, RSSIMAX = -100, RSSITemp;

	BleTemp = Vaule_Return_Count_CS[HandleNum];
	InfoCut = BleTemp.Find(_T("Devices Found:"));

	//��ȡ���豸����
	BleTime = BleTemp.Right(BleTemp.GetLength() - InfoCut - 14);
	CutTime = atoi(CStringA(BleTime.TrimLeft(_T(" "))));

	//����ȡ����Ϣ�����и�
	BleTemp = BleTemp.Left(InfoCut);
	BleTemp = BleTemp.Right(BleTemp.GetLength() - 11);

	//����Ӧ�ÿ�ס����,ͬһ��������Բ����ö���߳�ͬʱ���ʣ���Ȼ���п��ܻ���ɷ��ʳ�ͻ��Ҫ���ǵ���ǰ���е����������������������Ͼ�Ҫ��ӣ�
	int DeqSizeInt=DongleTestRssiDeq.size();
	if (DeqSizeInt < 2)
	{
		DongleTestRssiDeq.push_back(HandleNum);
	}
	else
	{
		DongleTestRssiDeq.insert(DongleTestRssiDeq.begin() + 1, HandleNum);
	}


	//�ж�һ�µ�ǰ�ǲ����Ѿ���������ˣ������ʱ��ֱ��ʧ��
	if (DequeContinueControlFun(HandleNum, DongleTestRssiDeq) == FALSE)
	{
		return FALSE;
	}


	//����ȡ����Ϣ����������
	int Devices = 0;
	for (int i = 0; i < CutTime; i++)
	{
		CString StrTemp;
		StrTemp.Format(_T("\r\n%d: 0x"), i);
		BleTemp = BleTemp.Right(BleTemp.GetLength() - BleTemp.Find(StrTemp) - 7);
		InfoTemp = BleTemp.Left(BleTemp.Find(_T("\r\n")));

		//ɸѡһ���ն�����
		if (InfoTemp.Find(SoftModel) == -1)
			continue;

		//ɸ��MAP���Ƿ��Ѿ��������MAC�ˣ�������ھ͸���һ��RSSI�����߿���ֱ����������ĸ�ֵ���������ǣ�
		for (int j = 0; j < 3; j++)
		{
			int CutTemp;
			CutTemp = InfoTemp.Find(_T(","));

			if (CutTemp == -1)
			{
				BleInfo[Devices][j] = InfoTemp;
				if (j != 2)
				{
					BleInfo[Devices][2] = _T("NULL");
				}
				break;
			}
			BleInfo[Devices][j] = InfoTemp.Left(InfoTemp.Find(_T(",")));
			InfoTemp = InfoTemp.Right(InfoTemp.GetLength() - InfoTemp.Find(_T(",")) - 2);
		}
		DongleRssiMap[BleInfo[Devices][0]] = atoi(BleInfo[Devices][1]);
		Devices++;
	}

	DongleTestRssiDeq.pop_front();

	int SizeInt = DongleRssiMap.size();
	CString SizeStr;
	SizeStr.Format("%d", SizeInt);
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��ǰ���ҵ����豸����Ϊ:" + SizeStr, HandleNum);
	
	map<CString,int>::iterator Countiter;
	Countiter = DongleRssiMap.begin();
	for (int a = 1; Countiter != DongleRssiMap.end(); Countiter++, a++)
	{
		CString Istr,Rssistr,Macstr;
		int Iint = Countiter->second;
		Macstr = Countiter->first;
		Istr.Format("%d", a);
		Rssistr.Format("%d", Iint);
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "�豸" + Istr + ":" + Macstr + "," + Rssistr, HandleNum);
	}

	return TRUE;
}

//����ɨ��ǹ���ܺ���
BOOL IMEIWrite_MulAT::DongleScanGunOnly(int HandleNum)
{
	//if (Software_Version[0] == "")
	//{
	//	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "���Ȼ�ȡ���ã�", HandleNum);
	//	return FALSE;
	//}

	if (hScanGun[0] != NULL)
	{
		//����Ӧ�ÿ�ס����,���豸�Ŷ����ӣ�ͬʱ���ӵĻ��������������
		DongleTestDataDeq.push_back(HandleNum);
		
		while(1)
		{
			//ɨ��ǹ����Ϣ���˾ͻ�ȡ
			if (DongleTestDataDeq.front() == HandleNum&&DongleScanGunFlag == TRUE)
			{
				GetDlgItem(IDC_SCANDATA_EDIT)->GetWindowTextA(DongleTestRssiInfoArray[HandleNum]);
				m_ShowNumberPortControlArray[HandleNum]->SetWindowTextA(DongleTestRssiInfoArray[HandleNum]);
				DongleScanGunFlag = FALSE;
				//DongleTestDataDeq.pop_front();
				GetDlgItem(IDC_SCANDATA_EDIT)->SetWindowTextA("");
				break;
			}
			Sleep(300);
			if (StopSign[HandleNum] == TRUE)
			{
				deque<int>::iterator pos;
				pos = find(DongleTestDataDeq.begin(), DongleTestDataDeq.end(), HandleNum);
				if (pos != DongleTestDataDeq.end())
					DongleTestDataDeq.erase(pos);
				return FALSE;
			}
		}
	}
	return TRUE;
}


/*��������ͷ��������*/

//ͼ�񰵽��ж�
BOOL IMEIWrite_MulAT::ImageAutoJudgeDarkCorner(int HandleNum, CString ImageSrc, int RGB, float Range)
{
	float DarkCornerDataFloat;
	CString  DarkCornerDataStr;
	string s = CT2A(ImageSrc.GetBuffer());
	DarkCornerDataFloat = ImageDarkCorner(s, RGB);
	ImageSrc.ReleaseBuffer();
	DarkCornerDataStr.Format("%f", DarkCornerDataFloat);
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��ǰͼƬ���ǰٷֱ�:" + DarkCornerDataStr, HandleNum);
	if (DarkCornerDataFloat <= Range)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//ͼ���������ж�
BOOL IMEIWrite_MulAT::ImageAutoJudgeDefinition(int HandleNum, CString ImageSrc, float Range)
{
	float DefinitionDataFloat;
	CString DefinitionDataStr;
	string s = CT2A(ImageSrc.GetBuffer());
	DefinitionDataFloat = ImageDefinition(s);
	ImageSrc.ReleaseBuffer();
	DefinitionDataStr.Format("%f", DefinitionDataFloat);
	LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��ǰͼƬ������:" + DefinitionDataStr, HandleNum);
	if (DefinitionDataFloat >= Range)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//�ݹ鴴��Ŀ¼
void IMEIWrite_MulAT::CreateDirectoryRecursionFun(CString Src, int StartCount)
{
	CString SrcTemp;
	int EndCount;
	EndCount = Src.Find(_T("\\"), StartCount);
	if (EndCount != -1)
	{
		SrcTemp = Src.Left(EndCount);
		if (!PathIsDirectory(SrcTemp))
		{
			::CreateDirectory(SrcTemp, NULL);
		}
		return CreateDirectoryRecursionFun(Src, EndCount + 1);//ʹ��β�ݹ飬��С��Դ����
	}
}

//��ȡ��������
CString IMEIWrite_MulAT::GetDatetime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d-%d-%d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}

//��ȡ��ǰʱ����
CString IMEIWrite_MulAT::GetCurrenttime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%02d_%02d_%02d"),time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//��ȡϵͳʱ��
CString IMEIWrite_MulAT::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d-%d-%d-%02d_%02d_%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}


/*����������������*/

//��ʼ�������׵�һЩ����������������ַ�ȣ�
void IMEIWrite_MulAT::PowerControlInitSetting()
{
	CFileFind finder;
	CString IniFileName = ".\\PowerTestSetting.ini";
	CString ValueStr, SettringStr,tempStr;
	CString SettringTitle1 = "MachineAddress", SettringTitle2 = "RelaySetting";
	int ValueInt;
	double ValueDouble;

	BOOL ifFind = finder.FindFile(IniFileName);//�ȼ���ļ��治����

	if (ifFind)
	{
		for (int i = 0; i < 2; i++)
		{
			CString temp;
			SettringStr.Format("Machine%dType", i);
			GetPrivateProfileString(SettringTitle1, SettringStr, _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
			PowerControlInstrNameArray[i] = ValueStr;
			ValueStr.ReleaseBuffer();
			SettringStr.Format("Machine%dNum", i);
			ValueInt = GetPrivateProfileInt(SettringTitle1, SettringStr, i + 4, IniFileName);
			tempStr.Format("%d",ValueInt);
			PowerControlInstrNameArray[i].Replace("X", tempStr);
		}

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("RelayFlag"), 0, IniFileName);
		RelayFlag = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("BackuppowerFlag"), 0, IniFileName);
		BackuppowerFlag = ValueInt;

		//GetPrivateProfileString���ܶ�������
		GetPrivateProfileString(SettringTitle2, _T("Voltage"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		m_Voltage = atof(ValueStr);
		ValueStr.ReleaseBuffer();

		GetPrivateProfileString(SettringTitle2, _T("Range"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		m_Range = atof(ValueStr);
		ValueStr.ReleaseBuffer();

		GetPrivateProfileString(SettringTitle2, _T("RelayAddress"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		PowerControlRelayInstrName = ValueStr;
		ValueStr.ReleaseBuffer();

		GetPrivateProfileString(SettringTitle2, _T("BackUpPowerType"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		BackUpPowerInstrName = ValueStr;
		ValueStr.ReleaseBuffer();

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("BackUpPowerNum"), 6, IniFileName);
		tempStr.Format("%d", ValueInt);
		BackUpPowerInstrName.Replace("X", tempStr);

		for (int i = 0; i < 8; i++)
		{
			SettringStr.Format("PortSetting%d", i);
			ValueInt = GetPrivateProfileInt(SettringTitle2, SettringStr, i+1, IniFileName);
			PowerControlRelayPortInstrName[i] = ValueInt;
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			PowerControlInstrNameArray[i].Format("GPIB0::%d::INSTR",5+i);
		}

		PowerControlRelayInstrName = "COM6";

		BackUpPowerInstrName.Format("GPIB0::%d::INSTR", 7);

		m_Range = 0.001;
		RelayFlag = 0;
		BackuppowerFlag = 0;
		m_Voltage = 3.0;

		for (int i = 0; i < 8; i++)
		{
			PowerControlRelayPortInstrName[i] = i+1;
		}
	}

}

//�������Ժ��ĺ���
BOOL IMEIWrite_MulAT::CurrentMainControlFun(int HandleNum)
{
	//������ⲿ���У����︺������ײ�����������

	char *p1 = PowerControlInstrNameArray[0].GetBuffer(), *p2 = PowerControlRelayInstrName.GetBuffer();

	//RelayFlagΪFALSEֻ�ⵥ��������ΪTRUEʱֻ��8���˿ڵĵ���
	if (RelayFlag == FALSE)
	{
		for (unsigned int i = 0; i< 3; i++) //У׼�Ĵ���
		{
			//��������
			PortCurrentVauleArray[HandleNum] = CurrentTestArray[0]->currentAverage(p1, 5, m_Range) * 1000; //��ά�򣺵���������
			Sleep(100);
			if (PortCurrentVauleArray[HandleNum] >= 0 && PortCurrentVauleArray[HandleNum] <= 12000); //
			break;
		}

		Current1Deq.pop_front();
	}
	else if (RelayFlag == TRUE)
	{
		if (HandleNum < 4)
		{
			PortCurrentVauleArray[HandleNum] = CurrentTestArray[0]->currentTest(PowerControlRelayPortInstrName[HandleNum], p1, p2, 3, 3, 1000, 0, 12.0, m_Range) * 1000;

			Current1Deq.pop_front();
		}
		else if (HandleNum > 3 && HandleNum < 8)
		{
			PortCurrentVauleArray[HandleNum] = CurrentTestArray[1]->currentTest(PowerControlRelayPortInstrName[HandleNum], p1, p2, 3, 3, 1000, 0, 12.0, m_Range) * 1000;

			Current2Deq.pop_front();
		}
		else
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "��������ʧ�ܣ���ǰֻ֧��ǰ8�����ڵ�������", HandleNum);
			return FALSE;
		}
	}

	return TRUE;
}

//���õ�ѹ������ֵ�����������������ڵ�ѹ�����̾ͻ�ȡ�����е�ֵ��������������ȡini�е�ֵ
void IMEIWrite_MulAT::SetVoltageAndRangeVaule(CString ConfigItem, double &m_Voltage, double &m_Range)
{
	int VoltageCut = ConfigItem.Find("��ѹ:");
	if (VoltageCut!=-1)
	{
		m_Voltage = atof(ConfigItem.Mid(VoltageCut + 5, ConfigItem.Find(",") - VoltageCut - 5));
	}
	int RangeCut = ConfigItem.Find("����:");
	if (RangeCut != -1)
	{
		m_Range = atof(ConfigItem.Right(ConfigItem.GetLength() - RangeCut-5));
	}
}

//��ѹ�������õ�ѹ�͵ȴ�ֵ�����������������ڵ�ѹ�͵ȴ��ͻ�ȡ�����е�ֵ���������������Ĭ��ֵ
void IMEIWrite_MulAT::SetHighVoltageAndDelayVaule(CString ConfigItem, double &m_HighVoltage, double &m_Delay)
{
	int VoltageCut = ConfigItem.Find("��ѹ:");
	if (VoltageCut != -1)
	{
		m_HighVoltage = atof(ConfigItem.Mid(VoltageCut + 5, ConfigItem.Find(",") - VoltageCut - 5));
	}
	else
	{
		m_HighVoltage = 3.8;
	}

	int RangeCut = ConfigItem.Find("�ȴ�:");
	if (RangeCut != -1)
	{
		m_Delay = atof(ConfigItem.Right(ConfigItem.GetLength() - RangeCut - 5));
	}
	else
	{
		m_Delay = 2000;
	}

}

//������������
void IMEIWrite_MulAT::OnBnClickedPowercontrolsettingButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   

	CPowerControlDlg PowerControlDlgDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = PowerControlDlgDlg.DoModal();  // �����Ի���

	if (IDCANCEL == nRes)
		return;

	PowerControlInitSetting();
}


/*�����ֶη��ظ�����*/

//�ֶη��ظ����ܱ�����ʼ����ÿ�����ڿ�ʼ��ʱ��Ҫ���Լ���Ӧ�ı�����ʼ��һ��
void IMEIWrite_MulAT::AntiDupDataInit(int HandleNum)
{
	for (int i = 0; i < AntiDupData; i++)
	{
		AntiDupDataArray[HandleNum][i] = "";
		AntiDupDataVauleArray[HandleNum][i] = "";
	}

	AntiDupDataLinkOrder[HandleNum] = "";
	AntiDupDataLinkFlag[HandleNum][0] = 0;
	AntiDupDataLinkFlag[HandleNum][1] = 0;

	AntiDupDataSNUploadFlag[HandleNum] = FALSE;
	AntiDupDataNoUploadCount[HandleNum][0] = 0;
	AntiDupDataNoUploadFlag[HandleNum] = FALSE;

}

//оƬID��麯��
BOOL IMEIWrite_MulAT::Data_AntiDupSNCheck(CAdoInterface& myado, int HandleNum, CString ChipIDStr)
{
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 5, "�����������ݿ�.......", HandleNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "���ݿ�����������...", HandleNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", HandleNum);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if (ChipIDStr == "")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "SNΪ��", HandleNum);
			return FALSE;
		}

		//�ȼ�����ݱ����Ƿ��д����ݣ��оͱ���û�оͼ���
		myado.OpenSheet("select SN from [GPSTest].[dbo].[Gps_AutoTest_AntiDup] WHERE SN ='" + ChipIDStr + "' ");//��1
		BOOL Barcode_Check = myado.Find("SN='" + ChipIDStr + "'");
		myado.CloseSheet();
		BOOL UP_Barcode, Barcode_Check_UP;

		if (Barcode_Check == TRUE)
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��оƬID�����ݿ����Ѵ��ڣ�", HandleNum);
			return FALSE;
		}
	}
	else
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "SN:" + ChipIDStr + "û�й�ѡ���ݿⲻ�ϴ�...", HandleNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;
}

//оƬID�ϴ�����
BOOL IMEIWrite_MulAT::Data_AntiDupSNUpload(CAdoInterface& myado, int HandleNum, CString ChipIDStr)
{
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 5, "�����������ݿ�.......", HandleNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "���ݿ�����������...", HandleNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", HandleNum);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if (ChipIDStr == "")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "SNΪ��", HandleNum);
			return FALSE;
		}

		BOOL UP_Barcode, Barcode_Check_UP;

		//��ʼ�ϴ����ݵ�Gps_AutoTest_AntiDup
		CString strSQL1 = "Insert into [GPSTest].[dbo].[Gps_AutoTest_AntiDup](SN,ZhiDan,Computer) values('" + ChipIDStr + "','" + ZhiDanCS + "','" + Hostname + Ipaddress + "')";

		UP_Barcode = myado.Execute(strSQL1, &var);

		if (UP_Barcode == TRUE)
		{
			if (var.intVal < 1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}


		if (UP_Barcode == TRUE&&Barcode_Check_UP == TRUE)
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "SN:" + ChipIDStr + "���ݿ��ϴ����ظ���ɹ�...", HandleNum);
		}
		else
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "SN:" + ChipIDStr + "оƬID�����غţ����ݿ��ϴ����ظ���ʧ��", HandleNum);
			return FALSE;
		}
	}
	else
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "SN:" + ChipIDStr + "û�й�ѡ���ݿⲻ�ϴ�...", HandleNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;
}


//�ֶμ�麯��
BOOL IMEIWrite_MulAT::Data_AntiDupDataNoCheck(CAdoInterface& myado, int HandleNum, CString ChipIDStr)
{
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 5, "�����������ݿ�.......", HandleNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "���ݿ�����������...", HandleNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", HandleNum);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if (ChipIDStr == "")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "SNΪ��", HandleNum);
			return FALSE;
		}

		BOOL UP_Barcode = FALSE, Barcode_Check_UP = FALSE, Barcode_Check = FALSE, Barcode_Check_Sub;
		CString SqlStr="select * from [GPSTest].[dbo].[Gps_AutoTest_AntiDup] WHERE";

		//��ƴ���ַ���
		SqlStr += " " + AntiDupDataArray[HandleNum][0] + "='" + AntiDupDataVauleArray[HandleNum][0] + "'";
		for (int i = 1; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
		{
			SqlStr += " OR " + AntiDupDataArray[HandleNum][i] + "='" + AntiDupDataVauleArray[HandleNum][i] + "'";
		}

		myado.OpenSheet(SqlStr);

		//�Ȳ��أ�����ʾ�ظ�
		for (int i = 0; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
		{
			Barcode_Check_Sub = myado.Find(AntiDupDataArray[HandleNum][i] + "='" + AntiDupDataVauleArray[HandleNum][i] + "'");

			//��ʾ�����ظ��ֶ�
			if (Barcode_Check_Sub == TRUE)
				LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, AntiDupDataArray[HandleNum][i] + ":" + AntiDupDataVauleArray[HandleNum][i] + "�Ѵ���", HandleNum);

			Barcode_Check |= Barcode_Check_Sub;
		}
	    myado.CloseSheet();

		//�����Ƿ����²��Է��������
		if (RTestChoose == TRUE)
		{
			//�ֶδ��ڵ������
			if (Barcode_Check == TRUE)
			{
				SqlStr = "select * from [GPSTest].[dbo].[Gps_AutoTest_AntiDup] WHERE SN='" + ChipIDStr + "'";

				//��ƴ���ַ���
				SqlStr += " AND " + AntiDupDataArray[HandleNum][0] + "='" + AntiDupDataVauleArray[HandleNum][0] + "'";
				for (int i = 1; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
				{
					SqlStr += " AND " + AntiDupDataArray[HandleNum][i] + "='" + AntiDupDataVauleArray[HandleNum][i] + "'";
				}

				myado.OpenSheet(SqlStr);
				Barcode_Check_Sub = myado.Find("SN='" + ChipIDStr + "'");
				
				//SN���ҵõ�������һ�£��Ͳ��ϴ����ݣ�����ԭ���̾���,��һ���Ǵ���϶�������
				if (Barcode_Check_Sub == TRUE)
				{
					myado.CloseSheet();
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��ǰоƬID���ֶ������ݿ��ж�Ӧ��оƬIDһ�£�������", HandleNum);
				}
				else if (Barcode_Check_Sub == FALSE)
				{
					myado.CloseSheet();
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��ǰоƬID���ֶ������ݿ��ж�Ӧ��оƬID��һ�£������Ƿ��غţ�", HandleNum);
					return FALSE;
				}

			}
			//�ֶβ����ڵ������
			else if (Barcode_Check == FALSE)
			{
				SqlStr = "select * from [GPSTest].[dbo].[Gps_AutoTest_AntiDup] WHERE SN ='" + ChipIDStr + "' ";

				//myado.OpenSheet("select * from [GPSTest].[dbo].[Gps_AutoTest_AntiDup] WHERE SN ='" + ChipIDStr + "' ");

				//�鿴��ǰ��
				for (int i = 0; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
				{
					//������õ���newid()����ƣ����Ե��ж���������36λ��ͬʱ������-�ַ�������ҵõ����ݵĻ��Ǿʹ����оƬID�Ѿ������������а󶨹�ϵ��
					CString SqlStr1 = SqlStr;
					SqlStr1 += "AND LEN(" + AntiDupDataArray[HandleNum][i] + ") != 36 AND " + AntiDupDataArray[HandleNum][i] + " not like '%-%'";
					myado.OpenSheet(SqlStr1);
					Barcode_Check_Sub = myado.Find("SN='" + ChipIDStr + "'");
					myado.CloseSheet();
					//��ʾ�����ظ��ֶ�
					if (Barcode_Check_Sub == TRUE)
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, ChipIDStr+"��Ӧ��"+AntiDupDataArray[HandleNum][i] + "�Ѵ�������", HandleNum);

					Barcode_Check |= Barcode_Check_Sub;
				}

				if (Barcode_Check == TRUE)
				{
						LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "оƬID�����غţ��������ݿ�", HandleNum);
						return FALSE;
				}
				else if (Barcode_Check_Sub == FALSE)
				{
					LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��оƬ��Ӧ���ֶ�Ϊ�գ����Գɹ��󽫻��ϴ��ֶ�", HandleNum);
					AntiDupDataNoUploadFlag[HandleNum] = TRUE;
				}
			}
		}
		else if (RTestChoose == FALSE)
		{
			//ǰ������ظ��ͱ�������ͽ��ϴ���־λ��Ϊ��

			if (Barcode_Check == TRUE)
			{
				if (AntiDupDataLinkFlag[HandleNum][0] == 0 || AntiDupDataLinkFlag[HandleNum][0] == 2)
				return FALSE;
			}
			else if (Barcode_Check == FALSE)
			{
				AntiDupDataNoUploadFlag[HandleNum] = TRUE;
			}
		}
	}
	else
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "SN:" + ChipIDStr + "û�й�ѡ���ݿⲻ�ϴ�...", HandleNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;
}

//�ֶ��ϴ�����
BOOL IMEIWrite_MulAT::Data_AntiDupDataNoUpload(CAdoInterface& myado, int HandleNum, CString ChipIDStr)
{
	_variant_t var;
	CString DB_FAIL;
	if (IMEI_Setdlg.Initial_Connect_DB == TRUE)
	{
		CString Conn = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", IMEI_Setdlg.m_server, IMEI_Setdlg.m_db, IMEI_Setdlg.m_user, IMEI_Setdlg.m_pwd);

		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 5, "�����������ݿ�.......", HandleNum);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
		}
		else
		{
			//LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "���ݿ�����������...", HandleNum);
		}
		if (DB_FAIL == "FAIL")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "�������ݿ�ʧ�ܣ��޷��ϴ����ݣ�������������.......", HandleNum);
			return FALSE;
		}

		OrderNumbersControl.GetWindowTextA(ZhiDanCS);
		if (ChipIDStr == "")
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "SNΪ��", HandleNum);
			return FALSE;
		}

		BOOL UP_Barcode = FALSE, Barcode_Check_UP = FALSE, Barcode_Check = FALSE, Barcode_Check_Sub;

		//BOOL UP_Barcode, Barcode_Check_UP;


		//�ȼ�����ݱ��Ƿ����оƬID�����ھͲ��룬�����ھ͸���
		myado.OpenSheet("select SN from [GPSTest].[dbo].[Gps_AutoTest_AntiDup] WHERE SN ='" + ChipIDStr + "' ");//��1
		Barcode_Check = myado.Find("SN='" + ChipIDStr + "'");
		myado.CloseSheet();

		//���²���
		if (Barcode_Check == TRUE)
		{
			CString strSQL_Write_Barcode = "UPDATE [GPSTest].[dbo].[Gps_AutoTest_AntiDup] SET " + AntiDupDataArray[HandleNum][0] + "='" + AntiDupDataVauleArray[HandleNum][0] + "'";
			
			for (int i = 1; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
			{
				strSQL_Write_Barcode += ","+AntiDupDataArray[HandleNum][i] + "='" + AntiDupDataVauleArray[HandleNum][i]+"'";

			}
			strSQL_Write_Barcode +=  " WHERE SN ='" + ChipIDStr + "'";
			
			UP_Barcode = myado.Execute(strSQL_Write_Barcode, &var);

		}
		//�������
		else if (Barcode_Check == FALSE)
		{
			CString strSQL1 = "Insert into [GPSTest].[dbo].[Gps_AutoTest_AntiDup](SN,ZhiDan,Computer";

			for (int i = 0; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
			{
				strSQL1 += "," + AntiDupDataArray[HandleNum][i];
			}
			strSQL1 += ") values('" + ChipIDStr + "','" + ZhiDanCS + "','" + Hostname + Ipaddress + "'";

			for (int i = 0; i < AntiDupDataNoUploadCount[HandleNum][0]; i++)
			{
				strSQL1 += ",'" + AntiDupDataVauleArray[HandleNum][i]+"'";
			}

			strSQL1 += ")";

			UP_Barcode = myado.Execute(strSQL1, &var);
		}

		if (UP_Barcode == TRUE)
		{
			if (var.intVal < 1)//if(var.intVal!=1)
				Barcode_Check_UP = FALSE;
			else
				Barcode_Check_UP = TRUE;
			//�ٴμ��
		}


		if (UP_Barcode == TRUE&&Barcode_Check_UP == TRUE)
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "���ݿ��ϴ��ֶε����ظ���ɹ�...", HandleNum);
		}
		else
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 128, "���ݿ��ϴ��ֶε����ظ���ʧ��", HandleNum);
			return FALSE;
		}
	}
	else
	{
		LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 0, "SN:" + ChipIDStr + "û�й�ѡ���ݿⲻ�ϴ�...", HandleNum);
		return TRUE;
	}
	myado.CloseDataLibrary();
	return TRUE;
}


/*DAM�̵�����������*/

CString IMEIWrite_MulAT::UART_CRC16_Work(unsigned char CRC_Buf[], int CRC_Leni)
{
	int i, j;
	unsigned short CRC_Sumx, CRC_Temp;
	CString CRC_Rec;
	CRC_Sumx = 0xFFFF;

	for (i = 0; i < CRC_Leni; i++)
	{
		CRC_Sumx = (CRC_Buf[i]) ^ CRC_Sumx;//���
		for (j = 0; j < 8; j++)
		{
			if (CRC_Sumx & 0x01)
			{
				CRC_Sumx >>= 1;
				CRC_Sumx ^= 0xa001;
			}
			else
			{
				CRC_Sumx >>= 1;
			}
		}
	}
	CRC_Temp = CRC_Sumx >> 8;
	CRC_Temp = CRC_Temp | (CRC_Sumx << 8);
	printf("%04X\n", CRC_Temp);
	CRC_Rec.Format("%04X", CRC_Temp);
	return (CRC_Rec);
}

/*
�ƿ��ļ̵������ƺ���
Address:���Ƶļ̵����ĵ�ַ ���⣺-1��ѯ�̵��� -2ȫ��ȫ��
PowerAction:���ض���
����ֵΪ����
*/
CString IMEIWrite_MulAT::PowerSet(int Address, bool PowerAction)
{
	//CPublicFunction pf;
	CString HexTemp, SendText, DictateNum;
	int HexLen = 1;
	unsigned short CrcTemp;
	//SendText.Format("%X", atoi(RadioAddress));
	SendText.Format("%X", 254);//254�Ǽ̵����㲥��Ĭ�ϵ�ַ
	if (Address == -1)             //��ʱ����
	{
		HexTemp = "01";
		HexLen++;
		return "FALSE";
	}
	else if (Address == -2)		 //��ʱ����
	{
		HexTemp = "0F";
		HexLen++;
		return "FALSE";
	}
	else if (Address > -1 && Address < 32)
	{
		HexTemp.Format("0500%02X", Address);
		HexLen += 3;
	}
	else
	{
		return "FALSE";
	}
	SendText += HexTemp;
	if (PowerAction)
	{
		HexTemp = "FF00";
	}
	else
	{
		HexTemp = "0000";
	}
	HexLen += 2;
	SendText += HexTemp;
	BYTE * HexTemp2;
	HexTemp2 = CStrToByte(6, SendText);
	//unsigned char buff[6] = { 0 };
	//buff[0] = 0xFE;
	//buff[1] = 0x05;
	//buff[2] = 0x00;
	//buff[3] = 0x00;
	//buff[4] = 0x00;
	//buff[5] = 0x00;
	HexTemp = UART_CRC16_Work(HexTemp2, 6);
	SendText += HexTemp;
	return SendText;
}

BOOL IMEIWrite_MulAT::DamControlFun(int HandleNum, CString OrderNum)
{
	int count,DamNo;//DamNoΪ�̵����˿�
	CString CommandStr;
	BOOL ActionJudge;//�̵���������TRUEΪ�򿪣�FALSEΪ�ر�

	//��(�ر�)ͨ·Ϊ���Ե������1��Ӧ�̵����˿�0-7�����2��Ӧ�̵����˿�8-15�����3��Ӧ�̵����˿�16-23�����4��Ӧ�̵����˿�24-31
	if (OrderNum.Find("ͨ·") != -1)
	{
		ExternalCircuit[HandleNum][0] = TRUE;
		if (OrderNum.Find("��") != -1)
		{
			ActionJudge = TRUE;
		}
		else if (OrderNum.Find("�ر�") != -1)
		{
			ActionJudge = FALSE;
		}
		DamNo = HandleNum;
	}
	else if (OrderNum.Find("���")!=-1)
	{
		count = sscanf_s(CStringA(OrderNum), "���%d", &DamNo);//�ø�ʽ��������һ������
		if (count == 0 || DamNo>4 || DamNo<1)
		{
			LogShow_exchange(m_ResultArray[HandleNum], Final_Result_ControlArray[HandleNum], 6, "��Ŀ���ƴ���ȷ��Ϊ���1�����2�����3�����4��(��)", HandleNum);
			return FALSE;
		}
		ExternalCircuit[HandleNum][DamNo] = TRUE;
		DamNo = HandleNum + (DamNo - 1) * 8;
		if (OrderNum.Find("��") != -1)
		{
			ActionJudge = TRUE;
		}
		else if (OrderNum.Find("��") != -1)
		{
			ActionJudge = FALSE;
		}

	}
	CommandStr=PowerSet(DamNo, ActionJudge);
	CString ReturnStr;
	int Damcount=0;
	do{
		Sleep(400);
		Damcount++;
		Send_Serial_Order(&ReturnStr, CommandStr, 16, (LPSTR)(LPCTSTR)CommandStr, "", 1, 2);
		if (ReturnStr == "Analysis_SUCCESS")
		{
			break;
		}
	} while (Damcount < 6);

	if (ReturnStr == "FAIL")
	{

		return FALSE;
	}
	Sleep(100);
	return TRUE;
}

BOOL IMEIWrite_MulAT::CloseExternalCircuit(int HandleNum)
{
	BOOL Result=TRUE;

	if (ExternalCircuit[HandleNum][0] == TRUE)
	{
	    Result &= DamControlFun(HandleNum, "�ر�ͨ·");
		Sleep(250);
	}
	if (ExternalCircuit[HandleNum][1] == TRUE)
	{
		Result &= DamControlFun(HandleNum, "���1��");
		Sleep(250);
	}
	if (ExternalCircuit[HandleNum][2] == TRUE)
	{
		Result &= DamControlFun(HandleNum, "���2��");
		Sleep(250);
	}
	if (ExternalCircuit[HandleNum][3] == TRUE)
	{
		Result &= DamControlFun(HandleNum, "���3��");
		Sleep(250);
	}
	if (ExternalCircuit[HandleNum][4] == TRUE)
	{
		Result &= DamControlFun(HandleNum, "���4��");
		Sleep(250);
	}
	return Result;
}








