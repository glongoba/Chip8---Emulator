#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "chip8.h"

bool init_graphics(SDL_Window **,SDL_Renderer **);
void render_graphics(Chip8 *,SDL_Renderer *);
void handle_input(Chip8 *,bool *);

#endif