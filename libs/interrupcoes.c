#include "interrupcoes.h"

#define botaoB 6
#define botaoA 5

int sensibilidadeSom = 0;

void gpio_callback(uint gpio, uint32_t events) {
    static uint32_t ultimo_tempo = 0;

    if (millis() - ultimo_tempo < 150) return; 
    ultimo_tempo = millis();

    if (gpio == botaoA) {
        sensibilidadeSom++;
    } 
    else if (gpio == botaoB) {
        sensibilidadeSom--;
    }

    if      (sensibilidadeSom <= 0)  sensibilidadeSom = 0;
    else if (sensibilidadeSom >= 19) sensibilidadeSom = 19;
}