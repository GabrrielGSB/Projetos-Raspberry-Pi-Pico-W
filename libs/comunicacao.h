#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define pinSDA 14
#define pinSCL 15   

void configurarI2C();

#endif