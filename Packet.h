#pragma once
#pragma pack(push,1)
/////////////////////////////////////////////////////////////////////
// www.gamecodi.com						이주행 master@gamecodi.com
//
//
/////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------

패킷데이터 정의.


자신의 캐릭터에 대한 패킷을 서버에게 보낼 때, 모두 자신이 먼저
액션을 취함과 동시에 패킷을 서버로 보내주도록 한다.

- 이동 키 입력 시 이동동작을 취함과 동시에 이동 패킷을 보내도록 한다.
- 공격키 입력 시 공격 동작을 취하면서 패킷을 보낸다.
- 충돌 처리 및 데미지에 대한 정보는 서버에서 처리 후 통보하게 된다.


---------------------------------------------------------------*/


#ifndef __PACKET_DEFINE__
#define __PACKET_DEFINE__


//---------------------------------------------------------------
// 패킷헤더.
//
//---------------------------------------------------------------
/*
	BYTE	byCode;			// 패킷코드 0x89 고정.
	BYTE	bySize;			// 패킷 사이즈.(페이로드)
	BYTE	byType;			// 패킷타입.
*/
class PACKET_HEADER
{
public:
	unsigned char	byCode;			// 패킷코드 0x89 고정.
	unsigned char	bySize;			// 패킷 사이즈.
	unsigned char	byType;			// 패킷타입.
};


#define	dfPACKET_SC_CREATE_MY_CHARACTER			0
//---------------------------------------------------------------
// 클라이언트 자신의 캐릭터 할당		Server -> Client
//
// 서버에 접속시 최초로 받게되는 패킷으로 자신이 할당받은 ID 와
// 자신의 최초 위치, HP 를 받게 된다. (처음에 한번 받게 됨)
// 
// 이 패킷을 받으면 자신의 ID,X,Y,HP 를 저장하고 캐릭터를 생성시켜야 한다.
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------

class SC_CREATE_MY_CHARACTER : public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
	char HP;
};

#define	dfPACKET_SC_CREATE_OTHER_CHARACTER		1
//---------------------------------------------------------------
// 다른 클라이언트의 캐릭터 생성 패킷		Server -> Client
//
// 처음 서버에 접속시 이미 접속되어 있던 캐릭터들의 정보
// 또는 게임중에 접속된 클라이언트들의 생성용 정보.
//
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------


class SC_CREATE_OTHER_CHARACTER : public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
	char HP;

};
#define	dfPACKET_SC_DELETE_CHARACTER			2
//---------------------------------------------------------------
// 캐릭터 삭제 패킷						Server -> Client
//
// 캐릭터의 접속해제 또는 캐릭터가 죽었을때 전송됨.
//
//	4	-	ID
//
//---------------------------------------------------------------

class SC_DELETE_CHARACTER : public PACKET_HEADER
{
public:
	int ID;
};


#define	dfPACKET_CS_MOVE_START					10
//---------------------------------------------------------------
// 캐릭터 이동시작 패킷						Client -> Server
//
// 자신의 캐릭터 이동시작시 이 패킷을 보낸다.
// 이동 중에는 본 패킷을 보내지 않으며, 키 입력이 변경되었을 경우에만
// 보내줘야 한다.
//
// (왼쪽 이동중 위로 이동 / 왼쪽 이동중 왼쪽 위로 이동... 등등)
//
//	1	-	Direction	( 방향 디파인 값 8방향 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
class CS_MOVE_START :public PACKET_HEADER
{
public:
	char Direction;
	short X;
	short Y;
};

#define dfPACKET_MOVE_DIR_LL					0
#define dfPACKET_MOVE_DIR_LU					1
#define dfPACKET_MOVE_DIR_UU					2
#define dfPACKET_MOVE_DIR_RU					3
#define dfPACKET_MOVE_DIR_RR					4
#define dfPACKET_MOVE_DIR_RD					5
#define dfPACKET_MOVE_DIR_DD					6
#define dfPACKET_MOVE_DIR_LD					7




#define	dfPACKET_SC_MOVE_START					11
//---------------------------------------------------------------
// 캐릭터 이동시작 패킷						Server -> Client
//
// 다른 유저의 캐릭터 이동시 본 패킷을 받는다.
// 패킷 수신시 해당 캐릭터를 찾아 이동처리를 해주도록 한다.
// 
// 패킷 수신 시 해당 키가 계속해서 눌린것으로 생각하고
// 해당 방향으로 계속 이동을 하고 있어야만 한다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값 8방향 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class SC_MOVE_START : public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
};



#define	dfPACKET_CS_MOVE_STOP					12
//---------------------------------------------------------------
// 캐릭터 이동중지 패킷						Client -> Server
//
// 이동중 키보드 입력이 없어서 정지되었을 때, 이 패킷을 서버에 보내준다.
// 이동중 방향 전환시에는 스탑을 보내지 않는다.
//
//	1	-	Direction	( 방향 디파인 값 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class CS_MOVE_STOP : public PACKET_HEADER
{
public:
	char Direction;
	short X;
	short Y;
};



#define	dfPACKET_SC_MOVE_STOP					13
//---------------------------------------------------------------
// 캐릭터 이동중지 패킷						Server -> Client
//
// ID 에 해당하는 캐릭터가 이동을 멈춘것이므로 
// 캐릭터를 찾아서 방향과, 좌표를 입력해주고 멈추도록 처리한다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class SC_MOVE_STOP : public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
};



#define	dfPACKET_CS_ATTACK1						20
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Client -> Server
//
// 공격 키 입력시 본 패킷을 서버에게 보낸다.
// 충돌 및 데미지에 대한 결과는 서버에서 알려 줄 것이다.
//
// 공격 동작 시작시 한번만 서버에게 보내줘야 한다.
//
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y	
//
//---------------------------------------------------------------

class CS_ATTACK1 :public PACKET_HEADER
{
public:
	char Direction;
	short X;
	short Y;
};




#define	dfPACKET_SC_ATTACK1						21
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격1번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class SC_ATTACK1 : public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
};



#define	dfPACKET_CS_ATTACK2						22
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Client -> Server
//
// 공격 키 입력시 본 패킷을 서버에게 보낸다.
// 충돌 및 데미지에 대한 결과는 서버에서 알려 줄 것이다.
//
// 공격 동작 시작시 한번만 서버에게 보내줘야 한다.
//
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class CS_ATTACK2 : public PACKET_HEADER
{
public:
	char Direction;
	short X;
	short Y;
};

#define	dfPACKET_SC_ATTACK2						23
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격2번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class SC_ATTACK2 :public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
};

#define	dfPACKET_CS_ATTACK3						24
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Client -> Server
//
// 공격 키 입력시 본 패킷을 서버에게 보낸다.
// 충돌 및 데미지에 대한 결과는 서버에서 알려 줄 것이다.
//
// 공격 동작 시작시 한번만 서버에게 보내줘야 한다.
//
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class CS_ATTACK3 : public PACKET_HEADER
{
public:
	char Direction;
	short X;
	short Y;
};

#define	dfPACKET_SC_ATTACK3						25
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격3번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

class SC_ATTACK3 : public PACKET_HEADER
{
public:
	int ID;
	char Direction;
	short X;
	short Y;
};



#define	dfPACKET_SC_DAMAGE						30
//---------------------------------------------------------------
// 캐릭터 데미지 패킷							Server -> Client
//
// 공격에 맞은 캐릭터의 정보를 보냄.
//
//	4	-	AttackID	( 공격자 ID )
//	4	-	DamageID	( 피해자 ID )
//	1	-	DamageHP	( 피해자 HP )
//
//---------------------------------------------------------------


class SC_DAMAGE : public PACKET_HEADER
{
public:
	int AttackID;
	int DamageID;
	char DamageHP;
};

#define	dfPACKET_CS_ECHO						252
//---------------------------------------------------------------
// Echo 용 패킷					Client -> Server
//
//	4	-	Time
//
//---------------------------------------------------------------
class CS_ECHO : public PACKET_HEADER
{
public:
	int time;
};


#define	dfPACKET_SC_ECHO						253
//---------------------------------------------------------------
// Echo 응답 패킷				Server -> Client
//
//	4	-	Time
//
//---------------------------------------------------------------

class SC_ECHO : public PACKET_HEADER
{
public:
	int time;
};



// 사용안함...
#define	dfPACKET_CS_SYNC						250
//---------------------------------------------------------------
// 동기화를 위한 패킷					Client -> Server
//
//
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------


#define	dfPACKET_SC_SYNC						251
//---------------------------------------------------------------
// 동기화를 위한 패킷					Server -> Client
//
// 서버로부터 동기화 패킷을 받으면 해당 캐릭터를 찾아서
// 캐릭터 좌표를 보정해준다.
//
//	4	-	ID
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------

// new
// //-----------------------------------------------------------------

#define dfSPEED_PLAYER_X 6
#define dfSPEED_PLAYER_Y 4

#define dfRANGE_MOVE_TOP	0
#define dfRANGE_MOVE_LEFT	0
#define dfRANGE_MOVE_RIGHT	6400
#define dfRANGE_MOVE_BOTTOM	6400


// 섹터 갯수
#define dfSECTOR_MAX_X 32// (64/2)
#define dfSECTOR_MAX_Y 32 //(64/2)

// 섹터 하나의 크기
#define dfSECTOR_ONE_SIZE_X (dfRANGE_MOVE_RIGHT/dfSECTOR_MAX_X)

#define dfSECTOR_ONE_SIZE_Y (dfRANGE_MOVE_RIGHT/dfSECTOR_MAX_Y)

#define dfATTACK1_DAMAGE 3
#define dfATTACK2_DAMAGE 4
#define dfATTACK3_DAMAGE 5

#endif


#pragma pack(pop)



