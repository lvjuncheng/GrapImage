#include "stdafx.h"
#include "SockComm.h"
//#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

bool CSockComm::m_bHasInitLib = false;
int CSockComm::m_sockCount = 0;

CSockComm::CSockComm():
m_acceptThread(NULL),
m_workThread(NULL),
m_bCreateSocket(false),
m_bKeepLoop(false),
m_curValidRecvDataSize(0),
m_curValidSendDataSize(0),
m_curRecvResult(0),
m_curSendResult(0)
{
	++CSockComm::m_sockCount;
	for (int i = 0; i < 3; ++i)
	{
		m_eventGroup[i] = NULL;
		m_replyEventGroup[i] = NULL;
	}
}


CSockComm::~CSockComm()
{
	m_bKeepLoop = false;
	HANDLE tmpHandles[2] = { 0 };
	tmpHandles[0] = m_acceptThread;
	tmpHandles[1] = m_workThread;
	WaitForMultipleObjects(2, tmpHandles, TRUE, INFINITE);
	CloseHandle(m_acceptThread);
	m_acceptThread = NULL;
	CloseHandle(m_workThread);
	m_workThread = NULL;
	for (int i = 0; i < 3; ++i)
	{
		if (m_eventGroup[i])
		{
			CloseHandle(m_eventGroup[i]);
			m_eventGroup[i] = NULL;
		}
		if (m_replyEventGroup[i])
		{
			CloseHandle(m_replyEventGroup[i]);
			m_replyEventGroup[i] = NULL;
		}
	}
	if (m_bCreateSocket)
	{
		closesocket(m_socket);
		m_bCreateSocket = false;
	}
	if (m_bAccepted)
	{
		closesocket(m_clientSocket);
		m_bAccepted = false;
	}
	if (0 == --CSockComm::m_sockCount)
	{
		WSACleanup();
	}
}

bool CSockComm::CreateCommunication(unsigned long ip, unsigned short port)
{
	if (false == CSockComm::m_bHasInitLib)
	{
		WSADATA wd;
		if (0 == WSAStartup(MAKEWORD(2, 2), &wd))
		{
			if (2 == LOBYTE(wd.wVersion) && 2 == HIBYTE(wd.wVersion))
			{
				m_bHasInitLib = true;
			}
		}
	}
	if (false == m_bHasInitLib)
	{
		printf("Init Sock Lib Failed\n");
		WSACleanup();
		return false;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		printf("Create Socket Failed, Error: %d\n", WSAGetLastError());
		return false;
	}
	memset(&m_serverSockAddr, 0, sizeof(sockaddr_in));
	m_serverSockAddr.sin_family = AF_INET;
	m_serverSockAddr.sin_addr.S_un.S_addr = ip;
	m_serverSockAddr.sin_port = port;
	int ret = bind(m_socket, (sockaddr*)&m_serverSockAddr, sizeof(m_serverSockAddr));
	if (SOCKET_ERROR == ret)
	{
		closesocket(m_socket);
		printf("Bind Socket Failed\n");
		return false;
	}
	ret = listen(m_socket, 8);
	if (SOCKET_ERROR == ret)
	{
		printf("Listen Socket Failed\n");
		closesocket(m_socket);
		return false;
	}
	for (int i = 0; i < 3; ++i)
	{
		m_eventGroup[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_replyEventGroup[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		ResetEvent(m_eventGroup[i]);
		ResetEvent(m_replyEventGroup[i]);
	}

	int sockAddrSize = sizeof(m_serverSockAddr);
	m_clientSocket = accept(m_socket, (sockaddr*)&(m_serverSockAddr), &sockAddrSize);

	m_bKeepLoop = true;
	DWORD tmpThreadID;
/*	m_acceptThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AcceptThread, this, 0, &tmpThreadID);
	if (NULL == m_acceptThread)
	{
		closesocket(m_socket);
		m_bKeepLoop = false;
		printf("Create Accept Thread Failed\n");
		return false;
	}*/
	m_workThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, this, 0, &tmpThreadID);
	if (NULL == m_workThread)
	{
		m_bKeepLoop = false;
		WaitForSingleObject(m_acceptThread, INFINITE);
		CloseHandle(m_acceptThread);
		m_acceptThread = NULL;
		closesocket(m_socket);
		m_bKeepLoop = false;
		printf("Create Work Thread Failed\n");
		return false;
	}
	m_bCreateSocket = true;
	return true;
}

bool CSockComm::Send(unsigned char* data, int byteSize, int time)
{
	if (NULL == m_workThread || false == m_bKeepLoop)
		return false;

	int tmpSize = byteSize > MAX_BUFFER_SIZE ? MAX_BUFFER_SIZE : byteSize;
	memcpy(m_sendBuffer, data, tmpSize);
	m_curValidSendDataSize = tmpSize;
	SetEvent(m_eventGroup[1]);
	DWORD ret = WaitForSingleObject(m_replyEventGroup[1], time);
	if (WAIT_FAILED == ret || WAIT_TIMEOUT == ret)
		return false;
	else
	{
		ResetEvent(m_replyEventGroup[1]);
		return true;
	}
}

bool CSockComm::Read(unsigned char* data, int& byteSize, int time)
{
	if (NULL == m_workThread || false == m_bKeepLoop)
		return false;

	SetEvent(m_eventGroup[0]);
	DWORD ret = WaitForSingleObject(m_replyEventGroup[0], time);
	if (WAIT_FAILED == ret || WAIT_TIMEOUT == ret)
		return false;
	else
	{
		ResetEvent(m_replyEventGroup[0]);
		int tmpSize = byteSize > m_curValidRecvDataSize ? m_curValidRecvDataSize : byteSize;
		memcpy(data, m_recvBuffer, tmpSize);
		byteSize = tmpSize;
		return true;
	}
}

DWORD CSockComm::AcceptThread(LPVOID pVoid)
{
	if (NULL == pVoid)
	{
		return 0;
	}
	CSockComm* pThis = (CSockComm*)pVoid;
	int sockAddrSize = sizeof(pThis->m_serverSockAddr);
	while (pThis->m_bKeepLoop)
	{
		int tmpAddrSize = 0;
		pThis->m_clientSocket = accept(pThis->m_socket, (sockaddr*)&(pThis->m_serverSockAddr), &tmpAddrSize);
		if (INVALID_SOCKET != pThis->m_clientSocket)
		{
			pThis->m_bAccepted = true;
			break;
		}
	}
	return 0;
}

DWORD CSockComm::WorkThread(LPVOID pVoid)
{
	if (NULL == pVoid)
	{
		return 0;
	}
	CSockComm* pThis = (CSockComm*)pVoid;
	bool bNeedExit = false;
	while (pThis->m_bKeepLoop)
	{
		DWORD tmpRet = WaitForMultipleObjects(3, pThis->m_eventGroup, FALSE, 5000);
		if (WAIT_TIMEOUT == tmpRet || WAIT_FAILED == tmpRet)
		{
			printf("WaitForMultipleObjects return: %d\n", tmpRet);
			continue;
		}
		int eventIndex = tmpRet - WAIT_OBJECT_0;
		ResetEvent(pThis->m_eventGroup[eventIndex]);
		int tmpSize = 0;
		switch (eventIndex)
		{
		case 0: // Read
			tmpSize = recv(pThis->m_clientSocket, pThis->m_recvBuffer, MAX_BUFFER_SIZE, 0);
			if (SOCKET_ERROR == tmpSize)
				pThis->m_curRecvResult = WSAGetLastError();
			else if (0 == tmpSize)
				pThis->m_curRecvResult = -1; // 连接断开
			pThis->m_curValidRecvDataSize = tmpSize;
			SetEvent(pThis->m_replyEventGroup[0]); // 通知读取到数据
			break;
		case 1: // Write
			tmpSize = send(pThis->m_clientSocket, pThis->m_sendBuffer, pThis->m_curValidSendDataSize, 0);
			if (SOCKET_ERROR == tmpSize)
				pThis->m_curSendResult = WSAGetLastError();
			else if (0 == tmpSize)
				pThis->m_curSendResult = -1;
			SetEvent(pThis->m_replyEventGroup[1]); // 通知完成发送
			break;
		case 2:
			bNeedExit = true;
			break;
		default:break;
		}
		if (bNeedExit)
		{
			break;
		}
	}
	printf("Exist Work Thread\n");
	return 0;
}