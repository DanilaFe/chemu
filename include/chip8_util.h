#ifndef CHIP8_UTIL_HEADER
#define CHIP8_UTIL_HEADER
#define CHIP8_USE_LOG 1
#include "chip8.h"

void chipDraw(chip_t* chip);
void chipLog(const char* logtext);
void chipPrintInfo();

#endif /* end of include guard: CHIP8_UTIL_HEADER */
