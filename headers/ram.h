//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_RAM_H
#define CHIP_EIGHT_RAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RAM_SIZE 4096

/**
 * @brief This structure is the main structure of the ram.
 * It contains the content of the ram and the size of the ram.
 * @param content is the content of the ram
 * @param sizeRAM is the size of the ram
 */
typedef struct
{
    uint8_t content[RAM_SIZE];
}structRam;

/**
 * @brief This function initializes the ram's structure.
 * @param theRAM is a pointer to the ram's structure
 * @param theSize is the size of the ram
 * @return 0 if the initialization is successful, -1 otherwise
 */
int init_RAM(structRam* ram);

/**
 * @brief This function destroys the ram's structure.
 * @param theRAM is a pointer to the ram's structure
 * @return 0 if the destruction is successful, -1 otherwise
 */
int destroy_RAM(structRam** ram);

/**
 * @brief This function writes a value to the ram.
 * @param addr is the address where the value will be written
 * @param theRAM is a pointer to the ram's structure
 * @param value_to_write is the value to write
 * @return 0 if the writing is successful, -1 otherwise
 */
int write_to_RAM(uint8_t addr, structRam* theRAM, uint8_t value_to_write);

/**
 * @brief This function reads a value from the ram.
    * @param addr is the address where the value will be read
    * @param theRAM is a pointer to the ram's structure
    * @param contentToChange is the pointer to the value to change
 * @return 0 if the reading is successful, -1 otherwise
 */
int read_from_RAM(uint8_t addr, structRam* theRAM, uint8_t* contentToChange); // V
//reading starting from an address

/**
 * @brief This function prints the content of the ram.
 * @param theRAM is a pointer to the ram's structure
 * @param length is the length of the content to print
 */
void print_ram_content(structRam* ram, size_t length);


#endif //CHIP_EIGHT_RAM_H
