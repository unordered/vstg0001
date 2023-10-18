#pragma once
#include "PlayerManager.h"
#include <memory>
#include <mutex>
#include <functional>
#include "SectorManager.h"
#include "SerializeBuffer.h"
// ������ ģȭ��
class PacketManager
{
public:
	// �������� ������
	void LogicFrame(int );
	// ��Ŷ ������ Ÿ�Ե��� ������ش�.
	void Init();
	// ��Ŷ�� ó�����ش�.
	int Process(int sessionId, char* packet, int Recvlength);
	void Connect(int sessionId);
	void DisConnect(int sessionId);


	void PrintServerLogicState(int sessionCount, int minFps, int syncCount, int& accumulateFrame);
	void SetPrintVarible(int* sendByte, int* recvByte, int* accpetCount);
	// ��Ŷ ���� �Լ�
	// �Ű�����: (���̵�, ��Ŷ, ����)
	std::function<void(int, char*, int)> mSendPacket;
	// ���� ���� �Լ�
	// �Ű�����: (���Ǿ��̵�)
	std::function<void(int)> mDisconnectSession;
	// ���� ���� ���� �Լ�
	std::function<void(int, int ,int)> mServerState;

	// ������ ��Ŷ ó�� �Լ�
	bool Process_dfPACKET_CS_MOVE_START(int playerId, char* pPacket, int length);
	bool Process_dfPACKET_CS_MOVE_STOP(int playerId, char* pPacket, int length);
	bool Process_dfPACKET_CS_ATTACK1(int playerId, char* pPacket, int length);
	bool Process_dfPACKET_CS_ATTACK2(int playerId, char* pPacket, int length);
	bool Process_dfPACKET_CS_ATTACK3(int playerId, char* pPacket, int length);
	bool Process_dfPACKET_CS_ECHO(int playerId, char* pPacket, int length);

	// ����->Ŭ���̾�Ʈ�� ��Ŷ �����
	void MAKE_PACKET_SC_CREATE_MY_CHARACTER(SerializeBuffer* buffer, int id, char Dir, short X, short Y, char HP);
	void MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(SerializeBuffer* buffer, int id, char Dir, short X, short Y, char HP);
	void MAKE_PACKET_SC_DELETE_CHARACTER(SerializeBuffer* buffer, int id);
	void MAKE_PAKCET_dfPACKET_SC_ATTACK1(SerializeBuffer* buffer, int id, char Dir, short X, short Y);
	void MAKE_PAKCET_dfPACKET_SC_ATTACK2(SerializeBuffer* buffer, int id, char Dir, short X, short Y);
	void MAKE_PAKCET_dfPACKET_SC_ATTACK3(SerializeBuffer* buffer, int id, char Dir, short X, short Y);
	void MAKE_PACKET_dfPACKET_SC_MOVE_START(SerializeBuffer* buffer, int id, char Dir, short X, short Y);
	void MAKE_PACKET_dfPACKET_SC_MOVE_STOP(SerializeBuffer* buffer, int id, char Dir, short X, short Y);
	void MAKE_PACKET_dfPACKET_SC_DAMAGE(SerializeBuffer* buffer, int id, int target_id, char target_hp);
	void MAKE_PACKET_dfPACKET_SC_SYNC(SerializeBuffer* buffer, int id, short X, short Y);


	// ������ ó�� �Լ�
	size_t RemovePlayer(int session_id);





private:
	std::shared_ptr<PlayerManager> mPlayerManager;
	std::unique_ptr<SectorManager> mSectorManager;

	std::unique_ptr<SerializeBuffer> mConnectSerializeBuffer;
	std::mutex mPacketManagerMutex;
	int* mNetworkSend;
	int* mNetworkRecv;
	int* mAcceptCount;
	int syncCount=0;

};