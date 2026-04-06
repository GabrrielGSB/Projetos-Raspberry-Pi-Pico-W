#ifndef DISPLAY_SSD1306_H
#define DISPLAY_SSD1306_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "comunicacao.h"
#include "displayLibs/ssd1306_i2c.h"
#include "displayLibs/ssd1306_font.h"

extern uint8_t display[ssd1306_buffer_length];

extern struct render_area frame_area;

const char* numToStr(uint num);

void configurarDisplay();
void atualizarDisplay();
void limparDisplay();
void desenharPixel(uint8_t x, uint8_t y);
void apagarPixel(uint8_t x, uint8_t y);
void desenharLinha(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void apagarLinha(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void desenharRetangulo(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void apagarRetangulo(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void desenharCirculo(uint8_t x0, uint8_t y0, uint8_t radius);
void apagarCirculo(uint8_t x0, uint8_t y0, uint8_t radius);
void desenharSimbolo(const char simbolo, uint8_t x, uint8_t y);
void desenharString(const char* str, uint8_t x, uint8_t y);

#endif