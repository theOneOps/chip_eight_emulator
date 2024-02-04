//
// Created by Billy on 27/01/2024.
//

#include "../headers/cpu.h"

void initializeStack(StackP* sp)
{
    // we initialize the top of the stack to -1
    sp->top = -1;
}

// Initialization of the cpu
void initializeCpu(cpu_t* theCPU)
{
    theCPU->PC = 0x200;
    theCPU->DT = 0;
    initializeStack(&theCPU->SP);
    theCPU->ST = 0;
    for(int iBoucle = 0; iBoucle < MAX_STACK_SIZE; iBoucle++)
    {
        theCPU->V[iBoucle] = 0;
        theCPU->SP.items[iBoucle] = 0;
    }

    theCPU->register_I = 0;

}


// Destruction of the cpu
void destroyCpu(cpu_t** theCPU)
{
    free(*theCPU);
    *theCPU = NULL;
}

// static only presented in the file


uint16_t read_instruction(cpu_t* theCPU, structRam* theRam)
{
    // Fetch the high byte and shift it left by 8 bits
    uint16_t high_byte = theRam->content[theCPU->PC] << 8;

    // Fetch the low byte
    uint16_t low_byte = theRam->content[theCPU->PC + 1];

    // Combine the high and low bytes to form the instruction
    uint16_t instruction = high_byte | low_byte;

    return instruction;
}

void interpreteOpCode(cpu_t* theCPU, structRam* theRam,
                      struct Display* display, struct Keyboard* keyBoard)
{
    uint16_t instruction = read_instruction(theCPU, theRam);
    theCPU->PC += 2;
    uint16_t fixedPart = instruction & 0xF000;

    // the main bits to extract
    uint16_t x = (instruction & 0x0F00) >> 8;
    uint16_t nnn = instruction & 0x0FFF;
    uint16_t kk = instruction & 0x00FF;
    uint16_t n = instruction & 0x000F; // Height of the sprite
    uint16_t y = (instruction & 0x00F0) >> 4;

    switch(fixedPart)
    {
        case 0x0000:
        {
            uint16_t value = instruction & 0x0FFF;
            switch(value)
            {
                case 0x00E0: // CLS display
                {
                    instruction_00E0_CLS(display);
                    break;
                }

                case 0x00EE: // 00EE
                {
                    //printf("returns from a subroutine 00E0 \n");
                    instruction_00EE_RET(theCPU);
                    break;
                }

                default: // SYS addr - This instruction is generally ignored in modern interpreters
                {
                    instruction_0000_Ingored();
                    break;
                }
            }
            break;
        }

        case 0x1000:
        {
            //printf("Jump JP addr \n");
            instruction_1nnn(theCPU, nnn);
            break;
        }


        case 0x2000:
        {
            //printf("call subroutine at nnn \n");
            instruction_2nnn(theCPU, nnn);
            break;
        }

        case 0x3000:
        {
            //printf("3xkk - SE Vx, byte \n");
            instruction_SE_SNE(theCPU, x, kk, 1);
            break;
        }

        case 0x4000:
        {
            //printf("4xkk - SNE Vx, byte \n");
            instruction_SE_SNE(theCPU, x, kk, 2);
            break;
        }

        case 0x5000:
        {
            //printf("5xy0 - SE Vx, Vy \n");
            instruction_SE_SNE(theCPU, x, y, 3);
            break;
        }

        case 0x6000:
        {
            //printf(" Load normal register with immediate value\n");
            instruction_6xkk(theCPU, x, kk);
            break;
        }

        case 0x7000:
        {
            //printf("7xkk - ADD Vx, byte \n");
            instruction_7xkk(theCPU, x, kk);
            break;
        }

        case 0x8000:
        {
            uint16_t newFixedPart = instruction & 0x000F;
            switch(newFixedPart)
            {
                case 0x0000: //0x8000
                {
                    //printf("8xy0 - LD Vx, Vy \n");
                    instruction_8xy03(theCPU, x, y, 0);
                    break;
                }
                case 0x0001: //0x8001
                {
                    //printf("8xy1 - OR Vx, Vy \n");
                    instruction_8xy03(theCPU, x, y, 1);
                    break;
                }
                case 0x0002: //0x8002
                {
                    //printf("8xy2 - AND Vx, Vy");
                    instruction_8xy03(theCPU, x, y, 2);
                    break;
                }
                case 0x0003: //0x8003
                {
                    //printf("8xy3 - XOR Vx, Vy \n");
                    instruction_8xy03(theCPU, x, y, 3);
                    break;
                }
                case 0x0004: //0x8004
                {
                    //printf("8xy4 - ADD Vx, Vy \n");
                    instruction_8xy4E(theCPU, x, y, 4);
                    break;
                }
                case 0x0005: //0x8005
                {
                    //printf("8xy5 - SUB Vx, Vy \n");
                    instruction_8xy4E(theCPU, x, y, 5);
                    break;
                }
                case 0x0006: //0x8006
                {
                    //printf("8xy6 - SHR Vx {, Vy} \n");
                    instruction_8xy4E(theCPU, x, y, 6);
                    break;
                }

                case 0x0007: //0x8007
                {
                    //printf("8xy7 - SUBN Vx, Vy \n");
                    instruction_8xy4E(theCPU, x, y, 7);
                    break;
                }

                case 0x000E: //0x800E
                {
                    //printf("8xyE - SHL Vx {, Vy} \n");
                    instruction_8xy4E(theCPU, x, y, 14);
                    break;
                }

                default: // SYS addr - This instruction is generally ignored in modern interpreters
                    //printf("SYS call to address 0x%03X ignored\n", newFixedPart);
                    break;
            }
            break;
        }

        case 0x9000:
        {
            //printf("9xy0 - SNE Vx, Vy \n");
            instruction_SE_SNE(theCPU, x, y, 4);
            break;
        }

        case 0xA000:
        {
            //printf("Load index register with immediate value Annn - LD I, addr \n");
            instruction_Annn(theCPU, nnn);
            break;
        }

        case 0xB000:
        {
            //printf("Bnnn - JP V0, addr \n");
            instruction_Bnnn(theCPU, nnn);
            break;
        }
        case 0xC000:
        {
            //printf("Cxkk - RND Vx, byte \n");
            instruction_Cxkk(theCPU, x, kk);
            break;
        }
        case 0xD000: // draw a sprite
        {
            //printf("draw a sprite \n");
            instruction_Dxyn(theCPU,theRam, x, y, n, display);
            break;
        }

        case 0xE000:
        {
            switch (instruction & 0x00FF)
            {
                case 0x009E: // keyboard
                {
                    instruction_Ex9E(theCPU, keyBoard, x);
                    break;
                }
                case 0x00A1: // keyboard
                {
                    instruction_ExA1(theCPU, keyBoard, x);
                    break;
                }
                default: // SYS addr - This instruction is generally ignored in modern interpreters
                    instruction_0000_Ingored();
                    break;
            }
            break;
        }
        case 0xF000:
        {
            switch(instruction & 0x00FF)
            {
                case 0x0007:
                {
                    instruction_Fx07(theCPU, x);
                    break;
                }

                case 0x000A: // keyboard
                {
                    instruction_Fx0A(theCPU, keyBoard, x);
                    break;
                }

                case 0x0015: // delay timer
                {
                    instruction_Fx15(theCPU, x);
                    break;
                }

                case 0x0018: // sound timer
                {
                    instruction_Fx18(theCPU, x);
                    break;
                }

                case 0x001E: // delay timer
                {
                    instruction_Fx1E(theCPU, x);
                    break;
                }

                case 0x0029: // sprite concerned
                {
                    instruction_Fx29(theCPU, x);
                    break;
                }

                case 0x0033: // register I
                {
                    instruction_Fx33(theCPU, theRam, x);
                    break;
                }

                case 0x0055:
                {
                    instruction_Fx55(theCPU,theRam, x);
                    break;
                }

                case 0x0065: // same as the previous instruction, but in read mode
                {
                    instruction_Fx65(theCPU,theRam, x);
                    break;
                }

                default: // SYS addr - This instruction is generally ignored in modern interpreters
                    instruction_0000_Ingored();
                    break;
            }
            break;
        }
        default:
            instruction_0000_Ingored();
            break;
    }
}


void instruction_0000_Ingored()
{
    //printf("SYS call to address ignored\n ");
}

void instruction_00E0_CLS(struct Display* display)
{
    Display_CLS(display);
}

void instruction_00EE_RET(cpu_t* cpu)
{
    // before changing the PC, we need to check if the stack is not empty or the top is upper than 15
    if (cpu->SP.top >= 0 && cpu->SP.top <= 15)
    {
        cpu->PC = cpu->SP.items[cpu->SP.top];
        cpu->SP.top -= 1; // Decrement the stack pointer
    }
}


void instruction_1nnn(cpu_t* cpu, uint16_t Vnnn)
{
    cpu->PC = Vnnn;
}
void instruction_2nnn(cpu_t* cpu, uint16_t Vnnn)
{
    // before adding the PC's value to the stack, we need to check if the stack is not full
    if (cpu->SP.top < 15)
    {
        cpu->SP.top += 1;
        cpu->SP.items[cpu->SP.top] = cpu->PC;
        cpu->PC = Vnnn; // Jump to the subroutine at nnn
    }
}

void instruction_SE_SNE(cpu_t* cpu, uint16_t Val1, uint16_t Val2, int sign)
{
    switch(sign)
    {
        case 1:
            if (cpu->V[Val1] == (uint8_t)Val2) // 3xkk
                cpu->PC += 2; // Skip the next instruction if Vx == kk
            break;

        case 2:
            if (cpu->V[Val1] != (uint8_t)Val2) // 4xkk
                cpu->PC += 2; // Skip the next instruction if Vx != kk
            break;

        case 3:
            if (cpu->V[Val1] == cpu->V[Val2]) // 5xy0
                cpu->PC += 2; // Skip the next instruction if Vx == Vy
            break;

        case 4:
            if (cpu->V[Val1] != cpu->V[Val2]) // 9xy0
                cpu->PC += 2; // Skip the next instruction if Vx != Vy
            break;

        default:
            instruction_0000_Ingored();
            break;
    }
}

void instruction_6xkk(cpu_t* cpu, uint16_t Vx, uint16_t Vkk)
{
    // Load the value kk into register Vx if Vx is less than 15
    if (Vx <= 15)
        cpu->V[Vx] = (uint8_t)Vkk;
}

void instruction_7xkk(cpu_t* cpu, uint16_t Vx, uint16_t Vkk)
{
    // Add the value kk to register Vx if Vx is less than 15
    if (Vx <= 15)
        cpu->V[Vx] += (uint8_t)Vkk;
}


void instruction_8xy03(cpu_t* cpu, uint16_t Vx, uint16_t Vy, int sign)
{
    switch(sign)
    {
        case 0: // 8xy0
            cpu->V[Vx] = cpu->V[Vy]; // Load the value of Vy into Vx
            break;
        case 1: // 8xy1
            cpu->V[Vx] |= cpu->V[Vy]; // Perform a bitwise OR operation on Vx and Vy and store the result in Vx
            cpu->V[15] = 0;
            break;
        case 2: // 8xy2
            cpu->V[Vx] &= cpu->V[Vy]; // Perform a bitwise AND operation on Vx and Vy and store the result in Vx
            cpu->V[15] = 0;
            break;
        case 3: // 8xy3
            cpu->V[Vx] ^= cpu->V[Vy]; // Perform a bitwise XOR operation on Vx and Vy and store the result in Vx
            cpu->V[15] = 0;
            break;
        default:
            instruction_0000_Ingored();
            break;
    }
}

void instruction_8xy4E(cpu_t* cpu, uint16_t Vx,uint16_t Vy, int sign)
{
    switch(sign)
    {
        case 4: // 8xy4
        {
            uint8_t sum = (cpu->V[Vx] + cpu->V[Vy]);
            cpu->V[15] = sum > 0xFF ? 1 : 0; // Set the carry flag to 1 if the sum is greater
            // than 255 unless set it to 0
            cpu->V[Vx] += cpu->V[Vy]; // Perform the addition and store the result in Vx
            break;
        }
        case 5: // 8xy5
            cpu->V[15] = (cpu->V[Vx] >= cpu->V[Vy]) ? 1 : 0;
            // Perform the subtraction and store the result in Vx
            cpu->V[Vx] -= cpu->V[Vy];
            break;

        case 6: // 8xy6
            if (Vy <= 15)
                cpu->V[Vx] = cpu->V[Vy];

            cpu->V[15] = cpu->V[Vx] & 1;
            cpu->V[Vx] >>= 1;
            break;

        case 7: // 8xy7
            cpu->V[15] = (cpu->V[Vy] >= cpu->V[Vx]) ? 1 : 0;
            // Perform the subtraction and store the result in Vx
            cpu->V[Vx] = (cpu->V[Vy] - cpu->V[Vx]);
            break;

        case 14: // 8xyE
            if (Vy <= 15)
                cpu->V[Vx] = cpu->V[Vy];

            // Set the carry flag to the value of the most significant bit of Vx
            cpu->V[15] = ((cpu->V[Vx] & 0x80) >> 7) == 1 ? 1 : 0;
            cpu->V[Vx] <<= 1; // Shift Vx left by 1 to double its value
            break;

        default:
            instruction_0000_Ingored();
            break;
    }
}

void instruction_Annn(cpu_t* cpu, uint16_t Vnnn)
{
    cpu->register_I = Vnnn;
}

void instruction_Bnnn(cpu_t* cpu, uint16_t Vnnn)
{
    uint16_t location  = Vnnn + (uint16_t)cpu->V[0];
    cpu->PC = location;
}

void instruction_Cxkk(cpu_t* cpu,uint16_t Vx, uint16_t Vkk)
{
    // Generate a random number between 0 and 255
    uint8_t randomNum = rand() % 256;

    // Set Vx to the result of a bitwise AND operation on the random number and kk
    cpu->V[Vx] = randomNum & Vkk;
}

void instruction_Dxyn(cpu_t* cpu, structRam* ram, uint16_t Vx, uint16_t Vy, uint16_t Vn, struct Display* display)
{
    uint16_t X = cpu->V[Vx]; // Extract the value of Vx
    uint16_t Y = cpu->V[Vy]; // Extract the value of Vy

    struct Sprite* sprite = (struct Sprite*)malloc(sizeof(struct Sprite));
    Sprite_init(sprite, Vn);

    for(uint16_t i = 0; i < Vn; i++)
    {
        if (cpu->register_I + i > 4095)
            return;
        else
        {
            Sprite_add(sprite, ram->content[cpu->register_I + i]);
        }
    }

    Display_DRW(display, sprite, (uint8_t)X, (uint8_t)Y, &cpu->V[15]);
    Display_update(display);
    Sprite_destroy(sprite);
}

void instruction_Ex9E(cpu_t* cpu, struct Keyboard* keyboard , uint16_t Vx)
{
    if (Vx >= 0 && Vx <= 15)
    {
        // Skip the next instruction if the key with the value of Vx is pressed
        if (Keyboard_get(keyboard, cpu->V[Vx]) == KEY_DOWN)
            cpu->PC += 2;
    }
}

void instruction_ExA1(cpu_t* cpu, struct Keyboard* keyboard , uint16_t Vx)
{
    if (Vx >= 0 && Vx <= 15)
    {
        // Skip the next instruction if the key with the value of Vx is not pressed
        if (Keyboard_get(keyboard, cpu->V[Vx]) == KEY_UP)
            cpu->PC += 2;
    }
}

void instruction_Fx07(cpu_t* cpu, uint16_t Vx)
{
    cpu->V[Vx] = cpu->DT;
}

void instruction_Fx0A(cpu_t* cpu, struct Keyboard* keyboard , uint16_t Vx)
{
    // Wait for a key press and store the result in Vx
    Keyboard_wait(keyboard, &cpu->V[Vx]);
}
void instruction_Fx15(cpu_t* cpu, uint16_t Vx)
{
    cpu->DT = cpu->V[Vx];
}

void instruction_Fx18(cpu_t* cpu, uint16_t Vx)
{
    cpu->ST = cpu->V[Vx];
}

void instruction_Fx1E(cpu_t* cpu, uint16_t Vx)
{
    cpu->register_I += cpu->V[Vx];
}

void instruction_Fx29(cpu_t* cpu, uint16_t Vx)
{
    uint16_t start_address = 0x600;
    if (Vx >= 0 && Vx <= 15)
        cpu->register_I = start_address + Vx * 5;
}

void instruction_Fx33(cpu_t* cpu, structRam* ram , uint16_t Vx)
{
    // Store the binary-coded decimal (BCD) representation of Vx at the addresses I, I+1, and I+2
    uint8_t value = cpu->V[Vx]; // Extract the value of Vx that will be converted to BCD
    ram->content[cpu->register_I] = value / 100; // Hundreds
    ram->content[cpu->register_I + 1] = (value / 10) % 10; // Tens
    ram->content[cpu->register_I + 2] = value % 10; // Ones
}

void instruction_Fx55(cpu_t* cpu, structRam* ram, uint16_t Vx)
{
    for (int i = 0; i <= Vx; i++)
    {
        // Store the values of registers V0 to Vx inclusive in memory starting at address I
        write_to_RAM(ram->content[cpu->register_I + i],ram, cpu->V[i]);
    }
    // The value of I is set to I + X + 1 after the operation
    cpu->register_I += 1;
}

void instruction_Fx65(cpu_t* cpu,structRam* ram, uint16_t Vx)
{
    for (int i = 0; i <= Vx; i++)
    {
        // Load the values of registers V0 to Vx inclusive from memory starting at address I
        read_from_RAM((uint8_t)cpu->register_I + i,ram, &(cpu->V[i]));
    }
    // The value of I is set to I + X + 1 after the operation
    cpu->register_I += 1;

}

