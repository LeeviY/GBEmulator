#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>

#include "cpu.h"
#include "gpu.h"
#include "debug.h"
#include "memory.h"

// Tilemap
//const unsigned int width = 16 * 8, height = 24 * 8;

// Framebuffer
unsigned int width = 160, height = 144;
const int scale = 2;
int displayMode = 0;

// Draws framebuffer
void drawFrameBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(-1, 1);
	glPixelZoom(scale, -scale);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
	glutSwapBuffers();
}

// Draws tilemap
void drawTileMap()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Byte data[24*8][16*8];

	for (int y = 0; y < 8 * 24; ++y)
	{
		for (int x = 0; x < 8 * 16; ++x)
		{
			Byte color = tiles[(y / 8) * 16 + (x / 8)][y][x];
			data[(8 * 24 - 1) - y][x] = color * 85;
			//printf("%4d %4d\n", (8 * 24 - 1) - y, x);
		}
	}
	glPixelZoom(scale, scale);
	glDrawPixels(width, height, GL_RED, GL_UNSIGNED_BYTE, data);
	glutSwapBuffers();
}

// Wrapper to be called outside main
void reDisplay()
{
	glutPostRedisplay();
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
void keyInputHandler(unsigned char key, int x, int y)
{
	switch (key) 
	{
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
	case 'f':
		swapDisplayMode();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	printf("%s", "Hello World!\n");

	// Set cpu states
	initCpu();

	loadBootstrap("dmg_boot.bin");
	loadRom("Tetris.gb");

	// glut window setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width * scale, height * scale);
	glutCreateWindow("GLUT");
	glClearColor(0, 0, 0, 1);

	// Set glut event callback functions
	glutIdleFunc(stepCpu);
	glutDisplayFunc(drawFrameBuffer);
	glutKeyboardFunc(keyInputHandler);

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