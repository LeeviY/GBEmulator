#include "debug.h"

int debugStop = 0;
int debugEnable = 0;
int debugPrint = 0;

void debugOn(int breakPoint)
{
	if (debugEnable) return;
	if (breakPoint)
	{
		printf("\n================================BREAK ON %04X==================================\n\n", breakPoint);
	}
	else
	{
		printf("\n==================================DEBUG ON=====================================\n\n");
	}
	debugEnable = 1;
	debugStop = 1;
}

void debugOff()
{
	if (!debugEnable) return;
	printf("\n==================================DEBUG OFF====================================\n\n");
	debugEnable = 0;
	debugStop = 0;
}

void printToggle()
{
	if (debugPrint)
	{
		debugPrint = 0;
	}
	else
	{
		debugPrint = 1;
	}
}