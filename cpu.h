#define _CRT_SECURE_NO_WARNINGS

#pragma once

#define FZ 0x80
#define FN 0x40
#define FH 0x20
#define FC 0x10

#define ISFZ (reg.F & FZ)
#define ISFN (reg.F & FN)
#define ISFH (reg.F & FH)
#define ISFC (reg.F & FC)

#define SETFZ(x) (reg.F = reg.F & ~(FZ) | (!!(x) << 7))
#define SETFN(x) (reg.F = reg.F & ~(FN) | (!!(x) << 6))
#define SETFH(x) (reg.F = reg.F & ~(FH) | (!!(x) << 5))
#define SETFC(x) (reg.F = reg.F & ~(FC) | (!!(x) << 4))

#define VBLANK (1 << 0)
#define LCD_STAT (1 << 1)
#define TIMER (1 << 2)
#define SERIAL (1 << 3)
#define JOYPAD (1 << 4)

#define printbits_n(x,n) for (int i=n;i;i--,putchar('0'|(x>>i)&1))
#define printbits_8(x) printbits_n(x,8)

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "memory.h"
#include "registers.h"
#include "gpu.h"

typedef unsigned char Byte;
typedef uint16_t Word;

// Stores function pointers to cpu operations
struct OpcodeTable
{
	Byte cc[4]; // Conditions
	void* alu[8]; // Arithmetic/logic operations
	void* rot[8]; // Rotation/shift operations

	void (*unPrefixed[256])(); // Normal operations
	void (*cbPrefixed[256])(); // Operations with cb prefix
} extern opcodeTable;

// Struct for storing parsed values of opcode
struct OpCode
{
	Byte x; // First two bits
	Byte y; // Middle three bits
	Byte z; // Last three bits
	Byte p;	// Last bit of y
	Byte q; // First two bits of y
} extern opcode;

// Struct for storing cpu variables
struct Cpu
{
	unsigned long clock;
	int stopped;
	Byte prefix;
} extern cpu;

struct Interrupt
{
	Byte master;
	Byte enable;
	Byte flags;

} extern interrupt;

// Decodes opcode into OpCode struct
void parseOpCode(struct OpCode* opCode, int hex);

// Initialize processor
void initCPU();

// Reset registers
void resetCPU();

// Step cpu one tick
void step();

void checkInterrupt();

//https://gb-archive.github.io/salvage/decoding_gbz80_opcodes/Decoding%20Gamboy%20Z80%20Opcodes.html
// x == 0
void NOP();
void STOP();
void LD_nn_SP();
void JR_n();
void JR_nc_n();
void JR_cc_n();
void LD_rp_nn();
void ADD_HL_rp();
void LDi_HLp_A();
void LDi_HLm_A();
void LDi_A_rp();
void INC_rp();
void INC();
void DEC();
void LD_n();
//void LD_HL_n();
void RRCA();
void RLA();
// x == 1
void LD();
// x == 2
void ADD();
void ADD_HL();
void SUB();
void SUB_n();
void SBC_A();
void AND();
void AND_n();
void XOR();
void CP();
void CP_HL();
void CP_n();
// x == 3
void LD_ff_n_A();
void LD_A_ff_n();
void LD_SP_HL();
void POP();
void RET();
void RETI();
void LD_ff_c_A();
void LD_nn_A();
void LD_A_nn();
void JP_nn();
void PRFX();
void DI();
void EI();
void CALL();
void PUSH();
void RST();
void RST40();

// Prefixed
// x == 0
void RL();
void SLA();
// x == 1
void BIT();
// x == 2
void RES();
// x == 3
void SET();