#pragma once

typedef unsigned char Byte;
typedef unsigned short Word;

extern Byte _romStart[0x100];

extern Byte _rom[0x8000];
extern Byte _vram[0x2000];
extern Byte _eram[0x2000];
extern Byte _wram[0x2000];
extern Byte _sram[0x1E00];
extern Byte _oam[0xA0];
extern Byte _io[0x80];
extern Byte _hram[0x80];

extern Byte memoryMap[0xFFFF][3];

extern int inbios;

// Loads bootstrap rom from file to _bios
void loadBootstrap(char* fileName);

// Loads rom from file to _rom
void loadRom(char* fileName);

void resetMemory();

// Writes entire gb memory to file
void memoryDump();

void copyDma(Word destination, Word source, size_t length);

// Read byte from given memory address
Byte rb(Word addr);
// Read two bytes from given memory address
Word rw(Word addr);
// Write byte to given memory address
void wb(Word addr, Byte val);
// Write two bytes to given memory address
void ww(Word addr, Word val);

// Read two bytes from top of the stack
Word popFromStack();
// Write two bytes to top of the stack
void pushToStack(Word val);

// Gets one operand
Word nextByte();
// Gets two operands
Word nextWord();