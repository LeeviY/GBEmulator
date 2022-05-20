#include "cpu.h"
#include "timer.h"

struct Timer timer;
//unsigned int timerModulo[4] = { 1024, 16, 64, 256 };
unsigned int timerModulo[4] = { 0x200, 0x8, 0x20, 0x80 };

void resetTimer()
{
	timer.clock = 0;
}

void stepTimer()
{
	static int lastBit = 0;

	for (int i = cpu.clock; i > 0; --i, ++timer.clock)
	{
		int bit = (timer.tac & 0b100) && (timer.clock & timerModulo[timer.tac & 0b11]);

		if (!bit && lastBit)
		{
			if (!++timer.tima)
			{
				timer.tima = timer.tma;
				interrupt.flags |= INT_TIMER;
			}
		}
		lastBit = bit;
	}

	/*if (timer.tac & 0b100)
	{
		for (int i = cpu.clock;i > 0; --i, ++timer.clock)
		{
			if (timer.clock % timerModulo[timer.tac & 0b11] == 0)
			{
				if (++timer.tima == 0)
				{
					timer.tima = timer.tma;
					interrupt.flags |= INT_TIMER;
				}
			}
		}
	}
	else
	{
		timer.clock += cpu.clock;
	}*/

	/*if (timer.divClock >= 256)
	{
		timer.divClock -= 256;
		timer.div++;
	}*/

	/*if (timer.tac & 0b100)
	{
		if (timer.tmaClock >= timerModulo[timer.tac & 0b11])
		{
			timer.tmaClock -= timerModulo[timer.tac & 0b11];
			if (++timer.tima == 0)
			{
				timer.tima = timer.tma;
				interrupt.flags |= INT_TIMER;
			}
		}
	}*/
}