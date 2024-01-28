//
// Created by Billy on 27/01/2024.
//


#include "../headers/cpu.h"

void initializeStack(StackP* sp)
{
    sp->top = -1;
}

bool isFull(StackP* sp)
{
    return sp->top == MAX_STACK_SIZE - 1;
}

bool isEmpty(StackP* sp)
{
    return sp->top == (uint16_t)-1;
}

void push(StackP *sp, uint16_t value)
{
    if (isFull(sp)) {
        printf("Erreur : the stack is full \n");
        return;
    }
    sp->items[++sp->top] = value;
}

int pop(StackP *sp)
{
    if (isEmpty(sp))
    {
        perror("the stack is empty, can't pop it \n");
    }

    uint16_t value = sp->items[sp->top];
    sp->items[sp->top] = -1;
    sp->top--;

    return value;
}

// Initialization of the cpu
void initializeCpu(cpu_t* theCPU)
{
    // Initialize V registers
    theCPU->V = (uint8_t*)calloc(16, sizeof(uint8_t)); // CHIP-8 has 16 8-bit
    // registers (V0 to VF)

    // Initialize the stack
    theCPU->SP = (StackP*)malloc(sizeof(StackP)); // Allocate memory for the stack
    if (theCPU->SP != NULL) {
        initializeStack(theCPU->SP); // Initialize the stack if allocation was successful
    }

    // Initialize other components
    theCPU->register_I = (uint16_t*)malloc(sizeof(uint16_t)); // Allocate memory for the I register
    if (theCPU->register_I != NULL) {
        *(theCPU->register_I) = 0; // Initialize the I register
    }

    theCPU->PC = (uint16_t*)malloc(sizeof(uint16_t)); // Allocate memory for the program counter
    if (theCPU->PC != NULL) {
        *(theCPU->PC) = 0x200; // Initialize the PC. Typically, CHIP-8 programs start at 0x200
    }

    theCPU->ST = (uint8_t*)malloc(sizeof(uint8_t)); // Allocate memory for the sound timer
    if (theCPU->ST != NULL) {
        *(theCPU->ST) = 0; // Initialize the sound timer
    }

    theCPU->DT = (uint8_t*)malloc(sizeof(uint8_t)); // Allocate memory for the delay timer
    if (theCPU->DT != NULL) {
        *(theCPU->DT) = 0; // Initialize the delay timer
    }
}


// Destruction of the cpu
void destroyCpu(cpu_t* theCPU)
{
    free(theCPU->V);
    theCPU = NULL;
}

// static only presented in the file


uint16_t read_instruction(cpu_t* theCPU, structRam* theRam) {
    // Fetch the high byte and shift it left by 8 bits
    uint16_t high_byte = theRam->content[*theCPU->PC] << 8;

    // Fetch the low byte
    uint16_t low_byte = theRam->content[(*theCPU->PC) + 1];

    // Combine the high and low bytes to form the instruction
    uint16_t instruction = high_byte | low_byte;

    //printf("The instruction: %04x \n", instruction);

    return instruction;
}

void interpreteOpCode(cpu_t* theCPU, structRam* theRam,
                      Chip8Display* display)
{
    uint16_t instruction = read_instruction(theCPU, theRam);
    uint16_t fixedPart = instruction & 0xF000;

    switch(fixedPart)
    {
        case 0x0000:
        {
            uint16_t nnn = instruction & 0x0FFF;
            switch(nnn)
            {
                case 0x00E0: // CLS display
                {
                    printf("clear the screen \n");
                    Display_clear(display);
                    break;
                }
                case 0x00EE: // 00EE
                {
                    *theCPU->PC = theCPU->SP->items[theCPU->SP->top];
                    pop(theCPU->SP);
                    break;
                }

                default: // SYS addr - This instruction is generally ignored in modern interpreters
                    printf("SYS call to address 0x%03X ignored\n", nnn);
                    break;
            }
            break;
        }

        case 0x1000:
        {
            uint16_t nnn = instruction & 0x0FFF;
            *theCPU->PC = nnn; // jump to nnn
            break;
        }

        case 0x2000:
        {
            push(theCPU->SP, *theCPU->PC);
            uint16_t nnn = instruction & 0x0FFF;
            *theCPU->PC = nnn;
            break;
        }

        case 0x3000:
        {
            uint16_t kk = instruction & 0x00FF;
            uint16_t x = instruction & 0x0F00;
            SNEE(true, theCPU, (uint16_t*)&theCPU->V[x >> 8], &kk);
            break;
        }

        case 0x4000:
        {
            uint16_t kk = instruction & 0x00FF;
            uint16_t x = instruction & 0x0F00;
            SNEE(false, theCPU, (uint16_t*)&theCPU->V[x >> 8], &kk);
            break;
        }

        case 0x5000:
        {
            uint16_t y = instruction & 0x00F0;
            uint16_t x = instruction & 0x0F00;
            SNEE(true, theCPU, (uint16_t*)&theCPU->V[x >> 8], (uint16_t*)&theCPU->V[y >> 4]);
            break;
        }
        case 0x6000:
        {
            printf(" Load normal register with immediate value\n");
            uint16_t kk = instruction & 0x00FF;
            uint16_t x = (instruction & 0x0F00) >> 8;
            theCPU->V[x] = kk;
        }
        case 0x7000:
        {
            uint16_t kk = instruction & 0x00FF;
            uint16_t x = (instruction & 0x0F00) >> 8;
            theCPU->V[x] += kk;
            break;
        }

        case 0x8000:
        {
            uint16_t newFixedPart = instruction & 0x000F;
            switch(newFixedPart)
            {
                case 0x0000: //0x8000
                {
                    uint16_t y = instruction & 0x00F0;
                    uint16_t x = instruction & 0x0F00;
                    theCPU->V[x >> 8] = theCPU->V[y >> 4];
                    break;
                }
                case 0x0001: //0x8001
                {
                    uint16_t y = instruction & 0x00F0;
                    uint16_t x = instruction & 0x0F00;
                    theCPU->V[x >> 8] |= theCPU->V[y >> 4];
                    break;
                }
                case 0x0002: //0x8002
                {
                    uint16_t y = instruction & 0x00F0;
                    uint16_t x = instruction & 0x0F00;
                    theCPU->V[x >> 8] &= theCPU->V[y >> 4];
                    break;
                }
                case 0x0003: //0x8003
                {
                    uint16_t y = instruction & 0x00F0;
                    uint16_t x = instruction & 0x0F00;
                    theCPU->V[x >> 8] ^= theCPU->V[y >> 4];
                    break;
                }
                case 0x0004: //0x8004
                {
                    uint16_t x = (instruction & 0x0F00) >> 8;
                    uint16_t y = (instruction & 0x00F0) >> 4;

                    uint16_t sum = theCPU->V[x] + theCPU->V[y];

                    theCPU->V[15] = sum > 0xFF ? 1 : 0;
                    theCPU->V[x] = sum & 0xFF; // Store only the lowest 8 bits back in Vx
                    break;
                }
                case 0x0005: //0x8005
                {
                    uint16_t x = (instruction & 0x0F00) >> 8;
                    uint16_t y = (instruction & 0x00F0) >> 4;
                    // Set VF to 1 if there's no borrow (Vx >= Vy), else to 0
                    theCPU->V[15] = theCPU->V[x] >= theCPU->V[y] ? 1 : 0;

                    // Perform the subtraction and store the result in Vx
                    theCPU->V[x] -= theCPU->V[y];
                    break;
                }
                case 0x0006: //0x8006
                {
                    uint16_t x = (instruction & 0x0F00) >> 8;
                    theCPU->V[15] = ((theCPU->V[x] & 0x01) == 1) ? 1 : 0;
                    theCPU->V[x] >>= 1;
                    break;
                }

                case 0x0007: //0x8007
                {
                    uint16_t x = (instruction & 0x0F00) >> 8;
                    uint16_t y = (instruction & 0x00F0) >> 4;
                    // Set VF to 1 if there's no borrow (Vx >= Vy), else to 0
                    theCPU->V[15] = theCPU->V[y] >= theCPU->V[x] ? 1 : 0;
                    // Perform the subtraction and store the result in Vx
                    theCPU->V[x] = theCPU->V[y] - theCPU->V[x];
                    break;
                }

                case 0x000E: //0x800E ????????
                {
                    uint16_t x = (instruction & 0x0F00) >> 8;
                    theCPU->V[15] = ((theCPU->V[x] & 0x80) >> 7) == 1 ? 1 : 0;
                    theCPU->V[x] <<= 1;
                    break;
                }
            }

        }

        case 0x9000:
        {
            uint16_t x = (instruction & 0x0F00) >> 8;
            uint16_t y = (instruction & 0x00F0) >> 4;

            if (theCPU->V[x] != theCPU->V[y])
                *theCPU->PC += 2;
            break;
        }
        case 0xA000:
        {
            printf("Load index register with immediate value \n");
            uint16_t nnn = instruction & 0x0FFF;
            *theCPU->register_I = nnn;
            break;
        }
        case 0xB000:
        {
            uint16_t location  = (instruction & 0x0FFF) + (uint16_t)theCPU->V[0];
            *theCPU->PC = location;
            break;
        }
        case 0xC000:
        {
            uint16_t x = (instruction & 0x0F00) >> 8; // Extract x from the instruction
            uint8_t kk = instruction & 0x00FF;        // Extract kk from the instruction

            // Generate a random number between 0 and 255
            uint8_t randomNum = rand() % 256;

            // Set Vx to the result of a bitwise AND operation on the random number and kk
            theCPU->V[x] = randomNum & kk;
            break;
        }
        case 0xD000: // draw a sprite
        {
            printf("draw a sprite \n");
            uint16_t x = theCPU->V[(instruction & 0x0F00) >> 8]; // Extract the value of Vx
            uint16_t y = theCPU->V[(instruction & 0x00F0) >> 4]; // Extract the value of Vy
            uint16_t n = instruction & 0x000F; // Height of the sprite

            uint8_t tab[n]; // Maximum height of a Chip-8 sprite is 15
            for(unsigned int i = 0; i < n; i++)
            {
                tab[i] = theRam->content[*(theCPU->register_I) + i];
            }

            DrawSprite(display, &theCPU->V[15], x, y, tab, n);
            Display_update(display);
            break;
        }

        case 0xE000:
        {
            switch (instruction & 0x00FF)
            {
                case 0x009E: // keyboard
                {
                    break;
                }
                case 0x00A1: // keyboard
                {
                    break;
                }
            }
            break;
        }
        case 0xF000:
        {
            switch(instruction & 0x00FF)
            {
                case 0x000A: // keyboard
                {

                    break;
                }

                case 0x0015: // delay timer
                {
                    break;
                }

                case 0x0018: // sound timer
                {
                    break;
                }

                case 0x001E: // delay timer
                {
                    break;
                }

                case 0x0029: // sprite concerned
                {
                    break;
                }

                case 0x0033: // register I
                {
                    break;
                }

                case 0x0055:
                {
                    uint16_t x = (instruction & 0x0F00) >> 8;
                    write_to_RAM_from_ADR(theCPU->V, &(theCPU->V[x]), (uint8_t*)theCPU->register_I, theRam);
                    break;
                }

                case 0x0065: // same as the previous instruction, but in read mode
                {
                    break;
                }
            }
            break;
        }
    }

    *theCPU->PC += 2;
}

void SNEE(bool isEqual, cpu_t* theCPU, const uint16_t* v1, const uint16_t* v2)
{
    if ((isEqual && (*v1 == *v2)) || (!isEqual && (*v1 != *v2)))
        *(theCPU->PC) += 2;
}
