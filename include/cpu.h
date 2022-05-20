#define _CRT_SECURE_NO_WARNINGS

#pragma once
#define ISFZ (reg.F & FZ)
#define ISFN (reg.F & FN)
#define ISFH (reg.F & FH)
#define ISFC (reg.F & FC)

#define SETFZ(x) (reg.F = reg.F & ~(FZ) | (!!(x) << 7))
#define SETFN(x) (reg.F = reg.F & ~(FN) | (!!(x) << 6))
#define SETFH(x) (reg.F = reg.F & ~(FH) | (!!(x) << 5))
#define SETFC(x) (reg.F = reg.F & ~(FC) | (!!(x) << 4))

#define printbits_n(x,n) for (int i=n;i;i--,putchar('0'|(x>>i)&1))
#define printbits_8(x) printbits_n(x,8)

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "memory.h"
#include "registers.h"
#include "ppu.h"

typedef unsigned char Byte;
typedef uint16_t Word;

enum
{
	FZ = 0x80,
	FN = 0x40,
	FH = 0x20,
	FC = 0x10
};

enum
{
	INT_VBLANK	 = (1 << 0),
	INT_LCD_STAT = (1 << 1),
	INT_TIMER	 = (1 << 2),
	INT_SERIAL	 = (1 << 3),
	INT_JOYPAD	 = (1 << 4),
};

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
	int halted;
	int toggleIntMaster; // first bit new int master value, last bit is change enable
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
void initCpu();

// Reset registers
void resetCpu();

extern unsigned int cpuclock;

// Step cpu one tick
void stepCpu();

void checkInterrupt();

//https://gb-archive.github.io/salvage/decoding_gbz80_opcodes/Decoding%20Gamboy%20Z80%20Opcodes.html

void UNDEF();
// x == 0
void NOP();
void STOP();
void LD_nni_SP();
void JR_n();
void JR_cc_n();
void LD_rp_nn();
void ADD_HL_rp();
void LD_rpi_A();
void LDI_HL_A();
void LDD_HL_A();
void LD_A_rpi();
void LDI_A_HL();
void LDD_A_HL();
void INC_rp();
void DEC_rp();
void INC();
void INC_HL();
void DEC();
void DEC_HL();
void LD_n();
void LD_HL_n();
void RLCA();
void RRCA();
void RLA();
void RRA();
void DAA();
void CPL();
void SCF();
void CCF();
// x == 1
void LD();
void LD_r_HL();
void LD_HL_r();
void HALT();
// x == 2
void ADD();
void ADD_HL();
void ADC();
void ADC_HL();
void SUB();
void SUB_HL();
void SBC_A();
void SBC_A_HL();
void AND();
void AND_HL();
void XOR();
void XOR_HL();
void OR();
void OR_HL();
void CP();
void CP_HL();
// x == 3
void RET_cc();
void LD_ff_n_A();
void ADD_SP_n();
void LD_A_ff_n();
void LD_HL_SP_n();
void POP();
void RET();
void RETI();
void JP_HL();
void LD_SP_HL();
void JP_cc_nn();
void LD_ff_C_A();
void LD_nni_A();
void LD_A_ff_C();
void LD_A_nni();
void JP_nn();
void PRFX();
void DI();
void EI();
void CALL_cc();
void PUSH();
void CALL();
void ADD_n();
void ADC_n();
void SUB_n();
void SBC_A_n();
void AND_n();
void XOR_n();
void OR_n();
void CP_n();
void RST();

// Prefixed
// x == 0
void RLC();
void RLC_HL();
void RRC();
void RRC_HL();
void RL();
void RL_HL();
void RR();
void RR_HL();
void SLA();
void SLA_HL();
void SRA();
void SRA_HL();
void SWAP();
void SWAP_HL();
void SRL();
void SRL_HL();
// x == 1
void BIT();
void BIT_HL();
// x == 2
void RES();
void RES_HL();
// x == 3
void SET();
void SET_HL();
