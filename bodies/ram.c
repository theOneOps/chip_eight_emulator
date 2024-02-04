//
// Created by Billy on 27/01/2024.
//

#include "../headers/ram.h"

int init_RAM(structRam* theRAM)
{

    // we initialize the ram with 0 for each cell (4096 cells)
    for (int iBoucle = 0; iBoucle < RAM_SIZE;iBoucle++)
    {
        theRAM->content[iBoucle] = 0;
    }
    return 0;
}

int destroy_RAM(structRam** theRAM)
{
    // here we just destroy the array(memory) of the ram
    free(*theRAM);
    *theRAM = NULL;

    return 0;
}

int write_to_RAM(uint8_t addr, structRam* theRAM, uint8_t value_to_write)
{
    // we check if the address is in the range of the ram
    if ((int)addr >= 0 && (int)addr < RAM_SIZE)
    {
        // if it is, we write the value in the ram
        theRAM->content[addr] = value_to_write;
        // and we return 0
        return 0;
    }
    // unless we return 1
    return 1;
}

int read_from_RAM(uint8_t addr, structRam* theRAM, uint8_t* contentToChange)
{
    // if the address is in the range of the ram
    if ((int)addr >= 0 && (int)addr < RAM_SIZE)
    {
        // we read the value in the ram, and we store it in the pointer
        *contentToChange = theRAM->content[addr];
        // and we return 0
        return 0;
    }
    // unless we return 1
    return 1;
}

void print_ram_content(structRam* ram, size_t length)
{
    // we print the content of the ram, from 512 to 512 + length
    // because the ram's memory used is from 512 to 4096
    for (int i = 512; i < (length + 512); i++)
    {
        printf("i %04x : %02x \n",i, ram->content[i]);
    }
    printf("\n");
}

