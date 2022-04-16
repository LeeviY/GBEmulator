#include "cpu.h"
#include "tables.h"
#include "debug.h"

struct OpcodeTable opcodeTable =
{
	.cc =
	{
		FZ, FZ, FC, FC
	},

	.unPrefixed =
	{
		NOP,		LD_rp_nn,	LD_rpi_A,	INC_rp,		INC,		DEC,		LD_n,		7,
		LD_nni_SP,	ADD_HL_rp,	LD_A_rpi,	DEC_rp,		INC,		DEC,		LD_n,		RRCA,
		STOP,		LD_rp_nn,	LD_rpi_A,	INC_rp,		INC,		DEC,		LD_n,		RLA,
		JR_n,		ADD_HL_rp,	LD_A_rpi,	DEC_rp,		INC,		DEC,		LD_n,		31,
		JR_nc_n,	LD_rp_nn,	LDI_HL_A,	INC_rp,		INC,		DEC,		LD_n,		39,
		JR_cc_n,	ADD_HL_rp,	LDI_A_HL,	DEC_rp,		INC,		DEC,		LD_n,		CPL,
		JR_nc_n,	LD_rp_nn,	LDD_HL_A,	INC_rp,		INC_HL,		DEC_HL,		LD_HL_n,	55,
		JR_cc_n,	ADD_HL_rp,	LDD_A_HL,	DEC_rp,		INC,		DEC,		LD_n,		63,

		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD,			LD,			LD,			LD,			LD,			LD,			LD_r_HL,	LD,
		LD_HL_r,	LD_HL_r,	LD_HL_r,	LD_HL_r,	LD_HL_r,	LD_HL_r,	118,		LD_HL_r,
		LD,			LD,			LD,			LD,			LD,			LD,			LD,			LD,

		ADD,		ADD,		ADD,		ADD,		ADD,		ADD,		ADD_HL,		ADD,
		ADC,		ADC,		ADC,		ADC,		ADC,		ADC,		142,		ADC,
		SUB,		SUB,		SUB,		SUB,		SUB,		SUB,		SUB,		SUB,
		SBC_A,		SBC_A,		SBC_A,		SBC_A,		SBC_A,		SBC_A,		158,		SBC_A,
		AND,		AND,		AND,		AND,		AND,		AND,		166,		AND,
		XOR,		XOR,		XOR,		XOR,		XOR,		XOR,		XOR_HL,		XOR,
		OR,			OR,			OR,			OR,			OR,			OR,			OR_HL,		OR,
		CP,			CP,			CP,			CP,			CP,			CP,			CP_HL,		CP,

		RET_nc,		POP,		JP_nc_nn,	JP_nn,		CALL_nc,	PUSH,		ADD_n,		RST,
		RET_cc,		RET,		JP_cc_nn,	PRFX,		CALL_cc,	CALL,		ADC_n,		RST,
		RET_nc,		POP,		JP_nc_nn,	UNDEF,		CALL_nc,	PUSH,		SUB_n,		RST,
		RET_cc,		RETI,		JP_cc_nn,	UNDEF,		CALL_cc,	UNDEF,		222,		RST,
		LD_ff_n_A,	POP,		LD_ff_C_A,	UNDEF,		UNDEF,		PUSH,		AND_n,		RST,
		232,		JP_HL,		LD_nni_A,	UNDEF,		UNDEF,		UNDEF,		238,		RST,
		LD_A_ff_n,	POP,		242,		DI,			UNDEF,		PUSH,		246,		RST,
		248,		LD_SP_HL,	LD_A_nni,	EI,			UNDEF,		UNDEF,		CP_n,		RST,
	},

	.cbPrefixed =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		RL, RL, RL, RL, RL, RL, 0, RL, 0, 0, 0, 0, 0, 0, 0, 0,
		SLA, SLA, SLA, SLA, SLA, SLA, 0, SLA, 0, 0, 0, 0, 0, 0, 0, 0,
		SWAP, SWAP, SWAP, SWAP, SWAP, SWAP, 0, SWAP, 0, 0, 0, 0, 0, 0, 0, 0,

		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT_HL, BIT,

		RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 0, RES,
		RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 0, RES,
		RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 0, RES,
		RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 0, RES,

		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET,
		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET,
		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET,
		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET
	}
};

struct OpCode opcode;

struct Cpu cpu;

struct Interrupt interrupt;

void parseOpCode(struct OpCode* opCode, int hex)
{
	opCode->x = hex >> 6;		//11000000
	opCode->y = hex >> 3 & 7;	//00111000
	opCode->z = hex & 7;		//00000111
	opCode->p = opCode->y >> 1; //00110111
	opCode->q = opCode->y & 1;	//00001000
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

	resetGpu();
}

void memoryDump()
{
	Byte* memoryMirror = (Byte*)malloc(0x10000 * sizeof(Byte));

	for (int i = 0; i < 0xFFFF + 1; ++i)
	{
		memoryMirror[i] = rb((Word)i);
	}

	FILE* write_ptr = fopen("test1.bin", "wb");
	fwrite(memoryMirror, sizeof(memoryMirror), 1, write_ptr);
}

// Debugger breakpoint
//Word bp = 0x029A;
//Word bp = 0x27A3;
Word bp = 0x1D5;
//Word bp = 0x02FA;


Word ei0 = 0x1172;
Word ei1 = 0xD8;
Word ei2 = 0x2BA;

void stepCpu()
{
	// Skip step is cpu is currently stopped
	if (cpu.stopped || debugStop)
		return;

	//if (reg.PC == ei0 || reg.PC == ei1 || reg.PC == ei2)
	{
		//debugOn(bp);
	}

	if (reg.PC == bp)
	{
		debugOn(bp);
	}

	// Get op
	Byte op = rb(reg.PC);
	reg.PC++;

	// Get opcode
	parseOpCode(&opcode, op);
	
	if (debugEnable)
	{
		printf("0x%04X OPC=0x%02X \"", reg.PC - 1, op);
		int count = 0;
		switch (numbers[op])
		{
		case 0:
			count = printf(opNames[op]);
			break;
		case 1:
			count = printf(opNames[op], nextByte());
			reg.PC--;
			break;
		case 2:
			count = printf(opNames[op], nextWord());
			reg.PC -= 2;
			break;
		}
		printf("\"%*c", 24 - count, ' ');
		//printf("x=%d z=%d y=%d q=%d p=%d ", opcode.x, opcode.z, opcode.y, opcode.q, opcode.p);
		printf("A=%02X F=%02X B=%02X C=%02X D=%02X E=%02X H=%02X L=%02X (HL)=%02X PC=%04X SP=%04X (SP)=%02X", reg.A, reg.F, reg.B, reg.C, reg.D, reg.E, reg.H, reg.L, rb(reg.HL), reg.PC, reg.SP, rb(reg.SP));
		printf(" IM=%02x IE=%02x IF=%02x ", interrupt.master, interrupt.enable, interrupt.flags);
		printf("gpu clock=%3d mode=%d liney=%3d\n", gpu.clock, gpu.mode, gpu.line);
	}

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

	stepGPU();
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

		if (toggle & VBLANK)
		{
			interrupt.flags &= ~VBLANK;
			reDisplay();

			INT40();
		}
		else if (toggle & LCD_STAT)
		{
			NOP();
		}
		else if (toggle & TIMER)
		{
			NOP();
		}
		else if (toggle & SERIAL)
		{
			NOP();
		}
		else if (toggle & JOYPAD)
		{
			NOP();
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
	reg.PC += (signed char)nextByte();
	/*signed char a = (signed char)nextWord();
	//printf("JUMP RIGHT HERE %x\n", a);
	reg.PC += a - 1;*/
}

void JR_nc_n()
{
	if (!(reg.F & opcodeTable.cc[opcode.y - 4]))
	{
		reg.PC += (signed char)nextByte();
		//reg.PC += (signed char)nextByte() - 1;

		cpu.clock += 1;
	}
	else
	{
		reg.PC++;
	}
}

void JR_cc_n()
{
	if (reg.F & opcodeTable.cc[opcode.y - 4])
	{
		reg.PC += (signed char)nextByte();

		cpu.clock += 1;
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
	wb(reg.A, rb(reg.HL++));
}

void LDD_A_HL()
{
	wb(reg.A, rb(reg.HL--));
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

void CPL()
{
	reg.A ^= 0xFF;
	SETFN(1);
	SETFH(1);
}

void SCF()
{

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

void SUB()
{
	Byte val = *reg.r[opcode.z];

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

void AND()
{
	reg.A &= *reg.r[opcode.z];

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
void RET_nc()
{
	if (!(reg.F & opcodeTable.cc[opcode.y]))
	{
		reg.PC = rwFromStack();
		reg.PC += 3;

		cpu.clock += 4;
	}
}

void RET_cc()
{
	if (reg.F & opcodeTable.cc[opcode.y])
	{
		reg.PC = rwFromStack();
		reg.PC += 3;

		cpu.clock += 4;
	}
}

void LD_ff_n_A()
{
	wb(0xFF00 + nextByte(), reg.A);
}

void LD_A_ff_n()
{
	reg.A = rb(0xFF00 + nextByte());
}

void POP()
{
	*reg.rp2[opcode.p] = rwFromStack();
}

void RET()
{
	reg.PC = rwFromStack();
	reg.PC += 3;
}

void RETI()
{
	EI();
	RET();
}

void JP_HL()
{
	reg.PC = rb(reg.HL);
}

void LD_SP_HL()
{
	reg.SP = reg.HL;
}

void JP_nc_nn()
{
	if (!(reg.F & opcodeTable.cc[opcode.y]))
	{
		reg.PC = nextWord();
	}
	else
	{
		reg.PC += 2;
	}
}

void JP_cc_nn()
{
	if (reg.F & opcodeTable.cc[opcode.y])
	{
		reg.PC = nextWord();
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

void CALL_nc()
{
	if (!(reg.F & opcodeTable.cc[opcode.y]))
	{
		wwToStack(reg.PC - 1);
		reg.PC = nextWord();
		cpu.clock += 3;
	}
}

void CALL_cc()
{
	if (reg.F & opcodeTable.cc[opcode.y])
	{
		wwToStack(reg.PC - 1);
		reg.PC = nextWord();
		cpu.clock += 3;
	}
}

void PUSH()
{
	wwToStack(*reg.rp2[opcode.p]);
}

void CALL()
{
	wwToStack(reg.PC - 1);
	reg.PC = nextWord();
}

void ADD_n()
{
	unsigned int result = reg.A + rb(*reg.r[opcode.z]);
	reg.A = (Byte)(result & 0xFF);

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(((reg.A & 0x0F) + (*reg.r[opcode.z] & 0x0F)) > 0x0F);
	SETFC(result & 0xFF00);
}

void ADC_n()
{
	Byte val = rb(*reg.r[opcode.z]) + (ISFC ? 1 : 0);
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

void AND_n()
{
	reg.A &= nextByte();

	SETFZ(!reg.A);
	SETFN(0);
	SETFH(1);
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
	wwToStack(reg.PC);
	reg.PC = opcode.y * 8;
	//reg.PC += 3;
}

void INT40()
{
	interrupt.master = 0;
	wwToStack(reg.PC - 1);
	reg.PC = 0x40;

	cpu.clock += 12;
}
// ============================================================================
// CB
// ============================================================================
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
	*reg.r[opcode.z] |= ~(1 << opcode.y);
}

void SET()
{
	*reg.r[opcode.z] |= (1 << opcode.y);
}
