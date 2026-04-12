#ifndef TELAS_DISPLAY_SSD1306_H
#define TELAS_DISPLAY_SSD1306_H

#include <stdint.h>
#include <stdbool.h>
#include "displaySSD1306.h"
#include "interrupcoes.h"
#include "arduinoStyle.h"

extern bool telaBoasVindasAconteceu;
extern bool noWIFI;
extern bool noHTTP;
extern int telaAtual;

void seletorDeTela(bool Luz1Ligada, bool Luz2Ligada, 
                   bool Luz3Ligada, bool Luz4Ligada);

#endif