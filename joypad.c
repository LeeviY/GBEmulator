#include "joypad.h"
#include "cpu.h"

enum JoypadKey
{
	RIGHT	= 1 << 0,
	LEFT	= 1 << 1,
	UP		= 1 << 2,
	DOWN	= 1 << 3,
	A		= 1 << 4,
	B		= 1 << 5,
	SELECT	= 1 << 6,
	START	= 1 << 7,
};

struct Joypad joypad =
{
	.keys = 0xFF,
	.select = 0x00
};

int getKeyCode(char key)
{
	int keyCode;

	switch (key)
	{
	case 'y':
		keyCode = UP;
		break;
	case 'h':
		keyCode = DOWN;
		break;
	case 'g':
		keyCode = LEFT;
		break;
	case 'j':
		keyCode = RIGHT;
		break;
	case 'p':
		keyCode = A;
		break;
	case 'l':
		keyCode = B;
		break;
	case 'm':
		keyCode = SELECT;
		break;
	case ',':
		keyCode = START;
		break;
	default:
		keyCode = 0;
		break;
	}

	return keyCode;
}

void inputKeyDown(Byte key)
{
	joypad.keys &= ~getKeyCode(key);
	interrupt.flags |= INT_JOYPAD;
}

void inputKeyUp(Byte key)
{
	joypad.keys |= getKeyCode(key);
	interrupt.flags &= ~INT_JOYPAD;
}