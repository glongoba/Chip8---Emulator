#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"
int main(){
    srand(time(NULL));
    Chip8 chip8;
    chip8_init(&chip8);
    load_rom(&chip8,"Tapeworm.ch8");
    SDL_Window* win;
    SDL_Renderer* renderer;
    init_graphics(&win,&renderer);
    bool running=true;
    while(running){
        uint32_t start_time=SDL_GetTicks();
        handle_input(&chip8,&running);
        for(uint8_t i=0;i<8;i++){
            chip8_decode_execute(&chip8);
        }
        
        uint32_t frame_time=SDL_GetTicks()-start_time;
        if((frame_time)<16){
            SDL_Delay((16-frame_time));
        }
        if(chip8.delayTimer>0) 
        chip8.delayTimer--;
        if(chip8.soundTimer>0)
        chip8.soundTimer--;
        render_graphics(&chip8,renderer);
    }
    return 0;
}