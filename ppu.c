#include "ppu.h"
#include "debug.h"

struct Ppu ppu;

const Byte palette[4] = { 255, 192, 96, 0 };
Byte bgPalette[4] = { 0 };
Byte objPalette[2][4] = { 0 };
Byte tiles[384][8][8] = { 0 };
Byte frameBuffer[160 * 144][3] = { 0 };


void resetPpu()
{
	ppu.clock = 0;
	ppu.mode = PPU_MODE_HBLANK;
	ppu.lcdc = 0;
	ppu.scx = 0;
	ppu.scy = 0;
	ppu.ly = 0;
	ppu.lyc = 0;
	ppu.lyWin = 0;
	ppu.wy = 0;
	ppu.wx = 0;
}

static int multiplier = 1;
void stepPpu1()
{
	if (!(ppu.lcdc & LCDC_ENABLE))
	{
		return;
	}

	if (cpu.clock == 0)
	{
		return;
	}

	ppu.clock += cpu.clock;

	switch (ppu.mode)
	{
	case PPU_MODE_OAM:
		if (ppu.clock >= 20 * multiplier)
		{
			ppu.mode = PPU_MODE_VRAM;
			ppu.clock -= 20 * multiplier;
		}
		break;

	case PPU_MODE_VRAM:
		if (ppu.clock >= 43 * multiplier)
		{

			Byte scan[160];
			drawLine(scan);
			drawObjects(scan);

			ppu.mode = PPU_MODE_HBLANK;
			ppu.clock -= 43 * multiplier;
		}
		break;

	case PPU_MODE_HBLANK:
		if (ppu.clock >= 51 * multiplier)
		{
			if (ppu.ly++ == 143)
			{
				reDisplay();
				interrupt.flags |= INT_VBLANK;

				ppu.mode = PPU_MODE_VBLANK;
			}
			else
			{
				ppu.mode = PPU_MODE_OAM;
			}

			ppu.clock -= 51 * multiplier;
		}
		break;

	case PPU_MODE_VBLANK:
		if (ppu.clock >= 114 * multiplier)
		{
			ppu.ly++;

			if (ppu.ly > 153)
			{
				ppu.mode = PPU_MODE_OAM;
				ppu.ly = 0;
			}
			ppu.clock -= 114 * multiplier;

			if (ppu.stat & STAT_LY_ENABLE && ppu.ly == ppu.lyc)
			{
				interrupt.flags |= INT_LCD_STAT;
			}
		}
		break;
	}
}

void stepPpu()
{
	if (!(ppu.lcdc & LCDC_ENABLE))
	{
		return;
	}

	if (cpu.clock == 0)
	{
		return;
	}

	ppu.clock += cpu.clock * multiplier;

	switch (ppu.mode)
	{
	case PPU_MODE_OAM:
		if (ppu.clock >= 80)
		{
			ppu.mode = PPU_MODE_VRAM;
			ppu.stat |= PPU_MODE_VRAM;

			ppu.clock -= 80;
		}
		break;

	case PPU_MODE_VRAM:
		if (ppu.clock >= 144)
		{
			Byte scan[160];
			drawLine(scan);
			drawObjects(scan);

			ppu.mode = PPU_MODE_HBLANK;

			if (ppu.stat & STAT_HBLANK_ENABLE)
			{
				interrupt.flags |= INT_LCD_STAT;
			}
			ppu.clock -= 144;
		}
		break;

	case PPU_MODE_HBLANK:
		if (ppu.clock >= 204)
		{
			if (ppu.ly++ == 143)
			{
				reDisplay();
				interrupt.flags |= INT_VBLANK;

				ppu.mode = PPU_MODE_VBLANK;

				if (ppu.stat & STAT_VBLANK_ENABLE)
				{
					interrupt.flags |= INT_LCD_STAT;
				}
			}
			else
			{
				ppu.mode = PPU_MODE_OAM;

				if (ppu.stat & STAT_OAM_ENABLE)
				{
					interrupt.flags |= INT_LCD_STAT;
				}
			}

			//printf("%d %d\n", ppu.stat, ppu.lyc);
			if (ppu.ly == ppu.lyc)
			{
				ppu.stat |= STAT_LY_FLAG;
				if (ppu.stat & STAT_LY_ENABLE)
				{
					interrupt.flags |= INT_LCD_STAT;
				}
			}
			else
			{
				ppu.stat &= ~STAT_LY_FLAG;
			}

			ppu.clock -= 204;
		}
		break;

	case PPU_MODE_VBLANK:
		if (ppu.clock >= 456)
		{
			ppu.ly++;

			if (ppu.ly > 153)
			{
				ppu.mode = PPU_MODE_OAM;

				if (ppu.stat & STAT_OAM_ENABLE)
				{
					interrupt.flags |= INT_LCD_STAT;
				}
				ppu.ly = 0;
			}

			if (ppu.stat & STAT_LY_ENABLE && ppu.ly == ppu.lyc)
			{
				interrupt.flags |= INT_LCD_STAT;
			}

			ppu.clock -= 456;
		}
		break;
	}
}

/*void drawLine1(Byte* scanPriority)
{
	int x, offset = ppu.ly * 160;
	int windowVisible = ppu.ly >= ppu.wy && ppu.lcdc & LCDC_WINENABLE && ppu.ly - ppu.wy < 144;

	for (x = 0; x < 160; ++x, ++offset)
	{
		unsigned int map_select, map_offset, tile_num, tile_addr, xm, ym;
		Byte b1, b2, mask, colour;

		if (windowVisible && x + 7 >= ppu.wx)
		{
			xm = x + 7 - ppu.wx;
			ym = ppu.ly - ppu.wy;
			map_select = ppu.lcdc & LCDC_TILEMAP;
		}
		else 
		{
			if (!(ppu.lcdc & LCDC_BGENABLE))
			{
				frameBuffer[offset][0] = palette[bgPalette[0]];
				frameBuffer[offset][1] = palette[bgPalette[0]];
				frameBuffer[offset][2] = palette[bgPalette[0]];
				continue;
			}
			xm = (x + ppu.scx) & 0xFF;
			ym = (ppu.ly + ppu.scy) & 0xFF;
			map_select = ppu.lcdc & LCDC_TILEMAP;
		}

		map_offset = (ym / 8) * 32 + xm / 8;

		tile_num = -_vram[0x1800 + map_select * 0x400 + map_offset];

		if (!(ppu.lcdc & LCDC_TILEDATA))
		{
			tile_num = (signed int)(tile_num)+128;
		}

		tiles[tile_num][ym][(x - (ppu.scx & 7)) % 8];

		b1 = _vram[tile_addr + (ym & 7) * 2];
		b2 = _vram[tile_addr + (ym & 7) * 2 + 1];
		mask = 128 >> (xm & 7);
		colour = (!!(b2 & mask) << 1) | !!(b1 & mask);

		scanPriority[x] = colour;

		frameBuffer[offset][0] = palette[bgPalette[colour]];
		frameBuffer[offset][1] = palette[bgPalette[colour]];
		frameBuffer[offset][2] = palette[bgPalette[colour]];
	}
}*/

void frameBufferSetColor(Byte color, int x)
{
	switch (color)
	{
	case 255:
		frameBuffer[ppu.ly * 160 + x][0] = 216;
		frameBuffer[ppu.ly * 160 + x][1] = 247;
		frameBuffer[ppu.ly * 160 + x][2] = 215;
		break;
	case 192:
		frameBuffer[ppu.ly * 160 + x][0] = 108;
		frameBuffer[ppu.ly * 160 + x][1] = 166;
		frameBuffer[ppu.ly * 160 + x][2] = 108;
		break;
	case 96:
		frameBuffer[ppu.ly * 160 + x][0] = 31;
		frameBuffer[ppu.ly * 160 + x][1] = 89;
		frameBuffer[ppu.ly * 160 + x][2] = 74;
		break;
	case 0:
		frameBuffer[ppu.ly * 160 + x][0] = 10;
		frameBuffer[ppu.ly * 160 + x][1] = 29;
		frameBuffer[ppu.ly * 160 + x][2] = 36;
		break;
	default:
		printf("COLOR NOT FOUND %d\n", color);
		break;
	}

	// BW
	/*frameBuffer[ppu.ly * 160 + x][0] = color;
	frameBuffer[ppu.ly * 160 + x][1] = color;
	frameBuffer[ppu.ly * 160 + x][2] = color;*/
}

void drawLine(Byte* scanPriority)
{
	Byte scy = ppu.scy;
	Byte scx = ppu.scx;
	Byte wy = ppu.wy;
	Byte wx = ppu.wx - 7;
	Byte ly = ppu.ly;

	Byte winEnable = ppu.lcdc & LCDC_WINENABLE && wy <= ly;
	Byte tileData = !(ppu.lcdc & LCDC_TILEDATA);
	
	Word bgMap = ppu.lcdc & LCDC_TILEMAP ? 0x1C00 : 0x1800;
	Word windowMap = ppu.lcdc & LCDC_WINTILEMAP ? 0x1C00 : 0x1800;

	if (winEnable && wx < 160)
	{
		ppu.lyWin++;
	}

	windowMap += ppu.lyWin / 8 * 32 % 0x400;
	bgMap += (ly + scy) / 8 * 32 % 0x400;

	for (int x = 0; x < 160; ++x)
	{
		if (winEnable && wx <= x)
		{
			int windowTile = _vram[windowMap + (x - wx) / 8 % 32];

			if (tileData)
			{
				windowTile = (windowTile ^ 0x80) + 128;
			}

			Byte color = bgPalette[tiles[windowTile][ppu.lyWin % 8][(x - wx) % 8]];
			scanPriority[x] = 4;

			frameBufferSetColor(color, x);
			/*if (x % 8 == 0)
			{
				frameBuffer[ly * 160 + x][0] = 0;
				frameBuffer[ly * 160 + x][1] = 255;
				frameBuffer[ly * 160 + x][2] = 0;
			}*/
		}
		else if (ppu.lcdc & LCDC_BGENABLE)
		{
			int bgTile = _vram[bgMap + (x + scx) / 8 % 32];

			if (tileData)
			{
				bgTile = (bgTile ^ 0x80) + 128;
			}

			Byte color = tiles[bgTile][(ly + scy) % 8][(x + (scx & 7)) % 8];
			scanPriority[x] = color;
			color = bgPalette[color];

			frameBufferSetColor(color, x);
			/*if (x % 8 == 0)
			{
				frameBuffer[ly * 160 + x][0] = 255;
				frameBuffer[ly * 160 + x][1] = 0;
				frameBuffer[ly * 160 + x][2] = 0;
			}*/
		}
		else
		{
			frameBufferSetColor(palette[0], x);
			/*if (x % 8 == 0)
			{
				frameBuffer[ly * 160 + x][0] = 0;
				frameBuffer[ly * 160 + x][1] = 0;
				frameBuffer[ly * 160 + x][2] = 255;
			}*/
		}
	}

	if (ly == 143)
	{
		ppu.lyWin = -1;
	}
}

void drawObjects(Byte* scanPriority)
{
	if (!(ppu.lcdc & LCDC_OBJENABLE))
	{
		return;
	}

	Byte ly = ppu.ly;
	int objectSize = ppu.lcdc & LCDC_OBJSIZE ? 16 : 8;

	// First byte is color second byte is start of object
	Byte scannedObj[160][2] = { 0 };
	int spriteCount = 0;

	for (int i = 0; i < 40 * 4; i += 4)
	{
		Byte objY = _oam[i] - 16;
		Byte objX = _oam[i + 1] - 8;
		Byte objTile = _oam[i + 2] & (ppu.lcdc & LCDC_OBJSIZE ? 0xFE : 0xFF);
		Byte objAttr = _oam[i + 3];

		if ((objY <= 0xFF - objectSize + 1 && ly < objY) || ly > objY + objectSize - 1)
		{
			continue;
		}

		if (objX >= 160 && objX <= 0xF8)
		{
			continue;
		}

		if (spriteCount++ == 10)
		{
			break;
		}

		Byte tileY = objAttr & OBJ_YFLIP ? objectSize - 1 - (ly - objY) : ly - objY;

		for (int x = 0; x < 8; ++x)
		{
			if (objX + x < 0 || objX + x >= 160)
			{
				continue;
			}

			// Check bg/window priority
			if ((!(objAttr & OBJ_PRIORITY) || !scanPriority[objX + x]) && scanPriority[objX + x] != 4)
			{
				// Check object priority
				if (!scannedObj[objX + x][0] || scannedObj[objX + x][1] > objX)
				{
					Byte tileX = objAttr & OBJ_XFLIP ? 7 - x : x;
					Byte color = tiles[objTile][tileY][tileX];

					if (color)
					{
						scannedObj[objX + x][0] = 1;
						scannedObj[objX + x][1] = objX;
						frameBufferSetColor(objPalette[objAttr & OBJ_PALETTE][color], objX + x);
					}
				}
			}
		}
	}
}

void updateTile(Word addr)
{
	addr &= 0x1FFE;

	Word tile = (addr >> 4) & 0x1FF;
	Word y = (addr >> 1) & 7;

	for (Byte x = 0; x < 8; ++x)
	{
		Byte bit = 1 << (7 - x);

		tiles[tile][y][x] = ((_vram[addr] & bit) ? 1 : 0) + ((_vram[addr + 1] & bit) ? 2 : 0);
	}
}