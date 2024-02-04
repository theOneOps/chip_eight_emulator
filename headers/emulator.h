//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_EMULATOR_H
#define CHIP_EIGHT_EMULATOR_H

#include "ram.h"
#include "cpu.h"
#include "../libprovided/include/display/display.h"
#include "../libprovided/include/keyboard/keyboard.h"
#include "../libprovided/include/speaker/speaker.h"

/**
 * @brief This structure is the main structure of the emulator.
 * It contains the ram, the cpu and the display.
 * @param memory is a pointer to the ram structure
 * @param processor is a pointer to the cpu structure
 * @param theDisplay is a pointer to the display structure
 * @param theKeyboard is a pointer to the keyboard structure
 */

typedef struct
{
    structRam* memory;
    cpu_t* processor;
    struct Display* theDisplay;
    struct Keyboard* KeyBoard;
    struct Speaker* Speaker;
}emulator;

/**
 * @brief This function initializes the emulator's structure.
 * @param theEmulator is a pointer to the emulator's structure
 * @param emulator_size is the size of the emulator's ram
 * @return 0 if the initialization is successful, -1 otherwise
 */
int initEmulator(emulator* theEmulator);

/**
 * @brief This function destroys the emulator's structure.
 * @param theEmulator is a pointer to the emulator's structure
 */
void destroyEmulator(emulator*);

/**
 * @brief This function reads the program from the file and
 * stores it in the ram.
 * @param rom is the path to the file containing the program
 * @param theEmulator is a pointer to the emulator's structure
 * @return 0 if the reading is successful, -1 otherwise
 */
int read_program(const char* rom, emulator*);

/**
 * @brief This function runs the emulator.
 * @param theEmulator is a pointer to the emulator's structure
 * @return true if the emulator is running, false otherwise
 */
bool runEmulator(emulator* theEmulator);

/**
 * @brief This function loads the sprite into the ram.
 * @param theRam is a pointer to the ram's structure
 */
void loadSpriteInRam(structRam* theRam);

#endif //CHIP_EIGHT_EMULATOR_H
