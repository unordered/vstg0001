#pragma once
#include "TCPServer.h"
#include "PacketManager.h"
#include <cstdio>


class TCPFighterServer : public TCPServer
{
public:
	virtual int OnRecv(int sessionId, char* recvBuffer, int length)
	{
		mPacketManager->Process(sessionId, recvBuffer, length);
		return length;
	}

	virtual void OnSend(int sessionId, int length)
	{
	//	printf("session id: %d, send length: %d \n", sessionId, length);
		
		return;
	}

	virtual bool OnAccept(int sessionId, char* ip, int port)
	{
		mPacketManager->Connect(sessionId);
		return true;
	}
	virtual void OnDisconnet(int sessionId)
	{
		mPacketManager->DisConnect(sessionId);
		return ;
	}


	void LogicFrame(int serverPort, int workerThreadNumber)
	{
		auto sendPacketFunc = [&](int sessionId,  char* packet, int length)
		{
			SendPacket(sessionId, packet, length);
		};

		auto deleteSession = [&](int sessionId)
		{
			ForceDeleteSession(sessionId);
			//DeleteSession(sessionId);
		};

		auto serverState = [&](int sec_sendByte, int sec_recvByte, int accept_count)
		{
			OnServerState(sec_sendByte, sec_recvByte, accept_count);
		};
		
		mPacketManager = std::make_unique<PacketManager>();

		
		mPacketManager->mSendPacket = sendPacketFunc;
		mPacketManager->mDisconnectSession = deleteSession;
		mPacketManager->mServerState = serverState;
		mPacketManager->Init();
		mPacketManager->SetPrintVarible(&sendByte, &recvByte, &acceptCount);
		mLogicThread = std::thread(&PacketManager::LogicFrame, mPacketManager.get(), 50);
	
		Run(serverPort, workerThreadNumber);

	}

	virtual void OnServerState(int sec_sendByte, int sec_recvByte, int accept_count)
	{
		
	//	printf("[Network I/O]\nNetwork Time Send: %dbyte/s\nNetwork Time Send: %dbyte/s\nsec accept:%d\n", sec_sendByte, sec_recvByte,accept_count);

	}


private:

	std::thread mLogicThread; 
	std::unique_ptr<PacketManager> mPacketManager = nullptr;

};