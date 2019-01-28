#pragma once

#include"SocketArrayList.h"
#include<WinSock2.h>

//ȫ�ֱ�������ȫ�ֺ���
BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
DWORD WINAPI ListenThreadFunc(LPVOID Lparam);
DWORD WINAPI ClientThreadProc(LPVOID Lparam);


class TServerScoket
{
public:
	TServerScoket();
	~TServerScoket();

private:
	BOOL equal(const CClientItem *p1, const CClientItem * p2);
	LRESULT OnTrayCallbackMsg(WPARAM wparam, LPARAM lparam);

/*�����Լ�����*/
public:
	BOOL m_isServerOpen;
	SOCKET m_SockListen;
	UINT m_ServicePort;
	HANDLE m_hListenThread;
	CArray<CClientItem> m_ClientArray;
	BOOL m_RecvFlag;//��������־λ
	CString m_PicName;//ͼƬ����
	CString m_PicData;//ͼƬ����

	BOOL StartServer(UINT m_ServicePort);//����������
	void StopServer();//�رշ�����
	void OpenRecv(CString PicName);//�򿪽���
	void CloseRecv();//�رս���
	BOOL CreatePic();//����ͼƬ

	void RemoveClientFromArray(CClientItem in_item);
};
