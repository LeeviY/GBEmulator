/*#pragma once

#include <stdint.h>

#include "memory.h"
#include "cpu.h"

typedef unsigned char Byte;
typedef uint16_t Word;

// Gpu mode codes
enum GpuMode
{
	GPU_MODE_HBLANK,
	GPU_MODE_VBLANK,
	GPU_MODE_OAM,
	GPU_MODE_VRAM
};

// Struct for storing gpu variables
struct Gpu
{
	unsigned long clock;
	unsigned long target;
	int mode;
	int scan;

	Byte lcdc;	// FF40
	Byte scy;	// FF42
	Byte scx;	// FF43
	Byte ly;	// FF44
	Byte lyc;	// FF45
	Byte wy;	// FF4B
	Byte wx;	// FF4A

} extern gpu;

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

extern Byte tiles[384][8][8];
extern Byte frameBuffer[160 * 144][3];

// Used for debug
void printTileMap();
void printFrameBuffer();

// Reset gpu to default values
void resetGpu();

// Steps gpus by one tick
void stepGPU();

void renderScan();

void updateTile(Word addr);*/