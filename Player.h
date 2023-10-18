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
	char before_dir; // 공격이나 위아래 이동에 필요한 방향
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
