#ifndef MATRIZ_LED_H
#define MATRIZ_LED_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#define numLEDs 25
#define pinoMatrizLED 7
#define resolucaoIntensidade 25

typedef struct infoMatrizLED{ uint8_t R, G, B; } infoMatrizLED;

typedef struct configMatrizLed{
    volatile uint8_t contador;
    volatile uint8_t intensidade[25];
} configMatrizLed;

extern struct repeating_timer timerMatrizLED;

extern configMatrizLed configMatrizLED;
extern infoMatrizLED leds[numLEDs];
extern PIO np_pio;
extern uint sm;

void configurarMatrizLED();
void definirIntensidadeLED_matriz(const uint8_t indice, 
                                        uint8_t intensidade);
void definirLED_matriz(const uint8_t indice, 
                       const uint8_t r, 
                       const uint8_t g, 
                       const uint8_t b);
void limparMatrizLED();
void preencherMatrizLED();
bool preencherMatrizLED_loop(struct repeating_timer *t);

void controlarRegiao2x2MatrizLED(uint8_t selecaoRegiao, 
                                 bool luz1Ligada, bool luz2Ligada, 
                                 bool luz3Ligada, bool luz4Ligada);

#endif 