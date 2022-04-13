#include "cpu.h"
#include "tables.h"

struct OpCode opcode = { 0, 0, 0 ,0 ,0 };

struct OpcodeTable opcodeTable =
{
	.cc =
	{
		FZ, FZ, FC, FC
	},

	.unPrefixed =
	{
		NOP, 1, 2, INC_rp, INC, DEC, LD_n, 7,
		LD_nn_SP, 9, 10, 11, INC, DEC, LD_n, RRCA,
		STOP, LD_rp_nn, 18, INC_rp, INC, DEC, LD_n, RLA,
		JR_n, ADD_HL_rp, LDi_A_rp, 27, INC, DEC, LD_n, 31,
		JR_nc_n, LD_rp_nn, LDi_HLp_A, INC_rp, INC, DEC, LD_n, 39,
		JR_cc_n, 41, 42, 43, INC, DEC, LD_n, 47,
		JR_nc_n, LD_rp_nn, LDi_HLm_A, INC_rp, 52, 53, 54, 55,
		JR_cc_n, 57, 58, 59, INC, DEC, LD_n, 63,

		LD, LD, LD, LD, LD, LD, LD, LD,
		LD, LD, LD, LD, LD, LD, LD, LD,
		LD, LD, LD, LD, LD, LD, LD, LD,
		LD, LD, LD, LD, LD, LD, LD, LD,
		LD, LD, LD, LD, LD, LD, LD, LD,
		LD, LD, LD, LD, LD, LD, LD, LD,
		LD, LD, LD, LD, LD, LD, 118, LD,
		LD, LD, LD, LD, LD, LD, LD, LD,

		ADD, ADD, ADD, ADD, ADD, ADD, ADD_HL, ADD,
		136, 137, 138, 139, 140, 141, 142, 143,
		SUB, SUB, SUB, SUB, SUB, SUB, SUB, SUB,
		SBC_A, SBC_A, SBC_A, SBC_A, SBC_A, SBC_A, 158, SBC_A,
		160, 161, 162, 163, 164, 165, 166, 167,
		XOR, XOR, XOR, XOR, XOR, XOR, 174, XOR,
		176, 177, 178, 179, 180, 181, 182, 183,
		CP, CP, CP, CP, CP, CP, CP_HL, CP,

		192, POP, 194, 195, 196, PUSH, 198, 199,
		200, RET, 202, PRFX, 204, CALL, 206, 207,
		208, POP, 210, 211, 212, PUSH, SUB_n, 215,
		216, 217, 218, 219, 220, 221, 222, 223,
		LD_ff_n_A, POP, LD_ff_c_A, 227, 228, PUSH, 230, 231,
		232, 233, LD_nn_A, 235, 236, 237, 238, 239,
		LD_A_ff_n, POP, 242, 243, 244, PUSH, 246, 247,
		248, LD_SP_HL, 250, EI, 252, 253, CP_n, RST,
	},

	.cbPrefixed =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		RL, RL, RL, RL, RL, RL, 0, RL, 0, 0, 0, 0, 0, 0, 0, 0,
		SLA, SLA, SLA, SLA, SLA, SLA, 0, SLA, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, 0, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, 0, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, 0, BIT,
		BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, 0, BIT,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET,
		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET,
		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET,
		SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, SET, 0, SET
	}
};

struct Cpu cpu =
{
	.clock = 0,
	.stopped = 0,
	.prefix = 0
};

struct Interrupt interrupt;

void parseOpCode(struct OpCode* opCode, int hex)
{
	opCode->x = hex >> 6;		//11000000
	opCode->y = hex >> 3 & 7;	//00111000
	opCode->z = hex & 7;		//00000111
	opCode->p = opCode->y >> 1; //00110111
	opCode->q = opCode->y & 1;	//00001000
}

void initCPU()
{
	reg.SP = 0;
	reg.PC = 0;

	// Init reference registers
	reg.r[0] = &reg.B;
	reg.r[1] = &reg.C;
	reg.r[2] = &reg.D;
	reg.r[3] = &reg.E;
	reg.r[4] = &reg.H;
	reg.r[5] = &reg.L;
	reg.r[6] = &reg.F; // Useless
	reg.r[7] = &reg.A;

	// Set references for register index arrays
	reg.rp[0] = reg.rp2[0] = &reg.BC;	// BC
	reg.rp[1] = reg.rp2[1] = &reg.DE;	// DE
	reg.rp[2] = reg.rp2[2] = &reg.HL;	// HL
	reg.rp[3] = &reg.SP;				// SP
	reg.rp2[3] = &reg.AF;				// AF

	cpu.clock = 0;
	cpu.stopped = 0;
	cpu.prefix = 0;
	//opcodeTable.unPrefixed[2][0][0] = &opcodeTable.alu; opcodeTable.alu[7] = &CP;

	loadRom("Pokemon_Red.gb");
}

void resetCPU()
{
	for (int i = 0; i < 8; ++i)
	{
		*reg.r[i] = 0;
	}
	reg.SP = 0;
	reg.PC = 0;
}

void step()
{
	// Skip step is cpu is currently stopped
	if (cpu.stopped) 
		return;

	Byte op = rb(reg.PC++);

	// Get opcode
	parseOpCode(&opcode, op);
	

	//printf("0x%-2x 0x%02x %-24s %3d pre=%d x=%d z=%d y=%d q=%d p=%d", reg.PC-1, op, opNames[op], op, cpu.prefix, opcode.x, opcode.z, opcode.y, opcode.q, opcode.p);	

	//printf(" A=%x F=%x B=%x C=%x D=%x E=%x H=%x L=%x PC=%x SP=%x (HL)=%x (SP)=%x\n", reg.A, reg.F, reg.B, reg.C, reg.D, reg.E, reg.H, reg.L, reg.PC, reg.SP, rb(reg.HL), rb(reg.SP));

	if (reg.C == 0x0 && reg.PC-1 > 0x60 && reg.PC-1 < 0x73)
		NOP();

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
}

void NOP()
{
}

void STOP()
{
	reg.PC++;
	cpu.stopped = 1;
}

void RRCA()
{
	Byte carry = reg.A & 0x01;
	if (carry) 
		FLAGSET(FC);
	else 
		FLAGCLEAR(FC);

	reg.A >>= 1;
	if (carry) 
		reg.A |= 0x80;

	FLAGCLEAR(FN | FZ | FH);
}

void RLA()
{
	int carry = FLAGISSET(FC) ? 1 : 0;

	FLAGEMPTY();

	if (reg.A & 0x80)
		FLAGSET(FC);

	reg.A <<= 1;
	reg.A += carry;

	if (!reg.A)
		FLAGSET(FZ);
}

void LD_rp_nn()
{
	*reg.rp[opcode.p] = nextOps();
}

void LD()
{
	*reg.r[opcode.y] = *reg.r[opcode.z];
}

void LD_n()
{
	*reg.r[opcode.y] = nextOp();
}

void LDi_A_rp()
{
	reg.A = rw(*reg.rp[opcode.p]);
}

void ADD_HL_rp()
{
	unsigned int res = reg.HL + *reg.rp[opcode.p];

	if (res & 0xFFFF0000) 
		FLAGSET(FC);
	else 
		FLAGCLEAR(FC);

	reg.HL = (unsigned short)(res & 0xFFFF);

	FLAGCLEAR(FN);

	if (((reg.HL & 0x0F) + (*reg.rp[opcode.p] & 0x0F)) > 0x0F) 
		FLAGSET(FH);
	else 
		FLAGCLEAR(FH);
}

void LDi_HLp_A()
{
	wb(reg.HL++, reg.A);
}

void LDi_HLm_A()
{
	wb(reg.HL--, reg.A);
}

void INC_rp()
{
	(*reg.rp[opcode.p])++;
}

void INC()
{
	if ((*reg.r[opcode.y] & 0x0F) == 0x0F)
		FLAGSET(FH);
	else
		FLAGCLEAR(FH);

	(*reg.r[opcode.y])++;

	if (!*reg.r[opcode.y]) 
		FLAGSET(FZ);
	else
		FLAGCLEAR(FZ);

	FLAGCLEAR(FN);
}

void DEC()
{
	Byte val = *reg.r[opcode.y];

	if (val & 0x0F)
		FLAGCLEAR(FH);
	else 
		FLAGSET(FH);

	val = --(*reg.r[opcode.y]);

	if (val) 
		FLAGCLEAR(FZ);
	else 
		FLAGSET(FZ);

	FLAGSET(FN);

	return val;
}

void LD_nn_SP()
{
	ww(nextOps(), reg.SP);
}

void LD_nn_A()
{
	wb(nextOps(), reg.A);
}

void JR_n()
{
	signed char a = (signed char)nextOps();
	//printf("JUMP RIGHT HERE %x\n", a);
	reg.PC += a - 1;

	cpu.clock += 4;
}

void JR_nc_n()
{
	if (!(reg.F & opcodeTable.cc[opcode.y - 4]))
	{
		signed char a = (signed char)nextOp();
		//printf("JUMP RIGHT HERE %x\n", a);
		reg.PC += a - 1;

		cpu.clock += 4;
	}
	reg.PC++;
}

void JR_cc_n()
{
	if (reg.F & opcodeTable.cc[opcode.y - 4])
	{
		signed char a = (signed char)nextOp();
		//printf("JUMP RIGHT HERE %x\n", a);
		reg.PC += a - 1;

		cpu.clock += 4;
	}
	reg.PC++;
}

// Add z to A, leaving result in A (ADD A, z)
void ADD()
{
	Byte a = reg.A;
	reg.A += *reg.r[opcode.z];
	reg.F = 0;
	if (!reg.A) 
		FLAGSET(FZ);
	if (reg.A > 0xFF) 
		FLAGSET(FC);
	if ((reg.A ^ *reg.r[opcode.z] ^ a) & 0x10)
		FLAGSET(FH);

	reg.A &= 255;
}

void ADD_HL()
{
	Byte a = reg.A;
	Byte val = rb(reg.HL);
	reg.A += rb(reg.HL);
	reg.F = 0;
	if (!reg.A) 
		FLAGSET(FZ);
	if (reg.A > 0xFF) 
		FLAGSET(FC);
	if ((reg.A ^ val ^ a) & 0x10)
		FLAGSET(FH);

	reg.A &= 255;
}

void SUB()
{
	Byte val = *reg.r[opcode.z];

	FLAGEMPTY();

	if (val > reg.A)
		FLAGSET(FC);

	if ((val & 0x0F) > (reg.A & 0x0F))
		FLAGSET(FH);

	reg.A -= val;

	if (!reg.A)
		FLAGSET(FZ);

	FLAGSET(FN);
}

void SUB_n()
{
	Byte val = nextOp();

	FLAGEMPTY();

	if (val > reg.A)
		FLAGSET(FC);

	if ((val & 0x0F) > (reg.A & 0x0F))
		FLAGSET(FH);

	reg.A -= val;

	if (!reg.A)
		FLAGSET(FZ);

	FLAGSET(FN);
}



void SBC_A()
{
	Byte val = *reg.r[opcode.y];
	val += ISFC ? 1 : 0;

	reg.F = 0;

	FLAGSET(FN);

	if (val > reg.A) FLAGSET(FC);
	if (val == reg.A) FLAGSET(FZ);
	if ((val & 0x0F) > (reg.A & 0x0F)) FLAGSET(FH);

	reg.A -= val;
}

void XOR()
{
	reg.A ^= *reg.r[opcode.z];
	
	FLAGEMPTY();

	if (!reg.A) 
		FLAGSET(FZ);
}

void CP()
{
	FLAGEMPTY();

	FLAGSET(FN);
	if (reg.A == *reg.r[opcode.z])
		FLAGSET(FZ);
	if (reg.A < *reg.r[opcode.z])
		FLAGSET(FC);
	if ((*reg.r[opcode.z] & 0x0F) > (reg.A & 0x0F))
		FLAGSET(FH);
}

void CP_HL()
{
	FLAGEMPTY();

	FLAGSET(FN);
	if (reg.A == rb(reg.HL))
		FLAGSET(FZ);
	if (reg.A < rb(reg.HL))
		FLAGSET(FC);
	if ((rb(reg.HL) & 0x0F) > (reg.A & 0x0F))
		FLAGSET(FH);
}

void CP_n()
{
	Byte operand = nextOp();

	FLAGSET(FC);

	if (reg.A == operand) FLAGSET(FZ);
	else FLAGCLEAR(FZ);

	if (operand > reg.A) FLAGSET(FC);
	else FLAGCLEAR(FC);

	if ((operand & 0x0F) > (reg.A & 0x0F)) FLAGSET(FH);
	else FLAGCLEAR(FH);
}

void LD_ff_n_A()
{
	wb(0xFF00 + nextOp(), reg.A);
}

void LD_A_ff_n()
{
	reg.A = rb(0xFF00 + nextOp());
}

void LD_SP_HL()
{
	reg.SP = reg.HL;
}

void POP()
{
	Byte p = opcode.p;

	*reg.rp2[p] = rw(reg.SP);
	reg.SP += 2;
}

void RET()
{
	reg.PC = rwStack();
	reg.PC += 3;
}

void RETI()
{
	interrupt.master = 1;
	reg.PC = rwStack();
}

void LD_ff_c_A()
{
	wb(0xFF00 + reg.C, reg.A);
}

void PUSH()
{
	Byte p = opcode.p;

	reg.SP -= 2;
	ww(reg.SP, *reg.rp2[p]);
}

void LD_A_nn()
{
	Word addr = nextOps();
	reg.A = rb(addr);
}

/*void JP_nn()
{
	signed char a = nextOps();
	//printf("JUMP RIGHT HERE %x\n", a);
	reg.PC += a - 1;

	cpu.clock += 4;
}*/

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

void CALL()
{
	wwStack(reg.PC-1);
	Word a = nextOps();
	printf("CALL RIGHT HERE %x\n", a);
	reg.PC = a;//nextOps();
}

void RST()
{
	wwStack(reg.PC);
	//reg.PC = opcode.y * 8;
	reg.PC = nextOp();
}

void RST40()
{
	// Disable further interrupts
	interrupt.master = 0;

	// Save current SP on the stack
	wwStack(reg.PC);

	// Jump to handler
	reg.PC = 0x40;
}

void RL()
{
	int carry = FLAGISSET(FC) ? 1 : 0;

	FLAGEMPTY();

	if (*reg.r[opcode.z] & 0x80)
		FLAGSET(FC);

	*reg.r[opcode.z] <<= 1;
	*reg.r[opcode.z] += carry;

	if (!*reg.r[opcode.z])
		FLAGSET(FZ);
}

void SLA()
{
	FLAGEMPTY();

	if (*reg.r[opcode.z] & 0x80) 
		FLAGSET(FC);

	*reg.r[opcode.z] <<= 1;

	if (!*reg.r[opcode.z]) 
		FLAGSET(FZ);
}

void BIT()
{
	if (!(*reg.r[opcode.z] & (1 << opcode.y)))
		FLAGSET(FZ);

	FLAGCLEAR(FN);
	FLAGSET(FH);
}

void SET()
{
	*reg.r[opcode.z] |= (1 << opcode.y);
}
