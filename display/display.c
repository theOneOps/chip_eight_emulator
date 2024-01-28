//
// Created by Billy on 27/01/2024.
//

#include "display.h"


int Display_init(Chip8Display *display) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create SDL window
    display->window = SDL_CreateWindow(
            "Chip8 Emulator",                   // Window title
            SDL_WINDOWPOS_CENTERED,            // Initial x position
            SDL_WINDOWPOS_CENTERED,            // Initial y position
            WIDTH,                              // Width, in pixels
            HEIGHT,                             // Height, in pixels
            SDL_WINDOW_OPENGL                  // Flags - see below
    );

    // Check that the window was successfully created
    if (display->window == NULL) {
        // In the case that the window could not be made...
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);

    // Check that the renderer was successfully created
    if (display->renderer == NULL) {
        // In the case that the renderer could not be made...
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Set size variables
    display->width = WIDTH;
    display->height = HEIGHT;

    return 0;

}

// Destroy window and renderer

void Display_destroy(Chip8Display *display) {
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
}

// Clear the window to black

void Display_clear(Chip8Display *display)
{

    // Set draw color to black for "off" pixels
    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(display->renderer);

    // Additionally, clear the pixels array to reflect the cleared display
    for (int y = 0; y < 32; ++y)
    {
        for (int x = 0; x < 64; ++x)
        {
            display->pixels[x][y] = 0; // Set each pixel to 0 (off)
        }
    }

    SDL_RenderPresent(display->renderer);
}

// Update the window

void Display_update(Chip8Display* display)
{
    // Set draw color to black for "off" pixels
    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(display->renderer);

    // Set draw color to white for "on" pixels
    SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);

    for (int y = 0; y < CHIP_HEIGHT; y++)
    {
        for (int x = 0; x < CHIP_WIDTH; x++)
        {
            if (display->pixels[x][y] == 1)
            {
                // Draw white pixel
                SDL_Rect pixelRect =
                        {
                                x*PIXEL_SCALE,
                                y*PIXEL_SCALE,
                                PIXEL_SCALE,
                                PIXEL_SCALE
                        };
                SDL_RenderFillRect(display->renderer, &pixelRect);
            }
        }
    }
    // Update the screen
    SDL_RenderPresent(display->renderer);
}

void DrawSprite(Chip8Display *display,uint8_t* lastV , uint8_t x, uint8_t y, uint8_t *sprite, uint8_t height)
{
    *lastV = 0; // Reset VF before drawing

    for (int row = 0; row < height; row++)
    {
        uint8_t spriteRow = sprite[row];

        for (int col = 0; col < 8; col++)
        {
            // Check if the sprite pixel is set
            if (spriteRow & (0x80 >> col))
            {
                // Calculate the actual x and y position in the frame buffer
                int xPos = (x  + col) % CHIP_WIDTH;
                int yPos = (y + row) % CHIP_HEIGHT;

                if (display->pixels[xPos][yPos] == 0)
                {
                    *lastV = 1; // Set VF to indicate collision
                }
                // XOR sprite pixel with the frame buffer and check for collision
                display->pixels[xPos][yPos] ^= 1;
            }
        }
    }
}
