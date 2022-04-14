#define LCDC_ENABLE (1 << 7)
#define LCDC_WINTILEMAP (1 << 6)
#define LCDC_WINENABLE (1 << 5)
#define LCDC_TILEDATA (1 << 4)
#define LCDC_TILEMAP (1 << 3)
#define LCDC_OBJSIZE (1 << 2)
#define LCDC_OBJENABLE (1 << 1)
#define LCDC_BGENABLE (1 << 0)

#define OBJ_PRIORITY (1 << 7)
#define OBJ_YFLIP (1 << 6)
#define OBJ_XFLIP (1 << 5)
#define OBJ_PALETTE (1 << 4)

#include "gpu.h"

struct Gpu gpu =
{
	.mode = GPU_MODE_HBLANK,
	.line = 0
};

Byte tiles[384][8][8];

const Byte palette[4] = { 255, 192, 96, 0 };

Byte bgPalette[4];
Byte objPalette[2][4];

Byte frameBuffer[160 * 144][3];

void printTileMap()
{
	for (int i = 0; i < 24; ++i)
	{
		for (int y = 0; y < 8; ++y)
		{
			printf("\n");
			for (int j = 0; j < 16; ++j)
			{
				for (int x = 0; x < 8; ++x)
				{
					int color = tiles[j + i * 24][y][x];

					//printf("%d ", color);
					switch (color)
					{
					case 3:
						printf("0");
						break;
					case 2:
						printf("*");
						break;
					case 1:
						printf(".");
						break;
					case 0:
						printf(" ");
						break;
					}
				}
			}
		}
	}
	printf("\n");
}

void printFrameBuffer()
{
	for (int i = 0; i < 160 * 144; ++i)
	{
		if (i % 160 == 0) printf("\n");
		int a = frameBuffer[i];
		switch (a)
		{
		case 255:
			printf("0");
			break;
		case 192:
			printf("*");
			break;
		case 96:
			printf(".");
			break;
		case 0:
			printf(" ");
			break;
		default:
			printf("X");
			break;
		}
		
	}
	printf("\n");
}

void reset()
{

}

void stepGPU()
{
	gpu.clock += cpu.clock - gpu.lastClock;

	gpu.lastClock = cpu.clock;

	//printf("%d", gpu.mode);

	switch (gpu.mode)
	{
	case GPU_MODE_OAM:
		if (gpu.clock >= 80)
		{
			gpu.mode = GPU_MODE_VRAM;
			gpu.clock -= 80;
		}
		break;

	case GPU_MODE_VRAM:
		if (gpu.clock >= 172)
		{
			gpu.mode = GPU_MODE_HBLANK;

			renderScan();
			gpu.clock -= 172;
		}
		break;

	case GPU_MODE_HBLANK:
		if (gpu.clock >= 204)
		{
			gpu.line++;
			gpu.mode = GPU_MODE_HBLANK;

			if (gpu.line == 143)
			{
				interrupt.flags |= VBLANK;
				gpu.mode = GPU_MODE_VBLANK;
				//drawTileMap();
				drawFrameBuffer();
			}
			else
			{
				gpu.mode = GPU_MODE_OAM;
			}
			gpu.clock -= 204;
		}
		break;

	case GPU_MODE_VBLANK:
		if (gpu.clock >= 456)
		{
			gpu.line++;

			if (gpu.line > 153)
			{
				gpu.mode = GPU_MODE_OAM;
				gpu.line = 0;
			}
			gpu.clock -= 456;
		}
		break;
	}
}

void renderScan()
{
	// Check if lcd is on
	/*if (!(gpu.lcdc & LCDC_ENABLE))
	{
		return;
	}*/

	int mapOffs = (gpu.lcdc & LCDC_TILEMAP) ? 0x1C00 : 0x1800;

	mapOffs += (((gpu.line + gpu.scy) & 0xFF) >> 3) << 5;

	//printf("%04X\n", mapOffs + 0x8000);

	int lineOffs = (gpu.scx >> 3);

	int x = gpu.scx & 7;
	int y = (gpu.line + gpu.scy) & 7;
	
	int canvasOffs = gpu.line * 160;

	Word tile = _vram[mapOffs + lineOffs];

	if (gpu.lcdc & LCDC_BGENABLE)
	{
		for (int i = 0; i < 160; i++)
		{
			// Get tile color from palette
			Byte color = tiles[tile][y][x];

			// Add pixel/color to framebuffer
			frameBuffer[canvasOffs + i][0] = bgPalette[color];
			frameBuffer[canvasOffs + i][1] = bgPalette[color];
			frameBuffer[canvasOffs + i][2] = bgPalette[color];

			// Next tile

			//canvasOffs++;

			if (++x == 8)
			{
				x = 0;
				lineOffs = (lineOffs + 1) & 31;
				tile = _vram[mapOffs + lineOffs];
			}
		}
	}
	
	if (gpu.lcdc & LCDC_OBJENABLE)
	{
	}
}

void updateTile(Word addr)
{
	//printf("Update tile %04X\n", addr);
	addr &= 0x1FFE;

	Word tile = (addr >> 4) & 0x1FF;

	Word y = (addr >> 1) & 7;

	for (Byte x = 0; x < 8; ++x) 
	{
		Byte bit = 1 << (7 - x);

		tiles[tile][y][x] = ((_vram[addr] & bit) ? 1 : 0) + ((_vram[addr + 1] & bit) ? 2 : 0);
	}
}
