#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "cpu.h"
#include "gpu.h"
#include "tables.h"

Byte _rom[0x8000];
Byte _oam[0x100];
Byte _vram[0x2000];
Byte _wram[0x2000];
Byte _eram[0x2000];
Byte _io[0x100];
Byte _zram[0x80];

int _inbios = 1; // Controls if first sector is read from boot or rom

void loadRom(char* fileName)
{
	FILE* fptr;
	fptr = fopen(fileName, "rb");

	fread(_rom, sizeof(Byte), 0x7FFF, fptr);
}

void copy(Word dest, Word source, size_t length)
{
	for (unsigned int i = 0; i < length; ++i)
	{
		wb(dest + i, rb(source + i));
	}
}

Byte rb(Word addr)
{
	//printf("Read from address 0x%02x\n", addr);

	if (_inbios)
	{
		if (addr < 0x100)
			return _bios[addr];
		else if (reg.PC == 0x100)
			_inbios = 0;
	}

	if (addr < 0x8000)
		return _rom[addr];

	else if (addr >= 0x8000 && addr < 0xA000)
		return _vram[addr - 0x8000];

	else if (addr >= 0xA000 && addr < 0xC000)
		return _eram[addr - 0xa000];

	else if (addr >= 0xC000 && addr < 0xFE00)
		return _wram[addr - 0xC000];

	else if (addr >= 0xFE00 && addr < 0xFF00)
		return _oam[addr - 0xFE00];

	else if (addr == 0xFF00)
	{

	}

	else if (addr == 0xFF04)
		return (Byte)rand();

	else if (addr == 0xFF0F)
		return interrupt.flags;

	else if (addr == 0xFF40)
		return gpu.lcdc;

	else if (addr == 0xFF42)
		return gpu.scy;

	else if (addr == 0xFF43)
		return gpu.scx;

	else if (addr == 0xFF44)
		return gpu.line;

	else if (addr == 0xFF0F)
		return interrupt.flags;

	else if (addr > 0xFF00 && addr < 0xFF80)
		return _io[addr - 0xFF00];

	else if (addr >= 0xFF80 && addr < 0xFFFF)
		return _zram[addr - 0xFF80];

	else if (addr == 0xFFFF) 
		return interrupt.enable;

	return 0;
}

Word rw(Word addr)
{
	return rb(addr) + (rb(addr + 1) << 8);
}

void wb(Word addr, Byte val)
{
	//printf("Write to address 0x%02x\n", addr);

	if (_inbios)
	{
		if (addr < 0x100)
		{
			_bios[addr] = val;
			return;
		}
		else if (reg.PC == 0x100)
			_inbios = 0;
	}

	if (addr >= 0xA000 && addr < 0xC000)
		_eram[addr - 0xA000] = val;

	else if (addr >= 0x8000 && addr < 0xA000)
	{
		_vram[addr - 0x8000] = val;
		if (addr < 0x9800)
		{
			updateTile(addr);
		}
	}

	if (addr < 0xC000)
	{
	}

	else if (addr >= 0xC000 && addr < 0xFE00)
		_wram[addr - 0xC000] = val;

	else if (addr >= 0xFE00 && addr < 0xFF00)
		_oam[addr - 0xFE00] = val;

	else if (addr == 0xFF0F)
		interrupt.flags = val;

	else if (addr == 0xFF40)
		gpu.lcdc = val;

	else if (addr == 0xFF42)
		gpu.scy = val;

	else if (addr == 0xFF43)
	{
		gpu.scx = val;
	}
		
	else if (addr == 0xFF46)
		copy(0xFE00, val << 8, 160);

	else if (addr == 0xFF47)
	{
		for (int i = 0; i < 4; i++)
		{
			bgPalette[i] = palette[(val >> (i * 2)) & 3];
		}
	}

	else if (addr == 0xFF48)
	{
		for (int i = 0; i < 4; i++)
		{
			objPalette[0][i] = palette[(val >> (i * 2)) & 3];
		}
	}

	else if (addr == 0xFF49)
	{
		for (int i = 0; i < 4; i++)
			objPalette[1][i] = palette[(val >> (i * 2)) & 3];
	}

	else if (addr == 0xFF0F)
		interrupt.flags = val;

	else if (addr >= 0xFF00 && addr < 0xFF80)
		_io[addr - 0xFF00] = val;

	else if (addr >= 0xFF80 && addr < 0xFFFF)
		_zram[addr - 0xFF80] = val;

	else if (addr == 0xFFFF)
		interrupt.enable = val;
}

void ww(Word addr, Word val)
{
	if (addr == 0xFFFE)
		NOP();

	wb(addr, (Byte)(val & 0x00FF));
	wb(addr + 1, (Byte)((val & 0xFF00) >> 8));

	//printf("WRITE 0x%04X TO 0x%04X\n", val, rw(addr));
}

Byte rwStack()
{
	Byte value = rw(reg.SP);
	reg.SP += 2;

	//printf("Stack read 0x%02x\n", value);

	return value;
}

void wwStack(Word val)
{
	reg.SP -= 2;
	ww(reg.SP, val);

	//printf("Stack write 0x%02x\n", val);
}

Byte nextOp()
{
	Byte op = rb(reg.PC);
	reg.PC++;
	return op;
}

Word nextOps() 
{
	Word op = rw(reg.PC);
	reg.PC += 2;
	return op;
}