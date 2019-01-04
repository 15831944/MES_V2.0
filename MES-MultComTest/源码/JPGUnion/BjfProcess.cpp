// BjfProcess.cpp: implementation of the CBjfProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BjfProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
//#define MAX_PATHC 256
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBjfVSJpg::CBjfVSJpg()
{
	//memset(m_FILE_HEADERTem.Redtime,0,strlen(m_FILE_HEADERTem.Redtime));
	//memset(m_FILE_HEADERTem.szType,0,strlen(m_FILE_HEADERTem.szType));

	memset(m_FILE_HEADERTem.Redtime,0,8);
	memset(m_FILE_HEADERTem.szType,0,42);
}

CBjfVSJpg::~CBjfVSJpg()
{

}
//============================================================
//�������ƣ�UnionAllFile
//�������أ�BOOL
//����������sDestFileNameΪҪ�����bjf�ļ���
//          sSourceFileNameΪ���jpg���ļ�ȫ�����飬
//          nFileCountΪjpg�ļ�������
//�������ܣ��ϳɶ���JPEGͼ��Ĭ�Ϻϳɺ��ͼƬ�����D��\\��ʱͼƬ
//��ע    �����Ҫ�ϳɵ��ļ���С��һ�£�������dwFileLenth
//          ֵ����ʵ���ļ��ĵĴ�С��
//============================================================
BOOL CBjfVSJpg::UnionAllFile(CString sDestFileName, CString *sSourceFileName)
{

	UNFILEBLOCK UNFileBlockTem;
	int PhotoPoint = m_FILE_HEADERTem.nFileCount;
	char BjfName[256];
	char JpgName[256];

	void *FileBuffer;
	DWORD FileLength;
	strcpy(BjfName,sDestFileName);

	int i;
	i=0;

	CFile fp,fp1;
	//�����ʧ�ܣ�����0.
	if( !fp.Open(BjfName ,CFile::modeCreate | CFile::modeWrite,NULL) )
		return FALSE;

	FileLength = 0;
    fp.Write(&m_FILE_HEADERTem,sizeof(FILE_HEADER));	
	//bjf�ļ�ͷ�ĳ���
	DWORD dBjfLength;
		
    dBjfLength=sizeof(FILE_HEADER);
	//ѭ����ָ��������ͼƬ�ļ�����ȡ���ݣ�д��Ҫ�ϳɵ�bjf�ļ���
	for(i=0;i<PhotoPoint;i++)
    {
		strcpy(JpgName,"");
		strcpy(JpgName,sSourceFileName[i]);
		if( !fp1.Open(JpgName,CFile::modeRead,NULL)  )
		{
			//continue;
			return FALSE;
		}
		FileLength = fp1.GetLength(); 
		FileBuffer = NULL;
		FileBuffer = malloc( FileLength );
		if( FileBuffer!=NULL)
		{
			FILE_BLOOK FILE_BLOOKTem;
			//��ͷ��ʼ��
			//JpgName�ļ�����������·����Ҫ���浽�ṹ��
			CString sTempFileName = "";
			int nSitu = 0;
			sSourceFileName[i].Format("%s",sSourceFileName[i]);	//��Ҫ����formatתһ�£�����mid����ȡ���ļ���������
			nSitu = sSourceFileName[i].ReverseFind('\\');
			sTempFileName = sSourceFileName[i].Mid(nSitu+1);
			//--·������
			//sTempFileName="C:\\Program Files\\���Ӿ���ϵͳ\\pic_zc\\"+sTempFileName;
			CString sFilePath="";
			sDestFileName.Format("%s",sDestFileName);
			nSitu = sDestFileName.ReverseFind('\\');
			sFilePath = sDestFileName.Mid(0,nSitu+1);
			sTempFileName=sFilePath+sTempFileName;

			strcpy(FILE_BLOOKTem.szFileName,sTempFileName);
            FILE_BLOOKTem.dwFileLenth=FileLength;
			//д���ļ�BLOOK��
			//fp.Write(&FILE_BLOOKTem,sizeof(FILE_BLOOK)); 
			UNFileBlockTem.FILE_BLOOKTem=FILE_BLOOKTem;
			UNFileBlockTem.ByteLineTem.dwFileLen=FileLength;
            //fp.Write(&UNFileBlockTem,sizeof(UNFILEBLOCK)); 
			fp.Write(&UNFileBlockTem,sizeof(FILE_BLOOK)); 
			//д�ļ�����
			fp.SeekToEnd(); 
			//�����ļ����ݺ�д��ָ���ļ���
			fp1.Read(FileBuffer, FileLength); 
			fp.Write(FileBuffer,FileLength); 	
			free(FileBuffer);
		}
		fp1.Close(); 
	}
	fp.Write(&m_FILE_TAILTem,sizeof(FILE_TAIL));		
	fp.Close(); 

	return TRUE;
}

//============================================================
//�������ƣ�GetOneFile
//�������أ�CString������ֵΪjpg�ļ�ȫ��
//����������sSourceFileNameΪbjf�ļ�����
//          nNumΪҪȡ����jpgͼƬ��
//�������ܣ��ֽ��ȡһ��JPEGͼ��Ĭ�Ϸֽ���ͼƬ����ں�BjfͬһĿ¼��
//          ���Ҫ�޸Ĵ��Ŀ¼������Ҫ�޸ĺ������ݡ�
//��ע    ��
//============================================================
CString CBjfVSJpg::GetOneFile(CString sSourceFileName, int nNum)
{
	int PhotoPoint = 0;

	//Ҫȡ����jpg��bjf��ĵڼ���,0��ʾ��1��,1��ʾ��2�ţ�2��ʾ��3�ţ�3��ʾ��4��
	int n=nNum;

	char JpgName[256],BjfName[256];
	strcpy(BjfName,sSourceFileName);

	void *FileBuffer;
	DWORD FileLength,TmpLen;

	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";

	//��FILE_HANDEAR
    ////FILE_HEADER FILE_HEADERTem;
	DWORD len=fp.GetLength()-sizeof(FILE_TAIL);
	fp.Seek(len,CFile::begin);
	fp.Read(&m_FILE_TAILTem, sizeof(FILE_TAIL));
	fp.SeekToBegin();
	fp.Read(&m_FILE_HEADERTem, sizeof(FILE_HEADER));

	//��FILE_BLOOK
	FILE_BLOOK FILE_BLOOKTem;
    //fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));

	//bjf�ļ�ͷ�ĳ���------Ҫ�������ݵ��ļ���ʼλ�õĳ���
	DWORD dBjfLength;

    dBjfLength=sizeof(FILE_HEADER);

	/////////////
	FileLength = dBjfLength+sizeof(FILE_BLOOK);
	PhotoPoint=m_FILE_HEADERTem.nFileCount;
	if(n>PhotoPoint)
		return "";
	///////////

	for(int i0=0;i0<PhotoPoint;i0++)
	{
		BYTE *tem;
		tem=new BYTE[500];
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(tem, 500);
		unsigned long length;
		//for(int i1=0;i1<500;i1++)
		int i1=260;
		{
			//if(tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256>20*1024&&
			//   tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256<45*1024)
			{
				// ::AfxMessageBox("find lenth memory");
				length=tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256;
				//break;
			}
		}
		delete tem;
		//�ƶ��ļ�ָ�롣
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));
        TmpLen=FILE_BLOOKTem.dwFileLenth;
		TmpLen=length;
		FILE_BLOOKTem.dwFileLenth=length;
		//�ж��Ƿ���Ҫ�򿪵�Դ�ļ���

		if(n==i0)
		{
			//��Ҫ�򿪵��ļ�Դ�ļ�
			fp.Seek(FileLength,CFile::begin);
		    FileBuffer = NULL;
			//�����ļ�BLOOK��С���ڴ档
		    FileBuffer = malloc( TmpLen );
			CString csFilename;
			csFilename=FILE_BLOOKTem.szFileName;
			int len=csFilename.ReverseFind('\\');
			csFilename=csFilename.Mid(len+1);
			strcpy(JpgName,csFilename);

			//�ļ����ټ��Ϻ�BjfͬĿ¼��·��
			int nSitu = 0;
			CString sTempFilePath = "";
			CString sFileName = "";
			sSourceFileName.Format("%s",sSourceFileName);	//��Ҫ����formatתһ�£�����mid����ȡ���ļ���������
			nSitu = sSourceFileName.ReverseFind('\\');
			sTempFilePath = sSourceFileName.Mid(0,nSitu);	//�õ�·��
			//--ȥ���ļ����е�·��

			sFileName.Format("%s",JpgName);
			//--����jpg�ļ�����
			CString szOrder;
			szOrder.Format("%d.jpg",nNum);
			//sFileName=sFileName+szOrder;
			//sFileName.Replace(".jpg",szOrder);
			sFileName = sTempFilePath + "\\" + sFileName;
			strcpy(JpgName,"");
			strcpy(JpgName,sFileName);

			if(FileBuffer!=NULL)
			{
				fp.Read(FileBuffer,TmpLen);
				//��ȡ�ļ�
				fp.Close();
				if( fp.Open(JpgName,CFile::modeCreate | CFile::modeWrite , NULL) )
				{
					//������������д���ļ�
					fp.Write(FileBuffer,TmpLen);
					fp.Close();
				}
				free(FileBuffer);
				CString filepath;
				filepath.Format(JpgName);
				//�����ļ�·��+�ļ�����
				return filepath;
			}
			else
			{
		    	fp.Close(); 
				return "";
			}
	    
		}
        //����Ҫ�򿪵�Դ�ļ������Ҫ�ƶ��ļ�ָ��ĳ��ȡ�
		FileLength+=FILE_BLOOKTem.dwFileLenth;
		FileLength+=sizeof(FILE_BLOOK);
	}
	return "";
}
//============================================================
//�������ƣ�GetOneFileForPath
//�������أ�CString������ֵΪjpg�ļ�ȫ��
//����������sSourceFileNameΪbjf�ļ�����
//          nNumΪҪȡ����jpgͼƬ��
//�������ܣ��ֽ��ȡһ��JPEGͼ��Ĭ�Ϸֽ���ͼƬ����ں�BjfͬһĿ¼��
//          ���Ҫ�޸Ĵ��Ŀ¼������Ҫ�޸ĺ������ݡ�
//��ע    ���˺���Ϊ���ݾ����ͼƬ�������һ�㲻���á�
//============================================================
CString CBjfVSJpg::GetOneFileForPath(CString sSourceFileName, int nNum)
{
	int PhotoPoint = 0;

	//Ҫȡ����jpg��bjf��ĵڼ���,0��ʾ��1��,1��ʾ��2�ţ�2��ʾ��3�ţ�3��ʾ��4��
	int n=nNum;

	char JpgName[256],BjfName[256];
	strcpy(BjfName,sSourceFileName);

	void *FileBuffer;
	DWORD FileLength,TmpLen;

	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";

	//��FILE_HANDEAR
    ////FILE_HEADER FILE_HEADERTem;
	DWORD len=fp.GetLength()-sizeof(FILE_TAIL);
	fp.Seek(len,CFile::begin);
	fp.Read(&m_FILE_TAILTem, sizeof(FILE_TAIL));
	fp.SeekToBegin();
	fp.Read(&m_FILE_HEADERTem, sizeof(FILE_HEADER));

	//��FILE_BLOOK
	FILE_BLOOK FILE_BLOOKTem;
    //fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));

	//bjf�ļ�ͷ�ĳ���------Ҫ�������ݵ��ļ���ʼλ�õĳ���
	DWORD dBjfLength;

    dBjfLength=sizeof(FILE_HEADER);

	/////////////
	FileLength = dBjfLength+sizeof(FILE_BLOOK);
	PhotoPoint=m_FILE_HEADERTem.nFileCount;
	if(n>PhotoPoint)
		return "";
	///////////

	for(int i0=0;i0<PhotoPoint;i0++)
	{
		BYTE *tem;
		tem=new BYTE[500];
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(tem, 500);
		unsigned long length;
		//for(int i1=0;i1<500;i1++)
		int i1=260;
		{
			//if(tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256>20*1024&&
			//   tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256<45*1024)
			{
				// ::AfxMessageBox("find lenth memory");
				length=tem[i1+0]+tem[i1+1]*256+tem[i1+2]*256*256+tem[i1+3]*256*256*256;
				//break;
			}
		}
		delete tem;
		//�ƶ��ļ�ָ�롣
		fp.Seek(FileLength-sizeof(FILE_BLOOK),CFile::begin);
		fp.Read(&FILE_BLOOKTem, sizeof(FILE_BLOOK));
        TmpLen=FILE_BLOOKTem.dwFileLenth;
		TmpLen=length;
		FILE_BLOOKTem.dwFileLenth=length;
		//�ж��Ƿ���Ҫ�򿪵�Դ�ļ���

		if(n==i0)
		{
			//��Ҫ�򿪵��ļ�Դ�ļ�
			fp.Seek(FileLength,CFile::begin);
		    FileBuffer = NULL;
			//�����ļ�BLOOK��С���ڴ档
		    FileBuffer = malloc( TmpLen );
			strcpy(JpgName,FILE_BLOOKTem.szFileName);
			//�ļ����ټ��Ϻ�BjfͬĿ¼��·��
			int nSitu = 0;
			CString sTempFilePath = "";
			CString sFileName = "";
			sSourceFileName.Format("%s",sSourceFileName);	//��Ҫ����formatתһ�£�����mid����ȡ���ļ���������
			nSitu = sSourceFileName.ReverseFind('\\');
			sTempFilePath = sSourceFileName.Mid(0,nSitu);	//�õ�·��
			sFileName.Format("%s",JpgName);
			//--����jpg�ļ�����
			CString szOrder;
			szOrder.Format("%d.jpg",nNum);
			//sFileName=sFileName+szOrder;
			//sFileName.Replace(".jpg",szOrder);
			//sFileName = sTempFilePath + "\\" + sFileName;
			strcpy(JpgName,"");
			strcpy(JpgName,sFileName);

			if(FileBuffer!=NULL)
			{
				fp.Read(FileBuffer,TmpLen);
				//��ȡ�ļ�
				fp.Close();
				if( fp.Open(JpgName,CFile::modeCreate | CFile::modeWrite , NULL) )
				{
					//������������д���ļ�
					fp.Write(FileBuffer,TmpLen);
					fp.Close();
				}
				free(FileBuffer);
				CString filepath;
				filepath.Format(JpgName);
				//�����ļ�·��+�ļ�����
				return filepath;
			}
			else
			{
		    	fp.Close(); 
				return "";
			}
	    
		}
        //����Ҫ�򿪵�Դ�ļ������Ҫ�ƶ��ļ�ָ��ĳ��ȡ�
		FileLength+=FILE_BLOOKTem.dwFileLenth;
		FileLength+=sizeof(FILE_BLOOK);
	}
	return "";
}

//============================================================
//�������ƣ�GetJpgFileCount
//============================================================
int CBjfVSJpg::GetJpgFileCount(CString sSourceFileName)
{

	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return 0;


	//��FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
	fp.Close();
    return FILE_HEADERTem.nFileCount;
}
//============================================================
//�������ƣ�GetEveryFile
//�������أ�CStringָ�롣
//����������sSourceFileNameΪbjf�ļ���.
//         
//�������ܣ�����bjf�ļ����а�����jpg�ļ�������ȡÿ���ļ������浽ָ��Ŀ¼��
//          Ĭ��Ϊd:\\��ʱͼƬ��
//��ע    ��
//============================================================
CString * CBjfVSJpg::GetEveryFile(CString sSourceFileName)
{
	
	int filecount;
	CString *csSeparate;
    csSeparate=new CString[4];
	filecount=GetJpgFileCount(sSourceFileName);
	for(int i=0;i<filecount;i++)
		csSeparate[i]=GetOneFile(sSourceFileName, i);

	return csSeparate;

}
//============================================================
//�������ƣ�GetEveryFile
//�������أ�CStringָ�롣
//����������sSourceFileNameΪbjf�ļ���.
//         
//�������ܣ�����bjf�ļ����а�����jpg�ļ�������ȡÿ���ļ������浽ָ��Ŀ¼��
//          Ĭ��Ϊd:\\��ʱͼƬ��
//��ע    ��
//============================================================
CString * CBjfVSJpg::GetEveryFileGetPath(CString sSourceFileName)
{
	
	int filecount;
	CString *csSeparate;
    csSeparate=new CString[4];
	filecount=GetJpgFileCount(sSourceFileName);
	for(int i=0;i<filecount;i++)
		csSeparate[i]=GetOneFileForPath(sSourceFileName, i);

	return csSeparate;

}
//============================================================
//�������ƣ�ClassSet
//�������أ���
//����������FILE_HEADERTem�������ʼ��.
//         
//�������ܣ���ʼ���ļ�ͷ
//��ע    ��
//============================================================
void CBjfVSJpg::SetClassType(CString csType)
{
	strcpy(m_FILE_HEADERTem.szType,csType);
}

CString CBjfVSJpg::GetClassType()
{
	CString csType;
	csType.Format(m_FILE_HEADERTem.szType);
	return csType;
}
CString CBjfVSJpg::GetClassType(CString sSourceFileName)
{
	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";


	//��FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
    CString fileType;
    fileType.Format(FILE_HEADERTem.szType);
	fp.Close();
	return fileType;

}
//���ýṹ�е�Υ��ʱ��
void CBjfVSJpg::SetClassRedtime(CString csRedtime)
{
	CString szTem=csRedtime.Mid(0,8);
	strcpy(m_FILE_HEADERTem.Redtime,szTem);
	
}
void CBjfVSJpg::SetClassIllegaltime(CString csRedtime)
{
	CString szTem=csRedtime.Mid(0,40);
	strcpy(m_FILE_TAILTem.Redtime,szTem);
	
}
//�õ��ṹ�е�Υ��ʱ��
CString CBjfVSJpg::GetClassRedtime()
{
	CString csRedtime;
	csRedtime.Format(m_FILE_HEADERTem.Redtime);
	return csRedtime;
}
CString CBjfVSJpg::GetClassIllegaltime()
{
	CString csRedtime;
	csRedtime.Format(m_FILE_TAILTem.Redtime);
	return csRedtime;
}
//
CString CBjfVSJpg::GetClassRedtime(CString sSourceFileName)
{
	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return "";

	//��FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
    CString fileRedtime;
    fileRedtime.Format(FILE_HEADERTem.Redtime);
	fp.Close();
	return fileRedtime;
}

void CBjfVSJpg::SetClassFileCount(int nFileCount)
{
	m_FILE_HEADERTem.nFileCount=nFileCount;
}

int CBjfVSJpg::GetClassFileCount()
{
	return m_FILE_HEADERTem.nFileCount;
}

int CBjfVSJpg::GetClassFileCount(CString sSourceFileName)
{
	char BjfName[256];
	strcpy(BjfName,sSourceFileName);
	CFile fp;
	if( !fp.Open(BjfName ,CFile::modeRead,NULL) )
		return 0;

	//��FILE_HANDEAR
    FILE_HEADER FILE_HEADERTem;
	fp.Read(&FILE_HEADERTem, sizeof(FILE_HEADER));
    CString fileRedtime;
    fileRedtime.Format(FILE_HEADERTem.szType);
	fp.Close();
	return FILE_HEADERTem.nFileCount;
}
