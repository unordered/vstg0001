#pragma once
// ���� �����̳�
typedef struct Sector
{
	int SectorXPos;
	int SectorYPos;
}Sector;


typedef struct SectorAry
{
	int Count;
	Sector ary[9];
}SectorAry;

char LLorRR(char dir);
bool InRangeDamege(short target_x, short target_y, short x, short y, char Dir);
bool InRange(short target_x, short target_y, short x, short y);
