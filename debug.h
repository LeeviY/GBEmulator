#pragma once

extern int debugStop;
extern int debugEnable;
extern int debugPrint;

void debugOn(int breakPoint);
void debugOff();
void printToggle();