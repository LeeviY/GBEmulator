#include "debug.h"
#include "cpu.h"
#include "registers.h"
#include "ppu.h"
#include "tables.h"
#include "timer.h"

int debugStop = 0;
int debugEnable = 0;
int debugPrint = 0;
static int count = 0;

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
	count = 0;
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

void printCpu(char sep, char end)
{
	Byte op = rb(reg.PC);
	printf("0x%04X %c OPC=0x%02X \"", reg.PC, sep, op);
	int count = 0;
	if (cpu.prefix)
	{
		count = printf(cbNames[op]);
	}
	else
	{
		switch (numbers[op])
		{
		case 0:
			count = printf(opNames[op]);
			break;
		case 1:
			count = printf(opNames[op], rb(reg.PC + 1));
			break;
		case 2:
			count = printf(opNames[op], rw(reg.PC + 1));
			break;
		}
	}
	printf("\"%*c%c", 22 - count, ' ', end);
	//printf("x=%d z=%d y=%d q=%d p=%d ", opc.x, opc.z, opc.y, opc.q, opc.p);
}

void printRegs(char sep, char end)
{
	//printf("| A=%02X F=%02X B=%02X C=%02X D=%02X E=%02X H=%02X L=%02X (HL)=%02X SP=%04X (SP)=%02X%c", reg.A, reg.F, reg.B, reg.C, reg.D, reg.E, reg.H, reg.L, rb(reg.HL), reg.SP, rb(reg.SP), end);
	printf("%c AF=%04X BC=%04X DE=%04X HL=%04X (HL)=%02X SP=%04X (SP)=%02X%c", sep, reg.AF, reg.BC, reg.DE, reg.HL, rb(reg.HL), reg.SP, rb(reg.SP), end);
}

void printInt(char sep, char end)
{
	printf("%c IM=%02X IE=%02X IF=%02X clk=%2d %c", sep, interrupt.master, interrupt.enable, interrupt.flags, cpu.clock, end);
}

void printPpu(char sep, char end)
{
	static int timings[4] = { 204, 465, 80, 172 };
	int remain = timings[ppu.mode] / 2 - (ppu.clock + cpu.clock) / 2;
	printf("%c lcdc=%02X stat=%02X mode=%d ly=%02X rmn=%3d%c", sep, ppu.lcdc, ppu.stat, ppu.mode, ppu.ly, remain, end);
}

void printTimer(char sep, char end)
{
	printf("%c div=%2X tima=%2X tma=%2X tac=%2X%c", sep, timer.div, timer.tima, timer.tma, timer.tac, end);
}

void printUniquePCs()
{
	static Word seenPCs[0x2000];

	for (int i = 0; i < 0x2000 && !inbios; ++i)
	{
		if (seenPCs[i] == reg.PC)
		{
			break;
		}
		else if (seenPCs[i] == 0)
		{
			seenPCs[i] = reg.PC;
			printCpu('|', '\n');
			break;
		}
	}
}

void printUniqueOps(Byte op)
{
	static Word seenOps[0x200];
	static int CB = 0;

	Word opW = op;

	if (op == 0xCB)
	{
		CB = 1;
		return;
	}

	if (CB)
	{
		opW |= 0xCB00;
		CB = 0;
	}

	for (int i = 0; i < 0x200 && !inbios; ++i)
	{
		if (seenOps[i] == opW)
		{
			break;
		}
		else if (seenOps[i] == 0)
		{
			seenOps[i] = opW;
			printf("%4X\n", opW);
			break;
		}
	}
}