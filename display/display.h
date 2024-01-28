//
// Created by Billy on 27/01/2024.
//

#ifndef CHIP_EIGHT_DISPLAY_H
#define CHIP_EIGHT_DISPLAY_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define WIDTH 640
#define HEIGHT 320
#define PIXEL_SCALE 5
#define CHIP_WIDTH 64
#define CHIP_HEIGHT 32


// Define a structure to hold display-related information
typedef struct {
    SDL_Window *window;     // Pointer to the SDL_Window
    SDL_Renderer *renderer; // Pointer to the SDL_Renderer
    int width;              // Width of the window
    int height;             // Height of the window
    int pixels[CHIP_WIDTH][CHIP_HEIGHT];      // the 2D array representing the screen's display
} Chip8Display;


// Function prototypes

// Create a window

int Display_init(Chip8Display *display);

// Destroy window and renderer

void Display_destroy(Chip8Display *display);

// Clear the window to black

void Display_clear(Chip8Display *display);

// Update the window

void Display_update(Chip8Display *display);

// Draw a pixel

void Display_drawPixel(Chip8Display *display, int x, int y);

// Draw a sprite

void DrawSprite(Chip8Display *display,uint8_t* lastV , uint8_t x, uint8_t y, uint8_t *sprite, uint8_t height);

#endif //CHIP_EIGHT_DISPLAY_H
