#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <stdio.h>
#include <iostream>
#include "TCPFighterServer.h"

int main()
{
	TCPFighterServer* server = new TCPFighterServer();
	server->LogicFrame(20000,1);

	return 0;
}