#pragma once
#include <list>
#include "Player.h"
#include "Packet.h"
#include <mutex>
#include <vector>
#include "SerializeBuffer.h"


class SectorManager
{
public:
	SectorManager(std::function<void(int, char*, int)> sendPacket, std::shared_ptr<PlayerManager> playerManager)
	{
		mSendPacket = sendPacket;
		mPlayerManager = playerManager;
	}

	int FindSectorX(int xPos)
	{
		return xPos / dfSECTOR_ONE_SIZE_X;
	}
	// Y 좌표의 섹터를 찾는다.
	// 인수 yPos
	int FindSectorY(int yPos)
	{
		return yPos / dfSECTOR_ONE_SIZE_Y;
	}
	bool AddPlayerToSector(Player* p, int Sector_X, int Sector_Y)
	{
		std::lock_guard<std::mutex> guard(mPacketManagerMutex);
		g_Player_Sector[Sector_X][Sector_Y].push_back(p);
		return true;
	}

	bool DeletePlayerToSector(Player* p, int Sector_X, int Sector_Y)
	{
		std::lock_guard<std::mutex> guard(mPacketManagerMutex);
		g_Player_Sector[Sector_X][Sector_Y].remove(p);

		return true;
	}
	// 해당 플레이어의 주변 섹터 3*3의 플레이어들을 반환
	void GetAroundSectorPlayer(int PlayerId, std::vector<Player*>* vector)
	{
		std::lock_guard<std::mutex> sm(mPacketManagerMutex);
		Player* player = mPlayerManager->FindByPlayerId(PlayerId);
		SectorAry sectorAry;
		GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);

		for (int count = 0; count < sectorAry.Count; count++)
		{
			for (auto tempPlayer : g_Player_Sector[sectorAry.ary[count].SectorXPos][sectorAry.ary[count].SectorYPos])
			{
				vector->push_back(tempPlayer);
			}
		}
	}

	void SendSector(int sectorX, int sectorY, char* packet, int length)
	{
		std::lock_guard<std::mutex> sm(mPacketManagerMutex);
		for (auto tempPlayer : g_Player_Sector[sectorX][sectorY])
		{
			mSendPacket(tempPlayer->mSessionId, packet, length);
		}
	}

	void SendSectorAry(SectorAry* sectorAry, char* packet, int length, int exceptId)
	{
		std::lock_guard<std::mutex> sm(mPacketManagerMutex);
		for (int count = 0; count < sectorAry->Count; count++)
		{
			for (auto tempPlayer : g_Player_Sector[sectorAry->ary[count].SectorXPos][sectorAry->ary[count].SectorYPos])
			{
				if(exceptId != tempPlayer->mSessionId)
					mSendPacket(tempPlayer->mSessionId, packet, length);
			}
		}
	}

	void SectorAroundPlayer(SectorAry* sectorAry, std::vector<Player*>* vector)
	{
		std::lock_guard<std::mutex> sm(mPacketManagerMutex);
		for (int count = 0; count < sectorAry->Count; count++)
		{
			for (auto tempPlayer : g_Player_Sector[sectorAry->ary[count].SectorXPos][sectorAry->ary[count].SectorYPos])
			{
				vector->push_back(tempPlayer);
			}
		}
	}

	void SendPlayerSectorAry(int playerId, char* packet, int length, bool sendMe)
	{
		Player* player = mPlayerManager->FindByPlayerId(playerId);
		SectorAry sectorAry;
		GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);
		if(sendMe == true)
			SendSectorAry(&sectorAry, packet, length, -1);
		else
			SendSectorAry(&sectorAry, packet, length, playerId);
	}
	
	void GetSectorAround(int Sector_X, int Sector_Y, SectorAry* pSectorAround)
	{
		// [-1][-1] [-1][0]  [-1][1]
		// [0][-1] [0][0]  [0][1]
		// [1][-1] [1][0]  [1][1]
		pSectorAround->Count = 0;

		for (int SectorX = -1; SectorX <= 1; SectorX++)
		{
			for (int SectorY = -1; SectorY <= 1; SectorY++)
			{
				if (Sector_X + SectorX < 0 || Sector_X + SectorX >= dfSECTOR_MAX_X
					|| Sector_Y + SectorY < 0 || Sector_Y + SectorY >= dfSECTOR_MAX_Y)
				{
					continue;
				}
				else
				{

					pSectorAround->ary[pSectorAround->Count].SectorXPos = Sector_X + SectorX;
					pSectorAround->ary[pSectorAround->Count].SectorYPos = Sector_Y + SectorY;
					pSectorAround->Count++;
				}

			}

		}

		return;


	}

	bool SectorUpdate(Player* p, SectorAry* newSector, SectorAry* delSector)
	{
		int SectorX = FindSectorX(p->X);
		int SectorY = FindSectorY(p->Y);

		newSector->Count = 0;
		delSector->Count = 0;

		if (SectorX == p->curSectorXPos && SectorY == p->curSectorYPos)
		{

			return false;
		}

		GetSectorAround(p->curSectorXPos, p->curSectorYPos, &p->oldSectorAry);
		GetSectorAround(SectorX, SectorY, &p->newSectorAry);

		// 새로 생성된 섹터


		DeletePlayerToSector(p, p->curSectorXPos, p->curSectorYPos);

		p->curSectorXPos = SectorX;
		p->curSectorYPos = SectorY;

		AddPlayerToSector(p, p->curSectorXPos, p->curSectorYPos);

		for (int newSectorCount = 0; newSectorCount < p->newSectorAry.Count; newSectorCount++)
		{
			for (int oldSectorCount = 0; oldSectorCount < p->oldSectorAry.Count; oldSectorCount++)
			{
				if (p->newSectorAry.ary[newSectorCount].SectorXPos == p->oldSectorAry.ary[oldSectorCount].SectorXPos
					&& p->newSectorAry.ary[newSectorCount].SectorYPos == p->oldSectorAry.ary[oldSectorCount].SectorYPos)
				{
					break;
				}

				if (oldSectorCount == p->oldSectorAry.Count - 1)
				{

					newSector->ary[newSector->Count] = p->newSectorAry.ary[newSectorCount];
					newSector->Count++;
				}
			}
		}

		// 삭제된 섹터

		for (int oldSectorCount = 0; oldSectorCount < p->oldSectorAry.Count; oldSectorCount++)
		{
			for (int newSectorCount = 0; newSectorCount < p->newSectorAry.Count; newSectorCount++)
			{
				if (p->newSectorAry.ary[newSectorCount].SectorXPos == p->oldSectorAry.ary[oldSectorCount].SectorXPos
					&& p->newSectorAry.ary[newSectorCount].SectorYPos == p->oldSectorAry.ary[oldSectorCount].SectorYPos)
				{
					break;
				}

				if (newSectorCount == p->newSectorAry.Count - 1)
				{

					delSector->ary[delSector->Count] = p->oldSectorAry.ary[oldSectorCount];
					delSector->Count++;
				}
			}
		}


		return true;
	}


private:
	std::list<Player*> g_Player_Sector[dfSECTOR_MAX_X][dfSECTOR_MAX_Y];
	std::shared_ptr<PlayerManager> mPlayerManager;
	std::function<void(int, char*, int)> mSendPacket;
	std::mutex mPacketManagerMutex;
};