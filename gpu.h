#pragma once

#include <stdint.h>
#include <SFML/Graphics.h>

#include "memory.h"
#include "cpu.h"

typedef unsigned char Byte;
typedef uint16_t Word;

// Gpu mode codes
enum GpuMode
{
	GPU_MODE_OAM = 2,
	GPU_MODE_VRAM = 3,
	GPU_MODE_HBLANK = 0,
	GPU_MODE_VBLANK = 1,
};

// Struct for storing gpu variables
struct Gpu
{
	unsigned long clock;
	int mode;
	int line;

	Byte lcdc;
	Byte scx;
	Byte scy;

} extern gpu;

struct Object
{
	Byte y;
	Byte x;
	Byte tile;
	Byte options;
} extern obj;

extern Byte tiles[384][8][8];

extern const Byte palette[4];

extern Byte bgPalette[4];
extern Byte objPalette[2][4];

extern Byte frameBuffer[160 * 144];

// SFML draw
extern sfRenderWindow* window;
void drawWindow();

// Reset gpu to default values
void reset();

// Steps gpus by one tick
void stepGPU();

void renderScan();

void updateTile(Word addr, Byte val);