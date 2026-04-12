#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "pico/stdlib.h"
#include "interrupcoes.h"
#include "pico/cyw43_arch.h"

#define WIFI_SSID "MIROXO" 
#define WIFI_PASS "miroxo@123"

#define pinSDA 14
#define pinSCL 15   

void configurarI2C();
bool iniciarWiFi();

#endif