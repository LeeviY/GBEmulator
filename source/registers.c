#include "registers.h"
#include "memory.h"

struct Registers reg;

void initRegs()
{
	// Init reference registers
	reg.r[0] = &reg.B;
	reg.r[1] = &reg.C;
	reg.r[2] = &reg.D;
	reg.r[3] = &reg.E;
	reg.r[4] = &reg.H;
	reg.r[5] = &reg.L;
	reg.r[6] = &reg.F;// Not used should be (HL), set to F for reset
	reg.r[7] = &reg.A;

	// Set references for register index arrays
	reg.rp[0] = reg.rp2[0] = &reg.BC;	// BC
	reg.rp[1] = reg.rp2[1] = &reg.DE;	// DE
	reg.rp[2] = reg.rp2[2] = &reg.HL;	// HL
	reg.rp[3] = &reg.SP;				// SP
	reg.rp2[3] = &reg.AF;				// AF
}
