#include "stdafx.h"
#include "ZebraPrinter.h"

#include <fstream>
using namespace std;

//����ת16�����ַ����ڴ�ӡ
#include "ConvertCHN.h"

CZebraPrinter::CZebraPrinter(void)
{
}

CZebraPrinter::~CZebraPrinter(void)
{
}

bool CZebraPrinter::OnPrintFunction(int high,int width,int x,int y,char* SSID,char* PassWord,char* MSN,char* IMEI,char* Font,int X_Pos[10],int Y_Pos[10],char* CH_Font) 
{
	/*
	CString m_csIMEI;//�����IMEI��
	//��ȡLABEL����
	CString	csLABEL="";
	CString	csStr;
	//LABEL��ʽ��ʼ
	csLABEL="^XA\r\n";
	csLABEL+="^LH020,010\r\n";
	//����֤����
	csLABEL+="^FO000,000^AFN,28,15^FDIMEI";

	csLABEL+=m_csIMEI;
	csLABEL+="^FS\r\n";
	//����֤����	
	csLABEL+="^FO000,040^B3N,N,50,N,N^FD";
	csLABEL+=m_csIMEI;
	csLABEL+="^FS\r\n";
	//LABEL��ʽ����
	csLABEL+="^XZ\r\n";
	//��ô�ӡ��ǩ��
	//m_ceLabels.GetWindowText(m_csLabels);
	int	labels=atoi("2");
	//����LABEL��ʽ�������ļ�LABEL.ZPL
	ofstream ofsLabel("LABEL.ZPL",ios::binary);
	for(int i=0;i<labels;i++)
		ofsLabel.write(csLABEL,csLABEL.GetLength());
	ofsLabel.close();
	//���������ļ�����ӡ��	
	system("type LABEL.ZPL>LPT1");
	*/
//ɨ����һ��
	//...
	//PrintContent(high,width,x,y,SSID,PassWord,MSN,IMEI,Font,X_Pos,Y_Pos,CH_Font);
	if( hPLPort != INVALID_HANDLE_VALUE) 
	{
		CloseHandle( hPLPort);
		hPLPort = INVALID_HANDLE_VALUE;
	}

	hPLPort = CreateFile( "LPT1", GENERIC_READ| GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if( hPLPort == INVALID_HANDLE_VALUE) 
	{
		AfxMessageBox("����: ���ڴ�ʧ��,�޷���ӡ����!");
		return false;
	}

	DWORD dwActWrite = 0;
	//int   MAX_BUFLEN = 40960;																				//NOT EXCEED THIS!��Ҫ����40960
	//BOOL r0=WriteFile( hPLPort, csBuffer.GetBuffer(MAX_BUFLEN), csBuffer.GetLength(), &dwActWrite, NULL);	//ͨ�����ڸ���ӡ����������
	CString csBuffer;
	csBuffer=PrintContent(high,width,x,y,SSID,PassWord,MSN,IMEI,Font,X_Pos,Y_Pos,CH_Font);					//Font:��ӡ��������λͼ���� A\B\D\E\F\G\H\GS\0  ʸ������P\Q\R\S\T\U\V
	BOOL Result=WriteFile( hPLPort, csBuffer, csBuffer.GetLength(), &dwActWrite, NULL);						//ͨ�����ڸ���ӡ����������
	if((Result==FALSE)||(csBuffer.GetLength()!=dwActWrite))
	{
		AfxMessageBox("��ӡʧ�ܣ�");
		return false;
	}
	csBuffer.ReleaseBuffer();
	if( hPLPort != INVALID_HANDLE_VALUE) 
	{
		CloseHandle( hPLPort);
		hPLPort = INVALID_HANDLE_VALUE;
	}
	return true;
}

CString CZebraPrinter::PrintChinese(char *strSource,char* pCHN_ID,char* CH_Font,int high,int width,int x,int y) //CH_FontΪ���� �硰���塱
{
	/*
	������ǩʾ����  
	^XA								����ǩ��ʽ��ʼ����   
	^LH30,30						��Label Home position���ñ�ǩ��ʼ�ο�������Ϊ30,30��Ĭ��Ϊ 0,0    200dpi������8��=1mm  300dpi������12��=1mm  
	^FO20,10^AD^FDEXAMPLE^FS		��^FO20,10-������������ʼ���꣬��� ����20,10   ^AD-�趨����ΪD���ڲ����壩 ^FD-������ʼ����   EXAMPLEʵ�ʴ�ӡ����  ^FS-������ָ�����   
	^FO20,60^B3^FDAAA001^FS			��^B3-ѡ����������Ϊ39��   
	^XZ								����ǩ��ʽ��������
	*/


	/*
	^LTx 
	��ǩ�������£�Y�ᣩλ�������ƶ�	  
	x���ƶ�����ֵ����Χ��-120 - +120��
	  
	^LSa 
	��ǩ�������ң�X�ᣩλ�������ƶ�  
	a���ƶ�����ֵ����Χ��-9999 - +9999��

	^Af,o,h,w ��ӡ���ָ�ʽ���� 
	f�����壬Ĭ��Ϊ0��ѡ��Χ��A-Z,0-9����Ϊ��ӡ���ڲ����壬��TrueType���壩  
	o�����ַ���N=��ͨ��R=��ת90�ȣ�I=��ת180�ȣ�  B=��ת270��  
	h���ַ��߶ȣ���Χ��10-1500�� 

	^BYw,r,h �趨����Ĭ�ϲ���  
	w������ȱʡΪ2�㣬ѡ��Χ��1-10��  
	r�����ձ��ʣ�ȱʡΪ3.0��ѡ��Χ��2.0-3.0
	h������߶� 

	^B3o,e,h,f,g  ��ӡ39�� 
	o�����뷽��  
	e��Mod-43У��λ��N=No��Ĭ�ϣ���Y=Yes 
	h������߶ȣ���Χ��1-9999��  
	f���Ƿ��ӡ�ַ��У�Y=Yes��Ĭ�ϣ���N=No 
	g���ַ���λ�ã�N�������·���Y�������Ϸ� 

	^BCo,h,f,g,e,m  ��ӡ128��  
	e��UCCУ��λ��N=No��Ĭ�ϣ���Y=Yes  
	m��ģʽ��һ����ΪA���Զ�ѡ�񣩻���Բ��� 

	^PRp ��ӡ����  
	p���ٶȡ���2-8����A-E��=2Ӣ��/��-8Ӣ��/�� 

	^PQq ��ӡ����  q����������ѡ��Χ��1-99999999 

	^GBw,h,t  ��ӡ����  w�������ȣ�h�������ȣ�t���߿� 


	XG ����ͼ��
	^XGָ���ʽ
	^XG < Sve: > Objectname < .ext >, x, y
	^XG=����ͼ��

	Sve: = ����ͼ��ԭװ��
	���ӣ�ȱʡ���������ȵ�
	Objectname = ����ͼ������1-8���ַ� ȱʡ�ÓUNKNOWN��?/P>
	.ext = ��չ����3���ַ� �̶��ģ�ʼ����.GRF
	x = x����Ŵ����ӡ�
	ȱʡֵ��1
	��Сֵ��1�����ֵ��10

	y = y����Ŵ�����
	ȱʡֵ��1
	��Сֵ��1�����ֵ10

	��������^XGָ�����ͼ���DRAM��SAMPLE.GRFȻ���ӡ5����ͬ�ط���5��ͬ�ߴ���ͬһ��ǩ�ϵ����ӡ�
	�ƶ�ͼ��
	^IM���ƶ�ͼ��ָ��ִ��һ������ͼ��ֱ�Ӵ�����ԭ���ƶ�����ָ��ͬ������ͼ��ָ���û�ߴ������
	^IMָ���ʽ��
	^IM < Svc: > Objectname < .ext >
	^IM = �ƶ���������

	Svc: = ���ƶ�ͼ���Դװ��
	���ӵģ�ȱʡʱ���������ȵ�
	Objecname = ����ͼ������1-8�ַ���ȱʡ���ÓUNKNOWN����?/P>
	.ext = ��չ����3�ַ� �̶���ʼ����.GRF
	ע1����^FOָ����ҿ��Զ�λ�ڱ�ǩ�κ�λ�á�
	ע2����^IM��^XGָ����������ͼ���ƶ�ָ��û�зŴ����������ʽʱ����١�����һ������ͼ���������8��16��32


	*/
	CString DesData;																//DesData ������ȡ��   ��ӡ���ĵ�ZPLָ��
	CConvertCHN chn;
	//char *strSource = "����";														//��Ҫת���ĺ��֣�������ֹ��࣬��������������鳤��
	char strEnd[2048]="\0";															//ת������ַ��������ַ������͸���ӡ��
	//memset(strEnd,0,2048);
	char *strError ="";

	if(!chn.autoFormatCHNCode(strSource,strEnd,pCHN_ID,CH_Font,strError,high,width))	//����2Ϊת�����������   ����������Ϊת���󱣴�����ƣ���ӡ��ʱ���õ�����������8λ�ַ� "OUTSTR01"
	{
		return "ERROR";
	}
	DesData.Format("%s",strEnd);
	DesData.Format(DesData+"^FO%d,%d^XG%s,1,1^FS",x,y-5,pCHN_ID);//-5����һ��
	return DesData;
	
	/*
	~DG ����ͼ��
����~DG������ͼ��ִ�����¹��ܡ�
	1.	�ô�ӡ��Ϊͼ��ģʽ��
	2.	����ͼ�Ρ���������ֽ������ڱ�ǩ�е��ã�
	3.	����ͼ��ߴ�
	4.	����ʮ�������ַ�������ӡ��
	ע�⣺
	1.	Ϊʹ��ӡ�����ף�ʹ��ͼ����ʱ���Կո񡢾�㡢����չ������
	2.	����żȻ���ո��滻����Ҫ��ͼ�������ÿո񡣲�ͬͼ���ò�ͬ���֡�
	3.	�����ͼ������ͬһ���͵���ӡ������һ��ͼ��ɾ�����ɵڶ���ͼ����档
	��DGָ���ʽ
����	~DGd:o.x,t,w,DATA
	~DG	=	���ô�ӡ��Ϊ����ͼ��ģʽ
	d	=	����ͼ���Ŀ���豸��
 	 		ȱʡֵ��R: (DRAM)
	����ֵ��B: (�ڴ�ѡ��)
	o	=	ͼ����,1-8�ַ�,
 	 		��ȱʡʱ����UNKNOWN��������)
	x	=	��չ����3�ַ� �� �̶��ģ�ʼ���ǣ�GRF��
	t	=	ͼ���ܵ��ֽ���
	w	=	ÿ���ֽ���
	DATA	=	ASCIIʮ�����ƴ�ͼ����
����	��Ŀ����ʡ�ԣ�����UNKNOWN.GRF��Ϊ��ͼ���������ݴ�ʹ��ASCIIʮ�����ƴ�ͼ���壬ÿ���ַ���ʾˮƽ������ĸ��㡣
����	������һ���á�DGָ�����ͼ��DRAM�����ӡ�����ͼ������SAMPLE.GRF��
	~DGR:SAMPLE.GRF,00080,010,
	FFFFFFFFFFFFFFFFFFFF
	8000FFFF0000FFFF0001
	8000FFFF0000FFFF0001
	8000FFFF0000FFFF0001
	FFFF0000FFFF0000FFFF
	FFFF0000FFFF0000FFFF
	FFFF0000FFFF0000FFFF
	FFFFFFFFFFFFFFFFFFFF
	����t��ͼ�����ֽ����������¹�ʽ���㣺
����	X (���ף�����ӡ���ֱ��ʣ���/���ף� ��  Y�����ף� ����ӡ���ֱ棨��/���ף�/8����/�ֽڣ�=   ���ֽ�
����	X �ǵ�λ���׵�ͼ���ȡ�Y�ǵ�λ���׵�ͼ��߶ȡ���/���״�ӡ����̵Ĵ�ӡ�ֱ��ʡ�
����	���磬ȷ��ͼ��8���׿�16���׸ߣ���ӡ�ֱ���8��/���׵���ȷt�����乫ʽ�ǣ�
����	8��8��16��8/8=1024�ֽ�
	��
	����w��ÿ���ֽ����������¹�ʽ���㣺
����	X (���ף�����ӡ���ֱ��ʣ���/���ף� /8����/�ֽڣ�=   ÿ���ֽ���
����	x�ǵ�λ���׵�ͼ���䣬��/�����Ǵ�ӡ��ƫ�ƴ�ӡ�ֱ��ʡ�
����	���磬ȷ��ͼ��8���׿���ӡ�ֱ���8��/���׵���ȷw�������乫ʽ�ǣ�
��������	8��8/8 = 8�ֽ�
	ע�⣺
	1.	�����ֽ���һ�е��ֽ�
	2.	w��t��������ĵ�һ��ֵ
����	����<DATA>��һ��ʮ����������Ϊͼ���ʾ�ʹ�ӡ����ÿһʮ�������ַ�����ˮƽ�����ĸ��㡣��ͼ��ǰ�ĸ����ǰ׵ģ����ĸ����Ǻڵġ���������ĵ�00001111��ʮ�����Ʊ�ʾ������ֵ����OF��������ͼ�����������������ͼ���ʹ�ӡ����һ������ʮ����ֵ��
����	�ԡ�DG�͡�DBָ��ѡ������ѹ��������
����	�����ӡ����һ��κ����ѹ���������ڡ�DG�͡�DBָ�����ⷽ����������ͼ��͵�������ʵ�������ֽ������ܵ�ʱ�䡣
����	������1��2��3��4��5������19�����ں���ʮ������ֵ��ע��ֵ��G��ʼO��O��A��F�Ѿ���ʮ������ֵ��ʹ�ã�
	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V	W	X	Y
	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19
����	���磬��M6����ӡ����ʾ����ʮ���������ݡ�
��������	6666666
����	M��ֵ��7�����Դ���7��ʮ�����Ƶ�6
	g	h	i	j	k	l	m	n	o	p	q	r	s	t	u	v	w	x	y	z
	20	40	60	80	100	120	140	160	180	200	220	240	260	280	300	320	340	360	380	400
����	���磺��-hB����ӡ����ʾ��ʮ���������ݡ���
	BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
	40
����	hֵ��40�����Դ���40 ��ʮ�����Ƶ�B��
	�ظ�ֵ
����	�����ظ�ֵһ��ʹ�õõ��κ���Ҫֵ����vMB���͡�MvB��һ�������Ǵ���327��ʮ�����Ƶ�B����ӡ����
	�Ѷ��ţ���������У���ʾ�����ұߣ�����0��
	��̾�ţ���������У���ʾ�����ұ߶���1��
	ð�� ��������ʾ�ظ���һ�С�
����	����Ͳ���t�ֽ�����ӡ�󣬴�ӡ������ԭ���Ĵ�ӡģʽ��
	��������ͼ��ʱ��
����	���á�DGָ��ʱ���ⷽ������ʵ���ʹ�ӡ���������ֽ�����������ͼ6.1��6.2�ɿ�����
����	��ͼ6.2�����ʮ�����ƴ����ǵ��㣨0��������һ�����ţ���������������е��㡣���ʮ�����ƴ���������0���������һ����Ͷ��š���̾�ţ�������ð�ţ��������÷���ǰ��
����	���棺����༭���ı��н���ͬ��ZPLII����С��༭�����ְѵ��а���������ZPLII�����ı��н������磺�س��ͻ����ַ�����
����	ע����ͼ6.1��6.2�س��Ѿ�������ÿ�����еĽ�����
	��
	 
	��
	~DN ȡ������ͼ��
����	^DG�ڽ���ʹ�ӡ�����t���ֽں󣬴�ӡ������һ��ģʽ���á�DN��ȡ������ͼ��ָ�ͼ��ģʽ����ȡ��ԭ����ӡ�����ָ���
	��DNָ���ʽ
����	��DN
	��DN��ȡ������ͼ��
	ע�⣺�κ�^��ָ��������ء�

	��ʵ�����ǽ���strSource����ת����ͼ����ת����ʮ�����Ʊ���������ZPL��ѹ�������á�~DG��������뷢�͸���ӡ��������á�^XG���������ͼ���ӡ�����������~DGOUTSTR01��������ǽ�ͼ����뷢�͸���ӡ���������ڴ�ӡ���洢����ͬʱ�����ļ�������ΪOUTSTR01������������������ָ��ӡͼ������ֽں�ÿ�е��ֽ��������Ķ�������ͼ����롣
	*/

}

CString CZebraPrinter::PrintContent(int high,int width,int x,int y,char* SSID,char* PassWord,char* MSN,char* IMEI,char* Font,int X_Pos[10],int Y_Pos[10],char* CH_Font)					//��ϴ�ӡ��ZPL2 ����
{
	CString tmp;

//[0]--������ ֵƫ����  [1]--"WIFI���룺" �ַ�ƫ����  [2]--WIFI���� ֵƫ����  [3]--MSN ֵƫ����   [4]--IMEI  ֵƫ����  [5]--IMEI����ƫ����
//[0]--MSN Y��ƫ����   [1]--IMEI Y��ƫ����  [2]--IMEI���� Y��ƫ���� 

	tmp="^XA^IA6^XZ^FS^XA^FS^LH0,0^FS";
	tmp+="^FO";
	tmp.Format(tmp+"%d,%d^MD",x,y);																					//^MD����ǩ��ȣ�ָ������뵱ǰ�趨��ȵĹ�ϵ����Сֵ -30�����30��������������ȷָ��������
	//tmp+=PrintChinese("��������","OUTSTR01",CH_Font,high,width,x,y);
	tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[0],y,Font,high,width,SSID);								//^FO%d,%d^-������������ʼ���꣬��� ����20,10   ^AD-�趨����ΪD���ڲ����壩 ^FD-������ʼ����   EXAMPLEʵ�ʴ�ӡ����  ^FS-������ָ�����   
	
	//tmp+=PrintChinese("WIFI���룺","OUTSTR01",CH_Font,high,width,x+X_Pos[1],y);
	tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x,y+Y_Pos[1],Font,high,width,PassWord);								   
	
	//tmp+=PrintChinese("S/N��","OUTSTR01",CH_Font,high,width,x,y+Y_Pos[0]);
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[3],y+Y_Pos[0],Font,high,width,MSN);								   
	
	//tmp+=PrintChinese("IMEI��","OUTSTR01",CH_Font,high,width,x,y+Y_Pos[1]);
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[4],y+Y_Pos[1],Font,high,width,IMEI);			
	//tmp+=PrintChinese("IMEI��","OUTSTR01",CH_Font,high,width,x+Y_Pos[1],y+Y_Pos[0]);								//X_Pos[1] ��SM��IMEI����ͬ��
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^FD%s^FS",x+X_Pos[4],y+Y_Pos[0],Font,high,width,IMEI);		
	/////////////////////////////////////////////////////////////////////////////////
	//tmp.Format(tmp+"^FO%d,%d^A%sN,%d,%d^BCN,65,N,N^FD%s^FS",x+X_Pos[5],y+Y_Pos[2],Font,high,width,IMEI);	

	tmp+="^PQ1^FS";
	tmp+="^PRA^FS^XZ^FS^XA^EG^XZ"; //��ӡ����  


	return tmp;
	/*
	CString csBuffer;
	
	csBuffer +=		"^XA";															//^XA��ǩ��ʽ��ʼ����   ^LH30,30   ��Label Home position���ñ�ǩ��ʼ�ο�������Ϊ30,30��Ĭ��Ϊ 0,0 
	csBuffer +=		"^MMT";															//^MM����ӡģʽ��?���ô�ӡ�������ֻ���ģʽ֮һ��˺�ϣ����ߣ�������е���  ˺��
	csBuffer +=		"^LL0160";														//LL ��ǩ����
	csBuffer +=		"^LS0";															//���ƫ��
	csBuffer +=		"^FT0,10^XGOUTSTR01,1,1^FS";									//FT�����LHƫ�ƣ���ӡ���֣�OUTSTR01��Ϊת������ַ���  
	csBuffer +=		"^FO20,80^B3N,N,100,Y^FD20120914^FS";							//��ӡ����
	csBuffer +=		"^FD20120914^FS";
	csBuffer +=		"^XZ";
		
	^MTָ���ʽ
	^MT x
	^MT = ֽ����
	x = ����ֽ
	T = ��תӡֽ
	D = ����ֽ

	*/
	
	/*
	^PR����ӡ�ٶȣ�ָ�������ǩ��ӡ�ٶȺͻ�ת�ٶȣ���һ�ձ�ǩ��
	^PRP,S
	^PR=��ӡ�ٶ�
	P=��ӡ�ٶ�
	ȱʡֵ���ٶ�A
	���ֵ��A��2 50.8����/�루2Ӣ��/�룩
	B��3 76.2����/�루2Ӣ��/�룩
	C��4 101.6����/�루2Ӣ��/�룩
	5 127����/�루2Ӣ��/�룩
	D��6 152.4����/�루2Ӣ��/�룩
	E��8 203.2����/�루2Ӣ��/�룩

	S=��ת�ٶ�
	ȱʡֵ���ٶ�D
	���ֵ��A��2 50.8����/�루2Ӣ��/�룩
	B��3 76.2����/�루2Ӣ��/�룩
	C��4 101.6����/�루2Ӣ��/�룩
	5 127����/�루2Ӣ��/�룩
	D��6 152.4����/�루2Ӣ��/�룩
	E��8 203.2����/�루2Ӣ��/�룩
	��ӡ��ѡ���ٶȲ���ֱ���Ժ��������ʽ���ӡ���ػ���
	��ӡ���ٶ�Ӧ�þ�������ӡ����ȡ���ڱ�ǩ��ɫ������ӡ�ٶȺʹ�ӡ����ģʽ�����в������Ӧ���Ƿǳ���Ҫ�ġ�

	^EG��~EG��ɾ������ͼ��ָ�������ڴ�DRAM��ɾ������ͼ�󣨱�ǩ��ʽͼ���ʮ������ͼ��
	��EG��^EGָ���ʽ��
	��EG��^EG

	^MM����ӡģʽ��ָ����ȷ��һ�Ż�һ���ǩ�ѱ���ӡ���ӡ���Ķ����������ֲ�ͬ����ģʽ��
	Tear Off?��ӡ�󣬱�ǩ��������˺���ϣ���ǩ���ŵ�ֽ����������˺�¡�
	Rewind?  ��ǩ�͵�ֽ��������һ������������ӻ���װ�ã���һ�ű�ǩ��λ�ڴ�ӡͷ�£��޻��Ͷ�������
	Peel Off?��ӡ�󣬲��ֱ�ǩ�ӵ�ֽ�ϱ����롣��ӡֱֹͣ����ǩ��ȡ�ߡ���ֽ������ֻ�Ƶ�ֽ�Ļ������ϣ�ע��ѡ�û�������װ�ڴ�ӡ���ڣ���
	Cutter?���ſ�����ӡ�ĺ���һ��ǩ���ͽ���ֽ������ǩ���¡���ǩ���ػؽ���ӡ��������
	^MMָ���ʽ
	^MM=��ӡģʽ
	x = Ҫ��ģʽ
	T = ˺��
	R = ����
	P = ����
	C = �е�
	A = ����
	������������ȷָ��������

	*/
}