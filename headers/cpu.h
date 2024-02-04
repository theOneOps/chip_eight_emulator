//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_CPU_H
#define CHIP_EIGHT_CPU_H

#include "ram.h"
#include <stdbool.h>
#include "../libprovided/include/display/display.h"
#include "../libprovided/include/keyboard/keyboard.h"
#include "../libprovided/include/display/sprite.h"
#include "../libprovided/include/speaker/speaker.h"

#define MAX_STACK_SIZE 16
#define START_ADRESS 512

/**
 * @brief This structure is the main structure of the stack.
 * It contains the items and the top of the stack.
 * @param items is the items of the stack, it's an array of 16 elements
 * @param top is the top of the stack
 */
typedef struct StackP
{
    uint16_t items[MAX_STACK_SIZE];
    int top;
}StackP;


/**
 * @brief This structure is the main structure of the cpu.
 * It contains the V registers, the stack pointer, the register I, the program counter, the sound timer
 * and the delay timer.
 * @param V is the V registers
 * @param SP is the stack pointer
 * @param register_I is the register I
 * @param PC is the program counter
 * @param ST is the sound timer
 * @param DT is the delay timer
 */
typedef struct cpu_t
{

    uint8_t V[MAX_STACK_SIZE];// 16 registers from 0 to 15
    StackP SP;
    uint16_t register_I;
    uint16_t PC;
    uint8_t ST;
    uint8_t DT;
}cpu_t;

// Initialization of the StackP's function (SP)

/**
 * @brief This function initializes the stack's structure.
 * @param sp is a pointer to the stack's structure
 */
void initializeStack(StackP *sp);


/**
 * @brief This function initializes the cpu's structure.
 * @param theCPU is a pointer to the cpu's structure
 */
void initializeCpu(cpu_t* theCPU);

/**
 * @brief This function destroys the cpu's structure.
 * @param theCPU is a pointer to the cpu's structure
 */
void destroyCpu(cpu_t** theCPU);


/**
 * @brief This function allows to read the instruction pointed by the program counter.
 * @param theCPU is a pointer to the cpu's structure
 * @param theRam is a pointer to the ram's structure
 * @return the instruction that corresponds to the opcode we read
 */
uint16_t read_instruction(cpu_t* theCPU, structRam* theRam);

/**
 * @brief This function interprets the opcode and executes the corresponding instruction.
 * @param theCPU is a pointer to the cpu's structure
 * @param theRam is a pointer to the ram's structure
 * @param display is a pointer to the display's structure
 * @param keyBoard is a pointer to the keyboard's structure
 */
void interpreteOpCode(cpu_t* theCPU, structRam* theRam, struct Display* display,
        struct Keyboard* keyBoard);


// functions instructions

/**
 * @brief This function executes the instruction 0x00E0.
 * @param display is a pointer to the display's structure
 */
void instruction_0000_Ingored(); // V

/**
 * @brief This function executes the instruction 0x00EE.
 * @param theCPU is a pointer to the cpu's structure
 */
void instruction_00E0_CLS(struct Display* display); // V

/**
 * @brief This function executes the instruction 0x00EE.
 * @param theCPU is a pointer to the cpu's structure
 */
void instruction_00EE_RET(cpu_t* cpu); // V

/**
 * @brief This function executes the instruction 0x1nnn.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vnnn is the value of the 12-bit value, the lowest 12 bits of the instruction
 */
void instruction_1nnn(cpu_t* cpu, uint16_t Vnnn); // V

/**
 * @brief This function executes the instruction 0x2nnn.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vnnn is the value of the 12-bit value, the lowest 12 bits of the instruction
 */
void instruction_2nnn(cpu_t* cpu, uint16_t Vnnn); // V

/**
 * @brief This function executes the instruction 0x3xkk, 0x4xkk, 0x5xy0, 0x9xy0
 * @param theCPU is a pointer to the cpu's structure
 * @param Val1 is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Val2 is either (4-bit value, the upper 4 bits of the low byte of the instruction >> 4)
 * or 8-bit value, the lowest 8 bits of the instruction
 * @param sign is the sign of the instruction (1 if it's 0x3xkk, 2 if it's 0x4xkk, 3 if it's 0x5xy0, 4 if it's 0x9xy0)
 */
void instruction_SE_SNE(cpu_t* cpu, uint16_t Val1, uint16_t Val2, int sign);

/**
 * @brief This function executes the instruction 0x6xkk.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Vkk is the 8-bit value, the lowest 8 bits of the instruction
 */
void instruction_6xkk(cpu_t* cpu, uint16_t Vx, uint16_t Vkk); // V

/**
 * @brief This function executes the instruction 0x7xkk.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Vkk is the 8-bit value, the lowest 8 bits of the instruction
 */
void instruction_7xkk(cpu_t* cpu, uint16_t Vx, uint16_t Vkk); // V

/**
 * @brief This function executes the instruction 0x8xy0, 0x8xy1, 0x8xy2, 0x8xy3.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Vy is (4-bit value, the upper 4 bits of the low byte of the instruction >> 4)
 * @param sign is the sign of the instruction (0 if it's 0x8xy0, 1 if it's 0x8xy1, 2 if it's 0x8xy2, 3 if it's 0x8xy3)
 */
void instruction_8xy03(cpu_t* cpu, uint16_t Vx, uint16_t Vy, int sign);

/**
 * @brief This function executes the instruction 0x8xy4, 0x8xy5, 0x8xy6, 0x8xy7, 0x8xyE.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Vy is (4-bit value, the upper 4 bits of the low byte of the instruction >> 4)
 * @param value is the value of the instruction (4 if it's 0x8xy4, 5 if it's 0x8xy5, 6 if it's 0x8xy6, 7 if it's 0x8xy7,
 * 14 if it's 0x8xyE)
 */
void instruction_8xy4E(cpu_t* cpu, uint16_t Vx,uint16_t Vy, int value);

/**
 * @brief This function executes the instruction 0xAnnn.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vnnn is the value of the 12-bit value, the lowest 12 bits of the instruction
 */
void instruction_Annn(cpu_t* cpu, uint16_t Vnnn); // V

/**
 * @brief This function executes the instruction 0xBnnn.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vnnn is the value of the 12-bit value, the lowest 12 bits of the instruction
 */
void instruction_Bnnn(cpu_t* cpu, uint16_t Vnnn); // V

/**
 * @brief This function executes the instruction 0xCxkk.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Vkk is the 8-bit value, the lowest 8 bits of the instruction
 */
void instruction_Cxkk(cpu_t* cpu, uint16_t Vx, uint16_t Vkk); // V

/**
 * @brief This function executes the instruction 0xDxyn.
 * @param theCPU is a pointer to the cpu's structure
 * @param theRam is a pointer to the ram's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 * @param Vy is (4-bit value, the upper 4 bits of the low byte of the instruction >> 4)
 * @param Vn is the value of the lowest 4 bits of the instruction
 * @param display is a pointer to the display's structure
 */
void instruction_Dxyn(cpu_t* cpu, structRam* ram, uint16_t Vx, uint16_t Vy, uint16_t Vn, struct Display* display);

/**
 * @brief This function executes the instruction 0xEx9E.
 * @param theCPU is a pointer to the cpu's structure
 * @param keyBoard is a pointer to the keyboard's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Ex9E(cpu_t* cpu, struct Keyboard* keyboard, uint16_t Vx);

/**
 * @brief This function executes the instruction 0xExA1.
 * @param theCPU is a pointer to the cpu's structure
 * @param keyBoard is a pointer to the keyboard's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_ExA1(cpu_t* cpu, struct Keyboard* keyboard, uint16_t Vx);

/**
 * @brief This function executes the instruction 0xFx07.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx07(cpu_t* cpu, uint16_t Vx); // V

/**
 * @brief This function executes the instruction 0xFx0A.
 * @param theCPU is a pointer to the cpu's structure
 * @param keyBoard is a pointer to the keyboard's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx0A(cpu_t* cpu, struct Keyboard* keyboard, uint16_t Vx);

/**
 * @brief This function executes the instruction 0xFx15.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx18(cpu_t* cpu, uint16_t Vx); // V

/**
 * @brief This function executes the instruction 0xFx15.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx15(cpu_t* cpu, uint16_t Vx); // V

/**
 * @brief This function executes the instruction 0xFx18.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx1E(cpu_t* cpu, uint16_t Vx); // V

/**
 * @brief This function executes the instruction 0xFx29.
 * @param theCPU is a pointer to the cpu's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx29(cpu_t* cpu, uint16_t Vx); // V

/**
 * @brief This function executes the instruction 0xFx33.
 * @param theCPU is a pointer to the cpu's structure
 * @param theRam is a pointer to the ram's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx33(cpu_t* cpu, structRam* ram, uint16_t Vx);

/**
 * @brief This function executes the instruction 0xFx55.
 * @param theCPU is a pointer to the cpu's structure
 * @param theRam is a pointer to the ram's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx55(cpu_t* cpu, structRam* ram, uint16_t Vx); // V

/**
 * @brief This function executes the instruction 0xFx65.
 * @param theCPU is a pointer to the cpu's structure
 * @param theRam is a pointer to the ram's structure
 * @param Vx is (4-bit value, the lower 4 bits of the high byte of the instruction >> 8)
 */
void instruction_Fx65(cpu_t* cpu, structRam* ram, uint16_t Vx); // V


#endif //CHIP_EIGHT_CPU_H
