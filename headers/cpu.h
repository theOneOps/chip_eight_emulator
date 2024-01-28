//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_CPU_H
#define CHIP_EIGHT_CPU_H
#include "ram.h"
#include <stdbool.h>
#include "../display/display.h"

#define MAX_STACK_SIZE 16
#define INSTRUCTIONS_NUMBER 35

typedef struct StackP
{
    uint16_t items[MAX_STACK_SIZE];
    uint16_t top;
}StackP;

typedef struct cpu_t
{
    /** V registers go hand in hand with StackP because they work directly on program instructions
     The instructions and addresses are of the same type, therefore 16 bits for 2 bytes.
      And the PC goes with the register I: they are coded on 16 bits, because the addresses
      are 12 bits (2^12 = 4096 oct)
     */

    uint8_t* V;// 16 registers from 0 to 15
    StackP* SP;
    uint16_t* register_I; /** register I intervenes on operations linked to addresses */
    uint16_t* PC; /** contains the address of the next instruction */
    uint8_t* ST;
    uint8_t* DT;
}cpu_t;

// Initialization of the StackP's function (SP)

void initializeStack(StackP *sp);
bool isEmpty(StackP* sp);
bool isFull(StackP* sp);
void push(StackP *sp, uint16_t value);
int pop(StackP *sp);


// Initialization of the CPU

void initializeCpu(cpu_t* theCPU);
void destroyCpu(cpu_t* theCPU);


// allowed us to read an instruction
uint16_t read_instruction(cpu_t* theCPU, structRam* theRam);
// here we get the order that corresponds to the instruction we read

// in this function, we call the read_instruction and
// the recuperer_instruction and finally based on the order we get from
// recuperer_instruction, we execute some code and
// then we increment the program's counter

//struct Display* display, struct Keyboard* keyboard, struct Speaker* speaker
void interpreteOpCode(cpu_t* theCPU, structRam* theRam,
                      Chip8Display* display);

void SNEE(bool isEqual, cpu_t* theCPU, const uint16_t* v1, const uint16_t* v2);


#endif //CHIP_EIGHT_CPU_H
