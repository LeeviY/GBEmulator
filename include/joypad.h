typedef unsigned char Byte;

struct Joypad
{
	Byte directional;
	Byte action;
	Byte keys;
	Byte select;
} extern joypad;

enum
{
	SELECT_ACTION_BUTTONS = (1 << 5),
	SELECT_DIRECTION_BUTTONS = (1 << 4),
	INPUT_DOWN_START = (1 << 3),
	INPUT_UP_SELECT = (1 << 2),
	INPUT_LEFT_B = (1 << 1),
	INPUT_RIGHT_A = 1
};

void inputKeyDown(Byte key);
void inputKeyUp(Byte key);