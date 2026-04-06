#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/adc.h"

typedef struct Joystick{ uint16_t eixoX, eixoY; bool botao; } Joystick;

Joystick lerJoystick();

#endif