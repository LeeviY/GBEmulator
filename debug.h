#pragma once
typedef unsigned char Byte;

extern int debugStop;
extern int debugEnable;
extern int debugPrint;

void debugOn(int breakPoint);
void debugOff();
void printToggle();
void printCpu(Byte op, char end);
void printRegs(char end);
void printInt(char end);
void printPpu(char end);
void printUniquePCs();