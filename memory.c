#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "cpu.h"
#include "ppu.h"
#include "tables.h"
#include "joypad.h"
#include "timer.h"

Byte _bios[0x100];
Byte _rom[0x8000];
Byte _oam[0x100];
Byte _vram[0x2000];
Byte _wram[0x2000];
Byte _eram[0x2000];
Byte _io[0x100];
Byte _zram[0x80];

Byte memoryMap[0xFFFF][3];

int inbios = 1; // Controls if first sector is read from boot or rom

static Byte serialData = 0;
static Byte serialControl = 0;

void loadBootstrap(char* fileName)
{
	FILE* fptr = fopen(fileName, "rb");

	if (!fptr)
	{
		printf("Failed to load bootstrap\n");
		exit(-1);
	}

	fread(_bios, sizeof(Byte), 0x100, fptr);
}

void loadRom(char* fileName)
{
	char dir[60] = "roms/";
	strcpy(&dir[5], fileName);
	strcpy(&dir[strlen(dir)], ".gb");

	FILE* fptr = fopen(dir, "rb");

	if (!fptr)
	{
		printf("Failed to load rom\n");
		exit(-1);
	}

	fread(_rom, sizeof(Byte), 0x7FFF, fptr);
}

void copyDma(Word dest, Word source, size_t length)
{
	for (unsigned int i = 0; i < length; ++i)
	{
		wb(dest + i, rb(source + i));
	}
}

Byte rb(Word addr)
{
	//printf("Read from address 0x%02x\n", addr);
	/*memoryMap[addr][0] = 0;
	memoryMap[addr][1] = 255;
	memoryMap[addr][2] = 0;*/

	if (inbios)
	{
		if (addr < 0x100)
		{
			return _bios[addr];
		}
		else if (reg.PC == 0x100)
		{
			inbios = 0;
		}
	}

	switch (addr)
	{
	case 0xFF00:
		if ((joypad.select & (1 << 4)) == 0)
		{
			return 0xC0 | joypad.select | (joypad.keys & 0x0F);
		}
		else if ((joypad.select & (1 << 5)) == 0)
		{
			return 0xC0 | joypad.select | (joypad.keys >> 4);
		}
		return 0xFF;
	case 0xFF01:
		return serialData;
	case 0xFF02:
		return serialControl;
	case 0xFF04:
		return timer.div;//(Byte)rand();
	case 0xFF05:
		return timer.tima;
	case 0xFF06:
		return timer.tma;
	case 0xFF07:
		return timer.tac;
	case 0xFF0F:
		return interrupt.flags;
	case 0xFF40:
		return ppu.lcdc;
	case 0xFF41:
		return (ppu.stat & 0xFC) | ppu.mode;
	case 0xFF42:
		return ppu.scy;
	case 0xFF43:
		return ppu.scx;
	case 0xFF44:
		return ppu.ly;
	case 0xFF45:
		return ppu.lyc;
	case 0xFF4A:
		return ppu.wy;
	case 0xFF4B:
		return ppu.wx;
	case 0xFF4D: 
	case 0xFF74:
		return 0xFF;
	case 0xFFFF:
		return interrupt.enable;
	default:
		break;
	}

	if (addr < 0x8000)
	{
		return _rom[addr];
	}

	else if (addr >= 0x8000 && addr < 0xA000)
	{
		return _vram[addr - 0x8000];
	}

	else if (addr >= 0xA000 && addr < 0xC000)
	{
		return _eram[addr - 0xa000];
	}

	else if (addr >= 0xC000 && addr < 0xFE00)
	{
		return _wram[addr - 0xC000];
	}

	else if (addr >= 0xFE00 && addr < 0xFF00)
	{
		return _oam[addr - 0xFE00];
	}

	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
		return _io[addr - 0xFF00];
	}

	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		return _zram[addr - 0xFF80];
	}

	return 0xFF;
}

Word rw(Word addr)
{
	return rb(addr) | (rb(addr + 1) << 8);
}

void wb(Word addr, Byte val)
{
	//printf("Write to address 0x%02x\n", addr);
	/*memoryMap[addr][0] = 0;
	memoryMap[addr][1] = 0;
	memoryMap[addr][2] = 255;*/

	if (inbios)
	{
		if (addr < 0x100)
		{
			_bios[addr] = val;
			return;
		}
		else if (reg.PC == 0x100)
			inbios = 0;
	}

	switch (addr)
	{
	case 0xFF00:
		joypad.select = val; // Mask off bottom 4 bits
		return;
	case 0xFF01:
		serialData = val;
	case 0xFF02:
		serialControl = val;
	case 0xFF04:
		timer.div = 0;
		timer.divClock = 0;
	case 0xFF05:
		timer.tima = val;
	case 0xFF06:
		timer.tma = val;
	case 0xFF07:
		if (timer.tac & 3 != (val & 3))
		{
			timer.tmaClock = 0;
			timer.tima = timer.tma;
		}
		timer.tac = val;
	case 0xFF0F:
		interrupt.flags = val;
		return;
	case 0xFF40:
		ppu.lcdc = val;
		return;
	case 0xFF41:
		ppu.stat |= val & 0xF8; // Only allow write to upper 4 bits
		return;
	case 0xFF42:
		ppu.scy = val;
		return;
	case 0xFF43:
		ppu.scx = val;
		return;
	case 0xFF44:
		return; // Read only
	case 0xFF45:
		ppu.lyc = val;
		return;
	case 0xFF46:
		copyDma(0xFE00, val << 8, 160);
		return;
	case 0xFF47:
		for (int i = 0; i < 4; ++i)
		{
			bgPalette[i] = palette[(val >> (i * 2)) & 3];
		}
	case 0xFF48:
		for (int i = 0; i < 4; ++i)
		{
			objPalette[0][i] = palette[(val >> (i * 2)) & 3];
		}
	case 0xFF49:
		for (int i = 0; i < 4; ++i)
		{
			objPalette[1][i] = palette[(val >> (i * 2)) & 3];
		}
	case 0xFF4A:
		ppu.wy = val;
		return;
	case 0xFF4B:
		ppu.wx = val;
		return;
	case 0xFFFF:
		interrupt.enable = val;
		return;
	default:
		break;
	}

	if (addr >= 0xA000 && addr < 0xC000)
	{ 
		_eram[addr - 0xA000] = val;
	}

	else if (addr >= 0x8000 && addr < 0xA000)
	{
		_vram[addr - 0x8000] = val;
		if (addr < 0x9800)
		{
			updateTile(addr);
		}
	}

	if (addr >= 0xC000 && addr < 0xFE00)
	{
		_wram[addr - 0xC000] = val;
	}

	else if (addr >= 0xFE00 && addr < 0xFF00)
	{
		_oam[addr - 0xFE00] = val;
	}

	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
		_io[addr - 0xFF00] = val;
	}

	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		_zram[addr - 0xFF80] = val;
	}
}

void ww(Word addr, Word val)
{
	wb(addr, (Byte)(val & 0x00FF));
	wb(addr + 1, (Byte)((val & 0xFF00) >> 8));

	//printf("WRITE 0x%04X TO 0x%04X\n", val, rw(addr));
}

Word popFromStack()
{
	Word value = rw(reg.SP);
	reg.SP += 2;
	//printf("Stack read 0x%02x\n", value);
	return value;
}

void pushToStack(Word val)
{
	reg.SP -= 2;
	ww(reg.SP, val);
	//printf("Stack write 0x%02x\n", val);
}

Word nextByte()
{
	Word op = rb(reg.PC);
	reg.PC++;
	return op;
}

Word nextWord() 
{
	Word op = rw(reg.PC);
	reg.PC += 2;
	return op;
}