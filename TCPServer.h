#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "CRingBuffer.h"
#include <unordered_map>
#include <thread>

class TCPServer
{
public:
	int ServerPort = 0;
	int MaxClientNumber = 0;
	int WorkerThreadNumber = 0;

	// 출력 위해 쓰는 변수
	int sendByte;
	int recvByte;
	int acceptCount;
	CRITICAL_SECTION printLock;


	class Session
	{
	public:
		Session()
		{
			IO_COUNT = 0;
			IS_SEND = 0;
		}

		WSAOVERLAPPED recvOverlapped;
		WSAOVERLAPPED sendOverlapped;
		SOCKET sock;
		int mSessionId;
		RingBuffer recvRingBuffer;
		RingBuffer sendRingBuffer;

		char sendTempBuffer[4096 * 4];
		CRITICAL_SECTION sessionLock;

		LONG IO_COUNT;
		LONG IS_SEND;
	};

	std::unordered_map<int, Session*> session_map;
	CRITICAL_SECTION g_session_map_cs;


	std::thread thread[256];

	HANDLE IOCP;
	int g_session_id = 0;

	void AcceptThread();
	void WorkerThread();

	void RecvPost(int mSessionId);
	void SendPost(int mSessionId);

public:

	int SendPacket(int mSessionId, char* messsage, int size);
	void DeleteSession(int mSessionId);
	void ForceDeleteSession(int mSessionId);
	int Run(int serverPort, int workerThreadNumber);

	virtual int OnRecv(int mSessionId, char* recvBuffer, int length) = 0;
	virtual void OnSend(int mSessionId, int length) = 0;
	virtual bool OnAccept(int mSessionId, char* ip, int port) = 0;
	virtual void OnDisconnet(int mSessionId) = 0;
	virtual void OnServerState(int sec_sendByte, int sec_recvByte, int accept_count) = 0;
};