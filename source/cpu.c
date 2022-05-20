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
		JR_cc_n,	ADD_HL_rp,	LDD_A_HL,	DEC_rp,		INC,		DEC,		LD_n,		CCF,

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
		RLC, RLC, RLC, RLC, RLC, RLC, RLC_HL, RLC, RRC, RRC, RRC, RRC, RRC, RRC, RRC_HL, RRC,
		RL, RL, RL, RL, RL, RL, RL_HL, RL, RR, RR, RR, RR, RR, RR, RR_HL, RR,
		SLA, SLA, SLA, SLA, SLA, SLA, SLA_HL, SLA, SRA, SRA, SRA, SRA, SRA, SRA, SRA_HL, SRA,
		SWAP, SWAP, SWAP, SWAP, SWAP, SWAP, SWAP_HL, SWAP, SRL, SRL, SRL, SRL, SRL, SRL, SRL_HL, SRL,

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

struct OpCode opc;

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
	initRegs();
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

	interrupt.master = 0;

	cpu.clock = 4;
	cpu.prefix = 0;
	cpu.stopped = 0;

	resetMemory();
	resetTimer();
	resetPpu();

	cpuclock = 4;
}

// Debugger breakpoint
Word bp = 0x100;

const static int multiplier = 4;

unsigned int cpuclock;

void stepCpu()
{
	// Skip step is cpu is currently stopped
	if (cpu.stopped || debugStop)
		return;

	if (reg.PC == bp)
	{
		debugOn(bp);
	}
	//printUniquePCs();

	// Get op
	Byte op = rb(reg.PC);

	// Get opcode
	parseOpCode(&opc, op);

	//printUniqueOps(op);

	if (debugEnable || debugPrint)
	{
		static char sep = '/';
		sep = sep == '/' ? '|' : '/';

		printf("%3X ", cpuclock);
		printCpu(sep, ' ');
		printRegs(sep, ' ');
		printInt(sep, ' ');
		printPpu(sep, ' ');
		printTimer(sep, '\n');
	}
	
	cpu.clock = 0;

	// Check if ei or di queued interrupt master toggle
	if (cpu.toggleIntMaster & 2)
	{
		interrupt.master = cpu.toggleIntMaster & 1;
	}

	reg.PC++;
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

	checkInterrupt();
	cpu.clock *= multiplier;

	stepTimer();
	stepPpu();

	cpuclock += cpu.clock / 2;

	// Breakpoint toggle
	if (debugEnable && !cpu.prefix)
	{
		debugStop = 1;
	}
}

void setInterrupt(int type, Byte addr)
{
	if ((interrupt.enable & interrupt.flags) & type)
	{
		interrupt.flags &= ~type;
		interrupt.master = 0;
		pushToStack(reg.PC);
		reg.PC = addr;

		cpu.clock += 5;
	}
}

void checkInterrupt()
{
	if (interrupt.master && interrupt.enable && interrupt.flags)
	{
		for (int i = 0; i < 5; ++i)
		{
			setInterrupt(1 << i, 0x40 + i * 8);
		}
	}
}

// ============================================================================
// CPU instructions
// ============================================================================
void UNDEF()
{
	printf("UNDEFINED %02X", rb(reg.PC-1));
	debugOn(reg.PC);
}

void NOP()
{
}

void STOP()
{
	printf("STOPPED\n");
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
}

void JR_cc_n()
{
	int cc = reg.F & opcodeTable.cc[opc.y - 4];
	if ((opc.y % 2 == 0 ? !cc : cc))
	{
		reg.PC += (char)nextByte();

		cpu.clock += 1;
	}
	else
	{
		reg.PC++;
	}
}

void LD_rp_nn()
{
	*reg.rp[opc.p] = nextWord();
}

void ADD_HL_rp1()
{
	unsigned long result = reg.HL + *reg.rp[opc.p];

	SETFC(result & 0xFFFF0000);

	reg.HL = (unsigned short)(result & 0xFFFF);

	SETFN(0);
	SETFH(((reg.HL & 0x0F) + (*reg.rp[opc.p] & 0x0F)) > 0x0F);

	printf("%4X %2X\n", reg.HL, reg.F);
}

void ADD_HL_rp()
{
	Word result = reg.HL + *reg.rp[opc.p];

	SETFC(reg.HL > 0xFFFF - result);
	SETFN(0);
	SETFH(((reg.HL & 0xFFF) + (*reg.rp[opc.p] & 0xFFF)) > 0xFFF);
	reg.HL = result;

	//printf("%4X %2X\n", reg.HL, reg.F);
}

void LD_rpi_A()
{
	wb(*reg.rp[opc.p], reg.A);
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
	reg.A = rb(*reg.rp[opc.p]);
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
	++*reg.rp[opc.p];
}

void DEC_rp()
{
	--*reg.rp[opc.p];
}

void INC()
{
	SETFN(0);
	SETFH((*reg.r[opc.y] & 0x0F) + 1 > 0x0F);
	
	++*reg.r[opc.y];
	SETFZ(!*reg.r[opc.y]);
}

void INC_HL()
{
	Byte val = rb(reg.HL);
	SETFN(0);
	SETFH((val & 0x0F) + 1 > 0x0F);

	wb(reg.HL, val + 1);
	SETFZ(!*reg.r[opc.y]);
}

void DEC()
{	
	SETFN(1);
	SETFH(!(*reg.r[opc.y] & 0x0F));

	--*reg.r[opc.y];
	SETFZ(!*reg.r[opc.y]);
}

void DEC_HL()
{
	Byte val = rb(reg.HL);
	SETFN(1);
	SETFH(!(val & 0x0F));

	wb(reg.HL, val - 1);
	SETFZ(!*reg.r[opc.y]);
}

void LD_n()
{
	*reg.r[opc.y] = nextByte();
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
	Byte adj = ISFC ? 0x60 : 0;

	if (ISFH)
	{
		adj |= 0x6;
	}

	if (!ISFN)
	{
		if ((reg.A & 0xF) > 0x9)
		{
			adj |= 0x6;
		}
		if (reg.A > 0x99)
		{
			adj |= 0x60;
		}
		reg.A += adj;
	}
	else
	{
		reg.A -= adj;
	}

	SETFC(adj >= 0x60);
	SETFH(0);
	SETFZ(!reg.A);
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

void CCF()
{
	SETFN(0);
	SETFH(0);
	SETFC(!ISFC);
}

// ============================================================================
// x == 1
// ============================================================================
void LD()
{
	*reg.r[opc.y] = *reg.r[opc.z];
}

void LD_r_HL()
{
	*reg.r[opc.y] = rb(reg.HL);
}

void LD_HL_r()
{
	wb(reg.HL, *reg.r[opc.z]);
}

void HALT()
{
	if (!(interrupt.flags & 0x1F)) 
	{
		reg.PC--;
	}
}

// ============================================================================
// x == 2
// ============================================================================
void ADD()
{
	Byte val = *reg.r[opc.z];
	SETFC((Word)reg.A + (Word)val > 0xFF);
	SETFH(((reg.A & 0xF) + (val & 0xF)) > 0xF);
	SETFN(0);

	reg.A = reg.A + val;
	SETFZ(!reg.A);
}

void ADD_HL()
{
	Byte val = rb(reg.HL);
	SETFC((Word)reg.A + (Word)val > 0xFF);
	SETFH(((reg.A & 0xF) + (val & 0xF)) > 0xF);
	SETFN(0);

	reg.A = reg.A + val;
	SETFZ(!reg.A);
}

void ADC()
{
	Byte val = *reg.r[opc.z] + (ISFC ? 1 : 0);
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
	Byte val = *reg.r[opc.z];

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
	Byte val = *reg.r[opc.y];
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
	reg.A &= *reg.r[opc.z];

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
	reg.A |= *reg.r[opc.z];

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
	reg.A ^= *reg.r[opc.z];
	
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
	SETFZ(reg.A == *reg.r[opc.z]);
	SETFN(1);
	SETFH((*reg.r[opc.z] & 0x0F) > (reg.A & 0x0F));
	SETFC(reg.A < *reg.r[opc.z]);
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
	int cc = reg.F & opcodeTable.cc[opc.y];
	if ((opc.y % 2 == 0 ? !cc : cc))
	{
		reg.PC = popFromStack();
		cpu.clock += 3;
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
	*reg.rp2[opc.p] = popFromStack();
	if (opc.p == 3)
	{
		reg.F &= 0xF0;
	}
}

void RET()
{
	reg.PC = popFromStack();
}

void RETI()
{
	reg.PC = popFromStack();
	interrupt.master = 1;
}

void JP_HL()
{
	reg.PC = reg.HL;
}

void LD_SP_HL()
{
	reg.SP = reg.HL;
}

void JP_cc_nn()
{
	int cc = reg.F & opcodeTable.cc[opc.y];
	if ((opc.y % 2 == 0 ? !cc : cc))
	{
		reg.PC = nextWord();
		cpu.clock += 1;
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
	cpu.toggleIntMaster = 0b10;
}

void EI()
{
	cpu.toggleIntMaster = 0b11;
}

void CALL_cc()
{
	int cc = reg.F & opcodeTable.cc[opc.y];
	if ((opc.y % 2 == 0 ? !cc : cc))
	{
		Word val = nextWord();
		pushToStack(reg.PC);
		reg.PC = val;

		cpu.clock += 3;
	}
	else
	{
		reg.PC += 2;
	}
}

void PUSH()
{
	pushToStack(*reg.rp2[opc.p]);
}

void CALL()
{
	Word val = nextWord();
	pushToStack(reg.PC);
	reg.PC = val;
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
	Byte val = nextByte();

	SETFZ(reg.A == val);
	SETFN(1);
	SETFH((val & 0x0F) > (reg.A & 0x0F));
	SETFC(val > reg.A);
}

void RST()
{
	pushToStack(reg.PC);
	reg.PC = opc.y * 8;
}
// ============================================================================
// CB
// ============================================================================

void RLC()
{
	int carry = (*reg.r[opc.z] & 0x80);
	SETFC(carry);

	carry >>= 7;
	*reg.r[opc.z] <<= 1;
	*reg.r[opc.z] += carry;

	SETFZ(!*reg.r[opc.z]);
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

void RRC()
{
	int carry = (*reg.r[opc.z] & 0x01);

	*reg.r[opc.z] >>= 1;

	if (carry)
	{
		SETFC(1);
		*reg.r[opc.z] |= 0x80;
	}
	SETFZ(!*reg.r[opc.z]);
	SETFN(0);
	SETFH(0);
}

void RRC_HL()
{
	int carry = (*reg.r[opc.z] & 0x01);
	Byte val = rb(reg.HL);

	val >>= 1;

	if (carry)
	{
		SETFC(1);
		val |= 0x80;
	}
	SETFZ(!val);
	SETFN(0);
	SETFH(0);

	wb(reg.HL, val);
}

void RL()
{
	int carry = ISFC ? 1 : 0;

	SETFN(0);
	SETFH(0);
	SETFC(*reg.r[opc.z] & 0x80);

	*reg.r[opc.z] <<= 1;
	*reg.r[opc.z] += carry;
	SETFZ(!*reg.r[opc.z]);
}

void RL_HL()
{
	int carry = ISFC ? 1 : 0;

	Byte val = rb(reg.HL);

	SETFN(0);
	SETFH(0);
	SETFC(val & 0x80);

	val <<= 1;
	val += carry;
	SETFZ(!val);

	wb(reg.HL, val);
}

void RR()
{
	*reg.r[opc.z] >>= 1;

	if (ISFC)
	{
		*reg.r[opc.z] |= 0x80;
	}

	SETFZ(!*reg.r[opc.z]);
	SETFN(0);
	SETFH(0);
	SETFC(*reg.r[opc.z] & 1);
}

void RR_HL()
{
	Byte val = rb(reg.HL);
	val >>= 1;

	if (ISFC)
	{
		val |= 0x80;
	}

	SETFZ(!val);
	SETFN(0);
	SETFH(0);
	SETFC(val & 1);

	wb(reg.HL, val);
}

void SLA()
{
	SETFN(0);
	SETFH(0);
	SETFC(*reg.r[opc.z] & 0x80);

	*reg.r[opc.z] <<= 1;
	SETFZ(!*reg.r[opc.z]);
}

void SLA_HL()
{
	Byte val = rb(reg.HL);

	SETFN(0);
	SETFH(0);
	SETFC(val & 0x80);

	val <<= 1;
	SETFZ(!val);

	wb(reg.HL, val);
}

void SRA()
{
	SETFC(*reg.r[opc.z] & 1);
	*reg.r[opc.z] = (*reg.r[opc.z] & 0x80) | (*reg.r[opc.z] >> 1);
	SETFZ(!*reg.r[opc.z]);
	SETFN(0);
	SETFH(0);
}

void SRA_HL()
{
	Byte val = rb(reg.HL);

	SETFC(val & 1);
	val = (val & 0x80) | (val >> 1);
	SETFZ(!val);
	SETFN(0);
	SETFH(0);

	wb(reg.HL, val);
}

void SWAP()
{
	Byte val = *reg.r[opc.z];
	*reg.r[opc.z] = (val >> 4) | (val << 4);
	SETFZ(!val);
	SETFN(0);
	SETFH(0);
	SETFC(0);
}

void SWAP_HL()
{
	Byte val = rb(reg.HL);
	val = (val >> 4) | (val << 4);
	SETFZ(!val);
	SETFN(0);
	SETFH(0);
	SETFC(0);

	wb(reg.HL, val);
}

void SRL()
{
	SETFC(*reg.r[opc.z] & 0x01);

	*reg.r[opc.z] >>= 1;

	SETFZ(*reg.r[opc.z]);
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
	SETFZ(!(*reg.r[opc.z] & (1 << opc.y)));
	SETFN(0);
	SETFH(1);
}

void BIT_HL()
{
	SETFZ(!(rb(reg.HL) & (1 << opc.y)));
	SETFN(0);
	SETFH(1);
}

void RES()
{
	*reg.r[opc.z] &= ~(1 << opc.y);
}

void RES_HL()
{
	wb(reg.HL, rb(reg.HL) & ~(1 << opc.y));
}

void SET()
{
	*reg.r[opc.z] |= (1 << opc.y);
}

void SET_HL()
{
	wb(reg.HL, rb(reg.HL) | (1 << opc.y));
}