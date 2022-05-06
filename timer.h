typedef unsigned char Byte;

struct Timer
{
	Byte div;
	Byte tima;
	Byte tma;
	Byte tac;
	unsigned long divClock;
	unsigned long divClockMod;
	unsigned long tmaClock;
	unsigned long tmaClockMod;
} extern timer;

extern unsigned int timerModulo[4];

void resetTimer();
void stepTimer();