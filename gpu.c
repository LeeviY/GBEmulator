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

Byte frameBuffer[160 * 144];

// SFML
sfRenderWindow* window;

void drawWindow()
{
	//printf("WINDOW DRAW\n\n");

	sfIntRect rect = { 160, 144 };
	sfSprite* sprite = sfSprite_create();

	sfImage* image = sfImage_create(160, 144);

	/*for (int i = 0; i < (160 * 144); ++i)
	{
		if ((i+1) % 160 == 0) printf("%02d \n", i);
		printf("%d ", frameBuffer[i]);
	}*/

	for (int i = 0; i < (160 * 144); ++i)
	{
		sfColor color = { palette[frameBuffer[i]], palette[frameBuffer[i]], palette[frameBuffer[i]], 255 };
		sfImage_setPixel(image, i % 160, i % 144, color);
	}

	sfTexture* texture = sfTexture_createFromImage(image, &rect);
	sfSprite_setTexture(sprite, texture, 0);

	sfRenderWindow_drawSprite(window, sprite, NULL);
	sfWindow_display(window);

	sfSprite_destroy(sprite);
	sfImage_destroy(image);
	sfTexture_destroy(texture);
}

void reset()
{

}

void stepGPU()
{
	gpu.clock = 0;

	gpu.clock += cpu.clock;

	//printf("Line %d %d ", gpu.line, gpu.clock);

	switch (gpu.mode)
	{
	case GPU_MODE_OAM:
		if (gpu.clock >= 80)
		{
			gpu.clock -= 80;
			gpu.mode = GPU_MODE_VRAM;
		}
		break;
	case GPU_MODE_VRAM:
		if (gpu.clock >= 172)
		{
			gpu.clock -= 172;
			gpu.mode = GPU_MODE_HBLANK;

			renderScan();
		}
		break;
	case GPU_MODE_HBLANK:
		if (gpu.clock >= 204)
		{
			gpu.clock -= 204;
			gpu.line++;
			gpu.mode = GPU_MODE_HBLANK;

			if (gpu.line == 143)
			{
				gpu.mode = GPU_MODE_VBLANK;
				drawWindow();
			}
			else
			{
				gpu.mode = GPU_MODE_OAM;
			}
		}
		break;
	case GPU_MODE_VBLANK:
		if (gpu.clock >= 456)
		{
			gpu.clock -= 456;
			gpu.line++;

			if (gpu.line > 153)
			{
				// Restart scan
				gpu.mode = GPU_MODE_OAM;
				gpu.line = 0;
			}
		}
		break;
	default:
		break;
	}
}

void renderScan()
{
	int mapOffs = (gpu.lcdc & LCDC_TILEMAP) ? 0x1C00 : 0x1800;
	mapOffs += (((gpu.line + gpu.scy) & 255) >> 3) << 5;
	int lineOffs = (gpu.scx >> 3);

	int x = gpu.scx & 7;
	int y = (gpu.line + gpu.scy) & 7;
	
	int canvasOffs = gpu.line * 160;

	Word tile = _vram[mapOffs + lineOffs];

	Byte scanlineRow[160];

	/*if (gpu.lcdc & LCDC_TILEDATA && tile < 128)
		tile += 256;*/

	for (int i = 0; i < 160; i++)
	{
		// Get tile color from palette
		Byte color = tiles[tile][y][x];

		scanlineRow[i] = color;

		// Add pixel/color to framebuffer
		frameBuffer[canvasOffs] = bgPalette[palette[color]];

		// Next tile
		if (++x == 8)
		{
			x = 0;
			lineOffs = (lineOffs + 1) & 31;
			tile = _vram[mapOffs + lineOffs];
			/*if (gpu.lcdc & LCDC_TILEMAP && tile < 128)
				tile += 256;*/
		}
	}

	for (int i = 0; i < 40; i++) 
	{
		struct Object obj = { _oam[i], _oam[i] + 1, _oam[i] + 2, _oam[i] + 3 };

		int ox = obj.x - 8;
		int oy = obj.y - 16;

		if (oy <= gpu.line && (oy + 8) > gpu.line)
		{
			Byte* pal = objPalette[obj.options & OBJ_PALETTE];

			int pixelOffset;
			pixelOffset = gpu.line * 160 + ox;

			Byte tileRow;
			if (obj.options & OBJ_YFLIP) 
				tileRow = 7 - (gpu.line - oy);
			else 
				tileRow = gpu.line - oy;

			int x;
			for (x = 0; x < 8; x++) 
			{
				if (ox + x >= 0 && ox + x < 160 && (~(obj.options & OBJ_PRIORITY) ||
					!scanlineRow[ox + x]))
				{
					Byte color;

					if (obj.options & OBJ_XFLIP)
						color = tiles[obj.tile][tileRow][7 - x];
					else
						color = tiles[obj.tile][tileRow][x];

					if (color)
						frameBuffer[pixelOffset] = pal[color];
				}
			}
		}
	}
}

void updateTile(Word addr, Byte val)
{
	//printf("Update tile %x %x\n", addr, val);
	addr &= 0x1FFE;

	Word tile = (addr >> 4) & 511;
	Word y = (addr >> 1) & 7;

	Byte bitIndex;
	for (Byte x = 0; x < 8; x++) 
	{
		bitIndex = 1 << (7 - x);
		
		tiles[tile][y][x] = ((_vram[addr] & bitIndex) ? 1 : 0) + ((_vram[addr + 1] & bitIndex) ? 2 : 0);
	}
}
