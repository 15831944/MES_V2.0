#include "stdafx.h"
#include "TServerScoket.h"

TServerScoket::TServerScoket()
{
	m_isServerOpen = FALSE;
	m_SockListen = NULL;
	m_ServicePort = 8260;
	m_hListenThread = NULL;
	m_RecvFlag = FALSE;
}


TServerScoket::~TServerScoket()
{
}


BOOL TServerScoket::StartServer(UINT ServicePort)
{
	AfxSocketInit();
	m_ServicePort = ServicePort;
	m_hListenThread = CreateThread(NULL, 0, ListenThreadFunc, this, 0, 0);
	if (m_hListenThread == NULL)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}
}

void TServerScoket::StopServer()
{
	int AllClient = m_ClientArray.GetCount();
	for (int idx = 0; idx < AllClient; idx++)
	{
		closesocket(m_ClientArray.GetAt(idx).m_ClientSocket);
	}
	m_ClientArray.RemoveAll();
	closesocket(m_SockListen);
	m_isServerOpen = FALSE;
}

DWORD WINAPI ListenThreadFunc(LPVOID Lparam)
{
	TServerScoket * pServer = (TServerScoket *)Lparam;
	if (INVALID_SOCKET == (pServer->m_SockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		GetLastError();
		AfxMessageBox(_T("����socketʧ��"));
		return FALSE;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(pServer->m_ServicePort);
	if (0 != bind(pServer->m_SockListen, (sockaddr *)&service, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("�󶨶˿�ʧ��"));
		return FALSE;
	}
	if (0 != listen(pServer->m_SockListen, 5))
	{
		AfxMessageBox(_T("�����˿�ʧ��"));
		return FALSE;
	}
	//����ѭ���������˿�
	while (TRUE)
	{
		if (socket_Select(pServer->m_SockListen, 100, TRUE))
		{
			sockaddr_in clientAddr;
			int iLen = sizeof(sockaddr_in);
			SOCKET accSock = accept(pServer->m_SockListen, (struct sockaddr *)&clientAddr, &iLen);
			if (accSock == INVALID_SOCKET)
			{
				continue;
			}
			//���ڵ��������
			CClientItem tItem;
			tItem.m_ClientSocket = accSock;
			tItem.m_strIp = inet_ntoa(clientAddr.sin_addr); //IP��ַ
			int idx = pServer->m_ClientArray.Add(tItem); //idx�ǵ�x�����ӵĿͻ���
			pServer->m_ClientArray[0].m_pMainWnd = pServer;
			tItem.m_hThread = CreateThread(NULL, 0, ClientThreadProc, //����һ���̲߳�����:CREATE_SUSPENDED
				&(pServer->m_ClientArray.GetAt(idx)), CREATE_SUSPENDED, NULL);
			pServer->m_ClientArray.GetAt(idx).m_hThread = tItem.m_hThread;
			//�Ȱ�hThread�����˽ڵ㣬�ſ�ʼִ���̣߳�����
			ResumeThread(tItem.m_hThread);
			Sleep(100);
		}
		Sleep(100);
	}
}

// 16���������ַ�ת��
BYTE ConvertHexChar(BYTE ch)
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
BYTE * StrDecToCHex(int &length, CString str)
{
	BYTE	tmpByte = 0x00;
	int		slen = str.GetLength();

	length = 0;

	BYTE * h_data = new BYTE[slen];

	::memset(h_data, '\0', sizeof(h_data));

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

#define MAX_BUFF 1024
DWORD WINAPI ClientThreadProc(LPVOID Lparam){ //�����첽IOģ��ѭ����ȡsocket�ڵ���Ϣ�������͸������û�
	USES_CONVERSION;
	//TServerScoket * pServer = (TServerScoket *)Lparam;
	CString PicStr, strMsg = _T(""), temp = _T("");
	CClientItem ClientItem = *(CClientItem *)Lparam;
	while (TRUE)
	{
		if (socket_Select(ClientItem.m_ClientSocket, 500, TRUE))
		{
			char szRev[MAX_BUFF] = { 0 };
			int iRet = recv(ClientItem.m_ClientSocket, szRev, sizeof(szRev), '\0');
			if (iRet > 0 && ClientItem.m_pMainWnd->m_RecvFlag == TRUE)
			{
				for (int i = 0; i < iRet; i++)
				{
					//if (temp != _T('\0'))
					//{
					temp.Format(_T("%02X"), (unsigned char)szRev[i]);
					strMsg += temp;
					//}
				}
				ClientItem.m_pMainWnd->m_PicData += strMsg;
				strMsg = "";
			}
			else{
				strMsg = ClientItem.m_strIp + _T(" ���뿪");
				ClientItem.m_pMainWnd->RemoveClientFromArray(ClientItem);
				break;
			}
		}
		//Sleep(1);  //����ͼ������Ҫ�������Ϊ��������ͷ�ڴ������ͻ��Զ��뿪��Ȼ���Զ��˳����whileѭ��������Ҫ�����������⣬���˴�����whileѭ���Ļ�������Ӱ�촫���ٶ�
	}
	return 0;
}

BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead){
	FD_SET fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iRet = 0;
	if (bRead)
	{
		iRet = select(0, &fdset, NULL, NULL, &tv);
	}
	else
	{
		iRet = select(0, NULL, &fdset, NULL, &tv);
	}
	if (iRet <= 0)
	{
		return FALSE;
	}
	else if (FD_ISSET(hSocket, &fdset))
	{
		return TRUE;
	}
	return FALSE;
}

void TServerScoket::RemoveClientFromArray(CClientItem in_item){
	for (int idx = 0; idx < m_ClientArray.GetCount(); idx++)
	{
		if (in_item.m_ClientSocket == m_ClientArray[idx].m_ClientSocket &&
			in_item.m_hThread == m_ClientArray[idx].m_hThread &&
			in_item.m_strIp == m_ClientArray[idx].m_strIp)
		{
			m_ClientArray.RemoveAt(idx);
		}
	}
	return;
}

//�򿪽���
void TServerScoket::OpenRecv(CString PicName)
{
	m_RecvFlag = TRUE;
	m_PicName = PicName;
}

//�رս���
void TServerScoket::CloseRecv()
{
	m_RecvFlag = FALSE;
}

//����ͼƬ
BOOL TServerScoket::CreatePic()
{
	int dwSize = m_PicData.GetLength() / 2;
	//�ж�ͷβ�Ƿ���ȷ,β���ж���Ҫ��һ��ռ䣬���óɺ���16���ֽ��ҵõ�FFD9����
	if (m_PicData.Find(_T("FFD8FF")) == 0 && m_PicData.Find(_T("FFD9"))>(dwSize * 2 - 16))
	{
		CFile file;
		GetLastError();
		file.Open(m_PicName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);
		BYTE *buf = StrDecToCHex(dwSize, m_PicData);
		file.Write(buf, dwSize);
		file.Close();
	}
	else
	{
		m_PicData = "";
		return FALSE;
	}
	m_PicData = "";
	return TRUE;
}