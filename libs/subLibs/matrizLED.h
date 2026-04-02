#ifndef MATRIZ_LED_H
#define MATRIZ_LED_H

#include <stdint.h>

#define numLEDs 25

matrizLEDs leds[numLEDs];

void iniciarMatrizLED(uint pin);
void definirMatrizLED(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b);
void limparMatrizLED();
void escreverMatrizLED();

#endif 