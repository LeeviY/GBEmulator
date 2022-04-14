#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>

#include "cpu.h"
#include "gpu.h"

// Tilemap
//const unsigned int width = 16 * 8, height = 24 * 8;

// Framebuffer
const unsigned int width = 160, height = 144;

void drawFrameBuffer()
{
	//printf("DRAW FRAME\n");
	//printTileMap();
	//printFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT);

	/*Byte data[144 * 160];

	for (int i = 0; i < (160 * 144); ++i)
	{
		Byte c = 255 - frameBuffer[i];
		data[i] = c;
	}*/
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
	glutSwapBuffers();
	glutPostRedisplay();
}

void drawTileMap()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Byte data[24*8][16*8];

	/*for (int y = 0; y < 8 * 24; ++y)
	{
		for (int x = 0; x < 8 * 16; ++x)
		{
			Byte color = tiles[(y / 8) * 16 + (x / 8)][y][x];
			data[y][x] = color * 85;
		}
	}*/

	for (int y = 0; y < 8 * 24; ++y)
	{
		for (int x = 0; x < 8 * 16; ++x)
		{
			Byte color = tiles[(y / 8) * 16 + (x / 8)][y][x];
			data[(8 * 24 - 1) - y][x] = color * 85;
		}
	}

	glDrawPixels(width, height, GL_RED, GL_UNSIGNED_BYTE, data);
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	printf("%s", "Hello World!\n");

	initCPU();
	resetCPU();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("GLUT");

	glClearColor(0, 0, 0, 1);

	int count = 0;
	while (1)
	{
		step();
		stepGPU();
		checkInterrupt();
		if (count++ == 3000)
		{
			Sleep(1);
			count = 0;
		}
	}

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