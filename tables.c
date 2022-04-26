#pragma once

#include "tables.h"

const unsigned char insTicks[256] =
{
//  0  1  2  3  4  5  6  7   8  9  a  b  c  d  e  f
	1, 3, 2, 2, 1, 1, 2, 1,  5, 2, 2, 2, 1, 1, 2, 1,// 0
	0, 3, 2, 2, 1, 1, 2, 1,  3, 2, 2, 2, 1, 1, 2, 1,// 1
	2, 3, 2, 2, 1, 1, 2, 1,  2, 2, 2, 2, 1, 1, 2, 1,// 2
	2, 3, 2, 2, 3, 3, 3, 1,  2, 2, 2, 2, 1, 1, 2, 1,// 3
							    
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// 4
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// 5
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// 6
	2, 2, 2, 2, 2, 2, 1, 2,  1, 1, 1, 1, 1, 1, 2, 1,// 7
							    
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// 8
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// 9
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// a
	1, 1, 1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1, 2, 1,// b
							    
	2, 3, 3, 4, 3, 4, 2, 4,  2, 4, 3, 0, 3, 6, 2, 4,// c
	2, 3, 3, 0, 3, 4, 2, 4,  2, 4, 3, 0, 3, 0, 2, 4,// d
	3, 3, 2, 0, 0, 4, 2, 4,  4, 1, 4, 0, 0, 0, 2, 4,// e
	3, 3, 2, 1, 0, 4, 2, 4,  3, 2, 4, 1, 0, 0, 2, 4,// f
};


const unsigned char cbInsTicks[256] =
{
//  0  1  2  3  4  5  6  7   8  9  a  b  c  d  e  f
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// 0
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// 1
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// 2
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// 3

	2, 2, 2, 2, 2, 2, 3, 2,  2, 2, 2, 2, 2, 2, 3, 2,// 4
	2, 2, 2, 2, 2, 2, 3, 2,  2, 2, 2, 2, 2, 2, 3, 2,// 5
	2, 2, 2, 2, 2, 2, 3, 2,  2, 2, 2, 2, 2, 2, 3, 2,// 6
	2, 2, 2, 2, 2, 2, 3, 2,  2, 2, 2, 2, 2, 2, 3, 2,// 7

	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// 8
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// 9
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// a
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// b

	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// c
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// d
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// e
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,// f
};

const char* opNames[256] =
{
	"NOP","LD BC, 0x%04X","LD (BC), A","INC BC","INC B","DEC B","LD B, 0x%02X","RLCA",
	"LD (0x%04X), SP","ADD HL, BC","LD A, (BC)","DEC BC","INC C","DEC C","LD C, 0x%02X","RRCA",
	"STOP","LD DE, 0x%04X","LD (DE), A","INC DE","INC D","DEC D","LD D, 0x%02X","RLA",
	"JR 0x%02X","ADD HL, DE","LD A, (DE)","DEC DE","INC E","DEC E","LD E, 0x%02X","RRA",
	"JR NZ, 0x%02X","LD HL, 0x%04X","LDI (HL), A","INC HL","INC H","DEC H","LD H, 0x%02X","DAA",
	"JR Z, 0x%02X","ADD HL, HL","LDI A, (HL)","DEC HL","INC L","DEC L","LD L, 0x%02X","CPL",
	"JR NC, 0x%02X","LD SP, 0x%04X","LDD (HL), A","INC SP","INC (HL)","DEC (HL)","LD (HL), 0x%02X","SCF",
	"JR C, 0x%02X","ADD HL, SP","LDD A, (HL)","DEC SP","INC A","DEC A","LD A, 0x%02X","CCF",
	"LD B, B","LD B, C","LD B, D","LD B, E","LD B, H","LD B, L","LD B, (HL)","LD B, A",
	"LD C, B","LD C, C","LD C, D","LD C, E","LD C, H","LD C, L","LD C, (HL)","LD C, A",
	"LD D, B","LD D, C","LD D, D","LD D, E","LD D, H","LD D, L","LD D, (HL)","LD D, A",
	"LD E, B","LD E, C","LD E, D","LD E, E","LD E, H","LD E, L","LD E, (HL)","LD E, A",
	"LD H, B","LD H, C","LD H, D","LD H, E","LD H, H","LD H, L","LD H, (HL)","LD H, A",
	"LD L, B","LD L, C","LD L, D","LD L, E","LD L, H","LD L, L","LD L, (HL)","LD L, A",
	"LD (HL), B","LD (HL), C","LD (HL), D","LD (HL), E","LD (HL), H","LD (HL), L","HALT","LD (HL), A",
	"LD A, B","LD A, C","LD A, D","LD A, E","LD A, H","LD A, L","LD A, (HL)","LD A, A",
	"ADD A, B","ADD A, C","ADD A, D","ADD A, E","ADD A, H","ADD A, L","ADD A, (HL)","ADD A",
	"ADC B","ADC C","ADC D","ADC E","ADC H","ADC L","ADC (HL)","ADC A",
	"SUB B","SUB C","SUB D","SUB E","SUB H","SUB L","SUB (HL)","SUB A",
	"SBC B","SBC C","SBC D","SBC E","SBC H","SBC L","SBC (HL)","SBC A",
	"AND B","AND C","AND D","AND E","AND H","AND L","AND (HL)","AND A",
	"XOR B","XOR C","XOR D","XOR E","XOR H","XOR L","XOR (HL)","XOR A",
	"OR B","OR C","OR D","OR E","OR H","OR L","OR (HL)","OR A",
	"CP B","CP C","CP D","CP E","CP H","CP L","CP (HL)","CP A",
	"RET NZ","POP BC","JP NZ, 0x%04X","JP 0x%04X","CALL NZ, 0x%04X","PUSH BC","ADD A, 0x%02X","RST 0x00",
	"RET Z","RET","JP Z, 0x%04X","CB %02X","CALL Z, 0x%04X","CALL 0x%04X","ADC 0x%02X","RST 0x08",
	"RET NC","POP DE","JP NC, 0x%04X","UNKNOWN","CALL NC, 0x%04X","PUSH DE","SUB 0x%02X","RST 0x10",
	"RET C","RETI","JP C, 0x%04X","UNKNOWN","CALL C, 0x%04X","UNKNOWN","SBC 0x%02X","RST 0x18",
	"LD (0xFF00 + 0x%02X), A","POP HL","LD (0xFF00 + C), A","UNKNOWN","UNKNOWN","PUSH HL","AND 0x%02X","RST 0x20",
	"ADD SP,0x%02X","JP HL","LD (0x%04X), A","UNKNOWN","UNKNOWN","UNKNOWN","XOR 0x%02X","RST 0x28",
	"LD A, (0xFF00 + 0x%02X)","POP AF","LD A, (0xFF00 + C)","DI","UNKNOWN","PUSH AF","OR 0x%02X","RST 0x30",
	"LD HL, SP+0x%02X","LD SP, HL","LD A, (0x%04X)","EI","UNKNOWN","UNKNOWN","CP 0x%02X","RST 0x38"
};

const int numbers[256] = {
	 0,2,0,0,0,0,1,0, 2,0,0,0,0,0,1,0,
	 1,2,0,0,0,0,1,0, 1,0,0,0,0,0,1,0,
	 1,2,0,0,0,0,1,0, 1,0,0,0,0,0,1,0,
	 1,2,0,0,0,0,1,0, 1,0,0,0,0,0,1,0,

	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	 0,0,2,2,2,0,1,0, 0,0,2,1,2,2,1,0,
	 0,0,2,0,2,0,1,0, 0,0,2,0,2,0,1,0,
	 1,0,0,0,0,0,1,0, 1,0,2,0,0,0,1,0,
	 1,0,0,0,0,0,1,0, 1,0,2,0,0,0,1,0
};