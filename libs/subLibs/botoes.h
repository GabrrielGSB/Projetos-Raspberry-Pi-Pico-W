#ifndef BOTOES_H
#define BOTOES_H

#include <stdbool.h>
#include "interrupcoes.h"
#include "arduinoStyle.h"

#define botaoB 6
#define botaoA 5

void configurarBotoes();
bool lerBotaoA();
bool lerBotaoB();

#endif