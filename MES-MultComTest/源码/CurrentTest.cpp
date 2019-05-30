#include "stdafx.h"
#include "CurrentTest.h"
#include "Relay32.h"
#include "Keysight34461A.h"
#include "IT6400.h"
//#pragma comment(lib,"IT6400.lib")
//#pragma comment(lib,"Keysight34461A.lib")
//#pragma comment(lib,"Relay32.lib")

/*
*���ߣ���ά��
*���ܣ����Ե���ƽ��ֵ
*ʱ�䣺2019��2��18��16:06:09
����ֵ ƽ������ double��
������� ��
����1���ڼ�·��Ʒ
����2���Ǳ��ں�
����3���̵������ں�
����4�����Դ���
����5��У׼�����������Լ��飬�����һ�����ͨ���Ͳ��ٲ��ԣ���
����6���̵����պ���ʱ�೤ʱ�� ��ʼ���Ե���
����7������������
����8������������
����9���������Ե�����

*/


CurrentTest::CurrentTest()
{
}


CurrentTest::~CurrentTest()
{
}

bool CurrentTest::RelayChange(int proNum, static char InstrName[], static char RelayInstrName[], int testTimes, int  Delaytime)
{
	HINSTANCE bEzdDLL = ::LoadLibraryEx("Relay32.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	typedef void(*CMDSET)(char * COM, int32_t ReleyNum, LVBoolean on, LVBoolean* relay, LVBoolean* serial);
	CMDSET CmdSet = (CMDSET)GetProcAddress(bEzdDLL, "CmdSet");

	int32_t RelayNum1 = proNum;//�̵���1��
	int32_t RelayNum2 = proNum + 1;//�̵���2
	const  LVBoolean on = true;
	const  LVBoolean off = false;

	LVBoolean relay1 = false; //�̵���ָ��1
	LVBoolean* relay1_Pt = &relay1;
	LVBoolean serial1 = false;
	LVBoolean* serial1_Pt = &serial1;

	LVBoolean relay2 = false; //�̵���ָ��2
	LVBoolean* relay2_Pt = &relay2;
	LVBoolean serial2 = false;
	LVBoolean* serial2_Pt = &serial2;

	int count = 0;

	do
	{
		CmdSet(RelayInstrName, RelayNum2, on, relay2_Pt, serial2_Pt);//���Ե�Դ�̵����պ�

		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum1, on, relay1_Pt, serial1_Pt);//���õ�Դ�̵����Ͽ�

		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);


	Sleep(Delaytime); //��ά�򣺼̵������رպ�֮����΢�ӳ�һ����ԣ����˾�������̵����պϣ�������һ�㲨��

	do
	{
		CmdSet(RelayInstrName, RelayNum1, off, relay1_Pt, serial1_Pt);//��ά�� ���õ�Դ�̵����պ�
		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum2, off, relay2_Pt, serial2_Pt);//��ά�� ���Ե�Դ�̵����Ͽ�
		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	FreeLibrary(bEzdDLL);

	return true;
}

double CurrentTest::currentTest(int proNum, static char InstrName[], static char RelayInstrName[], int testTimes, int calibrationTimes, int  Delaytime, double currLowLimit, double curr_HightLimit, double currRange)
{
	HINSTANCE bEzdDLL = ::LoadLibraryEx("Relay32.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	typedef void(*CMDSET)(char * COM, int32_t ReleyNum, LVBoolean on, LVBoolean* relay, LVBoolean* serial);
	CMDSET CmdSet = (CMDSET)GetProcAddress(bEzdDLL, "CmdSet");

	double currAverage; //��ά�����յĲ��Խ��
	int32_t RelayNum1 = proNum;//�̵���1��
	int32_t RelayNum2 = proNum + 1;//�̵���2
	const  LVBoolean on = true;
	const  LVBoolean off = false;

	LVBoolean relay1 = false; //�̵���ָ��1
	LVBoolean* relay1_Pt = &relay1;
	LVBoolean serial1 = false;
	LVBoolean* serial1_Pt = &serial1;

	LVBoolean relay2 = false; //�̵���ָ��2
	LVBoolean* relay2_Pt = &relay2;
	LVBoolean serial2 = false;
	LVBoolean* serial2_Pt = &serial2;

	int count = 0;

	do
	{
		CmdSet(RelayInstrName, RelayNum2, on, relay2_Pt, serial2_Pt);//���Ե�Դ�̵����պ�

		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum1, on, relay1_Pt, serial1_Pt);//���õ�Դ�̵����Ͽ�

		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	Sleep(Delaytime); //��ά�򣺼̵������رպ�֮����΢�ӳ�һ����ԣ����˾�������̵����պϣ�������һ�㲨��

	for (unsigned int i = 0; i< calibrationTimes; i++) //У׼�Ĵ���
	{
		//��������
		currAverage = currentAverage(InstrName, testTimes, currRange); //��ά�򣺵���������
		if (currAverage >= currLowLimit && currAverage <= curr_HightLimit); //
		break;
	}

	do
	{
		CmdSet(RelayInstrName, RelayNum1, off, relay1_Pt, serial1_Pt);//��ά�� ���õ�Դ�̵����պ�
		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum2, off, relay2_Pt, serial2_Pt);//��ά�� ���Ե�Դ�̵����Ͽ�
		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	FreeLibrary(bEzdDLL);

	return  currAverage;
}


/**
���ߣ���ά��
���ܣ���ƽ��ֵ
ʱ�䣺2019��2��18��14:53:06
*/


double CurrentTest::average(vector<double>::const_iterator begin, vector<double>::const_iterator end, vector<double>::size_type elementsNum)
{
	double currAverage;
	double sum = 0;
	while (begin != end)
	{
		sum = sum + (*begin);
		begin++;
	}
	currAverage = sum / elementsNum;
	return currAverage;
}

/**
���ߣ���ά��
���ã����Ե���
ʱ�䣺2019��2��18��16:13:18
����ֵ������ƽ��ֵ
�������1�������Ĵ��ںţ�����GPIB��
�������2�����Դ���

*/


double CurrentTest::currentAverage(char InstrName[], int testNum, double currRange)
{
	HINSTANCE hEzdDLL = ::LoadLibraryEx("IT6400.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	typedef void(*DEFAULTSET)(char * InstrName, uintptr_t*VisaNameOut, LVBoolean * DefaSetValid);
	DEFAULTSET Defaultset = (DEFAULTSET)GetProcAddress(hEzdDLL, "Defaultset");

	typedef void(*CMEAS)(char * InstrName, double *CMeasValue, LVBoolean * CMeasValid);
	CMEAS CMeas = (CMEAS)GetProcAddress(hEzdDLL, "CMeas");

	HINSTANCE aEzdDLL = ::LoadLibraryEx("Keysight34461A.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	typedef int32_t(*CONFMEAS)(char * InstrName, double ManuaRange1, uintptr_t * VISAResourceNameOut);
	CONFMEAS ConfMeas = (CONFMEAS)GetProcAddress(aEzdDLL, "ConfMeas");

	typedef int32_t(*CONFMEASB)(char * InstrName, double ManuaRange1, uintptr_t * VISAResourceNameOut);
	CONFMEASB ConfMeasB = (CONFMEASB)GetProcAddress(aEzdDLL, "ConfMeasB");


	uintptr_t* VisaNameOut = 0;//���ں�
	LVBoolean DefaSetInit = false;
	LVBoolean * DefaSetValid = &DefaSetInit;//Ĭ�ϳ�ʼ��
	Defaultset(InstrName, VisaNameOut, DefaSetValid);

	double CInit = 0;
	double *CMeasValue = &CInit;
	LVBoolean CMeasInit = false;
	LVBoolean *CMeasValid = &CMeasInit;//��ŵ�������ֵ�ı���


	vector<double> cur_Arr; //��Ų��Ե�����ֵ������
	double curr_Average = 0;  //��ŵȴ�ģʽ�µ�����ƽ��

	ConfMeas(InstrName, currRange, VisaNameOut); //���Խ���֮ǰ�������̵�С

	ConfMeasB(InstrName, currRange, VisaNameOut);

	for (int i = 0; i<testNum; i++)
	{
		ConfMeas(InstrName, currRange, VisaNameOut); //���Խ���֮ǰ�������̵�С
		ConfMeasB(InstrName, currRange, VisaNameOut);
		CMeas(InstrName, CMeasValue, CMeasValid);
		ConfMeas(InstrName, currRange, VisaNameOut); //���Խ���֮ǰ�������̵�С
		ConfMeasB(InstrName, currRange, VisaNameOut);
		if (CMeasInit)
		{
			cur_Arr.push_back(CInit);
		}
	}
	ConfMeas(InstrName, 3, VisaNameOut); //���Խ���֮�󣬽����̵��󣬵���3A�Ĳ��Է�Χ�Ѿ��㹻��������
	ConfMeasB(InstrName, 3, VisaNameOut);
	curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());

	FreeLibrary(hEzdDLL);
	FreeLibrary(aEzdDLL);

	return curr_Average;
}


/**
���ߣ���ά��
���ܣ����õ�ѹֵ
ʱ�䣺2019��2��18��15:32:52
����ֵ��bool ���õ�ѹ�Ƿ���ȷ
�������1��InstrName[] �Ǳ��ںŻ���
�������2�� voltage Ԥ���ѹֵ

*/

bool CurrentTest::PowerOn(char InstrName[], double voltage)
{
	HINSTANCE hEzdDLL = ::LoadLibraryEx("IT6400.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	typedef void(*DEFAULTSET)(char * InstrName, uintptr_t*VisaNameOut, LVBoolean * DefaSetValid);
	DEFAULTSET Defaultset = (DEFAULTSET)GetProcAddress(hEzdDLL, "Defaultset");
	typedef void(*VLIMITCONF)(char * InstrName, double VLimit, LVBoolean EnVLimitSet, LVBoolean * VLimitSetValid);
	VLIMITCONF VLimitConf = (VLIMITCONF)GetProcAddress(hEzdDLL, "VLimitConf");
	typedef void(*VSET)(char * InstrName, float VSetValue, LVBoolean * VSetValid);
	VSET VSet = (VSET)GetProcAddress(hEzdDLL, "VSet");
	typedef void(*ENOUTPUT)(char * InstrName, LVBoolean EnOutput, LVBoolean * OutputSetValid);
	ENOUTPUT EnOutput = (ENOUTPUT)GetProcAddress(hEzdDLL, "EnOutput");
	//static char  = "GPIB0::23::INSTR";//�豸����PC�Ĵ�����
	uintptr_t* VisaNameOut = 0;//���ں�

	LVBoolean DefaSetInit = false;
	LVBoolean * DefaSetValid = &DefaSetInit;//Ĭ�ϳ�ʼ��
	LVBoolean OutputEnable;

	float VSetValue = voltage;
	LVBoolean VSetInit = false;
	LVBoolean *VsetValid = &VSetInit;//���õ�ѹ

	double VLimit = voltage*1.2; //��ά�򣺵�ѹ����1.2��
	LVBoolean EnVLimitSet = true;
	LVBoolean VLimitSetInit = false;
	LVBoolean *VLimitSetValid = &VLimitSetInit;//��ѹ����

	double VInit = 0;
	double *VMeasValue = &VInit;
	LVBoolean VoltMeasInit = false;
	LVBoolean *VMeasValid = &VoltMeasInit;//��ѹ����ֵ

	float CSetValue = 0.3;
	LVBoolean CSetInit = false;
	LVBoolean* CSetValid = &CSetInit;//����Ԥ��ֵ����

	LVBoolean EnableOutput = true;
	LVBoolean OutputInit = false;
	LVBoolean *OutputSetValid = &OutputInit;//�������
	OutputEnable = (bool)OutputInit;

	Defaultset(InstrName, VisaNameOut, DefaSetValid);
	VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
	VSet(InstrName, VSetValue, VsetValid);
	EnOutput(InstrName, EnableOutput, OutputSetValid);

	FreeLibrary(hEzdDLL);

	return OutputInit;
}




