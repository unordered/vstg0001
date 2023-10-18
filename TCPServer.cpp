#pragma once
#include "TCPServer.h"
#include <thread>

void TCPServer::AcceptThread()
{
	// ���� �ʱ�ȭ.

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		printf("socket()");
		return;
	}



	int retval;

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(this->ServerPort);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		printf("bind()"); return;
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		printf("listen()"); return;
	}

	while (true)
	{
		// accept ����
		SOCKET clinetSock = accept(listen_sock, NULL, NULL);
		if (clinetSock == INVALID_SOCKET) {
			printf("accept() error!");
			break;
		}

		acceptCount++;

		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);

		getpeername(clinetSock, (SOCKADDR*)&clientaddr, &addrlen);

		/*printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));*/





			// ���� ����
		Session* newSession = new Session();

		newSession->sock = clinetSock;
		newSession->mSessionId = g_session_id++;
		newSession->IO_COUNT = 0;
		WSABUF wsabuf[2];


		InitializeCriticalSection(&newSession->sessionLock);

		wsabuf[0].buf = newSession->recvRingBuffer.GetWriteBufferPtr();
		wsabuf[0].len = newSession->recvRingBuffer.GetNotBrokenPutSize();

		ZeroMemory(&newSession->recvOverlapped, sizeof(newSession->recvOverlapped));
		ZeroMemory(&newSession->sendOverlapped, sizeof(newSession->sendOverlapped));

		// CreateIoCompletionPort�� ���
		HANDLE handle = CreateIoCompletionPort((HANDLE)clinetSock, IOCP, (ULONG_PTR)newSession, NULL);
		if (handle == NULL)
		{
			printf("CreateIoCompletionPort() error by register socket!");
			break;
		}



		// ���� �ʿ� �߰�
		EnterCriticalSection(&g_session_map_cs);
		session_map.insert(std::pair<int, Session*>(newSession->mSessionId, newSession));
		LeaveCriticalSection(&g_session_map_cs);
		// overlapped ���� recv �ɾ� ��. 




		DWORD flags = 0;
		DWORD recvbytes;
		newSession->IO_COUNT++;
		int retVal = WSARecv(newSession->sock, wsabuf, 1, &recvbytes, &flags, &newSession->recvOverlapped, NULL);
		if (retVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				// ���� ���� ��ƾ...
				printf("ACCEPT THREAD RECV ERROR! %d\n", WSAGetLastError());
				InterlockedDecrement(&newSession->IO_COUNT);
				DeleteSession(newSession->mSessionId);
				continue;
			}
		}

		bool isAccpet = OnAccept(newSession->mSessionId, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		if (isAccpet == false)
		{
			DeleteSession(newSession->mSessionId);
			continue;
		}


	}

	return;

}


void TCPServer::WorkerThread()
{
	char recvBuffer[20000];

	int myThreadID = GetCurrentThreadId();

	bool start = false;
	int mStartTime = 0;         //�������ýð�
	int accumulateTime = 0;
	int tookTime = 0;

	while (true)
	{
		// printf("map size: %d\n", session_map.size());
		DWORD cbTransferred = 0;
		SOCKET clinet_sock = 0;
		Session* session = nullptr;
		// GQCS
		OVERLAPPED* overlapped = nullptr;
		int iocpret = GetQueuedCompletionStatus(IOCP, &cbTransferred, (PULONG_PTR)&session, (LPOVERLAPPED*)&overlapped, INFINITE);
		

		EnterCriticalSection(&printLock);
		if (start == false)
		{
			mStartTime = clock(); 
			start = true;
		}
		else
		{
			tookTime = clock() - mStartTime;	//�ɸ��ð�
			accumulateTime += tookTime;

			if (accumulateTime > 1000)
			{
			//	printf("TICK\n");
			//	accumulateTime = 0;
	
				accumulateTime = 0;
				start = false;
			}
		}

		LeaveCriticalSection(&printLock);



		if (myThreadID != GetCurrentThreadId())
		{
			printf("before Thread id: %d, current Thread id: %d\n", myThreadID, GetCurrentThreadId());

		}
		if (iocpret == 0)
		{
			//	printf("GQCS error! error code: %d\n", WSAGetLastError());

			if (session != nullptr)
			{
				InterlockedDecrement(&session->IO_COUNT);
				DeleteSession(session->mSessionId);
			}
			continue;

		}

		// OVERLAPPED == 0 �϶� ó��
		// OVERLAPPED�� null�̸�, session�� ��ȿ���� �ʴ�.
		// OVERLAPPED�� null�� ���
		// 1. IOCP ��ü ����
		// 2. timeout�� �� ���
		// 3. POST�� ���� �޽��� �־��� ��.
		if (overlapped == nullptr)
		{
			printf("overlapped: NULL, error: %d\n", WSAGetLastError());

			exit(-1);
		}

		if (cbTransferred == 0)
		{
			printf("r] cbTransferred == %d\n", cbTransferred);
			printf("session id: %d ����\n", session->mSessionId);
			// ���� ����
			InterlockedDecrement(&session->IO_COUNT);
			DeleteSession(session->mSessionId);
			continue;
		}

		else if (overlapped == &session->recvOverlapped)
		{

			// recv ó��
		//	printf("session id: %d, [recvByte: %d]\n", session->session_id, cbTransferred);
			recvByte += cbTransferred;
			session->recvRingBuffer.MoveWritePos(cbTransferred);

			int recvSize = session->recvRingBuffer.Get(recvBuffer, session->recvRingBuffer.GetUseSize());


			// ������ ------------------------------------------------------------------------------------------------

			// sned RingBuffer�� Recv�� �� �����͸� �ٷ� �� �ִ´�.
			//int sendSize = session->sendRingBuffer.Enqueue(recvBuffer, recvSize);
			//SendPacket(session->session_id, recvBuffer, recvSize);
			int processLen = OnRecv(session->mSessionId, recvBuffer, recvSize);

			// ������ ----------------------------------------------------------------------------------------------------

			// ó���ϰ� ���� ���� �ٽ� Recv ���ۿ� �־��ش�. 
			if (processLen < recvSize)
			{
				session->recvRingBuffer.Put((char*)recvBuffer + processLen, recvSize - processLen);
			}
			// Send�� ������. 

			SendPost(session->mSessionId);



			// Recv �ٽ� �ɾ��ش�.
			RecvPost(session->mSessionId);
			InterlockedDecrement(&session->IO_COUNT);
			if (session->IO_COUNT == 0)
			{
				DeleteSession(session->mSessionId);
			}
		}
		else if (overlapped == &session->sendOverlapped)
		{

			// send �Ϸ� ó��
		//	printf("session id: %d, [sendByte: %d]\n", session->session_id, cbTransferred);
			session->sendRingBuffer.Lock();
			int sendSize = session->sendRingBuffer.RemoveData(cbTransferred);
			//session->sendRingBuffer.UnLock();
			InterlockedDecrement(&session->IS_SEND);
			OnSend(session->mSessionId, cbTransferred);
			sendByte += cbTransferred;
			//printf("send RingBuffer ���� ũ��: %d\n", session->sendRingBuffer.GetUseSize());

			//session->sendRingBuffer.Lock();

			if (session->sendRingBuffer.GetUseSize() > 0)
			{
				session->sendRingBuffer.UnLock();
				SendPost(session->mSessionId);
			}
			else
			{
				session->sendRingBuffer.UnLock();
			}


			InterlockedDecrement(&session->IO_COUNT);


			if (session->IO_COUNT == 0)
			{
				DeleteSession(session->mSessionId);
			}

		}
		else
		{
			printf("key�� send�� recv�� 0�� �ƴϴ�.\n");
			break;
		}


	}

	return;
}

void TCPServer::RecvPost(int mSessionId)
{
	//	EnterCriticalSection(&g_sync_cs);

	EnterCriticalSection(&g_session_map_cs);
	auto element = session_map.find(mSessionId);
	if (element == session_map.end())
	{

		LeaveCriticalSection(&g_session_map_cs);
		//	LeaveCriticalSection(&g_sync_cs);
		return;
	}
	else
	{
		LeaveCriticalSection(&g_session_map_cs);
	}

	Session* session = element->second;

	WSABUF wsabuf[2];
	DWORD flags = 0;
	int retVal;

	wsabuf[0].buf = session->recvRingBuffer.GetWriteBufferPtr();
	wsabuf[0].len = session->recvRingBuffer.GetNotBrokenPutSize();


	wsabuf[1].buf = session->recvRingBuffer.begin;
	wsabuf[1].len = session->recvRingBuffer.GetFreeSize() - session->recvRingBuffer.GetNotBrokenPutSize();

	ZeroMemory(&session->recvOverlapped, sizeof(session->recvOverlapped));

	//printf("WSA SEND before\n");

	InterlockedIncrement(&session->IO_COUNT);

	retVal = WSARecv(session->sock, wsabuf, 2, NULL, &flags, &session->recvOverlapped, NULL);
	//  printf("WSA SEND after\n");

	  // ���� ó��, OVERLLAPED �߻� �� �ߴ�. ���� ���ۿ� �ִ� �͸� �ܾ�´�.
	if (retVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING) {
			InterlockedDecrement(&session->IO_COUNT);
			//	printf("WSARecv Error!\n");
			DeleteSession(session->mSessionId);
			//	LeaveCriticalSection(&g_sync_cs);
			return;
		}
	}
	//LeaveCriticalSection(&g_sync_cs);
}

void TCPServer::SendPost(int mSessionId)
{
	//	EnterCriticalSection(&g_sync_cs);

	EnterCriticalSection(&g_session_map_cs);
	auto element = session_map.find(mSessionId);
	if (element == session_map.end())
	{

		LeaveCriticalSection(&g_session_map_cs);
		//	LeaveCriticalSection(&g_sync_cs);
		return;
	}
	else
	{
		LeaveCriticalSection(&g_session_map_cs);
	}


	Session* session = element->second;

	session->sendRingBuffer.Lock();


	int ret = InterlockedIncrement(&session->IS_SEND);

	if (ret >= 2)
	{
		InterlockedDecrement(&session->IS_SEND);
		//	LeaveCriticalSection(&g_session_map_cs);
		//	LeaveCriticalSection(&g_sync_cs);
		session->sendRingBuffer.UnLock();
		return;
	}


	InterlockedIncrement(&session->IO_COUNT);

	WSABUF wsabuf[2];
	DWORD flags = 0;
	int retVal;
	DWORD sendbyte = 0;

	//	session->sendRingBuffer.Lock();



	wsabuf[0].buf = session->sendRingBuffer.GetReadBufferPtr();
	wsabuf[0].len = session->sendRingBuffer.GetNotBrokenGetSize();


	wsabuf[1].buf = session->sendRingBuffer.begin;
	wsabuf[1].len = session->sendRingBuffer.GetUseSize() - session->sendRingBuffer.GetNotBrokenGetSize();

	ZeroMemory(&session->sendOverlapped, sizeof(session->sendOverlapped));

	if (session->sendRingBuffer.GetUseSize() == 0)
	{
		//printf("SendPost ���� GetUseSize�� zero!\n");
		InterlockedDecrement(&session->IS_SEND);
		InterlockedDecrement(&session->IO_COUNT);
		session->sendRingBuffer.UnLock();
		return;
	}


	//printf("WSA SEND before\n");


	retVal = WSASend(session->sock, wsabuf, 2, &sendbyte, 0, &session->sendOverlapped, NULL);
	//  printf("WSA SEND after\n");
	session->sendRingBuffer.UnLock();
	// ���� ó��, OVERLLAPED �߻� �� �ߴ�. ���� ���ۿ� �ִ� �͸� �ܾ�´�.
	if (retVal == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING) {
			InterlockedDecrement(&session->IO_COUNT);
			InterlockedDecrement(&session->IS_SEND);
			//	printf("WSARecv Error!\n");
			DeleteSession(session->mSessionId);
			//	LeaveCriticalSection(&g_sync_cs);
			return;
		}
	}

	//	LeaveCriticalSection(&g_sync_cs);
}

int TCPServer::SendPacket(int mSessionId, char* messsage, int size)
{
	//	EnterCriticalSection(&g_sync_cs);

	EnterCriticalSection(&g_session_map_cs);
	auto element = session_map.find(mSessionId);
	if (element == session_map.end())
	{

		LeaveCriticalSection(&g_session_map_cs);
		//	LeaveCriticalSection(&g_sync_cs);
		return 0;
	}
	else
	{
		LeaveCriticalSection(&g_session_map_cs);
	}


	Session* session = element->second;

	session->sendRingBuffer.Lock();
	session->sendRingBuffer.Put(messsage, size);

	session->sendRingBuffer.UnLock();

	SendPost(session->mSessionId);
	return size;
}

void TCPServer::DeleteSession(int mSessionId)
{
	EnterCriticalSection(&g_session_map_cs);
	auto element = session_map.find(mSessionId);
	if (element == session_map.end())
	{

		LeaveCriticalSection(&g_session_map_cs);
		return;
	}


	Session* delSession = element->second;
	if (delSession->IO_COUNT > 0)
	{
		LeaveCriticalSection(&g_session_map_cs);
		return;
	}

	//	printf("Ŭ���̾�Ʈ ���� session id: %d\n", delSession->session_id);

		// ���� ����
	if (delSession->sock != -1)
		closesocket(delSession->sock);


	//���� �ʿ��� ����
	session_map.erase(element->first);


	// ���� ��ü�� ����
	delete delSession;
	LeaveCriticalSection(&g_session_map_cs);



	OnDisconnet(mSessionId);


	//delete delSession;

	//OnDisconnet(session_id);
}

void TCPServer::ForceDeleteSession(int mSessionId)
{

	EnterCriticalSection(&g_session_map_cs);
	auto element = session_map.find(mSessionId);
	if (element == session_map.end())
	{

		LeaveCriticalSection(&g_session_map_cs);
		return;
	}


	Session* delSession = element->second;

	closesocket(delSession->sock);
	delSession->sock = -1;
	LeaveCriticalSection(&g_session_map_cs);
}



int TCPServer::Run(int serverPort, int workerThreadNumber)
{
	this->ServerPort = serverPort;
	this->WorkerThreadNumber = workerThreadNumber;

	
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;
	InitializeCriticalSection(&printLock);
	InitializeCriticalSection(&g_session_map_cs);
	//InitializeCriticalSection(&g_sync_cs);

	// ������ ��� ����
	IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (IOCP == NULL)
	{
		printf("create iocp error!\n");
		return -1;
	}
	//DWORD threadID;
	DWORD dwThrdParam = 1;
	// Accept �������, worker thread�� ������ ���� �̺�Ʈ ��ü ����

	//workerThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AcceptThread, &dwThrdParam, 0, &threadID);
	thread[0] = std::thread(&TCPServer::AcceptThread, this);
	//if (thread[0] == 0)
	//{
	//	printf("worker ������ ���� ����!\n");
	//	return -1;
	//}

	for (int i = 1; i <= this->WorkerThreadNumber; i++)
	{
		//workerThread[i] = CreateThread(NULL, 0, WorkerThread, &dwThrdParam, 0, &threadID);
		thread[i] = std::thread(&TCPServer::WorkerThread, this);
		/*	if (workerThread[i] == 0)
			{
				printf("������ ���� ����!\n");
				return -1;
			}*/
	}




	//int ret = WaitForMultipleObjects(this->WorkerThreadNumber + 1, workerThread, TRUE, INFINITE);

	/*if (ret == WAIT_OBJECT_0) {
		printf("hThrd : the kernel object is signaled\n");
	}
	else if (ret == WAIT_TIMEOUT) {
		printf("hThrd : the timeout interval elapsed, and the object's state is nonsignaled\n");
	}*/

	for (int i = 0; i <= this->WorkerThreadNumber; i++)
	{
		thread[i].join();
	}



	return 0;

}


