//
// Created by Billy on 27/01/2024.
//

#include "../headers/ram.h"

int init_RAM(structRam* theRAM, unsigned int theSize)
{
    // we create an array of size "size" with each
    // square filled with 0 (calloc)
    theRAM->content = (uint8_t*)malloc(sizeof(uint8_t) * theSize);
    theRAM->sizeRAM = theSize;
    // if it's not successful, we return 1
    if (theRAM == NULL)
        return 1;

    // unless, it works, so we return 0
    return 0;
}

int destroy_RAM(structRam* theRAM)
{
    // here we just destroy the array(memory)
    // within the ram
    free(theRAM->content);
    theRAM = NULL;

    if (theRAM == NULL)
        return 0;
    return 1;
}

int write_to_RAM(uint8_t* addr, structRam* theRAM, uint8_t value_to_write)
{
    // we get the starting address of the ram's memory
    uint8_t* debAdr = (theRAM->content)+512; // 512

    // we loop until we get the address we supposed
    // to change the content of
    while(addr != debAdr)
    {
        debAdr++;
    }

    // if we didn't find it, that's mean, there is problem
    // so we return 1

    if (addr != debAdr)
        return 1;
    // unless we found it, so we change the address's content
    *debAdr = value_to_write;
    // in that case, we return 0
    return 0;
}

int read_from_RAM(uint8_t* addr, structRam* theRAM, uint8_t** addrContent)
{
    uint8_t* debAdr = (theRAM->content)+512; // 512
    while(addr != debAdr)
    {
        debAdr++;
    }
    // if I find the address, then I change the value of this addrContent
    if (addr != debAdr)
        return 1;

    *addrContent = debAdr;
    return 0;
}

int write_to_RAM_from_ADR(uint8_t* fromAdr,uint8_t* toAdr, uint8_t* startingAdr,
                          structRam* theRAM)
{
    // the starting address of the ram
    uint8_t* debAdr = (theRAM->content)+0x200; // 512
    //now, we loop until we get the address we supposed to
    // start writing from
    while(startingAdr != debAdr)
    {
        debAdr++;
    }
    // if we didn't get the address, that's mean
    // starting's adress wasn't a valid address from start
    if (startingAdr != debAdr)
        return 1;

    // unless we found it, we loop until we write what
    // we want to write
    for(uint8_t* i = fromAdr; i <= toAdr; i++)
    {
        *debAdr = *i;
        debAdr++;
    }

    return 0;
}

void print_ram_content(structRam* ram)
{
    unsigned i = 0;
    while(ram->content[i] != 0)
    {
        printf("%02x", ram->content[i]);
        i++;
    }
    printf("\n");
}

uint16_t concat_instruction(structRam* theRam, uint16_t* pc)
{
    if (*pc >= theRam->sizeRAM - 1) {
        // Handle the error: either return a special value or handle it another way
        return 0xFFFF; // Example error value, adjust as needed
    }

    return (theRam->content[*pc] << 8) | theRam->content[*pc + 1];
}


