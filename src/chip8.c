#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "chip8.h"
#define SA 0x050 // START ADDRESS

// inizializzazione dell'array di caratteri, ogni 5 "elementi" (byte) abbiamo un carattere esadecimale
const uint8_t fontset[80] = {

    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F

};


// serve per fare si che all'interno della memoria RAM vengano "impressi i caratteri"
void chip8_init(Chip8 *chip8){
    srand(time(NULL));
    //memset -> inclusa con string.h e azzera completamente la struct
    memset(chip8,0,sizeof(Chip8));

    // a partire dall'indirizzo di memoria 0x050 che è quella da cui partire
    for(int i=0;i<80;i++){ //in alternativa al for si poteva utilizzare memcpy
        chip8->memory[SA+i]=fontset[i];
    }

    // si setta il programm counter a 200 (512 in decimale) -> come abbiamo detto il chip8
    // dedica all'avvio 512 byte all'interprete, quindi il pc viene settato al primo da cui si parte all'inizio
    // che è proprio 200
    chip8->pc=0x200;
    chip8->shift_flag=true;
}


// fetch : fase di prelievo dell'istruzione dalla memoria
// le istruzioni sono composte da due byte, per questo si prendono due istruzioni in maniera separata
// la prima subisce uno shift a sx di 8 posizioni così da avere 16 bit di cui gli ultimi 8 tutti zeri
// si fa l'OR a livello di bit fra le due parti, la prima avendo come ultimi 8 bit solo zeri, tutti 
// i valori saranno uguali alla seconda istruzione
// si incrementa il pc di 2
// OPCODE -> OPERAZIONE CHE DICE ALLA CPU COSA FARE
uint16_t chip8_fetch(Chip8 *chip8){
    
    uint8_t instruct1= chip8->memory[chip8->pc];
    uint8_t instruct2= chip8->memory[chip8->pc +1];
    uint16_t opcode;
    opcode=((uint16_t)instruct1) << 8;
    opcode = opcode | instruct2;
    chip8->pc +=2;
    return opcode;
}

void chip8_decode_execute(Chip8 *chip8){
    uint16_t opcode=chip8_fetch(chip8);
    uint8_t firstNibble= (opcode & 0xF000) >> 12;
    switch(firstNibble){
        case 0x0:
        {
            uint8_t condition= opcode & 0x00FF;
            if(condition == 0xE0){
                memset(chip8->display,0,sizeof(chip8->display));
                break;
            }
            if(condition == 0xEE){
                chip8->sp--;
                chip8->pc=chip8->Stack[chip8->sp];
                break;
            }
        }
        case 0x1:
        {
            uint16_t jump_value= opcode & 0x0FFF;
            chip8->pc=jump_value;
            break;
        }
        case 0x2:
        {
            chip8->Stack[chip8->sp]=chip8->pc;
            chip8->sp++;
            chip8->pc = opcode & 0x0FFF;
            break; 
        }
        case 0x3:
        {
            uint8_t check1= (opcode & 0x0F00) >> 8;
            uint8_t check2= opcode & 0x00FF;
            if(chip8->V[check1]==check2){
                chip8->pc+=2;
            }
            break;
        }
        case 0x4:
        {
            uint8_t check1= (opcode & 0x0F00) >> 8;
            uint8_t check2= opcode & 0x00FF;
            if(chip8->V[check1]!=check2){
                chip8->pc+=2;
            }
            break;
        }
        case 0x5:
        {   
            uint8_t x= (opcode & 0x0F00) >> 8;
            uint8_t y= (opcode & 0x00F0) >> 4;
            if(chip8->V[x]==chip8->V[y] && (opcode & 0x000F)==0x0)
                chip8->pc+=2;
            break;
        }
        case 0x6:
        {
            uint8_t x= (opcode & 0x0F00) >> 8;
            chip8->V[x]= (opcode & 0x00FF);
            break;
        }
        case 0x7:
        {
            uint8_t x= (opcode & 0x0F00) >> 8;
            chip8->V[x]= chip8->V[x] + (opcode & 0x00FF);
            break;
        }
        case 0x8:
        {
            uint8_t x= (opcode & 0x0F00) >> 8;
            uint8_t y= (opcode & 0x00F0) >> 4;
            uint8_t d= (opcode & 0x000F);
            switch(d){
                case 0x0:
                    chip8->V[x]=chip8->V[y];
                    break;
                case 0x1:
                    chip8->V[x]= (chip8->V[x] | chip8->V[y]);
                    break;
                case 0x2:
                    chip8->V[x]= (chip8->V[x] & chip8->V[y]);
                    break;
                case 0x3:
                    chip8->V[x]= (chip8->V[x] ^ chip8->V[y]);
                    break;
                case 0x4:
                {
                    uint16_t sum= (chip8->V[x] + chip8->V[y]);
                    if(sum > 0xFF)
                        chip8->V[0xF]=0x1;
                    else 
                        chip8->V[0xF]=0x0;
                    
                    chip8->V[x]=(uint8_t)sum;
                    break;
                }
                case 0x5:
                {   
                    if(chip8->V[x] >= chip8->V[y])
                        chip8->V[0xF]=0x1;
                    else chip8->V[0xF]=0x0;
                    chip8->V[x]= (chip8->V[x] - chip8->V[y]);
                    break;
                }
                case 0x6:
                {
                    if(chip8->shift_flag)
                        chip8->V[x]=chip8->V[y];
                    chip8->V[0xF]= (chip8->V[x] & 0x01);
                    chip8->V[x] >>= 1;
                    break;
                }
                case 0x7:
                {   
                    if(chip8->V[y] >= chip8->V[x])
                        chip8->V[0xF]=0x1;
                    else chip8->V[0xF]=0x0;
                    chip8->V[x]= (chip8->V[y] - chip8->V[x]);
                    break;
                }
                case 0xE:
                {
                    if(chip8->shift_flag)
                        chip8->V[x]=chip8->V[y];
                    chip8->V[0xF]=(chip8->V[x] & 0x80) >> 7;
                    chip8->V[x] <<= 1;
                    break;
                }
        }
        break;
    }
        case 0xA:
        {
            uint16_t set= opcode & 0x0FFF;
            chip8->I=set;
            break;
        }
        case 0xB:
        {   
            uint8_t x= (opcode & 0x0F00) >> 8;
            if(!chip8->shift_flag)
            chip8->pc= (chip8->V[x] + (opcode & ( 0x0FFF)));
            else chip8->pc= (chip8->V[0]+ (opcode & (0x0FFF)));
            break;
        }
        case 0x9:
        {
            uint8_t x= (opcode & 0x0F00) >> 8;
            uint8_t y= (opcode & 0x00F0) >> 4;
            if(chip8->V[x]!=chip8->V[y] && (opcode & 0x000F)==0x0)
                chip8->pc+=2;
            break;
        }
        case 0xC:
        {   
            uint8_t x= (opcode & 0x0F00) >> 8;
            uint8_t random=rand() % 256;
            chip8->V[x] = (random & (opcode & 0xFF));
            break;
        }
        case 0xD: // DXYN N pixel tall
        {   
            chip8->V[0xF]=0x0;
            uint8_t x= (opcode & 0x0F00) >> 8;
            uint8_t y= (opcode & 0x00F0) >> 4;
            uint8_t cordx= chip8->V[x];
            uint8_t cordy= chip8->V[y];
            uint8_t bit1;
            cordx=cordx%64;
            cordy=cordy%32;
            for(uint8_t i=0x0;i<(opcode & 0x000F);i++){
                uint8_t currenty = (cordy+i) % 32;
                for(uint8_t j=0x0;j<0x8;j++){
                    uint8_t currentx= (cordx+j) % 64;
                    bit1= ((chip8->memory[chip8->I +i] >> (7-j)) & 0x1);
                        if(bit1==1){
                            if(chip8->display[(currenty*64)+currentx]!=0x0){
                                chip8->V[0xF]=0x1;
                            }
                            chip8->display[(currenty*64)+currentx]^=1;
                        }
                    }
                }
               break; 
    }
    case 0xE:
    {
        if((opcode & 0x00FF) == 0x9E){
            uint8_t x= (opcode & 0x0F00) >>8;
            if(chip8->keypad[chip8->V[x]] == 0x1)
                chip8->pc +=2;
        }
        else if((opcode & 0x00FF) == 0xA1){
             uint8_t x= (opcode & 0x0F00) >>8;
            if(chip8->keypad[chip8->V[x]] == 0x0)
                chip8->pc +=2;
        }
        break;
    }
    case 0xF:
    {
        uint8_t check= (opcode & 0x00FF);
        switch(check){
            case 0x07:
                chip8->V[((opcode & 0x0F00)>>8)]= chip8->delayTimer;
                break;
            case 0x15:
                chip8->delayTimer = chip8->V[((opcode & 0x0F00)>>8)];
                break;
            case 0x18:
                chip8->soundTimer = chip8->V[((opcode & 0x0F00)>>8)];
                break;
            case 0x1E:
                chip8->I= chip8->I + chip8->V[((opcode & 0x0F00) >>8)];
                if(chip8->I >= 0x0FFF) chip8->V[0xF]=0x1;
                break;
            case 0x0A:
                {
                bool key=false;
                for(uint8_t i=0x0;i<0x10;i++){
                    if(chip8->keypad[i]==0x1){
                    chip8->V[((opcode & 0x0F00)>>8)]=i;
                    key=true;
                        break;
                    }
                }
                if(!key){
                chip8->pc-=2;}
                break;
                }
            case 0x29:
                {
                uint8_t x= (opcode & 0x0F00) >>8;
                chip8->I = SA + (chip8->V[x] * 5);
                break;
                }
            case 0x33:
                {
                uint8_t value= chip8->V[((opcode & 0x0F00) >> 8)];
                chip8->memory[chip8->I] = value / 100;

                chip8->memory[chip8->I+1] = (value / 10) % 10;

                chip8->memory[chip8->I+2] = value % 10;
                break;
                }
            case 0x55:
                {
                    uint8_t x= ((opcode & 0x0F00)>>8);
                    for(uint8_t i=0;i<=x;i++){
                        chip8->memory[chip8->I+i]=chip8->V[i];
                    }
                    break;
                }
            case 0x65:
                {
                    uint8_t x= ((opcode & 0x0F00)>>8);
                    for(uint8_t i=0;i<=x;i++){
                        chip8->V[i]=chip8->memory[chip8->I+i];
                    }
                    break;
                }
        }
    }
    default: break;
}
}


bool load_rom(Chip8 *chip8,const char* file_name){
    FILE* fp=fopen(file_name,"rb");
    if(fp==NULL){
        fprintf(stderr,"\n Caricamento file non andato a buon fine. \n");
        return false;
    }
    fseek(fp,0,SEEK_END);
    long int dim_file=ftell(fp);
    if(dim_file > 3584){
        fclose(fp);
        fprintf(stderr,"\n Caricamento file non andato a buon fine. \n");
        return false;
    }
    fseek(fp,0,SEEK_SET);
    fread(&chip8->memory[0x200],sizeof(uint8_t),dim_file,fp);
    fclose(fp);
    return true;
}

