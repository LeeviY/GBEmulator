#pragma once

#include <stdint.h>

#include "cpu.h"
#include "memory.h"

typedef unsigned char Byte;
typedef uint16_t Word;

enum
{
	LCDC_ENABLE		= (1 << 7),
	LCDC_WINTILEMAP	= (1 << 6),
	LCDC_WINENABLE	= (1 << 5),
	LCDC_TILEDATA	= (1 << 4),
	LCDC_TILEMAP	= (1 << 3),
	LCDC_OBJSIZE	= (1 << 2),
	LCDC_OBJENABLE	= (1 << 1),
	LCDC_BGENABLE	= (1 << 0)
};

enum
{
	STAT_LY_ENABLE		= (1 << 6),
	STAT_OAM_ENABLE		= (1 << 5),
	STAT_VBLANK_ENABLE	= (1 << 4),
	STAT_HBLANK_ENABLE	= (1 << 3),
	STAT_LY_FLAG		= (1 << 2),
	// 0 and 1 are modes
};

enum
{
	OBJ_PRIORITY	= (1 << 7),
	OBJ_YFLIP		= (1 << 6),
	OBJ_XFLIP		= (1 << 5),
	OBJ_PALETTE		= (1 << 4)
};

enum
{
	PPU_MODE_HBLANK,
	PPU_MODE_VBLANK,
	PPU_MODE_OAM,
	PPU_MODE_VRAM
};

struct Ppu
{
	int clock;
	int mode;

	Byte lcdc;	// FF40
	Byte stat;	// FF41
	Byte scy;	// FF42
	Byte scx;	// FF43
	Byte ly;	// FF44
	Byte lyc;	// FF45
	Byte lyWin;
	Byte wy;	// FF4B
	Byte wx;	// FF4A

} extern ppu;

struct Object
{
	Byte y;
	Byte x;
	Byte tile;
	Byte options;
};

extern const Byte palette[4];
extern Byte bgPalette[4];
extern Byte objPalette[2][4];
extern Byte tiles[16 * 24][8][8];
extern Byte frameBuffer[160 * 144][3];

void resetPpu();
void stepPpu();

void drawLine(Byte* scan);
void drawObjects(Byte* scan);
void updateTile(Word addr);