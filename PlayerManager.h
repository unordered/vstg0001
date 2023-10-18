#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include "Player.h"


class PlayerManager
{
public:
	void Init()
	{

	}

	// 플레이어 아이디로 플레이어 찾기
	Player* FindByPlayerId(int playerId)
	{
		std::lock_guard<std::mutex> lc(mPlayerManagerMutex);
		auto player = mPlayerMap.find(playerId);
		if (player != mPlayerMap.end())
		{
			return player->second;
		}
		else
		{
			return nullptr;
		}
	}

	// 플레이어 생성 후, 플레이어맵에 자동 추가
	// 반환 값: 생성된 플레이어
	Player* CreatePlayer()
	{

		std::lock_guard<std::mutex> lc(mPlayerManagerMutex);
		Player* p = new Player(mPlayerIdIncrement++);
		p->mPlayerId = mPlayerIdIncrement - 1;
		mPlayerMap.insert(std::pair<int, Player*>(p->mSessionId, p));
		return p;
	}

	// 플레이어 삭제
	size_t RemovePlayer(int PlayerId)
	{

		std::lock_guard<std::mutex> lc(mPlayerManagerMutex);
		auto player = mPlayerMap.find(PlayerId);
		if (player != mPlayerMap.end())
		{
			delete player->second;
			return mPlayerMap.erase(PlayerId);
		}
	 
	
		
		return 0;
	}

	std::unordered_map<int, Player*>* getPlayerMap()
	{

		std::lock_guard<std::mutex> lc(mPlayerManagerMutex);
		return &mPlayerMap;
	}

private:

	// 플레이어 ID, 플레이어
	std::unordered_map<int, Player*> mPlayerMap; 
	int mPlayerIdIncrement = 0;
	std::mutex mPlayerManagerMutex;
};
