//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_EMULATOR_H
#define CHIP_EIGHT_EMULATOR_H

#include "ram.h"
#include "cpu.h"
#include "../display/display.h"

typedef struct
{
    structRam* memory;
    cpu_t* processor;
    Chip8Display* theDisplay;
}emulator;


int initEmulator(emulator* theEmulator, unsigned int emulator_size);
void destroyEmulator(emulator*);
int read_program(const char* rom, emulator*);
bool runEmulator(emulator* theEmulator);

#endif //CHIP_EIGHT_EMULATOR_H
