#ifndef HTTP_H
#define HTTP_H

#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"
#include "comunicacao.h"
#include "matrizLED.h"

extern bool luz1Ligada, luz2Ligada, luz3Ligada, luz4Ligada; 

void iniciarServerHTTP();


#endif