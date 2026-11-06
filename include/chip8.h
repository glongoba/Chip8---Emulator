#ifndef CHIP8_H 
#define CHIP8_H


#include <stdint.h> // serve per avere degli interi a dimensione fissa (int8_t) -> rende il codice portabile,
// la u davanti indica unsigned -> senza segno -> per fare si che vengano rappresentati solo numeri positivi
// quindi vi è un numero maggiore rappresentabile con x bit.
#include <stdbool.h> // serve per utilizzare la variabili booleane

typedef struct{

    uint8_t memory[4096]; // memoria RAM con 4096 byte
    uint8_t V[16]; // numero di registri
    uint16_t pc; // programm counter
    uint16_t I; // registro di indirizzo

    uint16_t Stack[16]; // stack per le subroutine
    uint8_t sp; // stack pointer per puntare alla head dello stack

    uint8_t soundTimer;
    uint8_t delayTimer;

    uint8_t keypad[16]; //stato dei 16 tasti
    uint32_t display[64 * 32]; // buffer dello schermo

    bool shift_flag;
} Chip8;

extern const uint8_t fontset[80]; 
// extern -> viene definito in un altro file
// dim 80 -> sono 16 caratteri da rappresentare e ognuno necessita 5 byte (poichè l'altezza del pixel è di 5)
// grandezza del carattere -> 8 bit di lunghezza e 5 di altezza
// serve perchè il chip8 non ha un testo vero e proprio, i caratteri infatti non sono altro che degli sprite
// che hanno la forma di quel carattere e ci si aspetta che almeno tutti i caratteri esadecimali siano già presenti
// all'avvio dell'emulatore

//FUNZIONI
void chip8_init(Chip8 *);
uint16_t chip8_fetch(Chip8 *);
void chip8_decode_execute(Chip8 *chip8);
bool load_rom(Chip8 *,const char*);

#endif