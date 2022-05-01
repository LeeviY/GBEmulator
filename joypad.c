#include "joypad.h"
#include "cpu.h"

enum JoypadKey
{
	RIGHT	= 0b00000001,
	LEFT	= 0b00000010,
	UP		= 0b00000100,
	DOWN	= 0b00001000,
	A		= 0b00010000,
	B		= 0b00100000,
	SELECT	= 0b01000000,
	START	= 0b10000000,
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