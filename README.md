# Project : made a CHIP-8 emulator (still in progress)

# Description

This project is a CHIP-8 emulator written in C using the SDL2 library for graphics and input. The emulator is capable of running CHIP-8 programs and some games.

# Overview

I was supposed to implement a CHIP-8 emulator in C and make sure it valid the timmendus suite's test on chip8-test-roms repository.

## the tests (timmendus suite) `[x] is valid | [] is not valid`

- CHIP-8 splash screen[OK]
- IBM logo [OK]
- Corax+ opcode test [OK]
- Flags test[OK] but doesn't display the name of the flags like it should (carry, happy and so on...)
- Beep test [ok] can't hear the sound but the test is valid, you can control the bip
- Quirks test []
- Scrolling test []
# Installation

To compile the project, you will need to have the SDL2 library installed on your system

Then you will need to link a library to the program : `libprovided.a` (you can find it in the project folder)
