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

	else if (addr == 0xFF00)
	{
		if ((joypad.select & (1 << 4)) == 0)
		{
			for (int i = 5; i >= 0; --i)
			{
				Byte val = 0xC0 | joypad.select | (joypad.keys & 0x0F);
				printf("%d", val & (1 << i) ? 1 : 0);
			}
			printf("\n");
			return 0xC0 | joypad.select | (joypad.keys & 0x0F);
		}
		else if ((joypad.select & (1 << 5)) == 0)
		{
			for (int i = 5; i >= 0; --i)
			{
				Byte val = 0xC0 | joypad.select | (joypad.keys >> 4);
				printf("%d", val & (1 << i) ? 1 : 0);
			}
			printf("\n");
			return 0xC0 | joypad.select | (joypad.keys >> 4);
		}
		else if (joypad.select & 0x30)
		{
			return 0xFF;
		}
		else
		{
			return 0;
		}
	}

	else if (addr == 0xFF04)
	{
		return (Byte)rand();
	}

	else if (addr == 0xFF0F)
	{
		return interrupt.flags;
	}

	else if (addr == 0xFF40)
	{
		return ppu.lcdc;
	}

	else if (addr == 0xFF41)
	{
		return ppu.stat & 0xFC | ppu.mode;
	}

	else if (addr == 0xFF42)
	{
		return ppu.scy;
	}

	else if (addr == 0xFF43)
	{
		return ppu.scx;
	}

	else if (addr == 0xFF44)
	{
		return ppu.ly;
	}

	else if (addr == 0xFF45)
	{
		return ppu.lyc;
	}

	else if (addr == 0xFF4A)
	{
		return ppu.wy;
	}

	else if (addr == 0xFF4B)
	{
		return ppu.wx;
	}

	else if (addr == 0xFF4D || addr == 0xFF74)
	{
		return 0xFF;
	}

	else if (addr > 0xFF00 && addr < 0xFF80)
	{
		return _io[addr - 0xFF00];
	}

	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		return _zram[addr - 0xFF80];
	}

	else if (addr == 0xFFFF)
	{
		return interrupt.enable;
	}

	return 0;
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

	if (addr == 0xFF80) return;

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

	if (addr >= 0xA000 && addr < 0xC000)
	{ 
		_eram[addr - 0xA000] = val;
	}

	else if (addr >= 0x8000 && addr < 0xA000)
	{
		_vram[addr - 0x8000] = val;
		if (addr < 0x9800)
		{
			//printf("%X %X\n", addr, val);
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

	else if (addr == 0xFF00)
	{
		joypad.select = val; // Mask off bottom 4 bits
	}

	if (addr == 0xFF02 && val == 0x81)
	{
		//printf("%X\n", rb(0xFF01));
	}

	else if (addr == 0xFF0F)
	{
		interrupt.flags = val;
	}

	else if (addr == 0xFF40)
	{
		//printf("Write to lcdc %d %X %d\n", val, reg.PC, ppu.mode);
		ppu.lcdc = val;
	}

	else if (addr == 0xFF41)
	{
		ppu.stat |= val & 0x78; // Only allow write to upper 4 bits
	}

	else if (addr == 0xFF42)
	{
		ppu.scy = val;
	}

	else if (addr == 0xFF43)
	{
		ppu.scx = val;
	}

	//FF44 ly read only
		
	else if (addr == 0xFF45)
	{
		ppu.lyc = val;
	}

	else if (addr == 0xFF46)
	{
		copyDma(0xFE00, val << 8, 160);
	}

	else if (addr == 0xFF47)
	{
		
		for (int i = 0; i < 4; ++i)
		{
			bgPalette[i] = palette[(val >> (i * 2)) & 3];
		}
	}

	else if (addr == 0xFF48)
	{
		for (int i = 0; i < 4; ++i)
		{
			objPalette[0][i] = palette[(val >> (i * 2)) & 3];
		}
	}

	else if (addr == 0xFF49)
	{
		for (int i = 0; i < 4; ++i)
		{
			objPalette[1][i] = palette[(val >> (i * 2)) & 3];
		}
	}

	else if (addr == 0xFF4A)
	{
		ppu.wy = val;
	}

	else if (addr == 0xFF4B)
	{
		ppu.wx = val;
	}

	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
		_io[addr - 0xFF00] = val;
	}

	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		_zram[addr - 0xFF80] = val;
	}

	else if (addr == 0xFFFF)
	{
		interrupt.enable = val;
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