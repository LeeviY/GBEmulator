typedef unsigned char Byte;

struct Timer
{
	Byte tima;
	Byte tma;
	Byte tac;
	union
	{
		struct
		{
			Byte lowerDiv;
			Byte div;
		};
		Word clock;
	};
} extern timer;

extern unsigned int timerModulo[4];

void resetTimer();
void stepTimer();