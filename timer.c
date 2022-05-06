#include "cpu.h"
#include "timer.h"

struct Timer timer;
unsigned int timerModulo[4] = { 1024, 16, 64, 256 };

void resetTimer()
{
	timer.div = 0;
	timer.tima = 0;
	timer.tma = 0;
	timer.tac = 0;
	timer.divClock = 0;
	timer.tmaClock = 0;
}

void stepTimer()
{
	timer.divClock += cpu.clock;
	unsigned long rs = timer.divClock / 256;
	timer.divClock %= 256;
	timer.div += rs;

	if (timer.tac & 4)
	{
		timer.tmaClock += cpu.clock;
		unsigned int div = timerModulo[timer.tac & 3];

		if (timer.tmaClock >= div)
		{
			timer.tmaClock -= div;
			if (++timer.tima > 256)
			{
				timer.tima = timer.tma;
				interrupt.flags |= INT_TIMER;
			}
			//printf("%d\n", timer.div);
		}
	}
}