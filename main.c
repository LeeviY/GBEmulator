#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "cpu.h"
#include "gpu.h"

void initWindow()
{
	sfVideoMode mode = { 160, 144, 32 };
	window = sfRenderWindow_create(mode, "GB", sfDefaultStyle, NULL);

	sfColor bgColor = { 0xFF, 0xFF, 0xFF, 0xFF };

	sfRenderWindow_clear(window, bgColor);
	sfWindow_display(window);

	while (sfWindow_isOpen(window))
	{
		sfEvent event;
		while (sfWindow_pollEvent(window, &event))
		{
			if (event.type == sfEvtClosed)
			{
				sfRenderWindow_close(window);
			}
		}
		step();
		//Sleep(1000 / 120);
	}

	return 0;
}

int main(int argc, char** argv)
{
	printf("%s", "Hello World!\n");

	initCPU();
	resetCPU();

	initWindow();

	return 0;
}