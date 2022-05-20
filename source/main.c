#include <stdlib.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <windows.h>
#include <GL/glut.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>

#include "cpu.h"
#include "gpu.h"
#include "debug.h"
#include "memory.h"
#include "joypad.h"

typedef unsigned char Byte;

// Framebuffer
unsigned int width = 160, height = 144;
const int scale = 4;

static int fps = 60;
static int displayMode = 0;

static time_t begin = 0;

// Draws framebuffer
void drawFrameBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(scale, -scale);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
	glutSwapBuffers();
}

void drawMemory()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(scale, -scale);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, memoryMap);
	glutSwapBuffers();
}

// Draws tilemap
void drawTileMap()
{
	static Byte data[192 * 128][3] = { 0 };

	for (int y = 0; y < 192; ++y)
	{
		for (int x = 0; x < 128; ++x)
		{
			Byte color = tiles[(y / 8) * 16 + (x / 8)][y % 8][x % 8];
			data[y * 128 + x][0] = palette[color];
			data[y * 128 + x][1] = palette[color];
			data[y * 128 + x][2] = palette[color];
			//printf("%d\n", color);
			//printf("%4d %4d\n", (8 * 24 - 1) - y, x);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(scale, -scale);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	glutSwapBuffers();
}

// Wrapper to be called outside main
void reDisplay()
{
	glutPostRedisplay();

	while (clock(NULL) % (1000 / fps) != 0)
	{

	}
}

// Swaps between drawing from framebuffer and tilemap
void swapDisplayMode()
{
	if (displayMode)
	{
		width = 160, height = 144;
		glutReshapeWindow(width * scale, height * scale);
		glutDisplayFunc(drawFrameBuffer);
		displayMode = 0;
	}
	else
	{
		width = 16 * 8, height = 24 * 8;
		glutReshapeWindow(width * scale, height * scale);
		glutDisplayFunc(drawTileMap);
		displayMode = 1;
	}
	glutPostRedisplay();
}

// Event handler for keyboard input
void keyInputHandler(Byte key, int x, int y)
{
	//printf("%c\n", key);

	/*
	* esc - exit
	* space - display cursor coordinates
	* s - step debugger
	* e - debugger on
	* d - debugger off
	* r - print instructions
	* f - swap between display and tilemap
	* t - double speed toggle
	* 
	* y - up
	* h - down
	* g - left
	* j - right
	* l - a
	* p - b
	* m - select
	* , - start
	*/

	switch (key) 
	{
	case 27:
		exit(0);
		glutDestroyWindow("GameBoy");
		break;
	case ' ':
	{
		int rx = x / scale;
		int ry = y / scale;
		printf("x=%3d y=%3d tileX=%2d tileY=%2d tileNum=0x%03X\n", rx, ry, rx / 8, ry / 8, (ry / 8) * 16 + rx / 8);
	}	
		break;
	case 's':
		debugStop = 0;
		break;
	case 'e':
		debugOn(0);
		debugStop = 0;
		break;
	case 'd':
		debugOff();
		break;
	case 'r':
		printToggle();
		break;
	case 'f':
		swapDisplayMode();
		break;
	case 't':
		if (fps == 60)
			fps = 120;
		else
			fps = 60;
	default:
		inputKeyDown(key);
		break;
	}
}

// Event handler for keyboard input
void keyInputHandlerUp(Byte key, int x, int y)
{
	if (!debugEnable)
	{
		inputKeyUp(key);
	}
}

static int remain = 0;
void cycleCpu(int a)
{
	glutTimerFunc(16, cycleCpu, 0);
	int i = remain;
	for (;i < 70224;i += cpu.clock)
	{
		stepCpu();
	}
	remain = i - 70224;
}

int main(int argc, char** argv)
{
	printf("%s", "Hello World!\n");

	// Set cpu states
	initCpu();

	//loadRom("cpu_instrs/individual/01-special");
	//loadRom("cpu_instrs/individual/02-interrupts");
	//loadRom("cpu_instrs/individual/03-op sp,hl");
	//loadRom("cpu_instrs/individual/04-op r,imm");
	//loadRom("cpu_instrs/individual/05-op rp");
	//loadRom("cpu_instrs/individual/06-ld r,r");
	//loadRom("cpu_instrs/individual/07-jr,jp,call,ret,rst");
	//loadRom("cpu_instrs/individual/08-misc instrs");
	//loadRom("cpu_instrs/individual/09-op r,r");
	//loadRom("cpu_instrs/individual/10-bit ops");
	//loadRom("cpu_instrs/individual/11-op a,(hl)");

	//loadRom("cpu_instrs/cpu_instrs");

	//loadRom("instr_timing/instr_timing");
	//loadRom("dmg-acid2");									// pass
	
	//loadRom("mts/acceptance/bits/reg_f");					// pass
	//loadRom("mts/acceptance/boot_regs-dmgABC");			// pass
	//loadRom("mts/acceptance/instr/daa");					// pass
	//loadRom("mts/acceptance/halt_ime0_ei");				// pass
	//loadRom("mts/acceptance/halt_ime1_timing");			// pass
	//loadRom("mts/acceptance/if_ie_registers");			// pass
	//loadRom("mts/acceptance/ei_sequence");				// pass
	//loadRom("mts/acceptance/ei_timing");					// pass

	//loadRom("mts/acceptance/ppu/intr_2_0_timing");
	//loadRom("mts/acceptance/ppu/intr_2_mode0_timing");

	//loadRom("mts/acceptance/oam_dma/reg_read");

	//loadRom("mts/acceptance/boot_hwio-dmgABCmgb");
	//loadRom("mts/acceptance/timer/tim00");
	//loadRom("mts/acceptance/ppu/intr_2_0_timing");

	//loadRom("Tetris");
	//loadRom("Dr_Mario");
	//loadRom("Alleyway");
	//loadRom("Legend of Zelda");
	//loadRom("Pokemon_Red");

	loadBootstrap("dmg_boot.bin");

	// glut window setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width * scale, height * scale);
	glutCreateWindow("GameBoy");
	glClearColor(0, 0, 0, 1);
	//glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Set glut event callback functions
	glutIdleFunc(stepCpu);
	glutDisplayFunc(drawFrameBuffer);
	//glutDisplayFunc(drawMemory);
	glutKeyboardFunc(keyInputHandler);
	glutKeyboardUpFunc(keyInputHandlerUp);
	//glutTimerFunc(0, cycleCpu, 0);

	// Start with tilemap
	//swapDisplayMode();

	glutMainLoop();

	return 0;
}

/*int main() {

	sfVideoMode vmode = { 800, 600, 8 };

	sfRenderWindow* window = sfRenderWindow_create(vmode, "Title", sfDefaultStyle, NULL);

	sfRectangleShape* rect = sfRectangleShape_create();
	sfVector2f rectSize = { 100, 50 };
	sfRectangleShape_setSize(rect, rectSize);
	sfVector2f rectPosition = { 200, 200 };
	sfRectangleShape_setPosition(rect, rectPosition);

	sfCircleShape* circle = sfCircleShape_create();
	sfCircleShape_setPosition(circle, rectPosition);
	sfCircleShape_setRadius(circle, 20);
	sfCircleShape_setFillColor(circle, sfRed);
		

	while (sfRenderWindow_isOpen(window)) 
	{
		sfEvent e;
		while (sfRenderWindow_pollEvent(window, &e)) 
		{
			if (e.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}
		}

		sfRenderWindow_clear(window, sfTransparent);

		sfRenderWindow_drawRectangleShape(window, rect, NULL);
		sfRenderWindow_drawCircleShape(window, circle , NULL);

		sfRenderWindow_display(window);
	}

	sfRectangleShape_destroy(rect);

	return EXIT_SUCCESS;
}*/