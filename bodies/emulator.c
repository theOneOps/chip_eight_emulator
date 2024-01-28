//
// Created by Billy on 27/01/2024.
//

#include "../headers/emulator.h"
#include <time.h>


int initEmulator(emulator* theEmulator, unsigned int emulator_size)
{
    theEmulator->memory = (structRam*)malloc(sizeof(structRam));

    theEmulator->memory->content = (uint8_t*)malloc(sizeof(uint8_t) * emulator_size);
    theEmulator->memory->sizeRAM = emulator_size;

    printf("taille de la ram depuis initEmulator : %d \n", theEmulator->memory->sizeRAM);

    theEmulator->processor = (cpu_t*)malloc(sizeof(cpu_t));

    //theEmulator->processor->V[2] = 38;

    initializeCpu(theEmulator->processor);

    //theEmulator->processor->V[2] = 38;

    //theEmulator->mask = (masque_id*)malloc(sizeof(masque_id));

    // initlaization of the mask structure on which out decode's functions will rely on
    //initialize_masque_id(theEmulator->mask);

    // initialization of the screen's display

    theEmulator->theDisplay = (Chip8Display*)malloc(sizeof(Chip8Display));

    int ret = Display_init(theEmulator->theDisplay);

    return 0;
}

void destroyEmulator(emulator* theEmulator)
{
    if (theEmulator)
    {
        // destruction of the cpu's structure
        destroyCpu(theEmulator->processor); // Free CPU and its internal structures
        free(theEmulator->processor);

        // destruction of the ram's structure
        destroy_RAM(theEmulator->memory); // Free RAM and its internal content
        free(theEmulator->memory);

        // destruction of the display's structure
        Display_destroy(theEmulator->theDisplay);
        free(theEmulator->theDisplay);

        free(theEmulator);
    }
}

int read_program(const char* filePath, emulator* theEmulator)
{
    // Open the ROM file
    FILE *rom = fopen(filePath, "rb");
    if (rom == NULL)
    {
        fprintf(stderr, "Failed to open ROM file: %s\n", filePath);
        return 0; // Indicate failure
    }

    // Seek to the end of the file to determine its size
    fseek(rom, 0, SEEK_END);
    long romSize = ftell(rom);
    rewind(rom); // Reset the file position indicator to the beginning of the file

    // Allocate memory to store the ROM
    uint8_t *romBuffer = (unsigned char *)malloc(sizeof(uint8_t) * romSize);
    if (romBuffer == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for ROM\n");
        fclose(rom);
        return 0; // Indicate failure
    }

    // Copy the ROM into the buffer
    size_t bytesRead = fread(romBuffer, sizeof(uint8_t), romSize, rom);
    if (bytesRead != romSize)
    {
        fprintf(stderr, "Failed to read ROM file\n");
        free(romBuffer);
        fclose(rom);
        return 0; // Indicate failure
    }

    printf("taille ram : %d \n", theEmulator->memory->sizeRAM);

    /**
    for (int i = 0; i < romSize; i++)
    {
        printf("%02x \n", romBuffer[i]);
    }
    */

    // Copy the ROM from the buffer into the Chip-8 memory
    printf("fait fait avant \n");

    int CHIP8_PROGRAM_LOAD_ADDRESS = 512;
    for (int i = 0; i < romSize; i++)
    {
        theEmulator->memory->content[CHIP8_PROGRAM_LOAD_ADDRESS + i] = romBuffer[i];
    }
    printf("hello world \n");

    /**
    for (int i = 0; i < romSize; i++)
    {
        printf("%02x\n",theEmulator->memory->content[CHIP8_PROGRAM_LOAD_ADDRESS + i]);
    }
    */

    // printf("le counter : %02x \n", *theEmulator->processor->PC);
    // printf("le contenu du registre 2 est %d \n", theEmulator->processor->V[2]);
    // Clean up
    free(romBuffer);
    fclose(rom);

    return 1; // Indicate success
}


bool runEmulator(emulator* theEmulator)
{
    srand(time(0));
    SDL_Event event;
    bool isRunning = true;

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                break;
            }
            // Handle other SDL events (e.g., keyboard input) as needed
        }

        // Decode and Execute
        interpreteOpCode(theEmulator->processor, theEmulator->memory,
                         theEmulator->theDisplay);

        // Handle Timers
        if (*(theEmulator->processor->DT) > 0)
        {
            --(*(theEmulator->processor->DT));
        }

        if (*(theEmulator->processor->ST) > 0)
        {
            // Sound logic here
            --(*(theEmulator->processor->ST));
        }

        // Implement your timing logic here (e.g., delay to simulate 60Hz operation)
        SDL_Delay(1000 / 60); // Delay for approximately 1/60th of a second
    }

    destroyEmulator(theEmulator);

    return false;
}

