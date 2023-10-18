#include "PacketManager.h"
#include "Packet.h"
#include "Content.h"
#include "FPS.h"



void PacketManager::LogicFrame(int serverSetFps)
{
	Fps fps;
	int serverfps = serverSetFps;
	SerializeBuffer tempBuffer;
	std::vector<Player*> tempPlayerVector;

	int accumulateFrame = 0;
	while (1)
	{
		accumulateFrame++;
		fps.Update();
		bool wait = fps.Wait();
		if (wait == true)
		{
			continue;
		}
		fps.FrameIncrement();
		// fps.Draw();
		time_t curTime = time(NULL);
		SectorAry newSector;
		SectorAry oldSector;

		
		PrintServerLogicState(mPlayerManager->getPlayerMap()->size(), fps.getmFps(), (int)syncCount, accumulateFrame);

			//for (int i = 0; i < g_Player_vector.size(); i++)
		   // for(auto Player : mPlayerManager->getPlayerMap())
			for (auto player = mPlayerManager->getPlayerMap()->begin(); player != mPlayerManager->getPlayerMap()->end(); ++player) {
			{
					Player* g_Player_vector[1];
					g_Player_vector[0] = player->second;
					int i=0;

				if ((int)curTime - (int)g_Player_vector[i]->lastRecvTime > (int)30)
				{
		//			g_Player_vector[i]->disconnect = true;
		//			continue;
				}


				if (g_Player_vector[i]->move == true && g_Player_vector[i]->disconnect == false)
				{
					{
						if ((g_Player_vector[i]->X >= dfRANGE_MOVE_RIGHT - 1 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_RR) ||
							(g_Player_vector[i]->X >= dfRANGE_MOVE_RIGHT - 1 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_RU) ||
							(g_Player_vector[i]->X >= dfRANGE_MOVE_RIGHT - 1 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_RD)
							)
						{
							g_Player_vector[i]->X = dfRANGE_MOVE_RIGHT - 1;
							continue;
						}
						if ((g_Player_vector[i]->Y >= dfRANGE_MOVE_BOTTOM - 1 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_DD) ||
							(g_Player_vector[i]->Y >= dfRANGE_MOVE_BOTTOM - 1 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_LD) ||
							(g_Player_vector[i]->Y >= dfRANGE_MOVE_BOTTOM - 1 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_RD))
						{
							g_Player_vector[i]->Y = dfRANGE_MOVE_BOTTOM - 1;
							continue;
						}

						if ((g_Player_vector[i]->Y <= dfRANGE_MOVE_TOP && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_UU) ||
							(g_Player_vector[i]->Y <= dfRANGE_MOVE_TOP && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_LU) ||
							(g_Player_vector[i]->Y <= dfRANGE_MOVE_TOP && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_RU))
						{
							g_Player_vector[i]->Y = dfRANGE_MOVE_TOP;
							continue;
						}

						if ((g_Player_vector[i]->X <= dfRANGE_MOVE_LEFT + 3 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_LL) ||
							(g_Player_vector[i]->X <= dfRANGE_MOVE_LEFT + 3 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_LD) ||
							(g_Player_vector[i]->X <= dfRANGE_MOVE_LEFT + 3 && g_Player_vector[i]->Dir == dfPACKET_MOVE_DIR_LU))
						{
							g_Player_vector[i]->X = dfRANGE_MOVE_LEFT + 3;
							continue;
						}

					//		printf("[MOVE] ID: %d, [%d, %d]\n", g_Player_vector[i]->mSessionId, g_Player_vector[i]->X, g_Player_vector[i]->Y);
						switch (g_Player_vector[i]->Dir)
						{
						case dfPACKET_MOVE_DIR_LL:
							g_Player_vector[i]->X -= dfSPEED_PLAYER_X;
							break;
						case dfPACKET_MOVE_DIR_LU:
							g_Player_vector[i]->X -= dfSPEED_PLAYER_X;
							g_Player_vector[i]->Y -= dfSPEED_PLAYER_Y;
							break;
						case dfPACKET_MOVE_DIR_UU:

							g_Player_vector[i]->Y -= dfSPEED_PLAYER_Y;
							break;
						case dfPACKET_MOVE_DIR_RU:
							g_Player_vector[i]->X += dfSPEED_PLAYER_X;
							g_Player_vector[i]->Y -= dfSPEED_PLAYER_Y;
							break;
						case dfPACKET_MOVE_DIR_RR:
							g_Player_vector[i]->X += dfSPEED_PLAYER_X;

							break;
						case dfPACKET_MOVE_DIR_RD:
							g_Player_vector[i]->X += dfSPEED_PLAYER_X;
							g_Player_vector[i]->Y += dfSPEED_PLAYER_Y;
							break;
						case dfPACKET_MOVE_DIR_DD:
							g_Player_vector[i]->Y += dfSPEED_PLAYER_Y;

							break;
						case dfPACKET_MOVE_DIR_LD:
							g_Player_vector[i]->X -= dfSPEED_PLAYER_X;
							g_Player_vector[i]->Y += dfSPEED_PLAYER_Y;
							break;
						default:
							break;
						}

						if (g_Player_vector[i]->X >= dfRANGE_MOVE_RIGHT)
						{
							g_Player_vector[i]->X = dfRANGE_MOVE_RIGHT - 1;
						}
						if (g_Player_vector[i]->X < 0)
						{
							g_Player_vector[i]->X = 0;
						}

						if (g_Player_vector[i]->Y < dfRANGE_MOVE_TOP)
						{
							g_Player_vector[i]->Y = 0;
						}
						if (g_Player_vector[i]->Y >= dfRANGE_MOVE_BOTTOM)
						{
							g_Player_vector[i]->Y = dfRANGE_MOVE_BOTTOM - 1;
						}
					}

					// 섹터를 다시 구하고, 섹터가 변경 됐으면 함수 호출!


			//		printf("현재 섹터[%2d, %2d]\n", g_Player_vector[i]->curSectorXPos, g_Player_vector[i]->curSectorYPos);

					bool isUpdate = false;
					if (g_Player_vector[i]->disconnect == false)
						isUpdate = mSectorManager->SectorUpdate(g_Player_vector[i], &newSector, &oldSector);



					//size_t playerCount = 0;
					//// 섹터내의 전체 숫자
					//for (int sectorX = 0; sectorX < dfSECTOR_MAX_X; sectorX++)
					//{
					//	for (int sectorY = 0; sectorY < dfSECTOR_MAX_Y; sectorY++)
					//	{
					//		playerCount += g_Player_Sector[sectorX][sectorY].size();
					//	}
					//}

					//		printf("섹터 내의 전체 유저 숫자: %d\n", playerCount);


					if (isUpdate)
					{




						// 1. 내 캐릭터에게 삭제되는 섹터에 존재하던 플레이어들 삭제 패킷
				/*		for (int count = 0; count < oldSector.Count; count++)
						{
							for (auto p : g_Player_Sector[oldSector.ary[count].SectorXPos][oldSector.ary[count].SectorYPos])
							{
								if (p->session_id != g_Player_vector[i]->session_id)
								{
									MAKE_PACKET_SC_DELETE_CHARACTER(tempBuffer, p->session_id);
									SendPlayer(g_Player_vector[i]->session_id, tempBuffer);
								}


							}
						}*/

						// TODO
						
						tempPlayerVector.clear();
					/*	for (int count = 0; count < oldSector.Count; count++)
						{*/
							mSectorManager->SectorAroundPlayer(&oldSector, &tempPlayerVector);
							for (auto p : tempPlayerVector)
							{
								if (p->mSessionId != g_Player_vector[0]->mSessionId)
								{
									MAKE_PACKET_SC_DELETE_CHARACTER(&tempBuffer, p->mSessionId);
									mSendPacket(g_Player_vector[0]->mSessionId, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize());
					

								}
							}
					//	}


		/*				SectorAry sectorAry;
						mSectorManager->GetSectorAround(g_Player_vector[i]->curSectorXPos, g_Player_vector[i]->curSectorYPos, &sectorAry);

						std::vector<Player*> playerList;
						mSectorManager->GetAroundSectorPlayer(g_Player_vector[i]->mPlayerId, &playerList);

						for (auto p : playerList)
						{
							if (p->mSessionId != g_Player_vector[i]->mSessionId)
							{
								MAKE_PACKET_SC_DELETE_CHARACTER(mSerializeBuffer.get(), p->mSessionId);
								mSendPacket(g_Player_vector[i]->mSessionId, mSerializeBuffer->GetBufferPtr(), mSerializeBuffer->GetDataSize());
							}
						}*/



						// 2. 삭제되는 섹터의 플레이어들에게 내 캐릭터의 삭제 패킷


				/*		MAKE_PACKET_SC_DELETE_CHARACTER(tempBuffer, g_Player_vector[i]->session_id);

						SendSector(g_Player_vector[i]->session_id, tempBuffer, &oldSector, false);*/

						MAKE_PACKET_SC_DELETE_CHARACTER(&tempBuffer, g_Player_vector[i]->mSessionId);
						mSectorManager->SendSectorAry(&oldSector, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), g_Player_vector[i]->mSessionId);



						// 3. 내 캐릭터에게 생성되는 섹터의 플레이어들 생성 패킷
						//for (int count = 0; count < newSector.Count; count++)
						//{
						//	for (auto p : g_Player_Sector[newSector.ary[count].SectorXPos][newSector.ary[count].SectorYPos])
						//	{
						//		MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(tempBuffer, p->session_id, p->Dir, p->X, p->Y, p->HP);
						//		SendPlayer(g_Player_vector[i]->session_id, tempBuffer);

						//		// 3-1. 내 캐릭터에게 생성되는 섹터의 플레이어들 이동 패킷 
						//		if (p->move)
						//		{
						//			MAKE_PACKET_dfPACKET_SC_MOVE_START(tempBuffer, p->session_id, p->before_dir, p->X, p->Y);
						//			SendPlayer(g_Player_vector[i]->session_id, tempBuffer);

						//		}
						//	}


						//}

						
						tempPlayerVector.clear();
						mSectorManager->SectorAroundPlayer(&newSector, &tempPlayerVector);

						for (auto tempP : tempPlayerVector)
						{
							MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(&tempBuffer, tempP->mSessionId, tempP->Dir, tempP->X, tempP->Y, tempP->HP);
							mSendPacket(g_Player_vector[i]->mSessionId, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize());

							// 3-1. 내 캐릭터에게 생성되는 섹터의 플레이어들 이동 패킷 
							if (tempP->move)
							{
								MAKE_PACKET_dfPACKET_SC_MOVE_START(&tempBuffer, tempP->mSessionId, tempP->before_dir, tempP->X, tempP->Y);
								mSendPacket(g_Player_vector[i]->mSessionId, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize());

							}
						}
					



						// 4. 다른 캐릭터에게 생성되는 내 캐릭터의 생성 패킷

				//		printf("dir: %d, before dir: %d\n", g_Player_vector[i]->Dir,  g_Player_vector[i]->before_dir);
			/*			MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(tempBuffer, g_Player_vector[i]->session_id, g_Player_vector[i]->before_dir,
							g_Player_vector[i]->X, g_Player_vector[i]->Y, g_Player_vector[i]->HP);

						SendSector(g_Player_vector[i]->session_id, tempBuffer, &newSector, false);*/

						MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(&tempBuffer, g_Player_vector[i]->mSessionId, g_Player_vector[i]->before_dir,
							g_Player_vector[i]->X, g_Player_vector[i]->Y, g_Player_vector[i]->HP);


						mSectorManager->SendSectorAry(&newSector, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), g_Player_vector[i]->mPlayerId);

						// 4-1. 새로 생성되는 섹터의 캐릭터에게 내 캐릭터의 이동 패킷 

						//MAKE_PACKET_dfPACKET_SC_MOVE_START(tempBuffer, g_Player_vector[i]->session_id, g_Player_vector[i]->Dir,
						//	g_Player_vector[i]->X, g_Player_vector[i]->Y);
						//SendSector(g_Player_vector[i]->session_id, tempBuffer, &newSector, false);

						MAKE_PACKET_dfPACKET_SC_MOVE_START(&tempBuffer, g_Player_vector[i]->mSessionId, g_Player_vector[i]->Dir,
						g_Player_vector[i]->X, g_Player_vector[i]->Y);

						mSectorManager->SendSectorAry(&newSector, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), g_Player_vector[i]->mPlayerId);


					}


				}
			}

		}

		// 제거
		{
			//홀수일 경우 삭제
		

				for (auto it = mPlayerManager->getPlayerMap()->begin(); it != mPlayerManager->getPlayerMap()->end();) {


					//}
					//for (auto it = mPlayerManager->getPlayerMap().begin(); it != mPlayerManager->getPlayerMap().end();) {
					//	//iterator는 원소값을 가리키는 포인터

						if ((it->second)->disconnect == true) {
							//		OnDisconnet((*it)->session_id);
								//	DeleteSession((*it)->session_id);
							//(*it)->disconnect = false;
							
							auto before = it;
							auto afterIt = ++it;
							size_t removeCount = RemovePlayer((before->second)->mSessionId);
							
							if (removeCount != 0)
							{
								it = afterIt;
								
							}
							


						}
						else {
							//지우지 않을 경우 다음 포인터로
							it++;
						}



					//}
				}
		}
	}
}


void PacketManager::Init()
{
	mPlayerManager = std::make_shared<PlayerManager>();
	mConnectSerializeBuffer = std::make_unique<SerializeBuffer>();
	mSectorManager = std::make_unique<SectorManager>(mSendPacket,mPlayerManager);
	
}


// 리턴 값: 처리한 크기
int PacketManager::Process(int sessionId, char* packet, int Recvlength)
{
	//std::lock_guard<std::mutex> lc(mMutex);
	int restLen = Recvlength;
	while (restLen >= sizeof(PACKET_HEADER))
	{
		Player* player = mPlayerManager->FindByPlayerId(sessionId);
		
		if (player == nullptr)
		{
			return Recvlength - restLen;
		}
		player->lastRecvTime = time(NULL);
		PACKET_HEADER* header = (PACKET_HEADER*)packet;

		if (header->byCode != 0x89)
		{
			DisConnect(sessionId);
		}
		else
		{
			switch (header->byType)
			{
			case dfPACKET_CS_MOVE_START:
				Process_dfPACKET_CS_MOVE_START(player->mSessionId, packet, header->bySize);
				restLen -= header->bySize+sizeof(PACKET_HEADER);
				break;
			case dfPACKET_CS_MOVE_STOP:
				Process_dfPACKET_CS_MOVE_STOP(player->mSessionId, packet, header->bySize);
				restLen -= header->bySize + sizeof(PACKET_HEADER);;
				break;
			case dfPACKET_CS_ATTACK1:
				Process_dfPACKET_CS_ATTACK1(player->mSessionId, packet, header->bySize);
				restLen -= header->bySize + sizeof(PACKET_HEADER);;
				break;
			case dfPACKET_CS_ATTACK2:
				Process_dfPACKET_CS_ATTACK2(player->mSessionId, packet, header->bySize);
				restLen -= header->bySize + sizeof(PACKET_HEADER);;
				break;
			case dfPACKET_CS_ATTACK3:
				Process_dfPACKET_CS_ATTACK3(player->mSessionId, packet, header->bySize);
				restLen -= header->bySize + sizeof(PACKET_HEADER);;
				break;
			case dfPACKET_CS_ECHO:
				restLen -= header->bySize + sizeof(PACKET_HEADER);
				break;
			default:
				restLen -= header->bySize + sizeof(PACKET_HEADER);
				
				break;
			}

		}
	}

	return Recvlength-restLen;

}

void PacketManager::Connect(int sessionId)
{


	std::lock_guard<std::mutex> lm(mPacketManagerMutex);
	Player* newPlayer = mPlayerManager->CreatePlayer();
	if (rand() % 2 == 0)
	{
		newPlayer->Dir = dfPACKET_MOVE_DIR_LL;
	}
	else
	{
		newPlayer->Dir = dfPACKET_MOVE_DIR_RR;
	}

	newPlayer->before_dir = LLorRR(newPlayer->Dir);
	newPlayer->HP = 100;
	newPlayer->X = rand() % 6400;// 6400 - 1;
		newPlayer->Y = rand() % 6400;// 6400 - 1;
	newPlayer->disconnect = false;
	newPlayer->move = false;
	newPlayer->curSectorXPos = newPlayer->X / dfSECTOR_ONE_SIZE_X;
	newPlayer->curSectorYPos = newPlayer->Y / dfSECTOR_ONE_SIZE_Y;
	// TODO 섹터에 추가 g_Player_Sector[new_player->curSectorXPos][new_player->curSectorYPos].
    mSectorManager->AddPlayerToSector(newPlayer, newPlayer->curSectorXPos, newPlayer->curSectorYPos);
	newPlayer->lastRecvTime = time(NULL);

	MAKE_PACKET_SC_CREATE_MY_CHARACTER(mConnectSerializeBuffer.get(), newPlayer->mSessionId,
		newPlayer->Dir, newPlayer->X, newPlayer->Y, newPlayer->HP);
	
	mSendPacket(newPlayer->mSessionId, mConnectSerializeBuffer->GetBufferPtr(), mConnectSerializeBuffer->GetDataSize());
	// 2. 나에게 현재 접속해 있는 다른 클라이언트의 생성 패킷 보낸다. 

	SectorAry sectorAry;
	mSectorManager->GetSectorAround(newPlayer->curSectorXPos, newPlayer->curSectorYPos, &sectorAry);

	std::vector<Player*> playerList;
	mSectorManager->GetAroundSectorPlayer(newPlayer->mPlayerId, &playerList);

	for (auto p : playerList)
	{
		if (p->mSessionId != newPlayer->mSessionId)
		{
			MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(mConnectSerializeBuffer.get(), p->mSessionId, p->Dir, p->X, p->Y, p->HP);
			mSendPacket(newPlayer->mSessionId, mConnectSerializeBuffer->GetBufferPtr(), mConnectSerializeBuffer->GetDataSize());
		}
	}

	// 3. 현재 접속해 있는 다른 캐릭터에게 내 생성 패킷 보낸다.
	MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(mConnectSerializeBuffer.get(), newPlayer->mSessionId,
		newPlayer->Dir, newPlayer->X, newPlayer->Y, newPlayer->HP);
	mSectorManager->SendSectorAry(&sectorAry, mConnectSerializeBuffer->GetBufferPtr(), mConnectSerializeBuffer->GetDataSize(), newPlayer->mSessionId);

	return;
}

void PacketManager::DisConnect(int sessionId)
{
	bool lock = mPacketManagerMutex.try_lock();
//	std::lock_guard<std::mutex> guard(mPlayerManagerMutex);
	
	Player* delPlayer = mPlayerManager->FindByPlayerId(sessionId);
	if (delPlayer != nullptr)
	{
		delPlayer->disconnect = true;
	}

	if (lock == true)
	{
		mPacketManagerMutex.unlock();
	}
	
}


int mStartTime = 0;         //측정개시시각
int accumulateTime =0;
int tookTime = 0;
bool start = false;
void PacketManager::PrintServerLogicState(int sessionCount, int minFps, int sync_count, int& accumulateFrame)
{
	if (start == false)
	{
		mStartTime = clock(); start = true;
		return;
	}
	else
	{
		tookTime = clock() - mStartTime;	//걸린시간
		accumulateTime += tookTime;
	}
	
	
	if (accumulateTime > 1000)
	{
		system("cls");
		printf("[Detail]\n세션 수: %d\nSNYC PACKET:%d\n\n[SERVER LOGIC]\n로직 프레임:%d\n스킵 프레임:%d\n", sessionCount, sync_count, minFps, accumulateFrame);
		printf("[Network I/O]\nNetwork Time Send: %dbyte/sec\nNetwork Time Recv: %dbyte/sec\naccept Count:%d\n", *mNetworkSend, *mNetworkRecv, *mAcceptCount);
		*mNetworkSend = *mNetworkRecv = 0;
		accumulateTime = 0;
		accumulateFrame = 0;
		start = false;
	}
}

void PacketManager::SetPrintVarible(int* sendByte, int* recvByte, int* accpetCount)
{
	mNetworkSend = sendByte;
	mNetworkRecv = recvByte;
	mAcceptCount = accpetCount;
	
}



bool PacketManager::Process_dfPACKET_CS_MOVE_START(int playerId, char* pPacket, int length)
{

	std::lock_guard<std::mutex> lm(mPacketManagerMutex);
	Player* player = mPlayerManager->FindByPlayerId(playerId);

	if (player == nullptr)
	{
		return 0;
	}


	// 플레이어 맵에서 삭제



	SerializeBuffer tempBuffer;

	SectorAry sectorAry;
	CS_MOVE_START* cs_move_start = (CS_MOVE_START*)(pPacket);
	// 서버와 클라이언트의 위치 차이를 확인해서, 위치가 너무 크게 틀어졌으면 싱크 패킷을 보낸다..
	if (InRange(player->X, player->Y, cs_move_start->X, cs_move_start->Y) == false)
	{

		MAKE_PACKET_dfPACKET_SC_SYNC(&tempBuffer, player->mPlayerId, player->X, player->Y);
		//printf("START [SYNC] Session ID: %d, Server: [%d, %d],  Client: [%d, %d]\n", player->session_id, player->X, player->Y, cs_move_start->X, cs_move_start->Y);
		//SendPacket(player->session_id, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize());

		mSectorManager->GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);

	//	GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);

		mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(),-1);

	//	SendSector(player->session_id, &tempBuffer, &sectorAry, true);

		// SendBroadCast(player->session_id, &tempBuffer, true);
		cs_move_start->X = player->X;
		cs_move_start->Y = player->Y;

	}
	else
	{

	}



	player->X = cs_move_start->X;
	player->Y = cs_move_start->Y;

	//	printf("[Client START] ID: %d, [%d, %d], Dir: %d\n", player->session_id, cs_move_start->X, cs_move_start->Y, cs_move_start->Direction);

	//	printf("[Server START] ID: %d, [%d, %d], Dir: %d\n", player->session_id, player->X, player->Y, player->Dir);
		// 너무 크게 차이 안나면 클라 위치 따른다.
	player->move = true;

	char before_dir = LLorRR(player->Dir);
	if (before_dir != -1)
	{
		player->before_dir = before_dir;
	}
	player->Dir = cs_move_start->Direction;

	//cs_move_start->X 


//	whatDir(cs_move_start->Direction);

	
	mSectorManager->GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);

	MAKE_PACKET_dfPACKET_SC_MOVE_START(&tempBuffer, player->mPlayerId, cs_move_start->Direction,	player->X, player->Y);

	mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), player->mPlayerId);

	return false;
}

bool PacketManager::Process_dfPACKET_CS_MOVE_STOP(int playerId, char* pPacket, int length)
{

	std::lock_guard<std::mutex> lm(mPacketManagerMutex);
	Player* player = mPlayerManager->FindByPlayerId(playerId);

	if (player == nullptr)
	{
		return 0;
	}


	// 플레이어 맵에서 삭제



	SerializeBuffer tempBuffer;

	SectorAry sectorAry;
	CS_MOVE_STOP* cs_move_stop = (CS_MOVE_STOP*)(pPacket);

	player->move = false;

	// 서버와 클라이언트의 위치 차이를 확인해서, 위치가 너무 크게 틀어졌으면 클라를 끊는지 않고 SYNC 패킷을 보낸다.
	if (InRange(player->X, player->Y, cs_move_stop->X, cs_move_stop->Y) == false)
	{

		//	printf("[Client STOP] ID: %d, [%d, %d], Dir: %d\n", player->session_id, cs_move_stop->X, cs_move_stop->Y, cs_move_stop->Direction);

		//	printf("[Server STOP] ID: %d, [%d, %d], Dir: %d\n", player->session_id, player->X, player->Y, player->Dir);



	
	//GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);
		mSectorManager->GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);




		MAKE_PACKET_dfPACKET_SC_MOVE_STOP(&tempBuffer, player->mPlayerId, cs_move_stop->Direction,
			player->X, player->Y);


		mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), player->mPlayerId);



		//						SendBroadCast(player->session_id, &tempBuffer, false);

		MAKE_PACKET_dfPACKET_SC_SYNC(&tempBuffer, player->mSessionId, player->X, player->Y);
	//	printf("STOP [SYNC] Session ID: %d, Server: [%d, %d],  Client: [%d, %d]\n", player->session_id, player->X, player->Y, cs_move_stop->X, cs_move_stop->Y);
		//	SendPacket(player->session_id, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize());

		mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), -1);




		player->Dir = cs_move_stop->Direction;

		char before_dir = LLorRR(player->Dir);
		if (before_dir != -1)
		{
			player->before_dir = before_dir;

		}

	}
	else
	{

		//		printf("[Client STOP] ID: %d, [%d, %d], Dir: %d\n", player->session_id, cs_move_stop->X, cs_move_stop->Y, cs_move_stop->Direction);
//
				//		printf("[Server STOP] ID: %d, [%d, %d], Dir: %d\n", player->session_id, player->X, player->Y, player->Dir);



		player->X = cs_move_stop->X;
		player->Y = cs_move_stop->Y;

		player->Dir = cs_move_stop->Direction;

		char before_dir = LLorRR(player->Dir);
		if (before_dir != -1)
		{
			player->before_dir = before_dir;
		}

		//SectorAry sectorAry;
	//	GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);
		mSectorManager->GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);




		MAKE_PACKET_dfPACKET_SC_MOVE_STOP(&tempBuffer, player->mSessionId, cs_move_stop->Direction,
			cs_move_stop->X, cs_move_stop->Y);
		//	SendBroadCast(player->session_id, &tempBuffer, false);


	//	SendSector(player->session_id, &tempBuffer, &sectorAry, false);


		mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), player->mPlayerId);



	}



	/*	char temp_packet_buffer[100];
		int data_size = tempBuffer.GetDataSize();

		tempBuffer.GetData(temp_packet_buffer, data_size);

		EnterCriticalSection(&g_Player_vector_cs);
		for (int j = 0; j < g_Player_vector.size(); j++)
		{
			if (g_Player_vector[j]->session_id != player->session_id)
			{
				SendPacket(g_Player_vector[j]->session_id, temp_packet_buffer, data_size);


			}
		}
		LeaveCriticalSection(&g_Player_vector_cs);*/
	return false;
}

bool PacketManager::Process_dfPACKET_CS_ATTACK1(int playerId, char* pPacket, int length)
{
	std::lock_guard<std::mutex> lm(mPacketManagerMutex);
	Player* player = mPlayerManager->FindByPlayerId(playerId);

	if (player == nullptr)
	{
		return 0;
	}


	CS_ATTACK1* cs_attack1 = (CS_ATTACK1*)(pPacket);;

	//			printf("[ATTACK1] Session ID: %d, Server: [%d, %d],  Client: [%d, %d]\n", player->session_id, player->X, player->Y, cs_attack1->X, cs_attack1->Y);
	SectorAry sectorAry;

	SerializeBuffer tempBuffer;
	// 서버와 클라이언트의 위치 차이를 확인해서, 위치가 너무 크게 틀어졌으면 클라를 끊는지 않고 SYNC 패킷을 보낸다.
	if (InRange(player->X, player->Y, cs_attack1->X, cs_attack1->Y) == false)
	{
		MAKE_PACKET_dfPACKET_SC_SYNC(&tempBuffer, player->mPlayerId, player->X, player->Y);

		//printf("ATTACK [SYNC] Session ID: %d, Server: [%d, %d],  Client: [%d, %d]\n", player->session_id, player->X, player->Y, cs_attack1->X, cs_attack1->Y);
		//	SendBroadCast(player->session_id, &tempBuffer, true);
		mSectorManager->GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);
		mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), -1);
		// SendSector(player->session_id, &tempBuffer, &sectorAry, true);

	}
	else
	{

		player->X = cs_attack1->X;
		player->Y = cs_attack1->Y;

		player->Dir = cs_attack1->Direction;

	}



	MAKE_PACKET_dfPACKET_SC_MOVE_STOP(&tempBuffer, player->mSessionId, cs_attack1->Direction,
		player->X, player->Y);
	//	SendBroadCast(player->session_id, &tempBuffer, false);

	//SendSector(player->session_id, &tempBuffer, &sectorAry, false);
	mSectorManager->SendPlayerSectorAry(player->mPlayerId, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), false);


	player->move = false;


	// 공격 패킷을 나 제외 다른 유저에게 쏴준다.
	MAKE_PAKCET_dfPACKET_SC_ATTACK1(&tempBuffer, player->mSessionId, player->Dir,
		player->X, player->Y);
	//SendBroadCast(player->session_id, &tempBuffer, false);
	mSectorManager->GetSectorAround(player->curSectorXPos, player->curSectorYPos, &sectorAry);
	//SendSector(player->mSessionId, &tempBuffer, &sectorAry, false);
	mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), player->mPlayerId);


	std::vector<Player* > hitedVector;
	mSectorManager->GetAroundSectorPlayer(player->mPlayerId, &hitedVector);


	for (int i = 0; i < hitedVector.size(); i++)
	{
		if (player->mPlayerId == hitedVector[i]->mPlayerId)
			continue;
		bool isHit = InRangeDamege(hitedVector[i]->X, hitedVector[i]->Y, player->X, player->Y, player->before_dir);

		if (isHit)
		{
			MAKE_PACKET_dfPACKET_SC_DAMAGE(&tempBuffer, player->mPlayerId, hitedVector[i]->mPlayerId, hitedVector[i]->HP - dfATTACK1_DAMAGE);
			hitedVector[i]->HP -= dfATTACK1_DAMAGE;
			if (hitedVector[i]->HP <= 0)
			{
				hitedVector[i]->disconnect = true;
			}
			// SendSectorAry 패킷을 HP 쏠 때, 공격자 주변이 아니라 피격자 주변 9섹터에 뿌려야 된다. 
			SectorAry demagedsectorAry;
			mSectorManager->GetSectorAround(hitedVector[i]->curSectorXPos, hitedVector[i]->curSectorYPos, &demagedsectorAry);
			mSectorManager->SendSectorAry(&demagedsectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), -1);
		//	mSectorManager->SendSectorAry(&sectorAry, tempBuffer.GetBufferPtr(), tempBuffer.GetDataSize(), -1);
		}
	}




	return false;
}

bool PacketManager::Process_dfPACKET_CS_ATTACK2(int playerId, char* pPacket, int length)
{
	std::lock_guard<std::mutex> lm(mPacketManagerMutex);
	Player* player = mPlayerManager->FindByPlayerId(playerId);

	if (player == nullptr)
	{
		return 0;
	}



	return false;
}

bool PacketManager::Process_dfPACKET_CS_ATTACK3(int playerId, char* pPacket, int length)
{
	return false;
}

bool PacketManager::Process_dfPACKET_CS_ECHO(int playerId, char* pPacket, int length)
{
	return false;
}

void PacketManager::MAKE_PACKET_SC_CREATE_MY_CHARACTER(SerializeBuffer* buffer, int id, char Dir, short X, short Y, char HP)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 10;
	header.byType = dfPACKET_SC_CREATE_MY_CHARACTER;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;
	*buffer << HP;
	return;
}

void PacketManager::MAKE_PACKET_SC_CREATE_OTHER_CHARACTER(SerializeBuffer* buffer, int id, char Dir, short X, short Y, char HP)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 10;
	header.byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;
	*buffer << HP;
	return;
}

void PacketManager::MAKE_PACKET_SC_DELETE_CHARACTER(SerializeBuffer* buffer, int id)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 4;
	header.byType = dfPACKET_SC_DELETE_CHARACTER;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;

	return;
}

void PacketManager::MAKE_PAKCET_dfPACKET_SC_ATTACK1(SerializeBuffer* buffer, int id, char Dir, short X, short Y)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 9;
	header.byType = dfPACKET_SC_ATTACK1;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;

	return;
}

void PacketManager::MAKE_PAKCET_dfPACKET_SC_ATTACK2(SerializeBuffer* buffer, int id, char Dir, short X, short Y)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 9;
	header.byType = dfPACKET_SC_ATTACK2;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;

	return;
}

void PacketManager::MAKE_PAKCET_dfPACKET_SC_ATTACK3(SerializeBuffer* buffer, int id, char Dir, short X, short Y)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 9;
	header.byType = dfPACKET_SC_ATTACK3;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;

	return;
}

void PacketManager::MAKE_PACKET_dfPACKET_SC_MOVE_START(SerializeBuffer* buffer, int id, char Dir, short X, short Y)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 9;
	header.byType = dfPACKET_SC_MOVE_START;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;
}

void PacketManager::MAKE_PACKET_dfPACKET_SC_MOVE_STOP(SerializeBuffer* buffer, int id, char Dir, short X, short Y)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 9;
	header.byType = dfPACKET_SC_MOVE_STOP;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << Dir;
	*buffer << X;
	*buffer << Y;
}

void PacketManager::MAKE_PACKET_dfPACKET_SC_DAMAGE(SerializeBuffer* buffer, int id, int target_id, char target_hp)
{
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 9;
	header.byType = dfPACKET_SC_DAMAGE;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << target_id;
	*buffer << target_hp;
}

void PacketManager::MAKE_PACKET_dfPACKET_SC_SYNC(SerializeBuffer* buffer, int id, short X, short Y)
{
	syncCount++;
	buffer->Clear();
	PACKET_HEADER header;
	header.byCode = 0x89;
	header.bySize = 8;
	header.byType = dfPACKET_SC_SYNC;
	*buffer << header.byCode;
	*buffer << header.bySize;
	*buffer << header.byType;
	*buffer << id;
	*buffer << X;
	*buffer << Y;
}

size_t PacketManager::RemovePlayer(int session_id)
{
	Player* deletePlayer = mPlayerManager->FindByPlayerId(session_id);

	if (deletePlayer == nullptr)
	{
		return 0;
	}
	SerializeBuffer* tempBuffer = new SerializeBuffer;


	// 플레이어 맵에서 삭제
	std::lock_guard<std::mutex> lm(mPacketManagerMutex);




	tempBuffer->Clear();
	MAKE_PACKET_SC_DELETE_CHARACTER(tempBuffer, deletePlayer->mSessionId);
	SectorAry sectorAry;
	mSectorManager->GetSectorAround(deletePlayer->curSectorXPos, deletePlayer->curSectorYPos, &sectorAry);
	mSectorManager->SendSectorAry(&sectorAry, tempBuffer->GetBufferPtr(), tempBuffer->GetDataSize(), -1);// deletePlayer->session_id, tempBuffer, & sectorAry, false);


	// 섹터에서 삭제
	mSectorManager->DeletePlayerToSector(deletePlayer, deletePlayer->curSectorXPos, deletePlayer->curSectorYPos);
//	DeletePlayerToSector(deletePlayer, deletePlayer->curSectorXPos, deletePlayer->curSectorYPos);

	//	printf("g_Player_Sector에서 제거 session id: %d\n", session_id);
	
	
	mDisconnectSession(deletePlayer->mSessionId);
	size_t deleteCount = mPlayerManager->RemovePlayer(session_id);
	

//	ForceDeleteSession(session_id);
	delete tempBuffer;
	return deleteCount;
}

