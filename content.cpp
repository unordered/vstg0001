#include "Content.h"
#include "Packet.h"
#include <cstdio>
#include <stdlib.h>
bool InRangeDamege(short target_x, short target_y, short x, short y, char Dir)
{
	if (Dir == dfPACKET_MOVE_DIR_RR)
	{
		if (target_x <= x + 100 && target_x >= x && abs(target_y - y) < 50)
		{
			return true;
		}
	}
	else if (Dir == dfPACKET_MOVE_DIR_LL)
	{

		if (target_x >= x - 100 && target_x <= x && abs(target_y - y) < 50)
		{
			return true;
		}
	}
	else
	{
		exit(1);
		printf("방향 오류\n");
		return false;
	}

	return false;
}



char LLorRR(char dir)
{
	switch (dir)
	{
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LD:
		return dfPACKET_MOVE_DIR_LL;
		break;



	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RD:
		return dfPACKET_MOVE_DIR_RR;
		break;


	case dfPACKET_MOVE_DIR_UU:
	case dfPACKET_MOVE_DIR_DD:	
		return -1;
		break;

	default:
		printf("방향 오류\n");
		break;
		return -1;
	}

	return -1;
}

bool InRange(short target_x, short target_y, short x, short y)
{
	const short dif_x = 50;
	const short dif_y = 50;

	if (abs(target_x - x) > dif_x)
	{
		return false;
	}


	if (abs(target_y - y) > dif_y)
	{
		return false;
	}

	return true;
}