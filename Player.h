#pragma once
#include <ctime>
#include "Packet.h"
#include "Content.h"

class Player
{
public:
	Player(int sessionId)
		:mSessionId(sessionId)
	{

	}
	int mSessionId;
	int mPlayerId;
	bool disconnect;

	bool move;
	char before_dir; // �����̳� ���Ʒ� �̵��� �ʿ��� ����
	char Dir;
	short X;
	short Y;
	char HP;

	time_t lastRecvTime;


	SectorAry oldSectorAry;
	SectorAry newSectorAry;

	int curSectorXPos;
	int curSectorYPos;


};
