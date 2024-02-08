//
// Created by Billy on 27/01/2024.
//

#include <time.h>
#include "../headers/emulator.h"


int initEmulator(emulator* theEmulator)
{
    theEmulator->memory = (structRam*)malloc(sizeof(structRam));

    init_RAM(theEmulator->memory);

    if(theEmulator->memory == NULL)
    {
        fprintf(stderr, "error in the ram 's initialization \n");
        destroy_RAM(&theEmulator->memory); // Free RAM and its internal content
    }

    // load sprites in the memory
    loadSpriteInRam(theEmulator->memory);

    theEmulator->processor = (cpu_t*)malloc(sizeof(cpu_t));

    initializeCpu(theEmulator->processor);

    if(theEmulator->processor == NULL)
    {
        fprintf(stderr, "error in the processor 's initialization \n");
        destroyCpu(&theEmulator->processor); // Free processor and its internal content
    }

    // initialization of the screen's display

    theEmulator->theDisplay = (struct Display*)malloc(sizeof(struct Display));

    Display_init(theEmulator->theDisplay, 10);

    // initialization of the keyboard's structure

    theEmulator->KeyBoard = (struct Keyboard*)malloc(sizeof(struct Keyboard));

    Keyboard_init(theEmulator->KeyBoard);

    // initialization of the speaker's structure

    theEmulator->Speaker = (struct Speaker*)malloc(sizeof(struct Speaker));

    Speaker_init(theEmulator->Speaker);

    return 0;
}

void loadSpriteInRam(structRam* theRam) {
    uint8_t key_characters[16][5] = {{0x00F0, 0x0090, 0x0090, 0x0090, 0x00F0}, // 0
                                     {0x0020, 0x0060, 0x0020, 0x0020, 0x0070}, // 1
                                     {0x00F0, 0x0010, 0x00F0, 0x0080, 0x00F0}, // 2
                                     {0x00F0, 0x0010, 0x00F0, 0x0010, 0x00F0}, // 3
                                     {0x0090, 0x0090, 0x00F0, 0x0010, 0x0010}, // 4
                                     {0x00F0, 0x0080, 0x00F0, 0x0010, 0x00F0}, // 5
                                     {0x00F0, 0x0080, 0x00F0, 0x0090, 0x00F0}, // 6
                                     {0x00F0, 0x0010, 0x0020, 0x0040, 0x0040}, // 7
                                     {0x00F0, 0x0090, 0x00F0, 0x0090, 0x00F0}, // 8
                                     {0x00F0, 0x0090, 0x00F0, 0x0010, 0x00F0}, // 9
                                     {0x00F0, 0x0090, 0x00F0, 0x0090, 0x0090}, // A
                                     {0x00E0, 0x0090, 0x00E0, 0x0090, 0x00E0}, // B
                                     {0x00F0, 0x0080, 0x0080, 0x0080, 0x00F0}, // C
                                     {0x00E0, 0x0090, 0x0090, 0x0090, 0x00E0}, //D
                                     {0x00F0, 0x0080, 0x00F0, 0x0080, 0x00F0}, // E
                                     {0x00F0, 0x0080, 0x00F0, 0x0080, 0x0080}}; // F

    // Choose the starting point in RAM, for example, 0x600
    int start_address = 0x600;  // Starting at 1536 in decimal

    for (int i = 0; i < 16; i++)
    {  // For each character sprite
        for (int j = 0; j < 5; j++)
        {  // For each line of the sprite
            theRam->content[start_address + i * 5 + j] = key_characters[i][j];
        }
    }
}

void destroyEmulator(emulator* theEmulator)
{
    if (theEmulator)
    {
        // destruction of the cpu's structure
        destroyCpu(&theEmulator->processor); // Free CPU and its internal structures
        if (theEmulator->processor != NULL)
        {
            fprintf(stderr, "error in the cpu 's destruction \n");
        }
        // destruction of the ram's structure
        destroy_RAM(&theEmulator->memory); // Free RAM and its internal content

        if (theEmulator->memory != NULL)
        {
            fprintf(stderr, "error in the cpu 's destruction \n");
        }
        if (theEmulator->memory != NULL)
        {
            fprintf(stderr, "error in the ram 's destruction \n");
        }
        // destruction of the display's structure
        Display_destroy(theEmulator->theDisplay);
        free(theEmulator->theDisplay);

        // destruction of the keyBoard's structure
        Keyboard_destroy(theEmulator->KeyBoard);
        free(theEmulator->KeyBoard);

        // destruction of the keyBoard's structure
        Speaker_destroy(theEmulator->Speaker);

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
    long romSize = ftell(rom); //get the rom's size
    rewind(rom); // Reset the file position indicator to the beginning of the file

    // Copy the ROM into the buffer
    size_t bytesRead = fread(theEmulator->memory->content + START_ADRESS,
                             sizeof(uint8_t), romSize, rom);

    if (bytesRead != romSize)
    {
        fprintf(stderr, "Failed to read ROM file\n");
        fclose(rom);
        return 0; // Indicate failure
    }
    //print_ram_content(theEmulator->memory, bytesRead);
    fclose(rom);

    return 1; // Indicate success
}


bool runEmulator(emulator* theEmulator)
{
    srand(time(0));
    // we create an event to handle the keyboard's input
    SDL_Event event;
    // we create a boolean to check if the emulator is running or still running
    bool isRunning = false;

    while (!isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            // if the key pressed is SDL_QUIT, then the emulator stops, and we quit the program
            if (event.type == SDL_QUIT)
            {
                destroyEmulator(theEmulator);
                isRunning = true;
                break;
            }
        }

        if (isRunning == true)
            break;
        // here, we interpret the opcode, and execute it and update the emulator's state
        // (display, keyboard, speaker 's variable)
        interpreteOpCode(theEmulator->processor, theEmulator->memory,
                         theEmulator->theDisplay, theEmulator->KeyBoard);


        // Handle sound timer
        if (theEmulator->processor->ST > 0)
        {
            --theEmulator->processor->ST;
        }

        //update the speaker of the emulator

        if (theEmulator->processor->ST > 0)
            Speaker_on(theEmulator->Speaker);
        else
            Speaker_off(theEmulator->Speaker);


        // we are using the SDL_Delay function to slow down the execution of the emulator
        SDL_Delay(theEmulator->processor->DT);
    }

    return isRunning;
}



