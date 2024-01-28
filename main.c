#include <stdio.h>
#include <SDL.h>
#include "headers/emulator.h"

int main(int argc, char* argv[])
{
    //int errcode;

    if (argc != 2)
    {
        fprintf(stderr, "Unexpected number of arguments.\n");
        fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
        return 1;
    }
    emulator* theEmulator = (emulator*)malloc(sizeof(emulator));
    printf("The SDL library has been initialized successfully.\n");
    if (!initEmulator(theEmulator, 4096))
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
        // destroyEmulator(theEmulator);
    }
    return 0;
}
