//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_RAM_H
#define CHIP_EIGHT_RAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t* content;
    unsigned int sizeRAM;
}structRam;


int init_RAM(structRam* ram, unsigned int value);

int destroy_RAM(structRam*);

int write_to_RAM(uint8_t* addr, structRam* theRAM, uint8_t value_to_write); // V
int write_to_RAM_from_ADR(uint8_t* fromAdr,uint8_t* toAdr, uint8_t* startingAdr, structRam* theRAM);
//writing starting from an address
int read_from_RAM(uint8_t* addr, structRam* theRAM, uint8_t** addrContent); // V
//reading starting from an address

void print_ram_content(structRam*);

uint16_t concat_instruction(structRam* theRam, uint16_t* pc);


#endif //CHIP_EIGHT_RAM_H
