#include "debug.h"
#include "cpu.h"
#include "registers.h"
#include "ppu.h"
#include "tables.h"

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

void printCpu(char end)
{
	Byte op = rb(reg.PC);
	printf("0x%04X | OPC=0x%02X \"", reg.PC, op);
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
	printf("\"%*c%c", 24 - count, ' ', end);
	//printf("x=%d z=%d y=%d q=%d p=%d ", opcode.x, opcode.z, opcode.y, opcode.q, opcode.p);
}

void printRegs(char end)
{
	printf("| A=%02X F=%02X B=%02X C=%02X D=%02X E=%02X H=%02X L=%02X (HL)=%02X PC=%04X SP=%04X (SP)=%02X%c", reg.A, reg.F, reg.B, reg.C, reg.D, reg.E, reg.H, reg.L, rb(reg.HL), reg.PC, reg.SP, rb(reg.SP), end);
}

void printInt(char end)
{
	printf("| IM=%02x IE=%02x IF=%02x clk=%2d %c", interrupt.master, interrupt.enable, interrupt.flags, cpu.clock, end);
}

void printPpu(char end)
{
	printf("| gpu clock=%3d mode=%d ly=%3d stat=%02X%c", ppu.clock, ppu.mode, ppu.ly, ppu.stat & 0xFC | ppu.mode, end);
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
			printCpu('\n');
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