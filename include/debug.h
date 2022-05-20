#pragma once
typedef unsigned char Byte;

extern int debugStop;
extern int debugEnable;
extern int debugPrint;

void debugOn(int breakPoint);
void debugOff();
void printToggle();
void printCpu(char sep, char end);
void printRegs(char sep, char end);
void printInt(char sep, char end);
void printPpu(char sep, char end);
void printUniquePCs();
void printUniqueOps(Byte op);