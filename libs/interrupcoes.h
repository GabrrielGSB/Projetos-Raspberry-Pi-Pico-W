#ifndef INTERRUCOES_H
#define INTERRUCOES_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "arduinoStyle.h"

extern int sensibilidadeSom;

void gpio_callback(uint gpio, uint32_t events);

#endif