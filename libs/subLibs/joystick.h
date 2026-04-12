#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/adc.h"
#include "arduinoStyle.h"
#include "soms.h"

#define bttJoystick 22 
#define DIREITA 2500
#define ESQUERDA 1500
#define CIMA 2500
#define BAIXO 1500

extern size_t timerBotaoJoy;
extern bool acaoJoyRealizada;

typedef struct Joystick{ uint16_t eixoX, eixoY; bool botao; } Joystick;

void configurarJoystick();
void lerJoystick(Joystick *joy);

void logicaMovimentoTelaLampadas(Joystick *joy, int *estadoSelecao);

void logicaSelecaoTelaLampadas(Joystick *joy, int *estadoSelecao,
                              bool *luz1Ligada, bool *luz2Ligada,
                              bool *luz3Ligada, bool *luz4Ligada);

#endif