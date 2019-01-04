#include "StdAfx.h"
#include "PictureProcess.h"
#include "BasicUse.h"

CPictureProcess::CPictureProcess(void)
//: m_GdiplusToken(0)
{
	//GDI+ͼ���ĳ�ʼ��
	//GdiplusStartup(&m_GdiplusToken,&m_GdiplusStartupInput,NULL);
}

CPictureProcess::~CPictureProcess(void)
{
	//GdiplusShutdown(m_GdiplusToken);
}


//ͼƬ��������
//BufΪ�����ͼ��byte
void CPictureProcess::AddTextToBmp(CString sFile, CWnd* pWnd, BYTE *Buf, 
								   int Width, int Height,
								   CString sInfo)
{

	if(Buf==NULL)	//��δ����ռ��С�򷵻�
		return ;
	
	if(sInfo == "")		//��Ҫ���ӵĴ�Ϊ""���򲻵���
		return ;

	//???
	int nWidthBytes = Width * 3;
	while( (nWidthBytes & 3)!=0)
		nWidthBytes ++;

	//����ԭ�л���
	CClientDC client(pWnd);
	CPen Pen(PS_SOLID,1,RGB(0,255,255));
	CPen * pOldPen=(CPen *)client.SelectObject(&Pen);
	CBrush Brush(RGB(0,255,255));
	CBrush * pOldBrush=(CBrush *)client.SelectObject(&Brush);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&client);

	CBitmap MemB;
	short Col=16,Row=27;	//���������С һ���ֽڵĿ���
	long length = sInfo.GetLength() ;
	long Len = Col*length;
	if(Len < 130)
		Len =130;
	MemB.CreateCompatibleBitmap(&client,Len,Row);	//����һ�����������Ա����
	CBitmap * pOld=MemDC.SelectObject(&MemB);
	MemDC.PatBlt(0,0,Col*length,Row,WHITENESS);
	MemDC.SetTextColor(RGB(255,0,0));	//������ɫ,��ɫ

	CFont OutFont;
	LOGFONT temlf;
	//temlf.lfHeight=-14; 
	temlf.lfHeight=16;	//�ִ�С
	temlf.lfWidth=0; 
	temlf.lfEscapement=0; 
	temlf.lfOrientation=0; 
	temlf.lfWeight=400; 
	temlf.lfItalic=0; 
	temlf.lfUnderline=0; 
	temlf.lfStrikeOut=0; 
	temlf.lfCharSet=134; 
	temlf.lfOutPrecision=3; 
	temlf.lfClipPrecision=2; 
	temlf.lfQuality=1; 
	temlf.lfPitchAndFamily=2;  
	OutFont.CreateFontIndirect (&temlf);

	CFont* def_font = MemDC.SelectObject(&OutFont);
	MemDC.TextOut(0,0,sInfo);		//������Ϣ����
	MemDC.SelectObject(def_font);

	COLORREF color;
	BYTE Red,Green,Blue;
	short i,j;
	//ѭ���õ�����
	for(j=Col*length-1;j>=0;j--)
	{
		for(i=0;i<Row;i++)
		{
			color = MemDC.GetPixel(j,i);
			if(color == RGB(0,0,0))		//����ɫʱ
			{
				i =-20;		//??
				break;
			}
		}
		if(i<0)
			break;
	}

	if(i<0)
	{
		if((j+2)<Col*length)
			Len = j+2;
		else
			Len = j;
	}

	long Offset1;
	int ii,jj,kk,ll;
	float Scale = 1.0;		//1.5;

	if(sInfo.GetLength()>58)
		Scale = (float)0.8;
	else
		Scale = (float)1.0;

	for(i=0;i<Row;i++)
	{
		for(j=0;j<Len;j++)
		{
			color = MemDC.GetPixel(j,i);	//����
			Blue = GetBValue(color);
			Green = GetGValue(color);
			Red = GetRValue(color);

			//��ɫ������
			if(Blue == 255 && Green == 255 && Red ==255)
				continue;
							
			kk = (int)(Scale*(i+1) +0.5);

			for(ii = (int)(Scale*i); ii<kk ; ii++)
			{
				ll = (int)(Scale*(j+1)+0.5);
				for(jj = int(Scale*j) ; jj<ll; jj++)
				{
					Offset1 = (Height-10 -ii+1) * nWidthBytes + jj * 3;
					Buf[Offset1] = Blue;
					Buf[Offset1+1] = Green;
					Buf[Offset1+2] = Red;
				}
			}
		}
	}

	MemDC.SelectObject(pOld);

	def_font->DeleteObject();
	pOldPen->DeleteObject();
	pOldBrush->DeleteObject();

}

//��ʾͼƬ����ť��:�ļ���ʽbmp
void CPictureProcess::ShowPicture(CString strPicName,HWND hwndStill)
{
	////////�������bmp��ʽ�Ĳ���ʾ//////////////
	int nFind = -1;
	CString strBMPStr = strPicName;
	strBMPStr.MakeUpper();
	nFind = strBMPStr.Find("BMP");
	if(nFind <= 0)
		return;
	//////////////////////////////////////////

	CBasicUse bUse;
	if (bUse.FileExists(strPicName) == FALSE){
		return;
	}

	CString LastDisplayFile = strPicName;

	BITMAPINFO bitmapinfo = GetBmpInfo(LastDisplayFile);
	if(bitmapinfo.bmiHeader.biWidth <= 0)
		return;

	long Width,Height;
	Width = bitmapinfo.bmiHeader.biWidth;
	Height = bitmapinfo.bmiHeader.biHeight;
	BYTE *Buf;
	Buf = new  BYTE[(long)(Height*Width*3)]; 
	GetBmpBuffer(LastDisplayFile,Buf);

	ShowPictureByByte(Buf,hwndStill,Width,Height);

	if(Buf != NULL){
		delete Buf;
	}

}

//��ʾͼƬ:��-stop
void CPictureProcess::ShowPicture(BYTE *bytePicName,HWND hwndStill){

	BYTE *Buf;

	Buf = bytePicName;

	int Width = 100;

	int Height = 100;
	
	ShowPictureByByte(Buf,hwndStill,Width,Height);

	if(Buf != NULL){
		delete Buf;
	}

}
//ͼƬ�ļ���������ʾ���ؼ��ϣ��������õ��ǰ�ť�ؼ���
void CPictureProcess::ShowPictureByByte(BYTE *Buf,HWND hwndStill,int Width,int Height)
{
	/////////��ʾͼƬ:hwndStillΪ�������ľ��////////////////////////
	RECT rc;
	::GetWindowRect( hwndStill, &rc );
	long lStillWidth = rc.right - rc.left;
	long lStillHeight = rc.bottom - rc.top;

 	//��ʾʶ�������ͼƬ
	BITMAPINFOHEADER bih;
	memset( &bih, 0, sizeof( bih ) );
	bih.biSize = sizeof( bih );
	bih.biWidth = Width;
	bih.biHeight = Height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;

	HDC hdcStill = ::GetDC( hwndStill );
	PAINTSTRUCT ps;
	::BeginPaint(hwndStill, &ps);
	int Prev = SetStretchBltMode(hdcStill, COLORONCOLOR);
	StretchDIBits( 
		hdcStill, 0, 0, 
		lStillWidth, lStillHeight, 
		0, 0, Width, Height, 
		Buf, 
		(BITMAPINFO*) &bih, 
		DIB_RGB_COLORS, 
		SRCCOPY );
	::EndPaint(hwndStill, &ps);
	::ReleaseDC( hwndStill, hdcStill ); 

	/*
	if(Buf != NULL){
		Buf = NULL;
		delete Buf;
	}
	*/

}

//jpgͼƬ�ļ�ת����BLOB���������Ա����ݿ���
VARIANT CPictureProcess::ChangePicFileToBYTE(CString strPicName){

	CFile imagefile;
	VARIANT	varBLOB;
		
	try{
		//
		if(imagefile.Open(strPicName,CFile::modeRead)){

			int nSize = imagefile.GetLength();          //�ȵõ�jpg�ļ�����

			BYTE * pBuffer = new BYTE [nSize];  //���ļ��Ĵ�С�ڶ�������һ���ڴ�
		 
			if (imagefile.Read(pBuffer, nSize) > 0 )    //��jpg�ļ�����pBuffer(��������һ���ڴ�)
			{   // +----------------------------------------------
				BYTE *pBuf = pBuffer;     ///������һ����ǰ�pBuffer���jpg���ݷŵ�����
				
				SAFEARRAY		*psa;
				SAFEARRAYBOUND	rgsabound[1];
		
				if(pBuf)
				{    
					rgsabound[0].lLbound = 0;
					rgsabound[0].cElements = nSize;
					psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
					for (long i = 0; i < (long)nSize; i++)
						SafeArrayPutElement (psa, &i, pBuf++);

					varBLOB.vt = VT_ARRAY | VT_UI1;
					varBLOB.parray = psa;
				}
							
				//�ͷ�
				if(pBuf != NULL)
					delete pBuf;
				
				SafeArrayDestroy(psa);	//�ͷ�

			}

			imagefile.Close();	

			if(pBuffer != NULL)
				delete pBuffer;
			
		}
		
		
	}catch(CException * e){
		CString error;
		error.Format("����!\r\n������Ϣ��%s",e->GetErrorMessage("ָ��",255));
	}

	return varBLOB;
}

//���ݿ�ͼ��ת����bmp�ļ�
BOOL CPictureProcess::DBStreamToBmpFile(VARIANT vDBStream,long lngStreamLength,CString strPicFile){

	BOOL ifSuccess = FALSE;

	try{
		
		if(BYTE *pBuffer = new BYTE [lngStreamLength+1])		///���������Ҫ�Ĵ洢�ռ�
		{	
			char *pBuf = NULL;

			SafeArrayAccessData(vDBStream.parray,(void **)&pBuf);

			memcpy(pBuffer,pBuf,lngStreamLength);				///�������ݵ�������m_pBMPBuffer

			SafeArrayUnaccessData (vDBStream.parray);
	
			CFile outFile(strPicFile,CFile::modeCreate|CFile::modeWrite);

			LPCTSTR buffer = (LPCTSTR)GlobalLock((HGLOBAL)pBuffer);

			outFile.Write(buffer,lngStreamLength);

			GlobalUnlock((HGLOBAL)pBuf);

			outFile.Close();

			SafeArrayUnaccessData (vDBStream.parray);
	
			if(pBuffer != NULL)
				delete pBuffer;

			pBuf=0;

			//��Ҫ��ʱ����ֹ�ļ������ٶ���


		}
		
		ifSuccess = TRUE;
	
	}catch(CException * e){
		CString error;
		error.Format("����!\r\n������Ϣ��%s",e->GetErrorMessage("ָ��",255));
		return FALSE;
	}

	return ifSuccess;

}

//�õ�bmpͼƬ�Ļ�����Ϣ�����ߣ�bits
//����ֵΪָ�����飬������ϢΪ����,bits��num ��λ[4]
BITMAPINFO CPictureProcess::GetBmpInfo(LPCTSTR lpszBmpFile)
{
	BITMAPINFO BitmapInfo;
	
	CString sFilePath = "";
	sFilePath = lpszBmpFile;

	if(sFilePath == "")
		return BitmapInfo;

	FILE *fp;

	//�ж��ļ��Ƿ���ڵ���һ�ַ�ʽ���������Ƿ������ݣ�
	fp = fopen(lpszBmpFile,"r");
	if(fp==NULL)
	{
		return BitmapInfo;
	}
	
	long Offset,Offset1;
	Offset = sizeof(BITMAPFILEHEADER);

	Offset1 = sizeof(BITMAPINFO);
	Offset1 = 40;
	
	fseek(fp,Offset,SEEK_SET);
	fread(&BitmapInfo,Offset1,1,fp);
	
	fclose(fp);
		
	return BitmapInfo;
}

//�õ�bmp������ǰ����newbuffer������ռ�Ĵ�С����lpszBmpFile��ȡ���ݵĴ�С
void CPictureProcess::GetBmpBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer)
{
	CString sFilePath = "";
	sFilePath = lpszBmpFile;

	if(sFilePath == "")
		return;

	FILE *fp;

	//�ж��ļ��Ƿ���ڵ���һ�ַ�ʽ���������Ƿ������ݣ�
	fp = fopen(lpszBmpFile,"r");
	if(fp==NULL)
	{
		return;
	}
	
	long Offset,Offset1;
	Offset = sizeof(BITMAPFILEHEADER);

	Offset1 = sizeof(BITMAPINFO);
	Offset1 = 40;
	BITMAPINFO BitmapInfo;
	fseek(fp,Offset,SEEK_SET);
	fread(&BitmapInfo,Offset1,1,fp);

	int Height = BitmapInfo.bmiHeader.biHeight;
	int Width = BitmapInfo.bmiHeader.biWidth;
	int nBits = BitmapInfo.bmiHeader.biBitCount;
	
	Offset = Offset+Offset1;
	fseek(fp,Offset,SEEK_SET);
	fread(newbuffer,(long)(Height*Width*nBits/8),1,fp);
	fclose(fp);
	
}

//�õ�bmpͼƬ��ת������������Ա㱱�����ӳ���ʶ����...
//���ڲ������ӣ���������õ��Ƕ�̬����÷�ʽ���򻯳��� 
void CPictureProcess::GetBmpVerBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer, CString dllName)
{
	CString sFilePath = "";
	sFilePath = lpszBmpFile;

	if(sFilePath == "")
		return;
	
	///////////////�õ�������///////////////////////////////////
	BOOL bGet = FALSE;
	CBasicUse bUse;
	if (bUse.FileExists(dllName) == FALSE)
		return;
	
	//�õ�ͼƬ�ķ�ת������Ȼ����ʶ���޷����С������ĳ���ʶ���Ǵ�ͷ��ʼ��
	HINSTANCE HDLL_picProcess;	//��̬���ӿ���غ�ľ��jpg->bmp
	HDLL_picProcess = LoadLibrary(dllName);

	typedef BOOL(*pGetBmpVerBuffer)(LPCTSTR lpszBmpFile,LPBYTE newbuffer); 
	pGetBmpVerBuffer GetBmpVerBuffer;
	GetBmpVerBuffer=(pGetBmpVerBuffer)GetProcAddress(HDLL_picProcess,"GetBmpVerBuffer");
	if(GetBmpVerBuffer != NULL){
		bGet = GetBmpVerBuffer(sFilePath,newbuffer);
	}
	
	//ж��dll�ļ�
	FreeLibrary(HDLL_picProcess);
	///////////////////////////////////////////////////////
	
}

//��������Ϊbmp
//lpszFile Ŀ���ļ�bmp; newbufferΪ���е�������
BOOL CPictureProcess::SaveImage(LPCTSTR lpszFile, LPBYTE newbuffer, 
								long lWidth,long lHeight )
{

	if(newbuffer == NULL)
		return FALSE;

	//create file///////////
	HANDLE hf = CreateFile(
        lpszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_ALWAYS, NULL, NULL );

    if( hf == INVALID_HANDLE_VALUE )
        return FALSE;

    // write out the file header
	long lBufferSize = lWidth * lHeight * 3;

    BITMAPFILEHEADER bfh;
    memset( &bfh, 0, sizeof( bfh ) );
    bfh.bfType = 'MB';
    bfh.bfSize = sizeof( bfh ) + lBufferSize + sizeof( BITMAPINFOHEADER );
    bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );

    DWORD dwWritten = 0;
    WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );

    // and the bitmap format
    BITMAPINFOHEADER bih;
    memset( &bih, 0, sizeof( bih ) );
    bih.biSize = sizeof( bih );
    bih.biWidth = lWidth;
    bih.biHeight = lHeight;
    bih.biPlanes = 1;
    bih.biBitCount = 24;	//ͼ��Ϊ���ɫ

    dwWritten = 0;
    WriteFile( hf, &bih, sizeof( bih ), &dwWritten, NULL );

    // and the bits themselves
    dwWritten = 0;
    WriteFile( hf, newbuffer, lBufferSize, &dwWritten, NULL );
    CloseHandle( hf );

	return TRUE;
}

/////bmp����߶Ⱥ󱣴�ͼƬ���Ա�鿴ԭʼͼƬ 288->576////////////////////
BOOL CPictureProcess::ZoomImageHeight(LPCTSTR lpszFile, int nHeightTimes)
{
	BOOL bSuccess = FALSE;

	CString strSourceFile = lpszFile;		//ԭʼͼ

	//ԭʼͼ��С��Ϣ
	long lWidthSource, lHeightSource, lSizeSource;
	//int nNum;

	BITMAPINFO bmpInfo = GetBmpInfo(strSourceFile);	//get bmp info
	if(bmpInfo.bmiHeader.biBitCount != 24)
		return FALSE;

	lWidthSource = bmpInfo.bmiHeader.biWidth;
	if (lWidthSource <= 0)
		return FALSE;
	lHeightSource = bmpInfo.bmiHeader.biHeight;
	lSizeSource = lWidthSource * lHeightSource * 3;
	
	LPBYTE lpImage;
	lpImage=new byte[lSizeSource];
	GetBmpBuffer(strSourceFile,lpImage);

	//////////////
	long lHeightDest = lHeightSource * nHeightTimes;	//�߶�
	long lWidthDest = lWidthSource;	//Ŀ��ͼƬ���Ŀ�Ⱥ�ԭ��Ҫ����һ��
	long lSizeDest = lWidthDest * lHeightDest * 3;	//lHeightDest����ڲ��� 

	LPBYTE BufLast;
	BufLast=new byte[lSizeDest];		//����Ŀ���������Ŀռ�
	ZoomImageHeight(lpImage,BufLast,lWidthSource,lHeightSource,nHeightTimes);
	
	::DeleteFile(lpszFile);		//ɾ��ԭʼͼƬ
	SaveImage(lpszFile,BufLast,lWidthDest,lHeightDest);	//����������ͼƬ

	if(BufLast != NULL)
		delete BufLast;

	return TRUE;
}

//ָ��bmp����������߶ȣ�lWidth��lHeightΪԭʼ���Ŀ��
//Ŀ�����Ŀ��ԭʼ��һ�����߶���ԭʼ����2��
void CPictureProcess::ZoomImageHeight(LPBYTE sourcebuffer, LPBYTE destbuffer,
									  long lWidth,long lHeight,int nHeightTimes)
{

	int BmpHeight = 0;
	if(lHeight == 288 && lWidth >= 704)
	{
		//����߶�
		BmpHeight = lHeight;
		lHeight = lHeight * nHeightTimes;
	}else
		return;

	long len = lWidth * 3;
	LPBYTE newbuffer=new byte[lWidth*lHeight * 3];
	
	//Ŀ��ͼƬ�߶���ԭͼƬ�߶�2��������ѹ��ͼƬ 288->576
	for(int i=0;i<BmpHeight;i++)
	{
		//�ƶ�2�γ�ȫͼ
		memcpy(destbuffer + i*len*nHeightTimes, sourcebuffer+i*len,len);
		memcpy(destbuffer + i*len*nHeightTimes + len, sourcebuffer+i*len,len);
	}

	destbuffer-=lWidth*lHeight*3;

	if(newbuffer != NULL)
		delete newbuffer;		//ɾ��ָ��

	/*/////////////////////////////////////////////
	//�߶�Ϊ288ʱ��Ҫ�Ƚ�������
	//�Ϸ�����Ҳ���� 
	for (int i=0;i<288;i++)
	{
		 memcpy(newbuffer,lpImage,ptSize.x*3);
		 newbuffer+=ptSize.x*3;
		 memcpy(newbuffer,lpImage,ptSize.x*3);
		 newbuffer+=ptSize.x*3;
		 lpImage+=ptSize.x*3;
		 
	}
	destbuffer-=lWidth*lHeight*3;

	if(newbuffer != NULL)
		delete newbuffer;
	*//////////////////////////////////////////////

}

//jpg�ļ�ת����bmp�������ر�ת�����ļ���
//��ʱ������ʾ�ģ���ʾ֮��Ҫɾ��
CString CPictureProcess::ChangeJPGToBMP(CString strJPGFileName,CString dllName)
{

	CBasicUse bUse;

	FILE *fp;

	CString strTmp = strJPGFileName;
		
	CString LastDisplayFile = "";
	
	if(bUse.FileExists(strJPGFileName) == FALSE){
		return "";
	}

	//ת������Ϊ.bmp�ģ���ȡ·����
	strTmp = bUse.JPGToBMPString(strTmp);
	
	strTmp = strTmp.Mid(strTmp.ReverseFind('\\') + 1);

	//�ŵ���ʱĿ¼�£��Ա���ʾͼƬ��
	bUse.CreatePath(bUse.GetApp_Path() + "\\tempPic");
	LastDisplayFile = bUse.GetApp_Path() + "\\tempPic\\" + strTmp;

	//////////////����ʱ��ʱ����///////////
	
	//�����ʱͼƬû���ݣ�����ʾ
	if( strcmp(LastDisplayFile,"")!=0)
	{
		fp = fopen(LastDisplayFile,"r");
		if(fp!=NULL)
		{
			fclose(fp);
			unlink(LastDisplayFile);		//ɾ���ļ�
		}
	}
	
	////��CString -> char��Ϊ��JpgToBmp11�����Ĳ���
	char FileName_JPG[256];
	char FileName_BMP[256];

	strcpy(FileName_JPG,(LPCTSTR)strJPGFileName);

	LastDisplayFile.Format("%s",LastDisplayFile);

	strcpy(FileName_BMP,(LPCTSTR)LastDisplayFile);

	///��jpg��ʽ�ļ�ת����bmp��ʽ�ļ�///////////////////////////
	CString strCurAppPath = bUse.GetApp_Path();
	
	//���ö�̬���ӿ�,�������ķ�ʽ
	HINSTANCE HDLL_picProcess;	//��̬���ӿ���غ�ľ��jpg->bmp
	HDLL_picProcess = LoadLibrary(dllName);
	typedef BOOL(*pJpgToBmp)(LPCTSTR lpszJpgFile,LPCTSTR lpszBmpFile); 
	pJpgToBmp JpgToBmp;
	JpgToBmp=(pJpgToBmp)GetProcAddress(HDLL_picProcess,"JpgToBmp");
	if(JpgToBmp != NULL){
		BOOL res=JpgToBmp(FileName_JPG,FileName_BMP);
	}
	//ж��dll�ļ�
	FreeLibrary(HDLL_picProcess);

	LastDisplayFile.Format("%s",LastDisplayFile);
		
	return LastDisplayFile;

}


CString CPictureProcess::ChangeJPGToBMP2(CString strJPGFileName)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strCurPath;//CString strCurPath= m_bUse.GetApp_Path();
	char file_name[_MAX_PATH] = { 0 };
	GetModuleFileName(NULL, file_name, _MAX_PATH);
	CString strFileName = file_name;
	int num = strFileName.ReverseFind('\\');

	strCurPath = strFileName.Left(num+1);

	CBasicUse bUse;
	CString strTmp = bUse.JPGToBMPString(strJPGFileName);
	strTmp = strTmp.Mid(strTmp.ReverseFind('\\') + 1);

	bUse.CreatePath(strCurPath + "tempPic");
	CString JpgFile = strCurPath + "tempPic\\" + strTmp;

	//����jpg��jpeg�ļ�
	Bitmap *pBmp = Bitmap::FromFile(strJPGFileName.AllocSysString());
	if (pBmp)
	{
		CLSID clsid;
		//��ȡλͼ���ID
		GetCodecClsid(L"image/bmp", &clsid);

		//����Ϊλͼ����
		pBmp->Save(JpgFile.AllocSysString(),&clsid);
	}
	return JpgFile;
}
//��ȡ��������ͼ�����ID
int CPictureProcess::GetCodecClsid(const WCHAR* format, CLSID* pClsid)//�������ʶ��
{
	UINT  codenum = 0;   
	UINT  size = 0;   
	ImageCodecInfo* pImageCodecInfo = NULL;

	//��ȡͼ��ı����С
	GetImageEncodersSize(&codenum, &size);
	if(size == 0)
		return -1 ;  

	//����ͼ�������Ϣ������
	pImageCodecInfo = new ImageCodecInfo[size];
	if(pImageCodecInfo == NULL)
		return -1; 

	//��ȡͼ�������Ϣ
	GetImageEncoders(codenum, size, pImageCodecInfo);
	for(UINT j = 0; j < codenum; ++j)
	{
		//�Ƚ�ͼ����룬�Ƿ��������ͬ
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			//��ȡͼ�����͵���ID
			*pClsid = pImageCodecInfo[j].Clsid;

			//�ͷ�ͼ�������Ϣ������
			delete []pImageCodecInfo;
			return 0;  
		}    

	} 
	//�ͷ�ͼ�������Ϣ������
	delete []pImageCodecInfo;
	return -1;  
}






//jpg�ļ�ת����bmp�������ر�ת�����ļ���
//Ҫ�ŵ���bmpͬĿ¼��
CString CPictureProcess::ChangeBMPToJPG(CString strBMPFileName,CString dllName)
{

	CBasicUse bUse;
	

	CString LastDisplayFile = "";
	CString strTmp = strBMPFileName;
			
	if(bUse.FileExists(strBMPFileName) == FALSE){
		return "";
	}

	//ת������Ϊ.bmp�ģ���ȡ·����
	strTmp = bUse.BMPToJPGString(strTmp);
	LastDisplayFile = strTmp;	//�õ�jpg��

	FILE *fp;
	//�����ʱͼƬû���ݣ�����ʾ
	if( strcmp(LastDisplayFile,"")!=0)
	{
		fp = fopen(LastDisplayFile,"r");
		if(fp!=NULL)
		{
			fclose(fp);
			unlink(LastDisplayFile);		//ɾ���ļ�
		}
	}
	
	////��CString -> char��Ϊ��JpgToBmp11�����Ĳ���
	char FileName_JPG[256];
	char FileName_BMP[256];

	strcpy(FileName_BMP,(LPCTSTR)strBMPFileName);

	LastDisplayFile.Format("%s",LastDisplayFile);

	strcpy(FileName_JPG,(LPCTSTR)LastDisplayFile);

	///��jpg��ʽ�ļ�ת����bmp��ʽ�ļ�///////////////////////////
	CString strCurAppPath = bUse.GetApp_Path();
	
	//���ö�̬���ӿ�,�������ķ�ʽ
	HINSTANCE HDLL_picProcess;	//��̬���ӿ���غ�ľ��jpg->bmp
	HDLL_picProcess = LoadLibrary(dllName);
	typedef BOOL(*pBmpToJpg)(LPCTSTR lpszBmpFile,LPCTSTR lpszJpgFile); 
	pBmpToJpg BmpToJpg;
	BmpToJpg=(pBmpToJpg)GetProcAddress(HDLL_picProcess,"BmpToJpg");
	if(BmpToJpg != NULL){
		BOOL res=BmpToJpg(FileName_BMP,FileName_JPG);
	}
	//ж��dll�ļ�
	FreeLibrary(HDLL_picProcess);

	LastDisplayFile.Format("%s",LastDisplayFile);
		
	return LastDisplayFile;	//

}

CString CPictureProcess::ChangeBMPToJPG2(CString strBMPFileName)
{
	CBasicUse bUse;

	CString  JpgFile= bUse.BMPToJPGString(strBMPFileName);

	//����λͼ�ļ�
	Bitmap *pBmp = Bitmap::FromFile(strBMPFileName.AllocSysString());
	if (pBmp)
	{
		//������ID
		CLSID clsid;

		//����ѹ������
		int nQuality = 95;

		//����������
		EncoderParameters Encoders;

		//���ò�������
		Encoders.Count = 1;

		//���ò������
		Encoders.Parameter[0].Guid = EncoderQuality;

		//���ò�������
		Encoders.Parameter[0].Type = EncoderParameterValueTypeLong;

		//���ò���ֵ����
		Encoders.Parameter[0].NumberOfValues = 1;

		//����ѹ������
		Encoders.Parameter[0].Value = &nQuality;

		//��ȡJPEG��ID
		GetCodecClsid(L"image/jpeg", &clsid);

		//����ΪJPEG�ļ�
		pBmp->Save(JpgFile.AllocSysString(),&clsid,&Encoders);
	}
	return JpgFile;
}