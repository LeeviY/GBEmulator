#pragma once

typedef unsigned char Byte;
typedef unsigned short Word;

extern const unsigned char ioReset[0x100];

extern Byte _rom[0x8000];	// Rom0 and 1
extern Byte _oam[0x100];	// Object Attribute Memory
extern Byte _vram[0x2000];	// Video ram
extern Byte _wram[0x2000];	// Working ram
extern Byte _eram[0x2000];	// Echo ram
extern Byte _io[0x100];		// IO
extern Byte _zram[0x80];	// Zero page

// Loads rom from file to _rom
void loadRom(char* fileName);

void copy(Word destination, Word source, size_t length);

// Read byte from given memory address
Byte rb(Word addr);
// Read two bytes from given memory address
Word rw(Word addr);
// Write byte to given memory address
void wb(Word addr, Byte val);
// Write two bytes to given memory address
void ww(Word addr, Word val);

// Read two bytes from top of the stack
Byte rwStack();
// Write two bytes to top of the stack
void wwStack(Word val);

// Gets one operand
Byte nextOp();
// Gets two operands
Word nextOps();