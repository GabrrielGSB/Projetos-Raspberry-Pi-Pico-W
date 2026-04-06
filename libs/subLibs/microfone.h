#ifndef MICROFONE_H
#define MICROFONE_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "arduinoStyle.h"

#define pinMicrofone 28
#define numAmostras 128

extern uint16_t adcBuffer[numAmostras];

void configurarMicrofone();
void lerMicrofone();


#endif