#include <stdio.h>
#include <SDL.h>
#include "headers/emulator.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Unexpected number of arguments.\n");
        fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
        return 1;
    }
    // we initialize the SDL library
    emulator* theEmulator = (emulator*)malloc(sizeof(emulator));
    if (theEmulator == NULL)
    {
        fprintf(stderr, "error in the emulator 's initialization \n");
        destroyEmulator(theEmulator); // Free processor and its internal content
    }
    if (!initEmulator(theEmulator))
    {
        printf("The CHIP 8 emulator is ready.\n");
        if (read_program(argv[1], theEmulator))
        {
            printf("The ROM <%s> has been loaded.\n",argv[1]);
            printf("Run...\n");
            while (
                    !runEmulator(theEmulator)
                    );
        }
    }
    return 0;
}