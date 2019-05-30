// WriteIMEIDlg.h : header file
//

#if !defined(AFX_WRITEIMEIDLG_H__B59F17C9_CC97_4D41_B3AB_136C646D895A__INCLUDED_)
#define AFX_WRITEIMEIDLG_H__B59F17C9_CC97_4D41_B3AB_136C646D895A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColReadOnlyEdit.h"
#include "afxwin.h"

#include "IniFile.H"
#define		STRING_PATH_CONFIG		"\\config\\Seting_CONFIG.dll"
#define		STRING_SECTION_CONFIG		"SETTING"

#define		SERVER_RESULT		"\\tem.dll"
#define		SERVER_RESULT_SECTION		"T_SERVER_SETTING"

#define     STRING_PATH_BLE         "config\\BLE_ITEM.dll"
#define     STRING_SECTION_BLE      "BLE"

#define     MAXPARANUM       100

#include "BT_Addr_Setting.h"
#include "Login.h"
#include "PhoneCommand.h"       //չѶƽ̨

#include "UDP.h"
#include "METAAPP.h"
#include "sp_brom.h"
#include "spmeta.h"

//SOCKETͨ��
#include "ActiveSock.h"
#include "EventWrapper.h"
/////////////////////////////////////////////////////////////////////////////
// CWriteIMEIDlg dialog
/*typedef enum _MOCOR_VER_E
{
	MOCOR_VER_PRE09A37=0,
	MOCOR_VER_AFTER09A37
} MOCOR_VER_E;   */

#define destPORT 137  //nbtstat name port
#define myPORT 4321

#define USB_COM_PORT  9999
#define BT_ADDRESS_MAX_LENGTH  12   
#if !defined(STRUCT_PARAMETER)
#define STRUCT_PARAMETER
//�ṹ��1
typedef struct {
	signed int iMeta_handle;
	signed int iMetaAP_handle;
	AUTH_HANDLE_T  t_AuthHandle;
	signed int bootstop;

	bool bIsDatabaseInitialized [2]; //[0] for Modem; [1] for AP
	bool bAuthenEnable;
	bool bIsConnected;

} MetaCommon_struct;
static MetaCommon_struct g_Meta;  


//�ṹ��2
typedef struct {

	META_Connect_Req tMETA_Connect_Req;
	META_Connect_Report tMETA_Connect_Report;
	METAAPP_RESULT eMetaApp_Result;
	//E_INPUTBOX_STATUS eRunStatus;//����״̬

		
	META_RESULT eMeta_Result;
	FT_NVRAM_WRITE_REQ tNVRAM_WriteReq;
	FT_NVRAM_READ_REQ tNVRAM_ReadReq;
	FT_NVRAM_READ_CNF tNVRAM_ReadCnf;
	
	HANDLE m_WriteToNVRAMEvent;
	HANDLE m_ReadFromNVRAMEvent;

	META_ConnectByUSB_Req usb_req;
	META_ConnectByUSB_Report usb_report;

}  MetaModem_struct;
static MetaModem_struct g_MetaModem;    

#define META_CONNECT_TIME_OUT  30000  //30000   
#define META_BOOT_TIMERS  3   

//DUT����
typedef enum {
	TARGET_FEATURE_PHONE = 0,
	TARGET_SMARTPHONE,
	PCMCIA_CARD,
	
} E_TARGET_TYPE;

typedef struct {

	META_ConnectInMETA_Req tMETA_Connect_Req;
	META_ConnectInMETA_Report tMETA_Connect_Report;
	META_RESULT eMeta_Result;
	META_RESULT eMetaAP_Result;    //�¼�
	METAAPP_RESULT eMetaApp_Result;//�¼�
	//WM_META_ConnectInMETA_Req tMETA_Connect_ReqWM;      //�¼�
	//WM_META_ConnectInMETA_Report tMETA_Connect_ReportWM;//�¼�
	//E_INPUTBOX_STATUS eRunStatus;
	FT_NVRAM_WRITE_REQ tNVRAM_WriteReq;
	FT_NVRAM_READ_REQ tNVRAM_ReadReq;
	FT_NVRAM_READ_CNF tNVRAM_ReadCnf;
	//EXEC_FUNC cb;
	HANDLE m_WriteToNVRAMEvent;
	HANDLE m_ReadFromNVRAMEvent;
} MetaModem6516_struct;
static MetaModem6516_struct g_Meta6516Modem; 

//------------------------------------------------------------------------------
// New boot mode for SP                                                               
static SP_BOOT_ARG_S m_stModeArg;    

typedef struct  {
	unsigned long  read_retry_time;			//     0 means default  (2400 times)
	unsigned long  read_interval_timeout;	// ms, 0 means default  (10 ms)
	int			 * m_p_stopflag;
}Meta_Boot_Arg;

//Errors
typedef enum 
{
	EBOOT_SUCCESS = 10000,
	EBOOT_EXCEPTION,

	//USER
	USER_PRESS_STOP_BUTTON,
	WAIT_FOR_PLATFORM_ERROR,

	// COM PORT ERROR
	OPEN_COM_PORT_ERROR = 11000,
	SETUP_COM_PORT_ERROR,
	GET_COM_STATE_ERROR,
	SET_COM_STATE_ERROR,
	PURGE_COM_PORT_ERROR,
	SET_COMM_TIMEOUTS_ERROR,

	//Boot ERROR
	READ_READY_PATTERN_ERROR = 12000,

	WRITE_META_PATTERN_ERROR,
	READ_META_ACK_PATTERN_ERROR,

	WRITE_ADVMETA_PATTERN_ERROR,
	READ_ADVMETA_ACK_PATTERN_ERROR,

	WRITE_DOWNLOAD_PATTERN_ERROR,
	READ_DOWNLOAD_ACK_PATTERN_ERROR,

	WRITE_FACTORYM_PATTERN_ERROR,
	READ_FACTORYM_ACK_PATTERN_ERROR,

	WRITE_FACTFACT_PATTERN_ERROR,
	READ_FACTFACT_ACK_PATTERN_ERROR,

	WRITE_SWITCHMD_PATTERN_ERROR,
	READ_SWITCHMD_ACK_PATTERN_ERROR,

	WRITE_MPWAIT_PATTERN_ERROR,
	READ_MPWAIT_ACK_PATTERN_ERROR,

	WRITE_MPGOON_PATTERN_ERROR,
	READ_MPGOON_ACK_PATTERN_ERROR,

	WRITE_CLEAN_BOOT_PATTERN_ERROR,
	READ_CLEAN_BOOT_ACK_PATTERN_ERROR,

	//USB PORT ERROR
	OPEN_USB_PORT_ERROR = 13000,
	USB_PORT_READ_FILE_ERROR,
	USB_PORT_READ_FILE_TIMEOUT_ERROR,
	USB_PORT_WRITE_FILE_ERROR,
	USB_PORT_READ_FILE_LEN_ERROR,
	USB_PORT_WRITE_FILE_LEN_ERROR,
	USB_PORT_SET_TIMEOUT_ERROR,
	USB_PORT_TOO_MANY_ERROR,
	USB_PORT_LOAD_WINDEV_DLL_ERROR,

	//ANDROID DOWNLOAD ERROR
	NOT_FLASH_INFO_PKT_ERROR = 14000,
	NOT_PL_INFO_PKT_ERROR,
	NOT_IMAGE_INFO_PKT_ERROR,
	NOT_RESP_INFO_PKT_ERROR,
	NOT_FOUND_INFO_PKT_ERROR,
	OPEN_IMAGE_FILE_ERROR,
	GET_IMAGE_FILE_SIZE_ERROR,
	IMAGE_FILE_PATH_IS_NULL_ERROR,
	READ_IMAGE_FILE_ERROR,
	WRITE_IMAGE_FILE_ERROR,
	SEND_IMAGE_FILE_ERROR,
	UNKNOWN_SEND_IMAGE_CHECK_PACKET_ERROR,
	SEND_PATTERN_ERROR,
	SEND_PATTERN_SIZE_ERROR,
	ANDROID_VERIFY_FAIL_ERROR,

	//Secured download
	CHECK_SECURED_MARK_ERROR,
	GET_ENCRYPT_INFO_ERROR,
	GET_ENCRYPT_DATA_ERROR,
	SET_FILE_POINTER_ERROR,

	//DA download error
	SEND_DA_VERIFY_DATA_ERROR,
	RECEIVE_DA_VERIFY_RESPONSE_ERROR,
	SEND_DA_INFO_ERROR,
	SEND_DA_DATA_ERROR,
	DA_LOCKED_ERROR,

         //Image patch & lock
	INFORM_IMAGE_PACKET_ERROR,
	PATCH_CMD_PACKET_ERROR,
	SEND_SECURITY_PACKET_ERROR,
	IMAGE_LOCK_RESPONSE_ERROR,
	SET_LOCK_RESPONSE_ERROR,
	RECEIVE_CHECKSUM_ERROR,
	META_LOCKED_ERROR,

	//Partition table checking
	SEND_PARTITION_INFO_PACKET_ERROR,
	RECEIVE_PARTITION_RESPONSE_ERROR,

	//Target response error message
	TARGET_NOMEM_ERROR        = 16000,        /* no memory */
	TARGET_NAND_RD_ERROR,
	TARGET_NAND_WR_ERROR,
	TARGET_NAND_ER_ERROR,
	TARGET_WRONG_SEQ_ERROR,
	TARGET_WRONG_ADDR_ERROR,
	TARGET_WRONG_PKT_SZ_ERROR,
	TARGET_EXCEED_BOUNDARY_ERROR,
	TARGET_INVALID_TABLE,
	TARGET_SPACE_NOT_FOUND,
	TARGET_UNKNOWN_ERROR,

	//Other Error
	OPEN_FLASH_BIN_FILE_ERROR    = 20000,
	READ_FLASH_BIN_FILE_ERROR,
	ALLOC_MEMORY_ERROR,

	EBOOT_RESULT_END = 0x7FFFFFFF,

}EBOOT_RESULT;

typedef enum
	{
		PreloaderUSB = 0,
		BootROMUSB = 1
	}USBType;

static const GUID GUID_PORT_CLASS_USB2SER ={0x4D36E978L,0xE325,0x11CE,{0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}};


typedef struct {

	//META_ConnectInMETA_Req tMETA_Connect_Req;
	//META_ConnectInMETA_Report tMETA_Connect_Report;
	META_RESULT eMeta_Result;
	META_RESULT eMetaAP_Result;    //�¼�
	METAAPP_RESULT eMetaApp_Result;//�¼�
	WM_META_ConnectInMETA_Req tMETA_Connect_Req;      //�¼�
	WM_META_ConnectInMETA_Report tMETA_Connect_Report;//�¼�
	//E_INPUTBOX_STATUS eRunStatus;
	FT_NVRAM_WRITE_REQ tNVRAM_WriteReq;
	FT_NVRAM_READ_REQ tNVRAM_ReadReq;
	FT_NVRAM_READ_CNF tNVRAM_ReadCnf;
	//EXEC_FUNC cb;
	HANDLE m_WriteToNVRAMEvent;
	HANDLE m_ReadFromNVRAMEvent;
} MetaAP_struct;
static MetaAP_struct g_MetaAP;  


//WIFI��ַ
#define EEPROM_SIZE 512  


typedef struct
{
    CString paraID;     //ָ��Ψһ��ID��//û����
    CString paraContent;//��Ӧ��ָ������//û����

	bool     ifCheck;//�Ƿ�ѡ��
	CString showName;         //��ʾ���б��еĲ�����
    CString Low_Limit_Value;  //����ֵ
	CString High_Limit_Value; //����ֵ���ַ�Χ
	CString Other_ITEM;       //ָ��
}PARAMETER_MAIN;

//дNVRAM�������ͣ�����дMT6592оƬ��BT��ַ
typedef enum
{
    WRITE_BARCODE = 0,
    WRITE_IMEI,
    WRITE_BT,
    WRITE_WIFI,
    WRITE_ETHERNET_MAC,
    WRITE_PRODINFO,
    WRITE_TEMPERATURE
}WriteData_Type_e;


#define MT5921 0x5921
#define MT5931 0x5931
#define MT6620 0x6620
#define MT6628 0x6628
#define MT6571 0x6571
#define MT6572 0x6572
#define MT6582 0x6582
#define MT6592 0x0092
#define MT6595 0x6630
#define MT8127 0x8127

#endif
////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

class CWriteIMEIDlg : public CDialog
{
// Construction
public:
	int autow;
	int TotalCount;
	void PrintError(CString cs);
	CWriteIMEIDlg(CWnd* pParent = NULL);	// standard constructor
	bool INITIAL_PROCESS;//��һ������ʱ��IMEIҪ�仯���쳣����
	int  INITIAL_Paint;  //��ʼ���У��ƶ�����
// Dialog Data
	//{{AFX_DATA(CWriteIMEIDlg)
	enum { IDD = IDD_WRITEIMEI_DIALOG };
	CComboBox	m_Auto;
	CStatic	m_Static_Type;
	CButton	m_Write;
	CComboBox	m_Type;
	CComboBox	m_Baud;

	CString IMEI_OK_LAST;   //��������

	CEdit	m_Result;
	CString LOG_Show;
	CString LOG_Show_Old;
	CString LOG_Show_Old_ALL;
	CString LOG_Time;
	CComboBox	m_Port;
	CColReadOnlyEdit	m_Finalresult;
	CString	m_IMEI;
	CString BoardNumber;
	CString Software_version;
	CString IMEI_Num;

	CString Order_IMEI;
	CString Order_BT;
	CString Order_Software_version;
	CString Order_Barcode;

	CString Order_Recover_Status;
	CString Order_Copy_Para;     //���ݱ���

	//������
	CString Unlock_code_Number;
	//}}AFX_DATA


	//�������ݿ�
	CString   UP_DB_Data_M;
	CString   Initial_Connect_DB_M;
	CString	  DB_CONNECT_M;
	CString   UP_ONE_OK_SECOND_FAIL;
	CString   UP_SN_COPY;
	CString   UP_BT_COPY;
	CString   UP_BARCODE_COPY;
	CString   UP_IMEI_COPY;

	//IMEI
	CString   IMEI1_Choose_M;
	CString   IMEI2_Choose_M;
	CString   IMEI3_Choose_M;
	CString   IMEI4_Choose_M;
	CString   IMEI_CheckValid_Choose_M;
	CString   AutoIMEI_Choose_M;

	CString   UseSameIMEI_Choose_M;
	CString   IMEI_DB_Choose_M;
	CString   MAC_DB_Choose_M;
	
	//BT_MAC
	CString   BT_MAC_ADDR_Choose_M;
	CString   AutoBTAddr_Choose_M;

	CString   WIFI_MAC_ADDR_Choose_M;
	CString   AutoWIFIAddr_Choose_M;
	//���
	CString  BARCODE_Choose_M;
	CString  ChangeEN_Choose_M;
	//MSN
	CString   MSN_Upload_Choose_M;
	//ORACLE���ݿ�����
	CString    ORACLE_Connect_Choose_M;
	//�غ�
	CString   Check_repeat_Choose_M;

	CString   Software_Version_Check_Choose_M;
	CString   IMEI_MAC_Check_Choose_M;

	CString   Scan_Choose_M;

	CString   Repeat_AutoStart_M;

	//������
	CString  Unlock_Code_Choose_M;

	//������
	CString NETCODE_Choose_M;
	CString NETCODE_Only_Num_Choose_M;
	CString JUST_UpDate_DB_Choose_M;
	bool IMEI1_Exist;

	CString IMEI_Exist_Update_NC_Choose_M;

	CString Machine_Type_NC_CompareChoose_M;
	//����IMEI_MSN��Ӧ���е�ʹ�ñ�־
	CString UpdateIMEI_MSN_Choose_M;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteIMEIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWriteIMEIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWrite();
	afx_msg void OnSelchangeAuto();
	afx_msg void OnChangeImei();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CBT_Addr_Setting Setdlg;
	CBT_Addr_Setting Setdlg_Oracle;
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 
public:
	void  SaveConfig();
	void  SaveConfig_Change();
	void  LoadConfig();
	void  LoadConfig_Change();
	void  LoadResult_Time();//��ȡ��������Ϣ
	void  SaveResult_Time();
	int  CharToHexChar(char ch);
	CEdit IMEI_Control;
	CStatic m_Static_Type_BT;
	CString m_BT_Addr;
	CString m_BT_Addr_Temp1;
	CString m_BT_Addr_Temp2;
	CString string_BT;
	long BtAddrCount_int;
	long BtAddrTotal_int;

	long IMEI_Count_int;

	
	afx_msg void OnBnClickedButtonstart1();
	// ������ַ����
	//CString BtAddrCount_whole_situation;
	CString BtAddrCount;
	CString BtAddrTotal;
	CString BtAddrStart_M;
	CString BtAddrEnd_M;
	CString m_Result_C;
	//WIFI��ַ����
	CString WIFIAddrStart_M;
	CString WIFIAddrEnd_M;
	long    WIFIAddrCount_int;
	long    WIFIAddrTotal_int;
	//����
	CString NetCodeStart_M;
	CString NetCodeEnd_M;
	CString NetCode_SwiftNumber_M;
	//IMEI��
	CString IMEIStart_M;
	CString IMEIEnd_M;
	//�ϴ���Ϣ
	CString Order_Num_up_M;
	CString Machine_Type_M;
	CString Modulation_Tppe_M;
	CString Line_Number_M;
	CString Floor_M;
	CString Product_Address_M;
	CString Is_Return_M;
	CString Software_Version_M;
	CString Hardware_Version_M;
	CString ServerIP_MAC_M;      //����ʹ�ô�������
	CString ServerIP_MAC_M_NoTimes;
	long    USE_Times_Count;     //ʹ�ô�������
	CString Plat_Limit_Enable;   //ƽ̨ѡ����Ȩ������
	CString WatchDog_Time_M;
	CString AutoIMEI_Start_Num_M;//�Զ�����IMEI�ŵ���ʼ�ŵ�ǰ14λ�����ݸ�����    m_int64DefaultIMEI

	CString AutoWIFIAd_Start_Num_M;

	CString Operator_M;
	CString BT_Addr_EndShow_M;
	//�������ݿ�
	CString m_server_M;
	CString	m_db_M ;
	CString	m_user_M;
	CString	m_pwd_M ;

	//����ORACLE���ݿ�
	CString ORACLE_m_server_M;
	CString	ORACLE_m_user_M;
	CString	ORACLE_m_pwd_M ;


	CString DestFile_M;




	CString sTemp,szTemp;
	CEdit SN_Control;
	CString m_SN;
	int IMEIorSN;
	// SN��λ��
	int SN_Count;
	CString SN_Count_S;
	CString SN_PRE;
	CString IMEI_PRE;
	CEdit IMEI_PRE_Control;
	CEdit SN_PRE_Control;
	CEdit SN_Count_Control;
	afx_msg void OnBnClickedButton2();
	CString m_Type_S;
	int m_Type_N;

	//ƽ̨����
	CString Plat_Form_S;//�ǿؼ�����
	//CString Plat_Form_CS;//�ֲ�����
	//int  Plat_Form_N;    //����Ҫ

	afx_msg void OnEnChangeImei3();
	void MAIN_Function();//����������
	// �ȴ�����ʱ��
	long Wait_time;
	CString Wait_time_S;
	// msn�����ݿ��л�ȡ
	BOOL MSN_Relative_IMEI;
	CString MSN_Relative_IMEI_S;
	CButton MSN_Relative_IMEI_Control;

	BOOL BT_Write_Enable;
	CString BT_Write_Enable_S;
	CButton BT_Write_Enable_Control;
	CEdit m_BT_Addr_Control;

	CString Machine_Type;

	BOOL Recover_Status;
	CButton Recover_Status_Control;
	CString Recover_Status_S;

	// ���GSM��BT��Ϲ�λʹ��
	BOOL CHECK_GSM_TEST_Enable;
	BOOL CHECK_BT_TEST_Enable;
	CButton CHECK_GSM_TEST_Enable_Control;
	CButton CHECK_BT_TEST_Enable_Control;
	CString CHECK_GSM_TEST_Enable_S;
	CString CHECK_BT_TEST_Enable_S;
	afx_msg void OnBnClickedButton3();

	CString CheckIMEI_LOG;
	BOOL CheckIMEI();
	__int64 ComputeCD(__int64 nImei);
	// IMEI2
	CString m_IMEI2;
	CString m_IMEI3;
	CString m_IMEI4;
	// WIFI  MAC��ַ
	CString m_strWifi;
	//����汾
	CString Version_SoftWare;

	HANDLE m_hEnterModeSuccess;
	HANDLE m_hExitThread;
	//HANDLE m_hEnterModeThread;
	CWinThread* m_hEnterModeThread;
	//static DWORD EnterModeProc(LPVOID lpParam);//�̺߳�����ȫ�ֱ���
	DWORD EnterModeFunc(WPARAM, LPARAM);
	void ReadBack();
	bool WriteAndThenReadBack();//չѶ8810ƽ̨;wifi/btMACд�����������Ч��META��ʽ��
	void WriteFun();
	BOOL CheckVersion();
	int WriteMoc1();
	int WriteMoc2();

	void ReflashUI();
	void AutoCreateIMEI(LPTSTR lpszIMEI);    //�Զ�����IMEI��ַ
	void AutoCreateBTAddr(LPTSTR lpszTempBT);//�Զ�����BT��ַ
	void AutoCreateWifiAddr(LPTSTR lpszTempWIFI);

	DWORD m_dwStartTime;
	DWORD m_dwEnterModeWait;//������Ҫwaiting һ��ʱ��Ž�ģʽ

	CString m_bUsbMode;//USBģʽ
	__int64 m_int64DefaultIMEI;
	CString AutoIMEI; //�Զ����ɵ�IMEI��
	int m_nComPort;   //�˿ں�
	//int WriteMocChoose;//оƬѡ��
	CString SN_MAC;
	CString MAC;

	CString   TestMode_M;//����ģʽ

	CString   Unlock_code_M;//�������λ��



	afx_msg void OnBnClickedButton4();
	// IMEI�ؼ�
	CEdit IMEI2_Control;
	CEdit IMEI3_Control;
	CEdit IMEI4_Control;
	CEdit m_WIFI_Addr_Control;


	void BT_WIFI_Choose(); //BI/WIFI��ַдѡ��
	afx_msg void OnEnChangeImei7();
	afx_msg void OnEnChangeImei10();
	afx_msg void OnEnChangeImei9();
	afx_msg void OnEnChangeBtAddr();
	// BT_MAC��ַǰ�漸λ
	CString BT_PRE;
	CEdit BT_PRE_Control;
	afx_msg void OnEnChangeBtAddr2();
	// WIFIǰ׺
	CString WIFI_PRE;
	CEdit WIFI_PRE_Control;
	//IMEI�Ŷ�
	long    IMEIStart_Number;
	long    IMEIEnd_Number;
	//IMEI\BT_MAC\WIFI_MAC�жϽ��
	bool    IMEI2_Result,IMEI3_Result,IMEI4_Result,BT_Result,WIFI_Result;//�������ݿ����ʱ�������Ľ���ж�
	afx_msg void OnEnChangeEdit1();
	// IMEIʹ�ü���
	CString IMEI_Count;
	// MAC�����༭���Ƶ�ʹ�ܿ���
	CEdit BtAddrCount_Control;
	// IMEI�����༭�ؼ�����
	CEdit IMEI_Count_Control;
	afx_msg void OnBnClickedButton5();

	SP_HANDLE g_hDiagPhone;
	CHANNEL_ATTRIBUTE ca;

	//��ȡ��������Ϣ
	CString IPCommand(int Select_Order);
	void SendIP_Ping(int Select_Order);
	void OnTimerFunction(UINT nIDEvent);//�Ƕ�ʱ��
	CString PingOutput;
	void OnReceive();
	CUDP m_UDPSocket;

	//���ݿ������
	CString T_SERVER;
	CString USE_Times_Count_SERVER_CS;

	//MTK
	METAAPP_RESULT MetaAppResult;
	int HandleForMETA;
	METAAPP_RESULT Initialization(void);
	int PowerOn2Connect_SP(int WorkingMS); // Smart phone version of PowerOn2Connect

	//��ʼ��Modem
	bool MetaModemHandleInit ();
	//��ʼ��MetaAP
	bool MetaAPHandleInit () ;

	//��ʼ��Authentication
	bool REQ_AUTH_Create( void );
	bool REQ_AUTH_Load(const char *filepath );
	bool AuthHandleInit ();

	//��ʼ��SP Authentication
	bool SPATE_AUTH_Create( void );
	bool SPATE_AUTH_Load( const char *auth_filepath );
	bool SpAuthHandleInit ();

	//��ʼ��SPATE Authentication
	//bool SPATE_AUTH_Create( void );
	//bool SPATE_AUTH_Load( const char *auth_filepath );
	bool SpSecuHandleInit ();

	//META������ݳ�ʼ���ܺ���
	void MetaContextInit (void);

	//META������ݳ�ʼ�������Ӻ���
	int REQ_AUTH_Unload( void );
	int REQ_AUTH_Destroy( void );
	//META������ݳ�ʼ������
	void MetaContextDeInit (void);


	CString Get_Printer_Para(CString name);//��ȡ��ӡ���ò���
	bool DB_UP(int Write_Result_In);						//���ݿ��ϴ�����
	void OnCbnSelchangeCombo2_Fresh();	//ˢ�´���
	void Leadcore_Write();				//��о4Gģ��д��
	//M255-����AT
	DWORD OnWrite_Func (WPARAM, LPARAM);//M255-����AT
	//����������
	void TranslateMuch(CString m_old);
	//���ܻ�дIMEI���ܺ���
	void swap_c ( unsigned char* x, unsigned char* y );
	DWORD func_imei_meta_hdlr_feature_phone (WPARAM, LPARAM);

	//Nvram��ʼ��
	bool NvramDatabaseInit ();
	bool IsNvramDatabaseInit (void );
	//META���Ӳ�������
	void SetMetaConnectReq ( void );
	//METAģʽ�����ֻ�
	bool EnterMetaMode () ;


	//д������ַ����
	bool func_bt_address_meta_hdlr_feature_phone ( void );
	signed char  Ascii2BCD ( unsigned char  iAscii );
	META_RESULT REQ_WriteBT6611Addr2NVRAM_Start (unsigned int iBTId, unsigned char* pBTAddress);
	META_RESULT REQ_ReadFromNVRAM ( void );
	META_RESULT REQ_WriteNVRAM(void);

	META_RESULT REQ_ReadBT6611AddrFromNVRAM_Start ( unsigned int iBTId, unsigned char* pBTAddress );

	//дWIFI��ַ����
	bool func_wifi_mac_meta_hdlr_feature_phone ( void );


	////////////////////////////���ܻ�////////////////////////////

	//���ܻ�дIMEI���ܺ���
	DWORD func_imei_meta_hdlr_smart_phone_modem (WPARAM, LPARAM);
	//���ܻ�----META���Ӳ�������
	void Seg_MetaModemConnectReq ( void ) ;
	char Imei[4][16];
	//���ܻ�----METAmodemģʽ�����ֻ�
	bool EnterMetaModemMode ( void );
	int Connect_with_preloader(void);//�����Ӻ���
	bool search_preloader_port_success;
	int search_kernel_port(void );    //�����Ӻ���
	bool search_kernel_port_success;

	unsigned short kernel_port;

	//�л�ģʽ
	bool Switch2WM_META_Mode ( const int meta_handle );
	void Seg_MetaAPConnectReq ( void );

	/////////////////////////////////////////////
	//���ܻ�дBT��ַ����
	bool func_bt_address_meta_hdlr_smart_phone ( void );
	//д������ַ�����Ӻ���
	META_RESULT REQ_WriteBTAddr2NVRAM_Start (unsigned int iBTId, unsigned char* pBTAddress);
	META_RESULT REQ_Write2NVRAM(void);
	META_RESULT REQ_ReadFromNVRAM_AP( void );

	META_RESULT REQ_ReadBTAddrFromNVRAM_Start ( unsigned int iBTId, unsigned char* pBTAddress );

	//////////////////////////////////////////////////////////////////////////////////////////////////
	META_RESULT REQ_WriteAP_NVRAM_Start(WriteData_Type_e dataType, char *pInData, unsigned short iRID);//MT6592оƬд������ַ
	META_RESULT ConductBarcodeData(char *pOutData, unsigned short RID_para, char *pInDatabuf, int bufSize);
	META_RESULT ConductBTAddrData(char *pOutData, unsigned short RID_para, char *pInDatabuf, int bufSize,bool m_bWriteNvram);
	META_RESULT ConductWifiAddrData(char *pOutData, unsigned short RID_para, char *pInDatabuf, int bufSize,bool m_bWriteNvram);
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//���ܻ�Nvram��ʼ��
	bool APNvramDatabaseInit () ;
	bool IsAPNvramDatabaseInit ();
	//���ܻ�----METAAPģʽ�����ֻ�(д������ַ)
	bool EnterMetaAPMode ( void );
    //���ܻ����⴦��
	SetCleanBootFlag_REQ req;
	SetCleanBootFlag_CNF cnf;


	//���ܻ�дWIFI_MAC��ַ�ܺ���
	bool func_wifi_mac_meta_hdlr_smart_phone_Ex ( void );
	//дWIFI��ַ�����Ӻ���
	META_RESULT REQ_WriteWifiMAC2NVRAM_Start (WiFi_MacAddress_S *MACAddress);
	void Checksum_Generation(unsigned char StorageBuf[]);
	META_RESULT REQ_ReadWifiMACFromNVRAM_Start ( WiFi_MacAddress_S *MACAddress );
	//ApMetaת��Modem
	META_RESULT ApMetaToModemMeta ( void );
	META_RESULT ModemMetaToAPMeta (void);
	// MTK_META�����ļ�·�� Modem Database
	CString		m_strExchangeFileName;//�����ļ�����·��
	CString m_sPath;
	afx_msg void OnBnClickedButton6();
	// ƽ̨ѡ��չѶ��MTK���ܻ���MTK���ܻ�
	CComboBox Plat_Form;
	// Modem_Database�ļ�����ʹ��
	CButton Modem_Database_Control;
	// �ȴ�ʱ������ʹ��
	CEdit Wait_time_Control;
	// ���浱ǰ���������
	CButton SaveCurrentSetting_Control;

	//�ϴ�ORACLE���ݿ�
	bool Update_Oracle_Database();
	// ����Ļ������
	CEdit BARCODE_Input_Control;
	CString BARCODE_Input;
	afx_msg void OnEnChangeBtAddr3();
	afx_msg void OnBnClickedOk();
	// MTK���ܻ������ļ�
	CString m_sPath_AP;
	CString m_strExchangeFileName_Ap;
	afx_msg void OnBnClickedButton7();
	CButton AP_Database_Control;

	void func_four_in_one_hdlr_smart_phone ( void );

	bool   INITIAL_POWER;
	afx_msg void OnBnClickedButton8();
	CButton DB_Find_Control;
	afx_msg void OnBnClickedButtonstart10();

public:
	PARAMETER_MAIN paraArray[MAXPARANUM];
	// BLE�������̴߳�����ť
	CButton Sensor_Test_Thread;
	afx_msg void OnEnChangeNetcode();
	void Only_Update_Netcode();      //�����ϴ�����
	// ����ſ���
	CEdit NETCODE_Control;
	// ����ǰ׺
	CEdit NETCODE_PRE_Control;
	CString NETCODE_PRE;
	// ����ɨ������
	CString NETCODE_Input;
	// ����λ������
	CEdit NETCODE_Count_Control;
	int NETCODE_Count;
	CString NETCODE_Count_S;

	//Story
	void Story_Show();

	//��о4Gд��LOG��ʾ
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	// �����ӡ���������ýű�
	CString PrinterScript;
	CEdit PrinterScript_Control;
	afx_msg void OnBnClickedButtonstart11();
	// �ֶ�ѡ���ӡ��ֽ
	CButton Manul_Print_Control;
	afx_msg void OnBnClickedButtonstart13();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton33();
public:
	BOOL Permission();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEIMEIDLG_H__B59F17C9_CC97_4D41_B3AB_136C646D895A__INCLUDED_)
