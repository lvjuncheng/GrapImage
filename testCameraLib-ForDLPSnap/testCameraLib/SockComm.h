#pragma once
//#include <Windows.h>
#include <WinSock2.h>

#define MAX_BUFFER_SIZE 1024

class CSockComm
{
public:
	CSockComm();
	~CSockComm();
	bool CreateCommunication(unsigned long ip, unsigned short port);
	bool Send(unsigned char* data, int byteSize, int time);
	bool Read(unsigned char* data, int& byteSize, int time);

private:
	static DWORD AcceptThread(LPVOID pVoid);
	static DWORD WorkThread(LPVOID pVoid);
private:
	static bool m_bHasInitLib;
	static int m_sockCount;
	sockaddr_in m_serverSockAddr;
	SOCKET m_socket;
	SOCKET m_clientSocket;
	HANDLE m_acceptThread;
	HANDLE m_workThread;
	HANDLE m_eventGroup[3]; // Read Write Cancel
	HANDLE m_replyEventGroup[3]; //Reply Read Write Cancel
	bool m_bCreateSocket;
	bool m_bKeepLoop;
	bool m_bAccepted;
	int m_curValidRecvDataSize;
	int m_curValidSendDataSize;
	int m_curRecvResult;
	int m_curSendResult;
	char m_recvBuffer[MAX_BUFFER_SIZE]; 
	char m_sendBuffer[MAX_BUFFER_SIZE];
};

