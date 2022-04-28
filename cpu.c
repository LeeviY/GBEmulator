#include "cpu.h"
#include "tables.h"
#include "debug.h"
#include "time.h"
#include "joypad.h"

struct OpcodeTable opcodeTable =
{
	.cc =
	{
		FZ, FZ, FC, FC
	},

	.unPrefixed =
	{
		NOP,		LD_rp_nn,	LD_rpi_A,	INC_rp,		INC,		DEC,		LD_n,		RLCA,
		LD_nni_SP,	ADD_HL_rp,	LD_A_rpi,	DEC_rp,		INC,		DEC,		LD_n,		RRCA,
		STOP,		LD_rp_nn,	LD_rpi_A,	INC_rp,		INC,		DEC,		LD_n,		RLA,
		JR_n,		ADD_HL_rp,	LD_A_rpi,	DEC_rp,		INC,		DEC,		LD_n,		RRA,
		JR_cc_n,	LD_rp_nn,	LDI_HL_A,	INC_rp,		INC,		DEC,		LD_n,		DAA,
		JR_cc_n,	ADD_HL_rp,	LDI_A_HL,	DEC_rp,		INC,		DEC,		LD_n,		CPL,
		JR_cc_n,	LD_rp_nn,	LDD_HL_A,	INC_rp,		INC_HL,		DEC_HL,		LD_HL_n,	SCF,
		JR_cc_n,	ADD_HL_rp,	LDD_A_HL,	DEC_rp,		INC,		DEC,		LD_n,		63,

		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD_HL_r,	LD_HL_r,	LD_HL_r,	LD_HL_r,	LD_HL_r,	LD_HL_r,	HALT,		LD_HL_r,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,

		ADD,		ADD,		ADD,		ADD,		ADD,		ADD,		ADD_HL,		ADD,
		ADC,		ADC,		ADC,		ADC,		ADC,		ADC,		ADC_HL,		ADC,
		SUB,		SUB,		SUB,		SUB,		SUB,		SUB,		SUB_HL,		SUB,
		SBC_A,		SBC_A,		SBC_A,		SBC_A,		SBC_A,		SBC_A,		SBC_A_HL,	SBC_A,
		AND,		AND,		AND,		AND,		AND,		AND,		AND_HL,		AND,
		XOR,		XOR,		XOR,		XOR,		XOR,		XOR,		XOR_HL,		XOR,
		OR,			OR,			OR,			OR,			OR,			OR,			OR_HL,		OR,
		CP,			CP,			CP,			CP,			CP,			CP,			CP_HL,		CP,

		RET_cc,		POP,		JP_cc_nn,	JP_nn,		CALL_cc,	PUSH,		ADD_n,		RST,
		RET_cc,		RET,		JP_cc_nn,	PRFX,		CALL_cc,	CALL,		ADC_n,		RST,
		RET_cc,		POP,		JP_cc_nn,	UNDEF,		CALL_cc,	PUSH,		SUB_n,		RST,
		RET_cc,		RETI,		JP_cc_nn,	UNDEF,		CALL_cc,	UNDEF,		SBC_A_n,	RST,
		LD_ff_n_A,	POP,		LD_ff_C_A,	UNDEF,		UNDEF,		PUSH,		AND_n,		RST,
		ADD_SP_n,	JP_HL,		LD_nni_A,	UNDEF,		UNDEF,		UNDEF,		XOR_n,		RST,
		LD_A_ff_n,	POP,		LD_A_ff_C,	DI,			UNDEF,		PUSH,		OR_n,		RST,
		LD_HL_SP_n,	LD_SP_HL,	LD_A_nni,	EI,			UNDEF,		UNDEF,		CP_n,		RST,
	},

	.cbPrefixed =
	{
		RLC, RLC, RLC, RLC, RLC, RLC, RLC_HL, RLC, 0, 0, 0, 0, 0, 0, 0, 0,
		RL, RL, RL, RL, RL, RL, 0, RL, RR, RR, RR, RR, RR, RR, 0, RR,
		SLA, SLA, SLA, SLA, SLA, SLA, 0, SLA, 0, 0, 0, 0, 0, 0, 0, 0,
		SWAP, SWAP, SWAP, SWAP, SWAP, SWAP, 0, SWAP, SRL, SRL, SRL, SRL, SRL, SRL, SRL_HL, SRL,

		BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,

		RES, RES, RES, RES, RES, RES, RES_HL, RES, RES, RES, RES, RES, RES, RES, RES_HL, RES,
		RES, RES, RES, RES, RES, RES, RES_HL, RES, RES, RES, RES, RES, RES, RES, RES_HL, RES,
		RES, RES, RES, RES, RES, RES, RES_HL, RES, RES, RES, RES, RES, RES, RES, RES_HL, RES,
		RES, RES, RES, RES, RES, RES, RES_HL, RES, RES, RES, RES, RES, RES, RES, RES_HL, RES,

		SET, SET, SET, SET, SET, SET, SET_HL, SET, SET, SET, SET, SET, SET, SET, SET_HL, SET,
		SET, SET, SET, SET, SET, SET, SET_HL, SET, SET, SET, SET, SET, SET, SET, SET_HL, SET,
		SET, SET, SET, SET, SET, SET, SET_HL, SET, SET, SET, SET, SET, SET, SET, SET_HL, SET,
		SET, SET, SET, SET, SET, SET, SET_HL, SET, SET, SET, SET, SET, SET, SET, SET_HL, SET,
	}
};

struct OpCode opcode;

struct Cpu cpu;

struct Interrupt interrupt;

void parseOpCode(struct OpCode* opCode, int hex)
{
	opCode->x = (hex & 0x11000000) >> 6;
	opCode->y = (hex & 0b00111000) >> 3;
	opCode->z =  hex & 0b00000111;
	opCode->p = (hex & 0b00110000) >> 4;
	opCode->q = (hex & 0b00001000) >> 3;
}

void initCpu()
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

	resetCpu();
}

void resetCpu()
{
	for (int i = 0; i < 8; ++i)
	{
		*reg.r[i] = 0;
	}
	reg.SP = 0;
	reg.PC = 0;

	interrupt.master = 1;
	interrupt.enable = 0;
	interrupt.flags = 0;

	cpu.clock = 0;
	cpu.prefix = 0;
	cpu.stopped = 0;

	resetPpu();

	/*for (int i = 0; i < 0x2000; ++i)
	{
		seenPCs[i] = 0x8000;
	}*/
}

void memoryDump()
{
	Byte* memoryMirror = (Byte*)malloc(0x10000 * sizeof(Byte));

	if (!memoryMirror)
	{
		return;
	}

	for (int i = 0; i < 0xFFFF + 1; ++i)
	{
		memoryMirror[i] = rb((Word)i);
	}

	FILE* write_ptr = fopen("test1.bin", "wb");
	fwrite(memoryMirror, sizeof(memoryMirror), 1, write_ptr);
}

// Debugger breakpoint
Word bp = -1;

const static int multiplier = 1;

void stepCpu()
{
	// Skip step is cpu is currently stopped
	if (cpu.stopped || debugStop)
		return;

	if (reg.PC == bp)
	{
		debugOn(bp);
	}

	/*memoryMap[reg.PC][0] = 255;
	memoryMap[reg.PC][1] = 0;
	memoryMap[reg.PC][2] = 0;*/

	// Get op
	Byte op = rb(reg.PC++);

	// Get opcode
	parseOpCode(&opcode, op);

	if (debugEnable || debugPrint)
	{
		printCpu(op,' ');
		printRegs(' ');
		printInt(' ');
		printPpu('\n');
	}
	cpu.clock = 0;

	// Check if last operation was 0xCB/prefix
	if (cpu.prefix)
	{
		// CB operations
		(*opcodeTable.cbPrefixed[op])();
		cpu.clock += cbInsTicks[op];
		cpu.prefix = 0;
	}
	else
	{
		// Normal operations
		(*opcodeTable.unPrefixed[op])();
		cpu.clock += insTicks[op];
	}

	stepPpu();
	checkInterrupt();

	// Breakpoint toggle
	if (debugEnable)
	{
		debugStop = 1;
	}
}

void checkInterrupt()
{
	if (interrupt.master && interrupt.enable && interrupt.flags)
	{
		Byte toggle = interrupt.enable & interrupt.flags;

		if (toggle & INT_VBLANK)
		{
			interrupt.flags &= ~INT_VBLANK;
			interrupt.master = 0;
			pushToStack(reg.PC);
			reg.PC = 0x40;

			cpu.clock += 12;
		}
		if (toggle & INT_LCD_STAT)
		{
			interrupt.flags &= ~INT_LCD_STAT;
			interrupt.master = 0;
			pushToStack(reg.PC);
			reg.PC = 0x48;

			cpu.clock += 12;
		}
		if (toggle & INT_TIMER)
		{
			interrupt.flags &= ~INT_TIMER;
			interrupt.master = 0;
			pushToStack(reg.PC);
			reg.PC = 0x50;

			cpu.clock += 12;
		}
		if (toggle & INT_SERIAL)
		{
			interrupt.flags &= ~INT_SERIAL;
			interrupt.master = 0;
			pushToStack(reg.PC);
			reg.PC = 0x58;

			cpu.clock += 12;
		}
		if (toggle & INT_JOYPAD)
		{
			interrupt.flags &= ~INT_JOYPAD;
			interrupt.master = 0;
			pushToStack(reg.PC);
			reg.PC = 0x60;

			cpu.clock += 12;
		}
	}
}

// ============================================================================
// CPU instructions
// ============================================================================
void UNDEF()
{
	printf("UNDEFINED %02X", rb(--reg.PC));
	debugOn(reg.PC);
}

void NOP()
{
}

void STOP()
{
	reg.PC++;
	cpu.stopped = 1;
}

void LD_nni_SP()
{
	ww(nextWord(), reg.SP);
}

void JR_n()
{
	reg.PC += (char)nextByte();
	/*signed char a = (signed char)nextWord();
	//printf("JUMP RIGHT HERE %x\n", a);
	reg.PC += a - 1;*/
}

void JR_cc_n()
{
	int cc = reg.F & opcodeTable.cc[opcode.y - 4];
	if ((opcode.y % 2 == 0 ? !cc : cc))
	{
		reg.PC += (char)nextByte();

		cpu.clock += 1 * multiplier;
	}
	else
	{
		reg.PC++;
	}
}

void LD_rp_nn()
{
	*reg.rp[opcode.p] = nextWord();
}

void ADD_HL_rp()
{
	unsigned int result = reg.HL + *reg.rp[opcode.p];

	SETFC(result & 0xFFFF0000);

	reg.HL = (unsigned short)(result & 0xFFFF);

	SETFN(0);
	SETFH(((reg.HL & 0x0F) + (*reg.rp[opcode.p] & 0x0F)) > 0x0F);
}

void LD_rpi_A()
{
	wb(*reg.rp[opcode.p], reg.A);
}

void LDI_HL_A()
{
	wb(reg.HL++, reg.A);
}

void LDD_HL_A()
{
	wb(reg.HL--, reg.A);
}

void LD_A_rpi()
{
	reg.A = rb(*reg.rp[opcode.p]);
}

void LDI_A_HL()
{
	reg.A = rb(reg.HL++);
}

void LDD_A_HL()
{
	reg.A = rb(reg.HL--);
}

void INC_rp()
{
	++*reg.rp[opcode.p];
}

void DEC_rp()
{
	--*reg.rp[opcode.p];
}

void INC()
{
	SETFN(0);
	SETFH((*reg.r[opcode.y] & 0x0F) == 0x0F);
	
	++*reg.r[opcode.y];
	SETFZ(!*reg.r[opcode.y]);
}

void INC_HL()
{
	Byte val = rb(reg.HL);
	SETFN(0);
	SETFH((val & 0x0F) == 0x0F);

	wb(reg.HL, val + 1);
	SETFZ(!*reg.r[opcode.y]);
}

void DEC()
{	
	SETFN(1);
	SETFH(!(*reg.r[opcode.y] & 0x0F));

	--*reg.r[opcode.y];
	SETFZ(!*reg.r[opcode.y]);
}

void DEC_HL()
{
	Byte val = rb(reg.HL);
	SETFN(1);
	SETFH(!(val & 0x0F));

	wb(reg.HL, val - 1);
	SETFZ(!*reg.r[opcode.y]);
}

void LD_n()
{
	*reg.r[opcode.y] = nextByte();
}

void LD_HL_n()
{
	wb(reg.HL, nextByte());
}

void RLCA()
{
	Byte carry = (reg.A & 0x80) >> 7;

	SETFZ(0);
	SETFN(0);
	SETFH(0);
	SETFC(carry);

	reg.A <<= 1;
	if (carry)
	{
		reg.A += carry;
	}
}

void RRCA()
{
	Byte carry = reg.A & 0x01;
	
	SETFZ(0);
	SETFN(0);
	SETFH(0);
	SETFC(carry);

	reg.A >>= 1;
	if (carry)
	{
		reg.A |= 0x80;
	}
}

void RLA()
{
	int carry = ISFC ? 1 : 0;
	
	SETFZ(0);
	SETFN(0);
	SETFH(0);
	SETFC(reg.A & 0x80);
	reg.A <<= 1;
	reg.A += carry;
}

void RRA()
{
	int carry = (ISFC ? 1 : 0) << 7;

	SETFZ(1);
	SETFN(1);
	SETFH(1);
	SETFC(reg.A & 1);
	reg.A >>= 1;
	reg.A += carry;
}

void DAA()
{
	Word val = reg.A;

	if (ISFN)
	{
		if (ISFH)
		{
			val = (val - 0x06) & 0xFF;
		}
		if (ISFC)
		{
			val -= 0x60;
		}
	}
	else 
	{
		if (ISFH || (val & 0xF) > 9)
		{
			val += 0x06;
		}
		if (ISFC || val > 0x9F)
		{
			val += 0x60;
		}
	}

	reg.A = val;
	SETFH(0);

	SETFZ(!reg.A);
	SETFC(val >= 0x100);
}

void CPL()
{
	reg.A ^= 0xFF;
	SETFN(1);
	SETFH(1);
}

void SCF()
{
	SETFC(1);
	SETFN(0);
	SETFH(0);
}

// ============================================================================
// x == 1
// ============================================================================
void LD()
{
	*reg.r[opcode.y] = *reg.r[opcode.z];
}

void LD_r_HL()
{
	*reg.r[opcode.y] = rb(reg.HL);
}

void LD_HL_r()
{
	wb(reg.HL, *reg.r[opcode.z]);
}

void HALT()
{
	if (!interrupt.flags) 
	{
		reg.PC--;
	}
}

// ============================================================================
// x == 2
// ============================================================================
void ADD()
{
	unsigned int result = reg.A + *reg.r[opcode.z];
	reg.A = (Byte)(result & 0xFF);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(((reg.A & 0x0F) + (*reg.r[opcode.z] & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);
}

void ADD_HL()
{
	unsigned int result = reg.A + rb(reg.HL);
	reg.A = (Byte)(result & 0xFF);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(((reg.A & 0x0F) + (rb(reg.HL) & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);
}

void ADC()
{
	Byte val = *reg.r[opcode.z] + (ISFC ? 1 : 0);
	unsigned int result = reg.A + val;

	SETFZ(val == reg.A);
	SETFN(1);
	SETFH(((reg.A & 0x0F) + (val & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);

	reg.A = (Byte)(result & 0xFF);
}

void ADC_HL()
{
	Byte val = rb(reg.HL) + (ISFC ? 1 : 0);
	unsigned int result = reg.A + val;

	SETFZ(val == reg.A);
	SETFN(1);
	SETFH(((reg.A & 0x0F) + (val & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);

	reg.A = (Byte)(result & 0xFF);
}

void SUB()
{
	Byte val = *reg.r[opcode.z];

	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);

	reg.A -= val;
	SETFZ(!reg.A);
}

void SUB_HL()
{
	Byte val = rb(reg.HL);

	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);

	reg.A -= val;
	SETFZ(!reg.A);
}

void SBC_A()
{
	Byte val = *reg.r[opcode.y];
	val += ISFC ? 1 : 0;

	SETFZ(val == reg.A);
	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);

	reg.A -= val;
}

void SBC_A_HL()
{
	Byte val = rb(reg.HL);
	val += ISFC ? 1 : 0;

	SETFZ(val == reg.A);
	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);

	reg.A -= val;
}

void AND()
{
	reg.A &= *reg.r[opcode.z];

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(1);
	SETFC(0);
}

void AND_HL()
{
	reg.A &= rb(reg.HL);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(1);
	SETFC(0);
}

void OR()
{
	reg.A |= *reg.r[opcode.z];

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void OR_HL()
{
	reg.A |= rb(reg.HL);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void XOR()
{
	reg.A ^= *reg.r[opcode.z];
	
	SETFZ(!reg.A);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void XOR_HL()
{
	reg.A ^= rb(reg.HL);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void CP()
{
	SETFZ(reg.A == *reg.r[opcode.z]);
	SETFN(1);
	SETFH((*reg.r[opcode.z] & 0x0F) > (reg.A & 0x0F));
	SETFC(reg.A < *reg.r[opcode.z]);
}

void CP_HL()
{
	SETFZ(reg.A == rb(reg.HL));
	SETFN(1);
	SETFH((rb(reg.HL) & 0x0F) > (reg.A & 0x0F));
	SETFC(reg.A < rb(reg.HL));
}


// ============================================================================
// x == 3
// ============================================================================
void RET_cc()
{
	int cc = reg.F & opcodeTable.cc[opcode.y];
	if ((opcode.y % 2 == 0 ? !cc : cc))
	{
		reg.PC = popFromStack();
		reg.PC += 3;

		cpu.clock += 3 * multiplier;
	}
}

void LD_ff_n_A()
{
	wb(0xFF00 + nextByte(), reg.A);
}

void ADD_SP_n()
{
	int val = nextByte();
	int res = reg.SP + val;

	SETFC(res & 0xF0);
	reg.SP = res & 0xFFFF;

	SETFZ(0);
	SETFN(0);
	SETFH((reg.SP & 0x0F) + (val & 0x0F) > 0x0F);
}

void LD_A_ff_n()
{
	reg.A = rb(0xFF00 + nextByte());
}

void LD_HL_SP_n()
{
	Byte val = nextByte();

	Word res = reg.SP + (char)val;

	SETFZ(0);
	SETFN(0);
	SETFH((reg.SP & 0x0F) + (val & 0x0F) > 0x0F);
	SETFC(res & 0xF0);

	reg.HL = (int16_t)(res & 0xFFFF);
}

void POP()
{
	*reg.rp2[opcode.p] = popFromStack();
}

void RET()
{
	reg.PC = popFromStack();
	reg.PC += 3;
}

void RETI()
{
	reg.PC = popFromStack();
	interrupt.master = 1;
}

void JP_HL()
{
	//reg.PC = rb(reg.HL);
	reg.PC = reg.HL;
}

void LD_SP_HL()
{
	reg.SP = reg.HL;
}

void JP_cc_nn()
{
	int cc = reg.F & opcodeTable.cc[opcode.y];
	if ((opcode.y % 2 == 0 ? !cc : cc))
	{
		reg.PC = nextWord();

		cpu.clock += 1 * multiplier;
	}
	else
	{
		reg.PC += 2;
	}
}

void LD_ff_C_A()
{
	wb(0xFF00 + reg.C, reg.A);
}

void LD_nni_A()
{
	wb(nextWord(), reg.A);
}

void LD_A_ff_C()
{
	reg.A = rb(0xFF00 + reg.C);
}

void LD_A_nni()
{
	reg.A = rb(nextWord());
}

void JP_nn()
{
	reg.PC = nextWord();
}

void PRFX()
{
	cpu.prefix = 1;
}

void DI()
{
	interrupt.master = 0;
}

void EI()
{
	interrupt.master = 1;
}

void CALL_cc()
{
	int cc = reg.F & opcodeTable.cc[opcode.y];
	if ((opcode.y % 2 == 0 ? !cc : cc))
	{
		pushToStack(reg.PC - 1);
		reg.PC = nextWord();

		cpu.clock += 3 * multiplier;
	}
}

void PUSH()
{
	pushToStack(*reg.rp2[opcode.p]);
}

void CALL()
{
	pushToStack(reg.PC - 1);
	reg.PC = nextWord();
}

void ADD_n()
{
	Byte val = nextByte();
	unsigned int result = reg.A + val;
	reg.A = (Byte)(result & 0xFF);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(((reg.A & 0x0F) + (val & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);
}

void ADC_n()
{
	Byte val = nextByte() + (ISFC ? 1 : 0);
	unsigned int result = reg.A + val;

	SETFZ(val == reg.A);
	SETFN(1);
	SETFH(((reg.A & 0x0F) + (val & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);

	reg.A = (Byte)(result & 0xFF);
}

void SUB_n()
{
	Byte val = nextByte();

	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);

	reg.A -= val;
	SETFZ(!reg.A);
}

void SBC_A_n()
{
	Byte val = nextByte();
	val += ISFC ? 1 : 0;

	SETFZ(val == reg.A);
	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);

	reg.A -= val;
}

void AND_n()
{
	reg.A &= nextByte();

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(1);
	SETFC(0);
}

void XOR_n()
{
	reg.A ^= nextByte();

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void OR_n()
{
	reg.A |= nextByte();

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void CP_n()
{
	Byte operand = nextByte();

	SETFZ(reg.A == operand);
	SETFN(1);
	SETFH((operand & 0x0F) > (reg.A & 0x0F));
	SETFC(operand > reg.A);
}

void RST()
{
	pushToStack(reg.PC);
	reg.PC = opcode.y * 8;
	//reg.PC += 3;
}
// ============================================================================
// CB
// ============================================================================

void RLC()
{
	int carry = (*reg.r[opcode.z] & 0x80);
	SETFC(carry);

	carry >>= 7;
	*reg.r[opcode.z] <<= 1;
	*reg.r[opcode.z] += carry;

	SETFZ(!*reg.r[opcode.z]);
	SETFN(0);
	SETFH(0);
}

void RLC_HL()
{
	int val = rb(reg.HL);
	int carry = (val & 0x80);
	SETFC(carry);

	carry >>= 7;

	val <<= 1;
	val += carry;
	wb(reg.HL, val);

	SETFZ(!val);
	SETFN(0);
	SETFH(0);
}

void RL()
{
	int carry = ISFC ? 1 : 0;

	SETFN(0);
	SETFH(0);
	SETFC(*reg.r[opcode.z] & 0x80);

	*reg.r[opcode.z] <<= 1;
	*reg.r[opcode.z] += carry;
	SETFZ(!*reg.r[opcode.z]);
}

void RR()
{
	*reg.r[opcode.z] >>= 1;

	if (ISFC)
	{
		*reg.r[opcode.z] |= 0x80;
	}

	SETFZ(!*reg.r[opcode.z]);
	SETFN(0);
	SETFH(0);
	SETFC(*reg.r[opcode.z] & 1);
}

void SLA()
{
	SETFN(0);
	SETFH(0);
	SETFC(*reg.r[opcode.z] & 0x80);

	*reg.r[opcode.z] <<= 1;
	SETFZ(!*reg.r[opcode.z]);
}

void SWAP()
{
	*reg.r[opcode.z] = ((*reg.r[opcode.z] & 0xF) << 4) | ((*reg.r[opcode.z] & 0xF0) >> 4);
	SETFZ(*reg.r[opcode.z]);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void SRL()
{
	SETFC(*reg.r[opcode.z] & 0x01);

	*reg.r[opcode.z] >>= 1;

	SETFZ(*reg.r[opcode.z]);
	SETFN(0);
	SETFH(0);
}

void SRL_HL()
{
	int val = rb(reg.HL);
	SETFC(val & 0x01);

	val >>= 1;

	wb(reg.HL, val);

	SETFZ(val);
	SETFN(0);
	SETFH(0);
}

void BIT()
{
	SETFZ(!(*reg.r[opcode.z] & (1 << opcode.y)));
	SETFN(0);
	SETFH(1);
}

void BIT_HL()
{
	SETFZ(!(rb(reg.HL) & (1 << opcode.y)));
	SETFN(0);
	SETFH(1);
}

void RES()
{
	*reg.r[opcode.z] &= ~(1 << opcode.y);
}

void RES_HL()
{
	wb(reg.HL, rb(reg.HL) & ~(1 << opcode.y));
}

void SET()
{
	*reg.r[opcode.z] |= (1 << opcode.y);
}

void SET_HL()
{
	wb(reg.HL, rb(reg.HL) | (1 << opcode.y));
}