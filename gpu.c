/*#define LCDC_ENABLE (1 << 7)
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

struct Gpu gpu;

const Byte palette[4] = { 255, 192, 96, 0 };
Byte bgPalette[4];
Byte objPalette[2][4];

Byte tiles[384][8][8];
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

void resetGpu()
{
	gpu.clock = 0;
	gpu.mode = GPU_MODE_HBLANK;
	gpu.ly = 0;

	gpu.lcdc = 0;
	gpu.scx = 0;
	gpu.scy = 0;
}

const static int multiplier = 1;

void stepGPUn()
{
	interrupt.flags = 0;
	gpu.clock += cpu.clock;

	if (!(gpu.lcdc & LCDC_ENABLE))
	{
		return;
	}

	if (gpu.clock >= gpu.target)
	{
		return;
	}

	if (gpu.ly == -1)
	{
		gpu.mode = GPU_MODE_OAM;
		gpu.ly = -1;

		gpu.clock %= 70224;
		gpu.target %= 70224;
	}

	//STAT

	switch (gpu.mode)
	{
	case GPU_MODE_OAM:
		gpu.target += 80 * multiplier;
		gpu.mode = GPU_MODE_VRAM;
		gpu.ly++;

		break;

	case GPU_MODE_VRAM:
		gpu.target += 170 * multiplier;
		gpu.mode = GPU_MODE_HBLANK;

		break;

	case GPU_MODE_HBLANK:
		gpu.target += 206 * multiplier;

		if (gpu.ly <= 143)
		{
			renderScan();
			gpu.mode = GPU_MODE_OAM;
		}
		else
		{
			gpu.mode = GPU_MODE_VBLANK;
		}

		break;

	case GPU_MODE_VBLANK:
		gpu.target += 456 * multiplier;
		gpu.mode = GPU_MODE_VBLANK;


		if (gpu.ly++ == 144)
		{
			interrupt.flags |= INT_VBLANK;
			reDisplay();
		}

		break;
	}
}

void stepGPU()
{
	gpu.clock += cpu.clock;

	switch (gpu.mode)
	{
	case GPU_MODE_OAM:
		if (gpu.clock >= 80 * multiplier)
		{
			gpu.mode = GPU_MODE_VRAM;
			gpu.clock -= 80 * multiplier;
		}
		break;

	case GPU_MODE_VRAM:
		if (gpu.clock >= 172 * multiplier)
		{
			gpu.mode = GPU_MODE_HBLANK;

			renderScan();
			gpu.clock -= 172 * multiplier;
		}
		break;

	case GPU_MODE_HBLANK:
		if (gpu.clock >= 204 * multiplier)
		{
			if (gpu.ly++ == 143)
			{
				interrupt.flags |= INT_VBLANK;
				gpu.mode = GPU_MODE_VBLANK;
				reDisplay();
			}
			else
			{
				gpu.mode = GPU_MODE_OAM;
			}

			gpu.clock -= 204 * multiplier;
		}
		break;

	case GPU_MODE_VBLANK:
		if (gpu.clock >= 456 * multiplier)
		{
			gpu.ly++;

			if (gpu.ly > 153)
			{
				gpu.mode = GPU_MODE_OAM;
				gpu.ly = 0;
			}
			gpu.clock -= 456 * multiplier;
		}
		break;
	}
}

void stepGPU1()
{
	gpu.clock += cpu.clock / 4;// - gpu.lastClock;

	//gpu.lastClock = cpu.clock;

	//printf("%d %d\n", gpu.clock, gpu.lastClock);

	switch (gpu.mode)
	{
	case GPU_MODE_HBLANK:
		if (gpu.clock >= 51)
		{
			if (gpu.ly++ == 143)
			{
				interrupt.flags |= INT_VBLANK;
				gpu.mode = GPU_MODE_VBLANK;
				reDisplay();
			}
			else
			{
				gpu.mode = GPU_MODE_OAM;
			}
			gpu.clock = 0;
		}
		break;

	case GPU_MODE_VBLANK:
		if (gpu.clock >= 114)
		{
			gpu.ly++;
			gpu.clock = 0;

			if (gpu.ly > 153)
			{
				gpu.mode = GPU_MODE_OAM;
				gpu.ly = 0;
			}
		}
		break;

	case GPU_MODE_OAM:
		if (gpu.clock >= 20)
		{
			gpu.mode = GPU_MODE_VRAM;
			gpu.clock = 0;
		}
		break;

	case GPU_MODE_VRAM:
		if (gpu.clock >= 43)
		{
			gpu.mode = GPU_MODE_HBLANK;
			gpu.clock = 0;

			if (gpu.lcdc & LCDC_ENABLE)
			{
				renderScan();
			}
		}
		break;
	}
}

static int window_ly = 0;

void scanBackground1(Byte* scan)
{
	int bgy = gpu.scy;
	int bgx = gpu.scx;
	int wy = gpu.wy;
	int wx = gpu.wx - 7;

	Word bgOffset = gpu.lcdc & LCDC_TILEMAP ? 0x1C00 : 0x1800;
	Word windowMap = gpu.lcdc & LCDC_WINTILEMAP ? 0x1C00 : 0x1800;

	int offset = bgx & 0B111;

	if (gpu.lcdc & LCDC_WINENABLE && wy <= gpu.ly && wx < 160)
	{
		window_ly++;
	}

	for (int x = 0; x < 160; ++x)
	{
		if (gpu.lcdc & LCDC_WINENABLE && wy <= gpu.ly && wx <= x)
		{
			int wt = _vram[windowMap + window_ly / 8 * 32 % 0x400 + (x - wx) / 8 % 32];

			if (!(gpu.lcdc & LCDC_TILEDATA))
			{
				wt = (wt ^ 0x80) + 128;
			}

			frameBuffer[gpu.ly * 160 + x][0] = tiles[8 * wt + window_ly % 8][(x - wx) % 8];
			frameBuffer[gpu.ly * 160 + x][1] = tiles[8 * wt + window_ly % 8][(x - wx) % 8];
			frameBuffer[gpu.ly * 160 + x][2] = tiles[8 * wt + window_ly % 8][(x - wx) % 8];
		}
		else if (gpu.lcdc & LCDC_BGENABLE)
		{
			int bt = _vram[bgOffset + ((gpu.ly + bgy) / 8 * 32) % 0x400 + ((x + bgx) / 8) % 32];

			if (!(gpu.lcdc & LCDC_TILEDATA))
			{
				bt = (bt ^ 0x80) + 128;
			}
			frameBuffer[gpu.ly * 160 + x][0] = tiles[8 * bt + (gpu.ly + bgy) % 8][(x - offset) % 8];
			frameBuffer[gpu.ly * 160 + x][1] = tiles[8 * bt + (gpu.ly + bgy) % 8][(x - offset) % 8];
			frameBuffer[gpu.ly * 160 + x][2] = tiles[8 * bt + (gpu.ly + bgy) % 8][(x - offset) % 8];
		}
		else
		{
			frameBuffer[gpu.ly * 160 + x][0] = palette[0];
			frameBuffer[gpu.ly * 160 + x][1] = palette[0];
			frameBuffer[gpu.ly * 160 + x][2] = palette[0];
		}
	}

	if (gpu.ly == 143)
	{
		window_ly = -1;
	}
}

void scanBackground(Byte* scan)
{
	int showWindow = gpu.lcdc & LCDC_WINENABLE && gpu.wy <= gpu.ly;

	int tileBase = (gpu.lcdc & LCDC_TILEMAP) ? 0x1C00 : 0x1800;


	tileBase += (((gpu.ly + gpu.scy) & 0xFF) >> 3) << 5;
	int lineOffs = (gpu.scx >> 3);
	int canvasOffs = gpu.ly * 160;

	int x = gpu.scx & 7;
	int y = (gpu.ly + gpu.scy) & 7;

	Word tile = _vram[tileBase + lineOffs];

	for (int i = 0; i < 160; i++)
	{
		// Get tile color from palette
		Byte color = tiles[tile][y][x];
		scan[i] = color;

		// Add pixel/color to framebuffer
		frameBuffer[canvasOffs + i][0] = bgPalette[color];
		frameBuffer[canvasOffs + i][1] = bgPalette[color];
		frameBuffer[canvasOffs + i][2] = bgPalette[color];

		// Next tile
		if (++x == 8)
		{
			x = 0;
			lineOffs = (lineOffs + 1) & 31;
			tile = _vram[tileBase + lineOffs];
		}
	}
}

void scanObjects(Byte* scan)
{
	for (int i = 0; i < 40; i += 4)
	{
		struct Object obj = { _oam[i], _oam[i + 1], _oam[i + 2], _oam[i + 3] };

		int objX = obj.x - 8;
		int objY = obj.y - 16;

		if (objY <= gpu.ly && (objY + 8) > gpu.ly)
		{
			Byte* objPal = objPalette[obj.options & OBJ_PALETTE];

			int canvasOffset = gpu.ly * 160 + objX;

			Byte tileRow;
			if (obj.options & OBJ_YFLIP)
			{
				tileRow = 7 - (gpu.ly - objY);
			}
			else
			{
				tileRow = gpu.ly - objY;
			}

			for (int x = 0; x < 8; ++x)
			{
				int offsetX = objX + x;
				if (offsetX >= 0 && offsetX < 160 && (~obj.options & OBJ_PRIORITY || !scan[offsetX]))
				{
					Byte color;

					if (obj.options & OBJ_XFLIP)
					{
						color = tiles[obj.tile][tileRow][7 - x];
					}
					else
					{
						color = tiles[obj.tile][tileRow][x];
					}

					if (color)
					{
						frameBuffer[canvasOffset + x][0] = objPal[color];
						frameBuffer[canvasOffset + x][1] = objPal[color];
						frameBuffer[canvasOffset + x][2] = objPal[color];
					}
				}
			}
		}
	}
}

void renderScan()
{
	if (!(gpu.lcdc & LCDC_ENABLE))
	{
		return;
	}

	Byte scanlineRow[160] = { 0 };

	if (gpu.lcdc & LCDC_BGENABLE)
	{
		scanBackground(&scanlineRow);
	}
	
	if (gpu.lcdc & LCDC_OBJENABLE)
	{
		scanObjects(&scanlineRow);
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
}*/
