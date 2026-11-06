#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "chip8.h"

bool init_graphics(SDL_Window **win,SDL_Renderer **renderer){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr,"\n Errore di inizializzazione grafica. \n");
        return false;
    }
    *win=SDL_CreateWindow(" CHIP-8 EMULATOR ", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,320,SDL_WINDOW_SHOWN);
    if(*win == NULL){
        fprintf(stderr,"\n Errore di inizializzazione grafica. \n");
        return false;
    }
    *renderer= SDL_CreateRenderer(*win,-1,SDL_RENDERER_ACCELERATED);
    if(*renderer == NULL){
        fprintf(stderr,"\n Errore di inizializzazione grafica. \n");
        SDL_DestroyWindow(*win);
        return false;
    }
    return true;
}

void render_graphics(Chip8 *chip8,SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,0,255,255);
    for(uint8_t y=0;y<32;y++){
        for(uint8_t x=0;x<64;x++){
            if(chip8->display[(y*64)+x] != 0){
                SDL_Rect rettangolo;
                rettangolo.x=(10*x);
                rettangolo.y=(10*y);
                rettangolo.w=10;
                rettangolo.h=10;
                SDL_RenderFillRect(renderer,&rettangolo);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void handle_input(Chip8 *chip8,bool *running){
    uint8_t value;
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT)
        *running=false;
        if(event.type == SDL_KEYDOWN || event.type== SDL_KEYUP){
        if(event.type == SDL_KEYDOWN)
            value=1;
        else
            value=0;
        switch(event.key.keysym.sym){
            case SDLK_1: chip8->keypad[1]=value; break;
            case SDLK_2: chip8->keypad[2]=value; break;
            case SDLK_3: chip8->keypad[3]=value; break;
            case SDLK_4: chip8->keypad[0xC]=value; break;
            case SDLK_q: chip8->keypad[4]=value; break;
            case SDLK_w: chip8->keypad[5]=value; break;
            case SDLK_e: chip8->keypad[6]=value; break;
            case SDLK_r: chip8->keypad[0xD]=value; break;
            case SDLK_a: chip8->keypad[7]=value; break;
            case SDLK_s: chip8->keypad[8]=value; break;
            case SDLK_d: chip8->keypad[9]=value; break;
            case SDLK_f: chip8->keypad[0xE]=value;  break;
            case SDLK_z: chip8->keypad[0xA]=value; break;
            case SDLK_x: chip8->keypad[0]=value; break;
            case SDLK_c: chip8->keypad[0xB]=value; break;
            case SDLK_v: chip8->keypad[0xF]=value; break;
        }    
        }
    }
    }
