#ifndef INTERRUCOES_H
#define INTERRUCOES_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "arduinoStyle.h"

#define pinSDA 14
#define pinSCL 15   

extern volatile int telaAtualint;
extern volatile size_t timerBotao;
extern volatile bool botaoAiniciouWIFI;
extern volatile bool botaoBiniciouServerHTTP;

void gpio_callback(uint gpio, uint32_t events);

#endif