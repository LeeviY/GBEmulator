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

Byte _romStart[0x100];

Byte _rom[0x8000];
Byte _vram[0x2000];
Byte _eram[0x2000];
Byte _wram[0x2000];
Byte _sram[0x1E00];
Byte _oam[0xA0];
Byte _io[0x80];
Byte _hram[0x80];

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

	fread(_rom, sizeof(Byte), 0x100, fptr);
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

	for (int i = 0; i < 0x100; ++i)
	{
		_romStart[i] = _rom[i];
	}
}

void resetMemory()
{
	Byte ioStart[0x80] = 
	{
		0xCF, 0x00, 0x7E, 0xFF, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1,
		0x80, 0x3F, 0x00, 0xFF, 0xBF, 0xFF, 0x3F, 0x00, 0xFF, 0xBF, 0x7F, 0xFF, 0x9F, 0xFF, 0xBF, 0xFF,
		0xFF, 0x00, 0x00, 0xBF, 0x00, 0x00, 0x70, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
		0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF 
	};

	for (int i = 0; i < 0x80; ++i)
	{
		wb(0xFF00 + i, ioStart[i]);
	}

	ppu.stat = 0x84;
	wb(0xFFFF, 0);	//ie
}

void memoryDump()
{
	Byte* memoryMirror = (Byte*)malloc(0x10000 * sizeof(Byte));

	if (!memoryMirror)
	{
		return;
	}

	for (int i = 0; i < 0xFFFF + 1; ++i)
	{
		memoryMirror[i] = rb((Word)i);
	}

	FILE* write_ptr = fopen("test.bin", "wb");
	fwrite(memoryMirror, 0xFFFF * sizeof(Byte), 1, write_ptr);
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
	if (addr < 0x8000)
	{
		return _rom[addr];
	}
	else if (addr >= 0x8000 && addr < 0xA000)
	{
		/*if (ppu.mode & PPU_MODE_VRAM)
		{
			printf("VRAM READ\n");
			return 0xFF;
		}*/
		return _vram[addr - 0x8000];
	}
	else if (addr >= 0xA000 && addr < 0xC000)
	{
		return _eram[addr - 0xA000];
	}
	else if (addr >= 0xC000 && addr < 0xE000)
	{
		return _wram[addr - 0xC000];
	}
	else if (addr >= 0xE000 && addr < 0xFE00)
	{
		return _wram[addr - 0xE000];
	}
	else if (addr >= 0xFE00 && addr < 0xFEA0)
	{
		/*if (ppu.mode & (PPU_MODE_VRAM | PPU_MODE_OAM))
		{
			printf("OAM READ\n");
			return 0xFF;
		}*/
		return _oam[addr - 0xFE00];
	}
	else if (addr >= 0xFEA0 && addr < 0xFF00)
	{
		//printf("There\n");
	}
	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
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
			return interrupt.flags | 0xE0;
		case 0xFF40:
			return ppu.lcdc;
		case 0xFF41:
			return ppu.stat;
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
		case 0xFF50:
			return 0xFF;
		case 0xFF4D:
			return 0xFF;
		default:
			return _io[addr - 0xFF00];
		}
	}
	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		return _hram[addr - 0xFF80];
	}
	else if (addr == 0xFFFF)
	{
		return interrupt.enable;
	}
}

Word rw(Word addr)
{
	return rb(addr) | (rb(addr + 1) << 8);
}

void wb(Word addr, Byte val)
{
	if (addr < 0x8000)
	{
		return _rom[addr];
	}
	else if (addr >= 0x8000 && addr < 0xA000)
	{
		/*if (ppu.mode & PPU_MODE_VRAM)
		{
			//printf("VRAM WRITE\n");
			return 0xFF;
		}*/
		_vram[addr - 0x8000] = val;
		
		if (addr < 0x9800)
		{
			updateTile(addr);
		}
	}
	else if (addr >= 0xA000 && addr < 0xC000)
	{
		_eram[addr - 0xA000] = val;
	}
	else if (addr >= 0xC000 && addr < 0xE000)
	{
		_wram[addr - 0xC000] = val;
	}
	else if (addr >= 0xE000 && addr < 0xFE00)
	{
		_wram[addr - 0xE000] = val;
	}
	else if (addr >= 0xFE00 && addr < 0xFEA0)
	{
		/*if (ppu.mode & (PPU_MODE_VRAM | PPU_MODE_OAM))
		{
			//printf("OAM WRITE\n");
			return;
		}*/
		_oam[addr - 0xFE00] = val;
	}
	else if (addr >= 0xFEA0 && addr < 0xFF00)
	{
		//printf("There\n");
	}
	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
		_io[addr - 0xFF00] = val;

		switch (addr)
		{
		case 0xFF00:
			joypad.select = val; //& 0xCF; // Mask off bottom 4 bits
			return;
		case 0xFF01:
			serialData = val;
			return;
		case 0xFF02:
			serialControl = val;
			return;
		case 0xFF04:
			timer.div = 0;
			timer.clock = 0;
			return;
		case 0xFF05:
			timer.tima = val;
			return;
		case 0xFF06:
			timer.tma = val;
			return;
		case 0xFF07:
			timer.tac = val;
			return;
		case 0xFF0F:
			interrupt.flags = val | 0xE0;
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
			return;
		case 0xFF48:
			for (int i = 0; i < 4; ++i)
			{
				objPalette[0][i] = palette[(val >> (i * 2)) & 3];
			}
			return;
		case 0xFF49:
			for (int i = 0; i < 4; ++i)
			{
				objPalette[1][i] = palette[(val >> (i * 2)) & 3];
			}
			return;
		case 0xFF50:
			if (val != 0 && inbios)
			{
				printf("Boot unloaded\n");
				for (int i = 0; i < 0x100; ++i)
				{
					_rom[i] = _romStart[i];
				}
				inbios = 0;

				/*for (int i = 0xFF00; i < 0xFF80; ++i)
				{
					printf("%4X %02X\n", i, rb(i));
				}*/
			}
			return;
		case 0xFF4A:
			ppu.wy = val;
			return;
		case 0xFF4B:
			ppu.wx = val;
			return;
		default:
			break;
		}
	}
	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		_hram[addr - 0xFF80] = val;
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
}

Word popFromStack()
{
	Word value = rw(reg.SP);
	reg.SP += 2;
	return value;
}

void pushToStack(Word val)
{
	reg.SP -= 2;
	ww(reg.SP, val);
}

Word nextByte()
{
	//cpuclock += 1;
	Word op = rb(reg.PC);
	reg.PC++;
	return op;
}

Word nextWord() 
{
	//cpuclock += 2;
	Word op = rw(reg.PC);
	reg.PC += 2;
	return op;
}